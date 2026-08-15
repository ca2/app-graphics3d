// Created by camilo on 2025-12-23 14:56 <3ThomasBorregaardSørensen!!
#include "platform.h"
#include "command_buffer.h"
#include "physical_device.h"
#include "renderer.h"
#include "texture.h"
#include "vk_init.h"
#include <ktx.h>


CLASS_DECL_GPU_VULKAN VkFormat get_ktx_vk_format(ktxTexture *pktxtexture, int &iChannelCount, int &iBitsPerChannel, int & iSrgb, int & iFloat);


namespace gpu_vulkan
{


   bool texture::KTXLoadFrom_ktxTexture(void *p_ktxTexture, VkQueue copyQueue, VkImageUsageFlags imageUsageFlags,
                                        VkImageLayout imageLayout, bool forceLinear)
   {
      
      auto pktxtexture = (ktxTexture *)p_ktxTexture;

      VkFormat vkformat = get_ktx_vk_format(pktxtexture, m_textureattributes.m_iChannelCount, m_textureattributes.m_iBitsPerChannel, m_textureattributes.m_iSrgb, m_textureattributes.m_iFloat);

      if (vkformat == VK_FORMAT_UNDEFINED)
      {

         VkFormat vkformatRetry =
            get_ktx_vk_format(pktxtexture, m_textureattributes.m_iChannelCount, m_textureattributes.m_iBitsPerChannel,
                              m_textureattributes.m_iSrgb, m_textureattributes.m_iFloat);
         // cannot upload to Vulkan
         throw ::exception(error_failed);
      }

      if (pktxtexture->isCubemap)
      {

         m_textureattributes.m_etexture = ::gpu::e_texture_cube_map;
         m_textureattributes.m_iLayerCount = 6;

      }
      else
      {

         m_textureattributes.m_etexture = ::gpu::e_texture_image;
         m_textureattributes.m_iLayerCount = 1;

      }

      ::cast<::gpu_vulkan::context> pcontext = m_pgpucontext;
      ::cast<::gpu_vulkan::device> pdevice = pcontext->m_pgpudevice;

      auto pphysicaldevice = pdevice->m_pphysicaldevice;

      // this->m_pDevice = device;
      //m_textureattributes.m_rectangleTarget.left = 0;
      //m_textureattributes.m_rectangleTarget.top = 0;
      // this->m_pDevice = pdevice;
      m_textureattributes.m_size.cx = pktxtexture->baseWidth;
      m_textureattributes.m_size.cy = pktxtexture->baseHeight;
      m_textureattributes.m_iMipCount = pktxtexture->numLevels;

      ktx_uint8_t *ktxTextureData = ktxTexture_GetData(pktxtexture);
      ktx_size_t ktxTextureSize = ktxTexture_GetDataSize(pktxtexture);

      VkFormatProperties formatProperties;
      vkGetPhysicalDeviceFormatProperties(pphysicaldevice->m_vkphysicaldevice, vkformat, &formatProperties);

      VkBool32 useStaging = !forceLinear;

      VkMemoryAllocateInfo memAllocInfo = vkinit::memoryAllocateInfo();
      VkMemoryRequirements memReqs;

      // VkCommandBuffer pcommandbufferCmd->m_vkcommandbuffer =
      // device->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

      auto pgpucommandbufferCmd = pcontext->beginSingleTimeCommands(pcontext->m_pgpudevice->transfer_queue());

      ::cast<::gpu_vulkan::command_buffer> pcommandbufferCmd = pgpucommandbufferCmd;

      if (useStaging)
      {
         VkBuffer stagingBuffer;
         VkDeviceMemory stagingMemory;

         VkBufferCreateInfo bufferCreateInfo = vkinit::bufferCreateInfo();
         bufferCreateInfo.size = ktxTextureSize;
         // This buffer is used as a transfer source for the buffer copy
         bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
         bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

         VkCheckResult(vkCreateBuffer(pcontext->logicalDevice(), &bufferCreateInfo, nullptr, &stagingBuffer));

         // Get memory requirements for the staging buffer (alignment, memory type bits)
         vkGetBufferMemoryRequirements(pcontext->logicalDevice(), stagingBuffer, &memReqs);

         memAllocInfo.allocationSize = memReqs.size;
         // Get memory type index for a host visible buffer
         memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(
            memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

         VkCheckResult(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &stagingMemory));
         VkCheckResult(vkBindBufferMemory(pcontext->logicalDevice(), stagingBuffer, stagingMemory, 0));

         // Copy texture data into staging buffer
         uint8_t *data;
         VkCheckResult(vkMapMemory(pcontext->logicalDevice(), stagingMemory, 0, memReqs.size, 0, (void **)&data));
         memcpy(data, ktxTextureData, ktxTextureSize);
         vkUnmapMemory(pcontext->logicalDevice(), stagingMemory);

         // Setup buffer copy regions for each mip level
         ::array_base<VkBufferImageCopy> bufferCopyRegions;

         for (uint32_t i = 0; i < m_textureattributes.m_iMipCount; i++)
         {
            ktx_size_t offset;
            KTX_error_code result = ktxTexture_GetImageOffset(pktxtexture, i, 0, 0, &offset);
            ASSERT(result == KTX_SUCCESS);

            VkBufferImageCopy bufferCopyRegion = {};
            bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            bufferCopyRegion.imageSubresource.mipLevel = i;
            bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
            bufferCopyRegion.imageSubresource.layerCount = 1;
            bufferCopyRegion.imageExtent.width = std::max(1u, pktxtexture->baseWidth >> i);
            bufferCopyRegion.imageExtent.height = std::max(1u, pktxtexture->baseHeight >> i);
            bufferCopyRegion.imageExtent.depth = 1;
            bufferCopyRegion.bufferOffset = offset;


            bufferCopyRegions.add(bufferCopyRegion);
         }

         // Create optimal tiled target image
         VkImageCreateInfo imageCreateInfo = vkinit::imageCreateInfo();
         imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
         imageCreateInfo.format = vkformat;
         imageCreateInfo.mipLevels = m_textureattributes.m_iMipCount;
         imageCreateInfo.arrayLayers = m_textureattributes.m_iLayerCount;
         imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
         imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
         imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
         imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
         imageCreateInfo.extent = {(uint32_t)m_textureattributes.m_size.width(),
                                   (uint32_t)m_textureattributes.m_size.height(), 1};
         imageCreateInfo.usage = imageUsageFlags;
         // Ensure that the TRANSFER_DST bit is set for staging
         if (!(imageCreateInfo.usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT))
         {
            imageCreateInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
         }
         if (pktxtexture->isCubemap)
         {

            imageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
         }
         VkCheckResult(vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &m_vkimage));

         vkGetImageMemoryRequirements(pcontext->logicalDevice(), m_vkimage, &memReqs);

         memAllocInfo.allocationSize = memReqs.size;

         memAllocInfo.memoryTypeIndex =
            pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
         VkCheckResult(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &m_vkdevicememory));
         VkCheckResult(vkBindImageMemory(pcontext->logicalDevice(), m_vkimage, m_vkdevicememory, 0));

         VkImageSubresourceRange subresourceRange = {};
         subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         subresourceRange.baseMipLevel = 0;
         subresourceRange.levelCount = m_textureattributes.m_iMipCount;
         subresourceRange.layerCount = 1;


         // Image barrier for optimal image (target)
         // Optimal image will be used as destination for the copy
         vulkan::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, m_vkimage, VK_IMAGE_LAYOUT_UNDEFINED,
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourceRange);

         // Copy mip levels from staging buffer
         vkCmdCopyBufferToImage(pcommandbufferCmd->m_vkcommandbuffer, stagingBuffer, m_vkimage,
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, static_cast<uint32_t>(bufferCopyRegions.size()),
                                bufferCopyRegions.data());

         // Change texture image layout to shader read after all mip levels have been copied
         // this->m_state.m_vkimagelayout = imageLayout;
         //::vulkan::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, m_vkimage,
         //: VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, imageLayout, subresourceRange);
         set_state(pcommandbufferCmd, ::gpu::e_texture_state_shader_read);

         // device->flushCommandBuffer(pcommandbufferCmd->m_vkcommandbuffer, copyQueue);

         pcontext->endSingleTimeCommands(pcommandbufferCmd);

         // Clean up staging resources
         vkDestroyBuffer(pcontext->logicalDevice(), stagingBuffer, nullptr);
         vkFreeMemory(pcontext->logicalDevice(), stagingMemory, nullptr);
      }
      else
      {
         // Prefer using optimal tiling, as linear tiling
         // may support only a small set of features
         // depending on implementation (e.g. no mip maps, only one layer, etc.)

         // Check if this support is supported for linear tiling
         ASSERT(formatProperties.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);

         VkImage mappableImage;
         VkDeviceMemory mappableMemory;

         VkImageCreateInfo imageCreateInfo = vkinit::imageCreateInfo();
         imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
         imageCreateInfo.format = vkformat;
         imageCreateInfo.extent = {(uint32_t)m_textureattributes.m_size.width(),
                                   (uint32_t)m_textureattributes.m_size.height(), 1};
         imageCreateInfo.mipLevels = 1;
         imageCreateInfo.arrayLayers = 1;
         imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
         imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
         imageCreateInfo.usage = imageUsageFlags;
         imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
         imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

         // Load mip map level 0 to linear tiling image
         VkCheckResult(vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &mappableImage));

         // Get memory requirements for this image
         // like size and alignment
         vkGetImageMemoryRequirements(pcontext->logicalDevice(), mappableImage, &memReqs);
         // Set memory allocation size to required memory size
         memAllocInfo.allocationSize = memReqs.size;

         // Get memory type that can be mapped to host memory
         memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(
            memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

         // Allocate host memory
         VkCheckResult(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &mappableMemory));

         // Bind allocated image for use
         VkCheckResult(vkBindImageMemory(pcontext->logicalDevice(), mappableImage, mappableMemory, 0));

         // Get sub resource layout
         // Mip map count, array layer, etc.
         VkImageSubresource subRes = {};
         subRes.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         subRes.mipLevel = 0;

         VkSubresourceLayout subResLayout;
         void *data;

         // Get sub resources layout
         // Includes row pitch, size offsets, etc.
         vkGetImageSubresourceLayout(pcontext->logicalDevice(), mappableImage, &subRes, &subResLayout);

         // Map image memory
         VkCheckResult(vkMapMemory(pcontext->logicalDevice(), mappableMemory, 0, memReqs.size, 0, &data));

         // Copy image data into memory
         memcpy(data, ktxTextureData, memReqs.size);

         vkUnmapMemory(pcontext->logicalDevice(), mappableMemory);

         // Linear tiled images don't need to be staged
         // and can be directly used as textures
         m_vkimage = mappableImage;
         m_vkdevicememory = mappableMemory;
         // m_state.m_vkimagelayout = imageLayout;

         _set_state(pcommandbufferCmd, {0, imageLayout, 0});

         // Setup image memory barrier
         // vulkan::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, m_vkimage, VK_IMAGE_ASPECT_COLOR_BIT,
         // VK_IMAGE_LAYOUT_UNDEFINED, imageLayout);

         pcontext->endSingleTimeCommands(pcommandbufferCmd);
      }
      //ktxTexture_Destroy(pktxtexture);
      // Create sampler with anisotropic filtering
      VkSamplerCreateInfo samplerCreateInfo{};
      samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
      samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
      samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
      samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
      samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
      samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
      samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
      samplerCreateInfo.minLod = 0.0f;
      samplerCreateInfo.maxLod = (float)(m_textureattributes.m_iMipCount);
      samplerCreateInfo.mipLodBias = 0.0f;
      samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;

      // Enable anisotropy if supported
      samplerCreateInfo.anisotropyEnable =
         pphysicaldevice->m_vkphysicaldevicefeatures.samplerAnisotropy ? VK_TRUE : VK_FALSE;
      samplerCreateInfo.maxAnisotropy = samplerCreateInfo.anisotropyEnable
                                           ? pphysicaldevice->m_vkphysicaldeviceproperties.limits.maxSamplerAnisotropy
                                           : 1.0f;

      samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

      VkCheckResult(vkCreateSampler(pcontext->logicalDevice(), &samplerCreateInfo, nullptr, &m_vksamplerDedicated));

      // Create image view
      VkImageViewCreateInfo viewCreateInfo{};
      viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      viewCreateInfo.format = vkformat;
      viewCreateInfo.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, (uint32_t)m_textureattributes.m_iMipCount, 0, 1};
      viewCreateInfo.image = m_vkimage;
      VkCheckResult(vkCreateImageView(pcontext->logicalDevice(), &viewCreateInfo, nullptr, &m_vkimageview));

      // Update descriptor for shader sampling
      /// UpdateDescriptor();

      return true;
   }


} // namespace gpu_vulkan
 



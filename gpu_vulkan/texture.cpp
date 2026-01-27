// Created by camilo on 2025-06-08 18:14 < 3ThomasBorregaardSørensen!!
#include "framework.h"
#include "binding.h"
#include "texture.h"
#include "buffer.h"
#include "command_buffer.h"
#include "context.h"
#include "descriptors.h"
#include "device.h"
#include "framework.h"
#include "initializers.h"
#include "physical_device.h"
#include "queue.h"
#include "render_target.h"
#include "renderer.h"
#include "shader.h"
#include "acme/filesystem/file/exception.h"
#include "acme/filesystem/filesystem/file_context.h"
#include "aura/graphics/image/context.h"
#include "aura/graphics/image/image.h"
#include "acme/graphics/image/pixmap.h"
#include "bred/gpu/context_lock.h"
#include "vk_init.h"
#include "gpu/_ktx.h"
#include "gpu_vulkan/offscreen_render_pass.h"
//#include <ktxvulkan.h>
//#include <tiny_gltf.h>
//#include <ktx.h>
//#include <ktxvulkan.h>
#include <math.h>
#include <stb/stb_image.h>

CLASS_DECL_GPU_VULKAN VkFormat get_ktx_vk_format(ktxTexture *pktxtexture, int & iChannelCount, int &iBitsPerChannel);


namespace gpu_vulkan
{


   texture_synchronization::texture_synchronization() {}


   texture_synchronization::~texture_synchronization() {}


   ::gpu_vulkan::render_pass *texture::get_render_pass()
   {

      if (!m_prenderpass)
      {

         update_render_pass();
      }

      return m_prenderpass;
   }


   //::gpu_vulkan::render_pass *texture::get_face_render_pass() 
   //{

   //   if (!m_prenderpassFace)
   //   {

   //      update_face_render_pass();
   //   }

   //   return m_prenderpassFace;
   //}


   
   void texture::update_render_pass()
   {

      if (!m_prenderpass)
      {
         m_prenderpass = øallocate offscreen_render_pass();
      }

      m_prenderpass->m_bWithDepth = m_textureflags.m_bWithDepth;

      m_prenderpass->m_bSrgb = m_textureattributes.m_iFloat >= 1;

      m_prenderpass->initialize(this);

      m_prenderpass->_update_render_pass(m_pgpurenderer->m_pgpucontext, this, m_prenderpass->m_prenderpassOld);

      // prenderpass->on_init_render_pass();

      //return prenderpass;

   }


   //void texture::update_face_render_pass()
   //{

   //   if (!m_prenderpassFace)
   //   {

   //      m_prenderpassFace = øallocate offscreen_render_pass();

   //   }

   //   m_prenderpassFace->m_bWithDepth = m_textureflags.m_bWithDepth;

   //   m_prenderpassFace->m_bSrgb = m_textureattributes.m_iFloat >= 1;

   //   m_prenderpassFace->initialize(this);

   //   m_prenderpassFace->_update_face_render_pass(m_pgpurenderer->m_pgpucontext, this, m_prenderpass->m_prenderpassOld);

   //}


   //VkFramebuffer texture::framebuffer2(::gpu_vulkan::render_pass *prenderpass, int iMip)
   //{

   //   auto &renderpass = m_mapRenderPass[prenderpass];

   //   if (!renderpass.m_vkframebuffer)
   //   {

   //      renderpass.m_vkframebuffer = _framebuffer2(prenderpass, iMip);

   //   }

   //   return renderpass.m_vkframebuffer;

   //}


   //   VkFramebuffer texture_synchronization::_get_frame_buffer(::gpu_vulkan::render_pass * prenderpass)
   //   {

   //   VkImageView imageView = m_ptexture->get_image_view();

   //   VkImageView attachments[2];

   //   attachments[0] = imageView;

   //   int iAttachmentCount;

   //   if (prenderpass->m_textureflags.m_bWithDepth)
   //   {

   //      VkImageView depthImageView = m_ptexture->get_depth_image_view();

   //      attachments[1] = depthImageView;

   //      iAttachmentCount = 2;

   //   }
   //   else
   //   {

   //      iAttachmentCount = 1;

   //   }

   //   VkExtent2D extent = m_prenderpass->getExtent();
   //   VkFramebufferCreateInfo framebufferInfo = {};
   //   framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
   //   framebufferInfo.renderPass = m_prenderpass->m_vkrenderpass;
   //   framebufferInfo.attachmentCount = iAttachmentCount;
   //   framebufferInfo.pAttachments = attachments;
   //   framebufferInfo.width = extent.width;
   //   framebufferInfo.height = extent.height;
   //   framebufferInfo.layers = 1;

   //   auto& vkframebuffer = m_vkframebuffer;

   //   ::cast < ::gpu_vulkan::context > pcontext = m_prenderpass->m_pgpucontext;

   //   if (vkCreateFramebuffer(
   //      pcontext->logicalDevice(),
   //      &framebufferInfo,
   //      nullptr,
   //      &vkframebuffer) != VK_SUCCESS)
   //   {

   //      throw ::exception(error_failed, "failed to create framebuffer!");

   //   }

   //   debug() << "created framebuffer " << vkframebuffer << "with image view " << imageView;

   //   return vkframebuffer;

   //}


   bool texture::_is_ok() const
   {

      return m_vkimage != VK_NULL_HANDLE;

   }


   VkFence texture_synchronization::in_flight_fence()
   {

      if (!m_vkfenceInFlight2)
      {

         ::cast<::gpu_vulkan::context> pcontext = m_ptexture->m_pgpurenderer->m_pgpucontext;

         VkFenceCreateInfo fenceInfo = {};
         fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
         fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

         if (vkCreateFence(pcontext->logicalDevice(), &fenceInfo, nullptr, &m_vkfenceInFlight2) != VK_SUCCESS)
         {

            throw ::exception(error_failed, "failed to create fence!");
         }
      }

      return m_vkfenceInFlight2;
   }


   texture_synchronization *texture::synchronization()
   {

      //::cast<::gpu_vulkan::render_target> prendertarget = pgpurendertarget;

      auto &psynchronization = this->m_ptexturesynchronization;

      // auto& synchronization = this->m_mapSynchronization[prendertarget];

      if (!psynchronization)
      {

         øconstruct_new(psynchronization);

         ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;

         // synchronization.m_prendertarget = nullptr;

         psynchronization->m_ptexture = this;

         VkSemaphoreCreateInfo semaphoreInfo = {};
         semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

         psynchronization->m_iImageAvailable = 0;

         if (vkCreateSemaphore(pcontext->logicalDevice(), &semaphoreInfo, nullptr,
                               &psynchronization->m_vksemaphoreAvailable) != VK_SUCCESS

             ||

             vkCreateSemaphore(pcontext->logicalDevice(), &semaphoreInfo, nullptr,
                               &psynchronization->m_vksemaphoreRenderFinished) != VK_SUCCESS)

         {

            throw ::exception(error_failed, "failed to create synchronization objects for a frame!");
         }
      }

      return psynchronization;
   }


   texture::texture()
   {

      m_vksamplerDedicated = VK_NULL_HANDLE;
      m_bOwnImage = false;

      m_vkformat = VK_FORMAT_UNDEFINED;

      //m_state.m_vkimagelayout = VK_IMAGE_LAYOUT_UNDEFINED;
      //m_state.m_vkaccessflags = 0;
      //m_state.m_vkpipelinestageflags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

      m_textureflags.m_bTransferTarget = true;
      new_texture.set_new_texture();

      m_vkimage = nullptr;
      m_vkimageview = nullptr;
      m_vkdevicememory = nullptr;
      //m_bCpuRead = false;
   }


   texture::~texture()
   {
   
      m_vkimage = nullptr;
   
   }

   
   class texture::layer &texture::current_layer(::gpu_vulkan::render_pass *prenderpass)
   {

      //ASSERT(m_textureattributes.m_etexture == ::gpu::e_texture_cube_map);

      auto &layer = m_mapRenderPassMipLayer.layer(this, prenderpass);

      return layer;

   }


   void texture::_set_image_data(const void *p, int w, int h, int channel_count, int bit_count_per_channel, bool bFloat)
   {

      information("texture::_set_image_data");

      ::gpu::context_lock contextlock(m_pgpurenderer->m_pgpucontext);

      auto blockData = p;

      auto blockSize = w * h * channel_count * bit_count_per_channel / 8;

      int width = w;

      int height = h;

      int channels = channel_count;

      //// stbi_set_flip_vertically_on_load(true);

      //auto imagedata = stbi_loadf_from_memory(blockData, blockSize, &width, &height, &channels, 0);

      //if (!imagedata)
      //{

      //   warning() << "Failed to load texture data";

      //   stbi_image_free(imagedata);

      //   return;
      //}

      //m_textureattributes.m_rectangleTarget = ::int_rectangle(::int_size(width, height));

      m_textureflags.m_bWithDepth = false;

      ::cast<::gpu_vulkan::device> pdevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;

      ::cast<::gpu_vulkan::physical_device> pphysicaldevice = pdevice->m_pphysicaldevice;

      m_textureattributes.m_iMipCount = 1;

      m_textureattributes.m_etexture = ::gpu::e_texture_image;
      // xxx
      // xxx//xxx
      // //xxx
      // //xxx
      // //xxx
      //xxx      m_vkformat = pphysicaldevice->findSupportedFormat({VK_FORMAT_R32G32B32A32_SFLOAT}, VK_IMAGE_TILING_OPTIMAL,
      //xxx VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT); xxx
      //xxx 
      // xxx
      // xxx
      // xxx

      ::memory memoryData;

      const void *dataSource = blockData;

      int iBytesPerChannel = -1;

      if ((channel_count == 3
         || channel_count == 4) && bit_count_per_channel == 32 && bFloat)
      {
         
          auto vkformatGuess1 = pphysicaldevice->findSupportedFormat(
            {VK_FORMAT_R32G32B32A32_SFLOAT},
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);

                   auto vkformatGuess2 = pphysicaldevice->findSupportedFormat({m_vkformat}, VK_IMAGE_TILING_OPTIMAL,
                                                                     VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);


          ASSERT(vkformatGuess1 == vkformatGuess2);

         iBytesPerChannel = 4;

         float *rgbaData = nullptr;

         if (channels == 3)
         {

            auto imagedata = (const float *)dataSource;

            size_t pixelCount = (size_t)width * height;

            memoryData.set_size(pixelCount * iBytesPerChannel * sizeof(float));

            rgbaData = (float *)memoryData.data();

            for (size_t i = 0; i < pixelCount; ++i)
            {

               rgbaData[i * 4 + 0] = imagedata[i * 3 + 0];
               rgbaData[i * 4 + 1] = imagedata[i * 3 + 1];
               rgbaData[i * 4 + 2] = imagedata[i * 3 + 2];
               rgbaData[i * 4 + 3] = 1.0f; // synthesized alpha
            }

            channels = 4;

            dataSource = rgbaData;
         }

      }
      else if (channel_count == 4 && bit_count_per_channel == 8 && !bFloat)
      {
         auto vkformatGuess1 = pphysicaldevice->findSupportedFormat(
            {pcontext->m_formatImageDefault},
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);

         auto vkformatGuess2 = pphysicaldevice->findSupportedFormat({m_vkformat}, VK_IMAGE_TILING_OPTIMAL,
                                                                    VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);

         ASSERT(vkformatGuess1 == vkformatGuess2);

         iBytesPerChannel = 1;

      }
      else
      {

         throw ::exception(error_wrong_state, "Not currently supported format");

      }




      const VkDeviceSize imageSize = VkDeviceSize(width) * height * channels * iBytesPerChannel;

      // Create image
      auto imageCreateInfo = ::vulkan::initializers::imageCreateInfo();
      imageCreateInfo.pNext = nullptr;
      imageCreateInfo.flags = 0;
      imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
      imageCreateInfo.format = m_vkformat;
      imageCreateInfo.extent = {(unsigned int)width, (unsigned int)height, 1};
      imageCreateInfo.mipLevels = 1;
      imageCreateInfo.arrayLayers = 1;
      imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
      // imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
      // imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
      imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
      imageCreateInfo.usage =
         // VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
         VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
      // imageCreateInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
      // imageCreateInfo.queueFamilyIndexCount = 2;
      // uint32_t concurrentQueueFamilyIndices[] = {pdevice->m_queuefamilyindexes.graphicsFamily,
      //                                          pdevice->m_queuefamilyindexes.transferFamily};
      // imageCreateInfo.pQueueFamilyIndices = concurrentQueueFamilyIndices;
      imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
      imageCreateInfo.queueFamilyIndexCount = 0;
      imageCreateInfo.pQueueFamilyIndices = nullptr;
      imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

      auto &state = mip_layer_state(0, 0);
      state.m_vkimagelayout = imageCreateInfo.initialLayout;
      state.m_vkpipelinestageflags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      state.m_vkaccessflags = 0;

      auto logicalDevice = pcontext->logicalDevice();

      VkCheckResult(vkCreateImage(logicalDevice, &imageCreateInfo, nullptr, &m_vkimage));

      VkMemoryRequirements memoryRequirements;
      vkGetImageMemoryRequirements(pcontext->logicalDevice(), m_vkimage, &memoryRequirements);

      VkMemoryAllocateInfo memoryAllocateInfo1 = {};
      memoryAllocateInfo1.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      memoryAllocateInfo1.allocationSize = memoryRequirements.size;
      memoryAllocateInfo1.memoryTypeIndex =
         pphysicaldevice->findMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
      VkCheckResult(vkAllocateMemory(pcontext->logicalDevice(), &memoryAllocateInfo1, nullptr, &m_vkdevicememory));

      VkCheckResult(vkBindImageMemory(pcontext->logicalDevice(), m_vkimage, m_vkdevicememory, 0));

      // Create staging buffer
      VkBuffer stagingBuffer;
      VkDeviceMemory stagingMemory;

      information() << "imageSize = " << imageSize;
      information() << "memoryRequirements.size = " << memoryRequirements.size;

      auto bufferCreateInfo = ::vulkan::initializers::bufferCreateInfo();
      bufferCreateInfo.pNext = nullptr;
      bufferCreateInfo.flags = 0;
      bufferCreateInfo.size = imageSize;
      bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
      // bufferCreateInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
      // bufferCreateInfo.queueFamilyIndexCount = 2;
      // bufferCreateInfo.pQueueFamilyIndices = concurrentQueueFamilyIndices;
      bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
      bufferCreateInfo.queueFamilyIndexCount = 0;
      bufferCreateInfo.pQueueFamilyIndices = nullptr;
      VkCheckResult(vkCreateBuffer(pcontext->logicalDevice(), &bufferCreateInfo, nullptr, &stagingBuffer));

      vkGetBufferMemoryRequirements(pcontext->logicalDevice(), stagingBuffer, &memoryRequirements);

      VkMemoryAllocateInfo memoryAllocateInfo2 = {};
      memoryAllocateInfo2.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      memoryAllocateInfo2.allocationSize = memoryRequirements.size;
      memoryAllocateInfo2.memoryTypeIndex = pphysicaldevice->findMemoryType(
         memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
      VkCheckResult(vkAllocateMemory(pcontext->logicalDevice(), &memoryAllocateInfo2, nullptr, &stagingMemory));

      VkCheckResult(vkBindBufferMemory(pcontext->logicalDevice(), stagingBuffer, stagingMemory, 0));

      unsigned char *data = nullptr;

      VkCheckResult(vkMapMemory(pcontext->logicalDevice(), stagingMemory, 0, imageSize, 0, (void **)&data));

      memcpy(data, dataSource, imageSize);

      vkUnmapMemory(pcontext->logicalDevice(), stagingMemory);

      ::pointer<::gpu_vulkan::command_buffer> pcommandbuffer =
         pcontext->beginSingleTimeCommands(pcontext->m_pgpudevice->transfer_queue(), ::gpu::e_command_buffer_transfer);

      auto vkcommandpoolTransfer = pcontext->getTransferCommandPool();

      ASSERT(pcommandbuffer != nullptr);
      ASSERT(pcommandbuffer->m_vkcommandbuffer != VK_NULL_HANDLE);
      ASSERT(pcommandbuffer->m_vkcommandpool == vkcommandpoolTransfer);
      ASSERT(pcommandbuffer->m_estate == ::gpu::command_buffer::e_state_recording);
      ASSERT(pcommandbuffer->m_vkcommandbufferlevel == VK_COMMAND_BUFFER_LEVEL_PRIMARY);

      _set_state(pcommandbuffer,
                 {VK_ACCESS_TRANSFER_WRITE_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_PIPELINE_STAGE_TRANSFER_BIT});

      // Copy buffer -> image
      VkBufferImageCopy region{};
      region.bufferOffset = 0;
      region.bufferRowLength = 0; // tightly packed
      region.bufferImageHeight = 0;
      region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      region.imageSubresource.mipLevel = 0;
      region.imageSubresource.baseArrayLayer = 0;
      region.imageSubresource.layerCount = 1;
      region.imageOffset = {0, 0, 0};
      region.imageExtent = {(uint32_t)width, (uint32_t)height, 1};

      vkCmdCopyBufferToImage(pcommandbuffer->m_vkcommandbuffer, stagingBuffer, m_vkimage,
                             VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

      _set_state(pcommandbuffer, {VK_ACCESS_SHADER_READ_BIT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                  VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT});


      pcontext->endSingleTimeCommands(pcommandbuffer);

      ////vkFreeMemory(pcontext->logicalDevice(), stagingMemory, nullptr);
      // m_descriptor3.imageView = m_vkimageview;
      // m_descriptor3.sampler = m_vksampler3;
      // m_descriptor3.imageLayout = m_state.m_vkimagelayout;


      // Create image view
      VkImageViewCreateInfo imageViewCreateInfo = {};
      imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      imageViewCreateInfo.pNext = nullptr;
      imageViewCreateInfo.flags = 0;
      imageViewCreateInfo.image = m_vkimage;
      imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      imageViewCreateInfo.format = m_vkformat;
      imageViewCreateInfo.components = {
         .r = VK_COMPONENT_SWIZZLE_IDENTITY,
         .g = VK_COMPONENT_SWIZZLE_IDENTITY,
         .b = VK_COMPONENT_SWIZZLE_IDENTITY,
         .a = VK_COMPONENT_SWIZZLE_IDENTITY,
      };
      imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
      imageViewCreateInfo.subresourceRange.levelCount = m_textureattributes.m_iMipCount;
      imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
      imageViewCreateInfo.subresourceRange.layerCount = 1;
      VkCheckResult(vkCreateImageView(pcontext->logicalDevice(), &imageViewCreateInfo, nullptr, &m_vkimageview));

      // Create sampler
      VkSamplerCreateInfo samplerCreateInfo = {};
      samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
      samplerCreateInfo.pNext = nullptr;
      samplerCreateInfo.flags = 0;
      samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
      samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
      samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
      samplerCreateInfo.mipLodBias = 0.0f;
      samplerCreateInfo.minLod = 0.0f;
      samplerCreateInfo.maxLod = (float)(m_textureattributes.m_iMipCount);
      samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
      samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
      samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
      samplerCreateInfo.anisotropyEnable =
         pphysicaldevice->m_vkphysicaldevicefeatures.samplerAnisotropy ? VK_TRUE : VK_FALSE;
      samplerCreateInfo.maxAnisotropy = samplerCreateInfo.anisotropyEnable
                                           ? pphysicaldevice->m_vkphysicaldeviceproperties.limits.maxSamplerAnisotropy
                                           : 1.0f;
      samplerCreateInfo.compareEnable = VK_FALSE;
      samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
      samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
      samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
      VkCheckResult(vkCreateSampler(pcontext->logicalDevice(), &samplerCreateInfo, nullptr, &m_vksampler3));

      vkDestroyBuffer(pcontext->logicalDevice(), stagingBuffer, nullptr);
      vkFreeMemory(pcontext->logicalDevice(), stagingMemory, nullptr);


      
   }



   void texture::_set_data(const ::gpu::texture_data &data)
   {

      if (data.is_image_array())
      {
      
         if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
         {
      
            _LoadCubeMap(data.imagea());

         }
         else
         {

            auto pimage = data.imagea().first();

            _set_image_data(pimage->data(), pimage->width(), pimage->height(), 4, 8, false);

         }

      }


   }


   void texture::_create_texture(const ::gpu::texture_data & data)
   {

      if (m_textureattributes.m_etexture & ::gpu::e_texture_depth)
      {

         get_depth_image();

         return;

      }

      ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;

      ::cast<context> pgpucontext = pcontext;

      ::cast<render_pass> prenderpass = m_pgpurenderer->render_target();

      VkImageCreateInfo imagecreateinfo = ::vulkan::initializers::imageCreateInfo();

      imagecreateinfo.imageType = VK_IMAGE_TYPE_2D;

      if (m_textureattributes.m_iFloat >= 1)
      {

         m_vkformat = VK_FORMAT_R32G32B32A32_SFLOAT;

      }
      else
      {

         m_vkformat = pcontext->m_formatImageDefault;

      }

      imagecreateinfo.format = m_vkformat;
      imagecreateinfo.extent.width = this->width();
      imagecreateinfo.extent.height = this->height();
      imagecreateinfo.extent.depth = 1;
      imagecreateinfo.mipLevels = this->mip_count();
      if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
      {
         imagecreateinfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
         imagecreateinfo.arrayLayers = 6;
      }
      else
      {
         imagecreateinfo.arrayLayers = 1;
      }
      imagecreateinfo.samples = VK_SAMPLE_COUNT_1_BIT;

      imagecreateinfo.usage = 0;

      if (m_textureflags.m_bTransferTarget && m_textureflags.m_bCpuRead)
      {

         imagecreateinfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

         imagecreateinfo.tiling = VK_IMAGE_TILING_LINEAR;
      }
      else
      {

         if (m_textureflags.m_bTransferTarget)
         {

            imagecreateinfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
         }

         if (m_textureflags.m_bTransferSource)
         {

            imagecreateinfo.usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
         }

         imagecreateinfo.usage |= VK_IMAGE_USAGE_SAMPLED_BIT;

         if (m_textureflags.m_bRenderTarget)
         {

            imagecreateinfo.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
         }

         imagecreateinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
      }

      //imagecreateinfo.initialLayout = m_state.m_vkimagelayout;
      imagecreateinfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

      VkMemoryPropertyFlags properties;

      if (m_textureflags.m_bCpuRead)
      {

         properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
      }
      else
      {

         properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
      }

      pcontext->createImageWithInfo(imagecreateinfo, properties, m_vkimage, m_vkdevicememory);

      state_t stateInitial;

      stateInitial.m_vkimagelayout = VK_IMAGE_LAYOUT_UNDEFINED;
      stateInitial.m_vkaccessflags = 0;
      stateInitial.m_vkpipelinestageflags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

      set_all_states(stateInitial);

      if ((((::uptr)m_vkimage) & 0xffff) == 0x014d)
      {

         information("((((::uptr)m_vkimage) & 0xffff) == 0x014d)");

      }
      else if ((((::uptr)m_vkimage) & 0xffff) == 0x0063)
      {

         information("((((::uptr)m_vkimage) & 0xffff) == 0x0063)");
      }
      else if ((((::uptr)m_vkimage) & 0xffff) == 0x016a)
      {

         information("((((::uptr)m_vkimage) & 0xffff) == 0x016a)");
      }
      else if ((((::uptr)m_vkimage) & 0xffff) == 0x00a7)
      {

         information("((((::uptr)m_vkimage) & 0xffff) == 0x00a7)");
      }
      else if ((((::uptr)m_vkimage) & 0xffff) == 0x016c)
      {

         information("((((::uptr)m_vkimage) & 0xffff) == 0x016c)");
      }
      else if ((((::uptr)m_vkimage) & 0xffff) == 0x0159)
      {

         information("((((::uptr)m_vkimage) & 0xffff) == 0x0159)");
      }
      
      
      
      _set_data(data);
      //if (data.is_image_array())
      //{
      //
      //   if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
      //   {
      //
      //      _LoadCubeMap(data.imagea());

      //   }

      //}

      if (m_textureflags.m_bWithDepth)
      {

         get_depth_image();

      }

   }


      void texture::set_all_states(const texture::state_t &state)
   {

      for (int iMip = 0; iMip < this->mip_count(); iMip++)
      {

         for (int iLayer = 0; iLayer < this->layer_count(); iLayer++)
         {

            mip_layer_state(iMip, iLayer) = state;
         }
      }
   }




   // void texture::initialize_image_texture(::gpu::renderer *prenderer, const ::int_rectangle &rectangleTarget,
   //                                        bool bWithDepth, const ::pointer_array<::image::image> &imagea,
   //                                        enum_type etype)
   // {
   //
   //    auto currentSize = m_rectangleTarget.size();
   //
   //    if (!m_vkimage || currentSize != rectangleTarget.size() && m_pgpurenderer != prenderer)
   //    {
   //
   //       ::gpu::texture::initialize_image_texture(prenderer, rectangleTarget, bWithDepth, imagea, etype);
   //
   //       create_image();
   //
   //    }
   //
   //    if (imagea.has_element())
   //    {
   //
   //       if (m_etexture == ::gpu::e_texture_cube_map)
   //       {
   //
   //          _LoadCubeMap(imagea);
   //       }
   //    }
   // }


   //void texture::initialize_cubemap_image_texture_with_mipmap(::gpu::renderer *pgpurenderer,
   //                                                           const ::int_rectangle &rectangleTarget, int iMipCount,
   //                                                           bool bRenderTarget, bool bShaderResourceView)
   //{

   //   ::gpu::texture::initialize_cubemap_image_texture_with_mipmap(pgpurenderer, rectangleTarget, iMipCount,
   //                                                                bRenderTarget, bShaderResourceView);

   //   //m_pgpurenderer = pgpurenderer;
   //   //m_rectangleTarget = rectangleTarget;
   //   //m_iMipCount = iMipCount;
   //   //m_bRenderTarget = bRenderTarget;
   //   //m_bShaderResourceView = bShaderResourceView;

   //}



   void texture::_LoadCubeMap(const ::pointer_array<::image::image> &imagea)
   {

      defer_throw_if_cube_map_images_are_not_ok(imagea);

      ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;

      ::cast<context> pgpucontext = pcontext;

      ::cast<device> pdevice = pcontext->m_pgpudevice;

      auto w = imagea.first()->width();

      auto h = imagea.first()->height();

      VkDeviceSize layerSize = w * h * 4;

      VkDeviceSize totalSize = layerSize * 6;

      auto pbufferStaging =
         pgpucontext->create_buffer(totalSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

      pbufferStaging->_assign_cube_map(imagea);

      ::pointer<command_buffer> pcommandbuffer = pcontext->beginSingleTimeCommands(pdevice->m_pqueueTransfer);

      pcontext->copyBufferToImage(pcommandbuffer, this, pbufferStaging);

      _set_state(pcommandbuffer, 
         {

            VK_ACCESS_TRANSFER_READ_BIT, 
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            VK_PIPELINE_STAGE_TRANSFER_BIT

         }
      );

      pcontext->endSingleTimeCommands(pcommandbuffer);
   }


   void texture::initialize_depth_texture(::gpu::renderer *pgpurenderer, const ::int_rectangle &rectangleTarget)
   {

      if (m_textureattributes.m_rectangleTarget == rectangleTarget && m_pgpurenderer == pgpurenderer)
      {

         return;
      }

      auto currentSize = m_textureattributes.m_rectangleTarget.size();

      ::gpu::texture::initialize_depth_texture(pgpurenderer, rectangleTarget);

      if (currentSize == rectangleTarget.size() && m_pgpurenderer == pgpurenderer)
      {

         return;
      }

      ASSERT(m_textureattributes.m_etexture & ::gpu::e_texture_depth);


      get_depth_image();

      ////}
      ////else
      ////{

      //::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

      //::cast < context > pgpucontext = pcontext;

      //::cast < render_pass > prenderpass = m_pgpurenderer->m_pgpurendertarget;

      // VkImageCreateInfo imagecreateinfo = ::vulkan::initializers::imageCreateInfo();

      // imagecreateinfo.imageType = VK_IMAGE_TYPE_2D;
      // imagecreateinfo.format = pcontext->m_formatImageDefault;
      // imagecreateinfo.extent.width = rectangleTarget.width();
      // imagecreateinfo.extent.height = rectangleTarget.height();
      // imagecreateinfo.extent.depth = 1;
      // imagecreateinfo.mipLevels = 1;
      // imagecreateinfo.arrayLayers = 1;
      // imagecreateinfo.samples = VK_SAMPLE_COUNT_1_BIT;

      // imagecreateinfo.usage = 0;

      // if (m_bTransferDst & m_bCpuRead)
      //{

      //   imagecreateinfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

      //   imagecreateinfo.tiling = VK_IMAGE_TILING_LINEAR;

      //}
      // else
      //{

      //   if (m_bTransferDst)
      //   {

      //      imagecreateinfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

      //   }

      //   imagecreateinfo.usage |=
      //      VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
      //      VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
      //      VK_IMAGE_USAGE_SAMPLED_BIT;

      //   imagecreateinfo.tiling = VK_IMAGE_TILING_OPTIMAL;

      //}

      // imagecreateinfo.initialLayout = m_vkimagelayout;

      // VkMemoryPropertyFlags properties;

      // if (m_bCpuRead)
      //{

      //   properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

      //}
      // else
      //{

      //   properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

      //}

      // pcontext->createImageWithInfo(
      //    imagecreateinfo,
      //    properties,
      //    m_vkimage,
      //    m_vkdevicememory);

      // if (m_textureflags.m_bWithDepth)
      //{

      //   get_depth_image();

      //}

      ////}
   }


   void texture::_set_state(::gpu_vulkan::command_buffer *pcommandbuffer, state_t state)
   {

      auto estate = pcommandbuffer->m_estate;

      ASSERT(estate == ::gpu::command_buffer::e_state_recording);

      VkImage image = m_vkimage;

      int iMip;

      int iMipCount;

      int iLayer;

      int iLayerCount;

      if (m_iCurrentMip >= 0)
      {

         iMip = m_iCurrentMip;

         iMipCount = 1;

      }
      else
      {

         iMip = 0;

         iMipCount = this->mip_count();

      }

      if (m_iCurrentLayer >= 0)
      {

         iLayer = m_iCurrentLayer;

         iLayerCount = 1;

      }
      else
      {

         iLayer = 0;

         iLayerCount = this->layer_count();

      }

      VkAccessFlags accessOld = mip_layer_state(iMip, iLayer).m_vkaccessflags;
      VkAccessFlags accessNew = state.m_vkaccessflags;
      VkImageLayout layoutOld = mip_layer_state(iMip, iLayer).m_vkimagelayout;
      VkImageLayout layoutNew = state.m_vkimagelayout;
      VkPipelineStageFlags stageOld = mip_layer_state(iMip, iLayer).m_vkpipelinestageflags;
      VkPipelineStageFlags stageNew = state.m_vkpipelinestageflags;

      // Quick no-op
      if (layoutOld == layoutNew && accessOld == accessNew && stageOld == stageNew)
      {

         return;
      }

      // If old layout is UNDEFINED, the src access must be 0 and src stage can be TOP_OF_PIPE
      if (layoutOld == VK_IMAGE_LAYOUT_UNDEFINED)
      {

         accessOld = 0;

         if (stageOld == 0)
         {

            stageOld = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

         }

      }

      // If no stage bits provided, derive conservative ones from access masks / layouts
      auto derive_stage_from_access = [](VkAccessFlags access) -> VkPipelineStageFlags
      {

         if (access == 0)
         {

            return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

         }

         VkPipelineStageFlags stages = 0;

         if (access & (VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT))
         {

            stages |= VK_PIPELINE_STAGE_TRANSFER_BIT;

         }

         if (access & (VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT))
         {

            stages |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;

         }

         if (access & (VK_ACCESS_HOST_READ_BIT | VK_ACCESS_HOST_WRITE_BIT))
         {

            stages |= VK_PIPELINE_STAGE_HOST_BIT;

         }

         if (access & (VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT))
         {

            stages |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

         }

         if (stages == 0)
         {

            stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

         }

         return stages;

      };

      if (stageOld == 0)
      {

         stageOld = derive_stage_from_access(accessOld);

      }

      if (stageNew == 0)
      {

         stageNew = derive_stage_from_access(accessNew);

      }

      // If new layout is transfer dst and no dst access set, set transfer write
      if (layoutNew == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && (accessNew & VK_ACCESS_TRANSFER_WRITE_BIT) == 0)
      {

         accessNew |= VK_ACCESS_TRANSFER_WRITE_BIT;

         if ((stageNew & VK_PIPELINE_STAGE_TRANSFER_BIT) == 0)
         {

            stageNew |= VK_PIPELINE_STAGE_TRANSFER_BIT;

         }

      }

      // If new layout is shader read only and no shader read access set, set it
      if (layoutNew == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && (accessNew & VK_ACCESS_SHADER_READ_BIT) == 0)
      {

         accessNew |= VK_ACCESS_SHADER_READ_BIT;

         if ((stageNew & (VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_VERTEX_SHADER_BIT |
                          VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT)) == 0)
         {

            stageNew |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

         }

      }

      // Build barrier
      VkImageMemoryBarrier barrier{};
      barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
      barrier.pNext = nullptr;
      barrier.srcAccessMask = accessOld;
      barrier.dstAccessMask = accessNew;
      barrier.oldLayout = layoutOld;
      barrier.newLayout = layoutNew;
      barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barrier.image = image;
      barrier.subresourceRange.baseMipLevel = iMip;
      barrier.subresourceRange.levelCount = iMipCount;
      barrier.subresourceRange.baseArrayLayer = iLayer;
      barrier.subresourceRange.layerCount = iLayerCount;

      // Set aspect mask according to texture type
      if (m_textureattributes.m_etexture == ::gpu::e_texture_depth)
      {

         barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

      }
      else if (m_textureattributes.m_etexture == ::gpu::e_texture_depth_stencil)
      {

         barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;

      }
      else if (m_textureattributes.m_etexture == ::gpu::e_texture_image ||
               m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
      {

         barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

      }
      else
      {

         throw ::exception(error_wrong_state);

      }

      // Finally call barrier
      vkCmdPipelineBarrier(pcommandbuffer->m_vkcommandbuffer, stageOld, stageNew, 0, 0, nullptr, 0, nullptr, 1,
                           &barrier);

      state_t stateNew;

      stateNew.m_vkimagelayout = layoutNew;
      stateNew.m_vkaccessflags = accessNew;
      stateNew.m_vkpipelinestageflags = stageNew;

      if (m_iCurrentMip >= 0)
      {

         if (m_iCurrentLayer >= 0)
         {

            auto & state = mip_layer_state(m_iCurrentMip, m_iCurrentLayer);

            state = stateNew;

         }
         else
         {

            for (int iLayer = 0; iLayer < this->layer_count(); iLayer++)
            {

               auto & state = mip_layer_state(m_iCurrentMip, iLayer);

               state = stateNew;

            }

         }

      }
      else if (m_iCurrentLayer >= 0)
      {

         for (int iMip = 0; iMip < this->mip_count(); iMip++)
         {

            auto & state = mip_layer_state(iMip, m_iCurrentLayer);

            state = stateNew;

         }

      }
      else
      {

         for (int iMip = 0; iMip < this->mip_count(); iMip++)
         {

            for (int iLayer = 0; iLayer < this->layer_count(); iLayer++)
            {

               auto & state = mip_layer_state(iMip, iLayer);

               state = stateNew;

               informationf("\"%s\".state(%d, %d) = %s", m_strTextureName.c_str(), iMip, iLayer, vk_image_layout_text(state.m_vkimagelayout).c_str());

               if (m_iTextureSerial == 12)
               {

                  if (state.m_vkimagelayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
                  {

                     information("vkimagelayout");

                  }

               }

            }

         }

      }

      //// Update stored state
      //m_state.m_vkimagelayout = layoutNew;
      //m_state.m_vkaccessflags = accessNew;
      //m_state.m_vkpipelinestageflags = stageNew;

   }


   VkImage texture::get_depth_image()
   {

      if (m_textureattributes.m_etexture & ::gpu::e_texture_depth)
      {

         if (m_textureflags.m_bCpuRead)
         {

            return VK_NULL_HANDLE;
         }

         if (m_vkimage)
         {

            return m_vkimage;
         }

         ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;

         VkFormat depthFormat = pcontext->findDepthFormat();
         VkImageCreateInfo imageInfo{};

         imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
         imageInfo.imageType = VK_IMAGE_TYPE_2D;
         imageInfo.extent.width = this->width();
         imageInfo.extent.height = this->height();
         imageInfo.extent.depth = 1;
         imageInfo.mipLevels = 1;
         imageInfo.arrayLayers = 1;
         imageInfo.format = m_vkformat = depthFormat;
         imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
         imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
         imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
         imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
         imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
         imageInfo.flags = 0;

         auto &depthImage = m_vkimage;

         auto &depthImageMemory = m_vkdevicememory;

         pcontext->createImageWithInfo(imageInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);

         //::cast < command_buffer > pcommandbuffer = m_pgpurenderer->getCurrentCommandBuffer2(::gpu::current_frame());

         //_new_state(
         //   pcommandbuffer,
         //   VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
         //   VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
         //   VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT);

         return m_vkimage;

      }
      else
      {

         ASSERT(m_textureflags.m_bWithDepth);

         ::cast<texture> ptexture = get_depth_texture();

         return ptexture->m_vkimage;
      }
   }


   // texture* texture::_get_depth_texture()
   //{
   //
   //    if (m_etexture & ::gpu::e_texture_depth)
   //    {

   //      return this;

   //   }

   //   if (m_ptextureDepth)
   //   {

   //      return m_ptextureDepth;

   //   }

   //   ødefer_construct_new(m_ptextureDepth);

   //   m_ptextureDepth->initialize_depth_texture(m_pgpurenderer, m_rectangleTarget);

   //   return m_ptextureDepth;

   //}


   void texture::layer::_create_framebuffer(::gpu_vulkan::texture * ptexture, ::gpu_vulkan::render_pass * prenderpass, int iAttachmentCount)
   {

      ::cast<::gpu_vulkan::context> pcontext = ptexture->m_pgpurenderer->m_pgpucontext;

      VkFramebufferCreateInfo framebufferInfo{};
      framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      framebufferInfo.renderPass = prenderpass->getRenderPass();
      framebufferInfo.attachmentCount = iAttachmentCount;
      framebufferInfo.pAttachments = m_vkimageviewaAttachment;

      if (ptexture->m_iCurrentLayer < 0)
      {

         m_size = ptexture->rectangle().size();
         m_iLayerCount = ptexture->m_textureattributes.m_iLayerCount;

      }
      else
      {

         m_size.cx = ptexture->mip_width();
         m_size.cy = ptexture->mip_height();
         m_iLayerCount = 1;

      }

      framebufferInfo.width = m_size.cx;
      framebufferInfo.height = m_size.cy;
      framebufferInfo.layers = m_iLayerCount;

      VkCheckResult(vkCreateFramebuffer(pcontext->logicalDevice(), &framebufferInfo, nullptr, &m_vkframebuffer));

   }


   void texture::layer::create_framebuffer(::gpu_vulkan::texture *ptexture, ::gpu_vulkan::render_pass *prenderpass)
   {

      int iAttachmentCount = 1;

      if (!m_vkimageviewaAttachment[0])
      {

         create_color_attachment(ptexture);

      }

      if (!m_vkimageviewaAttachment[1] && prenderpass->m_bWithDepth)
      {

         create_depth_attachment(ptexture);

         iAttachmentCount++;

      }

      if (!m_vkframebuffer)
      {

         _create_framebuffer(ptexture, prenderpass, iAttachmentCount);

      }

   }


   void texture::layer::create_color_attachment(::gpu_vulkan::texture *ptexture)
   {

      ::cast<::gpu_vulkan::context> pcontext = ptexture->m_pgpurenderer->m_pgpucontext;

      VkImageViewCreateInfo faceView{};
      faceView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      faceView.image = ptexture->m_vkimage;
      faceView.viewType = VK_IMAGE_VIEW_TYPE_2D;
      faceView.format = ptexture->m_vkformat;
      faceView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      faceView.subresourceRange.baseMipLevel = maximum(0, ptexture->m_iCurrentMip);
      faceView.subresourceRange.levelCount = ptexture->m_iCurrentLayer < 0 ? ptexture->m_textureattributes.m_iMipCount : 1;
      faceView.subresourceRange.baseArrayLayer = maximum(0, ptexture->m_iCurrentLayer); // <--- select the cube face
      faceView.subresourceRange.layerCount = ptexture->m_iCurrentLayer < 0 ? ptexture->m_textureattributes.m_iLayerCount : 1;
      VkCheckResult(vkCreateImageView(pcontext->logicalDevice(), &faceView, nullptr, &m_vkimageviewaAttachment[0]));

   }


   void texture::layer::create_depth_attachment(::gpu_vulkan::texture *ptexture)
   {

      ::cast<::gpu_vulkan::context> pcontext = ptexture->m_pgpurenderer->m_pgpucontext;

      VkFormat depthFormat = pcontext->findDepthFormat();

      VkImageViewCreateInfo faceView{};
      faceView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      faceView.image = ptexture->get_depth_image();
      faceView.viewType = VK_IMAGE_VIEW_TYPE_2D;
      faceView.format = depthFormat;
      faceView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
      faceView.subresourceRange.baseMipLevel = maximum(0, ptexture->m_iCurrentMip);
      faceView.subresourceRange.levelCount = ptexture->m_iCurrentLayer < 0 ? ptexture->m_textureattributes.m_iMipCount : 1;
      faceView.subresourceRange.baseArrayLayer = maximum(0, ptexture->m_iCurrentLayer); // <--- select the cube face
      faceView.subresourceRange.layerCount = ptexture->m_iCurrentLayer < 0 ? ptexture->m_textureattributes.m_iLayerCount : 1;
      VkCheckResult(vkCreateImageView(pcontext->logicalDevice(), &faceView, nullptr, &m_vkimageviewaAttachment[1]));

   }


   texture::layer &texture::render_pass_mip_layer_array::layer(::gpu_vulkan::texture *ptexture,
                                                               ::gpu_vulkan::render_pass *prenderpass)
   {

      auto vkrenderpass = prenderpass->getRenderPass();

      int iMip = maximum(0, ptexture->m_iCurrentMip);

      int iLayerPlusOne = maximum(0, ptexture->m_iCurrentLayer + 1);

      auto &layer = (*this)[vkrenderpass].ø(iMip).ø(iLayerPlusOne);

      if (layer.is_empty())
      {

         layer.create_framebuffer(ptexture, prenderpass);

      }

      return layer;

   }


   //VkImageView texture::cube::get_image_view(::gpu_vulkan::texture * ptexture, int iIndex)
   //{

   //   if (m_iImageViewCount < 6)
   //   {

   //      create_image_views(ptexture);

   //   }

   //   return m_imageviewa[iIndex];

   //}


   //VkFramebuffer texture::cube::layer_framebuffer(::gpu_vulkan::texture *ptexture,
   //                                               ::gpu_vulkan::render_pass *prenderpass,int iLayer, int iMip)
   //{

   //   auto &renderpass = m_mapRenderPass[prenderpass].ø(constrain(iMip, 0, 1024));

   //   if (!renderpass.m_framebuffera[iLayer])
   //   {

   //      renderpass.m_framebuffera[iLayer] = _layer_framebuffer(ptexture, prenderpass, iLayer, iMip);
   //   }

   //   return renderpass.m_framebuffera[iLayer];
   //}


   //VkFramebuffer texture::cube::framebuffer2(::gpu_vulkan::texture * ptexture, ::gpu_vulkan::render_pass *prenderpass, int iMip)
   //{

   //   auto &renderpass = m_mapRenderPass[prenderpass].ø(constrain(iMip, 0, 1024));

   //   if (!renderpass.m_framebuffera[iLayer])
   //   {

   //      renderpass.m_framebuffera[iLayer] = _layer_framebuffer(ptexture, prenderpass, iLayer, iMip);
   //   }

   //   return renderpass.m_framebuffera[iLayer];
   //}


   //VkFramebuffer texture::cube::_framebuffer2(::gpu_vulkan::texture * ptexture, ::gpu_vulkan::render_pass *prenderpass, int iMip)
   //{

   //   auto &framebuffercube = m_mapFramebufferCube[prenderpass->m_vkrenderpass].ø(0);

   //   if (framebuffercube.m_framebuffera[iFace])
   //   {

   //      return framebuffercube.m_framebuffera[iFace];
   //   
   //   
   //   }

   //   ::cast<::gpu_vulkan::context> pcontext = ptexture->m_pgpurenderer->m_pgpucontext;

   //   VkImageView attachments[1];

   //   attachments[0] = get_image_view(ptexture, iFace);

   //   int iAttachmentCount = 1;

   //   // if (prenderpass->m_flags.m_bWithDepth)
   //   //{

   //   //   if (!m_flags.m_bWithDepth)
   //   //   {

   //   //      m_flags.m_bWithDepth = true;
   //   //   }

   //   //   VkImageView depthImageView = get_depth_image_view();

   //   //   attachments[1] = depthImageView;

   //   //   iAttachmentCount = 2;
   //   //}
   //   // else
   //   //{

   //   //   iAttachmentCount = 1;
   //   //}

   //   VkFramebufferCreateInfo framebufferInfo{};
   //   framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
   //   framebufferInfo.renderPass = prenderpass->m_vkrenderpass;
   //   framebufferInfo.attachmentCount = 1;
   //   framebufferInfo.pAttachments = attachments;
   //   framebufferInfo.width = ptexture->rectangle().width();
   //   framebufferInfo.height = ptexture->rectangle().height();
   //   framebufferInfo.layers = 1;

   //   VkCheckResult(vkCreateFramebuffer(pcontext->logicalDevice(), &framebufferInfo, nullptr,
   //                                       &framebuffercube.m_framebuffera[iFace]));

   //   return framebuffercube.m_framebuffera[iFace];
   //}


   //::gpu_vulkan::render_pass * texture::cube::get_render_pass(::gpu_vulkan::texture * ptexture, int iIndex)
   //{

   //   auto & prenderpass = m_renderpassa[iIndex];

   //   if (!prenderpass)
   //   {

   //      øconstruct(prenderpass);

   //      prenderpass->on_init_render_pass

   //   }

   //   return prenderpass;

   //}


   VkImageView texture::get_image_view()
   {

      //if (iIndex >= 0 
      //   && m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
      //{

      //   return cube().get_image_view(this, iIndex);

      //}
      //else
      {

         if (m_textureflags.m_bCpuRead)
         {

            return VK_NULL_HANDLE;
         }

         if (m_vkimageview)
         {

            return m_vkimageview;
         }
         create_image_view();
         return m_vkimageview;
      }
   }


   VkSampler texture::get_vk_sampler()
   {

      if (m_vksamplerDedicated)
      {
         return m_vksamplerDedicated;
      }
      else if (m_vksampler3)
      {
         return m_vksampler3;
      }
      else
      {

         ::cast<context> pcontext = m_pgpurenderer->m_pgpucontext;

         return pcontext->_001VkSampler();
      }

      //::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;

      //return pcontext->_001VkSampler();

   }

   
   //VkFramebuffer texture::framebuffer2(::gpu_vulkan::render_pass * prenderpass, int iMip)
   //{

   //   return mip_layer_array().framebuffer(this , prenderpass, iFace, iMip);

   //}


   //VkFramebuffer texture::_framebuffer(::gpu_vulkan::render_pass * prenderpass, int iFace)
   //{

   //   return cube()._framebuffer(this, prenderpass, iFace, iMip);

   //}


   void texture::create_image_view()
   {

      
      ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;
      VkImageViewType viewType;
      if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
      {
         viewType = VK_IMAGE_VIEW_TYPE_CUBE;
      }
      else
      {
         viewType = VK_IMAGE_VIEW_TYPE_2D;
      }

      unsigned int uLayerCount;

      if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
      {

         uLayerCount = 6;
      }
      else
      {

         uLayerCount = 1;
      }

      VkImageViewCreateInfo viewInfo = {.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                                        .image = m_vkimage,
                                        .viewType = viewType,
                                        .format = m_vkformat,
                                        .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                                             .baseMipLevel = 0,
                                                             .levelCount = 1,
                                                             .baseArrayLayer = 0,
                                                             .layerCount = uLayerCount}};

      VkCheckResult(vkCreateImageView(pcontext->logicalDevice(), &viewInfo, NULL, &m_vkimageview));



   }


   void texture::set_pixels(const ::int_rectangle &rectangle, const void *data)
   {

      VkDeviceSize size = rectangle.area() * 4;

      ::pointer<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;

      auto pbufferStaging =
         pcontext->create_buffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

      pbufferStaging->_assign(data, size);

      if (ødefer_construct_new(m_p_001OnAfterEndFrame))
      {

         m_pgpurenderer->post_on_after_end_frame(
            [this, pcontext]()
            {
               auto p = ::transfer(m_p_001OnAfterEndFrame);

               auto pcommandbuffer = pcontext->beginSingleTimeCommands(pcontext->m_pgpudevice->transfer_queue());

               for (auto &pitem: p->m_itema)
               {

                  pcontext->copyBufferToImage(pcommandbuffer, pitem->m_ptexture, pitem->m_pbufferStaging,
                                              pitem->m_rectangle);
               }

               pcontext->endSingleTimeCommands(pcommandbuffer);
            });

      }

      auto ponafterendframeitem = øcreate_new<_001OnAfterEndFrameItem>();
      ponafterendframeitem->m_ptexture=this;
      ponafterendframeitem->m_pcontext = pcontext;
      ponafterendframeitem->m_pbufferStaging = pbufferStaging;
      ponafterendframeitem->m_rectangle = rectangle;
      m_p_001OnAfterEndFrame->m_itema.add(ponafterendframeitem);

      //m_pgpurenderer->post_on_after_end_frame(
      //   [this, pcontext, pbufferStaging, rectangle]()
      //   {

      //      auto pcommandbuffer = pcontext->beginSingleTimeCommands(pcontext->m_pgpudevice->transfer_queue());

      //      pcontext->copyBufferToImage(pcommandbuffer, this, pbufferStaging, rectangle);

      //      pcontext->endSingleTimeCommands(pcommandbuffer);

      //   });

      if (ødefer_construct_new(m_p_001OnNextFrameStart))
      {
         m_pgpurenderer->post_on_just_before_frame_next_start(
            [this, pcontext]()
            {

               auto p = ::transfer(m_p_001OnNextFrameStart);
               auto pgpucommandbuffer = pcontext->beginSingleTimeCommands(pcontext->m_pgpudevice->transfer_queue());

               ::cast<::gpu_vulkan::command_buffer> pcommandbuffer = pgpucommandbuffer;

               for (auto &ptexture: p->m_texturea)
               {

                  ptexture->_set_state(pcommandbuffer,
                                     {

                                                VK_ACCESS_TRANSFER_READ_BIT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                                VK_PIPELINE_STAGE_TRANSFER_BIT

                                             });
               }


               pcontext->endSingleTimeCommands(pcommandbuffer);
            });
      }

      m_p_001OnNextFrameStart->m_texturea.add_unique(this);

   }


   VkDescriptorImageInfo texture::descriptor_info()
   {


      VkDescriptorImageInfo imageinfo;

      imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      imageinfo.imageView = this->get_image_view();
      imageinfo.sampler = this->get_vk_sampler();

      return imageinfo;

   }


   VkDescriptorSet texture::_001DescriptorSet(::gpu_vulkan::shader *pshader, ::gpu::command_buffer *pgpucommandbuffer)
   {

      auto iFrame = pgpucommandbuffer->m_iCommandBufferFrameIndex;

      auto iFrameCount = pgpucommandbuffer->m_pgpurendertarget->get_frame_count();

      auto iSerial = pgpucommandbuffer->m_iSerial;

      if (iSerial == 74)
      {

         information("iSerial == 74");

      }

      if (iFrame < 0)
      {

         throw ::exception(error_wrong_state);

      }

      auto &pdescriptorseta = m_mapShaderDescriptorSetArray[pshader];

      if (pdescriptorseta && pdescriptorseta->size() >= iFrameCount)
      {

         ASSERT(pdescriptorseta->size() >= iFrameCount && pdescriptorseta->element_at(iFrame));

         //if ((((::uptr)shader.m_pvkdescriptorseta[iFrame]) & 0xffff) == 0x287)
         //{

         //   ::information("(shader.m_vkdescriptorseta[iFrame] & 0xffff) == 0x287");
         //}
         //else if ((((::uptr)shader.m_vkdescriptorseta[iFrame]) & 0xffff) == 0x1e5)
         //{

         //   ::information("(shader.m_vkdescriptorseta[iFrame] & 0xffff) == 0x1e5");
         //}

         return pdescriptorseta->element_at(iFrame);

      }

      VkDescriptorImageInfo imageinfo;

      ::cast<context> pcontext = m_pgpurenderer->m_pgpucontext;

      imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      imageinfo.imageView = get_image_view();
      imageinfo.sampler = pcontext->_001VkSampler();

      unsigned int uSamplerBinding = 0;

      // bind source expects the image to be the only binding
      // in the descriptor set
      // to avoid running out of descriptors when using a certain
      // size for the descriptor pool, going to store the
      // descriptor pool in the texture.

      auto pgpubindingslotset = _001BindingSlotSet(pshader, pgpucommandbuffer);

      ::cast<::gpu_vulkan::binding_set> pbindingset = pgpubindingslotset->m_pbindingset;

      ::cast<::gpu_vulkan::binding_slot_set> pbindingslotset = pgpubindingslotset;

      auto playout = pbindingset->descriptor_set_layout(pgpucommandbuffer);

      auto ppool = pbindingslotset->descriptor_pool(pgpucommandbuffer);

      // if (pshader->m_bindingSampler.is_set())
      //   uSamplerBinding = pshader->m_bindingSampler.m_uBinding;
      //else if (pshader->m_bindingCubeSampler.is_set())
      //   uSamplerBinding = pshader->m_bindingCubeSampler.m_uBinding;

      pdescriptorseta = ppool->allocate_descriptor_set_array(this);


      for (int i = 0; i < iFrameCount; i++)
      {

         if (!pdescriptorseta->ø(i))
         {

            VkDescriptorSet vkdescriptorset = VK_NULL_HANDLE;

            descriptor_writer(*playout, *ppool).writeImage(uSamplerBinding, &imageinfo).build(vkdescriptorset);

            pdescriptorseta->ø(i) = vkdescriptorset;

         }

      }

      auto vkdescriptorsetFrame = pdescriptorseta->element_at(iFrame);

      if ((((::uptr)vkdescriptorsetFrame) & 0xffff) == 0x287)
      {

         ::information("(vkdescriptorset & 0xffff) == 0x287");

      }
      else if ((((::uptr)vkdescriptorsetFrame) & 0xffff) == 0x1e5)
      {

         ::information("(vkdescriptorset & 0xffff) == 0x1e5");
      }

      return vkdescriptorsetFrame;

   }



         /// @brief Singular binding slot set (_001 case here), when the texture is
   /// the only binding in the descriptor set
   ::gpu::binding_slot_set * texture::_001BindingSlotSet(::gpu_vulkan::shader * pshader,
                                                      ::gpu::command_buffer * pgpucommandbuffer)
   {

      auto &pbindingslotset = m_mapBindingSlotSet1[pshader];

      if (pbindingslotset)
      {

         return pbindingslotset;

      }

      øconstruct_new(pbindingslotset);

      int iSet = 0;

      int iBinding = 0;

      pbindingslotset->m_pbindingset = pshader->_001GetSingularImageBindingSet(iSet);

      pbindingslotset->m_iSet = iSet;

      auto & bindingslot = pbindingslotset->add_new();

      bindingslot.m_pbinding = pbindingslotset->m_pbindingset->first();

      ASSERT(bindingslot.m_pbinding->is_image_sampler());

      bindingslot.m_iSet = iSet;

      bindingslot.m_iSlot = 0;

      bindingslot.m_ptexture = this;

      return pbindingslotset;

   }


   //VkFramebuffer texture::_framebuffer(::gpu_vulkan::render_pass *prenderpass, int iMip)
   //{

   //   ASSERT(iMip <= 0); // not yet implemented different mip level support
   //   // other than mip 0

   //   if (m_textureflags.m_bCpuRead)
   //   {

   //      return VK_NULL_HANDLE;
   //   }

   //   auto &framebuffer = m_mapFramebuffer[prenderpass->m_vkrenderpass];

   //   if (framebuffer)
   //   {
   //      return framebuffer;
   //   }

   //   ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;

   //   VkImageView attachments[2];

   //   attachments[0] = get_image_view();

   //   int iAttachmentCount;

   //   if (prenderpass->m_bWithDepth)
   //   {

   //      if (!m_textureflags.m_bWithDepth)
   //      {

   //         m_textureflags.m_bWithDepth = true;
   //      }

   //      VkImageView depthImageView = get_depth_image_view();

   //      attachments[1] = depthImageView;

   //      iAttachmentCount = 2;
   //   }
   //   else
   //   {

   //      iAttachmentCount = 1;
   //   }

   //   VkFramebufferCreateInfo fbInfo = {.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
   //                                     .renderPass = prenderpass->m_vkrenderpass,
   //                                     .attachmentCount = (uint32_t)iAttachmentCount,
   //                                     .pAttachments = attachments,
   //                                     .width = (uint32_t)this->width(),
   //                                     .height = (uint32_t)this->height(),
   //                                     .layers = 1};

   //   VkCheckResult(vkCreateFramebuffer(pcontext->logicalDevice(), &fbInfo, NULL, &framebuffer));

   //   return framebuffer;
   //}


   VkImageView texture::get_depth_image_view()
   {

      if (m_textureattributes.m_etexture & ::gpu::e_texture_depth)
      {

         if (!m_vkimageview)
         {

            ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;

            VkFormat depthFormat = pcontext->findDepthFormat();

            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = get_depth_image();
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = depthFormat;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(pcontext->logicalDevice(), &viewInfo, nullptr, &m_vkimageview) != VK_SUCCESS)
            {

               throw ::exception(error_failed, "failed to create texture image view!");
            }

            debug() << "created a depth resource";
         }

         return m_vkimageview;
      }
      else
      {

         // ASSERT(m_textureflags.m_bWithDepth);

         ::cast<texture> ptexture = get_depth_texture();

         if (!ptexture)
         {

            return nullptr;
         }

         return ptexture->get_depth_image_view();
      }
   }


   void texture::_attach(VkImage vkimage, ::gpu::enum_texture etexture)
   {

      destroy();

      m_bOwnImage = false;

      m_textureattributes.m_etexture = etexture;

      m_vkimage = vkimage;
   }

   unsigned int texture::_get_layer_count()
   {
      if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
      {

         return 6;
      }
      else
      {

         return 1;
      }
   }


   VkImageViewType texture::_get_image_view_type()
   {

      if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
      {

         return VK_IMAGE_VIEW_TYPE_CUBE;
      }
      else
      {
         return VK_IMAGE_VIEW_TYPE_2D;
      }
   }


   void texture::destroy()
   {

      m_vkimage = nullptr;
      m_vkdevicememory = nullptr;
      m_state2a.erase_all();
      //m_state.m_vkimagelayout = VK_IMAGE_LAYOUT_UNDEFINED;
      //m_state.m_vkaccessflags = VK_ACCESS_NONE;
      //m_state.m_vkpipelinestageflags = VK_PIPELINE_STAGE_NONE;
      m_vkimageview = nullptr;
   }




   //   bool VkSandboxTexture::STBLoadFromFile(const std::string &filename)
   //{
   //   int texWidth, texHeight, texChannels;
   //   stbi_uc *pixels = stbi_load(filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
   //   if (!pixels)
   //   {
   //      throw std::runtime_error("Failed to load texture image: " + filename);
   //   }
   //   VkDeviceSize imageSize = static_cast<VkDeviceSize>(texWidth) * texHeight * 4;

   //   // Stage data
   //   VkBuffer stagingBuffer;
   //   VkDeviceMemory stagingMemory;
   //   m_pDevice->createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
   //                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
   //                           stagingMemory);

   //   void *data;
   //   vkMapMemory(pcontext->logicalDevice(), stagingMemory, 0, imageSize, 0, &data);
   //   memcpy(data, pixels, static_cast<size_t>(imageSize));
   //   vkUnmapMemory(pcontext->logicalDevice(), stagingMemory);
   //   stbi_image_free(pixels);

   //   // Create and upload to 2D image
   //   CreateImage(static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight), VK_FORMAT_R8G8B8A8_SRGB,
   //               VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
   //               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
   //               1, // arrayLayers
   //               0 // flags
   //   );

   //   TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
   //                         1 // layerCount
   //   );
   //   CopyBufferToImage(stagingBuffer, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight),
   //                     1 // layerCount
   //   );
   //   TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //                         1 // layerCount
   //   );

   //   vkDestroyBuffer(pcontext->logicalDevice(), stagingBuffer, nullptr);
   //   vkFreeMemory(pcontext->logicalDevice(), stagingMemory, nullptr);

   //   // Create view and sampler for 2D
   //   CreateImageView(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_VIEW_TYPE_2D,
   //                   1 // layerCount
   //   );
   //   CreateSampler();
   //   UpdateDescriptor();

   //   return true;
   //}

   VkDeviceMemory texture::AllocateMemory(VkMemoryRequirements memRequirements, VkMemoryPropertyFlags properties)
   {
            
      ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;
      ::cast<::gpu_vulkan::device> pdevice = pcontext->m_pgpudevice;
            auto pphysicaldevice = pdevice->m_pphysicaldevice;
      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memRequirements.size;
      allocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(memRequirements.memoryTypeBits, properties);

      VkDeviceMemory memory;
      auto vkresultAllocateMemory = vkAllocateMemory(pcontext->logicalDevice(), &allocInfo, nullptr, &memory);

      pdevice->_defer_throw_vkresult(vkresultAllocateMemory, "Failed to allocate image memory!");

      return memory;
   }


      bool texture::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
                                      VkImageUsageFlags usage, VkMemoryPropertyFlags properties, uint32_t arrayLayers,
                                      VkImageCreateFlags flags)
   {

            ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;
      ::cast<::gpu_vulkan::device> pdevice = pcontext->m_pgpudevice;


      VkImageCreateInfo info{};
      info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
      info.flags = flags;
      info.imageType = VK_IMAGE_TYPE_2D;
      info.extent = {width, height, 1};
      info.mipLevels = 1;
      info.arrayLayers = arrayLayers;
      info.format = format;
      info.tiling = tiling;
      info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      info.usage = usage;
      info.samples = VK_SAMPLE_COUNT_1_BIT;
      info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

      auto vkresultCreateImage = vkCreateImage(pcontext->logicalDevice(), &info, nullptr, &m_vkimage);

      pdevice->_defer_throw_vkresult(vkresultCreateImage, "Failed to create image!");
      //{
      //   throw ::exception(error_failed,
      //}
      VkMemoryRequirements memReq;
      vkGetImageMemoryRequirements(pcontext->logicalDevice(), m_vkimage, &memReq);
      m_vkdevicememory = AllocateMemory(memReq, properties);
      vkBindImageMemory(pcontext->logicalDevice(), m_vkimage, m_vkdevicememory, 0);

      return true;

   }

      
void texture::create_sampler()
   {

         ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;
      ::cast<::gpu_vulkan::device> pdevice = pcontext->m_pgpudevice;

      VkSamplerCreateInfo samplerInfo{};
      samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
      samplerInfo.magFilter = VK_FILTER_LINEAR;
      samplerInfo.minFilter = VK_FILTER_LINEAR;
      samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
      samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
      samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
      samplerInfo.anisotropyEnable = VK_TRUE;
      samplerInfo.maxAnisotropy = 16.0f;
      samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
      samplerInfo.unnormalizedCoordinates = VK_FALSE;
      samplerInfo.compareEnable = VK_FALSE;
      samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

      auto vkresultCreateSampler = vkCreateSampler(pcontext->logicalDevice(), &samplerInfo, nullptr, &m_vksamplerDedicated);

      pdevice->_defer_throw_vkresult(vkresultCreateSampler, "Failed to create texture sampler!");

}

   bool texture::imaging_load_from_file(const ::file::path &path)
   {

      information("texture::imaging_load_from_file");
      ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;
      ::cast<::gpu_vulkan::device> pdevice = pcontext->m_pgpudevice;


      int texWidth, texHeight, texChannels;
      ::pointer<::gpu_vulkan::buffer> pbuffer;
      {
      auto pimage = image()->path_image(path);
      if (!pimage)
      {
         throw ::exception(error_failed, "Failed to load texture image: " + path);
      }

      texWidth = pimage->width();
      texHeight = pimage->height();

      VkDeviceSize imageSize = static_cast<VkDeviceSize>(texWidth) * texHeight * 4;

      pbuffer = pcontext->create_buffer(
         imageSize,

         VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
                                             

      //// Stage data
      //VkBuffer stagingBuffer;
      //VkDeviceMemory stagingMemory;
      //pdevice->createBuffer(imageSize, ,
      //                      ,
      //                      stagingMemory);

      void *data = pbuffer->map();
      
      //vkMapMemory(pcontext->logicalDevice(), stagingMemory, 0, imageSize, 0, &data);
      
      auto pimage32 = (image32_t *)data;
      pimage32->copy(pimage->size(), pimage->width() * 4, pimage);

      pbuffer->unmap();
      //vkUnmapMemory(pcontext->logicalDevice(), stagingMemory);
   }
      //stbi_image_free(pixels);

      // Create and upload to 2D image
      CreateImage(static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight), VK_FORMAT_R8G8B8A8_SRGB,
                  VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                  1, // arrayLayers
                  0 // flags
      );

      pcontext->transitionImageLayout(m_vkimage, m_vkformat, VK_IMAGE_LAYOUT_UNDEFINED,
                                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_textureattributes.m_iMipCount,
                                       1); // layerCount

               auto pcommandbuffer = pcontext->beginSingleTimeCommands(nullptr);
               ::int_rectangle r(::int_point(0, 0), ::int_size(texWidth, texHeight));
               pcontext->copyBufferToImage(pcommandbuffer, this, pbuffer, r);
                                      // 1 // layerCount
      pcontext->endSingleTimeCommands(nullptr);
      pcontext->transitionImageLayout(
         m_vkimage, m_vkformat, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
         VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 
         m_textureattributes.m_iMipCount,
          1); // layerCount
      //TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //                      1 // layerCount
      //);

      //vkDestroyBuffer(pcontext->logicalDevice(), stagingBuffer, nullptr);
      //vkFreeMemory(pcontext->logicalDevice(), stagingMemory, nullptr);

      // Create view and sampler for 2D
      //;
      //;
      //CreateImageView(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_VIEW_TYPE_2D,
                      //1 // layerCount
      //);

      create_image_view();

      //CreateSampler();
      create_sampler();
      //UpdateDescriptor();

      return true;
   }


   
//void texture::TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t layerCount)
//   {
//      //VkCommandBuffer cmd = m_pDevice->beginSingleTimeCommands();
//      m_pDevice->transitionImageLayout(m_image, m_format, oldLayout, newLayout, m_iMipCount, layerCount);
//      //m_pDevice->endSingleTimeCommands(cmd);
//   }
//
//   void texture::CopyBufferToImage(VkBuffer buffer, uint32_t width, uint32_t height, uint32_t layerCount)
//   {
//      m_pDevice->copyBufferToImage(buffer, m_image, width, height, layerCount);
//   }

   //bool VkSandboxTexture::KTXLoadFromFile(const std::string &filename, VkFormat format, VkSandboxDevice *device,
   //                                       VkQueue copyQueue, VkImageUsageFlags imageUsageFlags,
   //                                       VkImageLayout imageLayout, bool forceLinear)
   //{
   //   ktxTexture *ktxTexture;
   //   ktxResult result = loadKTXFile(filename, &ktxTexture);
   //   assert(result == KTX_SUCCESS);

   //   this->m_pDevice = device;
   //   m_width = ktxTexture->baseWidth;
   //   m_height = ktxTexture->baseHeight;
   //   m_iMipCount = ktxTexture->numLevels;

   //   ktx_uint8_t *ktxTextureData = ktxTexture_GetData(ktxTexture);
   //   ktx_size_t ktxTextureSize = ktxTexture_GetSize(ktxTexture);

   //   VkFormatProperties formatProperties;
   //   vkGetPhysicalDeviceFormatProperties(device->m_physicalDevice, format, &formatProperties);

   //   VkBool32 useStaging = !forceLinear;

   //   VkMemoryAllocateInfo memAllocInfo = vkinit::memoryAllocateInfo();
   //   VkMemoryRequirements memReqs;

   //   VkCommandBuffer pcommandbufferCmd->m_vkcommandbuffer = device->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

   //   if (useStaging)
   //   {
   //      VkBuffer stagingBuffer;
   //      VkDeviceMemory stagingMemory;

   //      VkBufferCreateInfo bufferCreateInfo = vkinit::bufferCreateInfo();
   //      bufferCreateInfo.size = ktxTextureSize;
   //      // This buffer is used as a transfer source for the buffer copy
   //      bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
   //      bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

   //      VkCheckResult(vkCreateBuffer(pcontext->logicalDevice(), &bufferCreateInfo, nullptr, &stagingBuffer));

   //      // Get memory requirements for the staging buffer (alignment, memory type bits)
   //      vkGetBufferMemoryRequirements(pcontext->logicalDevice(), stagingBuffer, &memReqs);

   //      memAllocInfo.allocationSize = memReqs.size;
   //      // Get memory type index for a host visible buffer
   //      memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(
   //         memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

   //      VkCheckResult(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &stagingMemory));
   //      VkCheckResult(vkBindBufferMemory(pcontext->logicalDevice(), stagingBuffer, stagingMemory, 0));

   //      // Copy texture data into staging buffer
   //      uint8_t *data;
   //      VkCheckResult(vkMapMemory(pcontext->logicalDevice(), stagingMemory, 0, memReqs.size, 0, (void **)&data));
   //      memcpy(data, ktxTextureData, ktxTextureSize);
   //      vkUnmapMemory(pcontext->logicalDevice(), stagingMemory);

   //      // Setup buffer copy regions for each mip level
   //      std::vector<VkBufferImageCopy> bufferCopyRegions;

   //      for (uint32_t i = 0; i < m_iMipCount; i++)
   //      {
   //         ktx_size_t offset;
   //         KTX_error_code result = ktxTexture_GetImageOffset(ktxTexture, i, 0, 0, &offset);
   //         assert(result == KTX_SUCCESS);

   //         VkBufferImageCopy bufferCopyRegion = {};
   //         bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   //         bufferCopyRegion.imageSubresource.mipLevel = i;
   //         bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
   //         bufferCopyRegion.imageSubresource.layerCount = 1;
   //         bufferCopyRegion.imageExtent.width = std::max(1u, ktxTexture->baseWidth >> i);
   //         bufferCopyRegion.imageExtent.height = std::max(1u, ktxTexture->baseHeight >> i);
   //         bufferCopyRegion.imageExtent.depth = 1;
   //         bufferCopyRegion.bufferOffset = offset;


   //         bufferCopyRegions.push_back(bufferCopyRegion);
   //      }

   //      // Create optimal tiled target image
   //      VkImageCreateInfo imageCreateInfo = vkinit::imageCreateInfo();
   //      imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
   //      imageCreateInfo.format = format;
   //      imageCreateInfo.mipLevels = m_iMipCount;
   //      imageCreateInfo.arrayLayers = 1;
   //      imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
   //      imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
   //      imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
   //      imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
   //      imageCreateInfo.extent = {m_width, m_height, 1};
   //      imageCreateInfo.usage = imageUsageFlags;
   //      // Ensure that the TRANSFER_DST bit is set for staging
   //      if (!(imageCreateInfo.usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT))
   //      {
   //         imageCreateInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
   //      }
   //      VkCheckResult(vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &m_image));

   //      vkGetImageMemoryRequirements(pcontext->logicalDevice(), m_image, &memReqs);

   //      memAllocInfo.allocationSize = memReqs.size;

   //      memAllocInfo.memoryTypeIndex =
   //         pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
   //      VkCheckResult(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &m_deviceMemory));
   //      VkCheckResult(vkBindImageMemory(pcontext->logicalDevice(), m_image, m_deviceMemory, 0));

   //      VkImageSubresourceRange subresourceRange = {};
   //      subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   //      subresourceRange.baseMipLevel = 0;
   //      subresourceRange.levelCount = m_iMipCount;
   //      subresourceRange.layerCount = 1;


   //      // Image barrier for optimal image (target)
   //      // Optimal image will be used as destination for the copy
   //      tools::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, m_image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
   //                            subresourceRange);

   //      // Copy mip levels from staging buffer
   //      vkCmdCopyBufferToImage(pcommandbufferCmd->m_vkcommandbuffer, stagingBuffer, m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
   //                             static_cast<uint32_t>(bufferCopyRegions.size()), bufferCopyRegions.data());

   //      // Change texture image layout to shader read after all mip levels have been copied
   //      this->m_imageLayout = imageLayout;
   //      tools::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, imageLayout, subresourceRange);

   //      device->flushCommandBuffer(pcommandbufferCmd->m_vkcommandbuffer, copyQueue);

   //      // Clean up staging resources
   //      vkDestroyBuffer(pcontext->logicalDevice(), stagingBuffer, nullptr);
   //      vkFreeMemory(pcontext->logicalDevice(), stagingMemory, nullptr);
   //   }
   //   else
   //   {
   //      // Prefer using optimal tiling, as linear tiling
   //      // may support only a small set of features
   //      // depending on implementation (e.g. no mip maps, only one layer, etc.)

   //      // Check if this support is supported for linear tiling
   //      assert(formatProperties.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);

   //      VkImage mappableImage;
   //      VkDeviceMemory mappableMemory;

   //      VkImageCreateInfo imageCreateInfo = vkinit::imageCreateInfo();
   //      imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
   //      imageCreateInfo.format = format;
   //      imageCreateInfo.extent = {m_width, m_height, 1};
   //      imageCreateInfo.mipLevels = 1;
   //      imageCreateInfo.arrayLayers = 1;
   //      imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
   //      imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
   //      imageCreateInfo.usage = imageUsageFlags;
   //      imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
   //      imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

   //      // Load mip map level 0 to linear tiling image
   //      VkCheckResult(vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &mappableImage));

   //      // Get memory requirements for this image
   //      // like size and alignment
   //      vkGetImageMemoryRequirements(pcontext->logicalDevice(), mappableImage, &memReqs);
   //      // Set memory allocation size to required memory size
   //      memAllocInfo.allocationSize = memReqs.size;

   //      // Get memory type that can be mapped to host memory
   //      memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(
   //         memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

   //      // Allocate host memory
   //      VkCheckResult(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &mappableMemory));

   //      // Bind allocated image for use
   //      VkCheckResult(vkBindImageMemory(pcontext->logicalDevice(), mappableImage, mappableMemory, 0));

   //      // Get sub resource layout
   //      // Mip map count, array layer, etc.
   //      VkImageSubresource subRes = {};
   //      subRes.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   //      subRes.mipLevel = 0;

   //      VkSubresourceLayout subResLayout;
   //      void *data;

   //      // Get sub resources layout
   //      // Includes row pitch, size offsets, etc.
   //      vkGetImageSubresourceLayout(pcontext->logicalDevice(), mappableImage, &subRes, &subResLayout);

   //      // Map image memory
   //      VkCheckResult(vkMapMemory(pcontext->logicalDevice(), mappableMemory, 0, memReqs.size, 0, &data));

   //      // Copy image data into memory
   //      memcpy(data, ktxTextureData, memReqs.size);

   //      vkUnmapMemory(pcontext->logicalDevice(), mappableMemory);

   //      // Linear tiled images don't need to be staged
   //      // and can be directly used as textures
   //      m_image = mappableImage;
   //      m_deviceMemory = mappableMemory;
   //      this->m_imageLayout = imageLayout;

   //      // Setup image memory barrier
   //      tools::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, m_image, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_UNDEFINED, imageLayout);

   //      device->flushCommandBuffer(pcommandbufferCmd->m_vkcommandbuffer, copyQueue);
   //   }
   //   ktxTexture_Destroy(ktxTexture);
   //   // Create sampler with anisotropic filtering
   //   VkSamplerCreateInfo samplerCreateInfo{};
   //   samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
   //   samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
   //   samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
   //   samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
   //   samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
   //   samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
   //   samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
   //   samplerCreateInfo.minLod = 0.0f;
   //   samplerCreateInfo.maxLod = static_cast<float>(m_iMipCount);
   //   samplerCreateInfo.mipLodBias = 0.0f;
   //   samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;

   //   // Enable anisotropy if supported
   //   samplerCreateInfo.anisotropyEnable = device->m_enabledFeatures.samplerAnisotropy ? VK_TRUE : VK_FALSE;
   //   samplerCreateInfo.maxAnisotropy =
   //      samplerCreateInfo.anisotropyEnable ? device->m_deviceProperties.limits.maxSamplerAnisotropy : 1.0f;

   //   samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

   //   VkCheckResult(vkCreateSampler(pcontext->logicalDevice(), &samplerCreateInfo, nullptr, &m_sampler));

   //   // Create image view
   //   VkImageViewCreateInfo viewCreateInfo{};
   //   viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
   //   viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
   //   viewCreateInfo.format = format;
   //   viewCreateInfo.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, m_iMipCount, 0, 1};
   //   viewCreateInfo.image = m_image;
   //   VkCheckResult(vkCreateImageView(pcontext->logicalDevice(), &viewCreateInfo, nullptr, &m_view));

   //   // Update descriptor for shader sampling
   //   UpdateDescriptor();

   //   return true;
   //}




      bool texture::KTXLoadFromFile(const ::file::path  & path, 
                                          VkQueue copyQueue, VkImageUsageFlags imageUsageFlags,
                                          VkImageLayout imageLayout, bool forceLinear)
   {
      ktxTexture *ktxTexture;
      ktxResult result = ::gpu::loadKTXFile(this, path, &ktxTexture);
      ASSERT(result == KTX_SUCCESS);

      KTXLoadFrom_ktxTexture(ktxTexture, copyQueue, imageUsageFlags, imageLayout, forceLinear);
      

      //::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;
      //::cast<::gpu_vulkan::device> pdevice = pcontext->m_pgpudevice;

      //auto pphysicaldevice = pdevice->m_pphysicaldevice;

      ////this->m_pDevice = device;
      //m_textureattributes.m_rectangleTarget.left = 0;
      //m_textureattributes.m_rectangleTarget.top = 0;
      //// this->m_pDevice = pdevice;
      //m_textureattributes.m_rectangleTarget.right = ktxTexture->baseWidth;
      //m_textureattributes.m_rectangleTarget.bottom = ktxTexture->baseHeight;
      //m_textureattributes.m_iMipCount = ktxTexture->numLevels;

      //ktx_uint8_t *ktxTextureData = ktxTexture_GetData(ktxTexture);
      //ktx_size_t ktxTextureSize = ktxTexture_GetDataSize(ktxTexture);

      //VkFormatProperties formatProperties;
      //vkGetPhysicalDeviceFormatProperties(pphysicaldevice->m_vkphysicaldevice, format, &formatProperties);

      //VkBool32 useStaging = !forceLinear;

      //VkMemoryAllocateInfo memAllocInfo = vkinit::memoryAllocateInfo();
      //VkMemoryRequirements memReqs;

      ////VkCommandBuffer pcommandbufferCmd->m_vkcommandbuffer = device->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

      //auto pgpucommandbufferCmd = pcontext->beginSingleTimeCommands(pcontext->m_pgpudevice->transfer_queue());

      //::cast<::gpu_vulkan::command_buffer> pcommandbufferCmd = pgpucommandbufferCmd;

      //if (useStaging)
      //{
      //   VkBuffer stagingBuffer;
      //   VkDeviceMemory stagingMemory;

      //   VkBufferCreateInfo bufferCreateInfo = vkinit::bufferCreateInfo();
      //   bufferCreateInfo.size = ktxTextureSize;
      //   // This buffer is used as a transfer source for the buffer copy
      //   bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
      //   bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

      //   VkCheckResult(vkCreateBuffer(pcontext->logicalDevice(), &bufferCreateInfo, nullptr, &stagingBuffer));

      //   // Get memory requirements for the staging buffer (alignment, memory type bits)
      //   vkGetBufferMemoryRequirements(pcontext->logicalDevice(), stagingBuffer, &memReqs);

      //   memAllocInfo.allocationSize = memReqs.size;
      //   // Get memory type index for a host visible buffer
      //   memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(
      //      memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

      //   VkCheckResult(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &stagingMemory));
      //   VkCheckResult(vkBindBufferMemory(pcontext->logicalDevice(), stagingBuffer, stagingMemory, 0));

      //   // Copy texture data into staging buffer
      //   uint8_t *data;
      //   VkCheckResult(vkMapMemory(pcontext->logicalDevice(), stagingMemory, 0, memReqs.size, 0, (void **)&data));
      //   memcpy(data, ktxTextureData, ktxTextureSize);
      //   vkUnmapMemory(pcontext->logicalDevice(), stagingMemory);

      //   // Setup buffer copy regions for each mip level
      //   std::vector<VkBufferImageCopy> bufferCopyRegions;

      //   for (uint32_t i = 0; i <m_textureattributes.m_iMipCount; i++)
      //   {
      //      ktx_size_t offset;
      //      KTX_error_code result = ktxTexture_GetImageOffset(ktxTexture, i, 0, 0, &offset);
      //      assert(result == KTX_SUCCESS);

      //      VkBufferImageCopy bufferCopyRegion = {};
      //      bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      //      bufferCopyRegion.imageSubresource.mipLevel = i;
      //      bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
      //      bufferCopyRegion.imageSubresource.layerCount = 1;
      //      bufferCopyRegion.imageExtent.width = std::max(1u, ktxTexture->baseWidth >> i);
      //      bufferCopyRegion.imageExtent.height = std::max(1u, ktxTexture->baseHeight >> i);
      //      bufferCopyRegion.imageExtent.depth = 1;
      //      bufferCopyRegion.bufferOffset = offset;


      //      bufferCopyRegions.push_back(bufferCopyRegion);
      //   }

      //   // Create optimal tiled target image
      //   VkImageCreateInfo imageCreateInfo = vkinit::imageCreateInfo();
      //   imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
      //   imageCreateInfo.format = format;
      //   imageCreateInfo.mipLevels = m_textureattributes.m_iMipCount;
      //   imageCreateInfo.arrayLayers = 1;
      //   imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
      //   imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
      //   imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
      //   imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      //   imageCreateInfo.extent = 
      //   {
      //      (uint32_t)m_textureattributes.m_rectangleTarget.width(),
      //      (uint32_t)m_textureattributes.m_rectangleTarget.height(), 1
      //   };
      //   imageCreateInfo.usage = imageUsageFlags;
      //   // Ensure that the TRANSFER_DST bit is set for staging
      //   if (!(imageCreateInfo.usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT))
      //   {
      //      imageCreateInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
      //   }
      //   VkCheckResult(vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &m_vkimage));

      //   vkGetImageMemoryRequirements(pcontext->logicalDevice(), m_vkimage, &memReqs);

      //   memAllocInfo.allocationSize = memReqs.size;

      //   memAllocInfo.memoryTypeIndex =
      //      pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
      //   VkCheckResult(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &m_vkdevicememory));
      //   VkCheckResult(vkBindImageMemory(pcontext->logicalDevice(), m_vkimage, m_vkdevicememory, 0));

      //   VkImageSubresourceRange subresourceRange = {};
      //   subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      //   subresourceRange.baseMipLevel = 0;
      //   subresourceRange.levelCount = m_textureattributes.m_iMipCount;
      //   subresourceRange.layerCount = 1;


      //   // Image barrier for optimal image (target)
      //   // Optimal image will be used as destination for the copy
      //   vulkan::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, m_vkimage, VK_IMAGE_LAYOUT_UNDEFINED,
      //                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      //                         subresourceRange);

      //   // Copy mip levels from staging buffer
      //   vkCmdCopyBufferToImage(pcommandbufferCmd->m_vkcommandbuffer, stagingBuffer, m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      //                          static_cast<uint32_t>(bufferCopyRegions.size()), bufferCopyRegions.data());

      //   // Change texture image layout to shader read after all mip levels have been copied
      //   //this->m_state.m_vkimagelayout = imageLayout;
      //   //::vulkan::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, imageLayout, subresourceRange);
      //   set_state(pcommandbufferCmd, ::gpu::e_texture_state_shader_read);

      //   //device->flushCommandBuffer(pcommandbufferCmd->m_vkcommandbuffer, copyQueue);

      //   pcontext->endSingleTimeCommands(pcommandbufferCmd);

      //   // Clean up staging resources
      //   vkDestroyBuffer(pcontext->logicalDevice(), stagingBuffer, nullptr);
      //   vkFreeMemory(pcontext->logicalDevice(), stagingMemory, nullptr);
      //}
      //else
      //{
      //   // Prefer using optimal tiling, as linear tiling
      //   // may support only a small set of features
      //   // depending on implementation (e.g. no mip maps, only one layer, etc.)

      //   // Check if this support is supported for linear tiling
      //   assert(formatProperties.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);

      //   VkImage mappableImage;
      //   VkDeviceMemory mappableMemory;

      //   VkImageCreateInfo imageCreateInfo = vkinit::imageCreateInfo();
      //   imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
      //   imageCreateInfo.format = format;
      //   imageCreateInfo.extent = {
      //      (uint32_t) m_textureattributes.m_rectangleTarget.width(),
      //      (uint32_t)m_textureattributes.m_rectangleTarget.height(),
      //      1};
      //   imageCreateInfo.mipLevels = 1;
      //   imageCreateInfo.arrayLayers = 1;
      //   imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
      //   imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
      //   imageCreateInfo.usage = imageUsageFlags;
      //   imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
      //   imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

      //   // Load mip map level 0 to linear tiling image
      //   VkCheckResult(vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &mappableImage));

      //   // Get memory requirements for this image
      //   // like size and alignment
      //   vkGetImageMemoryRequirements(pcontext->logicalDevice(), mappableImage, &memReqs);
      //   // Set memory allocation size to required memory size
      //   memAllocInfo.allocationSize = memReqs.size;

      //   // Get memory type that can be mapped to host memory
      //   memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(
      //      memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

      //   // Allocate host memory
      //   VkCheckResult(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &mappableMemory));

      //   // Bind allocated image for use
      //   VkCheckResult(vkBindImageMemory(pcontext->logicalDevice(), mappableImage, mappableMemory, 0));

      //   // Get sub resource layout
      //   // Mip map count, array layer, etc.
      //   VkImageSubresource subRes = {};
      //   subRes.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      //   subRes.mipLevel = 0;

      //   VkSubresourceLayout subResLayout;
      //   void *data;

      //   // Get sub resources layout
      //   // Includes row pitch, size offsets, etc.
      //   vkGetImageSubresourceLayout(pcontext->logicalDevice(), mappableImage, &subRes, &subResLayout);

      //   // Map image memory
      //   VkCheckResult(vkMapMemory(pcontext->logicalDevice(), mappableMemory, 0, memReqs.size, 0, &data));

      //   // Copy image data into memory
      //   memcpy(data, ktxTextureData, memReqs.size);

      //   vkUnmapMemory(pcontext->logicalDevice(), mappableMemory);

      //   // Linear tiled images don't need to be staged
      //   // and can be directly used as textures
      //   m_vkimage = mappableImage;
      //   m_vkdevicememory = mappableMemory;
      //   //m_state.m_vkimagelayout = imageLayout;

      //   _set_state(pcommandbufferCmd, {0, imageLayout, 0});

      //   // Setup image memory barrier
      //   //vulkan::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, m_vkimage, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_UNDEFINED, imageLayout);

      //   pcontext->endSingleTimeCommands(pcommandbufferCmd);

      //}
      //ktxTexture_Destroy(ktxTexture);
      //// Create sampler with anisotropic filtering
      //VkSamplerCreateInfo samplerCreateInfo{};
      //samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
      //samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
      //samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
      //samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
      //samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
      //samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
      //samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
      //samplerCreateInfo.minLod = 0.0f;
      //samplerCreateInfo.maxLod = (float)(m_textureattributes.m_iMipCount);
      //samplerCreateInfo.mipLodBias = 0.0f;
      //samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;

      //// Enable anisotropy if supported
      //samplerCreateInfo.anisotropyEnable = pphysicaldevice->m_vkphysicaldevicefeatures.samplerAnisotropy ? VK_TRUE : VK_FALSE;
      //samplerCreateInfo.maxAnisotropy =
      //   samplerCreateInfo.anisotropyEnable ? pphysicaldevice->m_vkphysicaldeviceproperties.limits.maxSamplerAnisotropy : 1.0f;

      //samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

      //VkCheckResult(vkCreateSampler(pcontext->logicalDevice(), &samplerCreateInfo, nullptr, &m_vksamplerDedicated));

      //// Create image view
      //VkImageViewCreateInfo viewCreateInfo{};
      //viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      //viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      //viewCreateInfo.format = format;
      //viewCreateInfo.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, (uint32_t) m_textureattributes.m_iMipCount, 0, 1};
      //viewCreateInfo.image = m_vkimage;
      //VkCheckResult(vkCreateImageView(pcontext->logicalDevice(), &viewCreateInfo, nullptr, &m_vkimageview));

      //// Update descriptor for shader sampling
      /////UpdateDescriptor();

      return true;
   }


   // // Loads a cubemap from a single KTX file
   // void texture::KtxLoadCubemapFromFile(const ::scoped_string& name, ::string filename, VkFormat format, device*
   // pdevice, VkQueue copyQueue, VkImageUsageFlags imageUsageFlags, VkImageLayout imageLayout)
   // {
   // 	m_bIsCubemap = true;
   //
   // 	ktxTexture* pktxtexture;
   // 	ktxResult result = loadKTXFile(filename, &pktxtexture);
   // 	ASSERT(result == KTX_SUCCESS);
   //
   // 	this->m_pDevice = pdevice;
   // 	m_rectangleTarget.width() = pktxtexture->baseWidth;
   // 	m_rectangleTarget.height() = pktxtexture->baseHeight;
   // 	m_iMipCount = pktxtexture->numLevels;
   //
   // 	ktx_uint8_t* ktxTextureData = ktxTexture_GetData(pktxtexture);
   // 	ktx_size_t ktxTextureSize = ktxTexture_GetSize(pktxtexture);
   //
   // 	VkMemoryAllocateInfo memAllocInfo = vkinit::memoryAllocateInfo();
   // 	VkMemoryRequirements memReqs;
   //
   // 	// Create a host-visible staging buffer that contains the raw image data
   // 	VkBuffer stagingBuffer;
   // 	VkDeviceMemory stagingMemory;
   //
   // 	VkBufferCreateInfo bufferCreateInfo = vkinit::bufferCreateInfo();
   // 	bufferCreateInfo.size = ktxTextureSize;
   // 	// This buffer is used as a transfer source for the buffer copy
   // 	bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
   // 	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
   //
   // 	VkCheckResult(vkCreateBuffer(pcontext->logicalDevice(), &bufferCreateInfo, nullptr, &stagingBuffer));
   //
   // 	// Get memory requirements for the staging buffer (alignment, memory type bits)
   // 	vkGetBufferMemoryRequirements(pcontext->logicalDevice(), stagingBuffer, &memReqs);
   //
   // 	memAllocInfo.allocationSize = memReqs.size;
   // 	// Get memory type index for a host visible buffer
   // 	memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(memReqs.memoryTypeBits,
   // VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
   //
   // 	VkCheckResult(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &stagingMemory));
   // 	VkCheckResult(vkBindBufferMemory(pcontext->logicalDevice(), stagingBuffer, stagingMemory, 0));
   //
   // 	// Copy texture data into staging buffer
   // 	uint8_t* data;
   // 	VkCheckResult(vkMapMemory(pcontext->logicalDevice(), stagingMemory, 0, memReqs.size, 0, (void**)&data));
   // 	memcpy(data, ktxTextureData, ktxTextureSize);
   // 	vkUnmapMemory(pcontext->logicalDevice(), stagingMemory);
   //
   // 	// Setup buffer copy regions for each face including all of its mip levels
   // 	::array_base<VkBufferImageCopy> bufferCopyRegions;
   //
   // 	for (uint32_t face = 0; face < 6; face++)
   // 	{
   // 		for (uint32_t level = 0; level < m_iMipCount; level++)
   // 		{
   // 			ktx_size_t offset;
   // 			KTX_error_code result = ktxTexture_GetImageOffset(ktxTexture, level, 0, face, &offset);
   // 			ASSERT(result == KTX_SUCCESS);
   //
   // 			VkBufferImageCopy bufferCopyRegion = {};
   // 			bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   // 			bufferCopyRegion.imageSubresource.mipLevel = level;
   // 			bufferCopyRegion.imageSubresource.baseArrayLayer = face;
   // 			bufferCopyRegion.imageSubresource.layerCount = 1;
   // 			bufferCopyRegion.imageExtent.width = pktxtexture->baseWidth >> level;
   // 			bufferCopyRegion.imageExtent.height = pktxtexture->baseHeight >> level;
   // 			bufferCopyRegion.imageExtent.depth = 1;
   // 			bufferCopyRegion.bufferOffset = offset & ~0xF;
   //
   // 			bufferCopyRegions.add(bufferCopyRegion);
   // 		}
   // 	}
   //
   // 	// Create optimal tiled target image
   // 	VkImageCreateInfo imageCreateInfo = vkinit::imageCreateInfo();
   // 	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
   // 	imageCreateInfo.format = format;
   // 	imageCreateInfo.mipLevels = m_iMipCount;
   // 	imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
   // 	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
   // 	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
   // 	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
   // 	imageCreateInfo.extent = { m_rectangleTarget.width(), m_rectangleTarget.height(), 1 };
   // 	imageCreateInfo.usage = imageUsageFlags;
   // 	// Ensure that the TRANSFER_DST bit is set for staging
   // 	if (!(imageCreateInfo.usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT))
   // 	{
   // 		imageCreateInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
   // 	}
   // 	// Cube faces count as array layers in Vulkan
   // 	imageCreateInfo.arrayLayers = 6;
   // 	// This flag is required for cube map images
   // 	imageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
   //
   //
   // 	VkCheckResult(vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &m_vkimage));
   //
   // 	vkGetImageMemoryRequirements(pcontext->logicalDevice(), m_vkimage, &memReqs);
   //
   // 	memAllocInfo.allocationSize = memReqs.size;
   // 	memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(memReqs.memoryTypeBits,
   // VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
   //
   // 	VkCheckResult(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &m_vkdevicememory));
   // 	VkCheckResult(vkBindImageMemory(pcontext->logicalDevice(), m_vkimage, m_vkdevicememory, 0));
   //
   // 	// Use a separate command buffer for texture loading
   // 	VkCommandBuffer pcommandbufferCopy->m_vkcommandbuffer = pdevice->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
   //
   // 	// Image barrier for optimal image (target)
   // 	// Set initial layout for all array layers (faces) of the optimal (target) tiled texture
   // 	VkImageSubresourceRange subresourceRange = {};
   // 	subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   // 	subresourceRange.baseMipLevel = 0;
   // 	subresourceRange.levelCount = m_iMipCount;
   // 	subresourceRange.layerCount = 6;
   //
   // 	tools::setImageLayout(
   // 		pcommandbufferCopy->m_vkcommandbuffer,
   // 		m_vkimage,
   // 		VK_IMAGE_LAYOUT_UNDEFINED,
   // 		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
   // 		subresourceRange);
   //
   // 	// Copy the cube map faces from the staging buffer to the optimal tiled image
   // 	vkCmdCopyBufferToImage(
   // 		pcommandbufferCopy->m_vkcommandbuffer,
   // 		stagingBuffer,
   // 		m_vkimage,
   // 		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
   // 		static_cast<uint32_t>(bufferCopyRegions.size()),
   // 		bufferCopyRegions.data());
   //
   // 	// Change texture image layout to shader read after all faces have been copied
   // 	this->m_vkimageLayout = imageLayout;
   // 	tools::setImageLayout(
   // 		pcommandbufferCopy->m_vkcommandbuffer,
   // 		m_vkimage,
   // 		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
   // 		imageLayout,
   // 		subresourceRange);
   //
   // 	pdevice->flushCommandBuffer(pcommandbufferCopy->m_vkcommandbuffer, copyQueue);
   //
   // 	// Create sampler
   // 	VkSamplerCreateInfo samplerCreateInfo = vkinit::samplerCreateInfo();
   // 	samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
   // 	samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
   // 	samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
   // 	samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
   // 	samplerCreateInfo.addressModeV = samplerCreateInfo.addressModeU;
   // 	samplerCreateInfo.addressModeW = samplerCreateInfo.addressModeU;
   // 	samplerCreateInfo.mipLodBias = 0.0f;
   // 	samplerCreateInfo.maxAnisotropy = pdevice->m_enabledFeatures.samplerAnisotropy ?
   // pdevice->m_deviceProperties.limits.maxSamplerAnisotropy : 1.0f; 	samplerCreateInfo.anisotropyEnable =
   // pdevice->m_enabledFeatures.samplerAnisotropy ? VK_TRUE : VK_FALSE; 	samplerCreateInfo.compareOp =
   // VK_COMPARE_OP_NEVER; 	samplerCreateInfo.minLod = 0.0f; 	samplerCreateInfo.maxLod = (float)m_iMipCount;
   // 	samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
   // 	VkCheckResult(vkCreateSampler(pcontext->logicalDevice(), &samplerCreateInfo, nullptr, &m_sampler));
   //
   // 	// Create image view
   // 	VkImageViewCreateInfo viewCreateInfo = vkinit::imageViewCreateInfo();
   // 	viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
   // 	viewCreateInfo.format = format;
   // 	viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   // 	viewCreateInfo.subresourceRange.baseMipLevel = 0;
   // 	viewCreateInfo.subresourceRange.levelCount = m_iMipCount;
   // 	viewCreateInfo.subresourceRange.baseArrayLayer = 0;
   // 	viewCreateInfo.subresourceRange.layerCount = 6;
   // 	viewCreateInfo.image = m_vkimage;
   // 	VkCheckResult(vkCreateImageView(pcontext->logicalDevice(), &viewCreateInfo, nullptr, &m_view));
   //
   // 	// Clean up staging resources
   // 	ktxTexture_Destroy(pktxtexture);
   // 	vkDestroyBuffer(pcontext->logicalDevice(), stagingBuffer, nullptr);
   // 	vkFreeMemory(pcontext->logicalDevice(), stagingMemory, nullptr);
   //
   // 	// Update descriptor image info member that can be used for setting up descriptor sets
   // 	UpdateDescriptor();
   // }
   //
   //




   // Loads a cubemap from a single KTX file
   void texture::KtxLoadCubemapFromFile(
      const ::scoped_string &name, 
      ::string filename, 
      VkFormat format,
      VkQueue copyQueue,
      VkImageUsageFlags imageUsageFlags,
      VkImageLayout imageLayout)
   {
      m_textureattributes.m_etexture = ::gpu::e_texture_cube_map;

      ktxTexture *pktxtexture;
      ktxResult result = loadKTXFile(this, filename, &pktxtexture);
      ASSERT(result == KTX_SUCCESS);

      ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;

      ::cast<::gpu_vulkan::device> pgpudevice = pcontext->m_pgpudevice;

      auto pphysicaldevice = pgpudevice->m_pphysicaldevice;

      m_textureattributes.m_rectangleTarget.left = 0;
      m_textureattributes.m_rectangleTarget.top = 0;
      //this->m_pDevice = pdevice;
      m_textureattributes.m_rectangleTarget.right = pktxtexture->baseWidth;
      m_textureattributes.m_rectangleTarget.bottom = pktxtexture->baseHeight;
      m_textureattributes.m_iMipCount = pktxtexture->numLevels;

      ktx_uint8_t *ktxTextureData = ktxTexture_GetData(pktxtexture);
      ktx_size_t ktxTextureSize = ktxTexture_GetDataSize(pktxtexture);

      VkMemoryAllocateInfo memAllocInfo = vkinit::memoryAllocateInfo();
      VkMemoryRequirements memReqs;

      // Create a host-visible staging buffer that contains the raw image data
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

      // Setup buffer copy regions for each face including all of its mip levels
      ::array_base<VkBufferImageCopy> bufferCopyRegions;

      for (uint32_t face = 0; face < 6; face++)
      {
         for (uint32_t level = 0; level < m_textureattributes.m_iMipCount; level++)
         {
            ktx_size_t offset;
            KTX_error_code result = ktxTexture_GetImageOffset(pktxtexture, level, 0, face, &offset);
            ASSERT(result == KTX_SUCCESS);

            VkBufferImageCopy bufferCopyRegion = {};
            bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            bufferCopyRegion.imageSubresource.mipLevel = level;
            bufferCopyRegion.imageSubresource.baseArrayLayer = face;
            bufferCopyRegion.imageSubresource.layerCount = 1;
            bufferCopyRegion.imageExtent.width = pktxtexture->baseWidth >> level;
            bufferCopyRegion.imageExtent.height = pktxtexture->baseHeight >> level;
            bufferCopyRegion.imageExtent.depth = 1;
            bufferCopyRegion.bufferOffset = offset & ~0xF;

            bufferCopyRegions.add(bufferCopyRegion);
         }
      }

      // Create optimal tiled target image
      VkImageCreateInfo imageCreateInfo = vkinit::imageCreateInfo();
      imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
      imageCreateInfo.format = format;
      imageCreateInfo.mipLevels = m_textureattributes.m_iMipCount;
      imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
      imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
      imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
      imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      imageCreateInfo.extent = 
      {
         (uint32_t) m_textureattributes.m_rectangleTarget.width(),
         (uint32_t) m_textureattributes.m_rectangleTarget.height(),
         1
      };
      imageCreateInfo.usage = imageUsageFlags;
      // Ensure that the TRANSFER_DST bit is set for staging
      if (!(imageCreateInfo.usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT))
      {
         imageCreateInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
      }
      // Cube faces count as array layers in Vulkan
      imageCreateInfo.arrayLayers = 6;
      // This flag is required for cube map images
      imageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;


      VkCheckResult(vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &m_vkimage));

      vkGetImageMemoryRequirements(pcontext->logicalDevice(), m_vkimage, &memReqs);

      memAllocInfo.allocationSize = memReqs.size;
      memAllocInfo.memoryTypeIndex =
         pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

      VkCheckResult(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &m_vkdevicememory));
      VkCheckResult(vkBindImageMemory(pcontext->logicalDevice(), m_vkimage, m_vkdevicememory, 0));

      // Use a separate command buffer for texture loading
      //VkCommandBuffer pcommandbufferCopy->m_vkcommandbuffer = pdevice->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

      auto pgpucommandbufferCopy = pcontext->beginSingleTimeCommands(pcontext->m_pgpudevice->transfer_queue());

      ::cast<command_buffer> pcommandbufferCopy = pgpucommandbufferCopy;

      // Image barrier for optimal image (target)
      // Set initial layout for all array layers (faces) of the optimal (target) tiled texture
      VkImageSubresourceRange subresourceRange = {};
      subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      subresourceRange.baseMipLevel = 0;
      subresourceRange.levelCount = m_textureattributes.m_iMipCount;
      subresourceRange.layerCount = 6;

      vulkan::setImageLayout(pcommandbufferCopy->m_vkcommandbuffer, m_vkimage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                            subresourceRange);

      // Copy the cube map faces from the staging buffer to the optimal tiled image
      vkCmdCopyBufferToImage(pcommandbufferCopy->m_vkcommandbuffer, stagingBuffer, m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                             static_cast<uint32_t>(bufferCopyRegions.size()), bufferCopyRegions.data());

      // Change texture image layout to shader read after all faces have been copied
      //m_state.m_vkimagelayout = imageLayout;
      ///vulkan::setImageLayout(pcommandbufferCopy->m_vkcommandbuffer, m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, imageLayout, subresourceRange);
      set_state(pcommandbufferCopy, ::gpu::e_texture_state_shader_read);

      //pdevice->flushCommandBuffer(pcommandbufferCopy->m_vkcommandbuffer, copyQueue);

      pcontext->endSingleTimeCommands(pcommandbufferCopy);

      // Create sampler
      VkSamplerCreateInfo samplerCreateInfo = vkinit::samplerCreateInfo();
      samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
      samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
      samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
      samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
      samplerCreateInfo.addressModeV = samplerCreateInfo.addressModeU;
      samplerCreateInfo.addressModeW = samplerCreateInfo.addressModeU;
      samplerCreateInfo.mipLodBias = 0.0f;
      samplerCreateInfo.maxAnisotropy =
         pphysicaldevice->m_vkphysicaldevicefeatures.samplerAnisotropy ? 
         pphysicaldevice->m_vkphysicaldeviceproperties.limits.maxSamplerAnisotropy
                                           : 1.0f;
      samplerCreateInfo.anisotropyEnable = 
         pphysicaldevice->m_vkphysicaldevicefeatures.samplerAnisotropy ? VK_TRUE : VK_FALSE;
      samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
      samplerCreateInfo.minLod = 0.0f;
      samplerCreateInfo.maxLod = (float)m_textureattributes.m_iMipCount;
      samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
      VkCheckResult(vkCreateSampler(pcontext->logicalDevice(), &samplerCreateInfo, nullptr, &m_vksamplerDedicated));

      // Create image view
      VkImageViewCreateInfo viewCreateInfo = vkinit::imageViewCreateInfo();
      viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
      viewCreateInfo.format = format;
      viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      viewCreateInfo.subresourceRange.baseMipLevel = 0;
      viewCreateInfo.subresourceRange.levelCount = m_textureattributes.m_iMipCount;
      viewCreateInfo.subresourceRange.baseArrayLayer = 0;
      viewCreateInfo.subresourceRange.layerCount = 6;
      viewCreateInfo.image = m_vkimage;
      VkCheckResult(vkCreateImageView(pcontext->logicalDevice(), &viewCreateInfo, nullptr, &m_vkimageview));

      // Clean up staging resources
      ktxTexture_Destroy(pktxtexture);
      vkDestroyBuffer(pcontext->logicalDevice(), stagingBuffer, nullptr);
      vkFreeMemory(pcontext->logicalDevice(), stagingMemory, nullptr);

      // Update descriptor image info member that can be used for setting up descriptor sets
      //UpdateDescriptor();

   }


   
	//void texture::UpdateDescriptor()
 //  {

 //     if (m_vksamplerDedicated)
 //     {
 //        m_descriptor3.sampler = m_vksamplerDedicated;
 //     }
 //     else if (m_vksampler3)
 //     {

 //        m_descriptor3.sampler = m_vksampler3;
 //     }
 //     else
 //     {
 //        
 //        ::cast<context> pcontext = m_pgpurenderer->m_pgpucontext;
 //        
 //        m_descriptor3.sampler = pcontext->_001VkSampler();

 //     }
 //     m_descriptor3.imageView = get_image_view();
 //     m_descriptor3.imageLayout = m_state.m_vkimagelayout;
 //  }


   bool texture::is_in_shader_sampling_state()
   {

      if (mip_layer_state(0,0).m_vkimagelayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
      {

         return true;

      }
      else
      {

         return false;

      }

   }


   void texture::initialize_hdr_texture_on_memory(::gpu::renderer *pgpurenderer, const ::block &block)
   {

      ::gpu::context_lock contextlock(pgpurenderer->m_pgpucontext);

      m_pgpurenderer = pgpurenderer;

      auto blockData = block.data();

      auto blockSize = block.size();

      int width = 0;
      
      int height = 0;
      
      int channels;

      //stbi_set_flip_vertically_on_load(true);

      auto imagedata = stbi_loadf_from_memory(
         blockData, blockSize, &width, &height, &channels, 0);

      if (!imagedata)
      {

         warning() << "Failed to load texture data";

         stbi_image_free(imagedata);

         return;

      }

      m_textureattributes.m_rectangleTarget = ::int_rectangle(::int_size(width, height));

      m_textureflags.m_bWithDepth = false;

      ::cast<::gpu_vulkan::device> pdevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;

      ::cast<::gpu_vulkan::physical_device> pphysicaldevice = pdevice->m_pphysicaldevice;

     m_textureattributes.m_iMipCount = 1;

      m_textureattributes.m_etexture = ::gpu::e_texture_image;

      m_vkformat = pphysicaldevice->findSupportedFormat(
         {
            VK_FORMAT_R32G32B32A32_SFLOAT
         },
         VK_IMAGE_TILING_OPTIMAL, 
         VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);

      //int iBytesPerChannel = 4;

      //::memory memoryRgba;
      //
      //float * rgbaData = nullptr; 

      //if (channels == 3)
      //{

      //   size_t pixelCount = (size_t)width * height;

      //   memoryRgba.set_size(pixelCount * iBytesPerChannel * sizeof(float));
      //   
      //   rgbaData = (float *) memoryRgba.data();

      //   for (size_t i = 0; i < pixelCount; ++i)
      //   {
      //      
      //      rgbaData[i * 4 + 0] = imagedata[i * 3 + 0];
      //      rgbaData[i * 4 + 1] = imagedata[i * 3 + 1];
      //      rgbaData[i * 4 + 2] = imagedata[i * 3 + 2];
      //      rgbaData[i * 4 + 3] = 1.0f; // synthesized alpha

      //   }

      //   channels = 4;

      //}

      //const VkDeviceSize imageSize = VkDeviceSize(width) * height * channels * iBytesPerChannel;

      // Create image
      auto imageCreateInfo = ::vulkan::initializers::imageCreateInfo();
      imageCreateInfo.pNext = nullptr;
      imageCreateInfo.flags = 0;
      imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
      imageCreateInfo.format = m_vkformat;
      imageCreateInfo.extent = {(unsigned int)width, (unsigned int)height, 1};
      imageCreateInfo.mipLevels = 1;
      imageCreateInfo.arrayLayers = 1;
      imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
      //imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
      //imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
      imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
      imageCreateInfo.usage =
         //VK_IMAGE_USAGE_TRANSFER_SRC_BIT | 
         VK_IMAGE_USAGE_TRANSFER_DST_BIT |
         VK_IMAGE_USAGE_SAMPLED_BIT;
      //imageCreateInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
      //imageCreateInfo.queueFamilyIndexCount = 2;
      //uint32_t concurrentQueueFamilyIndices[] = {pdevice->m_queuefamilyindexes.graphicsFamily,
      //                                         pdevice->m_queuefamilyindexes.transferFamily};
      //imageCreateInfo.pQueueFamilyIndices = concurrentQueueFamilyIndices;
      imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
      imageCreateInfo.queueFamilyIndexCount = 0;
      imageCreateInfo.pQueueFamilyIndices = nullptr;
      imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

      auto &state = mip_layer_state(0, 0);
      state.m_vkimagelayout = imageCreateInfo.initialLayout;
      state.m_vkpipelinestageflags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      state.m_vkaccessflags = 0;
      
      auto logicalDevice = pcontext->logicalDevice();

      VkCheckResult(vkCreateImage(logicalDevice, &imageCreateInfo, nullptr, &m_vkimage));


      _set_image_data(imagedata, width, height, channels, 32, true);

      //VkMemoryRequirements memoryRequirements;
      //vkGetImageMemoryRequirements(pcontext->logicalDevice(), m_vkimage, &memoryRequirements);

      //VkMemoryAllocateInfo memoryAllocateInfo1 = {};
      //memoryAllocateInfo1.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      //memoryAllocateInfo1.allocationSize = memoryRequirements.size;
      //memoryAllocateInfo1.memoryTypeIndex =
      //   pphysicaldevice->findMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
      //VkCheckResult(vkAllocateMemory(pcontext->logicalDevice(), &memoryAllocateInfo1, nullptr, &m_vkdevicememory));

      //VkCheckResult(vkBindImageMemory(pcontext->logicalDevice(), m_vkimage, m_vkdevicememory, 0));

      //// Create staging buffer
      //VkBuffer stagingBuffer;
      //VkDeviceMemory stagingMemory;

      //information() << "imageSize = " << imageSize;
      //information() << "memoryRequirements.size = " << memoryRequirements.size;

      //auto bufferCreateInfo =::vulkan::initializers::bufferCreateInfo();
      //bufferCreateInfo.pNext = nullptr;
      //bufferCreateInfo.flags = 0;
      //bufferCreateInfo.size = imageSize;
      //bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
      ////bufferCreateInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
      ////bufferCreateInfo.queueFamilyIndexCount = 2;
      ////bufferCreateInfo.pQueueFamilyIndices = concurrentQueueFamilyIndices;
      //bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
      //bufferCreateInfo.queueFamilyIndexCount = 0;
      //bufferCreateInfo.pQueueFamilyIndices = nullptr;
      //VkCheckResult(vkCreateBuffer(pcontext->logicalDevice(), &bufferCreateInfo, nullptr, &stagingBuffer));

      //vkGetBufferMemoryRequirements(pcontext->logicalDevice(), stagingBuffer, &memoryRequirements);

      //VkMemoryAllocateInfo memoryAllocateInfo2 = {};
      //memoryAllocateInfo2.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      //memoryAllocateInfo2.allocationSize = memoryRequirements.size;
      //memoryAllocateInfo2.memoryTypeIndex = pphysicaldevice->findMemoryType(
      //   memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
      //VkCheckResult(vkAllocateMemory(pcontext->logicalDevice(), &memoryAllocateInfo2, nullptr, &stagingMemory));

      //VkCheckResult(vkBindBufferMemory(pcontext->logicalDevice(), stagingBuffer, stagingMemory, 0));

      //unsigned char *data = nullptr;

      //VkCheckResult(vkMapMemory(pcontext->logicalDevice(), stagingMemory, 0, imageSize, 0, (void **)&data));

      //memcpy(data, rgbaData ? rgbaData : imagedata, imageSize);

      //vkUnmapMemory(pcontext->logicalDevice(), stagingMemory);

      //::pointer<::gpu_vulkan::command_buffer> pcommandbuffer =
      //   pcontext->beginSingleTimeCommands(pcontext->m_pgpudevice->transfer_queue(),
      //      ::gpu::e_command_buffer_transfer);

      //auto vkcommandpoolTransfer = pcontext->getTransferCommandPool();

      //assert(pcommandbuffer != nullptr);
      //assert(pcommandbuffer->m_vkcommandbuffer != VK_NULL_HANDLE);
      //assert(pcommandbuffer->m_vkcommandpool == vkcommandpoolTransfer);
      //assert(pcommandbuffer->m_estate == ::gpu::command_buffer::e_state_recording);
      //assert(pcommandbuffer->m_vkcommandbufferlevel == VK_COMMAND_BUFFER_LEVEL_PRIMARY);

      //_set_state(pcommandbuffer, 
      //   {
      //      VK_ACCESS_TRANSFER_WRITE_BIT,
      //      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      //      VK_PIPELINE_STAGE_TRANSFER_BIT
      //   });

      //// Copy buffer -> image
      //VkBufferImageCopy region{};
      //region.bufferOffset = 0;
      //region.bufferRowLength = 0; // tightly packed
      //region.bufferImageHeight = 0;
      //region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      //region.imageSubresource.mipLevel = 0;
      //region.imageSubresource.baseArrayLayer = 0;
      //region.imageSubresource.layerCount = 1;
      //region.imageOffset = {0, 0, 0};
      //region.imageExtent = {(uint32_t)width, (uint32_t)height, 1};

      //vkCmdCopyBufferToImage(pcommandbuffer->m_vkcommandbuffer,
      //   stagingBuffer, m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1,
      //                       &region);

      //_set_state(pcommandbuffer,
      //   {
      //      VK_ACCESS_SHADER_READ_BIT, 
      //      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 
      //      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT});

      //
      //pcontext->endSingleTimeCommands(pcommandbuffer);

      //////vkFreeMemory(pcontext->logicalDevice(), stagingMemory, nullptr);
      ////m_descriptor3.imageView = m_vkimageview;
      ////m_descriptor3.sampler = m_vksampler3;
      ////m_descriptor3.imageLayout = m_state.m_vkimagelayout;


      //// Create image view
      //VkImageViewCreateInfo imageViewCreateInfo = {};
      //imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      //imageViewCreateInfo.pNext = nullptr;
      //imageViewCreateInfo.flags = 0;
      //imageViewCreateInfo.image = m_vkimage;
      //imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      //imageViewCreateInfo.format = m_vkformat;
      //imageViewCreateInfo.components = {
      //   .r = VK_COMPONENT_SWIZZLE_IDENTITY,
      //   .g = VK_COMPONENT_SWIZZLE_IDENTITY,
      //   .b = VK_COMPONENT_SWIZZLE_IDENTITY,
      //   .a = VK_COMPONENT_SWIZZLE_IDENTITY,
      //};
      //imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      //imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
      //imageViewCreateInfo.subresourceRange.levelCount =m_textureattributes.m_iMipCount;
      //imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
      //imageViewCreateInfo.subresourceRange.layerCount = 1;
      //VkCheckResult(vkCreateImageView(pcontext->logicalDevice(), &imageViewCreateInfo, nullptr, &m_vkimageview));

      //// Create sampler
      //VkSamplerCreateInfo samplerCreateInfo = {};
      //samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
      //samplerCreateInfo.pNext = nullptr;
      //samplerCreateInfo.flags = 0;
      //samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
      //samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
      //samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
      //samplerCreateInfo.mipLodBias = 0.0f;
      //samplerCreateInfo.minLod = 0.0f;
      //samplerCreateInfo.maxLod = (float)(m_textureattributes.m_iMipCount);
      //samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
      //samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
      //samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
      //samplerCreateInfo.anisotropyEnable =
      //   pphysicaldevice->m_vkphysicaldevicefeatures.samplerAnisotropy ? VK_TRUE : VK_FALSE;
      //samplerCreateInfo.maxAnisotropy = samplerCreateInfo.anisotropyEnable
      //                                     ? pphysicaldevice->m_vkphysicaldeviceproperties.limits.maxSamplerAnisotropy
      //                                     : 1.0f;
      //samplerCreateInfo.compareEnable = VK_FALSE;
      //samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
      //samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
      //samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
      //VkCheckResult(vkCreateSampler(pcontext->logicalDevice(), &samplerCreateInfo, nullptr, &m_vksampler3));

      //vkDestroyBuffer(pcontext->logicalDevice(), stagingBuffer, nullptr);
      //vkFreeMemory(pcontext->logicalDevice(), stagingMemory, nullptr);
      stbi_image_free(imagedata);

   }


//   void texture::load_Cubemap(const ::file::path & path)
//   {
//      //RenderSystem &renderSystem = RenderSystem::instance();
//      ///TextureManager &textureManager = TextureManager::instance();
//      ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;
//      ::cast<::gpu_vulkan::device> pgpudevice = pcontext->m_pgpudevice;
//      ::cast<::gpu_vulkan::renderer> prenderer = m_pgpurenderer;
//      auto pphysicaldevice = pgpudevice->m_pphysicaldevice;
//
//      VkImageFormatProperties formatProperties;
//      assert(("[ERROR] Unsupported texture format",
//              !vkGetPhysicalDeviceImageFormatProperties(
//                 pphysicaldevice->m_vkphysicaldevice, m_vkformat, VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL,
//                 VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 0,
//                 &formatProperties)));
//
//
//#pragma region Create cubemap resources
//      stbi_set_flip_vertically_on_load(true);
//
//            m_vkformat = VK_FORMAT_R16G16B16A16_SFLOAT;
//      stb_load
//
//      //void *textureData[6];
//      //// Load images
//      //int width, height, channels;
//      //bool hdr = m_vkformat == VK_FORMAT_R16_SFLOAT || m_vkformat == VK_FORMAT_R16G16_SFLOAT ||
//      //           m_vkformat == VK_FORMAT_R16G16B16_SFLOAT ||
//      //           m_vkformat == VK_FORMAT_R16G16B16A16_SFLOAT || m_vkformat == VK_FORMAT_R32_SFLOAT ||
//      //           m_vkformat == VK_FORMAT_R32G32_SFLOAT || m_vkformat == VK_FORMAT_R32G32B32_SFLOAT ||
//      //           m_vkformat == VK_FORMAT_R32G32B32A32_SFLOAT;
//      //if (hdr)
//      //{
//      //   if (formatInfo.bytesPerChannel == 4)
//      //   {
//      //      for (unsigned int i = 0; i < 6; i++)
//      //      {
//      //         textureData[i] =
//      //            stbi_loadf(cubemapInfo.directories[i].c_str(), &width, &height, &channels, formatInfo.nChannels);
//      //      }
//      //   }
//      //   else if (formatInfo.bytesPerChannel == 2)
//      //   {
//      //      for (unsigned int i = 0; i < 6; i++)
//      //      {
//      //         float *data =
//      //            stbi_loadf(cubemapInfo.directories[i].c_str(), &width, &height, &channels, formatInfo.nChannels);
//      //         unsigned long long dataSize = width * height * formatInfo.nChannels;
//
//      //         textureData[i] = new float16[dataSize];
//      //         for (unsigned long long j = 0; j < dataSize; j++)
//      //         {
//      //            ((float16 *)textureData[i])[j] = floatToFloat16(data[j]);
//      //         }
//      //         stbi_image_free((void *)data);
//      //      }
//      //   }
//      //}
//      //else
//      //{
//      //   for (unsigned int i = 0; i < 6; i++)
//      //   {
//      //      textureData[i] =
//      //         stbi_load(cubemapInfo.directories[i].c_str(), &width, &height, &channels, formatInfo.nChannels);
//      //   }
//      //}
//
//      const VkDeviceSize imageSize =
//         6 * VkDeviceSize(width) * height * formatInfo.nChannels * formatInfo.bytesPerChannel;
//      unsigned int nMips = unsigned int(std::floor(std::log2(width > height ? width : height))) + 1;
//
//      assert(("[ERROR] Unsupported texture format",
//              formatProperties.maxExtent.width >= width && formatProperties.maxExtent.height >= height &&
//                 formatProperties.maxExtent.depth >= 1 && formatProperties.maxMipLevels >= 1 &&
//                 formatProperties.maxArrayLayers >= 1 && formatProperties.sampleCounts & VK_SAMPLE_COUNT_1_BIT &&
//                 formatProperties.maxResourceSize >= imageSize));
//
//      // Create image
//      VkImageCreateInfo imageCreateInfo = {};
//      imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
//      imageCreateInfo.pNext = nullptr;
//      imageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
//      imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
//      imageCreateInfo.format = m_vkformat;
//      imageCreateInfo.extent = {unsigned int(width), unsigned int(height), 1};
//      imageCreateInfo.mipLevels = nMips;
//      imageCreateInfo.arrayLayers = 6;
//      imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
//      imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
//      imageCreateInfo.usage =
//         VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
//      imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
//      imageCreateInfo.queueFamilyIndexCount = 0;
//      imageCreateInfo.pQueueFamilyIndices = nullptr;
//      imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//      VkResult result = vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &m_vkimage);
//      validateResult(result);
//
//      VkMemoryRequirements memoryRequirements;
//      vkGetImageMemoryRequirements(pcontext->logicalDevice(), m_vkimage, &memoryRequirements);
//
//      VkMemoryAllocateInfo memoryAllocateInfo = {};
//      memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//      memoryAllocateInfo.allocationSize = memoryRequirements.size;
//      memoryAllocateInfo.memoryTypeIndex =
//         memoryTypeFromProperties(renderSystem.mPhysicalDeviceMemoryProperties, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
//      result = vkAllocateMemory(pcontext->logicalDevice(), &memoryAllocateInfo, nullptr, &m_vkdevicememory);
//      validateResult(result);
//
//      result = vkBindImageMemory(pcontext->logicalDevice(), m_vkimage, m_vkdevicememory, 0);
//      validateResult(result);
//
//      // Create staging buffer
//      VkBuffer stagingBuffer;
//      VkDeviceMemory stagingMemory;
//
//      VkBufferCreateInfo bufferCreateInfo = {};
//      bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//      bufferCreateInfo.pNext = nullptr;
//      bufferCreateInfo.flags = 0;
//      bufferCreateInfo.size = imageSize;
//      bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
//      bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
//      bufferCreateInfo.queueFamilyIndexCount = 0;
//      bufferCreateInfo.pQueueFamilyIndices = nullptr;
//      result = vkCreateBuffer(pcontext->logicalDevice(), &bufferCreateInfo, nullptr, &stagingBuffer);
//      validateResult(result);
//
//      vkGetBufferMemoryRequirements(pcontext->logicalDevice(), stagingBuffer, &memoryRequirements);
//
//      memoryAllocateInfo.allocationSize = memoryRequirements.size;
//      memoryAllocateInfo.memoryTypeIndex =
//         memoryTypeFromProperties(renderSystem.mPhysicalDeviceMemoryProperties,
//                                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
//      result = vkAllocateMemory(pcontext->logicalDevice(), &memoryAllocateInfo, nullptr, &stagingMemory);
//      validateResult(result);
//
//      result = vkBindBufferMemory(pcontext->logicalDevice(), stagingBuffer, stagingMemory, 0);
//      validateResult(result);
//
//      unsigned char *data;
//      result = vkMapMemory(pcontext->logicalDevice(), stagingMemory, 0, imageSize, 0, (void **)&data);
//      validateResult(result);
//
//      unsigned long long dataLayer =
//         unsigned long long(width) * height * formatInfo.nChannels * formatInfo.bytesPerChannel;
//      for (unsigned int i = 0; i < 6; i++)
//      {
//         memcpy((void *)(data + i * dataLayer), textureData[i], dataLayer);
//         stbi_image_free(textureData[i]);
//      }
//
//      vkUnmapMemory(pcontext->logicalDevice(), stagingMemory);
//
//      result = vkBeginCommandBuffer(textureManager.mCommandBuffer, &renderSystem.mCommandBufferBeginInfo);
//      validateResult(result);
//
//      VkImageMemoryBarrier barrier = {};
//      barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
//      barrier.pNext = nullptr;
//      barrier.srcAccessMask = 0;
//      barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT /* Additional >> */ | VK_ACCESS_TRANSFER_READ_BIT;
//      barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//      barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
//      barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
//      barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
//      barrier.image = m_vkimage;
//      barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//      barrier.subresourceRange.baseMipLevel = 0;
//      barrier.subresourceRange.levelCount = nMips;
//      barrier.subresourceRange.baseArrayLayer = 0;
//      barrier.subresourceRange.layerCount = 6;
//      vkCmdPipelineBarrier(textureManager.mCommandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
//                           VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
//
//      VkBufferImageCopy copyRegion = {};
//      copyRegion.bufferOffset = 0;
//      copyRegion.bufferRowLength = 0;
//      copyRegion.bufferImageHeight = 0;
//      copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//      copyRegion.imageSubresource.mipLevel = 0;
//      copyRegion.imageSubresource.baseArrayLayer = 0;
//      copyRegion.imageSubresource.layerCount = 6;
//      copyRegion.imageOffset = {0, 0, 0};
//      copyRegion.imageExtent = {unsigned int(width), unsigned int(height), 1};
//      vkCmdCopyBufferToImage(textureManager.mCommandBuffer, stagingBuffer, m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
//                             1, &copyRegion);
//
//      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
//      barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
//      barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
//      barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
//      barrier.subresourceRange.levelCount = 1;
//
//      VkImageBlit imageBlit = {};
//      imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//      imageBlit.srcSubresource.baseArrayLayer = 0;
//      imageBlit.srcSubresource.layerCount = 6;
//      imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//      imageBlit.dstSubresource.baseArrayLayer = 0;
//      imageBlit.dstSubresource.layerCount = 6;
//
//      unsigned int mipWidth = width, mipHeight = height;
//      for (unsigned int i = 1; i < nMips; i++)
//      {
//         barrier.subresourceRange.baseMipLevel = i - 1;
//         vkCmdPipelineBarrier(textureManager.mCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
//                              VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
//
//         imageBlit.srcSubresource.mipLevel = i - 1;
//         imageBlit.srcOffsets[0] = {0, 0, 0};
//         imageBlit.srcOffsets[1] = {int(mipWidth), int(mipHeight), 1};
//
//         imageBlit.dstSubresource.mipLevel = i;
//         if (mipWidth > 1)
//            mipWidth /= 2;
//         if (mipHeight > 1)
//            mipHeight /= 2;
//         imageBlit.dstOffsets[0] = {0, 0, 0};
//         imageBlit.dstOffsets[1] = {int(mipWidth), int(mipHeight), 1};
//
//         vkCmdBlitImage(textureManager.mCommandBuffer, m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_vkimage,
//                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageBlit, VK_FILTER_LINEAR);
//      }
//
//      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
//      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
//      barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//      for (unsigned int i = 0; i < nMips; i++)
//      {
//         barrier.oldLayout =
//            i == nMips - 1 ? VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL : VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
//         barrier.subresourceRange.baseMipLevel = i;
//         vkCmdPipelineBarrier(textureManager.mCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
//                              VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
//      }
//
//      result = vkEndCommandBuffer(textureManager.mCommandBuffer);
//      validateResult(result);
//
//      VkSubmitInfo submitInfo = {};
//      submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
//      submitInfo.pNext = nullptr;
//      submitInfo.waitSemaphoreCount = 0;
//      submitInfo.pWaitSemaphores = nullptr;
//      submitInfo.pWaitDstStageMask = nullptr;
//      submitInfo.commandBufferCount = 1;
//      submitInfo.pCommandBuffers = &textureManager.mCommandBuffer;
//      submitInfo.signalSemaphoreCount = 0;
//      submitInfo.pSignalSemaphores = nullptr;
//      result = vkQueueSubmit(renderSystem.mGraphicsQueue, 1, &submitInfo, NULL);
//      validateResult(result);
//
//      // Create image view
//      VkImageViewCreateInfo imageViewCreateInfo = {};
//      imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
//      imageViewCreateInfo.pNext = nullptr;
//      imageViewCreateInfo.flags = 0;
//      imageViewCreateInfo.image = m_vkimage;
//      imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
//      imageViewCreateInfo.format = m_vkformat;
//      imageViewCreateInfo.components = formatInfo.componentMapping;
//      imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//      imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
//      imageViewCreateInfo.subresourceRange.levelCount = nMips;
//      imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
//      imageViewCreateInfo.subresourceRange.layerCount = 6;
//      result = vkCreateImageView(pcontext->logicalDevice(), &imageViewCreateInfo, nullptr, &m_vkimageView);
//      validateResult(result);
//
//      // Create sampler
//      VkSamplerCreateInfo samplerCreateInfo = {};
//      samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
//      samplerCreateInfo.pNext = nullptr;
//      samplerCreateInfo.flags = 0;
//      samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
//      samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
//      samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
//      samplerCreateInfo.mipLodBias = 0.0f;
//      samplerCreateInfo.minLod = 0.0f;
//      samplerCreateInfo.maxLod = float(nMips);
//      samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
//      ;
//      samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
//      samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
//      samplerCreateInfo.anisotropyEnable = VK_TRUE;
//      samplerCreateInfo.maxAnisotropy = renderSystem.mPhysicalDeviceProperties.limits.maxSamplerAnisotropy;
//      samplerCreateInfo.compareEnable = VK_FALSE;
//      samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
//      samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
//      samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
//      result = vkCreateSampler(pcontext->logicalDevice(), &samplerCreateInfo, nullptr, &mSampler);
//      validateResult(result);
//
//      result = vkQueueWaitIdle(renderSystem.mGraphicsQueue);
//      validateResult(result);
//      result = vkResetCommandBuffer(textureManager.mCommandBuffer, 0);
//      validateResult(result);
//
//      vkDestroyBuffer(pcontext->logicalDevice(), stagingBuffer, nullptr);
//      vkFreeMemory(pcontext->logicalDevice(), stagingMemory, nullptr);
//#pragma endregion
//   }


   void texture::initialize_texture_from_file_path(::gpu::renderer *pgpurenderer, const ::file::path &pathImage, bool isSrgb)
   {

      this->m_pgpurenderer = pgpurenderer;

      if (pathImage.case_insensitive_ends(".ktx"))
      {

         pgpurenderer->m_pgpucontext->load_ktx_texture_from_file_path(this, pathImage);

      }
      else
      {

         auto pimage = image()->get_image(pathImage);

         if (::is_ok(pimage))
         {

            ::pointer_array<::image::image> imagea;

            imagea.add(pimage);

            initialize_texture_from_image(m_pgpurenderer, imagea);

         }

      }

   }

//    void texture::_fromglTfImage(tinygltf::Image *pgltfimage, const ::file::path & path,
//                                ::gpu::renderer * pgpurenderer, bool isSrgb)
//    {
//       this->m_pgpurenderer = pgpurenderer;
//       ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;
//       ::cast<::gpu_vulkan::device> pgpudevice = pcontext->m_pgpudevice;
//       ::cast<::gpu_vulkan::renderer> prenderer = pgpurenderer;
//       auto pphysicaldevice = pgpudevice->m_pphysicaldevice;
//
//       bool isKtx = false;
//       // Image points to an external ktx file
//       if (pgltfimage && ::string(pgltfimage->uri.c_str()).case_insensitive_ends(".ktx"))
//       {
//          isKtx = true;
//       }
//
//       ::cast<::gpu_vulkan::queue> pgpuqueueTransfer = pgpudevice->m_pqueueTransfer;
//
//       VkQueue copyQueue = pgpuqueueTransfer->m_vkqueue;
//       //VkFormat format;
//
//       if (!isKtx && pgltfimage)
//       {
//          // Texture was loaded using STB_Image
//
//          unsigned char *buffer = nullptr;
//          VkDeviceSize bufferSize = 0;
//          bool deleteBuffer = false;
//          if (pgltfimage->component == 3)
//          {
//             // Most devices don't support RGB only on Vulkan so convert if necessary
//             // TODO: Check actual format support and transform only if required
//             bufferSize = pgltfimage->width * pgltfimage->height * 4;
//             buffer = new unsigned char[bufferSize];
//             unsigned char *rgba = buffer;
//             unsigned char *rgb = &pgltfimage->image[0];
//             for (size_t i = 0; i < pgltfimage->width * pgltfimage->height; ++i)
//             {
//                for (int32_t j = 0; j < 3; ++j)
//                {
//                   rgba[j] = rgb[j];
//                }
//                rgba += 4;
//                rgb += 3;
//             }
//             deleteBuffer = true;
//          }
//          else
//          {
//             buffer = &pgltfimage->image[0];
//             bufferSize = pgltfimage->image.size();
//          }
//          int iSize = pgltfimage->width * pgltfimage->height * 4;
//          for (size_t i = 0; i < iSize; i+=4)
//          {
//             auto t = buffer[i];
//             buffer[i] = buffer[i+2];
//             buffer[i + 2] = t;
//          }
//          int h = pgltfimage->height;
//          int halfh = h / 2;
//          ::memory memoryLine;
//          memoryLine.set_size(pgltfimage->width * 4);
//          auto p = buffer;
//          for (size_t y = 0; y < halfh; y++)
//          {
//             memcpy(memoryLine.data(), p + y * pgltfimage->width * 4, memoryLine.size());
//             memcpy(p + y * pgltfimage->width * 4, p + (h - 1-y) * pgltfimage->width * 4, memoryLine.size());
//             memcpy(p + (h - 1 - y) * pgltfimage->width * 4, memoryLine.data(), memoryLine.size());
//          }
//
//          m_vkformat = isSrgb ? VK_FORMAT_R8G8B8A8_SRGB : VK_FORMAT_R8G8B8A8_UNORM;
//
//          VkFormatProperties formatProperties;
//
//
//          m_textureattributes.m_rectangleTarget.set_width(pgltfimage->width);
//         m_textureattributes. m_rectangleTarget.set_height(pgltfimage->height);
//         m_textureattributes. m_iMipCount = (uint32_t)(floor(::log2((double)::maximum(m_textureattributes.m_rectangleTarget.width(),
//             m_textureattributes.m_rectangleTarget.height()))) + 1.0);
//
//          vkGetPhysicalDeviceFormatProperties(pphysicaldevice->m_vkphysicaldevice, m_vkformat, &formatProperties);
//          assert(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT);
//          assert(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT);
//
//          VkMemoryAllocateInfo memAllocInfo{};
//          memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//          VkMemoryRequirements memReqs{};
//
//          VkBuffer stagingBuffer;
//          VkDeviceMemory stagingMemory;
//
//          VkBufferCreateInfo bufferCreateInfo{};
//          bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//          bufferCreateInfo.size = bufferSize;
//          bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
//          bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
//          VkCheckResult(vkCreateBuffer(pcontext->logicalDevice(), &bufferCreateInfo, nullptr, &stagingBuffer));
//          vkGetBufferMemoryRequirements(pcontext->logicalDevice(), stagingBuffer, &memReqs);
//          memAllocInfo.allocationSize = memReqs.size;
//          memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(
//             memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
//          VkCheckResult(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &stagingMemory));
//          VkCheckResult(vkBindBufferMemory(pcontext->logicalDevice(), stagingBuffer, stagingMemory, 0));
//
//          uint8_t *data;
//          VkCheckResult(vkMapMemory(pcontext->logicalDevice(), stagingMemory, 0, memReqs.size, 0, (void **)&data));
//          memcpy(data, buffer, bufferSize);
//          vkUnmapMemory(pcontext->logicalDevice(), stagingMemory);
//
//          VkImageCreateInfo imageCreateInfo{};
//          imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
//          imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
//          imageCreateInfo.format = m_vkformat;
//          imageCreateInfo.mipLevels = m_textureattributes.m_iMipCount;
//          imageCreateInfo.arrayLayers = 1;
//          imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
//          imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
//          imageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
//          imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
//          imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//          imageCreateInfo.extent = {(uint32_t)m_textureattributes. m_rectangleTarget.width(), (uint32_t)m_textureattributes.m_rectangleTarget.height(), 1};
//          imageCreateInfo.usage =
//             VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
//          VkCheckResult(vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &m_vkimage));
//          vkGetImageMemoryRequirements(pcontext->logicalDevice(), m_vkimage, &memReqs);
//          memAllocInfo.allocationSize = memReqs.size;
//          memAllocInfo.memoryTypeIndex =
//             pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
//          VkCheckResult(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &m_vkdevicememory));
//          VkCheckResult(vkBindImageMemory(pcontext->logicalDevice(), m_vkimage, m_vkdevicememory, 0));
//
//          // VkCommandBuffer pcommandbufferCopy->m_vkcommandbuffer =
//          // pcontext->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
//
//          auto pgpucommandbufferCopy = pcontext->beginSingleTimeCommands(pcontext->m_pgpudevice->transfer_queue());
//
//          ::cast<command_buffer> pcommandbufferCopy = pgpucommandbufferCopy;
//
//          VkImageSubresourceRange subresourceRange = {};
//          subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//          subresourceRange.levelCount = 1;
//          subresourceRange.layerCount = 1;
//
//          VkImageMemoryBarrier imageMemoryBarrier{};
//
//          imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
//          imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//          imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
//          imageMemoryBarrier.srcAccessMask = 0;
//          imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
//          imageMemoryBarrier.image = m_vkimage;
//          imageMemoryBarrier.subresourceRange = subresourceRange;
//          vkCmdPipelineBarrier(pcommandbufferCopy->m_vkcommandbuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
//                               VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
//
//          VkBufferImageCopy bufferCopyRegion = {};
//          bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//          bufferCopyRegion.imageSubresource.mipLevel = 0;
//          bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
//          bufferCopyRegion.imageSubresource.layerCount = 1;
//          bufferCopyRegion.imageExtent.width =m_textureattributes. m_rectangleTarget.width();
//          bufferCopyRegion.imageExtent.height = m_textureattributes.m_rectangleTarget.height();
//          bufferCopyRegion.imageExtent.depth = 1;
//
//          vkCmdCopyBufferToImage(pcommandbufferCopy->m_vkcommandbuffer, stagingBuffer, m_vkimage,
//                                 VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferCopyRegion);
//
//          imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
//          imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
//          imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
//          imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
//          imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
//          imageMemoryBarrier.image = m_vkimage;
//          imageMemoryBarrier.subresourceRange = subresourceRange;
//          vkCmdPipelineBarrier(pcommandbufferCopy->m_vkcommandbuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
//                               VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
//
//          pcontext->endSingleTimeCommands(pcommandbufferCopy);
//
//          vkDestroyBuffer(pcontext->logicalDevice(), stagingBuffer, nullptr);
//          vkFreeMemory(pcontext->logicalDevice(), stagingMemory, nullptr);
//
//          // Generate the mip chain (glTF uses jpg and png, so we need to create this manually)
//          // VkCommandBuffer blitCmd = device->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
//          auto pgpucommandbufferBlit = pcontext->beginSingleTimeCommands(pcontext->m_pgpudevice->transfer_queue());
//          ::cast<command_buffer> pcommandbufferBlit = pgpucommandbufferBlit;
//          for (uint32_t i = 1; i < m_textureattributes.m_iMipCount; i++)
//          {
//             VkImageBlit imageBlit{};
//
//             imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//             imageBlit.srcSubresource.layerCount = 1;
//             imageBlit.srcSubresource.mipLevel = i - 1;
//             imageBlit.srcOffsets[1].x = int32_t(m_textureattributes.m_rectangleTarget.width() >> (i - 1));
//             imageBlit.srcOffsets[1].y = int32_t(m_textureattributes.m_rectangleTarget.height() >> (i - 1));
//             imageBlit.srcOffsets[1].z = 1;
//
//             imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//             imageBlit.dstSubresource.layerCount = 1;
//             imageBlit.dstSubresource.mipLevel = i;
//             imageBlit.dstOffsets[1].x = int32_t(m_textureattributes.m_rectangleTarget.width() >> i);
//             imageBlit.dstOffsets[1].y = int32_t(m_textureattributes.m_rectangleTarget.height() >> i);
//             imageBlit.dstOffsets[1].z = 1;
//
//             VkImageSubresourceRange mipSubRange = {};
//             mipSubRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//             mipSubRange.baseMipLevel = i;
//             mipSubRange.levelCount = 1;
//             mipSubRange.layerCount = 1;
//
//             {
//                VkImageMemoryBarrier imageMemoryBarrier{};
//                imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
//                imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//                imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
//                imageMemoryBarrier.srcAccessMask = 0;
//                imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
//                imageMemoryBarrier.image = m_vkimage;
//                imageMemoryBarrier.subresourceRange = mipSubRange;
//                vkCmdPipelineBarrier(pcommandbufferBlit->m_vkcommandbuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
//                                     VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
//             }
//
//             vkCmdBlitImage(pcommandbufferBlit->m_vkcommandbuffer, m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_vkimage,
//                            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageBlit, VK_FILTER_LINEAR);
//
//             {
//                VkImageMemoryBarrier imageMemoryBarrier{};
//                imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
//                imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
//                imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
//                imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
//                imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
//                imageMemoryBarrier.image = m_vkimage;
//                imageMemoryBarrier.subresourceRange = mipSubRange;
//                vkCmdPipelineBarrier(pcommandbufferBlit->m_vkcommandbuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
//                                     VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
//             }
//          }
//
//          subresourceRange.levelCount = m_textureattributes.m_iMipCount;
//          mip_layer_state(0,0).m_vkimagelayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//
//          imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
//          imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
//          imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//          imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
//          imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
//          imageMemoryBarrier.image = m_vkimage;
//          imageMemoryBarrier.subresourceRange = subresourceRange;
//          vkCmdPipelineBarrier(pcommandbufferBlit->m_vkcommandbuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
//                               VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
//
//          if (deleteBuffer)
//          {
//             delete[] buffer;
//          }
//
//          // pcontext->endSingleTimeCommands(blitCmd, copyQueue, true);
//
//          pcontext->endSingleTimeCommands(pcommandbufferBlit);
//       }
//       else
//       {
//          // Texture is stored in an external ktx file
//          ::file::path filename = path;
//
//          if (pgltfimage)
//          {
//
//             filename /= pgltfimage->uri.c_str();
//
//          }
//
//          ktxTexture *pktxtexture;
//
//          ktxResult result = KTX_SUCCESS;
// #if defined(__ANDROID__)
//          AAsset *asset =
//             AAssetManager_open(androidApp->activity->assetManager, filename.c_str(), AASSET_MODE_STREAMING);
//          if (!asset)
//          {
//             vks::tools::exitFatal("Could not load texture from " + filename +
//                                      "\n\nMake sure the assets submodule has been checked out and is up-to-date.",
//                                   -1);
//          }
//          size_t size = AAsset_getLength(asset);
//          assert(size > 0);
//          ktx_uint8_t *textureData = new ktx_uint8_t[size];
//          AAsset_read(asset, textureData, size);
//          AAsset_close(asset);
//          result = ktxTexture_CreateFromMemory(textureData, size, KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &pktxtexture);
//          delete[] textureData;
// #else
//          // if (!tools::fileExists(filename))
//          //{
//          //    tools::exitFatal("Could not load texture from " + filename +
//          //                        "\n\nMake sure the assets submodule has been checked out and is up-to-date.",
//          //                     -1);
//          // }
//          if (!pcontext->file()->exists(filename))
//          {
//             throw ::file::exception(
//                error_file_not_found,
//                filename,
//                ::file::e_open_none,
//                "Could not load texture from " + ::string(filename) +
//                "\n\nMake sure the assets submodule has been checked out and is up-to-date.");
//          }
//
//          auto memory = pcontext->file()->as_memory(filename);
//          static ::memory m1;
//
//          if (m1.is_empty())
//          {
//
//             m1 = memory;
//          }
//          else
//          {
//
//             if (m1.compare(memory) == 0)
//             {
//
//                information("what?!?!");
//
//             }
//
//          }
//          result = ktxTexture_CreateFromMemory(memory.data(), memory.size(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT,
//                                               &pktxtexture);
//          // delete[] textureData;
//
//          // result = ktxTexture_CreateFromNamedFile(filename.c_str(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT,
//          // &ktxTexture);
// #endif
//          assert(result == KTX_SUCCESS);
//
//
//
//
//          this->m_pgpurenderer = pcontext->m_pgpurenderer;
//
//          m_textureattributes.m_rectangleTarget.set_width(pktxtexture->baseWidth);
//          m_textureattributes.m_rectangleTarget.set_height(pktxtexture->baseHeight);
//          m_textureattributes.m_iMipCount = pktxtexture->numLevels;
//
//          ktx_uint8_t *ktxTextureData = ktxTexture_GetData(pktxtexture);
//          ktx_size_t ktxTextureSize = ktxTexture_GetDataSize(pktxtexture);
//          m_vkformat = ktxTexture_GetVkFormat(pktxtexture);
//
//          // Get device properties for the requested texture format
//          VkFormatProperties formatProperties;
//          vkGetPhysicalDeviceFormatProperties(pphysicaldevice->m_vkphysicaldevice, m_vkformat, &formatProperties);
//
//          // VkCommandBuffer pcommandbufferCopy->m_vkcommandbuffer =
//          // device->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
//
//          auto pgpucommandbufferCopy = pcontext->beginSingleTimeCommands(pcontext->m_pgpudevice->transfer_queue());
//
//          ::cast<command_buffer> pcommandbufferCopy = pgpucommandbufferCopy;
//
//          VkBuffer stagingBuffer;
//          VkDeviceMemory stagingMemory;
//
//          VkBufferCreateInfo bufferCreateInfo = vkinit::bufferCreateInfo();
//          bufferCreateInfo.size = ktxTextureSize;
//          // This buffer is used as a transfer source for the buffer copy
//          bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
//          bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
//          VkCheckResult(vkCreateBuffer(pcontext->logicalDevice(), &bufferCreateInfo, nullptr, &stagingBuffer));
//
//          VkMemoryAllocateInfo memAllocInfo = vkinit::memoryAllocateInfo();
//          VkMemoryRequirements memReqs;
//          vkGetBufferMemoryRequirements(pcontext->logicalDevice(), stagingBuffer, &memReqs);
//          memAllocInfo.allocationSize = memReqs.size;
//          memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(
//             memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
//          VkCheckResult(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &stagingMemory));
//          VkCheckResult(vkBindBufferMemory(pcontext->logicalDevice(), stagingBuffer, stagingMemory, 0));
//
//          uint8_t *data;
//          VkCheckResult(vkMapMemory(pcontext->logicalDevice(), stagingMemory, 0, memReqs.size, 0, (void **)&data));
//          memcpy(data, ktxTextureData, ktxTextureSize);
//          vkUnmapMemory(pcontext->logicalDevice(), stagingMemory);
//
//          ::array_base<VkBufferImageCopy> bufferCopyRegions;
//          for (uint32_t i = 0; i < m_textureattributes.m_iMipCount; i++)
//          {
//             ktx_size_t offset;
//             KTX_error_code result = ktxTexture_GetImageOffset(pktxtexture, i, 0, 0, &offset);
//             assert(result == KTX_SUCCESS);
//             VkBufferImageCopy bufferCopyRegion = {};
//             bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//             bufferCopyRegion.imageSubresource.mipLevel = i;
//             bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
//             bufferCopyRegion.imageSubresource.layerCount = 1;
//             bufferCopyRegion.imageExtent.width = std::max(1u, pktxtexture->baseWidth >> i);
//             bufferCopyRegion.imageExtent.height = std::max(1u, pktxtexture->baseHeight >> i);
//             bufferCopyRegion.imageExtent.depth = 1;
//             bufferCopyRegion.bufferOffset = offset;
//             bufferCopyRegions.add(bufferCopyRegion);
//          }
//
//          // Create optimal tiled target image
//          VkImageCreateInfo imageCreateInfo = vkinit::imageCreateInfo();
//          imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
//          imageCreateInfo.format = m_vkformat;
//          imageCreateInfo.mipLevels = m_textureattributes.m_iMipCount;
//          imageCreateInfo.arrayLayers = 1;
//          imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
//          imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
//          imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
//          imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//          imageCreateInfo.extent.width = m_textureattributes.m_rectangleTarget.width();
//          imageCreateInfo.extent.height = m_textureattributes.m_rectangleTarget.height();
//          imageCreateInfo.extent.depth = 1;
//          imageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
//          VkCheckResult(vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &m_vkimage));
//
//          vkGetImageMemoryRequirements(pcontext->logicalDevice(), m_vkimage, &memReqs);
//          memAllocInfo.allocationSize = memReqs.size;
//          memAllocInfo.memoryTypeIndex =
//             pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
//          VkCheckResult(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &m_vkdevicememory));
//          VkCheckResult(vkBindImageMemory(pcontext->logicalDevice(), m_vkimage, m_vkdevicememory, 0));
//
//          VkImageSubresourceRange subresourceRange = {};
//          subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//          subresourceRange.baseMipLevel = 0;
//          subresourceRange.levelCount = m_textureattributes.m_iMipCount;
//          subresourceRange.layerCount = 1;
//
//          ::vulkan::setImageLayout(pcommandbufferCopy->m_vkcommandbuffer, m_vkimage, VK_IMAGE_LAYOUT_UNDEFINED,
//                                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourceRange);
//          vkCmdCopyBufferToImage(pcommandbufferCopy->m_vkcommandbuffer, stagingBuffer, m_vkimage,
//                                 VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, static_cast<uint32_t>(bufferCopyRegions.size()),
//                                 bufferCopyRegions.data());
//          ::vulkan::setImageLayout(pcommandbufferCopy->m_vkcommandbuffer, m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
//                                   VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, subresourceRange);
//          pcontext->endSingleTimeCommands(pcommandbufferCopy);
//          // pcontext->endSingleTimeCommands(pcommandbufferCopy->m_vkcommandbuffer, copyQueue);
//          mip_layer_state(0, 0).m_vkimagelayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//
//          vkDestroyBuffer(pcontext->logicalDevice(), stagingBuffer, nullptr);
//          vkFreeMemory(pcontext->logicalDevice(), stagingMemory, nullptr);
//
//          ktxTexture_Destroy(pktxtexture);
//       }
//
//       on_finish_load_texture();
//
//    }


   void texture::on_finish_load_texture()
   {
      ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;
      
      VkSamplerCreateInfo samplerInfo{};
      samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
      samplerInfo.magFilter = VK_FILTER_LINEAR;
      samplerInfo.minFilter = VK_FILTER_LINEAR;
      samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
      samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
      samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
      samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
      samplerInfo.compareOp = VK_COMPARE_OP_NEVER;
      samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
      samplerInfo.maxAnisotropy = 1.0;
      samplerInfo.anisotropyEnable = VK_FALSE;
      samplerInfo.maxLod = (float)m_textureattributes.m_iMipCount;
      samplerInfo.maxAnisotropy = 8.0f;
      // samplerInfo.anisotropyEnable = VK_TRUE;
      VkCheckResult(vkCreateSampler(pcontext->logicalDevice(), &samplerInfo, nullptr, &m_vksampler3));

      VkImageViewCreateInfo viewInfo{};
      viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      viewInfo.image = m_vkimage;
      viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      viewInfo.format = m_vkformat;
      viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      viewInfo.subresourceRange.layerCount = 1;
      viewInfo.subresourceRange.levelCount = m_textureattributes.m_iMipCount;
      VkCheckResult(vkCreateImageView(pcontext->logicalDevice(), &viewInfo, nullptr, &m_vkimageview));

      //m_descriptor3.sampler = m_vksampler3;
      //m_descriptor3.imageView = m_vkimageview;
      //m_descriptor3.imageLayout = m_state.m_vkimagelayout;

   }


   void texture::set_state(::gpu::command_buffer * pgpucommandbuffer, ::gpu::enum_texture_state etexturestate)
   {

      ::gpu::texture::set_state(pgpucommandbuffer, etexturestate);

      ::cast<::gpu_vulkan::command_buffer> pcommandbuffer = pgpucommandbuffer;

      if (etexturestate == ::gpu::e_texture_state_shader_read)
      {
         _set_state(pcommandbuffer,
                              {

                                 0,
                                 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                 VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
            }

         );
      }
      else if (etexturestate == ::gpu::e_texture_state_color_attachment)
      {

        _set_state(pcommandbuffer,
                    {

         VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
         VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                       VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT}

         );
      }


   }


   //      void generateCubemapMipmaps(VkCommandBuffer cmd, VkImage image, VkFormat format, int32_t width, int32_t height,
   //                            uint32_t mipLevels)
   //{

   //   int32_t mipWidth = width;

   //   int32_t mipHeight = height;

   //   for (uint32_t i = 1; i < mipLevels; i++)
   //   {

   //      VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
   //      barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED; // first use
   //      barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
   //      barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
   //      barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
   //      barrier.image = image;
   //      barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   //      barrier.subresourceRange.baseMipLevel = i;
   //      barrier.subresourceRange.levelCount = 1;
   //      barrier.subresourceRange.baseArrayLayer = 0;
   //      barrier.subresourceRange.layerCount = 6;
   //      barrier.srcAccessMask = 0;
   //      barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

   //      vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0,
   //                           nullptr, 1, &barrier);

   //      VkImageBlit blit{};
   //      blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   //      blit.srcSubresource.mipLevel = i - 1;
   //      blit.srcSubresource.baseArrayLayer = 0;
   //      blit.srcSubresource.layerCount = 6;
   //      blit.srcOffsets[0] = {0, 0, 0};
   //      blit.srcOffsets[1] = {mipWidth, mipHeight, 1};

   //      blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   //      blit.dstSubresource.mipLevel = i;
   //      blit.dstSubresource.baseArrayLayer = 0;
   //      blit.dstSubresource.layerCount = 6;
   //      blit.dstOffsets[0] = {0, 0, 0};
   //      blit.dstOffsets[1] = {std::max(1, mipWidth / 2), std::max(1, mipHeight / 2), 1};

   //      vkCmdBlitImage(cmd, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
   //                     1, &blit, VK_FILTER_LINEAR);

   //      // Transition this mip level to SRC for the next iteration
   //      barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
   //      barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
   //      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
   //      barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

   //      vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0,
   //                           nullptr, 1, &barrier);

   //      mipWidth = std::max(1, mipWidth / 2);
   //      mipHeight = std::max(1, mipHeight / 2);
   //   }

   //   // Finally transition all mips to SHADER_READ_ONLY_OPTIMAL
   //   VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
   //   barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
   //   barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
   //   barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
   //   barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
   //   barrier.image = image;
   //   barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   //   barrier.subresourceRange.baseMipLevel = 0;
   //   barrier.subresourceRange.levelCount = mipLevels;
   //   barrier.subresourceRange.baseArrayLayer = 0;
   //   barrier.subresourceRange.layerCount = 6;

   //   vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0,
   //                        nullptr, 1, &barrier);

   //}

   
   void texture::generate_mipmap(::gpu::command_buffer *pgpucommandbuffer)
   {

      ::cast<::gpu_vulkan::command_buffer> pcommandbuffer = pgpucommandbuffer;

      VkCommandBuffer cmd = pcommandbuffer->m_vkcommandbuffer;

      int32_t mipWidth = this->width();

      int32_t mipHeight = this->height();

      // First, transition all mips to VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
      VkImageMemoryBarrier barrierStart{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
      barrierStart.oldLayout = mip_layer_state(0, 0).m_vkimagelayout;
      barrierStart.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
      barrierStart.srcAccessMask = mip_layer_state(0, 0).m_vkaccessflags;
      barrierStart.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
      barrierStart.image = m_vkimage;
      barrierStart.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      barrierStart.subresourceRange.baseMipLevel = 0;
      barrierStart.subresourceRange.levelCount = m_textureattributes.m_iMipCount;
      barrierStart.subresourceRange.baseArrayLayer = 0;
      barrierStart.subresourceRange.layerCount = m_textureattributes.m_iLayerCount;

      vkCmdPipelineBarrier(cmd, mip_layer_state(0, 0).m_vkpipelinestageflags,
         VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0,
                           nullptr, 1, &barrierStart);


      for (uint32_t i = 1; i < m_textureattributes.m_iMipCount; i++)
      {

         VkImageMemoryBarrier barrierTransition{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
         barrierTransition.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED; // first use
         barrierTransition.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
         barrierTransition.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
         barrierTransition.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
         barrierTransition.image = m_vkimage;
         barrierTransition.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         barrierTransition.subresourceRange.baseMipLevel = i;
         barrierTransition.subresourceRange.levelCount = 1;
         barrierTransition.subresourceRange.baseArrayLayer = 0;
         barrierTransition.subresourceRange.layerCount = m_textureattributes.m_iLayerCount;
         barrierTransition.srcAccessMask = 0;
         barrierTransition.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

         vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0,
                              nullptr, 1, &barrierTransition);

         VkImageBlit blit{};
         blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         blit.srcSubresource.mipLevel = i - 1;
         blit.srcSubresource.baseArrayLayer = 0;
         blit.srcSubresource.layerCount = m_textureattributes.m_iLayerCount;
         blit.srcOffsets[0] = {0, 0, 0};
         blit.srcOffsets[1] = {mipWidth, mipHeight, 1};

         auto iNextHalfW = std::max(1, mipWidth / 2);
         auto iNextHalfH = std::max(1, mipHeight / 2);

         blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         blit.dstSubresource.mipLevel = i;
         blit.dstSubresource.baseArrayLayer = 0;
         blit.dstSubresource.layerCount = m_textureattributes.m_iLayerCount;
         blit.dstOffsets[0] = {0, 0, 0};
         blit.dstOffsets[1] = {iNextHalfW, iNextHalfH, 1};

         vkCmdBlitImage(cmd, m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        1, &blit, VK_FILTER_LINEAR);

         // Transition this mip level to SRC for the next iteration
         barrierTransition.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
         barrierTransition.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
         barrierTransition.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
         barrierTransition.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

         vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0,
                              nullptr, 1, &barrierTransition);

         mipWidth = iNextHalfW;
         mipHeight = iNextHalfH;
      }

      mip_layer_state(0, 0).m_vkaccessflags = VK_ACCESS_TRANSFER_READ_BIT;
      mip_layer_state(0, 0).m_vkimagelayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
      mip_layer_state(0, 0).m_vkpipelinestageflags = VK_PIPELINE_STAGE_TRANSFER_BIT;

      //// Finally transition all mips to SHADER_READ_ONLY_OPTIMAL
      //VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
      //barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
      //barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      //barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
      //barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
      //barrier.image = m_vkimage;
      //barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      //barrier.subresourceRange.baseMipLevel = 0;
      //barrier.subresourceRange.levelCount = m_textureattributes.m_iMipCount;
      //barrier.subresourceRange.baseArrayLayer = 0;
      //barrier.subresourceRange.layerCount = m_textureattributes.m_iLayerCount;

      //vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0,
      //                     nullptr, 1, &barrier);

   }


} // namespace gpu_vulkan

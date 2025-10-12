// From github:/tristancalderbank/OpenGL-PBR-Renderer/cubemap_framebuffer.cpp by
// camilo on 2025-09-26 19:52 <3ThomasBorregaardSorensen!!
// From gpu_opengl/ibl/cubemap_framebuffer.cpp by
// camilo on 2025-10-10 22:15 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "cubemap_framebuffer.h"
#include "bred/gpu/context_lock.h"
#include "gpu_vulkan/command_buffer.h"
#include "gpu_vulkan/context.h"
#include "gpu_vulkan/physical_device.h"
#include "gpu_vulkan/queue.h"
#include "texture.h"


namespace gpu_vulkan
{


   namespace ibl
   {


      cubemap_framebuffer::cubemap_framebuffer()
      {


      }


      cubemap_framebuffer::~cubemap_framebuffer()
      {


      }


      void cubemap_framebuffer::on_initialize_cubemap_framebuffer()
      {

         ::gpu::context_lock contextlock(m_pgpucontext);

         ::cast < gpu_vulkan::texture>ptexture = m_ptexture;

         ptexture->m_mipsLevel = (uint32_t)(floor(::log2((double)::maximum(ptexture->m_rectangleTarget.width(),
                                                                           ptexture->m_rectangleTarget.height()))) +
                                            1.0);
         ptexture->m_bTransferSrc = true;
         ptexture->initialize_image_texture(m_pgpucontext->m_pgpurenderer, ptexture->m_rectangleTarget, true,
                                            {}, ::gpu::texture::e_type_cube_map);
         // framebuffer
         //glGenFramebuffers(1, &ptexture->m_gluFbo);
         //GLCheckError("");
         //glBindFramebuffer(GL_FRAMEBUFFER, ptexture->m_gluFbo);
         //GLCheckError("");

         //// depth buffer
         //glGenRenderbuffers(1, &ptexture->m_gluDepthStencilRBO);
         //GLCheckError("");
         //ptexture->m_gluType = GL_TEXTURE_CUBE_MAP;
         //glBindRenderbuffer(GL_RENDERBUFFER, ptexture->m_gluDepthStencilRBO);
         //GLCheckError("");
         //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, ptexture->m_rectangleTarget.width(), ptexture->m_rectangleTarget.height());
         //GLCheckError("");

         //// attach the depth buffer
         //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, ptexture->m_gluDepthStencilRBO);
         //GLCheckError("");

         //// cubemap
         //glGenTextures(1, &ptexture->m_gluTextureID);
         //GLCheckError("");
         //glBindTexture(GL_TEXTURE_CUBE_MAP, ptexture->m_gluTextureID);
         //GLCheckError("");

         //// specify/allocate each face for the cubemap
         //for (auto i = 0; i < 6; i++)
         //{

         //   glTexImage2D(
         //      GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
         //      0,
         //      GL_RGB16F,
         //      ptexture->m_rectangleTarget.width(),
         //      ptexture->m_rectangleTarget.height(),
         //      0,
         //      GL_RGB,
         //      GL_FLOAT,
         //      nullptr);
         //   GLCheckError("");

         //}

         //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
         //GLCheckError("");
         //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
         //GLCheckError("");
         //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
         //GLCheckError("");
         //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         //GLCheckError("");
         //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         //GLCheckError("");

         //GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

         //if (status != GL_FRAMEBUFFER_COMPLETE)
         //{

         //   warning() << "Framebuffer incomplete! with status " << status;
         //}

         //glBindFramebuffer(GL_FRAMEBUFFER, 0);
         //GLCheckError("");

      }


      void cubemap_framebuffer::bind(::gpu::command_buffer *pgpucommandbuffer)
      {
         
         ::cast < gpu_vulkan::texture>ptexture = m_ptexture;
         ::cast<gpu_vulkan::context> pcontext = m_pgpucontext;
         pcontext->m_pshaderBound->bind(pgpucommandbuffer, ptexture);
         //glBindFramebuffer(GL_FRAMEBUFFER, ptexture->m_gluFbo);
         ///GLCheckError("");
      }
      void generateCubemapMipmaps(VkCommandBuffer cmd, VkImage image, VkFormat format, int32_t width, int32_t height,
                                  uint32_t mipLevels)
      {
         int32_t mipWidth = width;
         int32_t mipHeight = height;

         for (uint32_t i = 1; i < mipLevels; i++)
         {
            VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
            barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED; // first use
            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.image = image;
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.baseMipLevel = i;
            barrier.subresourceRange.levelCount = 1;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = 6;
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0,
                                 nullptr, 1, &barrier);

            VkImageBlit blit{};
            blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.srcSubresource.mipLevel = i - 1;
            blit.srcSubresource.baseArrayLayer = 0;
            blit.srcSubresource.layerCount = 6;
            blit.srcOffsets[0] = {0, 0, 0};
            blit.srcOffsets[1] = {mipWidth, mipHeight, 1};

            blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.dstSubresource.mipLevel = i;
            blit.dstSubresource.baseArrayLayer = 0;
            blit.dstSubresource.layerCount = 6;
            blit.dstOffsets[0] = {0, 0, 0};
            blit.dstOffsets[1] = {std::max(1, mipWidth / 2), std::max(1, mipHeight / 2), 1};

            vkCmdBlitImage(cmd, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

            // Transition this mip level to SRC for the next iteration
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0,
                                 nullptr, 1, &barrier);

            mipWidth = std::max(1, mipWidth / 2);
            mipHeight = std::max(1, mipHeight / 2);
         }

         // Finally transition all mips to SHADER_READ_ONLY_OPTIMAL
         VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
         barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
         barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
         barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
         barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
         barrier.image = image;
         barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         barrier.subresourceRange.baseMipLevel = 0;
         barrier.subresourceRange.levelCount = mipLevels;
         barrier.subresourceRange.baseArrayLayer = 0;
         barrier.subresourceRange.layerCount = 6;

         vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr,
                              0, nullptr, 1, &barrier);
      }

      void cubemap_framebuffer::generateMipmap()
      {
         ::cast < gpu_vulkan::texture>ptexture = m_ptexture;
         ::cast<gpu_vulkan::device> pdevice = m_pgpucontext->m_pgpudevice;
         ::cast<gpu_vulkan::physical_device> pphysicaldevice = pdevice->m_pphysicaldevice;
         //glBindTexture(GL_TEXTURE_CUBE_MAP, ptexture->m_gluTextureID);
         //GLCheckError("");
         //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
         //GLCheckError("");
         //glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
         //GLCheckError("");

         auto width = ptexture->m_rectangleTarget.width();

         auto height = ptexture->m_rectangleTarget.height();

         ::cast<::gpu_vulkan::context> pcontext = m_pgpucontext;

         auto pgpucommandbufferCopy = pcontext->beginSingleTimeCommands(pcontext->transfer_queue());

         ::cast<command_buffer> pcommandbufferCopy = pgpucommandbufferCopy;

         //ptexture->m_vkformat = VK_FORMAT_R16G16B16A16_SFLOAT;

         int iBytesPerChannel = 4;

         // void *textureData[6];
         //// Load images
         // int width, height, channels;
         // bool hdr = m_vkformat == VK_FORMAT_R16_SFLOAT || m_vkformat == VK_FORMAT_R16G16_SFLOAT ||
         //            m_vkformat == VK_FORMAT_R16G16B16_SFLOAT ||
         //            m_vkformat == VK_FORMAT_R16G16B16A16_SFLOAT || m_vkformat == VK_FORMAT_R32_SFLOAT ||
         //            m_vkformat == VK_FORMAT_R32G32_SFLOAT || m_vkformat == VK_FORMAT_R32G32B32_SFLOAT ||
         //            m_vkformat == VK_FORMAT_R32G32B32A32_SFLOAT;
         // if (hdr)
         //{
         //    if (formatInfo.bytesPerChannel == 4)
         //    {
         //       for (unsigned int i = 0; i < 6; i++)
         //       {
         //          textureData[i] =
         //             stbi_loadf(cubemapInfo.directories[i].c_str(), &width, &height, &channels,
         //             formatInfo.nChannels);
         //       }
         //    }
         //    else if (formatInfo.bytesPerChannel == 2)
         //    {
         //       for (unsigned int i = 0; i < 6; i++)
         //       {
         //          float *data =
         //             stbi_loadf(cubemapInfo.directories[i].c_str(), &width, &height, &channels,
         //             formatInfo.nChannels);
         //          unsigned long long dataSize = width * height * formatInfo.nChannels;

         //         textureData[i] = new float16[dataSize];
         //         for (unsigned long long j = 0; j < dataSize; j++)
         //         {
         //            ((float16 *)textureData[i])[j] = floatToFloat16(data[j]);
         //         }
         //         stbi_image_free((void *)data);
         //      }
         //   }
         //}
         // else
         //{
         //   for (unsigned int i = 0; i < 6; i++)
         //   {
         //      textureData[i] =
         //         stbi_load(cubemapInfo.directories[i].c_str(), &width, &height, &channels, formatInfo.nChannels);
         //   }
         //}

         //const VkDeviceSize imageSize = VkDeviceSize(width) * height * 4 * iBytesPerChannel;
         //// unsigned int nMips = unsigned int(std::floor(std::log2(width > height ? width : height))) + 1;

         //// assert(("[ERROR] Unsupported texture format",
         ////         formatProperties.maxExtent.width >= width && formatProperties.maxExtent.height >= height &&
         ////            formatProperties.maxExtent.depth >= 1 && formatProperties.maxMipLevels >= 1 &&
         ////            formatProperties.maxArrayLayers >= 1 && formatProperties.sampleCounts & VK_SAMPLE_COUNT_1_BIT &&
         ////            formatProperties.maxResourceSize >= imageSize));
         //int channels = 4;
         //// Create image
         //VkImageCreateInfo imageCreateInfo = {};
         //imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
         //imageCreateInfo.pNext = nullptr;
         //imageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
         //imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
         //imageCreateInfo.format = ptexture->m_vkformat;
         //imageCreateInfo.extent = {unsigned int(width), unsigned int(height), 1};
         //imageCreateInfo.mipLevels = ptexture->m_mipsLevel;
         //imageCreateInfo.arrayLayers = 6;
         //imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
         //imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
         //imageCreateInfo.usage =
         //   VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
         //imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
         //imageCreateInfo.queueFamilyIndexCount = 0;
         //imageCreateInfo.pQueueFamilyIndices = nullptr;
         //imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
         //VkResult result = vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &ptexture->m_vkimage);
         //// validateResult(result);

         //VkMemoryRequirements memoryRequirements;
         //vkGetImageMemoryRequirements(pcontext->logicalDevice(), ptexture->m_vkimage, &memoryRequirements);

         //VkMemoryAllocateInfo memoryAllocateInfo = {};
         //memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
         //memoryAllocateInfo.allocationSize = memoryRequirements.size;
         //memoryAllocateInfo.memoryTypeIndex =
         //   pphysicaldevice->findMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
         //VK_CHECK_RESULT(vkAllocateMemory(pcontext->logicalDevice(), &memoryAllocateInfo, nullptr, &ptexture->m_vkdevicememory));
         //// validateResult(result);

         //VK_CHECK_RESULT(vkBindImageMemory(pcontext->logicalDevice(), ptexture->m_vkimage, ptexture->m_vkdevicememory, 0));
         //// validateResult(result);

         //// Create staging buffer
         //VkBuffer stagingBuffer;
         //VkDeviceMemory stagingMemory;

         //VkBufferCreateInfo bufferCreateInfo = {};
         //bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
         //bufferCreateInfo.pNext = nullptr;
         //bufferCreateInfo.flags = 0;
         //bufferCreateInfo.size = memoryRequirements.size;
         //bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
         //bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
         //bufferCreateInfo.queueFamilyIndexCount = 0;
         //bufferCreateInfo.pQueueFamilyIndices = nullptr;
         //VK_CHECK_RESULT(vkCreateBuffer(pcontext->logicalDevice(), &bufferCreateInfo, nullptr, &stagingBuffer));
         ///// validateResult(result);

         //vkGetBufferMemoryRequirements(pcontext->logicalDevice(), stagingBuffer, &memoryRequirements);

         //memoryAllocateInfo.allocationSize = memoryRequirements.size;
         //memoryAllocateInfo.memoryTypeIndex =
         //   pphysicaldevice->findMemoryType(memoryRequirements.memoryTypeBits,
         //                                   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
         //VK_CHECK_RESULT(vkAllocateMemory(pcontext->logicalDevice(), &memoryAllocateInfo, nullptr, &stagingMemory));
         // //validateResult(result);

         //VK_CHECK_RESULT(vkBindBufferMemory(pcontext->logicalDevice(), stagingBuffer, stagingMemory, 0));
         //// validateResult(result);

         //unsigned char *data;
         //result = vkMapMemory(pcontext->logicalDevice(), stagingMemory, 0, imageSize, 0, (void **)&data);
         //// validateResult(result);

         //unsigned long long dataLayer = unsigned long long(width) * height * channels * iBytesPerChannel;
         //// for (unsigned int i = 0; i < 6; i++)
         ////{
         //memcpy((void *)(data), &imagedata, dataLayer);
         //// stbi_image_free(textureData[i]);
         ////}

          ptexture->_set_state(pcommandbufferCopy,
             {VK_ACCESS_TRANSFER_READ_BIT, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                                VK_PIPELINE_STAGE_TRANSFER_BIT});

         //vkUnmapMemory(pcontext->logicalDevice(), stagingMemory);
         generateCubemapMipmaps(pcommandbufferCopy->m_vkcommandbuffer, ptexture->m_vkimage, ptexture->m_vkformat,
                                ptexture->m_rectangleTarget.width(), ptexture->m_rectangleTarget.height(),
                                ptexture->m_mipsLevel);

         // Create image view
         VkImageViewCreateInfo imageViewCreateInfo = {};
         imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
         imageViewCreateInfo.pNext = nullptr;
         imageViewCreateInfo.flags = 0;
         imageViewCreateInfo.image = ptexture->m_vkimage;
         imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
         imageViewCreateInfo.format = ptexture->m_vkformat;
         imageViewCreateInfo.components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY,
         };
         imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
         imageViewCreateInfo.subresourceRange.levelCount = ptexture->m_mipsLevel;
         imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
         imageViewCreateInfo.subresourceRange.layerCount = 6;
         VK_CHECK_RESULT(vkCreateImageView(pcontext->logicalDevice(), &imageViewCreateInfo, nullptr, &ptexture->m_vkimageview));
         //;;validateResult(result);

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
         samplerCreateInfo.maxLod = float(ptexture->m_mipsLevel);
         samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
         ;
         samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
         samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
         samplerCreateInfo.anisotropyEnable = VK_TRUE;
         samplerCreateInfo.maxAnisotropy = pphysicaldevice->m_vkphysicaldeviceproperties.limits.maxSamplerAnisotropy;
         samplerCreateInfo.compareEnable = VK_FALSE;
         samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
         samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
         samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
         VK_CHECK_RESULT( vkCreateSampler(pcontext->logicalDevice(), &samplerCreateInfo, nullptr, &ptexture->m_vksampler3));
         // validateResult(result);
         ::cast<::gpu_vulkan::queue> pqueueTransfer = pcontext->m_pqueueTransfer;
         VK_CHECK_RESULT(vkQueueWaitIdle(pqueueTransfer->m_vkqueue));
         // validateResult(result);
         // result = vkResetCommandBuffer(textureManager.mCommandBuffer, 0);
         /// validateResult(result);

         ////vkFreeMemory(pcontext->logicalDevice(), stagingMemory, nullptr);
         //ptexture->_set_state(pcommandbufferCopy, 
         //   {VK_ACCESS_TRANSFER_READ_BIT, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
         //                                      VK_PIPELINE_STAGE_TRANSFER_BIT});
         //VkImageMemoryBarrier barrier = {};
         //barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
         //barrier.pNext = nullptr;
         //barrier.srcAccessMask = 0;
         //barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT /* Additional >> */ | VK_ACCESS_TRANSFER_READ_BIT;
         //barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
         //barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
         //barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
         //barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
         //barrier.image = ptexture->m_vkimage;
         //barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         //barrier.subresourceRange.baseMipLevel = 1;
         //barrier.subresourceRange.levelCount = ptexture->m_mipsLevel-1;
         //barrier.subresourceRange.baseArrayLayer = 0;
         //barrier.subresourceRange.layerCount = 6;
         //vkCmdPipelineBarrier(pcommandbufferCopy->m_vkcommandbuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
         //                     VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

         ////VkBufferImageCopy copyRegion = {};
         ////copyRegion.bufferOffset = 0;
         ////copyRegion.bufferRowLength = 0;
         ////copyRegion.bufferImageHeight = 0;
         ////copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         ////copyRegion.imageSubresource.mipLevel = 0;
         ////copyRegion.imageSubresource.baseArrayLayer = 0;
         ////copyRegion.imageSubresource.layerCount = 6;
         ////copyRegion.imageOffset = {0, 0, 0};
         ////copyRegion.imageExtent = {unsigned int(width), unsigned int(height), 1};
         ////vkCmdCopyBufferToImage(pcommandbufferCopy->m_vkcommandbuffer, 
         ////   stagingBuffer,
         ////   ptexture->m_vkimage,
         ////                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

         //barrier.srcAccessMask = 0;
         //barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
         //barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
         //barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
         //barrier.subresourceRange.levelCount = 1;

         //VkImageBlit imageBlit = {};
         //imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         //imageBlit.srcSubresource.baseArrayLayer = 0;
         //imageBlit.srcSubresource.layerCount = 6;
         //imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         //imageBlit.dstSubresource.baseArrayLayer = 0;
         //imageBlit.dstSubresource.layerCount = 6;

         //unsigned int mipWidth = width, mipHeight = height;
         //imageBlit.srcSubresource.mipLevel = 0;
         //imageBlit.srcOffsets[0] = {0, 0, 0};
         //imageBlit.srcOffsets[1] = {int(mipWidth), int(mipHeight), 1};

         //for (unsigned int i = 1; i < ptexture->m_mipsLevel; i++)
         //{

         //   imageBlit.dstSubresource.mipLevel = i;
         //   if (mipWidth > 1)
         //      mipWidth /= 2;
         //   if (mipHeight > 1)
         //      mipHeight /= 2;
         //   imageBlit.dstOffsets[0] = {0, 0, 0};
         //   imageBlit.dstOffsets[1] = {int(mipWidth), int(mipHeight), 1};

         //   //barrier.subresourceRange.baseMipLevel = i;
         //   //vkCmdPipelineBarrier(pcommandbufferCopy->m_vkcommandbuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
         //   //                     VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);


         //   vkCmdBlitImage(pcommandbufferCopy->m_vkcommandbuffer,ptexture->m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
         //                  ptexture->m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageBlit, VK_FILTER_LINEAR);
         //}

         //barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
         //barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
         //barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
         //barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
         //for (unsigned int i = 0; i < ptexture->m_mipsLevel; i++)
         //{
         //   barrier.subresourceRange.baseMipLevel = i;
         //   vkCmdPipelineBarrier(pcommandbufferCopy->m_vkcommandbuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
         //                        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
         //   barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
         //}
         ptexture->m_state.m_vkimagelayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

                  pcontext->endSingleTimeCommands(pcommandbufferCopy);


         ptexture->m_descriptor3.sampler = ptexture->m_vksampler3;
         ptexture->m_descriptor3.imageView = ptexture->m_vkimageview;
         ptexture->m_descriptor3.imageLayout = ptexture->m_state.m_vkimagelayout;
         //vkDestroyBuffer(pcontext->logicalDevice(), stagingBuffer, nullptr);
      
      }


      void cubemap_framebuffer::setCubeFace(unsigned int index)
      {
         ::cast < gpu_vulkan::texture>ptexture = m_ptexture;
         //glFramebufferTexture2D(
         //   GL_FRAMEBUFFER,
         //   GL_COLOR_ATTACHMENT0,
         //   GL_TEXTURE_CUBE_MAP_POSITIVE_X + index,
         //   ptexture->m_gluTextureID,
         //   0);
         //GLCheckError("");
      }


 /*     unsigned int cubemap_framebuffer::getCubemapTextureId()
      {
         ::cast < gpu_opengl::texture>ptexture = m_ptexture;
         return ptexture->m_gluTextureID;
      }
*/

   } // namespace ibl


} // namespace gpu_vulkan

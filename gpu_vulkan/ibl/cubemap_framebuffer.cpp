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
#include "gpu_vulkan/render_pass.h"
#include "gpu_vulkan/renderer.h"


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

         ::cast<gpu_vulkan::context> pcontext = m_pgpucontext;

         ptexture->m_mipsLevel = (uint32_t)(floor(::log2((double)::maximum(ptexture->m_rectangleTarget.width(),
                                                                           ptexture->m_rectangleTarget.height()))) +
                                            1.0);
         ptexture->m_bTransferSrc = true;
         ptexture->m_bWithDepth = false;
         ptexture->m_bSrgb = true;
         ptexture->initialize_image_texture(m_pgpucontext->m_pgpurenderer, ptexture->m_rectangleTarget, true,
                                            {}, ::gpu::texture::e_type_cube_map);

        
         for (uint32_t i = 0; i < 6; ++i)
         {
            VkImageViewCreateInfo faceView{};
            faceView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            faceView.image = ptexture->m_vkimage;
            faceView.viewType = VK_IMAGE_VIEW_TYPE_2D;
            faceView.format = ptexture->m_vkformat;
            faceView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            faceView.subresourceRange.baseMipLevel = 0;
            faceView.subresourceRange.levelCount = 1;
            faceView.subresourceRange.baseArrayLayer = i; // <--- select the cube face
            faceView.subresourceRange.layerCount = 1;
            VK_CHECK_RESULT(vkCreateImageView(pcontext->logicalDevice(), &faceView, nullptr, &m_imageviewa[i]));
         }
         
         //for (uint32_t i = 0; i < 6; ++i)
         //{
         //   
         //   VkImageView attachments[] = {m_imageviewa[i]};

         //   VkFramebufferCreateInfo framebufferInfo{};
         //   framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
         //   framebufferInfo.renderPass = renderPass;
         //   framebufferInfo.attachmentCount = 1;
         //   framebufferInfo.pAttachments = attachments;
         //   framebufferInfo.width = ptexture->m_rectangleTarget.width();
         //   framebufferInfo.height = ptexture->m_rectangleTarget.height();
         //   framebufferInfo.layers = 1;
         //   VK_CHECK_RESULT(vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffers[i]));

         //}

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


      VkFramebuffer cubemap_framebuffer::framebuffer(::gpu_vulkan::render_pass *prenderpass, int iFace)
      {

         auto &renderpass = m_mapRenderPass[prenderpass];

         if (!renderpass.m_framebuffera[iFace])
         {

            renderpass.m_framebuffera[iFace] = _framebuffer(prenderpass, iFace);

         }

         return renderpass.m_framebuffera[iFace];

      }



      VkFramebuffer cubemap_framebuffer::_framebuffer(::gpu_vulkan::render_pass *prenderpass, int iFace)
      {

         auto &framebuffercube = m_mapFramebufferCube[prenderpass->m_vkrenderpass];

         if (framebuffercube.m_framebuffera[iFace])
         {

            return framebuffercube.m_framebuffera[iFace];

         }

         ::cast<::gpu_vulkan::context> pcontext = m_pgpucontext;

         VkImageView attachments[1];

         attachments[0] = m_imageviewa[iFace];

         int iAttachmentCount = 1;

         //if (prenderpass->m_bWithDepth)
         //{

         //   if (!m_bWithDepth)
         //   {

         //      m_bWithDepth = true;
         //   }

         //   VkImageView depthImageView = get_depth_image_view();

         //   attachments[1] = depthImageView;

         //   iAttachmentCount = 2;
         //}
         //else
         //{

         //   iAttachmentCount = 1;
         //}

         VkFramebufferCreateInfo framebufferInfo{};
         framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
         framebufferInfo.renderPass = prenderpass->m_vkrenderpass;
         framebufferInfo.attachmentCount = 1;
         framebufferInfo.pAttachments = attachments;
         framebufferInfo.width = m_ptexture->m_rectangleTarget.width();
         framebufferInfo.height = m_ptexture->m_rectangleTarget.height();
         framebufferInfo.layers = 1;
         
         VK_CHECK_RESULT(vkCreateFramebuffer(pcontext->logicalDevice(), &framebufferInfo, nullptr,
                                             &framebuffercube.m_framebuffera[iFace]));

         return framebuffercube.m_framebuffera[iFace];

      }


      void cubemap_framebuffer::bind(::gpu::command_buffer *pgpucommandbuffer)
      {
         
         ::cast < gpu_vulkan::texture>ptexture = m_ptexture;
         ::cast<gpu_vulkan::context> pcontext = m_pgpucontext;
         pcontext->m_pshaderBound->bind(pgpucommandbuffer, ptexture);
         //glBindFramebuffer(GL_FRAMEBUFFER, ptexture->m_gluFbo);
         ///GLCheckError("");
      }


      void generateCubemapMipmaps(
         VkCommandBuffer cmd, 
         VkImage image,
         VkFormat format, 
         int32_t width, int32_t height,
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

         auto width = ptexture->m_rectangleTarget.width();

         auto height = ptexture->m_rectangleTarget.height();

         ::cast<::gpu_vulkan::context> pcontext = m_pgpucontext;

         auto pgpucommandbufferCopy = pcontext->beginSingleTimeCommands(pcontext->m_pgpudevice->transfer_queue());

         ::cast<command_buffer> pcommandbufferCopy = pgpucommandbufferCopy;

         ptexture->_set_state(pcommandbufferCopy,
            {
               VK_ACCESS_TRANSFER_READ_BIT,
               VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
               VK_PIPELINE_STAGE_TRANSFER_BIT
            });

         generateCubemapMipmaps(
            pcommandbufferCopy->m_vkcommandbuffer,
            ptexture->m_vkimage, 
            ptexture->m_vkformat,
            ptexture->m_rectangleTarget.width(), ptexture->m_rectangleTarget.height(),
            ptexture->m_mipsLevel);

         pcontext->endSingleTimeCommands(pcommandbufferCopy);


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
         samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
         samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
         samplerCreateInfo.anisotropyEnable =
            pphysicaldevice->m_vkphysicaldevicefeatures.samplerAnisotropy ? VK_TRUE : VK_FALSE;
         samplerCreateInfo.maxAnisotropy =
            samplerCreateInfo.anisotropyEnable
               ? pphysicaldevice->m_vkphysicaldeviceproperties.limits.maxSamplerAnisotropy
               : 1.0f;
         samplerCreateInfo.compareEnable = VK_FALSE;
         samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
         samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
         samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
         VK_CHECK_RESULT(vkCreateSampler(pcontext->logicalDevice(), &samplerCreateInfo, nullptr, &ptexture->m_vksampler3));

         ptexture->m_state.m_vkaccessflags = VK_ACCESS_SHADER_READ_BIT;
         ptexture->m_state.m_vkimagelayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
         ptexture->m_state.m_vkpipelinestageflags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

         ptexture->m_descriptor3.sampler = ptexture->m_vksampler3;
         ptexture->m_descriptor3.imageView = ptexture->m_vkimageview;
         ptexture->m_descriptor3.imageLayout = ptexture->m_state.m_vkimagelayout;
      
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

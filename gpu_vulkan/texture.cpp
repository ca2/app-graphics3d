// Created by camilo on 2025-06-08 18:14 < 3ThomasBorregaardSørensen!!
#include "framework.h"
#include "command_buffer.h"
#include "context.h"
#include "device.h"
#include "initializers.h"
#include "physical_device.h"
#include "renderer.h"
#include "texture.h"


namespace gpu_vulkan
{


   texture::texture()
   {

      m_bTransferDst = false;
      new_texture.set_new_texture();

      m_vkimage = nullptr;
      m_vkimagelayout = VK_IMAGE_LAYOUT_UNDEFINED;
      m_vkimageview = nullptr;
      m_vkdevicememory = nullptr;
      m_bCpuRead = false;

   }


   texture::~texture()
   {

   }


   void texture::initialize_gpu_texture(::gpu::renderer* prenderer, const ::int_rectangle& rectangleTarget) //, bool bCreateRenderTargetView, bool bCreateShaderResourceView)
   {

      if (m_rectangleTarget == rectangleTarget
         && m_pgpurenderer ==  prenderer)
      {

         return;

      }

      auto currentSize = m_rectangleTarget.size();

      ::gpu::texture::initialize_gpu_texture(prenderer, rectangleTarget);

      if (currentSize == rectangleTarget.size()
         && m_pgpurenderer == prenderer)
      {

         return;

      }

      m_vkimagelayout = VK_IMAGE_LAYOUT_UNDEFINED;
      m_vkaccessflags = 0;
      m_vkpipelinestageflags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

      ::cast < context > pgpucontext = pcontext;

      ::cast < render_pass > prenderpass = m_pgpurenderer->m_pgpurendertarget;

      VkImageCreateInfo imagecreateinfo = ::vulkan::initializers::imageCreateInfo();
      imagecreateinfo.imageType = VK_IMAGE_TYPE_2D;
      imagecreateinfo.format = pcontext->m_formatImageDefault;
      imagecreateinfo.extent.width = rectangleTarget.width();
      imagecreateinfo.extent.height = rectangleTarget.height();
      imagecreateinfo.extent.depth = 1;
      imagecreateinfo.mipLevels = 1;
      imagecreateinfo.arrayLayers = 1;
      imagecreateinfo.samples = VK_SAMPLE_COUNT_1_BIT;
      
      imagecreateinfo.usage = 0;

      if (m_bTransferDst & m_bCpuRead)
      {
         
         imagecreateinfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
         
         imagecreateinfo.tiling = VK_IMAGE_TILING_LINEAR;
         
      }
      else
      {

         if (m_bTransferDst)
         {

            imagecreateinfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

         }

         imagecreateinfo.usage |=
            VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
            VK_IMAGE_USAGE_SAMPLED_BIT;

         imagecreateinfo.tiling = VK_IMAGE_TILING_OPTIMAL;

      }
      
      imagecreateinfo.initialLayout = m_vkimagelayout;
      
      VkMemoryPropertyFlags properties;

      if (m_bCpuRead)
      {

         properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

      }
      else
      {
         
         properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

      }
      
      pcontext->createImageWithInfo(
         imagecreateinfo,
         properties,
         m_vkimage,
         m_vkdevicememory);

   }


   void texture::_new_state(::gpu_vulkan::command_buffer* pcommandbuffer,
      VkAccessFlags accessFlags,
      VkImageLayout newLayout,
      VkPipelineStageFlags pipelineStageFlags)
   {

      auto image = m_vkimage;
      
      auto accessOld = m_vkaccessflags;

      auto accessNew = accessFlags;

      auto layoutOld = m_vkimagelayout;

      auto layoutNew = newLayout;

      auto pipelineStageFlagsOld = m_vkpipelinestageflags;

      auto pipelineStageFlagsNew = pipelineStageFlags;

      // Optional: Skip no-op transitions
      if (layoutOld == layoutNew 
         && accessOld == accessNew && 
         pipelineStageFlagsOld == pipelineStageFlagsNew)
         return;

      //VkCommandBufferBeginInfo beginInfo =
      //{

      //   .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO

      //};

      //vkBeginCommandBuffer(pcommandbuffer->m_vkcommandbuffer, &beginInfo);

      VkImageMemoryBarrier barrier = {
          .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
          .srcAccessMask = accessOld,
          .dstAccessMask = accessNew,
          .oldLayout = layoutOld,
          .newLayout = layoutNew,
          .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
          .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
          .image = image,
          .subresourceRange = {
              .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
              .baseMipLevel = 0,
              .levelCount = 1,
              .baseArrayLayer = 0,
              .layerCount = 1
          }
      };

      vkCmdPipelineBarrier(
         pcommandbuffer->m_vkcommandbuffer,
         pipelineStageFlagsOld,
         pipelineStageFlagsNew,
         0,
         0, NULL,
         0, NULL,
         1, &barrier
      );

      m_vkimagelayout = layoutNew;
      m_vkaccessflags = accessNew;
      m_vkpipelinestageflags = pipelineStageFlagsNew;

      //vkEndCommandBuffer(cmdBuf);
      //VkSubmitInfo submitInfo = {
      //    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      //    .commandBufferCount = 1,
      //    .pCommandBuffers = &cmdBuf
      //};
      //vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
      //vkQueueWaitIdle(graphicsQueue);

   }


   //void texture::blend(::gpu::texture* ptexture, const ::int_rectangle& rectangleTarget)
   //{

   //   

   //}

   void texture::create_image_view()
   {

      if (m_bCpuRead)
      {

         return;

      }


      if (m_vkimageview)
      {

         return;

      }

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

      VkImageViewCreateInfo viewInfo = {
          .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
          .image = m_vkimage,
          .viewType = VK_IMAGE_VIEW_TYPE_2D,
          .format = VK_FORMAT_R8G8B8A8_UNORM,
          .subresourceRange = {
              .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
              .baseMipLevel = 0,
              .levelCount = 1,
              .baseArrayLayer = 0,
              .layerCount = 1
          }
      };

      VK_CHECK_RESULT(vkCreateImageView(pcontext->logicalDevice(), &viewInfo, NULL, &m_vkimageview));

   }


   VkFramebuffer texture::create_framebuffer(VkRenderPass renderpass)
   {

      if (m_bCpuRead)
      {

         return {};

      }

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

      VkFramebufferCreateInfo fbInfo = {
       .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
       .renderPass = renderpass,
       .attachmentCount = 1,
       .pAttachments = &m_vkimageview,
       .width = (uint32_t) m_rectangleTarget.width(),
       .height = (uint32_t)m_rectangleTarget.height(),
       .layers = 1
      };

      VkFramebuffer vkframebuffer;

      VK_CHECK_RESULT(vkCreateFramebuffer(pcontext->logicalDevice(), &fbInfo, NULL, &vkframebuffer));

      return vkframebuffer;

   }


} // namespace gpu_vulkan





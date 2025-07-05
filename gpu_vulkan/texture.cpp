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

      m_etype = e_type_none;

      m_vkimagelayout = VK_IMAGE_LAYOUT_UNDEFINED;
      m_vkaccessflags = 0;
      m_vkpipelinestageflags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

      m_bTransferDst = true;
      new_texture.set_new_texture();

      m_vkimage = nullptr;
      m_vkimageview = nullptr;
      m_vkdevicememory = nullptr;
      m_bCpuRead = false;

   }


   texture::~texture()
   {


   }


   void texture::initialize_image_texture(::gpu::renderer* prenderer, const ::int_rectangle& rectangleTarget, bool bWithDepth)
   {

      if (m_rectangleTarget == rectangleTarget
         && m_pgpurenderer ==  prenderer)
      {

         return;

      }

      auto currentSize = m_rectangleTarget.size();

      ::gpu::texture::initialize_image_texture(prenderer, rectangleTarget, bWithDepth);

      if (currentSize == rectangleTarget.size()
         && m_pgpurenderer == prenderer)
      {

         return;

      }

      if (m_etype & ::gpu::texture::e_type_depth)
      {

         get_depth_image();

      }
      else
      {

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

         if (m_bWithDepth)
         {

            get_depth_image();

         }

      }

   }


   void texture::_new_state(::gpu_vulkan::command_buffer* pcommandbuffer,
      VkAccessFlags accessFlags,
      VkImageLayout newLayout,
      VkPipelineStageFlags pipelineStageFlags)
   {

      ASSERT(pcommandbuffer->m_estate == ::gpu::command_buffer::e_state_recording);

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
              .baseMipLevel = 0,
              .levelCount = 1,
              .baseArrayLayer = 0,
              .layerCount = 1
          }
      };

      if(m_etype == e_type_depth)
      {
         
         barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

      }
      else if (m_etype == e_type_depth_stencil)
      {
         
         barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;

      }
      else if (m_etype == e_type_image)
      {

         barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

      }
      else
      {

         throw ::exception(error_wrong_state);

      }

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

   }


   VkImage texture::get_depth_image()
   {

      if (m_etype & ::gpu::texture::e_type_depth)
      {

         if (m_bCpuRead)
         {

            return VK_NULL_HANDLE;

         }

         if (m_vkimage)
         {

            return m_vkimage;

         }

         ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

         VkFormat depthFormat = pcontext->findDepthFormat();
         VkImageCreateInfo imageInfo{};

         imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
         imageInfo.imageType = VK_IMAGE_TYPE_2D;
         imageInfo.extent.width = m_rectangleTarget.width();
         imageInfo.extent.height = m_rectangleTarget.height();
         imageInfo.extent.depth = 1;
         imageInfo.mipLevels = 1;
         imageInfo.arrayLayers = 1;
         imageInfo.format = depthFormat;
         imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
         imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
         imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
         imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
         imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
         imageInfo.flags = 0;

         auto & depthImage = m_vkimage;

         auto & depthImageMemory = m_vkdevicememory;

         pcontext->createImageWithInfo(
            imageInfo,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            depthImage,
            depthImageMemory);

         ::cast < command_buffer > pcommandbuffer = m_pgpurenderer->getCurrentCommandBuffer2();

         _new_state(
            pcommandbuffer,
            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT);

         return m_vkimage;

      }
      else
      {

         return _get_depth_texture()->m_vkimage;

      }

   }


   texture* texture::_get_depth_texture()
   {
      
      if (m_etype & ::gpu::texture::e_type_depth)
      {

         return this;

      }

      if (m_ptextureDepth)
      {     

         return m_ptextureDepth;

      }

      __defer_construct_new(m_ptextureDepth);

      m_ptextureDepth->initialize_depth_texture(m_pgpurenderer, m_rectangleTarget);

      return m_ptextureDepth;

   }  


   VkImageView texture::get_image_view()
   {

      if (m_bCpuRead)
      {

         return VK_NULL_HANDLE;

      }

      if (m_vkimageview)
      {

         return m_vkimageview;

      }

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

      VkImageViewCreateInfo viewInfo = {
          .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
          .image = m_vkimage,
          .viewType = VK_IMAGE_VIEW_TYPE_2D,
          .format = pcontext->m_formatImageDefault,
          .subresourceRange = {
              .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
              .baseMipLevel = 0,
              .levelCount = 1,
              .baseArrayLayer = 0,
              .layerCount = 1
          }
      };

      VK_CHECK_RESULT(vkCreateImageView(pcontext->logicalDevice(), &viewInfo, NULL, &m_vkimageview));

      return m_vkimageview;

   }


   VkFramebuffer texture::get_framebuffer(VkRenderPass renderpass)
   {

      if (m_bCpuRead)
      {

         return VK_NULL_HANDLE;

      }

      auto & framebuffer = m_mapFramebuffer[renderpass];

      if(framebuffer)
      {
         return framebuffer;
      }

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

      VkImageView attachments[2];

      attachments[0] = get_image_view();

      int iAttachmentCount;

      if (m_ptextureDepth)
      {

         VkImageView depthImageView = get_depth_image_view();

         attachments[1] = depthImageView;

         iAttachmentCount = 2;

      }
      else
      {

         iAttachmentCount = 1;

      }

      VkFramebufferCreateInfo fbInfo = {
       .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
       .renderPass = renderpass,
       .attachmentCount = (uint32_t) iAttachmentCount,
       .pAttachments = attachments,
       .width = (uint32_t) m_rectangleTarget.width(),
       .height = (uint32_t)m_rectangleTarget.height(),
       .layers = 1
      };

      VK_CHECK_RESULT(vkCreateFramebuffer(
         pcontext->logicalDevice(), &fbInfo, NULL, &framebuffer));

      return framebuffer;

   }


   VkImageView texture::get_depth_image_view()
   {

      if (m_etype & ::gpu::texture::e_type_depth)
      {

         ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

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

         return m_vkimageview;

      }
      else
      {

         return _get_depth_texture()->get_depth_image_view();

      }

   }

} // namespace gpu_vulkan





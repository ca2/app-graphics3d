// Created by camilo on 2025-06-08 18:14 < 3ThomasBorregaardSørensen!!
#include "framework.h"
#include "buffer.h"
#include "command_buffer.h"
#include "context.h"
#include "descriptors.h"
#include "device.h"
#include "initializers.h"
#include "physical_device.h"
#include "render_target.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "acme/graphics/image/pixmap.h"
#include "aura/graphics/image/image.h"


namespace gpu_vulkan
{


   texture_synchronization::texture_synchronization()
   {

   }


   texture_synchronization::~texture_synchronization()
   {


   }


   VkFramebuffer texture::framebuffer(::gpu_vulkan::render_pass* prenderpass)
   {

      auto& renderpass = m_mapRenderPass[prenderpass];

      if (!renderpass.m_vkframebuffer)
      {

         renderpass.m_vkframebuffer = _framebuffer(prenderpass);

      }

      return renderpass.m_vkframebuffer;
   }


   //   VkFramebuffer texture_synchronization::_get_frame_buffer(::gpu_vulkan::render_pass * prenderpass)
   //   {

   //   VkImageView imageView = m_ptexture->get_image_view();

   //   VkImageView attachments[2];

   //   attachments[0] = imageView;

   //   int iAttachmentCount;

   //   if (prenderpass->m_bWithDepth)
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


   VkFence texture_synchronization::in_flight_fence()
   {

      if (!m_vkfenceInFlight2)
      {

         ::cast < ::gpu_vulkan::context > pcontext = m_ptexture->m_pgpurenderer->m_pgpucontext;

         VkFenceCreateInfo fenceInfo = {};
         fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
         fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

         if (vkCreateFence(
            pcontext->logicalDevice(),
            &fenceInfo,
            nullptr,
            &m_vkfenceInFlight2) != VK_SUCCESS
            )
         {

            throw ::exception(error_failed, "failed to create fence!");

         }

      }

      return m_vkfenceInFlight2;

   }


   texture_synchronization * texture::synchronization()
   {

      //::cast<::gpu_vulkan::render_target> prendertarget = pgpurendertarget;

      auto& psynchronization = this->m_ptexturesynchronization;

      //auto& synchronization = this->m_mapSynchronization[prendertarget];

      if (!psynchronization)
      {

         __construct_new(psynchronization);

         ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

         //synchronization.m_prendertarget = nullptr;

         psynchronization->m_ptexture = this;

         VkSemaphoreCreateInfo semaphoreInfo = {};
         semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

         psynchronization->m_iImageAvailable = 0;

         if (vkCreateSemaphore(
            pcontext->logicalDevice(),
            &semaphoreInfo,
            nullptr,
            &psynchronization->m_vksemaphoreAvailable
         ) != VK_SUCCESS

            ||

            vkCreateSemaphore(
               pcontext->logicalDevice(),
               &semaphoreInfo,
               nullptr, &psynchronization->m_vksemaphoreRenderFinished
            ) != VK_SUCCESS)

         {

            throw ::exception(error_failed, "failed to create synchronization objects for a frame!");

         }

      }

      return psynchronization;

   }


   texture::texture()
   {

      m_mipsLevel = 1;
      m_bOwnImage = false;

      m_etype = e_type_none;

      m_vkformat = VK_FORMAT_UNDEFINED;

      m_state.m_vkimagelayout = VK_IMAGE_LAYOUT_UNDEFINED;
      m_state.m_vkaccessflags = 0;
      m_state.m_vkpipelinestageflags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

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


   void texture::initialize_image_texture(::gpu::renderer* prenderer, const ::int_rectangle& rectangleTarget, bool bWithDepth, const ::pointer_array < ::image::image >& imagea, enum_type etype)
   {

      if (m_rectangleTarget == rectangleTarget
         && m_pgpurenderer == prenderer)
      {

         return;

      }

      auto currentSize = m_rectangleTarget.size();

      ::gpu::texture::initialize_image_texture(prenderer, rectangleTarget, bWithDepth, imagea, etype);

      if (currentSize == rectangleTarget.size()
         && m_pgpurenderer == prenderer)
      {

         return;

      }

      ASSERT(!(m_etype & ::gpu::texture::e_type_depth));

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

      ::cast < context > pgpucontext = pcontext;

      ::cast < render_pass > prenderpass = m_pgpurenderer->m_pgpurendertarget;

      VkImageCreateInfo imagecreateinfo = ::vulkan::initializers::imageCreateInfo();

      imagecreateinfo.imageType = VK_IMAGE_TYPE_2D;
      imagecreateinfo.format = m_vkformat = pcontext->m_formatImageDefault;
      imagecreateinfo.extent.width = rectangleTarget.width();
      imagecreateinfo.extent.height = rectangleTarget.height();
      imagecreateinfo.extent.depth = 1;
      imagecreateinfo.mipLevels = m_mipsLevel = 1;
      if (m_etype == e_type_cube_map)
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

         if (m_bTransferSrc)
         {

            imagecreateinfo.usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

         }

         imagecreateinfo.usage |= VK_IMAGE_USAGE_SAMPLED_BIT;

         if (m_bRenderTarget)
         {

            imagecreateinfo.usage |=
               VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

         }

         imagecreateinfo.tiling = VK_IMAGE_TILING_OPTIMAL;

      }

      imagecreateinfo.initialLayout = m_state.m_vkimagelayout;

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

      if(imagea.has_element())
      {

         if (m_etype == e_type_cube_map)
         {

            _LoadCubeMap(imagea);

         }

      }

   }


   void texture::_LoadCubeMap(const ::pointer_array < ::image::image >& imagea)
   {

      defer_throw_if_cube_map_images_are_not_ok(imagea);

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

      ::cast < context > pgpucontext = pcontext;

      ::cast < device > pdevice = pcontext->m_pgpudevice;

      auto w = imagea.first()->width();

      auto h = imagea.first()->height();
      
      VkDeviceSize layerSize = w * h * 4;
      
      VkDeviceSize totalSize = layerSize * 6;

      auto pbufferStaging = pgpucontext->create_buffer(
         totalSize,
         VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

      pbufferStaging->_assign_cube_map(imagea);

      ::pointer < command_buffer > pcommandbuffer = pcontext->beginSingleTimeCommands();

      pcontext->copyBufferToImage(pcommandbuffer, this, pbufferStaging);

      _set_state(pcommandbuffer,
         {

            VK_ACCESS_TRANSFER_READ_BIT,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            VK_PIPELINE_STAGE_TRANSFER_BIT

         });

      pcontext->endSingleTimeCommands(pcommandbuffer);

   }


   void texture::initialize_depth_texture(::gpu::renderer* pgpurenderer, const ::int_rectangle& rectangleTarget)
   {

      if (m_rectangleTarget == rectangleTarget
         && m_pgpurenderer == pgpurenderer)
      {

         return;

      }

      auto currentSize = m_rectangleTarget.size();

      ::gpu::texture::initialize_depth_texture(pgpurenderer, rectangleTarget);

      if (currentSize == rectangleTarget.size()
         && m_pgpurenderer == pgpurenderer)
      {

         return;

      }

      ASSERT(m_etype & ::gpu::texture::e_type_depth);


      get_depth_image();

      ////}
      ////else
      ////{

      //::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

      //::cast < context > pgpucontext = pcontext;

      //::cast < render_pass > prenderpass = m_pgpurenderer->m_pgpurendertarget;

      //VkImageCreateInfo imagecreateinfo = ::vulkan::initializers::imageCreateInfo();

      //imagecreateinfo.imageType = VK_IMAGE_TYPE_2D;
      //imagecreateinfo.format = pcontext->m_formatImageDefault;
      //imagecreateinfo.extent.width = rectangleTarget.width();
      //imagecreateinfo.extent.height = rectangleTarget.height();
      //imagecreateinfo.extent.depth = 1;
      //imagecreateinfo.mipLevels = 1;
      //imagecreateinfo.arrayLayers = 1;
      //imagecreateinfo.samples = VK_SAMPLE_COUNT_1_BIT;

      //imagecreateinfo.usage = 0;

      //if (m_bTransferDst & m_bCpuRead)
      //{

      //   imagecreateinfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

      //   imagecreateinfo.tiling = VK_IMAGE_TILING_LINEAR;

      //}
      //else
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

      //imagecreateinfo.initialLayout = m_vkimagelayout;

      //VkMemoryPropertyFlags properties;

      //if (m_bCpuRead)
      //{

      //   properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

      //}
      //else
      //{

      //   properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

      //}

      //pcontext->createImageWithInfo(
      //   imagecreateinfo,
      //   properties,
      //   m_vkimage,
      //   m_vkdevicememory);

      //if (m_bWithDepth)
      //{

      //   get_depth_image();

      //}

      ////}

   }


   void texture::_set_state(::gpu_vulkan::command_buffer* pcommandbuffer,
      state_t state)
   {

      ASSERT(pcommandbuffer->m_estate == ::gpu::command_buffer::e_state_recording);

      auto image = m_vkimage;

      auto accessOld = m_state.m_vkaccessflags;

      auto accessNew = state.m_vkaccessflags;

      auto layoutOld = m_state.m_vkimagelayout;

      auto layoutNew = state.m_vkimagelayout;

      auto pipelineStageFlagsOld = m_state.m_vkpipelinestageflags;

      auto pipelineStageFlagsNew = state.m_vkpipelinestageflags;

      // Optional: Skip no-op transitions
      if (layoutOld == layoutNew
         && accessOld == accessNew &&
         pipelineStageFlagsOld == pipelineStageFlagsNew)
         return;

      unsigned int iLayerCount;

      if (m_etype == e_type_cube_map)
      {

         iLayerCount = 6;

      }
      else
      {

         iLayerCount = 1;

      }

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
              .layerCount = iLayerCount
          }
      };

      if (m_etype == e_type_depth)
      {

         barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

      }
      else if (m_etype == e_type_depth_stencil)
      {

         barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;

      }
      else if (m_etype == e_type_image || m_etype == e_type_cube_map)
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

      m_state.m_vkimagelayout = layoutNew;
      m_state.m_vkaccessflags = accessNew;
      m_state.m_vkpipelinestageflags = pipelineStageFlagsNew;

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
         imageInfo.format = m_vkformat = depthFormat;
         imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
         imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
         imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
            | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
         imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
         imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
         imageInfo.flags = 0;

         auto& depthImage = m_vkimage;

         auto& depthImageMemory = m_vkdevicememory;

         pcontext->createImageWithInfo(
            imageInfo,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            depthImage,
            depthImageMemory);

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

         ASSERT(m_bWithDepth);

         ::cast < texture > ptexture = get_depth_texture();

         return ptexture->m_vkimage;

      }

   }


   //texture* texture::_get_depth_texture()
   //{
   //   
   //   if (m_etype & ::gpu::texture::e_type_depth)
   //   {

   //      return this;

   //   }

   //   if (m_ptextureDepth)
   //   {     

   //      return m_ptextureDepth;

   //   }

   //   __defer_construct_new(m_ptextureDepth);

   //   m_ptextureDepth->initialize_depth_texture(m_pgpurenderer, m_rectangleTarget);

   //   return m_ptextureDepth;

   //}  


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
      VkImageViewType viewType;
      if (m_etype == e_type_cube_map)
      {
         viewType = VK_IMAGE_VIEW_TYPE_CUBE;

      }
      else
      {
         viewType = VK_IMAGE_VIEW_TYPE_2D;
      }

      unsigned int uLayerCount;

      if (m_etype == e_type_cube_map)
      {

         uLayerCount = 6;

      }
      else
      {

         uLayerCount = 1;

      }

      VkImageViewCreateInfo viewInfo = {
          .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
          .image = m_vkimage,
          .viewType = viewType,
          .format = m_vkformat = pcontext->m_formatImageDefault,
          .subresourceRange = {
              .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
              .baseMipLevel = 0,
              .levelCount = 1,
              .baseArrayLayer = 0,
              .layerCount = uLayerCount
          }
      };

      VK_CHECK_RESULT(vkCreateImageView(pcontext->logicalDevice(), &viewInfo, NULL, &m_vkimageview));

      return m_vkimageview;

   }


   void texture::set_pixels(const ::int_rectangle & rectangle, const void * data)
   {

      VkDeviceSize size = rectangle.area() * 4;

      ::pointer < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

      auto pbufferStaging = pcontext->create_buffer(
         size,
         VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
      );

      pbufferStaging->_assign(data, size);

      ::cast < command_buffer > pcommandbuffer = pcontext->defer_get_upload_command_buffer();

      pcontext->copyBufferToImage(
         pcommandbuffer, 
         this,
         pbufferStaging,
         rectangle);

   }


   VkDescriptorSet texture::descriptor_set(::gpu_vulkan::shader* pshader)
   {

      auto& shader = m_mapShader[pshader];

      if (!shader.m_bNew)
      {

         return shader.m_vkdescriptorset;

      }

      VkDescriptorImageInfo imageinfo;

      ::cast < context > pcontext = m_pgpurenderer->m_pgpucontext;

      imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      imageinfo.imageView = get_image_view();
      imageinfo.sampler = pcontext->_001VkSampler();

      auto& playout = pshader->m_psetdescriptorlayout;

      auto& ppool = pshader->m_pdescriptorpool;

      unsigned int uSamplerBinding = 0;

      if (pshader->m_bindingSampler.is_set())
         uSamplerBinding = pshader->m_bindingSampler.m_uBinding;
      else if (pshader->m_bindingCubeSampler.is_set())
         uSamplerBinding = pshader->m_bindingCubeSampler.m_uBinding;

      descriptor_writer(*playout, *ppool)
         .writeImage(uSamplerBinding, &imageinfo)
         .build(shader.m_vkdescriptorset);

      shader.m_bNew = false;

      return shader.m_vkdescriptorset;

   }


   VkFramebuffer texture::_framebuffer(::gpu_vulkan::render_pass* prenderpass)
   {

      if (m_bCpuRead)
      {

         return VK_NULL_HANDLE;

      }

      auto& framebuffer = m_mapFramebuffer[prenderpass->m_vkrenderpass];

      if (framebuffer)
      {
         return framebuffer;
      }

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

      VkImageView attachments[2];

      attachments[0] = get_image_view();

      int iAttachmentCount;

      if (prenderpass->m_bWithDepth)
      {

         if (!m_bWithDepth)
         {

            m_bWithDepth = true;

         }

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
       .renderPass = prenderpass->m_vkrenderpass,
       .attachmentCount = (uint32_t)iAttachmentCount,
       .pAttachments = attachments,
       .width = (uint32_t)m_rectangleTarget.width(),
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

         if (!m_vkimageview)
         {

            ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

            VkFormat depthFormat = pcontext->findDepthFormat();

            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = get_depth_image();
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = m_vkformat = depthFormat;
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

         //ASSERT(m_bWithDepth);

         ::cast < texture > ptexture = get_depth_texture();

         if (!ptexture)
         {

            return nullptr;

         }

         return ptexture->get_depth_image_view();

      }

   }


   void texture::_attach(VkImage vkimage, enum_type etype)
   {

      destroy();

      m_bOwnImage = false;

      m_etype = etype;

      m_vkimage = vkimage;

   }

   unsigned int texture::_get_layer_count()
   {
      if (m_etype == e_type_cube_map)
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

      if (m_etype == e_type_cube_map)
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
      m_state.m_vkimagelayout = VK_IMAGE_LAYOUT_UNDEFINED;
      m_state.m_vkaccessflags = VK_ACCESS_NONE;
      m_state.m_vkpipelinestageflags = VK_PIPELINE_STAGE_NONE;
      m_vkimageview = nullptr;

   }


} // namespace gpu_vulkan





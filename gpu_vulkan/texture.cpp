// Created by camilo on 2025-06-08 18:14 < 3ThomasBorregaardSørensen!!
#include "framework.h"
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
#include "acme/filesystem/filesystem/file_context.h"
#include "aura/graphics/image/context.h"
#include "aura/graphics/image/image.h"
#include "acme/graphics/image/pixmap.h"
#include "vk_init.h"
#include "gpu/_ktx.h"
//#include <ktxvulkan.h>
#include <tiny_gltf.h>
#include <ktx.h>
#include <ktxvulkan.h>
#include <math.h>

namespace gpu_vulkan
{


   texture_synchronization::texture_synchronization() {}


   texture_synchronization::~texture_synchronization() {}


   VkFramebuffer texture::framebuffer(::gpu_vulkan::render_pass *prenderpass)
   {

      auto &renderpass = m_mapRenderPass[prenderpass];

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


   texture::~texture() {}


   void texture::initialize_image_texture(::gpu::renderer *prenderer, const ::int_rectangle &rectangleTarget,
                                          bool bWithDepth, const ::pointer_array<::image::image> &imagea,
                                          enum_type etype)
   {

      if (m_rectangleTarget == rectangleTarget && m_pgpurenderer == prenderer)
      {

         return;
      }

      auto currentSize = m_rectangleTarget.size();

      ::gpu::texture::initialize_image_texture(prenderer, rectangleTarget, bWithDepth, imagea, etype);

      if (currentSize == rectangleTarget.size() && m_pgpurenderer == prenderer)
      {

         return;
      }

      ASSERT(!(m_etype & ::gpu::texture::e_type_depth));

      ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;

      ::cast<context> pgpucontext = pcontext;

      ::cast<render_pass> prenderpass = m_pgpurenderer->m_pgpurendertarget;

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

            imagecreateinfo.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
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

      pcontext->createImageWithInfo(imagecreateinfo, properties, m_vkimage, m_vkdevicememory);

      if (m_bWithDepth)
      {

         get_depth_image();
      }

      if (imagea.has_element())
      {

         if (m_etype == e_type_cube_map)
         {

            _LoadCubeMap(imagea);
         }
      }
   }


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

      ::pointer<command_buffer> pcommandbuffer = pcontext->beginSingleTimeCommands(pcontext->m_pqueueTransfer);

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

      if (m_rectangleTarget == rectangleTarget && m_pgpurenderer == pgpurenderer)
      {

         return;
      }

      auto currentSize = m_rectangleTarget.size();

      ::gpu::texture::initialize_depth_texture(pgpurenderer, rectangleTarget);

      if (currentSize == rectangleTarget.size() && m_pgpurenderer == pgpurenderer)
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

      // if (m_bWithDepth)
      //{

      //   get_depth_image();

      //}

      ////}
   }


   void texture::_set_state(::gpu_vulkan::command_buffer *pcommandbuffer, state_t state)
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
      if (layoutOld == layoutNew && accessOld == accessNew && pipelineStageFlagsOld == pipelineStageFlagsNew)
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
         .subresourceRange = {.baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = iLayerCount}};

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

      vkCmdPipelineBarrier(pcommandbuffer->m_vkcommandbuffer, pipelineStageFlagsOld, pipelineStageFlagsNew, 0, 0, NULL,
                           0, NULL, 1, &barrier);

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

         ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;

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

         ASSERT(m_bWithDepth);

         ::cast<texture> ptexture = get_depth_texture();

         return ptexture->m_vkimage;
      }
   }


   // texture* texture::_get_depth_texture()
   //{
   //
   //    if (m_etype & ::gpu::texture::e_type_depth)
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
      create_image_view();
      return m_vkimageview;
   }

   void texture::create_image_view()
   {

      
      ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;
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

      VkImageViewCreateInfo viewInfo = {.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                                        .image = m_vkimage,
                                        .viewType = viewType,
                                        .format = m_vkformat = pcontext->m_formatImageDefault,
                                        .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                                             .baseMipLevel = 0,
                                                             .levelCount = 1,
                                                             .baseArrayLayer = 0,
                                                             .layerCount = uLayerCount}};

      VK_CHECK_RESULT(vkCreateImageView(pcontext->logicalDevice(), &viewInfo, NULL, &m_vkimageview));



   }


   void texture::set_pixels(const ::int_rectangle &rectangle, const void *data)
   {

      VkDeviceSize size = rectangle.area() * 4;

      ::pointer<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;

      auto pbufferStaging =
         pcontext->create_buffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

      pbufferStaging->_assign(data, size);

      m_pgpurenderer->post_on_after_end_frame(
         [this, pcontext, pbufferStaging, rectangle]()
         {

            auto pcommandbuffer = pcontext->beginSingleTimeCommands(pcontext->transfer_queue());

            pcontext->copyBufferToImage(pcommandbuffer, this, pbufferStaging, rectangle);

            pcontext->endSingleTimeCommands(pcommandbuffer);

         });

      m_pgpurenderer->post_on_just_before_frame_next_start(
         [this, pcontext]()
         {

            auto pgpucommandbuffer = pcontext->beginSingleTimeCommands(pcontext->transfer_queue());

            ::cast<::gpu_vulkan::command_buffer> pcommandbuffer = pgpucommandbuffer; 

            _set_state(pcommandbuffer, 
               {

                  VK_ACCESS_TRANSFER_READ_BIT,
                  VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                  VK_PIPELINE_STAGE_TRANSFER_BIT

               });


            pcontext->endSingleTimeCommands(pcommandbuffer);

         });


   }


   VkDescriptorSet texture::descriptor_set(::gpu_vulkan::shader *pshader)
   {

      auto &shader = m_mapShader[pshader];

      if (!shader.m_bNew)
      {

         return shader.m_vkdescriptorset;
      }

      VkDescriptorImageInfo imageinfo;

      ::cast<context> pcontext = m_pgpurenderer->m_pgpucontext;

      imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      imageinfo.imageView = get_image_view();
      imageinfo.sampler = pcontext->_001VkSampler();

      auto &playout = pshader->m_psetdescriptorlayout;

      auto &ppool = pshader->m_pdescriptorpool;

      unsigned int uSamplerBinding = 0;

      if (pshader->m_bindingSampler.is_set())
         uSamplerBinding = pshader->m_bindingSampler.m_uBinding;
      else if (pshader->m_bindingCubeSampler.is_set())
         uSamplerBinding = pshader->m_bindingCubeSampler.m_uBinding;

      descriptor_writer(*playout, *ppool).writeImage(uSamplerBinding, &imageinfo).build(shader.m_vkdescriptorset);

      shader.m_bNew = false;

      return shader.m_vkdescriptorset;
   }


   VkFramebuffer texture::_framebuffer(::gpu_vulkan::render_pass *prenderpass)
   {

      if (m_bCpuRead)
      {

         return VK_NULL_HANDLE;
      }

      auto &framebuffer = m_mapFramebuffer[prenderpass->m_vkrenderpass];

      if (framebuffer)
      {
         return framebuffer;
      }

      ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;

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

      VkFramebufferCreateInfo fbInfo = {.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                                        .renderPass = prenderpass->m_vkrenderpass,
                                        .attachmentCount = (uint32_t)iAttachmentCount,
                                        .pAttachments = attachments,
                                        .width = (uint32_t)m_rectangleTarget.width(),
                                        .height = (uint32_t)m_rectangleTarget.height(),
                                        .layers = 1};

      VK_CHECK_RESULT(vkCreateFramebuffer(pcontext->logicalDevice(), &fbInfo, NULL, &framebuffer));

      return framebuffer;
   }


   VkImageView texture::get_depth_image_view()
   {

      if (m_etype & ::gpu::texture::e_type_depth)
      {

         if (!m_vkimageview)
         {

            ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;

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

         // ASSERT(m_bWithDepth);

         ::cast<texture> ptexture = get_depth_texture();

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
                                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_mipLevels,
                                       1); // layerCount

               auto pcommandbuffer = pcontext->beginSingleTimeCommands(nullptr);
               ::int_rectangle r(::int_point(0, 0), ::int_size(texWidth, texHeight));
               pcontext->copyBufferToImage(pcommandbuffer, this, pbuffer, r);
                                      // 1 // layerCount
      pcontext->endSingleTimeCommands(nullptr);
      pcontext->transitionImageLayout(
         m_vkimage, m_vkformat, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, m_mipLevels,
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
      UpdateDescriptor();

      return true;
   }


   
//void texture::TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t layerCount)
//   {
//      //VkCommandBuffer cmd = m_pDevice->beginSingleTimeCommands();
//      m_pDevice->transitionImageLayout(m_image, m_format, oldLayout, newLayout, m_mipLevels, layerCount);
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
   //   m_mipLevels = ktxTexture->numLevels;

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

   //      VK_CHECK_RESULT(vkCreateBuffer(pcontext->logicalDevice(), &bufferCreateInfo, nullptr, &stagingBuffer));

   //      // Get memory requirements for the staging buffer (alignment, memory type bits)
   //      vkGetBufferMemoryRequirements(pcontext->logicalDevice(), stagingBuffer, &memReqs);

   //      memAllocInfo.allocationSize = memReqs.size;
   //      // Get memory type index for a host visible buffer
   //      memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(
   //         memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

   //      VK_CHECK_RESULT(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &stagingMemory));
   //      VK_CHECK_RESULT(vkBindBufferMemory(pcontext->logicalDevice(), stagingBuffer, stagingMemory, 0));

   //      // Copy texture data into staging buffer
   //      uint8_t *data;
   //      VK_CHECK_RESULT(vkMapMemory(pcontext->logicalDevice(), stagingMemory, 0, memReqs.size, 0, (void **)&data));
   //      memcpy(data, ktxTextureData, ktxTextureSize);
   //      vkUnmapMemory(pcontext->logicalDevice(), stagingMemory);

   //      // Setup buffer copy regions for each mip level
   //      std::vector<VkBufferImageCopy> bufferCopyRegions;

   //      for (uint32_t i = 0; i < m_mipLevels; i++)
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
   //      imageCreateInfo.mipLevels = m_mipLevels;
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
   //      VK_CHECK_RESULT(vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &m_image));

   //      vkGetImageMemoryRequirements(pcontext->logicalDevice(), m_image, &memReqs);

   //      memAllocInfo.allocationSize = memReqs.size;

   //      memAllocInfo.memoryTypeIndex =
   //         pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
   //      VK_CHECK_RESULT(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &m_deviceMemory));
   //      VK_CHECK_RESULT(vkBindImageMemory(pcontext->logicalDevice(), m_image, m_deviceMemory, 0));

   //      VkImageSubresourceRange subresourceRange = {};
   //      subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   //      subresourceRange.baseMipLevel = 0;
   //      subresourceRange.levelCount = m_mipLevels;
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
   //      VK_CHECK_RESULT(vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &mappableImage));

   //      // Get memory requirements for this image
   //      // like size and alignment
   //      vkGetImageMemoryRequirements(pcontext->logicalDevice(), mappableImage, &memReqs);
   //      // Set memory allocation size to required memory size
   //      memAllocInfo.allocationSize = memReqs.size;

   //      // Get memory type that can be mapped to host memory
   //      memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(
   //         memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

   //      // Allocate host memory
   //      VK_CHECK_RESULT(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &mappableMemory));

   //      // Bind allocated image for use
   //      VK_CHECK_RESULT(vkBindImageMemory(pcontext->logicalDevice(), mappableImage, mappableMemory, 0));

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
   //      VK_CHECK_RESULT(vkMapMemory(pcontext->logicalDevice(), mappableMemory, 0, memReqs.size, 0, &data));

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
   //   samplerCreateInfo.maxLod = static_cast<float>(m_mipLevels);
   //   samplerCreateInfo.mipLodBias = 0.0f;
   //   samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;

   //   // Enable anisotropy if supported
   //   samplerCreateInfo.anisotropyEnable = device->m_enabledFeatures.samplerAnisotropy ? VK_TRUE : VK_FALSE;
   //   samplerCreateInfo.maxAnisotropy =
   //      samplerCreateInfo.anisotropyEnable ? device->m_deviceProperties.limits.maxSamplerAnisotropy : 1.0f;

   //   samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

   //   VK_CHECK_RESULT(vkCreateSampler(pcontext->logicalDevice(), &samplerCreateInfo, nullptr, &m_sampler));

   //   // Create image view
   //   VkImageViewCreateInfo viewCreateInfo{};
   //   viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
   //   viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
   //   viewCreateInfo.format = format;
   //   viewCreateInfo.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, m_mipLevels, 0, 1};
   //   viewCreateInfo.image = m_image;
   //   VK_CHECK_RESULT(vkCreateImageView(pcontext->logicalDevice(), &viewCreateInfo, nullptr, &m_view));

   //   // Update descriptor for shader sampling
   //   UpdateDescriptor();

   //   return true;
   //}




      bool texture::KTXLoadFromFile(const ::file::path  & path, VkFormat format,
                                          VkQueue copyQueue, VkImageUsageFlags imageUsageFlags,
                                          VkImageLayout imageLayout, bool forceLinear)
   {
      ktxTexture *ktxTexture;
      ktxResult result = ::gpu::loadKTXFile(this, path, &ktxTexture);
      assert(result == KTX_SUCCESS);

      ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;
      ::cast<::gpu_vulkan::device> pdevice = pcontext->m_pgpudevice;

      auto pphysicaldevice = pdevice->m_pphysicaldevice;

      //this->m_pDevice = device;
      m_rectangleTarget.left() = 0;
      m_rectangleTarget.top() = 0;
      // this->m_pDevice = pdevice;
      m_rectangleTarget.right() = ktxTexture->baseWidth;
      m_rectangleTarget.bottom() = ktxTexture->baseHeight;
      m_mipLevels = ktxTexture->numLevels;

      ktx_uint8_t *ktxTextureData = ktxTexture_GetData(ktxTexture);
      ktx_size_t ktxTextureSize = ktxTexture_GetDataSize(ktxTexture);

      VkFormatProperties formatProperties;
      vkGetPhysicalDeviceFormatProperties(pphysicaldevice->m_vkphysicaldevice, format, &formatProperties);

      VkBool32 useStaging = !forceLinear;

      VkMemoryAllocateInfo memAllocInfo = vkinit::memoryAllocateInfo();
      VkMemoryRequirements memReqs;

      //VkCommandBuffer pcommandbufferCmd->m_vkcommandbuffer = device->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

      auto pgpucommandbufferCmd = pcontext->beginSingleTimeCommands(pcontext->transfer_queue());

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

         VK_CHECK_RESULT(vkCreateBuffer(pcontext->logicalDevice(), &bufferCreateInfo, nullptr, &stagingBuffer));

         // Get memory requirements for the staging buffer (alignment, memory type bits)
         vkGetBufferMemoryRequirements(pcontext->logicalDevice(), stagingBuffer, &memReqs);

         memAllocInfo.allocationSize = memReqs.size;
         // Get memory type index for a host visible buffer
         memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(
            memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

         VK_CHECK_RESULT(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &stagingMemory));
         VK_CHECK_RESULT(vkBindBufferMemory(pcontext->logicalDevice(), stagingBuffer, stagingMemory, 0));

         // Copy texture data into staging buffer
         uint8_t *data;
         VK_CHECK_RESULT(vkMapMemory(pcontext->logicalDevice(), stagingMemory, 0, memReqs.size, 0, (void **)&data));
         memcpy(data, ktxTextureData, ktxTextureSize);
         vkUnmapMemory(pcontext->logicalDevice(), stagingMemory);

         // Setup buffer copy regions for each mip level
         std::vector<VkBufferImageCopy> bufferCopyRegions;

         for (uint32_t i = 0; i < m_mipLevels; i++)
         {
            ktx_size_t offset;
            KTX_error_code result = ktxTexture_GetImageOffset(ktxTexture, i, 0, 0, &offset);
            assert(result == KTX_SUCCESS);

            VkBufferImageCopy bufferCopyRegion = {};
            bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            bufferCopyRegion.imageSubresource.mipLevel = i;
            bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
            bufferCopyRegion.imageSubresource.layerCount = 1;
            bufferCopyRegion.imageExtent.width = std::max(1u, ktxTexture->baseWidth >> i);
            bufferCopyRegion.imageExtent.height = std::max(1u, ktxTexture->baseHeight >> i);
            bufferCopyRegion.imageExtent.depth = 1;
            bufferCopyRegion.bufferOffset = offset;


            bufferCopyRegions.push_back(bufferCopyRegion);
         }

         // Create optimal tiled target image
         VkImageCreateInfo imageCreateInfo = vkinit::imageCreateInfo();
         imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
         imageCreateInfo.format = format;
         imageCreateInfo.mipLevels = m_mipLevels;
         imageCreateInfo.arrayLayers = 1;
         imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
         imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
         imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
         imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
         imageCreateInfo.extent = 
         {
            (uint32_t)m_rectangleTarget.width(), 
            (uint32_t)m_rectangleTarget.height(), 1
         };
         imageCreateInfo.usage = imageUsageFlags;
         // Ensure that the TRANSFER_DST bit is set for staging
         if (!(imageCreateInfo.usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT))
         {
            imageCreateInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
         }
         VK_CHECK_RESULT(vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &m_vkimage));

         vkGetImageMemoryRequirements(pcontext->logicalDevice(), m_vkimage, &memReqs);

         memAllocInfo.allocationSize = memReqs.size;

         memAllocInfo.memoryTypeIndex =
            pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
         VK_CHECK_RESULT(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &m_vkdevicememory));
         VK_CHECK_RESULT(vkBindImageMemory(pcontext->logicalDevice(), m_vkimage, m_vkdevicememory, 0));

         VkImageSubresourceRange subresourceRange = {};
         subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         subresourceRange.baseMipLevel = 0;
         subresourceRange.levelCount = m_mipLevels;
         subresourceRange.layerCount = 1;


         // Image barrier for optimal image (target)
         // Optimal image will be used as destination for the copy
         vulkan::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, m_vkimage, VK_IMAGE_LAYOUT_UNDEFINED,
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               subresourceRange);

         // Copy mip levels from staging buffer
         vkCmdCopyBufferToImage(pcommandbufferCmd->m_vkcommandbuffer, stagingBuffer, m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                static_cast<uint32_t>(bufferCopyRegions.size()), bufferCopyRegions.data());

         // Change texture image layout to shader read after all mip levels have been copied
         this->m_state.m_vkimagelayout = imageLayout;
         ::vulkan::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, imageLayout, subresourceRange);

         //device->flushCommandBuffer(pcommandbufferCmd->m_vkcommandbuffer, copyQueue);

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
         assert(formatProperties.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);

         VkImage mappableImage;
         VkDeviceMemory mappableMemory;

         VkImageCreateInfo imageCreateInfo = vkinit::imageCreateInfo();
         imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
         imageCreateInfo.format = format;
         imageCreateInfo.extent = {
            (uint32_t) m_rectangleTarget.width(), 
            (uint32_t)m_rectangleTarget.height(),
            1};
         imageCreateInfo.mipLevels = 1;
         imageCreateInfo.arrayLayers = 1;
         imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
         imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
         imageCreateInfo.usage = imageUsageFlags;
         imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
         imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

         // Load mip map level 0 to linear tiling image
         VK_CHECK_RESULT(vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &mappableImage));

         // Get memory requirements for this image
         // like size and alignment
         vkGetImageMemoryRequirements(pcontext->logicalDevice(), mappableImage, &memReqs);
         // Set memory allocation size to required memory size
         memAllocInfo.allocationSize = memReqs.size;

         // Get memory type that can be mapped to host memory
         memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(
            memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

         // Allocate host memory
         VK_CHECK_RESULT(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &mappableMemory));

         // Bind allocated image for use
         VK_CHECK_RESULT(vkBindImageMemory(pcontext->logicalDevice(), mappableImage, mappableMemory, 0));

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
         VK_CHECK_RESULT(vkMapMemory(pcontext->logicalDevice(), mappableMemory, 0, memReqs.size, 0, &data));

         // Copy image data into memory
         memcpy(data, ktxTextureData, memReqs.size);

         vkUnmapMemory(pcontext->logicalDevice(), mappableMemory);

         // Linear tiled images don't need to be staged
         // and can be directly used as textures
         m_vkimage = mappableImage;
         m_vkdevicememory = mappableMemory;
         m_state.m_vkimagelayout = imageLayout;

         // Setup image memory barrier
         vulkan::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, m_vkimage, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_UNDEFINED, imageLayout);

         pcontext->endSingleTimeCommands(pcommandbufferCmd);

      }
      ktxTexture_Destroy(ktxTexture);
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
      samplerCreateInfo.maxLod = static_cast<float>(m_mipLevels);
      samplerCreateInfo.mipLodBias = 0.0f;
      samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;

      // Enable anisotropy if supported
      samplerCreateInfo.anisotropyEnable = pphysicaldevice->m_vkphysicaldevicefeatures.samplerAnisotropy ? VK_TRUE : VK_FALSE;
      samplerCreateInfo.maxAnisotropy =
         samplerCreateInfo.anisotropyEnable ? pphysicaldevice->m_vkphysicaldeviceproperties.limits.maxSamplerAnisotropy : 1.0f;

      samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

      VK_CHECK_RESULT(vkCreateSampler(pcontext->logicalDevice(), &samplerCreateInfo, nullptr, &m_vksamplerDedicated));

      // Create image view
      VkImageViewCreateInfo viewCreateInfo{};
      viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      viewCreateInfo.format = format;
      viewCreateInfo.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, (uint32_t) m_mipLevels, 0, 1};
      viewCreateInfo.image = m_vkimage;
      VK_CHECK_RESULT(vkCreateImageView(pcontext->logicalDevice(), &viewCreateInfo, nullptr, &m_vkimageview));

      // Update descriptor for shader sampling
      UpdateDescriptor();

      return true;
   }

   // // Loads a cubemap from a single KTX file
   // void texture::KtxLoadCubemapFromFile(const ::scoped_string& name, ::string filename, VkFormat format, device*
   // pdevice, VkQueue copyQueue, VkImageUsageFlags imageUsageFlags, VkImageLayout imageLayout)
   // {
   // 	m_bIsCubemap = true;
   //
   // 	ktxTexture* ktxTexture;
   // 	ktxResult result = loadKTXFile(filename, &ktxTexture);
   // 	ASSERT(result == KTX_SUCCESS);
   //
   // 	this->m_pDevice = pdevice;
   // 	m_rectangleTarget.width() = ktxTexture->baseWidth;
   // 	m_rectangleTarget.height() = ktxTexture->baseHeight;
   // 	m_mipLevels = ktxTexture->numLevels;
   //
   // 	ktx_uint8_t* ktxTextureData = ktxTexture_GetData(ktxTexture);
   // 	ktx_size_t ktxTextureSize = ktxTexture_GetSize(ktxTexture);
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
   // 	VK_CHECK_RESULT(vkCreateBuffer(pcontext->logicalDevice(), &bufferCreateInfo, nullptr, &stagingBuffer));
   //
   // 	// Get memory requirements for the staging buffer (alignment, memory type bits)
   // 	vkGetBufferMemoryRequirements(pcontext->logicalDevice(), stagingBuffer, &memReqs);
   //
   // 	memAllocInfo.allocationSize = memReqs.size;
   // 	// Get memory type index for a host visible buffer
   // 	memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(memReqs.memoryTypeBits,
   // VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
   //
   // 	VK_CHECK_RESULT(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &stagingMemory));
   // 	VK_CHECK_RESULT(vkBindBufferMemory(pcontext->logicalDevice(), stagingBuffer, stagingMemory, 0));
   //
   // 	// Copy texture data into staging buffer
   // 	uint8_t* data;
   // 	VK_CHECK_RESULT(vkMapMemory(pcontext->logicalDevice(), stagingMemory, 0, memReqs.size, 0, (void**)&data));
   // 	memcpy(data, ktxTextureData, ktxTextureSize);
   // 	vkUnmapMemory(pcontext->logicalDevice(), stagingMemory);
   //
   // 	// Setup buffer copy regions for each face including all of its mip levels
   // 	::array_base<VkBufferImageCopy> bufferCopyRegions;
   //
   // 	for (uint32_t face = 0; face < 6; face++)
   // 	{
   // 		for (uint32_t level = 0; level < m_mipLevels; level++)
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
   // 			bufferCopyRegion.imageExtent.width = ktxTexture->baseWidth >> level;
   // 			bufferCopyRegion.imageExtent.height = ktxTexture->baseHeight >> level;
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
   // 	imageCreateInfo.mipLevels = m_mipLevels;
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
   // 	VK_CHECK_RESULT(vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &m_vkimage));
   //
   // 	vkGetImageMemoryRequirements(pcontext->logicalDevice(), m_vkimage, &memReqs);
   //
   // 	memAllocInfo.allocationSize = memReqs.size;
   // 	memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(memReqs.memoryTypeBits,
   // VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
   //
   // 	VK_CHECK_RESULT(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &m_vkdevicememory));
   // 	VK_CHECK_RESULT(vkBindImageMemory(pcontext->logicalDevice(), m_vkimage, m_vkdevicememory, 0));
   //
   // 	// Use a separate command buffer for texture loading
   // 	VkCommandBuffer pcommandbufferCopy->m_vkcommandbuffer = pdevice->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
   //
   // 	// Image barrier for optimal image (target)
   // 	// Set initial layout for all array layers (faces) of the optimal (target) tiled texture
   // 	VkImageSubresourceRange subresourceRange = {};
   // 	subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   // 	subresourceRange.baseMipLevel = 0;
   // 	subresourceRange.levelCount = m_mipLevels;
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
   // VK_COMPARE_OP_NEVER; 	samplerCreateInfo.minLod = 0.0f; 	samplerCreateInfo.maxLod = (float)m_mipLevels;
   // 	samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
   // 	VK_CHECK_RESULT(vkCreateSampler(pcontext->logicalDevice(), &samplerCreateInfo, nullptr, &m_sampler));
   //
   // 	// Create image view
   // 	VkImageViewCreateInfo viewCreateInfo = vkinit::imageViewCreateInfo();
   // 	viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
   // 	viewCreateInfo.format = format;
   // 	viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   // 	viewCreateInfo.subresourceRange.baseMipLevel = 0;
   // 	viewCreateInfo.subresourceRange.levelCount = m_mipLevels;
   // 	viewCreateInfo.subresourceRange.baseArrayLayer = 0;
   // 	viewCreateInfo.subresourceRange.layerCount = 6;
   // 	viewCreateInfo.image = m_vkimage;
   // 	VK_CHECK_RESULT(vkCreateImageView(pcontext->logicalDevice(), &viewCreateInfo, nullptr, &m_view));
   //
   // 	// Clean up staging resources
   // 	ktxTexture_Destroy(ktxTexture);
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
      m_etype = e_type_cube_map;

      ktxTexture *ktxTexture;
      ktxResult result = loadKTXFile(this, filename, &ktxTexture);
      ASSERT(result == KTX_SUCCESS);

      ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;

      ::cast<::gpu_vulkan::device> pgpudevice = pcontext->m_pgpudevice;

      auto pphysicaldevice = pgpudevice->m_pphysicaldevice;

      m_rectangleTarget.left() = 0;
      m_rectangleTarget.top() = 0;
      //this->m_pDevice = pdevice;
      m_rectangleTarget.right() = ktxTexture->baseWidth;
      m_rectangleTarget.bottom() = ktxTexture->baseHeight;
      m_mipLevels = ktxTexture->numLevels;

      ktx_uint8_t *ktxTextureData = ktxTexture_GetData(ktxTexture);
      ktx_size_t ktxTextureSize = ktxTexture_GetDataSize(ktxTexture);

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

      VK_CHECK_RESULT(vkCreateBuffer(pcontext->logicalDevice(), &bufferCreateInfo, nullptr, &stagingBuffer));

      // Get memory requirements for the staging buffer (alignment, memory type bits)
      vkGetBufferMemoryRequirements(pcontext->logicalDevice(), stagingBuffer, &memReqs);

      memAllocInfo.allocationSize = memReqs.size;
      // Get memory type index for a host visible buffer
      memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(
         memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

      VK_CHECK_RESULT(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &stagingMemory));
      VK_CHECK_RESULT(vkBindBufferMemory(pcontext->logicalDevice(), stagingBuffer, stagingMemory, 0));

      // Copy texture data into staging buffer
      uint8_t *data;
      VK_CHECK_RESULT(vkMapMemory(pcontext->logicalDevice(), stagingMemory, 0, memReqs.size, 0, (void **)&data));
      memcpy(data, ktxTextureData, ktxTextureSize);
      vkUnmapMemory(pcontext->logicalDevice(), stagingMemory);

      // Setup buffer copy regions for each face including all of its mip levels
      ::array_base<VkBufferImageCopy> bufferCopyRegions;

      for (uint32_t face = 0; face < 6; face++)
      {
         for (uint32_t level = 0; level < m_mipLevels; level++)
         {
            ktx_size_t offset;
            KTX_error_code result = ktxTexture_GetImageOffset(ktxTexture, level, 0, face, &offset);
            ASSERT(result == KTX_SUCCESS);

            VkBufferImageCopy bufferCopyRegion = {};
            bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            bufferCopyRegion.imageSubresource.mipLevel = level;
            bufferCopyRegion.imageSubresource.baseArrayLayer = face;
            bufferCopyRegion.imageSubresource.layerCount = 1;
            bufferCopyRegion.imageExtent.width = ktxTexture->baseWidth >> level;
            bufferCopyRegion.imageExtent.height = ktxTexture->baseHeight >> level;
            bufferCopyRegion.imageExtent.depth = 1;
            bufferCopyRegion.bufferOffset = offset & ~0xF;

            bufferCopyRegions.add(bufferCopyRegion);
         }
      }

      // Create optimal tiled target image
      VkImageCreateInfo imageCreateInfo = vkinit::imageCreateInfo();
      imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
      imageCreateInfo.format = format;
      imageCreateInfo.mipLevels = m_mipLevels;
      imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
      imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
      imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
      imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      imageCreateInfo.extent = 
      {
         (uint32_t) m_rectangleTarget.width(), 
         (uint32_t) m_rectangleTarget.height(), 
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


      VK_CHECK_RESULT(vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &m_vkimage));

      vkGetImageMemoryRequirements(pcontext->logicalDevice(), m_vkimage, &memReqs);

      memAllocInfo.allocationSize = memReqs.size;
      memAllocInfo.memoryTypeIndex =
         pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

      VK_CHECK_RESULT(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &m_vkdevicememory));
      VK_CHECK_RESULT(vkBindImageMemory(pcontext->logicalDevice(), m_vkimage, m_vkdevicememory, 0));

      // Use a separate command buffer for texture loading
      //VkCommandBuffer pcommandbufferCopy->m_vkcommandbuffer = pdevice->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

      auto pgpucommandbufferCopy = pcontext->beginSingleTimeCommands(pcontext->transfer_queue());

      ::cast<command_buffer> pcommandbufferCopy = pgpucommandbufferCopy;

      // Image barrier for optimal image (target)
      // Set initial layout for all array layers (faces) of the optimal (target) tiled texture
      VkImageSubresourceRange subresourceRange = {};
      subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      subresourceRange.baseMipLevel = 0;
      subresourceRange.levelCount = m_mipLevels;
      subresourceRange.layerCount = 6;

      vulkan::setImageLayout(pcommandbufferCopy->m_vkcommandbuffer, m_vkimage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                            subresourceRange);

      // Copy the cube map faces from the staging buffer to the optimal tiled image
      vkCmdCopyBufferToImage(pcommandbufferCopy->m_vkcommandbuffer, stagingBuffer, m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                             static_cast<uint32_t>(bufferCopyRegions.size()), bufferCopyRegions.data());

      // Change texture image layout to shader read after all faces have been copied
      m_state.m_vkimagelayout = imageLayout;
      vulkan::setImageLayout(pcommandbufferCopy->m_vkcommandbuffer, m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, imageLayout, subresourceRange);

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
      samplerCreateInfo.maxLod = (float)m_mipLevels;
      samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
      VK_CHECK_RESULT(vkCreateSampler(pcontext->logicalDevice(), &samplerCreateInfo, nullptr, &m_vksamplerDedicated));

      // Create image view
      VkImageViewCreateInfo viewCreateInfo = vkinit::imageViewCreateInfo();
      viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
      viewCreateInfo.format = format;
      viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      viewCreateInfo.subresourceRange.baseMipLevel = 0;
      viewCreateInfo.subresourceRange.levelCount = m_mipLevels;
      viewCreateInfo.subresourceRange.baseArrayLayer = 0;
      viewCreateInfo.subresourceRange.layerCount = 6;
      viewCreateInfo.image = m_vkimage;
      VK_CHECK_RESULT(vkCreateImageView(pcontext->logicalDevice(), &viewCreateInfo, nullptr, &m_vkimageview));

      // Clean up staging resources
      ktxTexture_Destroy(ktxTexture);
      vkDestroyBuffer(pcontext->logicalDevice(), stagingBuffer, nullptr);
      vkFreeMemory(pcontext->logicalDevice(), stagingMemory, nullptr);

      // Update descriptor image info member that can be used for setting up descriptor sets
      UpdateDescriptor();

   }


   
	void texture::UpdateDescriptor()
   {

      if (m_vksamplerDedicated)
      {
         m_descriptor3.sampler = m_vksamplerDedicated;
      }
      else if (m_vksampler3)
      {

         m_descriptor3.sampler = m_vksampler3;
      }
      else
      {
         
         ::cast<context> pcontext = m_pgpurenderer->m_pgpucontext;
         
         m_descriptor3.sampler = pcontext->_001VkSampler();

      }
      m_descriptor3.imageView = m_vkimageview;
      m_descriptor3.imageLayout = m_state.m_vkimagelayout;
   }


   bool texture::is_in_shader_sampling_state()
   {

      if (m_state.m_vkimagelayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
      {

         return true;

      }
      else
      {

         return false;

      }

   }


   void texture::initialize_image_texture(::gpu::renderer *pgpurenderer, const ::file::path &pathImage, bool isSrgb)
   {
      this->m_pgpurenderer = pgpurenderer;

      _fromglTfImage(nullptr, pathImage, pgpurenderer, isSrgb);

   }

   void texture::_fromglTfImage(tinygltf::Image *pgltfimage, const ::file::path & path,
                               ::gpu::renderer * pgpurenderer, bool isSrgb)
   {
      this->m_pgpurenderer = pgpurenderer;
      ::cast<::gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;
      ::cast<::gpu_vulkan::device> pgpudevice = pcontext->m_pgpudevice;
      ::cast<::gpu_vulkan::renderer> prenderer = pgpurenderer;
      auto pphysicaldevice = pgpudevice->m_pphysicaldevice;

      bool isKtx = false;
      // Image points to an external ktx file
      if (pgltfimage && ::string(pgltfimage->uri.c_str()).case_insensitive_ends(".ktx"))
      {
         isKtx = true;
      }
      ::cast<::gpu_vulkan::queue> pgpuqueueTransfer = pcontext->m_pqueueTransfer;
      VkQueue copyQueue = pgpuqueueTransfer->m_vkqueue;
      VkFormat format;

      if (!isKtx && pgltfimage)
      {
         // Texture was loaded using STB_Image

         unsigned char *buffer = nullptr;
         VkDeviceSize bufferSize = 0;
         bool deleteBuffer = false;
         if (pgltfimage->component == 3)
         {
            // Most devices don't support RGB only on Vulkan so convert if necessary
            // TODO: Check actual format support and transform only if required
            bufferSize = pgltfimage->width * pgltfimage->height * 4;
            buffer = new unsigned char[bufferSize];
            unsigned char *rgba = buffer;
            unsigned char *rgb = &pgltfimage->image[0];
            for (size_t i = 0; i < pgltfimage->width * pgltfimage->height; ++i)
            {
               for (int32_t j = 0; j < 3; ++j)
               {
                  rgba[j] = rgb[j];
               }
               rgba += 4;
               rgb += 3;
            }
            deleteBuffer = true;
         }
         else
         {
            buffer = &pgltfimage->image[0];
            bufferSize = pgltfimage->image.size();
         }

         format = isSrgb ? VK_FORMAT_R8G8B8A8_SRGB : VK_FORMAT_R8G8B8A8_UNORM;

         VkFormatProperties formatProperties;


         m_rectangleTarget.set_width(pgltfimage->width);
         m_rectangleTarget.set_height(pgltfimage->height);
         m_mipLevels = (uint32_t)(floor(::log2((double)::maximum(m_rectangleTarget.width(),
            m_rectangleTarget.height()))) + 1.0);

         vkGetPhysicalDeviceFormatProperties(pphysicaldevice->m_vkphysicaldevice, format, &formatProperties);
         assert(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT);
         assert(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT);

         VkMemoryAllocateInfo memAllocInfo{};
         memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
         VkMemoryRequirements memReqs{};

         VkBuffer stagingBuffer;
         VkDeviceMemory stagingMemory;

         VkBufferCreateInfo bufferCreateInfo{};
         bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
         bufferCreateInfo.size = bufferSize;
         bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
         bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
         VK_CHECK_RESULT(vkCreateBuffer(pcontext->logicalDevice(), &bufferCreateInfo, nullptr, &stagingBuffer));
         vkGetBufferMemoryRequirements(pcontext->logicalDevice(), stagingBuffer, &memReqs);
         memAllocInfo.allocationSize = memReqs.size;
         memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(
            memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
         VK_CHECK_RESULT(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &stagingMemory));
         VK_CHECK_RESULT(vkBindBufferMemory(pcontext->logicalDevice(), stagingBuffer, stagingMemory, 0));

         uint8_t *data;
         VK_CHECK_RESULT(vkMapMemory(pcontext->logicalDevice(), stagingMemory, 0, memReqs.size, 0, (void **)&data));
         memcpy(data, buffer, bufferSize);
         vkUnmapMemory(pcontext->logicalDevice(), stagingMemory);

         VkImageCreateInfo imageCreateInfo{};
         imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
         imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
         imageCreateInfo.format = format;
         imageCreateInfo.mipLevels = m_mipLevels;
         imageCreateInfo.arrayLayers = 1;
         imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
         imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
         imageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
         imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
         imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
         imageCreateInfo.extent = {(uint32_t) m_rectangleTarget.width(), (uint32_t)m_rectangleTarget.height(), 1};
         imageCreateInfo.usage =
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
         VK_CHECK_RESULT(vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &m_vkimage));
         vkGetImageMemoryRequirements(pcontext->logicalDevice(), m_vkimage, &memReqs);
         memAllocInfo.allocationSize = memReqs.size;
         memAllocInfo.memoryTypeIndex =
            pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
         VK_CHECK_RESULT(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &m_vkdevicememory));
         VK_CHECK_RESULT(vkBindImageMemory(pcontext->logicalDevice(), m_vkimage, m_vkdevicememory, 0));

         // VkCommandBuffer pcommandbufferCopy->m_vkcommandbuffer =
         // pcontext->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

         auto pgpucommandbufferCopy = pcontext->beginSingleTimeCommands(pcontext->transfer_queue());

         ::cast<command_buffer> pcommandbufferCopy = pgpucommandbufferCopy;

         VkImageSubresourceRange subresourceRange = {};
         subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         subresourceRange.levelCount = 1;
         subresourceRange.layerCount = 1;

         VkImageMemoryBarrier imageMemoryBarrier{};

         imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
         imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
         imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
         imageMemoryBarrier.srcAccessMask = 0;
         imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
         imageMemoryBarrier.image = m_vkimage;
         imageMemoryBarrier.subresourceRange = subresourceRange;
         vkCmdPipelineBarrier(pcommandbufferCopy->m_vkcommandbuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                              VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

         VkBufferImageCopy bufferCopyRegion = {};
         bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         bufferCopyRegion.imageSubresource.mipLevel = 0;
         bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
         bufferCopyRegion.imageSubresource.layerCount = 1;
         bufferCopyRegion.imageExtent.width = m_rectangleTarget.width();
         bufferCopyRegion.imageExtent.height = m_rectangleTarget.height();
         bufferCopyRegion.imageExtent.depth = 1;

         vkCmdCopyBufferToImage(pcommandbufferCopy->m_vkcommandbuffer, stagingBuffer, m_vkimage,
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferCopyRegion);

         imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
         imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
         imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
         imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
         imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
         imageMemoryBarrier.image = m_vkimage;
         imageMemoryBarrier.subresourceRange = subresourceRange;
         vkCmdPipelineBarrier(pcommandbufferCopy->m_vkcommandbuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                              VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

         pcontext->endSingleTimeCommands(pcommandbufferCopy);

         vkDestroyBuffer(pcontext->logicalDevice(), stagingBuffer, nullptr);
         vkFreeMemory(pcontext->logicalDevice(), stagingMemory, nullptr);

         // Generate the mip chain (glTF uses jpg and png, so we need to create this manually)
         // VkCommandBuffer blitCmd = device->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
         auto pgpucommandbufferBlit = pcontext->beginSingleTimeCommands(pcontext->transfer_queue());
         ::cast<command_buffer> pcommandbufferBlit = pgpucommandbufferBlit;
         for (uint32_t i = 1; i < m_mipLevels; i++)
         {
            VkImageBlit imageBlit{};

            imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageBlit.srcSubresource.layerCount = 1;
            imageBlit.srcSubresource.mipLevel = i - 1;
            imageBlit.srcOffsets[1].x = int32_t(m_rectangleTarget.width() >> (i - 1));
            imageBlit.srcOffsets[1].y = int32_t(m_rectangleTarget.height() >> (i - 1));
            imageBlit.srcOffsets[1].z = 1;

            imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageBlit.dstSubresource.layerCount = 1;
            imageBlit.dstSubresource.mipLevel = i;
            imageBlit.dstOffsets[1].x = int32_t(m_rectangleTarget.width() >> i);
            imageBlit.dstOffsets[1].y = int32_t(m_rectangleTarget.height() >> i);
            imageBlit.dstOffsets[1].z = 1;

            VkImageSubresourceRange mipSubRange = {};
            mipSubRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            mipSubRange.baseMipLevel = i;
            mipSubRange.levelCount = 1;
            mipSubRange.layerCount = 1;

            {
               VkImageMemoryBarrier imageMemoryBarrier{};
               imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
               imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
               imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
               imageMemoryBarrier.srcAccessMask = 0;
               imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
               imageMemoryBarrier.image = m_vkimage;
               imageMemoryBarrier.subresourceRange = mipSubRange;
               vkCmdPipelineBarrier(pcommandbufferBlit->m_vkcommandbuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                    VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
            }

            vkCmdBlitImage(pcommandbufferBlit->m_vkcommandbuffer, m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_vkimage,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageBlit, VK_FILTER_LINEAR);

            {
               VkImageMemoryBarrier imageMemoryBarrier{};
               imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
               imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
               imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
               imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
               imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
               imageMemoryBarrier.image = m_vkimage;
               imageMemoryBarrier.subresourceRange = mipSubRange;
               vkCmdPipelineBarrier(pcommandbufferBlit->m_vkcommandbuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                    VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
            }
         }

         subresourceRange.levelCount = m_mipLevels;
         m_state.m_vkimagelayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

         imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
         imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
         imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
         imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
         imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
         imageMemoryBarrier.image = m_vkimage;
         imageMemoryBarrier.subresourceRange = subresourceRange;
         vkCmdPipelineBarrier(pcommandbufferBlit->m_vkcommandbuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                              VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

         if (deleteBuffer)
         {
            delete[] buffer;
         }

         // pcontext->endSingleTimeCommands(blitCmd, copyQueue, true);

         pcontext->endSingleTimeCommands(pcommandbufferBlit);
      }
      else
      {
         // Texture is stored in an external ktx file
         ::file::path filename = path;
         
         if (pgltfimage)
         {

            filename /= pgltfimage->uri.c_str();

         }

         ktxTexture *ktxTexture;

         ktxResult result = KTX_SUCCESS;
#if defined(__ANDROID__)
         AAsset *asset =
            AAssetManager_open(androidApp->activity->assetManager, filename.c_str(), AASSET_MODE_STREAMING);
         if (!asset)
         {
            vks::tools::exitFatal("Could not load texture from " + filename +
                                     "\n\nMake sure the assets submodule has been checked out and is up-to-date.",
                                  -1);
         }
         size_t size = AAsset_getLength(asset);
         assert(size > 0);
         ktx_uint8_t *textureData = new ktx_uint8_t[size];
         AAsset_read(asset, textureData, size);
         AAsset_close(asset);
         result = ktxTexture_CreateFromMemory(textureData, size, KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &ktxTexture);
         delete[] textureData;
#else
         // if (!tools::fileExists(filename))
         //{
         //    tools::exitFatal("Could not load texture from " + filename +
         //                        "\n\nMake sure the assets submodule has been checked out and is up-to-date.",
         //                     -1);
         // }
         if (!pcontext->file()->exists(filename))
         {
            throw ::exception(error_failed,
                              "Could not load texture from " + ::string(filename) +
                                 "\n\nMake sure the assets submodule has been checked out and is up-to-date.");
         }

         auto memory = pcontext->file()->as_memory(filename);
         result = ktxTexture_CreateFromMemory(memory.data(), memory.size(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT,
                                              &ktxTexture);
         // delete[] textureData;

         // result = ktxTexture_CreateFromNamedFile(filename.c_str(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT,
         // &ktxTexture);
#endif
         assert(result == KTX_SUCCESS);




         this->m_pgpurenderer = pcontext->m_pgpurenderer;

         m_rectangleTarget.set_width(ktxTexture->baseWidth);
         m_rectangleTarget.set_height(ktxTexture->baseHeight);
         m_mipLevels = ktxTexture->numLevels;

         ktx_uint8_t *ktxTextureData = ktxTexture_GetData(ktxTexture);
         ktx_size_t ktxTextureSize = ktxTexture_GetDataSize(ktxTexture);
         m_vkformat = ktxTexture_GetVkFormat(ktxTexture);

         // Get device properties for the requested texture format
         VkFormatProperties formatProperties;
         vkGetPhysicalDeviceFormatProperties(pphysicaldevice->m_vkphysicaldevice, m_vkformat, &formatProperties);

         // VkCommandBuffer pcommandbufferCopy->m_vkcommandbuffer =
         // device->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

         auto pgpucommandbufferCopy = pcontext->beginSingleTimeCommands(pcontext->transfer_queue());

         ::cast<command_buffer> pcommandbufferCopy = pgpucommandbufferCopy;

         VkBuffer stagingBuffer;
         VkDeviceMemory stagingMemory;

         VkBufferCreateInfo bufferCreateInfo = vkinit::bufferCreateInfo();
         bufferCreateInfo.size = ktxTextureSize;
         // This buffer is used as a transfer source for the buffer copy
         bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
         bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
         VK_CHECK_RESULT(vkCreateBuffer(pcontext->logicalDevice(), &bufferCreateInfo, nullptr, &stagingBuffer));

         VkMemoryAllocateInfo memAllocInfo = vkinit::memoryAllocateInfo();
         VkMemoryRequirements memReqs;
         vkGetBufferMemoryRequirements(pcontext->logicalDevice(), stagingBuffer, &memReqs);
         memAllocInfo.allocationSize = memReqs.size;
         memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(
            memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
         VK_CHECK_RESULT(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &stagingMemory));
         VK_CHECK_RESULT(vkBindBufferMemory(pcontext->logicalDevice(), stagingBuffer, stagingMemory, 0));

         uint8_t *data;
         VK_CHECK_RESULT(vkMapMemory(pcontext->logicalDevice(), stagingMemory, 0, memReqs.size, 0, (void **)&data));
         memcpy(data, ktxTextureData, ktxTextureSize);
         vkUnmapMemory(pcontext->logicalDevice(), stagingMemory);

         ::array_base<VkBufferImageCopy> bufferCopyRegions;
         for (uint32_t i = 0; i < m_mipLevels; i++)
         {
            ktx_size_t offset;
            KTX_error_code result = ktxTexture_GetImageOffset(ktxTexture, i, 0, 0, &offset);
            assert(result == KTX_SUCCESS);
            VkBufferImageCopy bufferCopyRegion = {};
            bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            bufferCopyRegion.imageSubresource.mipLevel = i;
            bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
            bufferCopyRegion.imageSubresource.layerCount = 1;
            bufferCopyRegion.imageExtent.width = std::max(1u, ktxTexture->baseWidth >> i);
            bufferCopyRegion.imageExtent.height = std::max(1u, ktxTexture->baseHeight >> i);
            bufferCopyRegion.imageExtent.depth = 1;
            bufferCopyRegion.bufferOffset = offset;
            bufferCopyRegions.add(bufferCopyRegion);
         }

         // Create optimal tiled target image
         VkImageCreateInfo imageCreateInfo = vkinit::imageCreateInfo();
         imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
         imageCreateInfo.format = m_vkformat;
         imageCreateInfo.mipLevels = m_mipLevels;
         imageCreateInfo.arrayLayers = 1;
         imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
         imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
         imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
         imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
         imageCreateInfo.extent.width = m_rectangleTarget.width();
         imageCreateInfo.extent.height = m_rectangleTarget.height();
         imageCreateInfo.extent.depth = 1;
         imageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
         VK_CHECK_RESULT(vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &m_vkimage));

         vkGetImageMemoryRequirements(pcontext->logicalDevice(), m_vkimage, &memReqs);
         memAllocInfo.allocationSize = memReqs.size;
         memAllocInfo.memoryTypeIndex =
            pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
         VK_CHECK_RESULT(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &m_vkdevicememory));
         VK_CHECK_RESULT(vkBindImageMemory(pcontext->logicalDevice(), m_vkimage, m_vkdevicememory, 0));

         VkImageSubresourceRange subresourceRange = {};
         subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         subresourceRange.baseMipLevel = 0;
         subresourceRange.levelCount = m_mipLevels;
         subresourceRange.layerCount = 1;

         ::vulkan::setImageLayout(pcommandbufferCopy->m_vkcommandbuffer, m_vkimage, VK_IMAGE_LAYOUT_UNDEFINED,
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourceRange);
         vkCmdCopyBufferToImage(pcommandbufferCopy->m_vkcommandbuffer, stagingBuffer, m_vkimage,
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, static_cast<uint32_t>(bufferCopyRegions.size()),
                                bufferCopyRegions.data());
         ::vulkan::setImageLayout(pcommandbufferCopy->m_vkcommandbuffer, m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                  VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, subresourceRange);
         pcontext->endSingleTimeCommands(pcommandbufferCopy);
         // pcontext->endSingleTimeCommands(pcommandbufferCopy->m_vkcommandbuffer, copyQueue);
         m_state.m_vkimagelayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

         vkDestroyBuffer(pcontext->logicalDevice(), stagingBuffer, nullptr);
         vkFreeMemory(pcontext->logicalDevice(), stagingMemory, nullptr);

         ktxTexture_Destroy(ktxTexture);
      }

      on_finish_load_texture();

   }


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
      samplerInfo.maxLod = (float)m_mipLevels;
      samplerInfo.maxAnisotropy = 8.0f;
      // samplerInfo.anisotropyEnable = VK_TRUE;
      VK_CHECK_RESULT(vkCreateSampler(pcontext->logicalDevice(), &samplerInfo, nullptr, &m_vksampler3));

      VkImageViewCreateInfo viewInfo{};
      viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      viewInfo.image = m_vkimage;
      viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      viewInfo.format = m_vkformat;
      viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      viewInfo.subresourceRange.layerCount = 1;
      viewInfo.subresourceRange.levelCount = m_mipLevels;
      VK_CHECK_RESULT(vkCreateImageView(pcontext->logicalDevice(), &viewInfo, nullptr, &m_vkimageview));

      m_descriptor3.sampler = m_vksampler3;
      m_descriptor3.imageView = m_vkimageview;
      m_descriptor3.imageLayout = m_state.m_vkimagelayout;

   }




} // namespace gpu_vulkan

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

      new_texture.set_new_texture();

   }


   texture::~texture()
   {

   }


   void texture::initialize_gpu_texture(::gpu::renderer* prenderer, const ::int_size& size) //, bool bCreateRenderTargetView, bool bCreateShaderResourceView)
   {

      ::gpu::texture::initialize_gpu_texture(prenderer, size);

      m_vkimagelayout = VK_IMAGE_LAYOUT_UNDEFINED;

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

      ::cast < context > pgpucontext = pcontext;

      ::cast < render_pass > prenderpass = m_pgpurenderer->m_pgpurendertarget;

      //// Find a suitable depth format
      VkFormat fbDepthFormat;
      VkBool32 validDepthFormat = ::vulkan::getSupportedDepthFormat(
         pcontext->m_pgpudevice->m_pphysicaldevice->m_physicaldevice, &fbDepthFormat);
      ASSERT(validDepthFormat);

      //// Color attachment
      VkImageCreateInfo imagecreateinfo = ::vulkan::initializers::imageCreateInfo();
      imagecreateinfo.imageType = VK_IMAGE_TYPE_2D;
      imagecreateinfo.format = prenderpass->m_formatImage;
      imagecreateinfo.extent.width = m_size.cx();
      imagecreateinfo.extent.height = m_size.cy();
      imagecreateinfo.extent.depth = 1;
      imagecreateinfo.mipLevels = 1;
      imagecreateinfo.arrayLayers = 1;
      imagecreateinfo.samples = VK_SAMPLE_COUNT_1_BIT;
      imagecreateinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
      //// We will sample directly from the color attachment
      imagecreateinfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT |
         VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
      imagecreateinfo.initialLayout = m_vkimagelayout;

      pcontext->createImageWithInfo(
         imagecreateinfo,
         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
         m_vkimage,
         m_vkdevicememory);

   }


   void texture::_new_state(::gpu_vulkan::command_buffer* pcommandbuffer, VkImageLayout newLayout)
   {

      auto image = m_vkimage;

      auto layoutOld = m_vkimagelayout;

      auto layoutNew = newLayout;

      VkCommandBufferBeginInfo beginInfo = 
      {
          
         .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO

      };

      vkBeginCommandBuffer(pcommandbuffer->m_vkcommandbuffer, &beginInfo);

      VkImageMemoryBarrier barrier = {
          .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
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
         VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
         0,
         0, NULL,
         0, NULL,
         1, &barrier
      );

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


} // namespace gpu_vulkan





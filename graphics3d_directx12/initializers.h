#pragma once


#include "types.h"


namespace graphics3d_directx12
{


   namespace initializers
   {

      ////> init_cmd
      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkCommandPoolCreateInfo command_pool_create_info(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);
      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkCommandBufferAllocateInfo command_buffer_allocate_info(VkCommandPool pool, uint32_t count = 1);
      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkCommandBufferAllocateInfo command_buffer_allocate_info(
      //   VkCommandPool commandPool,
      //   VkCommandBufferLevel level,
      //   uint32_t bufferCount);
      ////< init_cmd

      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkCommandBufferBeginInfo command_buffer_begin_info(VkCommandBufferUsageFlags flags = 0);
      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkCommandBufferSubmitInfo command_buffer_submit_info(VkCommandBuffer cmd);

      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkFenceCreateInfo fence_create_info(VkFenceCreateFlags flags = 0);

      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkSemaphoreCreateInfo semaphore_create_info(VkSemaphoreCreateFlags flags = 0);

      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkSubmitInfo2 submit_info(VkCommandBufferSubmitInfo* cmd, VkSemaphoreSubmitInfo* signalSemaphoreInfo,
      //   VkSemaphoreSubmitInfo* waitSemaphoreInfo);
      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkPresentInfoKHR present_info();

      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkRenderingAttachmentInfo attachment_info(VkImageView view, VkClearValue* clear, VkImageLayout layout /*= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL*/);

      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkRenderingAttachmentInfo depth_attachment_info(VkImageView view,
      //   VkImageLayout layout /*= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL*/);

      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkRenderingInfo rendering_info(VkExtent2D renderExtent, VkRenderingAttachmentInfo* colorAttachment,
      //   VkRenderingAttachmentInfo* depthAttachment);

      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkImageSubresourceRange image_subresource_range(VkImageAspectFlags aspectMask);

      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkSemaphoreSubmitInfo semaphore_submit_info(VkPipelineStageFlags2 stageMask, VkSemaphore semaphore);
      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkDescriptorSetLayoutBinding descriptorset_layout_binding(VkDescriptorType type, VkShaderStageFlags stageFlags,
      //   uint32_t binding);
      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkDescriptorSetLayoutCreateInfo descriptorset_layout_create_info(VkDescriptorSetLayoutBinding* bindings,
      //   uint32_t bindingCount);
      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkWriteDescriptorSet write_descriptor_image(VkDescriptorType type, VkDescriptorSet dstSet,
      //   VkDescriptorImageInfo* imageInfo, uint32_t binding);
      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkWriteDescriptorSet write_descriptor_buffer(VkDescriptorType type, VkDescriptorSet dstSet,
      //   VkDescriptorBufferInfo* bufferInfo, uint32_t binding);
      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkDescriptorBufferInfo buffer_info(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range);

      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkImageCreateInfo image_create_info(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent);
      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkImageViewCreateInfo imageview_create_info(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags);
      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkPipelineLayoutCreateInfo pipeline_layout_create_info();
      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_info(VkShaderStageFlagBits stage,
      //   VkShaderModule shaderModule,
      //   const char* entry = "main");


      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkImageCreateInfo image_create_info();

      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkSamplerCreateInfo sampler_create_info();
      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkMemoryAllocateInfo memory_allocate_info();

      ///** @brief Initialize an image memory barrier with no image transfer ownership */
      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkImageMemoryBarrier image_memory_barrier();

      //CLASS_DECL_GRAPHICS3D_DIRECTX12 VkSubmitInfo submit_info();


   } // namespace initializers


} // namespace graphics3d_directx12




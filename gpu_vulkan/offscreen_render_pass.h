// From vk_swapchain by camilo on 2025-05-09 <3ThomasBorregaardSorensen!!
#pragma once


#include "render_pass.h"



namespace gpu_vulkan
{


	class CLASS_DECL_GPU_VULKAN offscreen_render_pass :
		virtual public render_pass
	{
	public:


		VkSampler m_vksampler;
		

		offscreen_render_pass();
		~offscreen_render_pass();

		void initialize_render_target(::gpu::renderer* pgpurenderer, const ::int_size &  windowExtent, ::pointer <::gpu::render_target>previous = {}) override;
		//VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
		//VkRenderPass getRenderPass() { return m_vkrenderpass; }
		//VkImageView getImageView(int index) { return m_imageview[index]; }
		//size_t imageCount() { return m_images.size(); }
		//VkFormat getImageFormat() { return imageFormat; }
		//VkExtent2D getExtent() { return m_extent; }
		//uint32_t width() { return m_extent.width; }
		//uint32_t height() { return m_extent.height; }

		//float extentAspectRatio() {
		//	return static_cast<float>(m_extent.width) / static_cast<float>(m_extent.height);
		//}
		VkFormat findDepthFormat();

		VkResult acquireNextImage() override;
		VkResult submitCommandBuffers(command_buffer * pcommandbuffer, 
			const ::array < VkSemaphore > & semaphoreaWait,
			const ::array < VkPipelineStageFlags > & stageaWait,
			const ::array < VkSemaphore >& semaphoreaSignal) override;

		//bool compareSwapFormats(const offscreen_render_pass& swapChain) const {
		//	return swapChain.depthFormat == depthFormat &&
		//		swapChain.imageFormat == imageFormat;
		//}

	//public:
		void on_init() override;
		void createRenderPassImpl();
		void createImageViews();
		void createDepthResources();
		void createRenderPass();
		void createFramebuffers();
		void createSyncObjects();

		//VkResult submitSamplingWork(const VkCommandBuffer buffer, uint32_t* imageIndex);
		VkResult submitSamplingWork(const VkCommandBuffer buffer);


		virtual void defer_resize(const ::int_size& size);

		::gpu::texture* current_texture();
		

	};


}  // namespace gpu_vulkan




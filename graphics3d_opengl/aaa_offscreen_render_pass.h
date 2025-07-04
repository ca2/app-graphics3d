// From vk_swapchain by camilo on 2025-05-09 <3ThomasBorregaardSorensen!!
#pragma once


#include "render_pass.h"


//// opengl headers
//#include <opengl/opengl.h>
//
//// std lib headers
//#include <string>
//#include <vector>
//#include <memory>


namespace graphics3d_opengl 
{

	class CLASS_DECL_GRAPHICS3D_OPENGL offscreen_render_pass :
		virtual public render_pass
	{
	public:


		static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

		offscreen_render_pass(context* pvkcdeviceRef, VkExtent2D windowExtent);
		offscreen_render_pass(context* pvkcdeviceRef, VkExtent2D windowExtent, ::pointer <render_pass>previous);
		~offscreen_render_pass();

		offscreen_render_pass(const offscreen_render_pass&) = delete;
		offscreen_render_pass& operator=(const offscreen_render_pass&) = delete;

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

		VkResult acquireNextImage(uint32_t* imageIndex) override;
		VkResult submitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);

		//bool compareSwapFormats(const offscreen_render_pass& swapChain) const {
		//	return swapChain.depthFormat == depthFormat &&
		//		swapChain.imageFormat == imageFormat;
		//}

	public:
		void init();
		void createRenderPassImpl();
		void createImageViews();
		void createDepthResources();
		void createRenderPass();
		void createFramebuffers();
		void createSyncObjects();

		//// Helper functions
		//VkSurfaceFormatKHR chooseSwapSurfaceFormat(
		//	const std::vector<VkSurfaceFormatKHR>& availableFormats);
		//VkPresentModeKHR chooseSwapPresentMode(
		//	const std::vector<VkPresentModeKHR>& availablePresentModes);
		//VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		//VkFormat imageFormat;
		//VkFormat depthFormat;
		//VkExtent2D m_extent;

		//std::vector<VkFramebuffer> swapChainFramebuffers;
		//VkRenderPass m_vkrenderpass;

		//std::vector<VkImage> depthImages;
		//std::vector<VkDeviceMemory> depthImageMemorys;
		//std::vector<VkImageView> depthImageViews;
		VkSampler m_vksampler;
		 std::vector<VkDeviceMemory> m_imagememories;
		//std::vector<VkImage> swapChainImages;
		//std::vector<VkImageView> swapChainImageViews;

		//context* m_pcontext;
		//VkExtent2D windowExtent;

		////VkSwapchainKHR swapChain;
		//::pointer<offscreen_render_pass> oldOffScreen;

		//std::vector<VkSemaphore> imageAvailableSemaphores;
		//std::vector<VkSemaphore> renderFinishedSemaphores;
		//std::vector<VkFence> inFlightFences;
		//std::vector<VkFence> imagesInFlight;
		//size_t currentFrame = 0;
	};

}  // namespace graphics3d_opengl



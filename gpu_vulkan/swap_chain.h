#pragma once


#include "bred/gpu/swap_chain.h"
#include "render_pass.h"


namespace gpu_vulkan
{


   class swap_chain :
      virtual public render_pass,
      virtual public ::gpu::swap_chain
   {
   public:


      VkSwapchainKHR                m_vkswapchain;
      uint32_t                      m_uCurrentSwapChainImage;
      ::pointer < ::gpu::context >  m_pgpucontextSwapChain;


      //swap_chain(renderer* pgpurenderer, VkExtent2D windowExtent);
      swap_chain();
      ~swap_chain();


      //int get_frame_index();


      //virtual ::gpu::texture* current_texture();
      void initialize_render_target(::gpu::renderer* pgpurenderer, const ::int_size & size, ::pointer <::gpu::render_target>previous = {}) override;

      //swap_chain_render_pass(const swap_chain_render_pass&) = delete;
      //swap_chain_render_pass& operator=(const swap_chain_render_pass&) = delete;

      //VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
      //VkRenderPass getRenderPass() { return m_vkrenderpass; }
      //VkImageView getImageView(int index) { return swapChainImageViews[index]; }
      //size_t imageCount() { return swapChainImages.size(); }
      //VkFormat getSwapChainImageFormat() { return swapChainImageFormat; }
      //VkExtent2D getSwapChainExtent() { return m_extent; }
      //uint32_t width() { return m_extent.width; }
      //uint32_t height() { return m_extent.height; }

      //float extentAspectRatio() {
      //   return static_cast<float>(m_extent.width) / static_cast<float>(m_extent.height);
      //}
      VkFormat findDepthFormat();

      VkResult acquireNextImage() override;
      VkResult submitCommandBuffers(command_buffer * pcommandbuffer) override;
      //int get_image_index() const override;
      //bool compareSwapFormats(const swap_chain_render_pass& m_swapchain) const {
      //   return m_swapchain.swapChainDepthFormat == swapChainDepthFormat &&
      //      m_swapchain.swapChainImageFormat == swapChainImageFormat;
      //}

   //public:
      void on_init() override;
      void createRenderPassImpl();
      void createImageViews();
      void createDepthResources();
      void createRenderPass();
      void createFramebuffers();
      void createSyncObjects();

      // Helper functions
      VkSurfaceFormatKHR chooseSwapSurfaceFormat(
         const ::array<VkSurfaceFormatKHR>& availableFormats);
      VkPresentModeKHR chooseSwapPresentMode(
         const ::array<VkPresentModeKHR>& availablePresentModes);
      VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

      /*VkFormat swapChainImageFormat;
      VkFormat swapChainDepthFormat;
      VkExtent2D m_extent;

      ::array<VkFramebuffer> swapChainFramebuffers;
      VkRenderPass m_vkrenderpass;

      ::array<VkImage> depthImages;
      ::array<VkDeviceMemory> depthImageMemorys;
      ::array<VkImageView> depthImageViews;
      ::array<VkImage> swapChainImages;
      ::array<VkImageView> swapChainImageViews;

      ::pointer < context > m_pgpucontext;
      VkExtent2D windowExtent;*/

      //::pointer<swap_chain_render_pass> oldSwapChain;

      //::array<VkSemaphore> imageAvailableSemaphores;
      //::array<VkSemaphore> renderFinishedSemaphores;
      //::array<VkFence> inFlightFences;
      //::array<VkFence> imagesInFlight;
      //size_t currentFrame = 0;

      void present(::gpu::texture * pgputexture) override;

      //void on_end_render(::gpu::frame* pgpuframe) override;


   };


}  // namespace gpu_vulkan



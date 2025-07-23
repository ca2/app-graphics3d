#pragma once


#include "bred/gpu/swap_chain.h"
#include "render_pass.h"


namespace gpu_vulkan
{


   class swap_chain :
      virtual public ::gpu_vulkan::render_pass,
      virtual public ::gpu::swap_chain
   {
   public:


      VkSwapchainKHR                m_vkswapchain;
      uint32_t                      m_uCurrentSwapChainImage;
      ::pointer < ::gpu::context >  m_pgpucontextSwapChain;
      ::pointer < ::gpu_vulkan::shader > m_pshaderPresent;

      //swap_chain(renderer* pgpurenderer, VkExtent2D windowExtent);
      swap_chain();
      ~swap_chain();


      //int get_frame_index();
      bool should_use_advanced_pipeline_synchronization() override;

      //virtual ::gpu::texture* current_texture();
      void update_render_pass(::gpu::context* pgpucontext, ::pointer <::gpu_vulkan::render_pass>previous = {}) override;
      void initialize_gpu_swap_chain(::gpu::renderer* pgpurenderer) override;
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

      int get_frame_index() override;

      VkResult acquireNextImage() override;
      VkResult submitCommandBuffers(
         command_buffer * pcommandbuffer,
         ::gpu::texture * pgputexture,
         const ::array < VkSemaphore >& semaphoreaWait,
         const ::array < VkPipelineStageFlags >& stageaWait,
         const ::array < VkSemaphore >& semaphoreaSignal) override;
      virtual int get_image_index() const;
      //bool compareSwapFormats(const swap_chain_render_pass& m_swapchain) const {
      //   return m_swapchain.swapChainDepthFormat == swapChainDepthFormat &&
      //      m_swapchain.swapChainImageFormat == swapChainImageFormat;
      //}

      //::gpu::texture* current_texture() override;


   //public:
      void on_init_render_pass() override;
      void create_images();
      //void createImageViews();
      //void createDepthResources();
      void createRenderPass();
      //void createFramebuffers();
      //void createSyncObjects();

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



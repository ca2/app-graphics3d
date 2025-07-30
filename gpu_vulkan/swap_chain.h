#pragma once


#include "bred/gpu/swap_chain.h"
#include "render_pass.h"


namespace gpu_vulkan
{


   class swap_chain :
      virtual public ::gpu::swap_chain
   {
   public:


      VkSwapchainKHR                m_vkswapchain;
      uint32_t                      m_uCurrentSwapChainImage;
      ::pointer < ::gpu::context >  m_pgpucontextSwapChain;
      ::pointer < ::gpu::shader > m_pshaderPresent;
      ::pointer < ::gpu_vulkan::render_pass > m_prenderpass;


      //bool m_bNeedRebuild;

      //swap_chain(renderer* pgpurenderer, VkExtent2D windowExtent);
      swap_chain();
      ~swap_chain();

      void on_new_swap_chain() override;

      //int get_frame_index();
      virtual bool should_use_advanced_pipeline_synchronization();

      //virtual ::gpu::texture* current_texture();
      //virtual void update_render_pass(::gpu::context* pgpucontext, ::pointer <::gpu_vulkan::render_pass>previous = {});
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

      //int get_frame_index() override;

      virtual VkResult acquireNextImage();
      virtual VkResult submitCommandBuffers2(
         command_buffer * pcommandbuffer,
         ::gpu::texture * pgputexture,
         const ::array < VkSemaphore >& semaphoreaWait,
         const ::array < VkPipelineStageFlags >& stageaWait,
         const ::array < VkSemaphore >& semaphoreaSignal);
      virtual int get_image_index() const;

      void create_images();

      VkSurfaceFormatKHR chooseSwapSurfaceFormat(
         const ::array<VkSurfaceFormatKHR>& availableFormats);
      VkPresentModeKHR chooseSwapPresentMode(
         const ::array<VkPresentModeKHR>& availablePresentModes);
      VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);


      void present(::gpu::texture * pgputexture) override;


   };


}  // namespace gpu_vulkan



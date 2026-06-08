// From vk_swapchain by camilo on 2025-05-09 01:40 <3ThomasBorregaardSorensen!!
#pragma once


#include "bred/gpu/render_target.h"
#include "context.h"
//#include "gpu_vulkan/render_pass.h"

// vulkan headers
#include "_gpu_vulkan.h"



namespace gpu_vulkan
{


   class CLASS_DECL_GPU_VULKAN render_target :
      virtual public ::gpu::render_target
      
   {
   public:



      //VkFormat m_formatImage;
      //VkFormat m_formatDepth;
      //VkExtent2D m_extent;

      //::array<VkFramebuffer> m_framebuffers;
      //VkRenderPass m_vkrenderpass;

      //::pointer < ::gpu_vulkan::render_pass > m_prenderpassDepth;
      //::pointer < ::gpu_vulkan::render_pass > m_prenderpassNoDepth;
      //::pointer<::gpu_vulkan::render_pass> m_prenderpassSrgb;




      /////::pointer < renderer >  m_pgpurenderer;

      //::array < VkSemaphore> m_semaphoreaSignalOnSubmit;
      //::array < VkSemaphore> m_semaphoreaWaitToSubmit;
      //::array < VkPipelineStageFlags > m_stageaWaitToSubmit;
      ////::array<VkImage> depthImages;
      ////::array<VkDeviceMemory> depthImageMemorys;
      ////::array<VkImageView> depthImageViews;
      ////::array<VkImage> m_images;
      ////::array<VkImageView> m_imageviews;

      ////context* m_pgpucontext;
      ////VkExtent2D windowExtent;

      ////VkSwapchainKHR swapChain;
      ////::pointer<render_pass> m_pvkcrenderpassOld;

      //::i32_array imageAvailable;
      //::array<VkSemaphore> imageAvailableSemaphores;
      //::array<VkSemaphore> renderFinishedSemaphores;
      //::array<VkFence>     inFlightFences;
      //::array<VkFence>     imagesInFlight;
      //bool                 m_bNeedRebuild;

      //// static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

      ////render_pass(renderer * prenderer, VkExtent2D windowExtent);
      ////render_pass(renderer * prenderer, VkExtent2D windowExtent, ::pointer <render_pass>previous);
      render_target();
      ~render_target();


      void initialize_render_target(::gpu::renderer* pgpurenderer, const ::i32_size & size, ::pointer <::gpu::render_target>previous = {}) override;


      //virtual ::gpu_vulkan::render_pass *render_pass_srgb(::gpu::texture * pgputextureTarget);
      //virtual ::gpu_vulkan::render_pass *render_pass_with_depth(::gpu::texture *pgputextureTarget);
      //virtual ::gpu_vulkan::render_pass *render_pass_no_depth(::gpu::texture *pgputextureTarget);
      // virtual ::gpu_vulkan::render_pass *render_pass_srgb(::gpu::texture * pgputextureTarget);
      // virtual ::gpu_vulkan::render_pass *render_pass_with_depth(::gpu::texture *pgputextureTarget);
      //virtual ::gpu_vulkan::render_pass *get_render_pass(::gpu::texture *pgputextureTarget);

      //virtual ::gpu_vulkan::render_pass *render_pass2(::gpu::texture *pgputextureTarget);

      //virtual ::pointer <::gpu_vulkan::render_pass > create_render_pass(bool bWithDepth, bool bSrgb = false);
      //virtual ::pointer<::gpu_vulkan::render_pass> create_render_pass(::gpu::texture * pgputextureTarget);



//      virtual void on_before_begin_render(frame* pframe);
//
//      VkFramebuffer getFrameBuffer(int index) { return m_framebuffers[index]; }
//      VkFramebuffer getCurrentFrameBuffer() { return m_framebuffers[get_frame_index()]; }
//      //virtual int get_image_index() const;
//      VkRenderPass getRenderPass() { return m_vkrenderpass; }
//      VkImageView getImageView(int index);
//      VkImageView getDepthImageView(int index);
//      //int imageCount() { return m_texturea.size(); }
////      VkFormat getImageFormat() { return m_pgpurenderer->m_pgpucontext; }
//      VkExtent2D getExtent() { return m_extent; }
//      int width() { return m_extent.width; }
//      int height() { return m_extent.height; }
//
//      float extentAspectRatio() {
//         return static_cast<float>(m_extent.width) / static_cast<float>(m_extent.height);
//      }
//      virtual VkFormat findDepthFormat();
//
//      //virtual VkResult acquireNextImage(uint32_t* imageIndex);
//      //virtual VkResult submitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);
//      virtual VkResult acquireNextImage();
//      virtual VkResult submitCommandBuffers(
//         command_buffer * pcommandbuffer, 
//         const ::array < VkSemaphore >& semaphoreaWait,
//         const ::array < VkPipelineStageFlags >& stageaWait,
//         const ::array < VkSemaphore >& semaphoreaSignal);
//
//      //bool compareFormats(const render_pass& renderpass) const {
//      //   return renderpass.m_formatDepth == m_formatDepth &&
//      //      renderpass.m_formatImage == m_formatImage;
//      //}
//

      void on_init() override;


      void on_create_render_target_texture(::gpu::texture_attributes &textureattributes,
                                           ::gpu::texture_flags &textureflags) override;

      //virtual void createRenderPassImpl();
      //virtual void createImageViews();
      //virtual void createDepthResources();
      //virtual void createRenderPass();
      //virtual void createFramebuffers();
      //virtual void createSyncObjects();


      ////virtual void defer_layout();
      ////// Helper functions
      ////VkSurfaceFormatKHR chooseSwapSurfaceFormat(
      ////   const ::array<VkSurfaceFormatKHR>& availableFormats);
      ////VkPresentModeKHR chooseSwapPresentMode(
      ////   const ::array<VkPresentModeKHR>& availablePresentModes);
      ////VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

   };


}  // namespace gpu_vulkan




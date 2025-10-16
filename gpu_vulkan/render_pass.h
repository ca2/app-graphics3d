// From render_pass now render target to just gpu_vulkan/render_pass by
// camilo on 2025-07-22 13:49 <3ThomasBorregaardSørensen!!
// From vk_swapchain by camilo on 2025-05-09 01:40 <3ThomasBorregaardSorensen!!
#pragma once


#include "bred/gpu/context_object.h"
#include "context.h"

// vulkan headers
#include "_gpu_vulkan.h"



namespace gpu_vulkan
{




   class CLASS_DECL_GPU_VULKAN render_pass :
      virtual public ::gpu::context_object
   {
   public:

      bool m_bWithDepth = false;
      bool m_bSrgb = false;
      bool m_bLoadClearOp = false;



      ///::pointer < ::pointer_array < ::gpu::texture > > m_ptexturea;
      VkFormat m_formatImage;
      VkFormat m_formatDepth;
      VkExtent2D m_extent;
      ::pointer < render_pass > m_prenderpassOld;

      //map < ::gpu::texture*, texture_t > m_mapTexture;
      
      VkRenderPass m_vkrenderpass;

      ///::pointer < renderer >  m_pgpurenderer;

      //::array < VkSemaphore> m_semaphoreaSignalOnSubmit;
      //::array < VkSemaphore> m_semaphoreaWaitToSubmit;
      //::array < VkPipelineStageFlags > m_stageaWaitToSubmit;



      //::array<VkImage> depthImages;
      //::array<VkDeviceMemory> depthImageMemorys;
      //::array<VkImageView> depthImageViews;
      //::array<VkImage> m_images;
      //::array<VkImageView> m_imageviews;

      //context* m_pgpucontext;
      //VkExtent2D windowExtent;

      //VkSwapchainKHR swapChain;
      //::pointer<render_pass> m_pvkcrenderpassOld;

      bool                 m_bNeedRebuild;
      //bool                 m_bLoadClearOp = false;

      // static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

      //render_pass(renderer * prenderer, VkExtent2D windowExtent);
      //render_pass(renderer * prenderer, VkExtent2D windowExtent, ::pointer <render_pass>previous);
      render_pass();
      ~render_pass();

      
      virtual bool should_use_advanced_pipeline_synchronization();


      virtual void update_render_pass(::gpu::context* pgpucontext, ::pointer <::gpu_vulkan::render_pass>previous = {});
      virtual void _update_render_pass(::gpu::context *pgpucontext, ::pointer<::gpu_vulkan::render_pass> previous = {});

      //virtual void on_before_begin_render(frame* pframe, ::gpu::texture * pgputexture);


      virtual int get_frame_index();
      //virtual ::gpu::texture* current_texture();
      //VkFramebuffer getFrameBuffer(int index) { return m_framebuffers[index]; }
      //VkFramebuffer getCurrentFrameBuffer() { return m_framebuffers[get_frame_index()]; }
      //virtual int get_image_index() const;
      VkRenderPass getRenderPass() { return m_vkrenderpass; }
      //VkImageView getImageView(int index);
      //VkImageView getDepthImageView(int index);
      //int imageCount() { return m_texturea.size(); }
//      VkFormat getImageFormat() { return m_pgpurenderer->m_pgpucontext; }
      VkExtent2D getExtent() { return m_extent; }
      int width() { return m_extent.width; }
      int height() { return m_extent.height; }

      float extentAspectRatio() {
         return static_cast<float>(m_extent.width) / static_cast<float>(m_extent.height);
      }
      virtual VkFormat findDepthFormat();

      //virtual VkResult acquireNextImage(uint32_t* imageIndex);
      //virtual VkResult submitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);
      virtual VkResult acquireNextImage();
      ///virtual VkResult submitCommandBuffers(
         /*command_buffer* pcommandbuffer,
         ::gpu::texture * pgputexture,
         const ::array < VkSemaphore >& semaphoreaWait,
         const ::array < VkPipelineStageFlags >& stageaWait,
         const ::array < VkSemaphore >& semaphoreaSignal);*/

      //bool compareFormats(const render_pass& renderpass) const {
      //   return renderpass.m_formatDepth == m_formatDepth &&
      //      renderpass.m_formatImage == m_formatImage;
      //}


      virtual void on_init_render_pass();
      //virtual void createRenderPassImpl();
      //virtual void createImageViews();
      //virtual void createDepthResources();
      virtual void createRenderPass();
      //virtual void createFramebuffers();
      //virtual void createSyncObjects();


      //virtual void defer_layout();
      //// Helper functions
      //VkSurfaceFormatKHR chooseSwapSurfaceFormat(
      //   const ::array<VkSurfaceFormatKHR>& availableFormats);
      //VkPresentModeKHR chooseSwapPresentMode(
      //   const ::array<VkPresentModeKHR>& availablePresentModes);
      //VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

   };


}  // namespace gpu_vulkan




// From vk_swapchain by camilo on 2025-05-09 01:40 <3ThomasBorregaardSorensen!!
#pragma once


#include "bred/gpu/render_target.h"
#include "context.h"

//// directx12 headers
//#include "_gpu_directx12.h"



namespace gpu_directx12
{

   
   class CLASS_DECL_GPU_DIRECTX12 render_target_view :
      virtual public ::gpu::render_target
   {
   public:

      //::comptr<ID3D12DescriptorHeap> m_rtvHeap;
      //::comptr<ID3D12DescriptorHeap> m_dsvHeap;
      //::pointer < depth_stencil >      m_pdepthstencil;
      UINT m_rtvDescriptorSize;

      //::int_size m_size;

      //::pointer < renderer >  m_pgpurenderer;

      ::pointer<render_target_view> m_prendertargetviewOld;

      ::int_array imageAvailable;
      bool                 m_bNeedRebuild;

      //static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

      render_target_view();
      //render_target_view(renderer* prenderer, const ::int_size & size);
      //render_target_view(renderer* prenderer, const ::int_size& size, ::pointer <render_target_view>previous);
      ~render_target_view();


      virtual void initialize_render_target(::gpu::renderer* prenderer, const ::int_size& size, ::pointer <::gpu::render_target>previous);



      virtual void on_before_begin_render(frame* pframe);

      //VkFramebuffer getFrameBuffer(int index) { return m_framebuffers[index]; }
      //VkFramebuffer getCurrentFrameBuffer() { return m_framebuffers[get_image_index()]; }
      //virtual int get_image_index() const;
      //VkRenderPass getRenderPass() { return m_vkrendertargetview; }
      //VkImageView getImageView(int index) { return m_imageviews[index]; }
      //size_t imageCount() { return m_images.size(); }
      //size_t imageCount() { return 0; }
      //VkFormat getImageFormat() { return m_formatImage; }
      //VkExtent2D getExtent() { return m_extent; }
      //uint32_t width() { return 16; }
      //uint32_t height() { return 9; }

      //virtual ::gpu::texture* current_texture();
      //virtual depth_stencil* current_depth_stencil();


      float extentAspectRatio() {
         return (float) width() / (float) height();
         //return static_cast<float>(m_extent.width) / static_cast<float>(m_extent.height);
      }
      //virtual VkFormat findDepthFormat();

      //virtual HRESULT acquireNextImage(uint32_t* imageIndex);
      //virtual HRESULT submitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);
      virtual HRESULT acquireNextImage();
      // virtual HRESULT submitCommandBuffers(const VkCommandBuffer* buffers);

      //bool compareFormats(const render_pass& rendertargetview) const {
      //   return rendertargetview.m_formatDepth == m_formatDepth &&
      //      rendertargetview.m_formatImage == m_formatImage;
      //}


      void on_init() override;
      virtual void createRenderPassImpl();
      virtual void createImageViews();
      virtual void createDepthResources();
      virtual void createRenderPass();
      virtual void createFramebuffers();
      virtual void createSyncObjects();


      //virtual void defer_layout();
      //// Helper functions
      //VkSurfaceFormatKHR chooseSwapSurfaceFormat(
      //   const ::array<VkSurfaceFormatKHR>& availableFormats);
      //VkPresentModeKHR chooseSwapPresentMode(
      //   const ::array<VkPresentModeKHR>& availablePresentModes);
      //VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

   };


}  // namespace gpu_directx12




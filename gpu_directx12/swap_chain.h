#pragma once


#include "bred/gpu/swap_chain.h"
#include "gpu_directx12/offscreen_render_target_view.h"


namespace gpu_directx12
{


   class swap_chain :
      virtual public ::gpu::swap_chain,
      virtual public ::gpu_directx12::offscreen_render_target_view
   {
   public:


      comptr<IDXGISwapChain3>                         m_pdxgiswapchain;
      int m_iSwapChainCount = -1;
      //::comptr<IDXGISwapChain> m_pdxgiswapchain;
      ::pointer < ::gpu_directx12::shader > m_pshaderPresent;
      ::comptr<ID3D12Resource> m_presourceSharedTexture;

      ::comptr < ID3D12RootSignature > m_prootsignature;

      HANDLE m_handleSharedTexture;
      ::pointer_array < texture > m_textureaSwapChain;
      //VkSwapchainKHR       m_vkswapchain;
      uint32_t             m_uCurrentSwapChainImage;
      //::int_size m_size;


      comptr<IDCompositionDevice> m_pdcompositiondevice;
      comptr<IDCompositionTarget> m_pdcompositiontarget;
      comptr<IDCompositionVisual> m_pdcompositionvisual;


      //::comptr<ID3D11VertexShader> m_pvertexshaderFullscreen;
      //::comptr<ID3D11PixelShader> m_ppixelshaderFullscreen;
      //::comptr < ID3D12PipelineState > m_ppipelinestate;
      //::comptr<ID3D11SamplerState> m_psamplerstateLinear;
      //::comptr<ID3D11ShaderResourceView> m_pshaderresourceviewShader;


      static constexpr int MAX_FRAMES_IN_FLIGHT = 2;



      swap_chain();
      ~swap_chain() override;



      void initialize_render_target(::gpu::renderer* prenderer, const ::int_size& size, ::pointer <::gpu::render_target>previous) override;


      void on_init() override;

      void initialize_swap_chain_window(::gpu::context* pcontext, ::windowing::window* pwindow);

      //void endDraw(::draw2d_gpu::graphics* pgraphics, ::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc) override;
      void present(::gpu::texture * pgputexture) override;
      void set_present_state() override;
      void swap_buffers() override;
      void get_new_swap_chain_index() override;
//      VkInstance instance{ VK_NULL_HANDLE };
//      VkDevice device{ VK_NULL_HANDLE };
//      VkPhysicalDevice physicalDevice{ VK_NULL_HANDLE };
//      VkSurfaceKHR surface{ VK_NULL_HANDLE };
//
//      //public:
//
//      VkFormat colorFormat{};
//      VkColorSpaceKHR colorSpace{};
//      VkSwapchainKHR swapChain{ VK_NULL_HANDLE };
//      ::array<VkImage> images{};
//      ::array<VkImageView> imageViews{};
//      uint32_t queueNodeIndex{ UINT32_MAX };
//
//#if defined(VK_USE_PLATFORM_WIN32_KHR)
//      void initSurface(void * platformHandle, void * platformWindow);
//#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
//      void initSurface(ANativeWindow * window);
//#elif defined(VK_USE_PLATFORM_DIRECTFB_EXT)
//      void initSurface(IDirectFB * dfb, IDirectFBSurface * window);
//#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
//      void initSurface(wl_display * display, wl_surface * window);
//#elif defined(VK_USE_PLATFORM_XCB_KHR)
//      void initSurface(xcb_connection_t * connection, xcb_window_t window);
//#elif (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK))
//      void initSurface(void * view);
//#elif defined(VK_USE_PLATFORM_METAL_EXT)
//      void initSurface(CAMetalLayer * metalLayer);
//#elif (defined(_DIRECT2DISPLAY) || defined(VK_USE_PLATFORM_HEADLESS_EXT))
//      void initSurface(uint32_t width, uint32_t height);
//#if defined(_DIRECT2DISPLAY)
//      void createDirect2DisplaySurface(uint32_t width, uint32_t height);
//#endif
//#elif defined(VK_USE_PLATFORM_SCREEN_QNX)
//      void initSurface(screen_context_t screen_context, screen_window_t screen_window);
//#endif
//      /* Set the DirectX12 objects required for swapchain creation and management, must be called before swapchain creation */
//      void setContext(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device);
//      /**
//      * Create the swapchain and get its images with given width and height
//      *
//      * @param width Pointer to the width of the swapchain (may be adjusted to fit the requirements of the swapchain)
//      * @param height Pointer to the height of the swapchain (may be adjusted to fit the requirements of the swapchain)
//      * @param vsync (Optional, default = false) Can be used to force vsync-ed rendering (by using VK_PRESENT_MODE_FIFO_KHR as presentation mode)
//      */
//      void create(uint32_t & width, uint32_t & height, bool vsync = false, bool fullscreen = false);
//      /**
//      * Acquires the next image in the swap chain
//      *
//      * @param presentCompleteSemaphore (Optional) Semaphore that is signaled when the image is ready for use
//      * @param imageIndex Pointer to the image index that will be increased if the next image could be acquired
//      *
//      * @note The function will always wait until the next image has been acquired by setting timeout to UINT64_MAX
//      *
//      * @return HRESULT of the image acquisition
//      */
//      HRESULT acquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t & imageIndex);
//      /**
//      * Queue an image for presentation
//      *
//      * @param queue Presentation queue for presenting the image
//      * @param imageIndex Index of the swapchain image to queue for presentation
//      * @param waitSemaphore (Optional) Semaphore that is waited on before the image is presented (only used if != VK_NULL_HANDLE)
//      *
//      * @return HRESULT of the queue presentation
//      */
//      HRESULT queuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore = VK_NULL_HANDLE);
//      /* Free all DirectX12 resources acquired by the swapchain */
//      void cleanup();


   };


} // namespace gpu_directx12




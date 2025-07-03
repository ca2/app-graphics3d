// From gpu_directx12::context by
// camilo on 2025-05-27 04:54 <3ThomasBorregaardSorensen!!
#pragma once


#include "bred/gpu/device.h"
#include "acme/prototype/prototype/memory.h"
#include "acme_windows_common/dxgi_device_source.h"
//#include "directx12/directx12.h"
#include <dcomp.h>
#include <d3d11.h>
#include <d3d11on12.h>

namespace gpu_directx12
{


   class CLASS_DECL_GPU_DIRECTX12 device :
      virtual public ::gpu::device
   {
   public:


#if defined(_DEBUG)

      ::comptr<ID3D12Debug> m_pdebug;

#endif

      comptr<ID3D12Device>                            m_pdevice;
      //comptr<ID3D11Device1>                           m_pdevice1;
      //comptr<IDXGIDevice>                             m_pdxgidevice;
      comptr<IDXGIFactory4>                           m_pdxgifactory4;
      //comptr<ID3D11DeviceContext>                     m_pdevicecontext;
      ::pointer<::windowing::window>                  m_pimpl;
      bool                                            m_bUseWarpDevice = false;
      ::array <::comptr<ID3D12Resource>>              m_resourceaBackBufferTexture;
      ::array < D3D12_CPU_DESCRIPTOR_HANDLE  >        m_handleaBackBufferRenderTargetView;
      //::comptr<ID3D12RenderargetView>                m_prendertargetviewBackBuffer;
      //::pointer < context >                           m_pcontextMainDraw2d;
      D3D_FEATURE_LEVEL                               m_featurelevel;
      //::pointer < context >                           m_pcontextMain;
      //::comptr<ID3D11Device> m_pd3d11device;
      //::comptr<ID3D11DeviceContext>  m_pd3d11context;
      //::comptr<ID3D11On12Device>  m_pd3d11on12;
      //::comptr<IDXGIDevice>  m_pdxgidevice;


      //comptr<IDCompositionDevice> m_pdcompositiondevice;
      //comptr<IDCompositionTarget> m_pdcompositiontarget;
      //comptr<IDCompositionVisual> m_pdcompositionvisual;
      //directx12();
      //~directx12() override;


      //itask									m_itaskGpu;
      //VkSampler                           m_vksampler001;

      //unsigned int                     m_VAO;
      //unsigned int                     m_VBO;

      //int                            m_iMatrixUniform;

      //int                           m_gluTextureBitmap1;
      //int                              m_iLastBitmap1Scan;
      //::int_size                       m_sizeBitmap1;

      //memory                           m_memorySwap;

      //bool                                m_bOffscreen;
      bool                             m_bMesa;
      //HGLRC                            m_hrc;
      //HDC                              m_hdc;
      //HWND                             m_hwnd;
      //HDC                              m_hdcGraphics;

         /** @brief Physical device representation */
      //VkPhysicalDevice m_physicaldevice;
      ::pointer < physical_device >       m_pphysicaldevice;
      /** @brief Logical device representation (application's view of the device) */
      //VkDevice m_vkdevice;
      ///** @brief Properties of the physical device including limits that the application can check against */
      //VkPhysicalDeviceProperties m_physicaldeviceproperties;
      ///** @brief Features of the physical device that an application can use to check if a feature is supported */
      //VkPhysicalDeviceFeatures m_physicaldevicefeatures;
      /** @brief Features that have been enabled for use on the physical device */
      //VkPhysicalDeviceFeatures m_physicaldevicefeaturesCreate;
      /** @brief Features that have been enabled for use on the physical device */
      //VkPhysicalDeviceFeatures m_physicaldevicefeaturesEnabled;
      ///** @brief Memory types and heaps of the physical device */
      //VkPhysicalDeviceMemoryProperties m_physicaldevicememoryproperties;
      ///** @brief Queue family properties of the physical device */
      //::array<VkQueueFamilyProperties> m_queuefamilypropertya;
      /** @brief List of extensions supported by the device */
      string_array m_straSupportedExtensions;
      ///** @brief Default command pool for the graphics queue family index */
      //VkCommandPool m_vkcommandpool;

      /** @brief Contains queue family indices */
      //directx12::QueueFamilyIndices m_queuefamilyindices;
      //struct
      //{
      //   uint32_t graphics;
      //   uint32_t compute;
      //   uint32_t transfer;
      //} m_queuefamilyindices;





      //graphics3d_directx12::context
#if defined(NDEBUG)
      const bool enableValidationLayers = false;
#else
      const bool enableValidationLayers = true;
#endif

      //VkInstance m_vkinstance;
      //VkDebugUtilsMessengerEXT debugMessenger;
      //VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
      //::pointer < ::user::graphics3d > m_pimpact;

      //VkCommandPool m_vkcommandpool;

      //VkDevice m_vkdevice;
      //VkSurfaceKHR m_vksurfacekhr;
      //VkQueue m_vkqueueGraphics;
      //VkQueue m_vkqueuePresent;

      ::array<const char*>       validationLayers;
      ::array<const char*>       deviceExtensions;
      ::procedure_array          m_procedureaOnTopFrameEnd;
      //::pointer < direct2d_draw2d_swap_chain >    m_pswapchain;




      device();
      ~device() override;




      void list_dred_breadcrumbs();

      void defer_throw_hresult(HRESULT hresult);

      virtual void get_debug_interface(UINT& dxgiFactoryFlags);

      virtual void initialize_swap_chain(::windowing::window* pwindow);
      virtual void initialize_cpu_buffer(const ::int_size & size);
      

      void initialize_gpu_device_for_swap_chain(::gpu::approach* pgpuapproach, ::windowing::window *pwindow) override;
      void initialize_gpu_device_for_off_screen(::gpu::approach* pgpuapproach, const ::int_rectangle& rectanglePlacement) override;

      //virtual ::gpu::context* main_draw2d_context();

      void GetHardwareAdapter(
         IDXGIFactory1* pFactory,
         IDXGIAdapter1** ppAdapter,
         bool requestHighPerformanceAdapter = false);

      void _create_d3d12_device();
      //string _001GetIntroProjection() override;
      //string _001GetIntroFragment() override;

      //void draw() override;
      //void start_drawing() override;
      //void global_transform() override;
      //void render() override;

      //void set_bitmap_1(::image::image* pimage) override;

      //void swap_buffers() override;

      //VkSampler _001VkSampler();

      //set_descriptor_layout* get_set_descriptor_layout();
      //virtual VkDescriptorSet getCurrentDescriptorSet(::gpu_directx12::renderer* prenderer);

      //::gpu_directx12::descriptor_pool* get_global_pool(int iFrameCount);

      //virtual void create_global_ubo(int iSize, int iFrameCount);
      //virtual void update_global_ubo(const ::block& block);

      //void clear(const ::color::color& color);

      //virtual void create_offscreen_buffer(const ::int_size& size);
      //virtual void _create_offscreen_buffer(const ::int_size& size);

      //virtual void resize_offscreen_buffer(const ::int_size& size);
      //virtual void destroy_offscreen_buffer();

   /*   virtual void translate_shader(string & str);

      virtual void _translate_shader(string_array& stra);*/

      //virtual void make_current();

      // virtual string load_fragment(const ::string & pszPath, enum_shader_source& eshadersource);

      //virtual string get_shader_version_text();

      //void set_matrix_uniform(const ::gpu::payload& uniformMatrix) override;


      virtual void defer_shader_memory(::memory& memory, const ::file::path& pathShader);



      //virtual void create_device();

      //void on_create_context(const ::gpu::start_context_t& startcontext) override;


  /*    VkDevice logicalDevice() const
      {

         return m_vkdevice;

      }*/

      //virtual HRESULT createLogicalDevice(
      //   VkPhysicalDeviceFeatures enabledFeatures,
      //   ::array<const char*> enabledExtensions,
      //   void* pNextChain,
      //   bool useSwapChain = true,
      //   VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
      ////virtual uint32_t getQueueFamilyIndex(VkQueueFlags queueFlags) const;
      //virtual VkCommandPool createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
      virtual bool isExtensionSupported(const ::scoped_string& scopedstrExtension);

      virtual void _create_offscreen_window(const ::int_size& size);

      //void defer_create_window_context(::windowing::window* pwindow) override;
      //void _defer_create_window_context(::windowing::window* pwindow) override;
      //virtual void _create_window_context(::windowing::window* pwindow);

      //virtual void _create_window_buffer();
      //void _create_offscreen_buffer(const ::int_size& size) override;
      //void resize_offscreen_buffer(const ::int_size& size) override;
      //void destroy_offscreen_buffer() override;

      //void make_current() override;

      bool is_mesa() override;

      //void render();

      string get_shader_version_text() override;

      void _translate_shader(string_array& straFragment) override;

      //void swap_buffers() override;


      //graphics3d_directx12::context


      //VkCommandPool getCommandPool() { return m_vkcommandpool; }
      //VkDevice logicalDevice() { return m_vkdevice; }

      //VkQueue graphicsQueue() { return m_vkqueueGraphics; }
      //VkQueue presentQueue() { return m_vkqueuePresent; }


      //// Buffer Helper Functions
      //void createBuffer(
      //   VkDeviceSize size,
      //   VkBufferUsageFlags usage,
      //   VkMemoryPropertyFlags properties,
      //   VkBuffer& buffer,
      //   VkDeviceMemory& bufferMemory);
      //void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
      //void copyBufferToImage(
      //   VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);
      //void createImageWithInfo(
      //   const VkImageCreateInfo& imageInfo,
      //   VkMemoryPropertyFlags properties,
      //   VkImage& image,
      //   VkDeviceMemory& imageMemory);


//      void submitWork(VkCommandBuffer cmdBuffer, VkQueue queue);


      //public:
      //void _createInstance();
      //void _setupDebugMessenger();
      //void _createSurface();
      //void _pickPhysicalDevice();
      //void _createLogicalDevice();
      //void _createCommandPool();

      // helper functions
      //bool isDeviceSuitable(VkPhysicalDevice pvkcdevice);
      //::array<const char *> getRequiredExtensions();
      bool checkValidationLayerSupport();
      //void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
      //void hasGflwRequiredInstanceExtensions();
      //bool checkDeviceExtensionSupport(VkPhysicalDevice pvkcdevice);


      //      //set_descriptor_layout* get_set_descriptor_layout(::gpu::context * pgpucontext);
      //virtual VkDescriptorSet getGlobalDescriptorSet(::gpu::context* pgpucontext, ::gpu_directx12::renderer* prenderer);

      //::gpu_directx12::descriptor_pool* get_global_pool(::gpu::context* pgpucontext, int iFrameCount);

      
      void on_top_end_frame() override;


      //ID3D12Device* draw_get_d3d11_device();
      //ID3D12Device1* draw_get_d3d11_device1();

      int get_type_size(::gpu::enum_type etype) override;
      void set_mat4(void* p, const ::glm::mat4& mat4) override;
      ::gpu::payload load_dds(const ::scoped_string& scopedstrImagePath) override;
      ::file::path shader_path(const ::file::path& pathShader) override;


   };


} // namespace gpu




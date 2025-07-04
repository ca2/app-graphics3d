#pragma once


#include "bred/gpu/context.h"
#include "acme/prototype/prototype/memory.h"
#include "gpu_directx12/device.h"
#include <d3d11_3.h>
#include <d3d11_4.h>


namespace gpu_directx12
{


   class CLASS_DECL_GPU_DIRECTX12 context :
      virtual public ::gpu::context,
      virtual public ::dxgi_device_source
   {
   public:

      int m_iResourceWrappingCount = 0;

      ::comptr<ID3D12CommandQueue>                                m_pcommandqueue;


      // Create an empty root signature.
   /*{
      CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
      rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

      ComPtr<ID3DBlob> signature;
      ComPtr<ID3DBlob> error;
      ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
      ThrowIfFailed(m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
   }*/
      //comptr < ID3D11Buffer>              m_pbufferGlobalUbo;
      //comptr<ID3D12DeviceContext>         m_pcontext;
      //comptr<ID3D12DeviceContext1>        m_pcontext1;

      //comptr < ID3D11RasterizerState> m_prasterizerstate;
      //itask									m_itaskGpu;
      //VkSampler m_vksampler001;

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
      ::pointer < device >                m_pgpudevice;
      ::pointer < context >               m_pgpucontextMain;


      class snapshot :
         virtual public ::particle {
      public:

         ::int_rectangle m_rectangleTarget;
         ::comptr < ID3D12Resource > m_presource;

         snapshot(ID3D12Resource* presource, const ::int_rectangle& rectangleTarget) :
            m_presource(presource),
            m_rectangleTarget(rectangleTarget)
         {

         }
      };

      //::array<::comptr<ID3D12Resource>>   m_resourceaSnapshot;
      ::pointer_array < snapshot >     m_snapshota;

//      /** @brief Logical device representation (application's view of the device) */
//      VkDevice m_vkdevice;
//      ///** @brief Properties of the physical device including limits that the application can check against */
//      //VkPhysicalDeviceProperties m_physicaldeviceproperties;
//      ///** @brief Features of the physical device that an application can use to check if a feature is supported */
//      //VkPhysicalDeviceFeatures m_physicaldevicefeatures;
//      /** @brief Features that have been enabled for use on the physical device */
//      VkPhysicalDeviceFeatures m_physicaldevicefeaturesCreate;
//      /** @brief Features that have been enabled for use on the physical device */
//      VkPhysicalDeviceFeatures m_physicaldevicefeaturesEnabled;
//      ///** @brief Memory types and heaps of the physical device */
//      //VkPhysicalDeviceMemoryProperties m_physicaldevicememoryproperties;
//      ///** @brief Queue family properties of the physical device */
//      //::array<VkQueueFamilyProperties> m_queuefamilypropertya;
//      /** @brief List of extensions supported by the device */
//      string_array m_straSupportedExtensions;
//      ///** @brief Default command pool for the graphics queue family index */
//      VkCommandPool m_vkcommandpool;
//
//      /** @brief Contains queue family indices */
//      directx12::QueueFamilyIndices m_queuefamilyindices;
//      //struct
//      //{
//      //   uint32_t graphics;
//      //   uint32_t compute;
//      //   uint32_t transfer;
//      //} m_queuefamilyindices;
//
//
//
//
//
//      //graphics3d_directx12::context
//#if defined(NDEBUG)
//      const bool enableValidationLayers = false;
//#else
//      const bool enableValidationLayers = true;
//#endif
//
//      VkInstance m_vkinstance;
//      VkDebugUtilsMessengerEXT debugMessenger;
//      //VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
//      //::pointer < ::user::graphics3d > m_pimpact;
//
//      //VkCommandPool m_vkcommandpool;
//
//      //VkDevice m_vkdevice;
//      //VkSurfaceKHR m_vksurfacekhr;
      //VkQueue m_vkqueueGraphics;
      //VkQueue m_vkqueuePresent;

      //::array<const char *> validationLayers;
      //::array<const char *> deviceExtensions;

      ::pointer<::gpu_directx12::set_descriptor_layout>           m_psetdescriptorlayoutGlobal;
      //::array<VkDescriptorSet>                                 m_descriptorsetsGlobal;
      //::pointer_array<::gpu_directx12::buffer>							m_uboBuffers;
      ::pointer <::gpu_directx12::descriptor_pool>                m_pdescriptorpoolGlobal;


      ::pointer <::gpu_directx12::shader>                m_pshaderBlend3;
      //::comptr < ID3D12BlendState >                      m_pd3d11blendstateBlend3;


      // For IDXGIDevice
      class d3d11on12 :
         virtual public particle
      {
      public:

         ::comptr<ID3D11Device> m_pd3d11device;
         ::comptr<ID3D11DeviceContext> m_pd3d11context;
         ::comptr<ID3D11On12Device> m_pd3d11on12;
         ::comptr<IDXGIDevice> m_pdxgidevice;


         ::comptr<ID3D12Fence> dx12Fence;
         UINT64 fenceValue = 0;
         ::comptr<ID3D11Fence> dx11Fence;
         ::comptr<ID3D11Device5> dx11Device5;
         ::comptr<ID3D11DeviceContext4> dx11Context4;
         HANDLE fenceEvent = NULL;
         HANDLE sharedFenceHandle = nullptr;

      };
      //::pointer_array < class d3d11on12 > >     m_d3d11on12a;
      ::pointer < class d3d11on12 > m_pd3d11on12;


      context();
      ~context() override;


      void on_create_context(::gpu::device* pgpudevice, const ::gpu::enum_output& eoutput, ::windowing::window* pwindow, const ::int_size& size) override;

      virtual void initialize_gpu_context_swap_chain(::gpu::device* pgpudevice, ::windowing::window* pwindow);

      string _001GetIntroProjection() override;
      string _001GetIntroFragment() override;

      void draw() override;
      void start_drawing() override;
      void global_transform() override;
      void render() override;
      void on_start_layer(::gpu::layer* player);
      void on_end_layer(::gpu::layer* player);
      void set_bitmap_1(::image::image *pimage) override;

      virtual ID3D12CommandQueue* command_queue();


      virtual d3d11on12* d3d11on12();

      //virtual ::gpu_directx12::texture* _layer_source_texture(::gpu::layer* player);

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
      
      void set_matrix_uniform(const ::gpu::payload & uniformMatrix) override;




      virtual void _create_context_directx12(::gpu::device* pgpudevice, const ::gpu::enum_output& eoutput, ::windowing::window* pwindow, const ::int_size& size);

      //void on_create_context(const ::gpu::start_context_t & startcontext) override;


      //VkDevice logicalDevice();

      //virtual HRESULT createLogicalDevice(
      //   VkPhysicalDeviceFeatures enabledFeatures,
      //   ::array<const char *> enabledExtensions,
      //   void * pNextChain,
      //   bool useSwapChain = true,
      //   VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
      ////virtual uint32_t getQueueFamilyIndex(VkQueueFlags queueFlags) const;
      //virtual VkCommandPool createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
      //virtual bool isExtensionSupported(const ::scoped_string & scopedstrExtension);

      virtual void _create_offscreen_window(const ::int_size & size);

      void defer_create_window_context(::windowing::window * pwindow) override;
      void _defer_create_window_context(::windowing::window * pwindow) override;
      virtual void _create_window_context(::windowing::window * pwindow);

      //virtual void _create_window_buffer();
      void _create_cpu_buffer(const ::int_size & size) override;
      void resize_cpu_buffer(const ::int_size & size) override;
      void destroy_cpu_buffer() override;

      //void make_current() override;

      bool is_mesa() override;

      //void render();

      string get_shader_version_text() override;

      void _translate_shader(string_array & straFragment) override;

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
      //   VkBuffer & buffer,
      //   VkDeviceMemory & bufferMemory);
      //VkCommandBuffer beginSingleTimeCommands();
      //void endSingleTimeCommands(VkCommandBuffer commandBuffer);
      //void endSingleTimeCommands(VkCommandBuffer commandBuffer, int iSubmitCount, VkSubmitInfo * psubmitinfo);
      //void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
      //void copyBufferToImage(
      //   VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);
      //void createImageWithInfo(
      //   const VkImageCreateInfo & imageInfo,
      //   VkMemoryPropertyFlags properties,
      //   VkImage & image,
      //   VkDeviceMemory & imageMemory);

      //VkPhysicalDeviceProperties properties;


      //void submitWork(VkCommandBuffer cmdBuffer, VkQueue queue);

      void copy(::gpu::texture* ptextureTarget, ::gpu::texture* ptextureSource) override;

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
      //bool checkValidationLayerSupport();
      //void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT & createInfo);
      //void hasGflwRequiredInstanceExtensions();
      //bool checkDeviceExtensionSupport(VkPhysicalDevice pvkcdevice);

      //      //set_descriptor_layout* get_set_descriptor_layout(::gpu::context * pgpucontext);
      //virtual VkDescriptorSet getGlobalDescriptorSet(::gpu_directx12::renderer* prenderer);

      //::gpu_directx12::descriptor_pool* get_global_pool(int iFrameCount);


      ////set_descriptor_layout* get_set_descriptor_layout(::gpu::context * pgpucontext);
      //virtual VkDescriptorSet getGlobalDescriptorSet(::gpu::context* pgpucontext, ::gpu_directx12::renderer* prenderer);

      //::gpu_directx12::descriptor_pool* get_global_pool(::gpu::context* pgpucontext, int iFrameCount);


      //IDXGIDevice* __get_dxgi_device();
      //ID3D11Device* __get_d3d11_device();
      //ID3D11DeviceContext* __get_d3d11_device_context();

      void create_global_ubo(int iSize, int iFrameCount) override;
      void update_global_ubo(const ::block& block) override;


      void engine_on_frame_context_initialization() override;

      //ID3D11DeviceContext* draw_get_d3d11_device_context();
      //ID3D11DeviceContext1* draw_get_d3d11_device_context1();

      //bool create_offscreen_graphics_for_swap_chain_blitting(::gpu::graphics* pgraphics, const ::int_size& size = {}) override;

      
      void __bind_draw2d_compositor(::gpu::compositor* pgpucompositor, ::gpu::layer* player) override;
      void __soft_unbind_draw2d_compositor(::gpu::compositor* pgpucompositor, ::gpu::layer* player) override;


      virtual void __bind_graphics3d_compositor(::gpu::compositor* pgpucompositor, ::gpu::layer* player);

      ::gpu::enum_output get_eoutput() override;


      void on_begin_draw_attach(::gpu::graphics* pgpugraphics) override;
      void draw2d_on_begin_draw(::gpu::graphics* pgpugraphics) override;
      

      void on_create_texture(::gpu::texture* pgputext) override;
      //void on_take_snapshot(::gpu::layer* pgpulayer) override;

      void merge_layers(::gpu::texture* ptextureTarget, ::pointer_array < ::gpu::layer >* playera) override;

      //void swap_buffers();
      IDXGIDevice* _get_dxgi_device() override;
      ::gpu::swap_chain* get_swap_chain() override;

   };


} // namespace gpu




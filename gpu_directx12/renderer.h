#pragma once

//#include "AppCore/vk_window.h"
//#include "cube/container.h"
#include "context.h"
//#include "offscreen.h"
//#include "swapchain.h"
#include "render_target_view.h"
#include "bred/gpu/renderer.h"


constexpr unsigned int FRAME_OVERLAP = 2;


namespace gpu_directx12
{

   class CLASS_DECL_GPU_DIRECTX12 shader_vertex_input :
      virtual public ::particle
   {
   public:

      //::array < VkVertexInputBindingDescription >     m_bindings;
      //::array < VkVertexInputAttributeDescription >   m_attribs;

   };


   class CLASS_DECL_GPU_DIRECTX12 renderer :
      virtual public ::gpu::renderer
   {
   public:

      ::color::hls m_hlsClear;

      struct CLASS_DECL_GPU_DIRECTX12 cpu_buffer_sampler :
         virtual public ::particle
      {


         //VkExtent2D			      m_vkextent2d;
         //VkDeviceMemory		      m_vkdevicememory;
         //VkImage				      m_vkimage;


         ::pointer < context >   m_pgpucontext;
         ::pointer < renderer >  m_prenderer;
         
         


         comptr<ID3D12Resource>       m_presourceStagingTexture;
         ::int_size              m_size;

         D3D12_RESOURCE_DESC m_desc;

         D3D12_PLACED_SUBRESOURCE_FOOTPRINT m_footprint = {};;

         cpu_buffer_sampler();
         ~cpu_buffer_sampler();


         void initialize_cpu_buffer_sampler(::gpu::context* pgpucontext);

         void clear();
         //void update(VkExtent2D vkextent2d);
         void destroy();

         void sample(texture * ptexture);

         void send_sample();

      };


      class descriptor :
         virtual public ::particle
      {
      public:
         //::array<VkDescriptorSet>   m_descriptorsets;
         //VkPipelineLayout		      m_vkpipelinelayout = nullptr;
      };

      class model :
         virtual public ::particle
      {
      public:
         //VkBuffer m_vertexBuffer = nullptr;
         //VkDeviceMemory m_vertexMemory = nullptr;
         //VkBuffer m_indexBuffer = nullptr;
         //VkDeviceMemory m_indexMemory = nullptr;
      };

      ::pointer<::gpu::shader>                        m_pshaderImageBlend;
      ::pointer<::gpu::shader>                        m_pshaderImageSet;
      //::pointer<::gpu::shader>                        m_pshaderLayerBlend;
      //map < VkImage, ::pointer < descriptor > >       m_imagedescriptor;
      //map < VkImage, ::pointer < model > >       m_imagemodel;
      //::pointer < ::user::graphics3d >	m_pimpact;
      ::pointer < context >				               m_pgpucontext;
      ::pointer < cpu_buffer_sampler >	               m_pcpubuffersampler;
      //::pointer<swap_chain_render_target_view>			m_pvkcswapchain;
      //::pointer<offscreen_render_target_view>			m_pvkcoffscreen;
      //::pointer<render_target_view>			            m_prendertargetview;
      //::pointer<renderer>			                     m_prendererResolve;
      //::pointer<::gpu::shader>                        m_pshaderResolve;
      //::pointer<model>                                m_pmodelResolve;

      //::array<VkCommandBuffer>	commandBuffers;
      //VkExtent2D m_extentRenderer;
      //int currentFrameIndex = 0;
      bool isFrameStarted = false;
      bool m_bNeedToRecreateSwapChain = false;
      //bool m_bOffScreen = true;
      //renderer(VkWindow &window, context * pvkcdevice);
      //::int_size m_sizeRenderer;

      ::pointer<::gpu_directx12::set_descriptor_layout>           m_psetdescriptorlayoutImageBlend;
      ::pointer <::gpu_directx12::descriptor_pool>                m_pdescriptorpoolImageBlend;


      ::pointer<::gpu_directx12::set_descriptor_layout>           m_psetdescriptorlayoutResolve;
      ::pointer <::gpu_directx12::descriptor_pool>                m_pdescriptorpoolResolve;
      ::pointer < descriptor >                                    m_pdescriptorResolve;

      //::comptr<ID3D12CommandQueue>                                m_pcommandqueue;
      ::comptr<ID3D12CommandQueue>                                m_pcommandqueueCopy;



      //::pointer_array < command_buffer > m_commandbuffera;
      ::pointer_array < command_buffer > m_commandbuffera;

      ::pointer < command_buffer > m_pcommandbufferSingleTime;
      ::pointer < command_buffer > m_pcommandbufferLoadAssets;
      ::pointer < command_buffer > m_pcommandbufferLoadAssets2;

      ::comptr<ID3D12DescriptorHeap> m_pheapCbv;
      ::comptr<ID3D12Resource> m_presourceGlobalUBO;
      void* m_pGlobalUBO;
      //int m_iPushPropertiesAddress = 0;

      //::array <UINT64 > m_fences; // fences values

//#ifdef HELLO_TRIANGLE_DEBUG
      ::pointer < ::gpu_directx12::shader > m_pshaderHelloTriangle;
      ::comptr<ID3D12Resource> m_presourceHelloTriangleVertexBuffer;
      D3D12_VERTEX_BUFFER_VIEW m_vertexbufferviewHelloTriangle;
//#endif

      ::procedure_array m_procedureaAfterEndRender;

      //::int_size m_sizeRenderer;


      renderer();
      ~renderer();


//      virtual void restart_frame_counter();
      void on_new_frame() override;
      void initialize_gpu_renderer(::gpu::context* pgpucontext) override;

      //int width()  override;
      //int height() override;

      //virtual ::pointer <command_buffer >getLoadAssetsCommandBuffer();

      virtual ::pointer <command_buffer >beginSingleTimeCommands(D3D12_COMMAND_LIST_TYPE ecommandlisttype);
      

      virtual void endSingleTimeCommands(ID3D12CommandQueue* pcommandqueue, command_buffer * pcommandbuffer);

      void WaitForGpu();
      //void on_start_layer(::gpu::layer* player);
      //void on_end_layer(::gpu::layer* player) override;

      //bool is_starting_frame()const
      //{

      //   return m_iFrameSerial2 == m_iCurrentFrame2;

      //}

      //VkRenderPass getRenderPass() const
      //{

      //   return m_prendertargetview->getRenderPass();
      //   //return m_bOffScreen ?
      //      //m_pvkcoffscreen->getRenderPass():
      //      //m_pvkcswapchain->getRenderPass(); 

      //}

      void sample();
      void gpu_blend(::draw2d::graphics * pgraphics);
      //void resolve_color_and_alpha_accumulation_buffers();
      //void _resolve_color_and_alpha_accumulation_buffers();
      void swap_chain();

      float getAspectRatio() const
      {

         //if (m_bOffScreen)
         {

            ::cast < render_target_view > pgpurendertargetview = m_pgpurendertarget;

            return pgpurendertargetview->extentAspectRatio();

         }
         //else
         //{

         //	return m_pvkcswapchain->extentAspectRatio();

         //}

      }


      bool isFrameInProgress() const { return isFrameStarted; }


      //VkCommandBuffer getCurrentCommandBuffer() const {
      //   assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
      //   return commandBuffers[get_frame_index()];
      //}

      virtual command_buffer * getCurrentCommandBuffer2();

      virtual command_buffer* getLoadAssetsCommandBuffer();


      //int get_frame_index() override;
      //int get_frame_count() override;

      //void defer_update_renderer() override;
      //void on_defer_update_renderer_allocate_render_target(::gpu::enum_output eoutput, const ::int_size& size, ::gpu::render_target* previous) override;

      //::pointer < ::gpu::render_target > allocate_offscreen_render_target() override;

      //::pointer < ::graphics3d::frame> beginFrame() override;
      //void endFrame() override;
      //void on_begin_render(::graphics3d::frame * pframe) override;
      //void on_end_render(::graphics3d::frame * pframe) override;

      void on_context_resize() override;


   //public:

      void createCommandBuffers();

      void freeCommandBuffers();
      //void recreateSwapchain();
      //void set_size(const ::int_size & size) override;

      //void prepareOffScreen();

      ::pointer < ::gpu::frame > beginFrame() override;
      void on_begin_render(::gpu::frame* pframeParam) override;
      //virtual void on_begin_render1(::gpu::frame* pframeParam);
      void on_end_render(::gpu::frame* pframeParam) override;
      void endFrame() override;
      void endDraw(::draw2d_gpu::graphics * pgraphics, ::user::interaction * puserinteraction) override;

      void on_begin_draw() override;
      void _on_begin_render(::gpu::frame* pgpuframe) override;
      void _on_end_render(::gpu::frame* pgpuframe) override;
      void on_end_draw() override;

      //void _set_image(VkImage image, const ::int_rectangle& rectangle, bool bYSwap);

      //void _blend_image(VkImage image, const ::int_rectangle& rectangle, bool bYSwap);
      //void _on_graphics_end_draw(VkImage image, const ::int_rectangle& rectangle);

      void blend(::gpu::layer* player);

      void _blend_renderer(::gpu_directx12::renderer* prendererSrc, bool bYSwap);
      void _on_graphics_end_draw(::gpu_directx12::renderer * prendererSrc);

      //void _on_frame_draw(::gpu_directx12::renderer* prendererUpper);

      //void _copy_image(VkImage image, const ::int_rectangle& rectangle, bool bYSwap);

      ::gpu::shader * get_image_blend_shader();

      ::gpu::shader* get_image_set_shader();



   };


} // namespace gpu_directx12




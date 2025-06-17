#pragma once

//#include "AppCore/vk_window.h"
//#include "cube/container.h"
#include "context.h"
//#include "offscreen.h"
//#include "swapchain.h"
#include "render_pass.h"
#include "bred/gpu/renderer.h"



namespace gpu_vulkan
{


   class CLASS_DECL_GPU_VULKAN shader_vertex_input :
      virtual public ::particle
   {
   public:

      ::array < VkVertexInputBindingDescription >     m_bindings;
      ::array < VkVertexInputAttributeDescription >   m_attribs;

   };


   class CLASS_DECL_GPU_VULKAN renderer :
      virtual public ::gpu::renderer
   {
   public:


      struct CLASS_DECL_GPU_VULKAN cpu_buffer_sampler :
         virtual public ::particle
      {


         VkExtent2D			      m_vkextent2d;
         VkDeviceMemory		      m_vkdevicememory;
         VkImage				      m_vkimage;


         ::pointer < context >   m_pgpucontext;
         ::pointer < renderer >  m_prenderer;


         cpu_buffer_sampler();
         ~cpu_buffer_sampler();


         void initialize_cpu_buffer_sampler(::gpu::context* pgpucontext);

         void clear();
         void update(VkExtent2D vkextent2d);
         void destroy();

         void sample(VkImage vkimage);

         void send_sample();

      };



      class model :
         virtual public ::particle
      {
      public:
         VkBuffer m_vertexBuffer = nullptr;
         VkDeviceMemory m_vertexMemory = nullptr;
         VkBuffer m_indexBuffer = nullptr;
         VkDeviceMemory m_indexMemory = nullptr;
      };


      //::pointer<blend2>                               m_pblend2;

      ::pointer < shader >                       m_pshaderImageBlend;
      ::pointer < shader >                       m_pshaderImageSet;
      ::pointer < shader >                       m_pshaderBlend2;
      ::pointer < shader >                       m_pshaderCopyImage;

      //::pointer<::gpu::shader>                        m_pshaderImageBlend;
      //::pointer<::gpu::shader>                        m_pshaderImageSet;
      //map < VkImage, ::pointer < descriptor > >       m_imagedescriptor;
      map < VkImage, ::pointer < model > >       m_imagemodel;
      //::pointer < ::user::graphics3d >	m_pimpact;
      ::pointer < context >				               m_pgpucontext;
      ::pointer < cpu_buffer_sampler >	               m_pcpubuffersampler;
      //::pointer<swap_chain_render_pass>			m_pvkcswapchain;
      //::pointer<offscreen_render_pass>			m_pvkcoffscreen;
      //::pointer<render_pass>			                  m_pvkcrenderpass;
      //::pointer<renderer>			                     m_prendererResolve;
      //::pointer<::gpu::shader>                        m_pshaderResolve;
      //::pointer<model>                                m_pmodelResolve;

      ::pointer_array<command_buffer>	         m_commandbuffera;
      VkExtent2D m_extentRenderer;
      //int currentFrameIndex = 0;
      bool isFrameStarted = false;
      bool m_bNeedToRecreateSwapChain = false;
      //bool m_bOffScreen = true;
      //renderer(VkWindow &window, context * pvkcdevice);

      //::pointer<::gpu_vulkan::set_descriptor_layout>           m_psetdescriptorlayoutImageBlend;
      //::pointer <::gpu_vulkan::descriptor_pool>                m_pdescriptorpoolImageBlend;


      ::pointer<shader  >           m_pshaderResolve;
      //::pointer<::gpu_vulkan::set_descriptor_layout>           m_psetdescriptorlayoutResolve;
      //::pointer <::gpu_vulkan::descriptor_pool>                m_pdescriptorpoolResolve;
      //::pointer < descriptor_set >                                      m_pdescriptorsetResolve;

      ::procedure_array m_procedureaAfterEndRender;

      //::collection::index m_iCurrentFrame2 = -1;
      //::collection::index m_iFrameSerial2 = -1;


      renderer();
      ~renderer();


      virtual void restart_frame_counter();
      void on_new_frame() override;
      void initialize_renderer(::gpu::context* pgpucontext) override;

      //int width()  override;
      //int height() override;

      bool is_starting_frame()const
      {

         return m_iFrameSerial2 == m_iCurrentFrame2;

      }

      VkRenderPass getRenderPass() const
      {

         ::cast < render_pass > prenderpass = m_pgpurendertarget;

         return prenderpass->getRenderPass();
         //return m_bOffScreen ?
            //m_pvkcoffscreen->getRenderPass():
            //m_pvkcswapchain->getRenderPass(); 

      }

      void sample();
      //void resolve_color_and_alpha_accumulation_buffers();
      //void _resolve_color_and_alpha_accumulation_buffers();
      void swap_chain();

      float getAspectRatio() const
      {

         //if (m_bOffScreen)
         {

            ::cast < render_pass > prenderpass = m_pgpurendertarget;

            return prenderpass->extentAspectRatio();

         }
         //else
         //{

         //	return m_pvkcswapchain->extentAspectRatio();

         //}

      }


      bool isFrameInProgress() const { return isFrameStarted; }


      command_buffer* getCurrentCommandBuffer();



      int get_frame_index() override;
      int get_frame_count() override;

      void defer_update_renderer();

      ::gpu::render_target* back_buffer_render_target() override;



      //::pointer < ::graphics3d::frame> beginFrame() override;
      //void endFrame() override;
      //void on_begin_render(::graphics3d::frame * pframe) override;
      //void on_end_render(::graphics3d::frame * pframe) override;

      void on_context_resize() override;

      void on_begin_draw() override;
      virtual void _on_begin_render();
      virtual void _on_end_render();
      void on_end_draw() override;

   //public:

      void createCommandBuffers();

      void freeCommandBuffers();
      //void recreateSwapchain();
      //void set_size(const ::int_size & size) override;

      //void prepareOffScreen();

      ::pointer < ::gpu::frame > beginFrame() override;
      void on_begin_render(::gpu::frame* pframeParam) override;
      void on_end_render(::gpu::frame* pframeParam) override;
      void endFrame() override;
      void endDraw(::draw2d_gpu::graphics * pgraphics, ::user::interaction * puserinteraction) override;


      void _set_image(::gpu::texture * pgputexture, const ::int_rectangle& rectangle, bool bYSwap);

      void _blend_image(::gpu::texture* pgputexture, const ::int_rectangle& rectangle, bool bYSwap);
      void _on_graphics_end_draw(::gpu::texture* pgputexture, const ::int_rectangle& rectangle);

      void _blend_renderer(::gpu_vulkan::renderer* prendererSrc, bool bYSwap);
      void _on_graphics_end_draw(::gpu_vulkan::renderer * prendererSrc);

      //void _on_frame_draw(::gpu_vulkan::renderer* prendererUpper);

      void copy(::gpu::texture* pgputextureTarget, ::gpu::texture* pgputextureSource) override;

      shader * _get_image_blend_shader();

      shader* _get_image_set_shader();

      void blend(::gpu::renderer* prenderer) override;

      void soft_restore_context() override;


      void blend(::gpu::texture* ptextureTarget, ::gpu::texture* ptextureSource) override;

   };


} // namespace gpu_vulkan




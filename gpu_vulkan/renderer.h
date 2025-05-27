#pragma once

//#include "AppCore/vk_window.h"
//#include "app-cube/cube/container.h"
#include "context.h"
//#include "offscreen.h"
//#include "swapchain.h"
#include "render_pass.h"
#include "app-cube/cube/gpu/renderer.h"


#include <memory>
#include <vector>
#include <cassert>

constexpr unsigned int FRAME_OVERLAP = 2;


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



      struct OffScreenSampler :
         virtual public ::particle
      {

         VkExtent2D			m_vkextent2d;
         VkDeviceMemory		m_vkdevicememory;
         VkImage				m_vkimage;

         ::pointer < context > m_pgpucontext;
         ::pointer < renderer > m_prenderer;




         OffScreenSampler();
         ~OffScreenSampler();


         void initialize_offscreen_sampler(::gpu::context* pgpucontext);

         void clear();
         void update(VkExtent2D vkextent2d);
         void destroy();

         void sample(VkImage vkimage);

         void send_sample();

      };

      class descriptor :
         virtual public ::particle
      {
      public:
         ::array<VkDescriptorSet>   m_descriptorsets;
         VkPipelineLayout		      m_vkpipelinelayout = nullptr;
      };

      class model :
         virtual public ::particle
      {
      public:
         VkBuffer m_vertexBuffer;
         VkDeviceMemory m_vertexMemory;
         VkBuffer m_indexBuffer;
         VkDeviceMemory m_indexMemory;
      };

      ::pointer<::gpu::shader>                        m_pshaderImageBlend;
      map < VkImage, ::pointer < descriptor > >       m_imagedescriptor;
      map < VkImage, ::pointer < model > >       m_imagemodel;
      //::pointer < ::cube::impact >	m_pimpact;
      ::pointer < context >				               m_pgpucontext;
      ::pointer < OffScreenSampler >	               m_poffscreensampler;
      //::pointer<swap_chain_render_pass>			m_pvkcswapchain;
      //::pointer<offscreen_render_pass>			m_pvkcoffscreen;
      ::pointer<render_pass>			                  m_pvkcrenderpass;
      ::array<VkCommandBuffer>	commandBuffers;
      VkExtent2D m_extentRenderer;
      uint32_t currentImageIndex;
      int currentFrameIndex = 0;
      bool isFrameStarted = false;

      bool m_bNeedToRecreateSwapChain = false;
      //bool m_bOffScreen = true;
      //renderer(VkWindow &window, context * pvkcdevice);

      ::pointer<::gpu_vulkan::set_descriptor_layout>           m_psetdescriptorlayoutImageBlend;
      ::pointer <::gpu_vulkan::descriptor_pool>                m_pdescriptorpoolImageBlend;
      ::procedure_array m_procedureaAfterEndRender;

      renderer();
      ~renderer();


      void initialize_renderer(::gpu::context* pgpucontext) override;

      //int width()  override;
      //int height() override;

      VkRenderPass getRenderPass() const
      {

         return m_pvkcrenderpass->getRenderPass();
         //return m_bOffScreen ?
            //m_pvkcoffscreen->getRenderPass():
            //m_pvkcswapchain->getRenderPass(); 

      }

      void sample();
      void swap_chain();

      float getAspectRatio() const
      {

         //if (m_bOffScreen)
         {

            return m_pvkcrenderpass->extentAspectRatio();

         }
         //else
         //{

         //	return m_pvkcswapchain->extentAspectRatio();

         //}

      }


      bool isFrameInProgress() const { return isFrameStarted; }


      VkCommandBuffer getCurrentCommandBuffer() const {
         assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
         return commandBuffers[currentFrameIndex];
      }

      int get_frame_index() override;
      int get_frame_count() override;

      void defer_update_render_pass();

      //::pointer < ::graphics3d::frame> beginFrame() override;
      //void endFrame() override;
      //void on_begin_render(::graphics3d::frame * pframe) override;
      //void on_end_render(::graphics3d::frame * pframe) override;

      void set_placement(const ::int_rectangle& rectanglePlacement) override;

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

      void _blend_image(VkImage image, const ::int_rectangle& rectangle);
      void _on_graphics_end_draw(VkImage image, const ::int_rectangle& rectangle);


      ::gpu::shader * get_image_blend_shader();

   };


} // namespace gpu_vulkan




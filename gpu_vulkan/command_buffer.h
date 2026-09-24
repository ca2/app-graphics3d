// Created by camilo on 2025-06-16 04:40 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/command_buffer.h"


namespace gpu_vulkan
{


   class command_buffer :
      virtual public ::gpu::command_buffer
   {
   public:


      
      VkCommandBuffer         m_vkcommandbuffer;
      VkCommandBufferLevel    m_vkcommandbufferlevel;
      VkCommandPool           m_vkcommandpool;
      VkViewport              m_vkviewport;
      VkRect2D                m_vkrect2dScissor;
      bool                    m_bViewportSet;
      bool                    m_bScissorSet;
      bool                    m_bFenceWaitIfNoPreexistingFence = false;

      //bool m_bPresentQueue;
      //::array < VkSemaphore> m_semaphoreaSignalOnSubmit;
      //::array < VkSemaphore> m_semaphoreaWaitToSubmit;
      ::array < VkPipelineStageFlags > m_vkpipelinestageflagsaWait;



      command_buffer();
      ~command_buffer() override;


      void initialize_command_buffer(::gpu::render_target * pgpurendertarget, ::gpu::queue * pqueue, ::gpu::enum_command_buffer ecommandbuffer) override;

      void begin_command_buffer(bool bOneTime) override;


      void wait_commands_to_execute() override;

      void submit_command_buffer(::gpu::layer * pgpulayer) override;

      virtual void set_line_width(float fLineWidth);

      void clear(::gpu::texture * pgputexture, const ::color::color & color) override;

      void set_viewport(const ::i32_rectangle & rectangle, const ::i32_size & sizeRaw = {}) override;

      void set_scissor(const ::i32_rectangle & rectangle, const ::i32_size & sizeRaw = {}) override;

      //void set_primitive_topology_triangle_strip() override;

      virtual void draw(::gpu_vulkan::model_buffer* pmodelbuffer);

      // virtual VkResult submitCommandBuffers(
      //    ::gpu::texture* pgputextureTarget,
      //    const ::pointer_array < ::gpu::texture >& gputextureaSource,
      //    const ::array < VkSemaphore >& semaphoreaWait,
      //    const ::array < VkPipelineStageFlags >& stageaWait,
      //    const ::array < VkSemaphore >& semaphoreaSignal,
      //    VkFence * pvkfence = nullptr);


      // virtual VkResult submitCommandBuffers(
      //    ::gpu::texture* pgputextureTarget,
      //    const ::pointer_array < ::gpu::texture >& gputextureaSource,
      //    const ::array < VkSemaphore >& semaphoreaWait,
      //    const ::array < VkPipelineStageFlags >& stageaWait,
      //    const ::array < VkSemaphore >& semaphoreaSignal);

      void draw_vertexes(int iVertexCount) override;
      void draw_indexes(int iIndexCount) override;


      void begin_render(::gpu::shader *pgpushader, ::gpu::texture_site *pgputexturesiteTarget) override;
      void end_render() override;



   };



} // namespace gpu_vulkan




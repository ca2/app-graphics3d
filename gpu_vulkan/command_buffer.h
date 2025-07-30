// Created by camilo on 2025-06-16 04:40 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/command_buffer.h"


namespace gpu_vulkan
{


   class command_buffer :
      virtual public ::gpu::command_buffer
   {
   public:


      
      VkCommandBuffer   m_vkcommandbuffer;
      VkCommandPool     m_vkcommandpool;
      VkFence           m_vkfence;

      //bool m_bPresentQueue;
      ::array < VkSemaphore> m_semaphoreaSignalOnSubmit;
      ::array < VkSemaphore> m_semaphoreaWaitToSubmit;
      ::array < VkPipelineStageFlags > m_stageaWaitToSubmit;



      command_buffer();
      ~command_buffer() override;


      void initialize_command_buffer(::gpu::render_target * pgpurendertarget, ::gpu::enum_command_buffer ecommandbuffer) override;

      void begin_command_buffer(bool bOneTime) override;


      void wait_commands_to_execute() override;

      void submit_command_buffer(::gpu::layer * pgpulayer) override;

      virtual void set_line_width(float fLineWidth);

      void set_viewport(const ::int_rectangle & rectangle) override;

      void set_scissor(const ::int_rectangle& rectangle) override;  

      //void set_primitive_topology_triangle_strip() override;

      virtual void draw(::gpu_vulkan::model_buffer* pmodelbuffer);

      virtual VkResult submitCommandBuffers(
         ::gpu::texture* pgputextureTarget,
         const ::pointer_array < ::gpu::texture >& gputextureaSource,
         const ::array < VkSemaphore >& semaphoreaWait,
         const ::array < VkPipelineStageFlags >& stageaWait,
         const ::array < VkSemaphore >& semaphoreaSignal,
         VkFence * pvkfence = nullptr);

      void draw_vertices(int iVertexCount) override;
      void draw_indices(int iIndexCount) override;


   };



} // namespace gpu_vulkan




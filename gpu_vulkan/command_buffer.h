// Created by camilo on 2025-06-16 04:40 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/command_buffer.h"


namespace gpu_vulkan
{


   class command_buffer :
      virtual public ::gpu::command_buffer
   {
   public:


      
      VkCommandBuffer m_vkcommandbuffer;


      command_buffer();
      ~command_buffer() override;


      void initialize_command_buffer(::gpu::render_target * pgpurendertarget) override;

      virtual void begin_command_buffer(bool bOneTime);


      void wait_commands_to_execute() override;

      void submit_command_buffer(::gpu::layer * pgpulayer) override;

      virtual void set_line_width(float fLineWidth);

      void set_viewport(const ::int_rectangle & rectangle) override;

      void set_scissor(const ::int_rectangle& rectangle) override;  

      virtual void draw(::gpu_vulkan::model_buffer* pmodelbuffer);


   };


} // namespace gpu_vulkan




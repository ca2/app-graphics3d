// Created by camilo on 2025-06-16 04:40 <3ThomasBorregaardSørensen!!
#pragma once


namespace gpu_vulkan
{


   class command_buffer :
      virtual public ::particle
   {
   public:


      ::pointer < ::gpu::renderer > m_pgpurenderer;

      VkCommandBuffer m_vkcommandbuffer;


      command_buffer();
      ~command_buffer() override;


      virtual void initialize_command_buffer(::gpu::renderer * pgpurenderer);

      virtual void begin_command_buffer();


   };


} // namespace gpu_vulkan




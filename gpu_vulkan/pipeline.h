#pragma once


#include "context.h"


namespace gpu_vulkan
{



   class CLASS_DECL_GPU_VULKAN pipeline :
      virtual public ::particle
   {
   public:


      ::pointer < renderer >        m_pgpurenderer;
      VkPipeline                    m_vkpipelineGraphics;
      VkShaderModule                m_vkshadermoduleVertex;
      VkShaderModule                m_vkshadermoduleFragment;
      VkPipelineLayout		         m_vkpipelinelayout;


      pipeline();
      ~pipeline() override;


      //virtual void initialize_pipeline(
      //   ::gpu::context * pgpucontext,
      //   const ::file::path & pathVert,
      //   const ::file::path & pathFrag,
      //   const pipeline_configuration & configInfo);

      virtual VkPipelineLayout _pipeline_layout();


      virtual void initialize_graphics_pipeline(
         ::gpu::renderer * pgpurenderer,
         const ::block & blockVertex,
         const ::block & blockFragment,
         const ::vulkan::pipeline_configuration & pipelineconfiguration);



      void bind(command_buffer * pcommandbuffer);

      void create_graphics_pipeline(
         const ::block & blockVertex,
         const ::block & blockFragment,
         const ::vulkan::pipeline_configuration & pipelineconfiguration);

      void create_shader_module(const ::block & block, VkShaderModule * shaderModule);

   };


} // namespace gpu_vulkan 




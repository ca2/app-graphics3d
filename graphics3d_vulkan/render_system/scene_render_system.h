// Changed by camilo on 2025-12-06 21:17 <3ThomasBorregaardSørensen!!
#pragma once


#include "app-graphics3d/graphics3d_vulkan/render_system.h"
#include "app-graphics3d/graphics3d/render_system/scene_render_system.h"
#include "bred/gpu/properties.h"


namespace graphics3d_vulkan
{


   class scene_render_system : virtual public ::graphics3d::scene_render_system,
                               virtual public ::graphics3d_vulkan::render_system
   {
   public:


      //struct push_constants
      //{

      //   floating_matrix4 modelMatrix;
      //   floating_matrix4 normalMatrix;

      //   int useTextureAlbedo;
      //   int useTextureNormal;
      //   //int useTextureMetallicRoughness;
      //   //int useTextureAmbientOcclusion; 
      //   //int useTextureEmissive; 

      //   ::floating_sequence3 albedo; 
      //   float metallic; 
      //   float roughness; 
      //   float ambientOcclusion; 
      //   ::floating_sequence3 emissive; // 44

      //   float bloomBrightnessCutoff; 
      //   //int useAlphaMask;
      //   //float alphaMaskCutOff;
      //   floating_sequence3 multiplier;

      //};


      //::pointer<::gpu_vulkan::descriptor_set_layout> m_pdescriptorsetlayoutIbl;
      //::pointer<::gpu_vulkan::descriptor_set_layout> m_pdescriptorsetlayoutPbr;
      //::pointer<::gpu_vulkan::descriptor_pool> m_pdescriptorpool;

      

      //::pointer<::gpu_vulkan::pipeline> m_ppipelineOpaque;
      //::pointer<::gpu_vulkan::pipeline> m_ppipelineMask;
      //::pointer<::gpu_vulkan::pipeline> m_ppipelineBlend;
//      VkPipelineLayout m_pipelineLayout;

      scene_render_system();
      ~scene_render_system();

      void on_prepare_2025(::gpu::context *pgpucontext);
      void on_prepare(::gpu::context *pgpucontext) override;
      //void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
      //void createPipeline(VkRenderPass renderPass);

      
      void on_render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscene) override;


   };


} // namespace graphics3d_vulkan








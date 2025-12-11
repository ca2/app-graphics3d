// From V0idsEmbrace.gltf_render_system by 
// camilo on 2025-04-24 21:28 <3ThomasBorregaardSorensen!!
#pragma once


#include "bred/graphics3d/render_systems/gltf_render_system.h"
#include "app-graphics3d/graphics3d_vulkan/render_system.h"


namespace graphics3d_vulkan
{


   class CLASS_DECL_GRAPHICS3D_VULKAN gltf_render_system : 
      virtual public ::graphics3d::gltf_render_system,
      virtual public ::graphics3d_vulkan::render_system
   {
   public:

      struct push_constants
      {

         floating_matrix4 modelMatrix;
         floating_matrix4 normalMatrix;

         int useTextureAlbedo;//0
         int useTextureMetallicRoughness;//4
         int useTextureNormal;//8
         int useTextureAmbientOcclusion;//12
         int useTextureEmissive;//16

         // 20

         ::floating_sequence3 albedo;//20
         float metallic;//32
         float roughness;//36
         float ambientOcclusion;//40
         ::floating_sequence3 emissive;//44

         // 36

         // 56

         //::floating_sequence3 cameraPosition;//56
         float bloomBrightnessCutoff;//68
         // 72
         floating_sequence3 multiplier;

      };


      ::pointer<::gpu_vulkan::descriptor_set_layout> m_pdescriptorsetlayoutIbl;
      ::array_base<VkDescriptorSet> m_vkdescriptorsetaIbl;
      ::pointer<::gpu_vulkan::descriptor_set_layout> m_pdescriptorsetlayoutPbr;
      ::pointer<::gpu_vulkan::descriptor_pool> m_pdescriptorpool;




      gltf_render_system();
      ~gltf_render_system();


      void on_prepare(::gpu::context *pgpucontext) override;

      virtual ::block embedded_pbr_vert();
      virtual ::block embedded_pbr_frag();


      void on_render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscene) override;


   };


} // namespace graphics3d_vulkan
 

DECLARE_GPU_PROPERTIES(CLASS_DECL_GRAPHICS3D_VULKAN, ::graphics3d_vulkan::gltf_render_system::push_constants)




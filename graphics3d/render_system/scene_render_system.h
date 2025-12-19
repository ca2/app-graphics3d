// Changed by camilo on 2025-12-06 21:36 <3ThomasBorregaardSørensen!!
#pragma once


#include "app-graphics3d/graphics3d/render_system/pbr_with_ibl_render_system.h"
#include "bred/gpu/properties.h"


namespace graphics3d
{


   class CLASS_DECL_GRAPHICS3D scene_render_system : 
      virtual public ::graphics3d::pbr_with_ibl_render_system
   {
   public:


            struct push_constants
      {

         floating_matrix4 modelMatrix;
         floating_matrix4 normalMatrix;

         int useTextureAlbedo;
         int useTextureNormal;
         // int useTextureMetallicRoughness;
         // int useTextureAmbientOcclusion;
         // int useTextureEmissive;

         ::floating_sequence3 albedo;
         float metallic;
         float roughness;
         float ambientOcclusion;
         ::floating_sequence3 emissive; // 44

         float bloomBrightnessCutoff;
         // int useAlphaMask;
         // float alphaMaskCutOff;
         floating_sequence3 multiplier;
         float prefilteredEnvMapMaxLod;
      };


      ::pointer<::gpu::binding_set> m_pbindingsetIbl;
      ::pointer<::gpu::binding_set> m_pbindingsetPbr;
      //::pointer<::gpu::descriptor_pool> m_pdescriptorpool;


      scene_render_system();
      ~scene_render_system();

      ::memory pbr_with_ibl_vert_memory() override;
      ::memory pbr_with_ibl_frag_memory() override;

      ::gpu::property *push_constants_properties() override;

      ::gpu::binding_set *pbr_binding_set() override;

      virtual ::memory scene_gltf_vert_memory();
      virtual ::memory scene_gltf_frag_memory();


      void on_prepare(::gpu::context *pgpucontext) override;

      void on_render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscene) override;


   };


} // namespace graphics3d



DECLARE_GPU_PROPERTIES(CLASS_DECL_GRAPHICS3D, ::graphics3d::scene_render_system::push_constants);




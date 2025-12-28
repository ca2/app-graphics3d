// From github:/tristancalderbank/OpenGL-PBR-Renderer/gltf_material.h by
// camilo on 2025-09-26 18:28 <3ThomasBorregaardSorensen!!
#pragma once


#include "gpu/gltf/material.h


namespace gpu_vulkan
{


   namespace gltf
   {


      class CLASS_DECL_GPU_VULKAN material :
         virtual public ::gpu::model::material
      {

      public:

         //bool useTextureAlbedo = false;
         //bool useTextureMetallicRoughness = false;
         //bool useTextureNormal = false;
         //bool useTextureAmbientOcclusion = false;
         //bool useTextureEmissive = false;

         //floating_sequence3 m_seq3Albedo = constants::colorRed;
         //float m_fMetallic = 1.0f;
         //float m_fRoughness = 0.0f;
         //float m_fAmbientOcclusion = 1.0f;
         //floating_sequence3 m_seq3Emissive = floating_sequence3(0.0, 0.0, 0.0);

         //::pointer<::gpu::texture> m_ptextureAlbedo;
         //::pointer<::gpu::texture> m_ptextureMetallicRoughness;
         //::pointer<::gpu::texture> m_ptextureNormal;
         //::pointer<::gpu::texture> m_ptextureAmbientOcclusion;
         //::pointer<::gpu::texture> m_ptextureEmissive;

         ::map_base<::gpu::binding_set *, ::pointer < ::array<VkDescriptorSet>>> m_mapdescriptorset;


                  virtual void update_binding_set(::gpu::binding_slot_set *pgpubindinslotset, uint32_t descriptorBindingFlags,
                                         ::gpu_vulkan::texture *fallbackTexture);

      };


   } // namespace gltf


} // namespace gpu_vulkan

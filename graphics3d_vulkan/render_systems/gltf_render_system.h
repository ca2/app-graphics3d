// From V0idsEmbrace.gltf_render_system by 
// camilo on 2025-04-24 21:28 <3ThomasBorregaardSorensen!!
#pragma once


// #include "SceneFoundry/graphics3d/render_system.h"

#include "bred/graphics3d/render_systems/gltf_render_system.h"
#include "app-graphics3d/graphics3d_vulkan/render_system.h"

// #include "SceneFoundry/graphics3d/asset_provider_i.h"
// #include "SceneFoundry/graphics3d/game_object_i.h"

// #include "device.h"
// #include "pipeline.h"
// #include "SceneFoundry/graphics3d/descriptor.h"

// #include "gltf.h"
// #include "SceneFoundry/graphics3d/renderer.h"

// STD
// #include <memory>
// #include <vector>

// #include <vulkan/vulkan.h>

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

         ::floating_sequence3 cameraPosition;//56
         float bloomBrightnessCutoff;//68
         // 72

      };


      //VkSandboxDevice &m_device;

      ::pointer<::gpu_vulkan::descriptor_set_layout> m_pdescriptorsetlayoutIbl;
      ::array_base<VkDescriptorSet> m_vkdescriptorsetaIbl;

      ::pointer<::gpu_vulkan::descriptor_set_layout> m_pdescriptorsetlayoutPbr;

      //::pointer<::gpu_vulkan::descriptor_set_layout> m_pdescriptorsetlayoutUbo;
      
      
      ::pointer<::gpu_vulkan::descriptor_pool> m_pdescriptorpool;
      //VkDescriptorSetLayout m_iblSetLayout;
      //VkDescriptorSet m_iblDescriptorSet;

      //::pointer<::gpu_vulkan::pipeline> m_opaquePipeline;
      //::pointer<::gpu_vulkan::pipeline> m_maskPipeline;
      //::pointer<::gpu_vulkan::pipeline> m_blendPipeline;
      //VkPipelineLayout m_pipelineLayout;

      //IAssetProvider &m_assets;




      gltf_render_system();
      ~gltf_render_system();


      //virtual void init(VkRenderPass renderPass, 
      //   ::gpu_vulkan::descriptor_set_layout * psetdescriptorlayoutGlobal,
      //          ::pointer < ::gpu_vulkan::descriptor_set_layout > &descriptorPool,
      //   size_t frameCount);
      //virtual void init(::gpu_vulkan::descriptor_set_layout *psetdescriptorlayoutGlobal,
        //                ::pointer<::gpu_vulkan::descriptor_set_layout> &descriptorPool, size_t frameCount);

      void on_prepare(::gpu::context *pgpucontext) override;

      virtual ::block embedded_pbr_vert();
      virtual ::block embedded_pbr_frag();

      //void initialize_GltfRenderSystem(VkRenderPass renderPass,
      //   ::gpu_vulkan::descriptor_set_layout *psetdescriptorlayoutGlobal, 
      //   ::graphics3d::asset_manager * passetmanager);
      //void initialize_GltfRenderSystem(

      //                                 ::gpu_vulkan::descriptor_set_layout *psetdescriptorlayoutGlobal,
      //                                 ::graphics3d::asset_manager *passetmanager);

      ///void initialize_GltfRenderSystem(

         //;
      ///;::gpu_vulkan::descriptor_set_layout *psetdescriptorlayoutGlobal);

      //void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
      //void createPipeline(VkRenderPass renderPass);

      
      void on_render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscene) override;


   };


} // namespace graphics3d_vulkan
 


DECLARE_GPU_PROPERTIES(CLASS_DECL_GRAPHICS3D_VULKAN, ::graphics3d_vulkan::gltf_render_system::push_constants)


// From V0idsEmbrace.gltf_render_system by 
// camilo on 2025-04-24 21:28 <3ThomasBorregaardSorensen!!
#pragma once


// #include "SceneFoundry/graphics3d/render_system.h"

#include "bred/graphics3d/render_systems/gltf_render_system.h"
#include "app-graphics3d/graphics3d_opengl/render_system.h"

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

namespace graphics3d_opengl
{


   class gltf_render_system : 
      virtual public ::graphics3d::gltf_render_system,
      virtual public ::graphics3d_opengl::render_system
   {
   public:

       const int TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP = 10;
      const int TEXTURE_UNIT_PREFILTERED_ENV_MAP = 11;
      const int TEXTURE_UNIT_BRDF_CONVOLUTION_MAP = 12;

      enum BloomDirection
      {
         BOTH = 0,
         HORIZONTAL = 1,
         VERTICAL = 2
      };

      //VkSandboxDevice &m_device;

      //::pointer<::gpu_opengl::descriptor_set_layout> m_pdescriptorsetlayoutIbl;
      //::array_base<VkDescriptorSet> m_vkdescriptorsetaIbl;

      //::pointer<::gpu_opengl::descriptor_set_layout> m_pdescriptorsetlayoutPbr;
      //::array_base<VkDescriptorSet> m_vkdescriptorsetaPbr;

      //::pointer<::gpu_opengl::descriptor_set_layout> m_pdescriptorsetlayoutUbo;
      
      
      //::pointer<::gpu_opengl::descriptor_pool> m_pdescriptorpool;
      //VkDescriptorSetLayout m_iblSetLayout;
      //VkDescriptorSet m_iblDescriptorSet;

      //::pointer<::gpu::shader> m_ppipelineOpaque;
      //::pointer<::gpu::shader> m_ppipelineMask;
      //::pointer<::gpu::shader> m_ppipelineBlend;
      // VkPipelineLayout m_pipelineLayout;

      //IAssetProvider &m_assets;




      gltf_render_system();
      ~gltf_render_system();


      //virtual void init(VkRenderPass renderPass, 
      //   ::gpu_opengl::descriptor_set_layout * psetdescriptorlayoutGlobal,
      //          ::pointer < ::gpu_opengl::descriptor_set_layout > &descriptorPool,
      //   size_t frameCount);
      //virtual void init(::gpu_opengl::descriptor_set_layout *psetdescriptorlayoutGlobal,
        //                ::pointer<::gpu_opengl::descriptor_set_layout> &descriptorPool, size_t frameCount);

      void on_prepare(::gpu::context *pgpucontext) override;

      //void initialize_GltfRenderSystem(VkRenderPass renderPass,
      //   ::gpu_opengl::descriptor_set_layout *psetdescriptorlayoutGlobal,
      //   ::graphics3d::asset_manager * passetmanager);
      //void initialize_GltfRenderSystem(

      //                                 ::gpu_opengl::descriptor_set_layout *psetdescriptorlayoutGlobal,
      //                                 ::graphics3d::asset_manager *passetmanager);

      ///void initialize_GltfRenderSystem(

         //;
      ///;::gpu_opengl::descriptor_set_layout *psetdescriptorlayoutGlobal);

      //void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
      //void createPipeline(VkRenderPass renderPass);

      
      void on_render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscene) override;


   };


} // namespace graphics3d_opengl
 



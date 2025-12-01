#pragma once
//#include "interfaces/asset_provider_i.h"
//#include "interfaces/game_object_i.h"
//#include "interfaces/model_i.h"
//#include "interfaces/render_system_i.h"
//
//#include "vulkan_wrapper/vulkan_descriptor.h"
//#include "vulkan_wrapper/vulkan_device.h"
//#include "vulkan_wrapper/vulkan_pipeline.h"
//
//#include "vulkan_wrapper/render_systems/gltf_render_system.h"
//#include "vulkan_wrapper/vulkan_gltf.h"
//#include "vulkan_wrapper/vulkan_renderer.h"

#include "app-graphics3d/graphics3d_opengl/render_system.h"
#include "bred/graphics3d/render_systems/scene_render_system.h"

//
//// STD
//#include <memory>
//#include <vector>
//
//#include <vulkan/vulkan.h>

namespace graphics3d_opengl
{


   class CLASS_DECL_GRAPHICS3D_OPENGL scene_render_system : virtual public ::graphics3d::scene_render_system,
                               virtual public ::graphics3d_opengl::render_system
   {
   public:
      const int TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP = 10;
      const int TEXTURE_UNIT_PREFILTERED_ENV_MAP = 11;
      const int TEXTURE_UNIT_BRDF_CONVOLUTION_MAP = 12;

      enum BloomDirection {
         BOTH = 0,
         HORIZONTAL = 1,
         VERTICAL = 2
     };


      
      //VkSandboxDevice &m_device;

      //VkDescriptorSetLayout m_globalSetLayout;
      /*VkDescriptorSetLayout m_iblSetLayout;
      VkDescriptorSet m_iblDescriptorSet;*/

      ::pointer<::gpu::shader> m_ppipelineOpaque;
      ::pointer<::gpu::shader> m_ppipelineMask;
      ::pointer<::gpu::shader> m_ppipelineBlend;
      //VkPipelineLayout m_pipelineLayout;

//      IAssetProvider &m_assets;

      // std::unique_ptr<VkSandboxDescriptorSetLayout> m_iblLayout;
      // std::vector<VkDescriptorSet>				  m_iblDescriptorSets;

      // std::unique_ptr<VkSandboxDescriptorSetLayout> m_pbrLayout;
      // std::vector<VkDescriptorSet>				  m_pbrDescriptorSets;


      //scene_render_system(VkSandboxDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout,
        //                IAssetProvider &assets);
      scene_render_system();
      ~scene_render_system();

      //scene_render_system(const scene_render_system &) = delete;
      //scene_render_system &operator=(const scene_render_system &) = delete;

      //void init(VkSandboxDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout,
      //          VkSandboxDescriptorPool &descriptorPool, size_t frameCount) override;

      //void render(FrameInfo &frame) override;

      void on_prepare(::gpu::context *pgpucontext) override;
   //private:
      //void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
      //void createPipeline(VkRenderPass renderPass);

      
      void on_render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscene) override;


   };


} // namespace graphics3d_opengl

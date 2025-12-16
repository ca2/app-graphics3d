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

#include "app-graphics3d/graphics3d_directx11/render_system.h"
#include "app-graphics3d/graphics3d/render_system/scene_render_system.h"

//
//// STD
//#include <memory>
//#include <vector>
//
//#include <vulkan/vulkan.h>

namespace graphics3d_directx11
{


   class scene_render_system : virtual public ::graphics3d::scene_render_system,
                               virtual public ::graphics3d_directx11::render_system
   {
   public:

      
      //VkSandboxDevice &m_device;

      //aaaVkDescriptorSetLayout m_globalSetLayout;
      /*aaaVkDescriptorSetLayout m_iblSetLayout;
      VkDescriptorSet m_iblDescriptorSet;*/

      ::pointer<::gpu_directx11::pipeline> m_ppipelineOpaque;
      ::pointer<::gpu_directx11::pipeline> m_ppipelineMask;
      ::pointer<::gpu_directx11::pipeline> m_ppipelineBlend;
      //VkPipelineLayout m_pipelineLayout;

//      IAssetProvider &m_assets;

      // std::unique_ptr<VkSandboxDescriptorSetLayout> m_iblLayout;
      // std::vector<VkDescriptorSet>				  m_iblDescriptorSets;

      // std::unique_ptr<VkSandboxDescriptorSetLayout> m_pbrLayout;
      // std::vector<VkDescriptorSet>				  m_pbrDescriptorSets;


      //scene_render_system(VkSandboxDevice &device, VkRenderPass renderPass, aaaVkDescriptorSetLayout globalSetLayout,
        //                IAssetProvider &assets);
      scene_render_system();
      ~scene_render_system();

      //scene_render_system(const scene_render_system &) = delete;
      //scene_render_system &operator=(const scene_render_system &) = delete;

      //void init(VkSandboxDevice &device, VkRenderPass renderPass, aaaVkDescriptorSetLayout globalSetLayout,
      //          VkSandboxDescriptorPool &descriptorPool, size_t frameCount) override;

      //void render(FrameInfo &frame) override;

      void on_prepare(::gpu::context *pgpucontext) override;
   //private:
      //void createPipelineLayout(aaaVkDescriptorSetLayout globalSetLayout);
      //void createPipeline(VkRenderPass renderPass);

      
      void on_render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscene) override;


   };


} // namespace graphics3d_directx11

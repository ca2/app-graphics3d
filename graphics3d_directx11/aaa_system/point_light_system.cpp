#include "framework.h"
#include "point_light_system.h"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <map>
#include <stdexcept>


namespace graphics3d_directx11
{


   point_light_system::point_light_system(
      ::gpu::context * pgpucontext, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
      : m_pgpucontext{ pgpucontext }
   {

      createPipelineLayout(globalSetLayout);
      createPipeline(renderPass);

   }


   point_light_system::~point_light_system()
   {

      vkDestroyPipelineLayout(m_pgpucontext->logicalDevice(), pipelineLayout, nullptr);

   }


   void point_light_system::createPipelineLayout() {

      
      VkDescriptorSetLayout globalSetLayout
      m_pshader = m_pengine->create_shader()
      {

      }

      VkPushConstantRange pushConstantRange{};
      pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
      pushConstantRange.offset = 0;
      pushConstantRange.size = sizeof(PointLightPushConstants);

      ::array<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

      VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
      pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
      pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
      pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
      pipelineLayoutInfo.pushConstantRangeCount = 1;
      pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
      //pipelineLayoutInfo.pPushConstantRanges = nullptr;
      if (vkCreatePipelineLayout(m_pgpucontext->logicalDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
         VK_SUCCESS) {
         throw ::exception(error_failed, "failed to create pipeline layout!");
      }
   }

   void point_light_system::createPipeline(VkRenderPass renderPass) {
      assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

      PipelineConfigInfo pipelineConfig{};
      pipeline::defaultPipelineConfigInfo(pipelineConfig);
      pipelineConfig.attributeDescriptions.clear();
      pipelineConfig.bindingDescriptions.clear();
      pipelineConfig.renderPass = renderPass;
      pipelineConfig.pipelineLayout = pipelineLayout;

      // Construct paths using PROJECT_ROOT_DIR
      std::string vertShaderPath = "matter://Shaders/SpirV/point_light.vert.spv";
      std::string fragShaderPath = "matter://Shaders/SpirV/point_light.frag.spv";

      m_ppipeline = __allocate pipeline();

      m_ppipeline->initialize_pipeline(
         m_pgpucontext,
         vertShaderPath.c_str(),
         fragShaderPath.c_str(),
         pipelineConfig
      );
   }
   void point_light_system::render(FrameInfo& frameInfo) {
      // sort lights
      std::map<float, ::collection::index> sorted;
      for (auto& kv : frameInfo.gameObjects) {
         auto& obj = kv.element2();

         ::cast < ::graphics3d::point_light > ppointlight = obj;
         if (ppointlight == nullptr) continue;

         // calculate distance
         auto offset = frameInfo.camera.GetPosition() - obj->m_transform.translation;
         float disSquared = glm::dot(offset, offset);
         sorted[disSquared] = obj->m_iId;
      }

      m_ppipeline->bind(frameInfo.commandBuffer);

      vkCmdBindDescriptorSets(
         frameInfo.commandBuffer,
         VK_PIPELINE_BIND_POINT_GRAPHICS,
         pipelineLayout,
         0,
         1,
         &frameInfo.globalDescriptorSet,
         0,
         nullptr);

      // iterate through sorted lights in reverse order
      for (auto it = sorted.rbegin(); it != sorted.rend(); ++it) {
         // use game obj id to find light object
         auto& obj = frameInfo.gameObjects[it->second];
         ::cast < ::graphics3d::point_light > ppointlight = obj;
         PointLightPushConstants push{};
         push.position = glm::vec4(obj->m_transform.translation, 1.f);
         push.color = glm::vec4(obj->m_color, ppointlight->m_pointlightcomponent.lightIntensity);
         push.radius = obj->m_transform.scale.x;

         vkCmdPushConstants(
            frameInfo.commandBuffer,
            pipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(PointLightPushConstants),
            &push);
         vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
      }

   }

   void point_light_system::update(FrameInfo& frameInfo, GlobalUbo& ubo)
   {

      auto rotateLight = glm::rotate(glm::mat4(1.f), 0.5f * frameInfo.frameTime, { 0.f, -1.f, 0.f });

      int lightIndex = 0;

      for (auto& kv : frameInfo.gameObjects)
      {

         auto& obj = kv.element2();

         ::cast < ::graphics3d::point_light > ppointlight = obj;

         if (ppointlight == nullptr) continue;

         assert(lightIndex < MAX_LIGHTS && "Point lights exceed maximum specified");

         // update light position
         ppointlight->m_transform.translation =
            glm::vec3(rotateLight * glm::vec4(ppointlight->m_transform.translation, 1.f));

         // copy light to ubo
         ubo.pointLights[lightIndex].position = 
            glm::vec4(ppointlight->m_transform.translation, 1.f);
         ubo.pointLights[lightIndex].color = 
            glm::vec4(obj->m_color, ppointlight->m_pointlightcomponent.lightIntensity);

         lightIndex += 1;
      }
      ubo.numLights = lightIndex;

   }

} // namespace graphics3d_directx11



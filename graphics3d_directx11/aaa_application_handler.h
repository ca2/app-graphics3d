#pragma once


#include "cube/application.h"


#include <vector>
#include <memory>
#include <deque>
#include <functional>

//#include "vma/vk_mem_alloc.h"
#include "context.h"
//#include "directx11-cube/graphics3d_directx11/base_application_no_swap_chain.h"
#include "descriptors.h"
#include "application_object.h"
#include "cube/container.h"
#include "renderer.h"


namespace graphics3d_directx11 
{


   //class DirectX11Example20 : public base_application_no_swap_chain
   //{
   //public:
   //   bool debugDisplay = false;

   //   struct {
   //      vkglTF::Model example;
   //      vkglTF::Model plane;
   //   } models;

   //   struct {
   //      ::graphics3d_directx11::Buffer vsShared;
   //      ::graphics3d_directx11::Buffer vsMirror;
   //      ::graphics3d_directx11::Buffer vsOffScreen;
   //   } uniformBuffers;

   //   struct UniformData {
   //      glm::mat4 projection;
   //      glm::mat4 view;
   //      glm::mat4 model;
   //      glm::vec4 lightPos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
   //   } uniformData;

   //   struct {
   //      VkPipeline debug{ VK_NULL_HANDLE };
   //      VkPipeline shaded{ VK_NULL_HANDLE };
   //      VkPipeline shadedOffscreen{ VK_NULL_HANDLE };
   //      VkPipeline mirror{ VK_NULL_HANDLE };
   //   } pipelines;

   //   struct {
   //      VkPipelineLayout textured{ VK_NULL_HANDLE };
   //      VkPipelineLayout shaded{ VK_NULL_HANDLE };
   //   } pipelineLayouts;

   //   struct {
   //      VkDescriptorSet offscreen{ VK_NULL_HANDLE };
   //      VkDescriptorSet mirror{ VK_NULL_HANDLE };
   //      VkDescriptorSet model{ VK_NULL_HANDLE };
   //   } descriptorSets;

   //   struct {
   //      VkDescriptorSetLayout textured{ VK_NULL_HANDLE };
   //      VkDescriptorSetLayout shaded{ VK_NULL_HANDLE };
   //   } descriptorSetLayouts;

   //   // Framebuffer for offscreen rendering
   //   struct FrameBufferAttachment {
   //      VkImage image;
   //      VkDeviceMemory mem;
   //      VkImageView view;
   //   };
   //   struct OffscreenPass {
   //      int32_t width, height;
   //      VkFramebuffer frameBuffer;
   //      FrameBufferAttachment color, depth;
   //      VkRenderPass renderPass;
   //      VkSampler sampler;
   //      VkDescriptorImageInfo descriptor;
   //   } offscreenPass{};

   //   glm::vec3 modelPosition = glm::vec3(0.0f, -1.0f, 0.0f);
   //   glm::vec3 modelRotation = glm::vec3(0.0f);

   //   DirectX11Example20();
   //   ~DirectX11Example20();

   //   // Setup the offscreen framebuffer for rendering the mirrored scene
   //   // The color attachment of this framebuffer will then be used to sample from in the fragment shader of the final pass
   //   void prepareOffscreen();

   //   void buildCommandBuffers();

   //   void loadAssets();

   //   void setupDescriptors();

   //   void preparePipelines();
   //   // Prepare and initialize uniform buffer containing shader uniforms
   //   void prepareUniformBuffers();

   //   void updateUniformBuffers();
   //   void updateUniformBufferOffscreen();

   //   void prepare();

   //   void submitWork(VkCommandBuffer cmdBuffer, VkQueue queue);

   //   void draw(const ::function < void(void*, int, int, int)>& callback);

   //   virtual void render(const ::function < void(void*, int, int, int)>& callback);

   //   virtual void OnUpdateUIOverlay(::graphics3d_directx11::UIOverlay* overlay);

   //   void sample(const ::function < void(void*, int, int, int)>& callback);

   //   void render_loop(const ::function < void(void*, int, int, int)>& callback);

   //};

	class CLASS_DECL_GRAPHICS3D_DIRECTX11 application_handler :
      public ::cube::application
   {
	public:
		//static constexpr int WIDTH = 800;
		//static constexpr int HEIGHT = 600;

            // Private Members
      //VkWindow _window{ WIDTH, HEIGHT, "DirectX11 window" };
      ::pointer < ::cube::container>     m_pcontainer;
      ::pointer < ::cube::application>     m_p3dapplication;


		application_handler();
		~application_handler();

		//application(const Application&) = delete;
		//Application& operator=(const Application&) = delete;

      void initialize_application(::cube::container * pcontainer) override;

      void run_application() override;

      void resize(int cx, int cy) override;

		void load_game_objects() override;


	};


} // namespace graphics3d_directx11




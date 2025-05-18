// Created by camilo on 2025-05-17 04:14 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "application.h"
#include "engine.h"
#include "input.h"
#include "scene.h"
#include "types.h"
#include "app-cube/cube/impact.h"
#include "acme/exception/interface_only.h"
#include "acme/platform/application.h"
#include "apex/database/client.h"
#include "apex/database/stream.h"


namespace graphics3d
{


	engine::engine()
	{


	} 

	engine::~engine()
	{


	}


   void engine::on_render_frame()
   {


   }


   void engine::on_update_frame(TransformComponent & transform, float frameTime)
   {

      m_pinput->handleMouseInput();

      m_pinput->updateLook(m_pinput->getX(), m_pinput->getY(), transform);

      m_pinput->updateMovement(frameTime, transform);

      //cameraController.moveInPlaneXZ(m_pimpact, frameTime, viewerObject);

      m_pcamera->setViewYXZ(transform.translation, transform.rotation);

   }


	void engine::run()
	{

      on_start_engine();

      //auto papp = get_app();

      //__øconstruct(m_pcontext);

      //m_pcontext->initialize_context(papp->m_pimpact);

      //__construct_new(m_prenderer);

      //m_prenderer->initialize_renderer(papp->m_pimpact, m_pcontext);

      //auto pglobalpoolbuilder = __allocate descriptor_pool::Builder();

      //pglobalpoolbuilder->initialize_builder(m_pcontext);
      //pglobalpoolbuilder->setMaxSets(render_pass::MAX_FRAMES_IN_FLIGHT);
      //pglobalpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, render_pass::MAX_FRAMES_IN_FLIGHT);

      //m_pglobalpool = pglobalpoolbuilder->build();

      ////m_pglobalpool->initialize_pool(pcontext);

      ////= __allocate
      ////   descriptor_pool::Builder(pcontext)
      ////   .setMaxSets(swap_chain_render_pass::MAX_FRAMES_IN_FLIGHT)
      ////   .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, swap_chain_render_pass::MAX_FRAMES_IN_FLIGHT)
      ////   .build();
      //m_pscene->on_load_scene();

      ////pcontext = __allocate context(m_pvulkandevice);

      //::pointer_array<buffer> uboBuffers;

      //uboBuffers.set_size(render_pass::MAX_FRAMES_IN_FLIGHT);

      //::cast < context > pcontext = m_pcontext;

      //for (int i = 0; i < uboBuffers.size(); i++)
      //{

      //   uboBuffers[i] = __allocate buffer();

      //   uboBuffers[i]->initialize_buffer(
      //      pcontext,
      //      sizeof(GlobalUbo),
      //      1,
      //      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      //      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

      //   uboBuffers[i]->map();

      //}
      //auto globalSetLayout = set_descriptor_layout::Builder(pcontext)
      //   .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
      //   .build();


      //std::vector<VkDescriptorSet> globalDescriptorSets(render_pass::MAX_FRAMES_IN_FLIGHT);

      //for (int i = 0; i < globalDescriptorSets.size(); i++)
      //{

      //   auto bufferInfo = uboBuffers[i]->descriptorInfo();

      //   descriptor_writer(*globalSetLayout, *m_pglobalpool)
      //      .writeBuffer(0, &bufferInfo)
      //      .build(globalDescriptorSets[i]);

      //}

      //SimpleRenderSystem simpleRenderSystem{
      //    pcontext,
      //    m_prenderer->getRenderPass(),
      //    globalSetLayout->getDescriptorSetLayout() };

      //point_light_system pointLightSystem{
      //    pcontext,
      //    m_prenderer->getRenderPass(),
      //    globalSetLayout->getDescriptorSetLayout()
      //};

      //camera camera{ glm::vec3(0.0f, 2.0f, -15.0f), -90.0f, 0.0f };
      //{ glm::vec3(0.0f, 2.0f, -15.0f), -90.0f, 0.0f };
      m_pcamera = m_pscene->get_default_camera();

      //VkcCamera camera(glm::vec3(0.0f, 2.0f, -10.0f), .0f, 0.0f);

      //auto viewerObject = __øcreate <::graphics3d::scene_object>();
      //papp->m_pimpact->m_bLastMouse = true;
      //viewerObject->m_transform.translation.z = -2.5f;

      TransformComponent transform;
      
      m_pinput = __allocate ::graphics3d::input();
      m_pinput->m_pimpact = m_pimpact;
      m_pinput->m_pkeymap = m_pimpact->m_pkeymap;
      /*    glfwSetInputMode(_window.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
          glfwSetWindowUserPointer(_window.getGLFWwindow(), &cameraController);*/
      m_pinput->m_bMouseAbsolute;

      ::pointer <::database::client> pdatabaseclient = m_papplication;

      if (pdatabaseclient)
      {

         pdatabaseclient->datastream()->get_block("camera", m_pcamera->as_block());
         pdatabaseclient->datastream()->get_block("transform", as_memory_block(transform));
         pdatabaseclient->datastream()->get_block("input", m_pinput->as_block());

      }

      auto pimpact = papp->m_pimpact;

      auto currentTime = std::chrono::high_resolution_clock::now();
      //while (!_window.shouldClose())
      while (!pimpact->m_bShouldClose && task_get_run())
      {

         task_iteration();
         //glfwPollEvents();

         auto newTime = std::chrono::high_resolution_clock::now();

         float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();

         currentTime = newTime;

         on_update_camera(transform, frameTime);


         if (m_prenderer->m_pvkcrenderpass->width() > 0
            && m_prenderer->m_pvkcrenderpass->height() > 0)
         {

            float aspect = m_prenderer->getAspectRatio();

            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

            on_render_frame();

         }

      }

      if (pdatabaseclient)
      {

         pdatabaseclient->datastream()->set("input", m_pinput->as_block());
         pdatabaseclient->datastream()->set("transform", as_memory_block(transform));
         pdatabaseclient->datastream()->set("camera", m_pcamera->as_block());

      }

      //if (pcontext->logicalDevice() != VK_NULL_HANDLE)
      //{

      //   vkDeviceWaitIdle(pcontext->logicalDevice());

      //}

	}


   void engine::on_start_engine()
   {


   }


	void engine::on_layout(int cx, int cy)
	{



	}


	void engine::on_mouse_move(int x, int y)
	{


	}


	::pointer<model> engine::create_model_from_file(const ::file::path& path)
	{

		model::Builder builder{};

		builder.loadModel(m_pcontext, path);

		auto pmodel = __øcreate < model>();

		pmodel->initialize_model(m_pcontext, builder);

		return pmodel;

	}




	void engine::add_scene(::graphics3d::scene* pscene)
	{

		m_mapScene[pscene->m_strName] = pscene;

	}


	void engine::set_current_scene(::graphics3d::scene* pscene)
	{

		m_pscene = pscene;

	}



} // namespace graphics3d



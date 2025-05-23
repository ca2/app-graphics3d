// Created by camilo on 2025-05-17 04:14 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "application.h"
#include "engine.h"
#include "input.h"
#include "renderer.h"
#include "scene.h"
#include "shader.h"
#include "types.h"
//#include "system/point_light_system.h"
//#include "system/simple_render_system.h"
#include "app-cube/cube/impact.h"
#include "app-cube/cube/gpu/approach.h"
#include "app-cube/cube/gpu/context.h"
#include "app-cube/cube/gpu/renderer.h"
#include "acme/exception/interface_only.h"
#include "acme/platform/application.h"
#include "apex/database/client.h"
#include "apex/database/stream.h"
#include <chrono>

namespace graphics3d
{


	engine::engine()
	{

	} 


	engine::~engine()
	{


	}


   void engine::initialize_engine(::cube::impact* pimpact)
   {

      m_pimpact = pimpact;

      m_pimpact->m_pengine = this;

      m_papproach = m_papplication->get_gpu();

      m_pinput = __allocate::graphics3d::input();
      m_pinput->m_pimpact = m_pimpact;
      m_pinput->m_pengine = this;
      m_pinput->m_pkeymap = m_pimpact->m_pkeymap;

   }


   void engine::on_render_frame()
   {

      auto prenderer = m_pgpucontext->m_prenderer;

      if (prenderer->rectangle().area() <= 0)
      {

         return;

      }

      if (auto pframe = prenderer->beginFrame())
      {

         on_begin_frame();
         // render
         prenderer->on_begin_render(pframe);

         if (m_pimpact->global_ubo_block().size() > 0)
         {

            update_global_ubo(m_pgpucontext);

         }

         m_pscene->on_render(m_pgpucontext);

         prenderer->on_end_render(pframe);

         prenderer->endFrame();

      }


   }


   void engine::create_global_ubo(::gpu::context* pgpucontext)
   {

   }


   //::file::path engine::shader_path(const ::file::path& pathShader)
   //{

   //   return pathShader;

   //}


   void engine::on_begin_frame()
   {


   }


   void engine::on_update_frame()
   {

      //m_fFrameTime = fFrameTime;

      m_pinput->prepare_mouse_input();

      m_pinput->process_mouse_input();

      //m_pinput->updateLook(m_pinput->getX(), m_pinput->getY(), transform);

      m_pinput->process_keyboard_input();

      //cameraController.moveInPlaneXZ(m_pimpact, frameTime, viewerObject);

      m_pcamera->setViewYXZ(m_transform.translation, m_transform.rotation);


      float aspect = m_pimpact->getAspectRatio();

      m_pcamera->setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);



   }


	void engine::run()
	{

      



      //auto papp = get_app();

      //__øconstruct(m_pgpucontext);

      //m_pgpucontext->initialize_context(papp->m_pimpact);

      //__construct_new(m_prenderer);

      //m_prenderer->initialize_renderer(papp->m_pimpact, m_pgpucontext);

      //auto pglobalpoolbuilder = __allocate descriptor_pool::Builder();

      //pglobalpoolbuilder->initialize_builder(m_pgpucontext);
      //pglobalpoolbuilder->setMaxSets(render_pass::MAX_FRAMES_IN_FLIGHT);
      //pglobalpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, render_pass::MAX_FRAMES_IN_FLIGHT);

      //m_pglobalpool = pglobalpoolbuilder->build();

      ////m_pglobalpool->initialize_pool(pgpucontext);

      ////= __allocate
      ////   descriptor_pool::Builder(pgpucontext)
      ////   .setMaxSets(swap_chain_render_pass::MAX_FRAMES_IN_FLIGHT)
      ////   .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, swap_chain_render_pass::MAX_FRAMES_IN_FLIGHT)
      ////   .build();
      //m_pscene->on_load_scene();

      ////pgpucontext = __allocate context(m_pvulkandevice);

      //::pointer_array<buffer> uboBuffers;

      //uboBuffers.set_size(render_pass::MAX_FRAMES_IN_FLIGHT);

      //::cast < context > pgpucontext = m_pgpucontext;

      //for (int i = 0; i < uboBuffers.size(); i++)
      //{

      //   uboBuffers[i] = __allocate buffer();

      //   uboBuffers[i]->initialize_buffer(
      //      pgpucontext,
      //      sizeof(GlobalUbo),
      //      1,
      //      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      //      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

      //   uboBuffers[i]->map();

      //}
      //auto globalSetLayout = set_descriptor_layout::Builder(pgpucontext)
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

      //simple_render_system simpleRenderSystem{
      //    pgpucontext,
      //    m_prenderer->getRenderPass(),
      //    globalSetLayout->getDescriptorSetLayout() };

      //point_light_system pointLightSystem{
      //    pgpucontext,
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
      m_transform.translation.z = -2.5f;
      //TransformComponent transform;
      
      /*    glfwSetInputMode(_window.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
          glfwSetWindowUserPointer(_window.getGLFWwindow(), &cameraController);*/
      //m_pinput->m_bMouseAbsolute;

      ::pointer <::database::client> pdatabaseclient = m_papplication;

      if (pdatabaseclient)
      {

         pdatabaseclient->datastream()->get_block("camera", m_pcamera->as_block());
         pdatabaseclient->datastream()->get_block("transform", as_memory_block(m_transform));
         pdatabaseclient->datastream()->get_block("input", m_pinput->as_block());

      }

      auto pimpact = m_pimpact;

      auto currentTime = std::chrono::high_resolution_clock::now();
      //while (!_window.shouldClose())
      while (!pimpact->m_bShouldClose && task_get_run())
      {

         task_iteration();
         //glfwPollEvents();

         auto newTime = std::chrono::high_resolution_clock::now();

         float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();

         currentTime = newTime;

         m_fFrameTime = frameTime;

         on_update_frame();

         on_render_frame();

      }

      if (pdatabaseclient)
      {

         pdatabaseclient->datastream()->set("input", m_pinput->as_block());
         pdatabaseclient->datastream()->set("transform", as_memory_block(m_transform));
         pdatabaseclient->datastream()->set("camera", m_pcamera->as_block());

      }

      //if (pgpucontext->logicalDevice() != VK_NULL_HANDLE)
      //{

      //   vkDeviceWaitIdle(pgpucontext->logicalDevice());

      //}

	}


   void engine::defer_start(const ::int_rectangle& rectanglePlacement)
   {

      auto papp = get_app();

      __øconstruct(m_pgpucontext);

      ::gpu::enum_output eoutput;

      if (m_papplication->m_bUseDraw2dProtoWindow)
      {

         eoutput = ::gpu::e_output_none;

      }
      else
      {

         eoutput = ::gpu::e_output_cpu_buffer;

         m_pgpucontext->m_callbackOffscreen = m_pimpact->m_callbackOffscreen;

      }

      m_pgpucontext->branch_synchronously();

      m_pgpucontext->_post([this, rectanglePlacement, eoutput]()
         {

            m_pgpucontext->initialize_gpu_context(m_papproach, eoutput);

            //            run_vulkan_example();


            m_pimpact->on_load_engine();

            start_engine(rectanglePlacement);

            run();

            //m_pimpact->m_ptaskEngine.release();

         });

   }


   void engine::update_global_ubo(::gpu::context* pgpucontext)
   {


   }


   void engine::start_engine(const ::int_rectangle& rectanglePlacement)
   {


   }


	void engine::on_layout(int cx, int cy)
	{

   }


	void engine::on_mouse_move(int x, int y)
	{


	}


	::pointer<model> engine::create_tinyobjloader_model(const ::file::path& path)
	{

		model::tinyobjloader_Builder builder{};

		builder.loadModel(m_pgpucontext, path);

		auto pmodel = __øcreate < model>();

		pmodel->initialize_model(m_pgpucontext, builder);

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



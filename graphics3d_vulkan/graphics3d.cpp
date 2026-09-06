// From graphics3d by camilo on 2026-09-04 22:37 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#include "platform.h"
//#include "buffer.h"
#include "graphics3d.h"
//#include "frame.h"
//#include "input.h"
//#include "offscreen_render_pass.h"
//#include "renderer.h"
//#include "swap_chain_render_pass.h"
#include "aura/platform/application.h"
#include "bred/user/user/graphics3d.h"
#include "app-graphics3d/gpu_vulkan/approach.h"
#include "app-graphics3d/gpu_vulkan/context.h"
#include "app-graphics3d/gpu_vulkan/descriptors.h"
#include "app-graphics3d/gpu_vulkan/renderer.h"
#include "bred/graphics3d/camera.h"
#include "bred/graphics3d/scene_base.h"
//#include "bred/graphics3d/system/simple_render_system.h"
//#include "bred/graphics3d/system/point_light_system.h"
#include "acme/platform/application.h"
#include "apex/database/client.h"
#include "apex/database/stream.h"
//#include "graphics3d/container.h"
#include <chrono>


namespace graphics3d_vulkan
{



   graphics3d::graphics3d()
   {

      //m_fYScale = -1.f;
      //m_fInputPitchFlip = -1.0f;

   }


   graphics3d::~graphics3d()
   {


   }


   void graphics3d::on_initialize_particle()
   {

      ::graphics3d::graphics3d::on_initialize_particle();

   }


   void graphics3d::initialize_graphics3d()
   {

      ::graphics3d::graphics3d::initialize_graphics3d();

   }


   //void graphics3d::defer_update_engine(const ::i32_rectangle& rectanglePlacement)
   //{

   //   auto pcontext = gpu_context();

   //   ::cast < ::gpu_vulkan::renderer> prenderer = pcontext->get_gpu_renderer();

   //   prenderer->defer_update_renderer();

   //   ::graphics3d::graphics3d::defer_update_engine(rectanglePlacement);

   //   //      construct_newø(m_prenderer);
   //   //
   //   //      //::graphics3d::graphics3d::m_prenderer = m_prenderer;
   //   //
   //   //      m_prenderer->initialize_renderer(m_pgpucontextCompositor);
   //   //
   //   //
   //   //      m_prenderer->set_placement(rectanglePlacement);
   //   //      //m_pglobalpool->initialize_pool(pgpucontext);
   //   //
   //   //      //= allocateø
   //   //      //   descriptor_pool::Builder(pgpucontext)
   //   //      //   .setMaxSets(swap_chain_render_pass::MAX_FRAMES_IN_FLIGHT)
   //   //      //   .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, swap_chain_render_pass::MAX_FRAMES_IN_FLIGHT)
   //   //      //   .build();
   //   //
   //   //      //pgpucontext = allocateø context(m_pvulkandevice);
   //   //      int iGlobalUboSize = m_pimpact->global_ubo_block().size();
   //   //
   //   //      if (iGlobalUboSize > 0)
   //   //      {
   //   //
   //   //         create_global_ubo(m_pgpucontextCompositor);
   //   //
   //   //      }
   //   //
   //   //
   //   ////          m_prenderer->getRenderPass(),
   //   //  //        globalSetLayout->getDescriptorSetLayout()
   //   //    //  };
   //   //
   //   //      m_pscene->on_load_scene(m_pgpucontextCompositor);


   //}


   //void graphics3d::on_begin_frame()
   //{

   //   //int frameIndex = m_prenderer->getFrameIndex();

   //   //FrameInfo frameInfo{ frameIndex, dt(), pcommandbuffer->m_vkcommandbuffer,
   //   //   *m_pcamera, m_globalDescriptorSets[frameIndex],
   //   //   m_pscene->m_mapObjects };

   //   // update
   //   //::graphics3d::GlobalUbo ubo{};
   //   //ubo.projection = m_pcamera->getProjection();
   //   //ubo.view = m_pcamera->getView();
   //   //ubo.inverseView = m_pcamera->getInverseView();
   //   //m_ppointlightsystem->update(m_pscene, ubo);
   //   //m_uboBuffers[frameIndex]->writeToBuffer(&ubo);
   //   //m_uboBuffers[frameIndex]->flush();



   //}


   ////void graphics3d::create_global_ubo(::gpu::context * pgpucontext)
   ////{

   ////   int iGlobalUboSize = m_pimpact->global_ubo_block().size();

   ////   if (iGlobalUboSize > 0)
   ////   {

   ////      m_papproach->create_global_ubo(pgpucontext,  iGlobalUboSize, ::gpu_vulkan::render_pass::MAX_FRAMES_IN_FLIGHT);

   ////   }

   ////}


   ////void graphics3d::update_global_ubo(::gpu::context* pgpucontext)
   ////{

   ////   if (m_pimpact->global_ubo_block().size() > 0)
   ////   {

   ////      m_pscene->on_update_global_ubo(pgpucontext);

   ////      m_papproach->update_global_ubo(pgpucontext, m_pimpact->global_ubo_block());

   ////   }

   ////}


   ////void graphics3d::on_render_frame()
   ////{

   ////   ::graphics3d::graphics3d::on_render_frame();

   ////}

   //void graphics3d::on_render_layer()
   //{

   //   ::graphics3d::graphics3d::on_render_layer();

   //}

   //////   ::cast < renderer > prenderer = m_prenderer;

   //////   if (prenderer->m_pvkcrenderpass->width() <= 0
   //////      || prenderer->m_pvkcrenderpass->height() <= 0)
   //////   {

   //////      return;

   //////   }


   //////   if (auto pcommandbuffer = m_prenderer->beginFrame())
   //////   {

   //////      on_begin_frame();
   //////      // render
   //////      m_prenderer->beginRenderPass(pcommandbuffer->m_vkcommandbuffer);

   //////      m_psimplerendersystem->renderGameObjects(m_pscene);
   //////      m_ppointlightsystem->render(m_pscene);

   //////      m_prenderer->endRenderPass(pcommandbuffer->m_vkcommandbuffer);
   //////      m_prenderer->endFrame();

   //////   }


   //////}


   ////::file::path graphics3d::_translate_shader_path(const ::file::path& pathShader)
   ////{

   ////   auto pathFolder = pathShader.folder();

   ////   return pathFolder / "vulkan/SpirV" / (pathShader.name() + ".spv");

   ////}


   //void graphics3d::do_draw_layer()
   //{

   //   ::graphics3d::graphics3d::do_draw_layer();

   //   //if (m_rectanglePlacementNew.is_empty())
   //   //{

   //   //   return;

   //   //}

   //   //::pointer < ::gpu_vulkan::context > pcontextUpper = pcontext;

   //   //m_pgpucontextCompositor->set_placement(m_rectanglePlacementNew);

   //   //::gpu::rear_guard rear_guard(pcontextUpper);

   //   //m_pgpucontextCompositor->send([this]()
   //   //   {

   //   //      ::gpu::context_guard guard(m_pgpucontextCompositor);

   //   //      m_pgpucontextCompositor->make_current();

   //   //      ::cast < ::gpu_vulkan::renderer > prenderer = m_pgpucontextCompositor->get_renderer(::gpu::e_scene_3d);

   //   //      prenderer->defer_update_renderer();

   //   //      try
   //   //      {

   //   //         m_pgpucontextCompositor->m_pgraphics3dengineinstance->_do_frame_step();

   //   //      }
   //   //      catch (...)
   //   //      {

   //   //      }

   //   //   });

   //   //if (1)
   //   //{

   //   //   if (pgpucontextUpper)
   //   //   {

   //   //      pgpucontextUpper->make_current();

   //   //      auto prenderer = m_pgpucontextCompositor->m_pgpurenderer;

   //   //      ::cast < ::gpu_vulkan::renderer > pgpurendererUpper = pgpucontextUpper->m_pgpurenderer;

   //   //      pgpurendererUpper->blend(prenderer);

   //   //      auto rectangleUpper = pgpucontextUpper->rectangle();

   //   //      VkViewport vp = {
   //   //         (float)rectangleUpper.left,
   //   //         (float)rectangleUpper.top,
   //   //         (float)rectangleUpper.width(),
   //   //         (float)rectangleUpper.height(),
   //   //         0.0f, 1.0f };

   //   //      VkRect2D sc = {
   //   //         {
   //   //            rectangleUpper.left,
   //   //            rectangleUpper.top,
   //   //         },
   //   //         {
   //   //            rectangleUpper.width(),
   //   //            rectangleUpper.height(),
   //   //         }
   //   //      };

   //   //      auto pcommandbuffer = pgpurendererUpper->getCurrentCommandBuffer();

   //   //      vkCmdSetViewport(pcommandbuffer->m_vkcommandbuffer, 0, 1, &vp);

   //   //      vkCmdSetScissor(pcommandbuffer->m_vkcommandbuffer, 0, 1, &sc);

   //   //   }

   //   //}

   //}


   //void graphics3d::_engine_on_frame_context_initialization()
   //{

   //   ::cast < ::gpu_vulkan::approach> papproach = m_papplication->get_gpu_approach();

   //   auto pcontext = gpu_context();

   //   papproach->engine_on_frame_context_initialization(pcontext);

   //   //m_psetdescriptorlayoutGlobal = descriptor_set_layout::Builder(pgpucontext)
   //   //   .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
   //   //   .build();



   //}


   //void graphics3d::_prepare_frame()
   //{

   //   ::graphics3d::graphics3d::_prepare_frame();

   //}


   //void graphics3d::run()
   //{

   //   ::graphics3d::graphics3d::run();

   //   //auto papp = get_app();

   //   //constructø(m_pgpucontextCompositor);

   //   //m_pgpucontextCompositor->initialize_context(papp->m_pimpact);

   //   //construct_newø(m_prenderer);

   //   //m_prenderer->initialize_renderer(papp->m_pimpact, m_pgpucontextCompositor);

   //   //auto pglobalpoolbuilder = allocateø descriptor_pool::Builder();

   //   //pglobalpoolbuilder->initialize_builder(m_pgpucontextCompositor);
   //   //pglobalpoolbuilder->setMaxSets(render_pass::MAX_FRAMES_IN_FLIGHT);
   //   //pglobalpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, render_pass::MAX_FRAMES_IN_FLIGHT);

   //   //m_pglobalpool = pglobalpoolbuilder->build();

   //   ////m_pglobalpool->initialize_pool(pgpucontext);

   //   ////= allocateø
   //   ////   descriptor_pool::Builder(pgpucontext)
   //   ////   .setMaxSets(swap_chain_render_pass::MAX_FRAMES_IN_FLIGHT)
   //   ////   .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, swap_chain_render_pass::MAX_FRAMES_IN_FLIGHT)
   //   ////   .build();
   //   //m_pscene->on_load_scene();

   //   ////pgpucontext = allocateø context(m_pvulkandevice);

   //   //::pointer_array<buffer> uboBuffers;

   //   //uboBuffers.set_size(render_pass::MAX_FRAMES_IN_FLIGHT);

   //   //::cast < context > pgpucontext = m_pgpucontextCompositor;

   //   //for (int i = 0; i < uboBuffers.size(); i++)
   //   //{

   //   //   uboBuffers[i] = allocateø buffer();

   //   //   uboBuffers[i]->initialize_buffer(
   //   //      pgpucontext,
   //   //      sizeof(GlobalUbo),
   //   //      1,
   //   //      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
   //   //      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

   //   //   uboBuffers[i]->map();

   //   //}
   //   //auto globalSetLayout = descriptor_set_layout::Builder(pgpucontext)
   //   //   .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
   //   //   .build();


   //   //::array<VkDescriptorSet> globalDescriptorSets(render_pass::MAX_FRAMES_IN_FLIGHT);

   //   //for (int i = 0; i < globalDescriptorSets.size(); i++)
   //   //{

   //   //   auto bufferInfo = uboBuffers[i]->descriptorInfo();

   //   //   descriptor_writer(*globalSetLayout, *m_pglobalpool)
   //   //      .writeBuffer(0, &bufferInfo)
   //   //      .build(globalDescriptorSets[i]);

   //   //}

   //   //SimpleRenderSystem simpleRenderSystem{
   //   //    pgpucontext,
   //   //    m_prenderer->getRenderPass(),
   //   //    globalSetLayout->getDescriptorSetLayout() };

   //   //point_light_system pointLightSystem{
   //   //    pgpucontext,
   //   //    m_prenderer->getRenderPass(),
   //   //    globalSetLayout->getDescriptorSetLayout()
   //   //};

   //   ////camera camera{ floating_sequence3(0.0f, 2.0f, -15.0f), -90.0f, 0.0f };
   //   ////{ floating_sequence3(0.0f, 2.0f, -15.0f), -90.0f, 0.0f };
   //   //auto camera = m_pscene->get_default_camera();

   //   ////VkcCamera camera(floating_sequence3(0.0f, 2.0f, -10.0f), .0f, 0.0f);

   //   //auto viewerObject = createø <::graphics3d::scene_object>();
   //   //papp->m_pimpact->m_bLastMouse = true;
   //   //viewerObject->m_transform.translation.z = -2.5f;
   //   //::graphics3d::input input;

   //   //input.m_pimpact = papp->m_pimpact;
   //   //input.m_pkeymap = papp->m_pimpact->m_pkeymap;
   //   ///*    glfwSetInputMode(_window.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   //   //    glfwSetWindowUserPointer(_window.getGLFWwindow(), &cameraController);*/
   //   //input.m_bMouseAbsolute;

   //   //::pointer <::database::client> pdatabaseclient = m_papplication;

   //   //if (pdatabaseclient)
   //   //{

   //   //   pdatabaseclient->datastream()->get_block("camera", as_memory_block(camera));
   //   //   pdatabaseclient->datastream()->get_block("transform", as_memory_block(viewerObject->m_transform));
   //   //   pdatabaseclient->datastream()->get_block("input", input.as_block());

   //   //}

   //   //auto pimpact = papp->m_pimpact;

   //   //auto currentTime = std::chrono::high_resolution_clock::now();
   //   ////while (!_window.shouldClose())
   //   //while (!pimpact->m_bShouldClose && task_get_run())
   //   //{

   //   //   task_iteration();
   //   //   //glfwPollEvents();

   //   //   auto newTime = std::chrono::high_resolution_clock::now();

   //   //   float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();

   //   //   currentTime = newTime;

   //   //   input.handleMouseInput();

   //   //   input.updateLook(input.getX(), input.getY(), viewerObject);

   //   //   input.updateMovement(frameTime, viewerObject);

   //   //   //cameraController.moveInPlaneXZ(m_pimpact, frameTime, viewerObject);

   //   //   camera.setViewYXZ(viewerObject->m_transform.translation, viewerObject->m_transform.rotation);

   //   //   if (m_prenderer->m_pvkcrenderpass->width() > 0
   //   //      && m_prenderer->m_pvkcrenderpass->height() > 0)
   //   //   {

   //   //      float aspect = m_prenderer->getAspectRatio();

   //   //      camera.setPerspectiveProjection(::radians(50.f), aspect, 0.1f, 100.f);

   //   //      if (auto pcommandbuffer = m_prenderer->beginFrame())
   //   //      {

   //   //         int frameIndex = m_prenderer->getFrameIndex();

   //   //         FrameInfo frameInfo{ frameIndex, frameTime, pcommandbuffer->m_vkcommandbuffer, camera, globalDescriptorSets[frameIndex], m_pscene->m_mapObjects };

   //   //         // update
   //   //         GlobalUbo ubo{};
   //   //         ubo.projection = camera.getProjection();
   //   //         ubo.view = camera.getView();
   //   //         ubo.inverseView = camera.getInverseView();
   //   //         pointLightSystem.update(frameInfo, ubo);
   //   //         uboBuffers[frameIndex]->writeToBuffer(&ubo);
   //   //         uboBuffers[frameIndex]->flush();

   //   //         // render
   //   //         m_prenderer->beginRenderPass(pcommandbuffer->m_vkcommandbuffer);

   //   //         simpleRenderSystem.renderGameObjects(frameInfo);
   //   //         pointLightSystem.render(frameInfo);

   //   //         m_prenderer->endRenderPass(pcommandbuffer->m_vkcommandbuffer);
   //   //         m_prenderer->endFrame();

   //   //      }

   //   //   }

   //   //}

   //   //if (pdatabaseclient)
   //   //{

   //   //   pdatabaseclient->datastream()->set("input", input.as_block());
   //   //   pdatabaseclient->datastream()->set("transform", as_memory_block(viewerObject->m_transform));
   //   //   pdatabaseclient->datastream()->set("camera", as_memory_block(camera));

   //   //}

   //   //if (pgpucontext->logicalDevice() != VK_NULL_HANDLE)
   //   //{

   //   //   vkDeviceWaitIdle(pgpucontext->logicalDevice());

   //   //}



   //}


   //void graphics3d::calculate_impact(::floating_matrix4 &matrixImpact, const ::graphics3d::camera &camera)
   //{

   //   ::graphics3d::graphics3d::calculate_impact(matrixImpact, camera);

   //   ///matrixImpact[2][2] *= -1.f;

   //}

   //floating_matrix4 graphics3d::perspective(const f32_angle &angleFovY, float aspect, float zNear, float zFar)
   //{

   // //float f = 1.0f / tanf(angleFovY.radians() * 0.5f);

   // //  floating_matrix4 M(0.0f);

   // //  M[0][0] = f / aspect; // X
   // //  M[1][1] = f; // Y

   // //  M[2][2] = zFar / (zFar - zNear);
   // //  M[2][3] = 1.0f;

   // //  M[3][2] = -(zFar * zNear) / (zFar - zNear);
   // //  M[3][3] = 0.0f;


   // //  M[1][1] *= -1.f; // Vulkan Y flip in the projection matrix

   //       float f = 1.0f / tanf(angleFovY.radians() * 0.5f);

   // floating_matrix4 M(0.0f);

   // M[0][0] = f / aspect;
   // M[1][1] = f;
   // M[2][2] = zFar / (zNear - zFar);
   // M[2][3] = -1.0f;
   // M[3][2] = (zNear * zFar) / (zNear - zFar);

   // // Vulkan requires Y-flip because its framebuffer space has Y down.
   //  M[1][1] *= -1.0f;
   //   return M;

   //}


} // graphics3d_vulkan



// From application_object by camilo on 2025-05-17 01:10 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "buffer.h"
#include "engine.h"
#include "frame.h"
#include "input.h"
#include "offscreen_render_pass.h"
//#include "renderer.h"
#include "swap_chain_render_pass.h"
#include "aura/platform/application.h"
#include "aura/user/user/graphics3d.h"
#include "gpu_directx11/approach.h"
#include "gpu_directx11/context.h"
#include "gpu_directx11/descriptors.h"
#include "gpu_directx11/renderer.h"
#include "gpu_directx11/offscreen_render_target_view.h"
#include "draw2d_direct2d/_.h"
#include "draw2d_direct2d/graphics.h"
#include "aura/graphics/graphics3d/camera.h"
#include "aura/graphics/graphics3d/scene.h"
#include "aura/windowing/window.h"
//#include "aura/graphics/graphics3d/system/simple_render_system.h"
//#include "aura/graphics/graphics3d/system/point_light_system.h"
#include "acme/platform/application.h"
#include "apex/database/client.h"
#include "apex/database/stream.h"
//#include "cube/container.h"
#include <chrono>


namespace graphics3d_directx11
{



   engine::engine()
   {
      m_fYScale = -1.0f;

   }


   engine::~engine()
   {


   }


   void engine::defer_update_engine(const ::int_rectangle& rectanglePlacement)
   {

      ::cast < ::gpu_directx11::renderer> prenderer = m_pgpucontext->m_pgpurenderer;

      prenderer->defer_update_renderer();

      ::graphics3d::engine::defer_update_engine(rectanglePlacement);

      //      __construct_new(m_prenderer);
      //
      //      //::graphics3d::engine::m_prenderer = m_prenderer;
      //
      //      m_prenderer->initialize_renderer(m_pgpucontext);
      //
      //
      //      m_prenderer->set_placement(rectanglePlacement);
      //      //m_pglobalpool->initialize_pool(pgpucontext);
      //
      //      //= __allocate
      //      //   descriptor_pool::Builder(pgpucontext)
      //      //   .setMaxSets(swap_chain_render_pass::MAX_FRAMES_IN_FLIGHT)
      //      //   .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, swap_chain_render_pass::MAX_FRAMES_IN_FLIGHT)
      //      //   .build();
      //
      //      //pgpucontext = __allocate context(m_pdirectx11device);
      //      int iGlobalUboSize = m_pimpact->global_ubo_block().size();
      //
      //      if (iGlobalUboSize > 0)
      //      {
      //
      //         create_global_ubo(m_pgpucontext);
      //
      //      }
      //
      //
      ////          m_prenderer->getRenderPass(),
      //  //        globalSetLayout->getDescriptorSetLayout()
      //    //  };
      //
      //      m_pscene->on_load_scene(m_pgpucontext);


   }


   void engine::on_begin_frame()
   {

      //int frameIndex = m_prenderer->getFrameIndex();

      //FrameInfo frameInfo{ frameIndex, dt(), commandBuffer,
      //   *m_pcamera, m_globalDescriptorSets[frameIndex],
      //   m_pscene->m_mapObjects };

      // update
      //::graphics3d::GlobalUbo ubo{};
      //ubo.projection = m_pcamera->getProjection();
      //ubo.view = m_pcamera->getView();
      //ubo.inverseView = m_pcamera->getInverseView();
      //m_ppointlightsystem->update(m_pscene, ubo);
      //m_uboBuffers[frameIndex]->writeToBuffer(&ubo);
      //m_uboBuffers[frameIndex]->flush();



   }


   //void engine::create_global_ubo(::gpu::context * pgpucontext)
   //{

   //   int iGlobalUboSize = m_pimpact->global_ubo_block().size();

   //   if (iGlobalUboSize > 0)
   //   {

   //      m_papproach->create_global_ubo(pgpucontext,  iGlobalUboSize, ::gpu_directx11::render_pass::MAX_FRAMES_IN_FLIGHT);

   //   }

   //}


   //void engine::update_global_ubo(::gpu::context* pgpucontext)
   //{

   //   if (m_pimpact->global_ubo_block().size() > 0)
   //   {

   //      m_pscene->on_update_global_ubo(pgpucontext);

   //      m_papproach->update_global_ubo(pgpucontext, m_pimpact->global_ubo_block());

   //   }

   //}


   void engine::on_render_frame()
   {

      graphics3d::engine::on_render_frame();

   }

   ////   ::cast < renderer > prenderer = m_prenderer;

   ////   if (prenderer->m_pvkcrenderpass->width() <= 0
   ////      || prenderer->m_pvkcrenderpass->height() <= 0)
   ////   {

   ////      return;

   ////   }


   ////   if (auto commandBuffer = m_prenderer->beginFrame())
   ////   {

   ////      on_begin_frame();
   ////      // render
   ////      m_prenderer->beginRenderPass(commandBuffer);

   ////      m_psimplerendersystem->renderGameObjects(m_pscene);
   ////      m_ppointlightsystem->render(m_pscene);

   ////      m_prenderer->endRenderPass(commandBuffer);
   ////      m_prenderer->endFrame();

   ////   }


   ////}


   //::file::path engine::_translate_shader_path(const ::file::path& pathShader)
   //{

   //   auto pathFolder = pathShader.folder();

   //   return pathFolder / "directx11/SpirV" / (pathShader.name() + ".spv");

   //}

   //glm::vec3 engine::camera_pole_up()
   //{

   //   return { 0.0f, -1.0f, 0.0f };

   //}


   void engine::do_frame_step(::gpu::context * pcontext)
   {

      ::graphics3d::engine::do_frame_step(pcontext);

      //if (m_rectanglePlacementNew.is_empty())
      //{

      //   return;

      //}

      //m_pgpucontext->set_placement(m_rectanglePlacementNew);

      //::gpu::rear_guard rear_guard(pcontextUpper);

      //m_pgpucontext->send([this]()
      //   {

      //      ::gpu::context_guard guard(m_pgpucontext);

      //      m_pgpucontext->make_current();

      //      ::cast < ::gpu_directx11::renderer > prenderer = m_pgpucontext->get_renderer(::gpu::e_scene_3d);

      //      prenderer->defer_update_renderer();

      //      try
      //      {

      //         m_pgpucontext->m_pengine->_do_frame_step();

      //      }
      //      catch (...)
      //      {

      //      }

      //   });


      //if (1)
      //{

      //   if (pcontextUpper)
      //   {

      //      pcontextUpper->make_current();

      //      ::cast < ::gpu_directx11::renderer > prendererUpper = pcontextUpper->m_pgpurenderer;

      //      //VkImage vkimage = prenderer->m_pvkcrenderpass->m_images[prenderer->get_frame_index()];

      //      //::int_rectangle rectangle = prenderer->m_pgpucontext->rectangle();

      //      //auto copyCmd = pgpucontext->beginSingleTimeCommands();

      //      //::directx11::insertImageMemoryBarrier(
      //      //   copyCmd,
      //      //   vkimage,
      //      //   0,
      //      //   VK_ACCESS_TRANSFER_WRITE_BIT,
      //      //   VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //      //   VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //      //   VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      //      //   VK_ACCESS_SHADER_READ_BIT,
      //      //   VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

      //      //VkSubmitInfo submitInfo{};
      //      //submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      //      //submitInfo.commandBufferCount = 1;
      //      //submitInfo.pCommandBuffers = &copyCmd;
      //      //::array<VkSemaphore> waitSemaphores;
      //      //::array<VkPipelineStageFlags> waitStages;
      //      //waitStages.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
      //      //waitSemaphores.add(prenderer->m_pvkcrenderpass->renderFinishedSemaphores[prenderer->get_frame_index()]);
      //      //submitInfo.waitSemaphoreCount = waitSemaphores.size();
      //      //submitInfo.pWaitSemaphores = waitSemaphores.data();
      //      //submitInfo.pWaitDstStageMask = waitStages.data();

      //      ////vkQueueWaitIdle(pgpucontext->graphicsQueue());

      //      //pgpucontext->endSingleTimeCommands(copyCmd);

      //      //prendererUpper->_blend_image(vkimage, pgpucontext->m_rectangle, true);

      //      //auto rectangleUpper = pcontextUpper->rectangle();

      //      //VkViewport vp = {
      //      //   (float)rectangleUpper.left(),
      //      //   (float)rectangleUpper.top(),
      //      //   (float)rectangleUpper.width(),
      //      //   (float)rectangleUpper.height(),
      //      //   0.0f, 1.0f };
      //      //VkRect2D sc = {
      //      //   {
      //      //   (float)rectangleUpper.left(),
      //      //   (float)rectangleUpper.top(),
      //      //   },
      //      //   {
      //      //            (float)rectangleUpper.width(),
      //      //   (float)rectangleUpper.height(),


      //      //}
      //      //};
      //      //vkCmdSetViewport(prendererUpper->getCurrentCommandBuffer(), 0, 1, &vp);
      //      //vkCmdSetScissor(prendererUpper->getCurrentCommandBuffer(), 0, 1, &sc);


      //   }

      //}

   }



   void engine::_engine_on_frame_context_initialization()
   {

      ::cast < ::gpu_directx11::approach> papproach = m_papplication->get_gpu();

      papproach->engine_on_frame_context_initialization(m_pgpucontext);

      //m_psetdescriptorlayoutGlobal = set_descriptor_layout::Builder(pgpucontext)
      //   .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
      //   .build();



   }


   void engine::_prepare_frame()
   {

      ::graphics3d::engine::_prepare_frame();

   }


   void engine::run()
   {

      ::graphics3d::engine::run();

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

      ////pgpucontext = __allocate context(m_pdirectx11device);

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


      //::array<VkDescriptorSet> globalDescriptorSets(render_pass::MAX_FRAMES_IN_FLIGHT);

      //for (int i = 0; i < globalDescriptorSets.size(); i++)
      //{

      //   auto bufferInfo = uboBuffers[i]->descriptorInfo();

      //   descriptor_writer(*globalSetLayout, *m_pglobalpool)
      //      .writeBuffer(0, &bufferInfo)
      //      .build(globalDescriptorSets[i]);

      //}

      //SimpleRenderSystem simpleRenderSystem{
      //    pgpucontext,
      //    m_prenderer->getRenderPass(),
      //    globalSetLayout->getDescriptorSetLayout() };

      //point_light_system pointLightSystem{
      //    pgpucontext,
      //    m_prenderer->getRenderPass(),
      //    globalSetLayout->getDescriptorSetLayout()
      //};

      ////camera camera{ glm::vec3(0.0f, 2.0f, -15.0f), -90.0f, 0.0f };
      ////{ glm::vec3(0.0f, 2.0f, -15.0f), -90.0f, 0.0f };
      //auto camera = m_pscene->get_default_camera();

      ////VkcCamera camera(glm::vec3(0.0f, 2.0f, -10.0f), .0f, 0.0f);

      //auto viewerObject = __øcreate <::graphics3d::scene_object>();
      //papp->m_pimpact->m_bLastMouse = true;
      //viewerObject->m_transform.translation.z = -2.5f;
      //::graphics3d::input input;

      //input.m_pimpact = papp->m_pimpact;
      //input.m_pkeymap = papp->m_pimpact->m_pkeymap;
      ///*    glfwSetInputMode(_window.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      //    glfwSetWindowUserPointer(_window.getGLFWwindow(), &cameraController);*/
      //input.m_bMouseAbsolute;

      //::pointer <::database::client> pdatabaseclient = m_papplication;

      //if (pdatabaseclient)
      //{

      //   pdatabaseclient->datastream()->get_block("camera", as_memory_block(camera));
      //   pdatabaseclient->datastream()->get_block("transform", as_memory_block(viewerObject->m_transform));
      //   pdatabaseclient->datastream()->get_block("input", input.as_block());

      //}

      //auto pimpact = papp->m_pimpact;

      //auto currentTime = std::chrono::high_resolution_clock::now();
      ////while (!_window.shouldClose())
      //while (!pimpact->m_bShouldClose && task_get_run())
      //{

      //   task_iteration();
      //   //glfwPollEvents();

      //   auto newTime = std::chrono::high_resolution_clock::now();

      //   float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();

      //   currentTime = newTime;

      //   input.handleMouseInput();

      //   input.updateLook(input.getX(), input.getY(), viewerObject);

      //   input.updateMovement(frameTime, viewerObject);

      //   //cameraController.moveInPlaneXZ(m_pimpact, frameTime, viewerObject);

      //   camera.setViewYXZ(viewerObject->m_transform.translation, viewerObject->m_transform.rotation);

      //   if (m_prenderer->m_pvkcrenderpass->width() > 0
      //      && m_prenderer->m_pvkcrenderpass->height() > 0)
      //   {

      //      float aspect = m_prenderer->getAspectRatio();

      //      camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

      //      if (auto commandBuffer = m_prenderer->beginFrame())
      //      {

      //         int frameIndex = m_prenderer->getFrameIndex();

      //         FrameInfo frameInfo{ frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex], m_pscene->m_mapObjects };

      //         // update
      //         GlobalUbo ubo{};
      //         ubo.projection = camera.getProjection();
      //         ubo.view = camera.getView();
      //         ubo.inverseView = camera.getInverseView();
      //         pointLightSystem.update(frameInfo, ubo);
      //         uboBuffers[frameIndex]->writeToBuffer(&ubo);
      //         uboBuffers[frameIndex]->flush();

      //         // render
      //         m_prenderer->beginRenderPass(commandBuffer);

      //         simpleRenderSystem.renderGameObjects(frameInfo);
      //         pointLightSystem.render(frameInfo);

      //         m_prenderer->endRenderPass(commandBuffer);
      //         m_prenderer->endFrame();

      //      }

      //   }

      //}

      //if (pdatabaseclient)
      //{

      //   pdatabaseclient->datastream()->set("input", input.as_block());
      //   pdatabaseclient->datastream()->set("transform", as_memory_block(viewerObject->m_transform));
      //   pdatabaseclient->datastream()->set("camera", as_memory_block(camera));

      //}

      //if (pgpucontext->logicalDevice() != VK_NULL_HANDLE)
      //{

      //   vkDeviceWaitIdle(pgpucontext->logicalDevice());

      //}



   }


   void engine::on_after_done_frame_step(::draw2d::graphics_pointer& pgraphics)
   {

      ::cast < ::draw2d_direct2d_directx11::graphics > pgraphics2d = pgraphics;

      if (pgraphics2d)
      {

         ::cast< ::gpu_directx11::context > pgpucontext = m_pgpucontext;
         ::cast< ::gpu_directx11::renderer > prenderer = pgpucontext->m_pgpurenderer;
         auto prendertargetview = prenderer->m_prendertargetview;
         ::cast < ::gpu_directx11::offscreen_render_target_view > poffscreenrendertargetview = prendertargetview;
         ::cast< ::gpu_directx11::device > pgpudevice = pgpucontext->m_pgpudevice;
         ID3D11Device* device = pgpudevice->m_pdevice;
         ID3D11DeviceContext* context = pgpucontext->m_pcontext;
         ID3D11Texture2D* offscreenTexture = poffscreenrendertargetview->m_ptextureOffscreen;
         if (!device || !context || !offscreenTexture)
         {
            throw ::exception(error_wrong_state);
         }



         //D3D11_TEXTURE2D_DESC texDesc = {};
         //texDesc.Width = width;
         //texDesc.Height = height;
         //texDesc.MipLevels = 1;
         //texDesc.ArraySize = 1;
         //texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
         //texDesc.SampleDesc.Count = 1;
         //texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
         //texDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;

         // ... Create texture using device->CreateTexture2D

         // 2. Wrap the texture in a DXGI surface
         comptr<IDXGISurface> dxgiSurface;
         offscreenTexture->QueryInterface(IID_PPV_ARGS(&dxgiSurface));

         // 3. Create the Direct2D bitmap
         D2D1_BITMAP_PROPERTIES1 bitmapProps =
            D2D1::BitmapProperties1(
               D2D1_BITMAP_OPTIONS_NONE,
               D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
            );

         comptr<ID2D1Bitmap1> bitmap;
         pgraphics2d->m_pdevicecontext->CreateBitmapFromDxgiSurface(
            dxgiSurface,
            &bitmapProps,
            &bitmap
         );

         // 4. Draw into the D2D1RenderTarget
         //d2dDeviceContext->BeginDraw();

         auto r = pgpucontext->m_rectangle;

         //pgraphics2d->m_pdevicecontext->DrawBitmap(
         //   bitmap,
         //   D2D1::RectF(r.left(), r.top(), r.width(), r.height()),
         //   1.0f, // opacity
         //   D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
         //   nullptr // source rect (optional)
         //);

         auto puserinteration = pgraphics2d->m_puserinteraction;

         


            ::int_rectangle rHost = r;
            if (puserinteration)
            {

               auto pwindow = puserinteration->window();

               if (pwindow)
               {

                  rHost = pwindow->get_window_rectangle();

               }

            }
            int iBottom= r.bottom();

            int iHostBottom = rHost.height();

            int iTop = r.top();

            int iNewTop = iHostBottom - iBottom;

         pgraphics2d->m_pdevicecontext->DrawImage(
            bitmap,
            D2D1::Point2F(0.f, 0.f),
            D2D1::RectF(r.left(), iNewTop, r.width(), r.height()),
            D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
            D2D1_COMPOSITE_MODE_SOURCE_OVER);

      }

   }

} // graphics3d_directx11



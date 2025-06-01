#include "framework.h"
#include "context.h"
#include "engine.h"
//#include "Core/Window.h"
#include "app-cube/cube/graphics3d/input.h"
#include "app-cube/cube/graphics3d/scene.h"
#include "renderer.h"
#include "mesh.h"
#include "shader.h"
#include <iostream>
#include "camera.h"
#include "app-cube/cube/gpu/approach.h"
#include "app-cube/cube/gpu/shader.h"
#include "app-cube/gpu_opengl/context.h"
#include "app-cube/gpu_opengl/device_win32.h"
#include "app-cube/gpu_opengl/frame_buffer.h"
//#include "AppCore/Application.h"
#include "system/basic_render_system.h"
#include "system/point_light_system.h"

#include "app-cube/cube/impact.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#pragma comment( lib, "glu32" )
#pragma comment( lib, "opengl32" )
//.lib; opengl32.lib;
// Acting weird flag = :   ------------------- **


namespace graphics3d_opengl
{


   engine::engine()
      : m_Running(true)
      //,m_Window("My Universe", 1100, 600),
   {

      m_bInitRenderData = true;
      m_vaoQuad = 0;
      m_vboQuad = 0;

      //m_bRunEngine = true;
   }


   void engine::on_initialize_particle()
   {

      ::graphics3d::engine::on_initialize_particle();

      //      gladLoadGL();


      //      m_pcamera->initialize(this);





            // Set the window for input management
            //input::SetGLFWWindow(m_Window.GetGLFWWindow());

            // Set up mouse callback and user pointer
            //glfwSetWindowUserPointer(m_Window.GetGLFWWindow(), this);
            //glfwSetCursorPosCallback(m_Window.GetGLFWWindow(), MouseCallback);
            //glfwSetInputMode(m_Window.GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // Hide cursor


   }

   // Destroy Game
   engine::~engine()
   {
   }


   void engine::initialize_engine(::cube::impact* pimpact)
   {

      ::graphics3d::engine::initialize_engine(pimpact);

   }


   //void engine::on_update_frame()
   //{
   //   
   //   //ProcessInput(deltaTime);

   //}

   void engine::on_render_frame()
   {

      //if (m_pcamera)
      //{

      //   m_pcamera->m_matrixProjection[1][1] *= -1.0f;

      //}

      ::graphics3d::engine::on_render_frame();


      // Clear the screen
//          m_prenderer->Clear();

          // Update and render the game (and the current scene)
      //Update(this->dt(), m_pcamera);
      //Render(m_prenderer, m_pcamera);

      // Swap buffers and poll for events
      //m_Window.SwapBuffers();
      //m_pimpact->present();
      // 
      // 

//      glDisable(GL_DEPTH_TEST);
//
//
//      //}
//
////            glDepthFunc(GL_LESS);
//
//      glPopAttrib();
//      glPopMatrix();


      //if (!m_papplication->m_bUseDraw2dProtoWindow
      //   && m_pimpact->m_callbackImage32CpuBuffer)
      //{
      //   void* p = nullptr;
      //   int w = m_pimpact->m_iWidth;
      //   int h = m_pimpact->m_iHeight;
      //   int stride = w * 4;
      //   m_memoryBuffer.set_size(stride * h);
      //   if (glReadnPixels)
      //   {
      //      glReadnPixels(
      //         0, 0,
      //         w, h,
      //         GL_BGRA,
      //         GL_UNSIGNED_BYTE,
      //         m_memoryBuffer.size(),
      //         m_memoryBuffer.data());
      //   }
      //   else
      //   {
      //      glReadPixels(
      //         0, 0,
      //         w, h,
      //         GL_BGRA,
      //         GL_UNSIGNED_BYTE,
      //         m_memoryBuffer.data());

      //   }

      //   m_pimpact->m_callbackImage32CpuBuffer(
      //      m_memoryBuffer.data(),
      //      w,
      //      h,
      //      stride);

      //}
      //else
      //{


      //   m_pimpact->m_callbackImage32CpuBuffer(
      //      nullptr,
      //      0,
      //      0,
      //      0);
      //}

   }


   void engine::defer_start(::windowing::window* pwindow, const ::int_rectangle& rectanglePlacement)
   {

      ::graphics3d::engine::defer_start(pwindow, rectanglePlacement);

      //auto papp = get_app();

      //__øconstruct(m_pgpucontext);

      ////m_pgpucontext->initialize_context(papp->m_pimpact);

      ////::cast < context > pgpucontext = m_pgpucontext;

      ////pgpucontext->m_pgpucontext->post([this]()
      ////   {

      ////      //            run_vulkan_example();


      ////   });

   }



   void engine::defer_update_engine(const ::int_rectangle& rectanglePlacement)
   {

      ::graphics3d::engine::defer_update_engine(rectanglePlacement);

      //auto papp = get_app();

      //__construct_new(m_prenderer);

      //m_prenderer->initialize_renderer(m_pgpucontext);

      ////::graphics3d::engine::m_prenderer = m_prenderer;



      ////auto pglobalpoolbuilder = __allocate descriptor_pool::Builder();

      ////pglobalpoolbuilder->initialize_builder(m_pgpucontext);
      ////pglobalpoolbuilder->setMaxSets(render_pass::MAX_FRAMES_IN_FLIGHT);
      ////pglobalpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, render_pass::MAX_FRAMES_IN_FLIGHT);

      ////m_pglobalpool = pglobalpoolbuilder->build();

      ////m_pglobalpool->initialize_pool(pgpucontext);

      ////= __allocate
      ////   descriptor_pool::Builder(pgpucontext)
      ////   .setMaxSets(swap_chain_render_pass::MAX_FRAMES_IN_FLIGHT)
      ////   .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, swap_chain_render_pass::MAX_FRAMES_IN_FLIGHT)
      ////   .build();
      //int iGlobalUboSize = m_pimpact->global_ubo_block().size();

      //if (iGlobalUboSize > 0)
      //{

      //   create_global_ubo(m_pgpucontext);

      //}

      //m_pscene->on_load_scene(m_pgpucontext);

      ////pgpucontext = __allocate context(m_pvulkandevice);

      ////m_uboBuffers.set_size(render_pass::MAX_FRAMES_IN_FLIGHT);

      //::cast < ::gpu_opengl::context > pgpucontext = m_pgpucontext;

      ////for (int i = 0; i < m_uboBuffers.size(); i++)
      ////{

      ////   m_uboBuffers[i] = __allocate buffer();

      ////   m_uboBuffers[i]->initialize_buffer(
      ////      pgpucontext,
      ////      sizeof(GlobalUbo),
      ////      1,
      ////      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      ////      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

      ////   m_uboBuffers[i]->map();

      ////}
      ////auto globalSetLayout = set_descriptor_layout::Builder(pgpucontext)
      ////   .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
      ////   .build();


      ////m_globalDescriptorSets.resize(render_pass::MAX_FRAMES_IN_FLIGHT);

      ////for (int i = 0; i < m_globalDescriptorSets.size(); i++)
      ////{

      ////   auto bufferInfo = m_uboBuffers[i]->descriptorInfo();

      ////   descriptor_writer(*globalSetLayout, *m_pglobalpool)
      ////      .writeBuffer(0, &bufferInfo)
      ////      .build(m_globalDescriptorSets[i]);

      ////}

      ////m_psimpleRenderSystem = __allocate SimpleRenderSystem{
      ////    pgpucontext };//,
      ////    //m_prenderer->getRenderPass(),
      ////    //globalSetLayout->getDescriptorSetLayout() };

      ////m_ppointLightSystem = __allocate point_light_system{
      ////    pgpucontext };
      ////,
      //  //  m_prenderer->getRenderPass(),
      //    //globalSetLayout->getDescriptorSetLayout()
      ////};

   }




   //void engine::create_global_ubo(::gpu::context* pgpucontext)
   //{

   //   ::graphics3d::engine::create_global_ubo(pgpucontext);


   //}


   //void engine::update_global_ubo(::gpu::context* pgpucontext)
   //{

   //   m_pscene->on_update_global_ubo(pgpucontext);

   //   glBindBuffer(GL_UNIFORM_BUFFER, m_globalUBO);

   //   // Map the entire buffer for writing
   //   void* ptr = glMapBufferRange(
   //      GL_UNIFORM_BUFFER,
   //      0,
   //      2 * sizeof(glm::mat4),
   //      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT
   //   );

   //   if (ptr) {
   //      // Cast to float* and copy matrices sequentially
   //      float* fptr = (float*)ptr;
   //      memcpy(fptr, m_pimpact->global_ubo_block().data(), m_pimpact->global_ubo_block().size());
   //      //memcpy(fptr + 16, glm::value_ptr(projection), sizeof(glm::mat4));
   //      glUnmapBuffer(GL_UNIFORM_BUFFER);
   //   }
   //   else {
   //      fprintf(stderr, "Failed to map UBO\n");
   //   }

   //   glBindBuffer(GL_UNIFORM_BUFFER, 0);
   //}


   void engine::on_begin_frame()
   {

      //// Must match the same binding point as in your C++ side using glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboBuffer)
      //layout(std140) uniform GlobalUbo {
      //   mat4 projection;
      //   mat4 view;
      //   mat4 invView;
      //   vec4 ambientLightColor;
      //   PointLight pointLights[10];
      //   int numLights; // Needs to be padded to 16 bytes in std140 layout
      //   // Add padding to align to 16 bytes
      //   int padding1;
      //   int padding2;
      //   int padding3;
      //};

   }


   void engine::do_frame_step()
   {

      if (m_rectanglePlacementNew.is_empty())
      {

         return;

      }

      ::pointer < ::gpu_opengl::context > pcontextUpper;

      ::pointer < ::gpu_opengl::context > pgpucontext = m_pgpucontext;

      ::cast < ::gpu_opengl::device_win32 > pgpudevice = pgpucontext->m_pgpudevice;

      if (pgpudevice
         && pgpudevice->m_pgpucontextCurrent2
         && pgpudevice->m_pgpucontextCurrent2 != m_pgpucontext)
      {

         pcontextUpper = pgpudevice->m_pgpucontextCurrent2;

      }

      m_pgpucontext->set_placement(m_rectanglePlacementNew);

      ::gpu::rear_guard rear_guard(pcontextUpper);

      ::gpu::context_guard guard(m_pgpucontext);

      _prepare_frame();

      //return;

      //// Quad for blending
      //float quad[] = {
      //    -1, -1, 0, 0,   1, -1, 1, 0,
      //    -1,  1, 0, 1,   1,  1, 1, 1
      //};

      //__defer_construct_new(m_pframebuffer);

      auto rectangle = m_rectanglePlacement;

      auto sizeHost = m_pimpact->top_level()->size();

      float wHost = sizeHost.width();
      float hHost = sizeHost.height();

      //m_pframebuffer->create(rectangle.size(), true);

      glPushMatrix();
      glPushAttrib(GL_ALL_ATTRIB_BITS);

      //m_pframebuffer->bind();

      glViewport(0, 0, rectangle.width(), rectangle.height());
      glEnable(GL_DEPTH_TEST);
      glDepthMask(GL_TRUE);
      glDepthFunc(GL_LESS);
      glClearColor(0.f, 0.f, 0.f, 0.f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      try
      {

         _do_frame_step();

      }
      catch (...)
      {


      }

      //m_pframebuffer->unbind();

      if (1)
      {

         if (pcontextUpper)
         {

            pcontextUpper->make_current();

            //return;

            glPopAttrib();
            glPopMatrix();

            glViewport(0, 0, wHost, hHost);

            glDisable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE);

            glEnable(GL_BLEND);



            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            ////glOrtho(0, size.cx() * d, size.cy() * d, 0.0f, 000.0f, 1000.0f);
            ////glOrtho(0, size.cx() * d, size.cy() * d, 0.0f, 000.0f, 1000.0f);
            //////glOrtho(0, size.cx() * d, 0.0f, size.cy() * d, 000.0f, 1000.0f);
            ////glOrtho(0, size.cx(), size.cy(), 0.0f, -1000.0f, 1000.0f);
            //glOrtho(0.f, size.cx(), 0.f, -size.cy(), -1.0f, 1.0f);
            auto bYSwap = true;
            if (bYSwap)
            {
               glOrtho(0.0f, wHost, hHost, 0, -1.0f, 1.0f);  // Flip Y
            }
            else
            {
               glOrtho(0.0f, wHost, 0, hHost, -1.0f, 1.0f);  // Flip Y
            }
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            if (!m_pshaderBlend)
            {

               __øconstruct(m_pshaderBlend);


               const char* quad_vertex_shader = "#version 330 core\n"
                  "layout(location = 0) in vec2 pos;\n"
                  "layout(location = 1) in vec2 texCoord;\n"
                  "out vec2 uv;\n"
                  "void main() {\n"
                  "    uv = texCoord;\n"
                  "    gl_Position = vec4(pos, 0.0, 1.0);\n"
                  "}";

               const char* blend_fragment_shader = "#version 330 core\n"
                  "in vec2 uv;\n"
                  "uniform sampler2D tex;\n"
                  "out vec4 FragColor;\n"
                  "void main() {\n"
                  "    FragColor = texture(tex, uv);\n"
                  "}";


               m_pshaderBlend->initialize_shader_with_block(
                  m_pgpucontext->m_pgpurenderer,
                  quad_vertex_shader,
                  blend_fragment_shader);

               glGenVertexArrays(1, &m_vaoQuad);
               glGenBuffers(1, &m_vboQuad);
               glBindVertexArray(m_vaoQuad);
               glBindBuffer(GL_ARRAY_BUFFER, m_vboQuad);
               glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
               glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
               glEnableVertexAttribArray(0);
               glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
               glEnableVertexAttribArray(1);

            }

            float WIDTH = wHost;
            float HEIGHT = hHost;

            float w = rectangle.width();
            float h = rectangle.height();
            float x = rectangle.left();
            float y = hHost - rectangle.bottom();

            // 3. Composite scene texture at 1:1 into UI FBO at position (200, 150)
            //float x = 200.0f, y = 150.0f, w = SCENE_W, h = SCENE_H;

            float l, r, b, t;

            if (1)
            {
               l = (x / WIDTH) * 2.0f - 1.0f;
               r = ((x + w) / WIDTH) * 2.0f - 1.0f;
               b = (y / HEIGHT) * 2.0f - 1.0f;
               t = ((y + h) / HEIGHT) * 2.0f - 1.0f;
            }
            else
            {
               l = rectangle.left();
               r = rectangle.right();
               b = rectangle.bottom();
               t = rectangle.top();
            }
            float quad[] = {
                   l, b,  0.0f, 0.0f,
                   r, b,  1.0f, 0.0f,
                   l, t,  0.0f, 1.0f,
                   r, t,  1.0f, 1.0f
            };

            if (1)
            {

               //glUseProgram(blendShader);
               m_pshaderBlend->bind();
               glActiveTexture(GL_TEXTURE0);
               auto texture = pgpucontext->m_pframebuffer->m_tex;
               glBindTexture(GL_TEXTURE_2D, texture);
               //glUniform1i(glGetUniformLocation(blendShader, "tex"), 0);
               ::cast < gpu_opengl::shader > pshader = m_pshaderBlend;
               pshader->_set_int("tex", 0);

               glBindBuffer(GL_ARRAY_BUFFER, m_vboQuad);
               glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quad), quad);
               glBindVertexArray(m_vaoQuad);
               glEnable(GL_BLEND);
               glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
               glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
               m_pshaderBlend->unbind();
               //}

            }

         }

      }

   }


   void engine::on_mouse_move(int x, int y)
   {


   }


//   // engine Run
//   void engine::run()
//   {
//      //return true;
//
//      float lastFrame = 0.0f;
//
//      // Main loop
//      while (m_Running && !m_pimpact->m_bShouldClose)
//      {
//
//         ::task_iteration();
//
//         //{
//
//         auto containerW = m_pimpact->m_iWidth;
//
//         auto containerH = m_pimpact->m_iHeight;
//
//         if (containerW <= 0 || containerH <= 0)
//         {
//
//            continue;
//
//         }
//
////         auto rectangle = m_pimpact->host_rectangle();
////
////         auto sizeHost = m_pimpact->top_level()->raw_rectangle().size();
////
////         auto rectangleW = rectangle.width();
////
////         auto rectangleH = rectangle.height();
////
////         glPushMatrix();
////         glPushAttrib(GL_ALL_ATTRIB_BITS);
////
////         glViewport(rectangle.left(), sizeHost.cy() - rectangleH - rectangle.top(), rectangleW, rectangleH);
////
////         glMatrixMode(GL_MODELVIEW);
////         glLoadIdentity();
////
////         //glOrtho(0.0f, rectangleW, 0, rectangleH, -1.0f, 1.0f);  // Flip Y
////
////         glEnable(GL_DEPTH_TEST);
////
////
////         glDepthFunc(GL_LESS);
////
////
////         // Frame Logic
////         float currentFrame = ::time::now().floating_second();
////         float deltaTime = currentFrame - lastFrame;
////         lastFrame = currentFrame;
////
////         deltaTime = minimum_maximum(deltaTime, 0.001, 0.016666666);
////
////         ProcessInput(deltaTime);
////
////         // Toggle wireframe mode
////         if (m_bWireframeMode) {
////            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Enable wireframe mode
////         }
////         else {
////            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Enable solid mode
////         }
////
////         // Clear the screen
//////          m_prenderer->Clear();
////
////          // Update and render the game (and the current scene)
////         Update(deltaTime, m_pcamera);
////         Render(m_prenderer, m_pcamera);
////
////         // Swap buffers and poll for events
////         //m_Window.SwapBuffers();
////         //m_pimpact->present();
////         // 
////         // 
////
////         glDisable(GL_DEPTH_TEST);
////
////         //}
////
////   //            glDepthFunc(GL_LESS);
////
////         glPopAttrib();
////         glPopMatrix();
////
////
////         if (!m_papplication->m_bUseDraw2dProtoWindow
////            && m_pimpact->m_callbackImage32CpuBuffer)
////         {
////            void* p = nullptr;
////            int w = m_pimpact->m_iWidth;
////            int h = m_pimpact->m_iHeight;
////            int stride = w * 4;
////            m_memoryBuffer.set_size(stride * h);
////            if (glReadnPixels)
////            {
////               glReadnPixels(
////                  0, 0,
////                  w, h,
////                  GL_BGRA,
////                  GL_UNSIGNED_BYTE,
////                  m_memoryBuffer.size(),
////                  m_memoryBuffer.data());
////            }
////            else
////            {
////               glReadPixels(
////                  0, 0,
////                  w, h,
////                  GL_BGRA,
////                  GL_UNSIGNED_BYTE,
////                  m_memoryBuffer.data());
////
////            }
////
////            m_pimpact->m_callbackImage32CpuBuffer(
////               m_memoryBuffer.data(),
////               w,
////               h,
////               stride);
////
////         }
////         else
////         {
////
////
////            m_pimpact->m_callbackImage32CpuBuffer(
////               nullptr,
////               0,
////               0,
////               0);
////         }
//
//         //glViewport(0, 0, m_sizeHost.cx(), m_sizeHost.cy());
//
//
//         //return true;
//
//      //}
//
//   //   //)
//
//   //   //m_Window.PollEvents();
//   //   ::task_iteration();
//      }
//
//   }
//

   //void task::run_loop()
   //{

   //   while (task_get_run())
   //   {

   //      task_run(0_s);

   //   }

   //}


   void engine::on_layout(const ::int_rectangle & rectanglePlacement)
   {

      ::graphics3d::engine::on_layout(rectanglePlacement);

      //::cast < ::gpu_opengl::context > pgpucontext = m_pgpucontext;

      //if (!pgpucontext)
      //{

      //   return;

      //}

      //pgpucontext->post([this, cx, cy]
      //   {

      //      ::cast < ::gpu_opengl::context > pgpucontext = m_pgpucontext;

      //      if (!m_prenderer)
      //      {

      //         pgpucontext->resize_offscreen_buffer({ cx, cy });

      //         m_pimpact->on_load_engine();

      //         run();

      //         //m_pimpact->m_ptaskEngine.release();

      //         return;


      //         //m_pinput = __allocate::graphics3d::input();

      //         //m_pinput->m_pimpact = m_pimpact;

      //         //m_pcamera = __allocate::graphics3d::camera(glm::vec3(0.0f, 3.0f, 3.0f), -90.0f, 0.0f);

      //         ////m_pcamera->m_pimpact

      //         ////m_pglcapplication = m_pimpact->start_opengl_application();
      //         ////__øconstruct(m_pgpucontext);

      //         //if (!m_papplication->m_bUseDraw2dProtoWindow)
      //         //{

      //         //   pgpucontext->m_pgpucontext->resize_offscreen_buffer({ cx, cy });

      //         //}

      //         //m_prenderer = __allocate::graphics3d_opengl::renderer();

      //         ////return;
      //         //// Initialize the game logic and scene data
      //         ////Init();

      //         //pgpucontext->m_pgpucontext->m_timeSample = 1_s / 60.0;

      //         //m_pgpucontext->m_rendera.add_unique(this);

      //      }

      //      pgpucontext->resize_offscreen_buffer({ cx, cy });

      //      //on_layout(cx, cy);

      //      m_pimpact->m_iWidth = cx;
      //      m_pimpact->m_iHeight = cy;

      //   });

   }


   //// Keyboard event listeners
   //void engine::ProcessInput(float deltaTime) 
   //{

   //   if (m_pinput->IsKeyPressed(::user::e_key_w)) 
   //   {

   //      m_pcamera->ProcessKeyboardInput(glc::FORWARD, deltaTime);

   //      if (m_pinput->IsKeyPressed(::user::e_key_left_shift))
   //      {

   //         m_pcamera->TeleportInDirection(glc::FORWARD);

   //      }

   //   }

   //   if (m_pinput->IsKeyPressed(::user::e_key_s)) 
   //   {

   //      m_pcamera->ProcessKeyboardInput(glc::BACKWARD, deltaTime);

   //      if (m_pinput->IsKeyPressed(::user::e_key_left_shift))
   //      {

   //         m_pcamera->TeleportInDirection(glc::BACKWARD);

   //      }

   //   }

   //   if (m_pinput->IsKeyPressed(::user::e_key_a)) 
   //   {

   //      m_pcamera->ProcessKeyboardInput(glc::LEFT, deltaTime);

   //      if (m_pinput->IsKeyPressed(::user::e_key_left_shift))
   //      {

   //         m_pcamera->TeleportInDirection(glc::LEFT);

   //      }

   //   }

   //   if (m_pinput->IsKeyPressed(::user::e_key_d))
   //   {

   //      m_pcamera->ProcessKeyboardInput(glc::RIGHT, deltaTime);

   //      if (m_pinput->IsKeyPressed(::user::e_key_left_shift))
   //      {

   //         m_pcamera->TeleportInDirection(glc::RIGHT);

   //      }
   //   }

   //   if (m_pinput->IsKeyPressed(::user::e_key_escape))
   //   {

   //      m_Running = false;

   //   }
   //   if (m_pinput->IsKeyPressed(::user::e_key_4))
   //   {
   //      m_bWireframeMode = !m_bWireframeMode;

   //   }
   //   // Check for jump (Space key)
   //   if (m_pinput->IsKeyPressed(::user::e_key_space))
   //   {
   //      float jumpHeight = 0.20f;  // Define how high the jump should be
   //      m_pcamera->Jump(jumpHeight);
   //   }
   //   // Teleport down (Left Control key)
   //   if (m_pinput->IsKeyPressed(::user::e_key_left_shift)) 
   //   {
   //      float teleportDistance = 0.1081f;  // Define the downward distance
   //      m_pcamera->TeleportDownward(teleportDistance);
   //   }

   //   // Idle movement
   //   if (!m_pinput->IsAnyKeyPressed()) { // You might want to implement IsAnyKeyPressed to check for movement keys
   //      // m_pcamera->UpdateIdleMovement(deltaTime);
   //   }


   //}


   //// Mouse event listeners
   //void engine::handleMouseMove(double dCursorX, double dCursorY)
   //{

   //   double x, y;
   //   double newx, newy;

   //   if (m_pimpact->is_absolute_mouse_position())
   //   {

   //      newx = dCursorX * 1.25 * MATH_PI;
   //      newy = dCursorY * 1.25 * MATH_PI / 2.0;

   //   }
   //   else
   //   {

   //      newx = dCursorX;
   //      newy = dCursorY;

   //   }
   //   //glfwGetCursorPos(window, &xpos, &ypos);

   //   //if (pvkcontainer->m_bFirstMouse) {
   //   //   _lastX = newx;
   //   //   _lastY = newy;
   //   //   pvkcontainer->m_bFirstMouse = false;
   //   //   xpos = _lastX;
   //   //   ypos = _lastY;
   //   //}
   //   //else
   //   if (!m_pimpact->is_absolute_mouse_position())
   //   {

   //      if (m_pimpact->m_bFirstMouse)
   //      {
   //         
   //         m_dMouseLastX = newx;
   //         m_dMouseLastY = newy;

   //         m_pimpact->m_bFirstMouse = false;

   //      }

   //   }

   //   if (m_pimpact->is_absolute_mouse_position())
   //   {

   //      x = m_dMouseLastX + (newx - m_dMouseLastX) * 0.05;
   //      y = m_dMouseLastY + (newy - m_dMouseLastY) * 0.05;
   //      m_Δx = x;
   //      m_Δy = -y;  // reversed Y

   //   }
   //   else
   //   {

   //      x = newx;
   //      y = newy;

   //      m_Δx = m_Δx + static_cast<float>(x - m_dMouseLastX - m_Δx) * 0.05;
   //      m_Δy = m_Δy + static_cast<float>(m_dMouseLastY - y - m_Δy) * 0.05;  // reversed Y


   //   }

   //   m_dMouseLastX = x;
   //   m_dMouseLastY = y;

   //   m_pcamera->ProcessMouseMovement(m_Δx, m_Δy);

   //}
   //{

   //   double xpos, ypos;
   //   double newx, newy;
   //   if (m_pimpact->is_absolute_mouse_position())
   //   {
   //      
   //      newx = dCursorX * 1.25 * MATH_PI;
   //      newy = dCursorY * 1.25 * MATH_PI / 2.0;

   //   }
   //   else
   //   {

   //      newx = dCursorX;
   //      newy = dCursorY;

   //   }
   //   //glfwGetCursorPos(window, &xpos, &ypos);

   //   //if (pvkcontainer->m_bFirstMouse) {
   //   //   _lastX = newx;
   //   //   _lastY = newy;
   //   //   pvkcontainer->m_bFirstMouse = false;
   //   //   xpos = _lastX;
   //   //   ypos = _lastY;
   //   //}
   //   //else
   //   if (!m_pimpact->is_absolute_mouse_position())
   //   {

   //      if (m_pimpact->m_bFirstMouse)
   //      {

   //         dCursorX = newx;
   //         dCursorY = newy;
   //         m_pimpact->m_bFirstMouse = false;

   //      }

   //   }
   //   {



   //   }

   //   if (m_pimpact->is_absolute_mouse_position())
   //   {

   //      xpos = m_pimpact->m_dMouseLastX + (newx - m_pimpact->m_dMouseLastX) * 0.05;
   //      ypos = m_pimpact->m_dMouseLastY + (newy - m_pimpact->m_dMouseLastY) * 0.05;
   //      m_Δx = xpos;
   //      m_Δy = -ypos;  // reversed Y
   //   }
   //   else
   //   {

   //      xpos = newx;
   //      ypos = newy;

   //      m_Δx = m_Δx + static_cast<float>(xpos - m_pimpact->m_dMouseLastX - m_Δx) * 0.05;
   //      m_Δy = m_Δy + static_cast<float>(m_pimpact->m_dMouseLastY - ypos - m_Δy) * 0.05;  // reversed Y


   //   }

   //   m_pimpact->m_dMouseLastX = xpos;
   //   m_pimpact->m_dMouseLastY = ypos;
   //   //auto x = (double)xpos;
   //   //auto y = (double)ypos;
   //   ////engine* engine = static_cast<engine*>(glfwGetWindowUserPointer(window));

   //   //if (m_pimpact->m_bFirstMouse)
   //   //{
   //   //   m_pimpact->m_dMouseLastX = x;
   //   //   m_pimpact->m_dMouseLastY = Y;
   //   //   m_bFirstMouse = false;
   //   //}

   //   //float Δx = x - m_fLastX;
   //   //float Δy = m_fLastY - y;  // Y-coordinates go from bottom to top
   //   //m_fLastX = x;
   //   //m_fLastY = y;


   //   m_pcamera->ProcessMouseMovement(m_Δx, m_Δy);

   //}





   //::pointer<::opengl::engine > start_opengl_engine(::cube::impact* pimpact, mouseState* pmousestate)
   //{

   //   auto popenglengine = pimpact->__create_new <::opengl::engine >();

   //   popenglengine->m_pimpact = pimpact;

   //   return popenglengine;

   //   }


   //void Application::Init()
   //{

   //   // Create and initialize the first scene  *** EVENTUALLY ADD ACTUAL SCENE CLASS TO MANAGE MULTIPLE UNIQUE RENDER DATA IE SCENE DATA
   //   m_prenderdataCurrentScene = __create_new<render_data>();

   //   m_bInitRenderData = true;

   //}


   //void Application::Update(float deltaTime, ::graphics3d::camera* pcamera)
   //{

   //   if (m_prenderdataCurrentScene)
   //   {

   //      glm::vec3 cameraPosition = pcamera->GetPosition(); // Get the camera's current position
   //      m_prenderdataCurrentScene->Update(deltaTime); // Pass both delta time and camera position

   //   }

   //}


   //void Application::run_application()
   //{



   //}


   void engine::Render(renderer* prenderer, ::graphics3d::camera* pcamera)
   {

      //if (m_prenderdataCurrentScene)
      //{

      //   m_prenderdataCurrentScene->Render(prenderer, pcamera);  // Call the scene's render function

      //}


   }


   //::pointer<::opengl::application > start_opengl_application(::glc::GlContainer* pvkcontainer, mouseState* pmousestate)
   //{

   //   auto pglapplication = pvkcontainer->__create_new < Application>();

   //   return pglapplication;

   //}


   //void engine::on_layout(int cx, int cy)
   //{

   //   //if (m_bInitRenderData && m_prenderdataCurrentScene)
   //   //{

   //   //   m_bInitRenderData = false;

   //   //   m_prenderdataCurrentScene->Init();  // Initialize scene-specific assets like shaders, meshes, etc.

   //   //}

   //}


} // namespace graphics3d_opengl

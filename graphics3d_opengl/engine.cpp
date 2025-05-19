#include "framework.h"
#include "context.h"
#include "engine.h"
//#include "Core/Window.h"
#include "app-cube/cube/graphics3d/input.h"
#include "renderer.h"
#include "mesh.h"
#include "shader.h"
#include <iostream>
#include "camera.h"
#include "aura/graphics/gpu/approach.h"
#include "aura/graphics/gpu/context.h"
//#include "AppCore/Application.h"
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
      //m_bRunEngine = true;
   }

   
   void engine::on_initialize_particle()
   {

      ::particle::on_initialize_particle();

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


   }


   // engine Run
   void engine::run() 
   {
       //return true;

      float lastFrame = 0.0f;

      // Main loop
      while (m_Running && !m_pimpact->m_bShouldClose)
      {

         ::task_iteration();

      //{

      auto containerW = m_pimpact->m_iWidth;

      auto containerH = m_pimpact->m_iHeight;

      if (containerW <= 0 || containerH <= 0)
      {

         continue;

      }

      auto rectangle = m_pimpact->host_rectangle();

      auto sizeHost = m_pimpact->top_level()->raw_rectangle().size();

      auto rectangleW = rectangle.width();

      auto rectangleH = rectangle.height();

      glPushMatrix();
      glPushAttrib(GL_ALL_ATTRIB_BITS);

      glViewport(rectangle.left(), sizeHost.cy() - rectangleH - rectangle.top(), rectangleW, rectangleH);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      //glOrtho(0.0f, rectangleW, 0, rectangleH, -1.0f, 1.0f);  // Flip Y

      glEnable(GL_DEPTH_TEST);


      glDepthFunc(GL_LESS);


          // Frame Logic
          float currentFrame = ::time::now().floating_second();
          float deltaTime = currentFrame - lastFrame;
          lastFrame = currentFrame;

          deltaTime = minimum_maximum(deltaTime, 0.001, 0.016666666);

          ProcessInput(deltaTime);

          // Toggle wireframe mode
          if (m_bWireframeMode) {
              glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Enable wireframe mode
          }
          else {
              glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Enable solid mode
          }

          // Clear the screen
//          m_prenderer->Clear();

          // Update and render the game (and the current scene)
          Update(deltaTime, m_pcamera);
          Render(m_prenderer, m_pcamera);

          // Swap buffers and poll for events
          //m_Window.SwapBuffers();
          //m_pimpact->present();
          // 
          // 

          glDisable(GL_DEPTH_TEST);

      //}

//            glDepthFunc(GL_LESS);

            glPopAttrib();
            glPopMatrix();


            if (!m_papplication->m_bUseDraw2dProtoWindow
                && m_pimpact->m_callbackOffscreen)
            {
               void* p = nullptr;
               int w = m_pimpact->m_iWidth;
               int h = m_pimpact->m_iHeight;
               int stride = w * 4;
               m_memoryBuffer.set_size(stride * h);
               if (glReadnPixels)
               {
                  glReadnPixels(
                     0, 0,
                     w, h,
                     GL_BGRA,
                     GL_UNSIGNED_BYTE,
                     m_memoryBuffer.size(),
                     m_memoryBuffer.data());
               }
               else
               {
                  glReadPixels(
                     0, 0,
                     w, h,
                     GL_BGRA,
                     GL_UNSIGNED_BYTE,
                     m_memoryBuffer.data());

               }

               m_pimpact->m_callbackOffscreen(
                  m_memoryBuffer.data(),
                  w,
                  h,
                  stride);

            }
            else
            {


                m_pimpact->m_callbackOffscreen(
                    nullptr,
                    0,
                    0,
                    0);
            }

            //glViewport(0, 0, m_sizeHost.cx(), m_sizeHost.cy());


            //return true;

         //}

      //   //)

      //   //m_Window.PollEvents();
      //   ::task_iteration();
      }

   }


   //void task::run_loop()
   //{

   //   while (task_get_run())
   //   {

   //      task_run(0_s);

   //   }

   //}


   void engine::on_layout(int cx, int cy)
   {

      ::cast < context > pcontext = m_pcontext;

      pcontext->m_pgpucontext->post([this, cx, cy]
         {

            ::cast < context > pcontext = m_pcontext;

            if (!m_prenderer)
            {

               m_pinput = __allocate ::graphics3d::input();

               m_pinput->m_pimpact = m_pimpact;

               m_pcamera = __allocate ::graphics3d::camera(glm::vec3(0.0f, 3.0f, 3.0f), -90.0f, 0.0f);

               //m_pcamera->m_pimpact

               //m_pglcapplication = m_pimpact->start_opengl_application();
               //__øconstruct(m_pcontext);

               if (!m_papplication->m_bUseDraw2dProtoWindow)
               {
                   
                  pcontext->m_pgpucontext->resize_offscreen_buffer({ cx, cy });

               }

               m_prenderer = __allocate ::graphics3d_opengl::renderer();

               //return;
               // Initialize the game logic and scene data
               Init();

               pcontext->m_pgpucontext->m_timeSample = 1_s/ 60.0;

               //m_pgpucontext->m_rendera.add_unique(this);

            }
            else
            {

               pcontext->m_pgpucontext->resize_offscreen_buffer({ cx, cy });

            }

            on_layout(cx, cy);

            m_pimpact->m_iWidth = cx;
            m_pimpact->m_iHeight = cy;

         });

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


   // Mouse event listeners
   void engine::handleMouseMove(double dCursorX, double dCursorY)
   {

      double x, y;
      double newx, newy;

      if (m_pimpact->is_absolute_mouse_position())
      {

         newx = dCursorX * 1.25 * MATH_PI;
         newy = dCursorY * 1.25 * MATH_PI / 2.0;

      }
      else
      {

         newx = dCursorX;
         newy = dCursorY;

      }
      //glfwGetCursorPos(window, &xpos, &ypos);

      //if (pvkcontainer->m_bFirstMouse) {
      //   _lastX = newx;
      //   _lastY = newy;
      //   pvkcontainer->m_bFirstMouse = false;
      //   xpos = _lastX;
      //   ypos = _lastY;
      //}
      //else
      if (!m_pimpact->is_absolute_mouse_position())
      {

         if (m_pimpact->m_bFirstMouse)
         {
            
            m_dMouseLastX = newx;
            m_dMouseLastY = newy;

            m_pimpact->m_bFirstMouse = false;

         }

      }

      if (m_pimpact->is_absolute_mouse_position())
      {

         x = m_dMouseLastX + (newx - m_dMouseLastX) * 0.05;
         y = m_dMouseLastY + (newy - m_dMouseLastY) * 0.05;
         m_Δx = x;
         m_Δy = -y;  // reversed Y

      }
      else
      {

         x = newx;
         y = newy;

         m_Δx = m_Δx + static_cast<float>(x - m_dMouseLastX - m_Δx) * 0.05;
         m_Δy = m_Δy + static_cast<float>(m_dMouseLastY - y - m_Δy) * 0.05;  // reversed Y


      }

      m_dMouseLastX = x;
      m_dMouseLastY = y;

      m_pcamera->ProcessMouseMovement(m_Δx, m_Δy);

   }
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





   ::pointer<::opengl::engine > start_opengl_engine(::::cube::impact* pimpact, mouseState* pmousestate)
   {

      auto popenglengine = pimpact->__create_new <::opengl::engine >();

      popenglengine->m_pimpact = pimpact;

      return popenglengine;

      }


   void Application::Init()
   {

      // Create and initialize the first scene  *** EVENTUALLY ADD ACTUAL SCENE CLASS TO MANAGE MULTIPLE UNIQUE RENDER DATA IE SCENE DATA
      m_prenderdataCurrentScene = __create_new<render_data>();

      m_bInitRenderData = true;

   }


   void Application::Update(float deltaTime, ::graphics3d::camera* pcamera)
   {

      if (m_prenderdataCurrentScene)
      {

         glm::vec3 cameraPosition = pcamera->GetPosition(); // Get the camera's current position
         m_prenderdataCurrentScene->Update(deltaTime); // Pass both delta time and camera position

      }

   }


   //void Application::run_application()
   //{



   //}


   void Application::Render(Renderer* prenderer, ::graphics3d::camera* pcamera)
   {

      if (m_prenderdataCurrentScene)
      {

         m_prenderdataCurrentScene->Render(prenderer, pcamera);  // Call the scene's render function

      }


   }


   ::pointer<::opengl::application > start_opengl_application(::glc::GlContainer* pvkcontainer, mouseState* pmousestate)
   {

      auto pglapplication = pvkcontainer->__create_new < Application>();

      return pglapplication;

   }


   void Application::resize(int cx, int cy)
   {

      if (m_bInitRenderData && m_prenderdataCurrentScene)
      {

         m_bInitRenderData = false;

         m_prenderdataCurrentScene->Init();  // Initialize scene-specific assets like shaders, meshes, etc.

      }

   }


} // namespace graphics3d_opengl

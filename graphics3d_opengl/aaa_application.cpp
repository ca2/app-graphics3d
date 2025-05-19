#include "framework.h"
#include "Application.h"
#include "Renderer/RenderData.h"
#include "Core/Camera.h"
#include  "Common/Types.h"


namespace glc
{


   Application::Application()
   {

      m_bInitRenderData = true;

   }


   Application::~Application()
   {
   

   }


   void Application::Init() 
   {

      // Create and initialize the first scene  *** EVENTUALLY ADD ACTUAL SCENE CLASS TO MANAGE MULTIPLE UNIQUE RENDER DATA IE SCENE DATA
      m_prenderdataCurrentScene = __create_new<RenderData>();

      m_bInitRenderData = true;

   }


   void Application::Update(float deltaTime, Camera *pcamera)
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


   void Application::Render(Renderer *prenderer, Camera *pcamera) 
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


} // namespace glc



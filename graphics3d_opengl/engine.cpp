#include "framework.h"
#include "context.h"
#include "engine.h"
#include "bred/graphics3d/input.h"
#include "bred/graphics3d/scene.h"
#include "mesh.h"
#include "shader.h"
#include <iostream>
#include "camera.h"
#include "bred/gpu/approach.h"
#include "bred/gpu/shader.h"
#include "aura/platform/application.h"
#include "gpu_opengl/context.h"
#include "gpu_opengl/device_win32.h"
#include "gpu_opengl/frame_buffer.h"
#include "system/basic_render_system.h"
#include "system/point_light_system.h"
#include "bred/user/user/graphics3d.h"

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


   void engine::initialize_engine(::user::graphics3d* pimpact)
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


      //if (!m_papplication->m_bUseSwapChainWindow
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


   void engine::defer_start(::user::graphics3d* pusergraphics3d, const ::int_rectangle& rectanglePlacement)
   {

      ::graphics3d::engine::defer_start(pusergraphics3d, rectanglePlacement);

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


      //if (m_rectanglePlacementNew.is_empty())
      //{

      //   return;

      //}

      //m_pgpucontext->set_placement(m_rectanglePlacementNew);

      //auto pdevice = m_pgpucontext->m_pgpudevice;

      //auto pcontextUpper = pdevice->current_context();

      //::gpu::rear_guard rear_guard(pcontextUpper);

      //::gpu::context_guard guard(m_pgpucontext);

      //_prepare_frame();

      //return;

      //// Quad for blending
      //float quad[] = {
      //    -1, -1, 0, 0,   1, -1, 1, 0,
      //    -1,  1, 0, 1,   1,  1, 1, 1
      //};

      //__defer_construct_new(m_pframebuffer);

      auto rectangle = m_rectanglePlacement;

      auto sizeHost = m_pusergraphics3d->top_level()->size();

      float wHost = (float) sizeHost.width();

      float hHost = (float) sizeHost.height();

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


   void engine::do_frame_step(::gpu::context* pcontext)
   {

      ::graphics3d::engine::do_frame_step(pcontext);

   }


   void engine::on_mouse_move(float x, float y)
   {


   }


   void engine::on_layout(const ::int_rectangle& rectanglePlacement)
   {

      ::graphics3d::engine::on_layout(rectanglePlacement);

   }

   
   void engine::Render(renderer* prenderer, ::graphics3d::camera* pcamera)
   {


   }


} // namespace graphics3d_opengl




#include "framework.h"
#include "context.h"
#include "gpu_opengl/context.h"
#include "gpu_opengl/cpu_buffer.h"
#include "frame.h"
#include "glad.h"
//#include "GLFW/glfw3.h"
#include <vector>
#include <map>
//#include "GLError.h"
#include "renderer.h"
//#include "mesh.h"
#include "app-cube/cube/impact.h"


namespace gpu_opengl
{


   renderer::renderer()
   {

      //glEnable(GL_DEPTH_TEST);


      //glDepthFunc(GL_LESS);
   }

   renderer::~renderer()
   {
   }


   //int renderer::width()
   //{

   //   //auto rectangle = m_pgpucontext->m_pimpact->host_rectangle();

   //   //auto sizeHost = m_pgpucontext->m_pimpact->top_level()->raw_rectangle().size();

   //   //auto rectangleW = rectangle.width();

   //   //return rectangleW;

   //   return m_pgpucontext->width();


   //}


   //int renderer::height()
   //{

   //   //auto rectangle = m_pgpucontext->m_pimpact->host_rectangle();

   //   //auto sizeHost = m_pgpucontext->m_pimpact->top_level()->raw_rectangle().size();

   //   //auto rectangleH = rectangle.height();

   //   //return rectangleH;

   //   return m_pgpucontext->height();

   //}


   ::pointer < ::gpu::frame > renderer::beginFrame()
   {

      //auto rectangle = m_pgpucontext->m_pimpact->host_rectangle();

      //auto sizeHost = m_pgpucontext->m_pimpact->top_level()->raw_rectangle().size();

      auto sizeHost = m_pgpucontext->size();

      auto r = this->rectangle();

      glPushMatrix();
      glPushAttrib(GL_ALL_ATTRIB_BITS);

      glViewport(r.left(), sizeHost.cy() - r.height() - r.top(), r.width(), r.height());

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      //glOrtho(0.0f, rectangleW, 0, rectangleH, -1.0f, 1.0f);  // Flip Y

      glEnable(GL_DEPTH_TEST);


      glDepthFunc(GL_LESS);


      //// Frame Logic
      //float currentFrame = ::time::now().floating_second();
      //float deltaTime = currentFrame - lastFrame;
      //lastFrame = currentFrame;

      //deltaTime = minimum_maximum(deltaTime, 0.001, 0.016666666);

      //ProcessInput(deltaTime);

      // Toggle wireframe mode
      //if (m_pgpucontext->m_pimpact->m_pengine->m_bWireframeMode) {
      //   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Enable wireframe mode
      //}
      //else {
      //   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Enable solid mode
      //}
      
      if (!m_pframe)
      {

         m_pframe = __create_new < ::gpu_opengl::frame >();

      }

      return ::gpu::renderer::beginFrame();

   }


   void renderer::on_begin_render(::gpu::frame* pframe)
   {


   }

   //void renderer::Clear() const
   //{

   //   // Clear the screen 
   //   GLCheckError();
   //   glClearColor(0.678f, 0.847f, 0.902f, 1.0f);
   //   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //   GLCheckError();
   //}

   //// sky_box
   //void renderer::DrawSkybox(const mesh* pskyboxMesh, ::gpu::shader* pshader) const
   //{
   //   GLCheckError();
   //   // Disable depth writing for the skybox
   //   glDepthFunc(GL_LEQUAL);
   //   glEnable(GL_BLEND);
   //   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   //   pshader->bind();

   //   pskyboxMesh->Bind();
   //   glDrawElements(GL_TRIANGLES, pskyboxMesh->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
   //   pskyboxMesh->Unbind();

   //   glDepthFunc(GL_LESS);
   //   pshader->unbind();
   //   GLCheckError();
   //}


   //void renderer::DrawInstanced(const ::array<mesh*>& meshes, ::gpu::shader* pshader, unsigned int instanceCount) const
   //{

   //   GLCheckError();

   //   for (const mesh* mesh : meshes) {
   //      mesh->Bind();
   //      glDrawElementsInstanced(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, (void*)(mesh->GetIndexOffset() * sizeof(unsigned int)), instanceCount);
   //      mesh->Unbind();
   //   }

   //   GLCheckError();
   //}


   //void renderer::DrawModel(const ::array<mesh*>& mehses, ::gpu::shader* pshader)
   //{

   //}


   void renderer::endFrame()
   {


      //glDisable(GL_DEPTH_TEST);


      //}

//            glDepthFunc(GL_LESS);

      glPopAttrib();
      glPopMatrix();

      //glFlush();


      if (m_papplication->m_bUseDraw2dProtoWindow)
      {

         _swap();

      }
      else
      {

         _sample();

      }

   }


   void renderer::_sample()
   {

      //      glDisable(GL_DEPTH_TEST);
      //
      //
      //      //}
      //
      ////            glDepthFunc(GL_LESS);
      //
      //      glPopAttrib();
      //      glPopMatrix();


      ::cast<context>pgpucontext = m_pgpucontext;

      if (pgpucontext)
      {

         auto pcpubuffer = pgpucontext->m_pcpubuffer;

         if (pcpubuffer)
         {

            pcpubuffer->gpu_read();

            auto callback = pgpucontext->m_callbackImage32CpuBuffer;

            auto data = pcpubuffer->m_pixmap.data();
            auto width = pcpubuffer->m_pixmap.width();
            auto height = pcpubuffer->m_pixmap.height();
            auto scan = pcpubuffer->m_pixmap.m_iScan;

            callback(data, width, height, scan);

         }

      }

      //      //m_pixmap.map();
      //
      //      auto cx = m_pixmap.m_size.cx();
      //
      //      auto cy = m_pixmap.m_size.cy();
      //
      //      //auto sizeNeeded = cx * cy * 4;
      //
      //      //m_pixmap.create(m_memory, sizeNeeded);
      //
      //      auto data = m_memory.data();
      //
      //#if defined(__APPLE__) || defined(__ANDROID__)
      //
      //      if (data != nullptr)
      //      {
      //         glReadBuffer(GL_FRONT);
      //
      //         //if(0)
      //         {
      //            glReadPixels(
      //               0, 0,
      //               cx, cy,
      //               GL_RGBA,
      //               GL_UNSIGNED_BYTE,
      //               data);
      //
      //         }
      //
      //      }
      //
      //      //m_pixmap.mult_alpha();
      //      information() << "after glReadPixels cx,cy : " << cx << ", " << cy;
      //
      //      //::memory_set(m_pixmap.m_pimage32Raw, 127, cx * cy * 4);
      //
      //#elif defined(LINUX) || defined(__BSD__)
      //
      //      glReadBuffer(GL_FRONT);
      //
      //
      //      glReadPixels(
      //         0, 0,
      //         cx, cy,
      //         GL_BGRA,
      //         GL_UNSIGNED_BYTE,
      //         m_pixmap.m_pimage32Raw);
      //
      //      //m_pixmap.mult_alpha();
      //
      //#else
      //
      //
      //      glReadBuffer(GL_FRONT);
      //
      //      //if (m_pgpucontext->is_mesa())
      //      if (!glReadnPixels)
      //      {
      //
      //         glReadPixels(
      //            0, 0,
      //            cx, cy,
      //            GL_BGRA,
      //            GL_UNSIGNED_BYTE,
      //            m_pixmap.m_pimage32Raw);
      //
      //      }
      //      else
      //      {
      //
      //         glReadnPixels(
      //            0, 0,
      //            cx, cy,
      //            GL_BGRA,
      //            GL_UNSIGNED_BYTE,
      //            cx * cy * 4,
      //            data);
      //
      //      }
      //
      //      int iError = glGetError();
      //
      //      if (iError != 0)
      //      {
      //
      //         printf("glReadnPixels error");
      //
      //      }
      //
      //      //::memory_set(m_pixmap.m_pimage32Raw, 127, cx * cy * 4);
      //
      //#endif
      //
      //      {
      //
      //         auto dst = (unsigned char*)data;
      //         auto size = cx * cy;
      //
      //         while (size > 0)
      //         {
      //            dst[0] = byte_clip(((int)dst[0] * (int)dst[3]) / 255);
      //            dst[1] = byte_clip(((int)dst[1] * (int)dst[3]) / 255);
      //            dst[2] = byte_clip(((int)dst[2] * (int)dst[3]) / 255);
      //            dst += 4;
      //            size--;
      //         }
      //
      //      }
      //
   }


   void renderer::_swap()
   {


   }


} // namespace gpu_opengl






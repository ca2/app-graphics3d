#include "framework.h"
#include "context.h"
#include "gpu_opengl/context.h"
#include "gpu_opengl/cpu_buffer.h"
#include "frame.h"
#include "frame_buffer.h"
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
      m_VAOFullScreenQuad = 0;
      m_VBOFullScreenQuad = 0;
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


   void renderer::on_context_resize()
   {

      //::gpu::renderer::set_placement(rectanglePlacement);

      ::cast < context > pgpucontext = m_pgpucontext;

      if (pgpucontext)
      {

         pgpucontext->update_framebuffer(pgpucontext->rectangle().size());

      }

   }


   ::pointer < ::gpu::frame > renderer::beginFrame()
   {

      //if(m_pgpucontext->m_eoutput == ::gpu::e_output_swap_chain)
      //{
      //   
      //   set_placement(m_pgpucontext->size());

      //}  

      //context_guard guard(this);

      m_pgpucontext->make_current();
      //glEnable(GL_DEPTH_TEST);
      //glDepthMask(GL_TRUE);
      //glDisable(GL_BLEND);
      glDepthFunc(GL_LESS);

      //glClear(GL_DEPTH_BUFFER_BIT);





      //glPushMatrix();
      //glPushAttrib(GL_ALL_ATTRIB_BITS);






      //auto rectangle = m_pgpucontext->m_pimpact->host_rectangle();

      ::cast < context > pgpucontext = m_pgpucontext;

      //auto sizeHost = m_pgpucontext->size();

      auto r = pgpucontext->rectangle();

      //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      //glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
      //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
      //glBlendFunc(GL_ZERO, GL_SRC_ALPHA);
      //glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
      // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
      //glEnable(GL_BLEND);


      int left;

      int top;

      int width;

      int height;

      if (m_pgpucontext->m_eoutput == ::gpu::e_output_cpu_buffer)
      {

         left = 0;

         top = 0;

         width = r.width();

         height = r.height();

         //glViewport(left, top, width, height);

         //m_pgpucontext->clear(::color::transparent);

      }
      else
      {

         auto sizeHost = pgpucontext->m_sizeHost;


         left = r.left();

         top = sizeHost.cy() - r.height() - r.top();
         
         width = r.width();

         height = r.height();

         //glScissor(left, top, width, height);
         //glEnable(GL_SCISSOR_TEST);
         //glViewport(left, top, width, height);

         //glOrtho(0.0f, width, height, 0, -1.0f, 1.0f);  // Flip Y

         //m_pgpucontext->clear(color::transparent);

         //glMatrixMode(GL_MODELVIEW);
         //glMatrixMode(GL_PROJECTION);
         //glLoadIdentity();

         //glOrtho(0.0f, width, 0, height, -1.0f, 1.0f);  // Flip Y

      }

      //m_pgpucontext->clear(argb(0.5f, 0.2f, 0.4f, 0.5f));

      //glScissor(left, top, width, height);
      //glEnable(GL_SCISSOR_TEST);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();



      //if (m_pgpucontext->m_eoutput == ::gpu::e_output_cpu_buffer)
      //{

      ////   glOrtho(0.0f, r.width(), 0, r.height(), -1.0f, 1.0f);  // Flip Y

      //}
      //else
      //{

       //glOrtho(0.0f, r.width()/3, r.height()/3, 0, -1.0f, 1.0f);  // Flip Y

      //}


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


   int renderer::get_frame_count() const
   {

      return 0;

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

      //glPopAttrib();
      //glPopMatrix();


      //glDisable(GL_DEPTH_TEST);
      //glDepthMask(GL_FALSE);
      //glEnable(GL_BLEND);
      //glDisable(GL_SCISSOR_TEST);
      glFlush();

      GLint drawFboId = 0, readFboId = 0;

      glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);
      glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFboId);

      int iGlError1 = glGetError();

      int iGlError2 = -1;

      if (drawFboId != 0)
      {

         glBindFramebuffer(GL_FRAMEBUFFER, 0);

         iGlError2 = glGetError();

      }


      if (m_pgpucontext->m_eoutput == ::gpu::e_output_swap_chain)
      {

         _swap();

      }
      else if (m_pgpucontext->m_eoutput == ::gpu::e_output_cpu_buffer)
      {

         _sample();

      }

      ::cast < ::gpu_opengl::context > pgpucontext = m_pgpucontext;

      pgpucontext->_release_current();

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

      m_pgpucontext->swap_buffers();

   }
   GLuint createFullscreenQuad(GLuint& quadVBO) {
      // Vertex data: (x, y, u, v)
#if 1
      float quadVertices[] = {
         //  Position   TexCoords
         -1.0f,  1.0f,  0.0f, 1.0f, // Top-left
         -1.0f, -1.0f,  0.0f, 0.0f, // Bottom-left
          1.0f, -1.0f,  1.0f, 0.0f, // Bottom-right

         -1.0f,  1.0f,  0.0f, 1.0f, // Top-left
          1.0f, -1.0f,  1.0f, 0.0f, // Bottom-right
          1.0f,  1.0f,  1.0f, 1.0f  // Top-right
      };
#else
      float quadVertices[] = {
         //  Position   TexCoords
         0.0f,  1.0f,  0.0f, 1.0f, // Top-left
         0.0f, 0.0f,  0.0f, 0.0f, // Bottom-left
          1.0f, 0.0f,  1.0f, 0.0f, // Bottom-right

         0.0f,  1.0f,  0.0f, 1.0f, // Top-left
          1.0f, 0.0f,  1.0f, 0.0f, // Bottom-right
          1.0f,  1.0f,  1.0f, 1.0f  // Top-right
      };
#endif
      GLuint quadVAO = 0;

      glGenVertexArrays(1, &quadVAO);
      glGenBuffers(1, &quadVBO);

      glBindVertexArray(quadVAO);
      glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

      // Position attribute (location = 0)
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);

      // Texture Coord attribute (location = 1)
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
      glEnableVertexAttribArray(1);

      // Unbind for safety
      glBindVertexArray(0);
      return quadVAO;

   }


   void renderer::_on_graphics_end_draw(::gpu_opengl::renderer* prendererSrc)
   {

      ::gpu_opengl::renderer* prenderer;

      prenderer = prendererSrc;

      auto rectangle = prenderer->m_pgpucontext->rectangle();

      m_pgpucontext->set_placement(rectangle);

      //VkImage image = prenderer->m_pvkcrenderpass->m_images[prenderer->get_frame_index()];

      //on_new_frame();

      //if (1)
      //{
      //   //auto cmdBuffer = m_pgpucontext->beginSingleTimeCommands();

      //   //VkImageMemoryBarrier barrier = {
      //   //    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      //   //    .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      //   //    .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
      //   //    .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //   //    .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //   //    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //   //    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //   //    .image = image,
      //   //    .subresourceRange = {
      //   //        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //   //        .baseMipLevel = 0,
      //   //        .levelCount = 1,
      //   //        .baseArrayLayer = 0,
      //   //        .layerCount = 1
      //   //    },
      //   //};

      //   //vkCmdPipelineBarrier(
      //   //   cmdBuffer,
      //   //   VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      //   //   VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      //   //   0,
      //   //   0, NULL,
      //   //   0, NULL,
      //   //   1, &barrier
      //   //);

      //   //VkSubmitInfo submitInfo = {};
      //   //submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

      //   //VkSemaphore waitSemaphores[] = { prendererSrc->m_pvkcrenderpass->renderFinishedSemaphores[prendererSrc->m_pvkcrenderpass->currentFrame] };
      //   //VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
      //   //submitInfo.waitSemaphoreCount = 1;
      //   //submitInfo.pWaitSemaphores = waitSemaphores;
      //   //submitInfo.pWaitDstStageMask = waitStages;

      //   //submitInfo.commandBufferCount = 1;
      //   //submitInfo.pCommandBuffers = &cmdBuffer;

      //   //m_pgpucontext->endSingleTimeCommands(cmdBuffer, 1, &submitInfo);
      //   auto cmdBuffer = m_pgpucontext->beginSingleTimeCommands();


      //   insertImageMemoryBarrier(cmdBuffer,
      //      image,
      //      0,
      //      VK_ACCESS_TRANSFER_WRITE_BIT,
      //      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //      VK_PIPELINE_STAGE_TRANSFER_BIT,
      //      VK_PIPELINE_STAGE_TRANSFER_BIT,
      //      VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });



      //   VkSubmitInfo submitInfo{};
      //   submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      //   submitInfo.commandBufferCount = 1;
      //   submitInfo.pCommandBuffers = &cmdBuffer;
      //   ::array<VkSemaphore> waitSemaphores;
      //   ::array<VkPipelineStageFlags> waitStages;
      //   waitStages.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
      //   waitSemaphores.add(prenderer->m_pvkcrenderpass->renderFinishedSemaphores[prenderer->get_frame_index()]);
      //   submitInfo.waitSemaphoreCount = waitSemaphores.size();
      //   submitInfo.pWaitSemaphores = waitSemaphores.data();
      //   submitInfo.pWaitDstStageMask = waitStages.data();
      //   m_pgpucontext->endSingleTimeCommands(cmdBuffer, 1, &submitInfo);

      //   //m_prendererResolve->m_pvkcrenderpass->m_semaphoreaWaitToSubmit.add(
      //   //   m_pvkcrenderpass->renderFinishedSemaphores[iPassCurrentFrame]
      //   //);


      //}

m_pgpucontext->send([this, prenderer]()
   {

      if (auto pframe = beginFrame())
      {

         //m_pvkcrenderpass->m_semaphoreaSignalOnSubmit.add(prendererSrc->m_pvkcrenderpass->imageAvailableSemaphores[prendererSrc->get_frame_index()]);


         //on_begin_frame();
         // render
         on_begin_render(pframe);

         //m_pgpucontext->clear(::argb(127, 140 / 2, 220 / 2, 240 / 2));
         m_pgpucontext->clear(::color::transparent);


         if (1)
         {


            //if (m_pimpact->global_ubo_block().size() > 0)
            //{

              // update_global_ubo(m_pgpucontext);

            //}

            //m_pscene->on_render(m_pgpucontext);

            //_blend_image(image, m_rectangle);

   //         glDisable(GL_BLEND);
   //         //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            auto pvertexshader = R"vertexshader(#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main() {
   gl_Position = vec4(aPos.xy, 0.0, 1.0);
   TexCoord = aTexCoord;
}
)vertexshader";


            auto pfragmentshader = R"fragmentshader(#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
   FragColor = texture(uTexture, TexCoord);
}
)fragmentshader";

            if (!m_pshaderBlend)
            {

               m_pshaderBlend = __create_new < ::gpu_opengl::shader >();

               m_pshaderBlend->initialize_shader_with_block(
                  this,
                  pvertexshader, pfragmentshader);


            }

            m_pshaderBlend->bind();

            if (1)
            {

               glActiveTexture(GL_TEXTURE0);

               int iGlError1 = glGetError();

               ::cast < context > pcontext = prenderer->m_pgpucontext;
               
               GLuint tex = pcontext->m_pframebuffer->m_tex;

               glBindTexture(GL_TEXTURE_2D, tex);

               int iGlError2 = glGetError();

               m_pshaderBlend->_set_int("uTexture", 0);

               if (!m_VAOFullScreenQuad)
               {

                  m_VAOFullScreenQuad = createFullscreenQuad(m_VBOFullScreenQuad);

               }

               glBindVertexArray(m_VAOFullScreenQuad);

               int iGlError00 = glGetError();

               glDrawArrays(GL_TRIANGLES, 0, 6); // assuming 2 triangles (quad)

               int iGlError01 = glGetError();

               glBindVertexArray(0);

               int iGlErrorA = glGetError();

               glBindTexture(GL_TEXTURE_2D, 0);

               int iGlErrorB = glGetError();

               debug() << "gl error";

            }

            m_pshaderBlend->unbind();

         }

         on_end_render(pframe);

         endFrame();


      }

   });


   }



} // namespace gpu_opengl






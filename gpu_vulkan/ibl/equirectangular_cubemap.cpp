// From github:/tristancalderbank/OpenGL-PBR-Renderer/equirectangular_cubemap.cpp by
// camilo on 2025-09-26 19:53 <3ThomasBorregaardSorensen!!
// From gpu_opengl/ibl/equirectangular_cubemap.h by
// camilo on 2025-10-10 22:03 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "equirectangular_cubemap.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/context.h"
#include "bred/graphics3d/skybox.h"
#include "gpu/timer.h"




#include "gpu/gltf/_constant.h"
#include "gpu_vulkan/_gpu_vulkan.h"
#include "gpu_vulkan/command_buffer.h"
#include "gpu_vulkan/context.h"
#include "gpu_vulkan/texture.h"
// #include "timer.h"
#include "cubemap_framebuffer.h"
#include "hdri_cube.h"


namespace gpu_vulkan
{


   namespace ibl
   {


      equirectangular_cubemap::equirectangular_cubemap()
      {


      }


      equirectangular_cubemap::~equirectangular_cubemap()
      {


      }


      ::block equirectangular_cubemap::embedded_ibl_hdri_cube_vert()
      {

         static unsigned int pvertexshader[] = {
#include "shader/hdricube.vert.spv.inl"
         };

         return ::as_memory_block(pvertexshader);

      }


      ::block equirectangular_cubemap::embedded_ibl_hdri_cube_frag()
      {

         static unsigned int pfragmentshader[] = 
         {
#include "shader/hdricube.frag.spv.inl"
         };

      return ::as_memory_block(pfragmentshader);

      }


      // equirectangular_cubemap::equirectangular_cubemap(const ::string &engineRoot, const ::string &hdriPath) {
      //     ::string hdriVertexShaderPath = engineRoot + "/src/ibl/shaders/hdri_cube.vert";
      //     ::string hdriFragmentShaderPath = engineRoot + "/src/ibl/shaders/hdri_cube.frag";
      //
      //     hdriShader = std::make_unique<Shader>(hdriVertexShaderPath.c_str(), hdriFragmentShaderPath.c_str());
      //     hdri_cube = std::make_unique<hdri_cube>(hdriPath);
      //     framebuffer = std::make_unique<cubemap_framebuffer>(cubemapWidth, cubemapHeight);
      // }


      void equirectangular_cubemap::compute()
      {

         ::gpu::Timer timer;

         ::pointer < ::gpu_vulkan::command_buffer > pgpucommandbuffer = m_pgpucontext->beginSingleTimeCommands(m_pgpucontext->m_pgpudevice->graphics_queue());

         floating_matrix4 model = ::gpu::gltf::mIndentity4;

         using namespace geometry3d;

         floating_matrix4 cameraAngles[] = 
         {
            ::graphics3d::lookAt(origin, unitX, -unitY),
            ::graphics3d::lookAt(origin, -unitX, -unitY),
            ::graphics3d::lookAt(origin, unitY, unitZ),  
            ::graphics3d::lookAt(origin, -unitY, -unitZ),
            ::graphics3d::lookAt(origin, unitZ, -unitY), 
            ::graphics3d::lookAt(origin, -unitZ, -unitY)
         };
         
         floating_matrix4 projection = glm::perspective(::radians(90.0f), // 90 degrees to cover one face
                                                 1.0f, // its a square
                                                 0.1f, 2.0f);

         m_pgpucontext->m_rectangle.set(0, 0, m_uCubemapWidth, m_uCubemapHeight);

         ::cast<::gpu_vulkan::context> pcontext = m_pgpucontext;
         // render the equirectangular HDR texture to a cubemap
         //m_pframebuffer->bind();
         // render to each side of the cubemap

         ::cast<::gpu_vulkan::texture> ptextureFramebuffer = m_pframebuffer->m_ptexture;

         ptextureFramebuffer->_set_state(pgpucommandbuffer, {

                                                               VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                                                               VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                                               VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
            });

         auto escene = ::gpu::e_scene_srgb;

         m_pshaderHdri->_bind(pgpucommandbuffer, escene);
         m_pshaderHdri->bind_source(pgpucommandbuffer, m_phdricube->m_ptextureHdr);

         for (auto i = 0; i < 6; i++)
         {

            pcontext->_001BeginRenderPass(pgpucommandbuffer, m_pframebuffer, i, escene);
            
            m_pshaderHdri->setModelViewProjectionMatrices(model, cameraAngles[i], projection);
            
            //m_pframebuffer->setCubeFace(i, m_pshaderHdri);

            auto pmat1 = (floating_matrix4 *)m_pshaderHdri->m_propertiesPushShared.m_blockWithoutSamplers.data();

            m_pshaderHdri->push_properties(pgpucommandbuffer);
               ///            pgpucommandbuffer->cle
            //;
            //;
            //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
   //         GLCheckError("");

                // auto pshader = pcommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext->
            //   m_pshaderBound;

            m_phdricube->draw(pgpucommandbuffer);

            pcontext->_001EndRenderPass(pgpucommandbuffer);


         }

         pcontext->endSingleTimeCommands(pgpucommandbuffer);

         m_pframebuffer->generateMipmap();

         timer.logDifference("Rendered equirectangular cubemap");

         //GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

         //if (status != GL_FRAMEBUFFER_COMPLETE)
         //{

         //   printf("Framebuffer incomplete!\n");
         //}

         //// timer.logDifference("Rendered specular brdf convolution map");

         //glBindFramebuffer(GL_FRAMEBUFFER, 0);
         //GLCheckError("");
      }


      //unsigned int equirectangular_cubemap::getCubemapId()
      //{

      //   ::cast<cubemap_framebuffer> pframebuffer = m_pframebuffer;
      //   return pframebuffer->getCubemapTextureId();
      //}


   } // namespace ibl


} // namespace gpu_vulkan

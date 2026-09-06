// From github:/tristancalderbank/OpenGL-PBR-Renderer/equirectangular_cubemap.cpp by
// camilo on 2025-09-26 19:53 <3ThomasBorregaardSorensen!!
// From gpu_opengl/ibl/equirectangular_cubemap.h by
// camilo on 2025-10-10 22:03 <3ThomasBorregaardSorensen!!
#include "platform.h"
#include "equirectangular_cubemap.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/context.h"
#include "bred/graphics3d/_functions.h"
#include "bred/graphics3d/engine_instance.h"
#include "bred/graphics3d/skybox.h"
#include "bred/platform/timer.h"




#include "gpu/model/_constant.h"
#include "gpu_vulkan/_gpu_vulkan.h"
#include "gpu_vulkan/command_buffer.h"
#include "gpu_vulkan/context.h"
#include "gpu_vulkan/texture.h"
// #include "timer.h"
//#include "cubemap_framebuffer.h"
//#include "hdri_cube.h"


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
#include "shader/equirectangular_cubemap.vert.spv.inl"

         };

         return ::as_memory_block(pvertexshader);

      }


      ::block equirectangular_cubemap::embedded_ibl_hdri_cube_frag()
      {

         static unsigned int pfragmentshader[] = 
         {
#include "shader/equirectangular_cubemap.frag.spv.inl"

         };

      return ::as_memory_block(pfragmentshader);

      }


      void equirectangular_cubemap::compute_equirectangular_cubemap(::gpu::command_buffer *pgpucommandbuffer)
      {

         ::gpu::ibl::equirectangular_cubemap::compute_equirectangular_cubemap(pgpucommandbuffer);

         //::bred::Timer timer;

         //::pointer < ::gpu_vulkan::command_buffer > pgpucommandbuffer = m_pgpucontext->beginSingleTimeCommands(m_pgpucontext->m_pgpudevice->graphics_queue());

         //using namespace graphics3d;

         //floating_matrix4 model = mIndentity4;

         //floating_matrix4 cameraAngles[] = 
         //{
         //   lookAt(origin, unitX, -unitY),
         //   lookAt(origin, -unitX, -unitY),
         //   lookAt(origin, unitY, unitZ), 
         //   lookAt(origin, -unitY, -unitZ),
         //   lookAt(origin, unitZ, -unitY), 
         //   lookAt(origin, -unitZ, -unitY)
         //};
         //
         //floating_matrix4 projection = m_pgpucontext->m_pgraphics3dengineinstance->perspective(
         //   90_degrees, // 90 degrees to cover one face
         //                                        1.0f, // its a square
         //                                        0.1f, 2.0f);

         ////projection[1][1] *= -1; // Invert Y for Vulkan  

         //m_pgpucontext->m_rectangle.set(0, 0, m_uCubemapWidth, m_uCubemapHeight);

         //::cast<::gpu_vulkan::context> pcontext = m_pgpucontext;
         //// render the equirectangular HDR texture to a cubemap
         ////m_pframebuffer->bind();
         //// render to each side of the cubemap

         //::cast<::gpu_vulkan::texture> ptextureFramebuffer = m_pframebuffer->m_ptexture;

         //ptextureFramebuffer->_set_state(pgpucommandbuffer, {

         //                                                      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
         //                                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
         //                                                      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
         //   });

         //auto escene = ::gpu::e_scene_srgb;

         //m_pshaderHdri->_bind(pgpucommandbuffer, escene);
         //m_pshaderHdri->bind_source(pgpucommandbuffer,m_ptextureHdr);

         //for (auto i = 0; i < 6; i++)
         //{

         //   pcontext->_001BeginRenderPassWithCubemap(pgpucommandbuffer, m_ptexture, i, escene);
         //   
         //   m_pshaderHdri->setModelViewProjection(model, cameraAngles[i], projection);
         //   
         //   //m_pframebuffer->setCubeFace(i, m_pshaderHdri);

         //   //m_pshaderHdri->set_sequence3("multiplier", {1.f, 1.f, 1.f});
         //   m_pshaderHdri->set_i32("faceIndex", i);

         //   m_pshaderHdri->push_properties(pgpucommandbuffer);

         //   m_prenderableCube->bind(pgpucommandbuffer);

         //   m_prenderableCube->draw(pgpucommandbuffer);

         //   m_prenderableCube->unbind(pgpucommandbuffer);

         //   pcontext->_001EndRenderPass(pgpucommandbuffer);

         //}

         //pcontext->endSingleTimeCommands(pgpucommandbuffer);

         //m_pframebuffer->generateMipmap();

         //timer.logDifference("Rendered equirectangular cubemap");

      }


   } // namespace ibl


} // namespace gpu_vulkan

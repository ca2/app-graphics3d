// From github:/tristancalderbank/OpenGL-PBR-Renderer/hdri_cube.cpp by
// camilo on 2025-09-26 19:54 <3ThomasBorregaardSorensen!!
// From gpu_opengl/ibl/hdri_cube.cpp by
// camilo on 2025-10-10 22:18 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "hdri_cube.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/context.h"
#include "bred/gpu/render_target.h"
#include "bred/gpu/renderer.h"
#include "gpu_vulkan/_gpu_vulkan.h"
#include "gpu_vulkan/cube.h"
#include "gpu_vulkan/texture.h"



 namespace gpu_vulkan
 {


    namespace ibl
    {

       hdri_cube::hdri_cube()
       {


       }


       hdri_cube::~hdri_cube()
       {


       }


       // hdri_cube::hdri_cube(const ::string &hdriPath) : hdrTexture(HDRTexture(hdriPath))
       // {
       //     cube = std::make_unique<Cube>();
       // }
       //
       // void
       // hdri_cube::Draw(Shader &shader) {
       //     shader.setInt("hdri", 0);
       //
       //     // draw mesh
       //     glBindTexture(GL_TEXTURE_2D, hdrTexture.getId());
       //     cube->Draw();
       // };
       //


       //void hdri_cube::initialize_hdri_cube(const ::scoped_string &hdriPath)
       //{

          //øconstruct_new(m_phdrtexture);

         // m_pcube = øcreate_new < ::gpu_opengl::cube > ();

       //}


       void hdri_cube::draw(::gpu::command_buffer *pcommandbuffer)
       {
          ///shader.setInt("hdri", 0);
          //auto pshader = pcommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext->
          //   m_pshaderBound;

          //pshader->set_int("hdri", 0);

          ::cast<::gpu_vulkan::texture> ptexture = m_ptextureHdr;
          // draw mesh
          //glBindTexture(GL_TEXTURE_2D, ptexture->m_gluTextureID);
          //GLCheckError("");
          //m_pcube->Draw();
          //m_pcube->bind(pcommandbuffer);
          //pcommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext->m_pshaderBound->bind_source(pcommandbuffer, ptexture);
          m_pcube->draw(pcommandbuffer);

       }

    } // namespace ibl


 } // namespace gpu_vulkan




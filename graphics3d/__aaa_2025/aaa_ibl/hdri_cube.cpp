// From github:/tristancalderbank/OpenGL-PBR-Renderer/hdri_cube.cpp by
// camilo on 2025-09-26 19:54 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "hdri_cube.h"
#include "hdr_texture.h"

#include "gpu/cube.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/context.h"
#include "bred/gpu/render_target.h"
#include "bred/gpu/renderer.h"

//
//
// namespace gpu
// {
//
//
//    namespace ibl
//    {
//
//       hdri_cube::hdri_cube()
//       {
//
//
//       }
//
//
//       hdri_cube::~hdri_cube()
//       {
//
//       }
//
//
//       void hdri_cube::initialize_hdri_cube(const ::scoped_string &hdriPath)
//       {
//
//          constructø(m_phdrtexture);
//
//          constructø(m_pcube);
//
//       }
//
//
//       void hdri_cube::draw(::gpu::command_buffer *pcommandbuffer)
//       {
//          // ///shader.setInt("hdri", 0);
//          // ::cast < ::gpu_opengl::shader > pshader = pcommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext->m_pshaderBound;
//          //
//          // pshader->set_i32("hdri", 0);
//          //
//          // ::cast < ::gpu_opengl::hdr_texture > phdrtexture = m_phdrtexture;
//          // // draw mesh
//          // glBindTexture(GL_TEXTURE_2D, phdrtexture->getgetId());
//          // //cube->Draw();
//          // //m_pcube->bind(pcommandbuffer);
//          // m_pcube->draw(pcommandbuffer);
//
//       };
//
//
//    } // namespace ibl
//
//
// } // namespace gpu

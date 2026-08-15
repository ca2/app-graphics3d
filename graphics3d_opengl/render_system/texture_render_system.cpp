// From wavefront_obj_render_system.cpp by camilo on 2026-07-25 01:23 <3ThomasBorregaardSørensen!! Mummi!! Bilbo!!
#include "platform.h"
#include "bred/gpu/renderable.h"
#include "bred/graphics3d/scene_renderable.h"
#include "shader/texture.frag.h"
#include "shader/texture.vert.h"
#include "texture_render_system.h"


namespace graphics3d_opengl
{


   texture_render_system::texture_render_system() {}


   texture_render_system::~texture_render_system() {}


   void texture_render_system::model_and_normal_matrices(::floating_matrix4 &matrixModel,
                                                               ::floating_matrix4 &matrixNormal,
                                                               ::graphics3d::scene_renderable *pscenerenderable)
   {

      auto T = ::floating_matrix4::translation(pscenerenderable->m_sequence3Translation);

      auto R = pscenerenderable->m_matrixRotation;

      auto s = pscenerenderable->m_sequence3Scaling;

      auto pszName = pscenerenderable->m_strName.c_str();

      if (pscenerenderable->m_ecoordinatesystem == ::gpu::e_coordinate_system_vulkan)
      {

         s.y = -s.y;

         // s.z = -s.z;
      }
      else
      {

         // s.x = -s.x;
         // s.y = -s.y;
         // s.z = -s.z;
      }

      // s.y = -s.y;
      // else if (pscenerenderable->m_ecoordinatesystem == ::gpu::e_coordinate_system_znf)
      //{

      //   s.y = -s.y;

      //   // s.z = -s.z;
      //}

      auto S = ::floating_matrix4::scaling(s);

      auto modelMatrix = T * R * S;

      matrixModel = modelMatrix;

      auto determinant = matrixModel.determinant();

      if (abs(determinant) <= FLT_EPSILON)
      {

         auto &pathRenderable = pscenerenderable->m_pathRenderable;

         auto pszPathRenderable = pathRenderable.c_str();

         ::string strName = pathRenderable.name();

         auto pszName = strName.c_str();

         ::string strName2 = pscenerenderable->m_strName;

         auto pszName2 = strName2.c_str();

         ::information("model ({}) matrix determinant near zero: {}", strName2, determinant);
      }
      else if (determinant < 0.f)
      {

         auto &pathRenderable = pscenerenderable->m_pathRenderable;

         auto pszPathRenderable = pathRenderable.c_str();

         ::string strName = pathRenderable.name();

         auto pszName = strName.c_str();

         ::string strName2 = pscenerenderable->m_strName;

         auto pszName2 = strName2.c_str();

         // information("");

         // ::information("model ({}) matrix determinant (< 0): {}",
         //    strName2,
         //    determinant);

         // modelMatrix = modelMatrix * ::floating_matrix4::scaling({-1.f, 1.f, 1.f});
      }

      auto n = ::floating_matrix3(matrixModel).inversed().transposed();

      // n = -1.f * n;

      matrixNormal = n;
   }


   ::memory texture_render_system::vert_shader_memory() { return g_psz_texture_vert; }


   ::memory texture_render_system::frag_shader_memory() { return g_psz_texture_frag; }


   void texture_render_system::on_before_draw_renderable(::gpu::context *pgpucontext,
                                                               ::graphics3d::scene_base *pscene,
                                                               ::graphics3d::scene_renderable *pscenerenderable)
   {

      glDisable(GL_CULL_FACE);

      // glEnable(GL_CULL_FACE);

      // auto pszName = pscenerenderable->m_strName.c_str();

      // if (pscenerenderable->m_ecoordinatesystem == ::gpu::e_coordinate_system_vulkan)
      //{

      //   glFrontFace(GL_CCW);

      //}
      // else
      //{

      //   glFrontFace(GL_CW);

      //}

      // if (pscenerenderable->m_ecoordinatesystem == ::gpu::e_coordinate_system_znf)
      //{

      //  glFrontFace(GL_CW);

      //}
      // else
      //{

      //  glFrontFace(GL_CCW);
      //
      //}


      // glFrontFace(GL_CCW);

      // glFrontFace(GL_CCW);

      // glDepthFunc(GL_LESS); // the default
      // glDepthRange(0.0, 1.0);

      // glDepthFunc(GL_GREATER);
      // glClearDepth(0.0);

      // glGetIntegerv(GL_DEPTH_BITS, &bits);
      // printf("Depth bits: %d\n", bits);
   }


} // namespace graphics3d_opengl

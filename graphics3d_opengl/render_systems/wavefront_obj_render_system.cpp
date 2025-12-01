#include "framework.h"
#include "wavefront_obj_render_system.h"
#include "bred/gpu/renderable.h"
#include "bred/graphics3d/scene_renderable.h"


namespace graphics3d_opengl
{


   wavefront_obj_render_system::wavefront_obj_render_system()
	{


	}


	wavefront_obj_render_system::~wavefront_obj_render_system()
	{

   
   }



   void wavefront_obj_render_system::model_and_normal_matrices(::floating_matrix4& matrixModel,
                                                  ::floating_matrix4 & matrixNormal, ::graphics3d::scene_renderable *pscenerenderable)
   {

      matrixModel = pscenerenderable->model_matrix();

      //if (pscenerenderable->m_ecoordinatesystem == ::gpu::e_coordinate_system_vulkan)
      //{
    
      //   s.y = -s.y;
   
      //}

      //s.z = -s.z;

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

         ::information("model ({}) matrix determinant (< 0): {}", 
            strName2,
            determinant);

         //modelMatrix = modelMatrix * ::floating_matrix4::scaling({-1.f, 1.f, 1.f});

      }

      auto n = ::floating_matrix3(matrixModel).inversed().transposed();

      n = -1.f * n;

      matrixNormal = n;

   }


    void wavefront_obj_render_system::on_before_draw_renderable(::gpu::context *pgpucontext,
                                                                     ::graphics3d::scene_base *pscene,
                                          ::graphics3d::scene_renderable *pscenerenderable)

	{

       if (pscenerenderable->m_ecoordinatesystem == ::gpu::e_coordinate_system_vulkan)
       {

       //   glFrontFace(GL_CW);

       }
       else
       {

         // glFrontFace(GL_CCW);
         
       }

       //glFrontFace(GL_CCW);

       //glDepthFunc(GL_LESS); // the default
       //glDepthRange(0.0, 1.0);

       //glDepthFunc(GL_GREATER);
       //glClearDepth(0.0);

       //glGetIntegerv(GL_DEPTH_BITS, &bits);
       //printf("Depth bits: %d\n", bits);

	}


} // namespace graphics3d_opengl





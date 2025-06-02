#pragma once


#include "application.h"
#include "acme/prototype/geometry2d/rectangle.h"
#include "acme/prototype/collection/string_map.h"
#include "apex/platform/app_consumer.h"
#include "cube/gpu/render.h"


namespace graphics3d_hello3d
{


   class CLASS_DECL_APP_COMPLEX_HELLO3D render :
      virtual public ::app_consumer < application, ::gpu::render >
   {
   public:


      ::string                              m_strRender;
      int                                    m_iRender;
      //bool                                m_bPendingShaderUpdate;
      //string                              m_strProjection;
      //string                              m_strFragment;
      //::pointer<::gpu::program>           m_pprogram;
      ::pointer<::gpu::program>           m_pprogram2;
      //string                              m_strFont1;
      //string_map < ::image::image_pointer >      m_pimagemap;
      //::image::image_pointer                     m_pimage1;
      //::image::image_pointer                     m_pimage2;
      //::color::hls                        m_hlsText;
      //class ::time                        m_timeStart;
      //::pointer<::gpu::object>            m_pobject;
      //string                              m_strMatrixUniform;
      //string                              m_strTextureUniform;
      //bool                                m_bPrepared;
      //::string                            m_strRender;
      unsigned int m_VAO;
      unsigned int m_VBO;
      unsigned int m_EBO;
      unsigned int m_texture;

      unsigned int m_VAO2;
      unsigned int m_VBO2;
      unsigned int m_EBO2;
      unsigned int m_texture2;

      render();
      ~render() override;



      ///// Initialization routines
      //virtual void set_vertex_source_code(const string & strVertexSourceCode);
      //virtual void set_fragment_source_code(const string & strFragmentSourceCode);
      //virtual void set_model_path(const ::payload & payloadFile);


      //string get_font();
      //::e_status set_font(const string& strFont);

      //virtual void set_update_shader();
      //virtual void defer_update_shader();
      //virtual ::e_status _update_shader();

      ////virtual void _001OnDraw(::draw2d::graphics_pointer & pgraphics);

      ////virtual void _001OnDraw1Through3(::draw2d::graphics_pointer & pgraphics);

      ////virtual void _001OnDrawArcs(::draw2d::graphics_pointer & pgraphics, bool bPath);

      //virtual void on_layout(::draw2d::graphics_pointer & pgraphics);

      //virtual void defer_prepare();

      //virtual void _prepare();

      void on_draw(gpu::context * pcontext) override;

      virtual void on_draw_triangle(gpu::context * pcontext);

      virtual void on_draw_triangle2(gpu::context * pcontext);

      virtual void on_draw_triangle_uniform(gpu::context * pcontext);

      virtual void on_draw_triangle_more_attributes(gpu::context * pcontext);

      virtual void on_draw_rectangle(gpu::context * pcontext);

      virtual void on_draw_rectangle_texture(gpu::context * pgpucontext);


   };


} // namespace graphics3d_hello3d




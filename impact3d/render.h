#pragma once


//#include "acme/prototype/geometry2d/_geometry2d.h"
#include "acme/prototype/geometry2d/rectangle.h"
#include "acme/prototype/collection/string_map.h"
#include "apex/platform/app_consumer.h"
#include "bred/gpu/render.h"


namespace graphics3d_impact3d
{


   class CLASS_DECL_APP_COMPLEX_IMPACT3D render :
      virtual public ::app_consumer < application, ::gpu::render >
   {
   public:


      bool                                m_bPendingShaderUpdate;
      string                              m_strProjection;
      string                              m_strFragment;
      //::pointer<::gpu::context>           m_papplication;
      //::pointer<::gpu::program>           m_pprogram;
      //int_rectangle                       m_rectangle;
      string                              m_strFont1;
      string_map < ::image::image_pointer >      m_pimagemap;
      ::image::image_pointer                     m_pimage1;
      ::image::image_pointer                     m_pimage2;
      ::color::hls                        m_hlsText;
      class ::time                          m_timeStart;
      ::pointer<::gpu::object>            m_pobject;
      string                              m_strMatrixUniform;
      string                              m_strTextureUniform;
      bool                                m_bPrepared;


      render();
      ~render() override;


#ifdef _DEBUG
      long long increment_reference_count() override;
      long long decrement_reference_count() override;
#endif


      void initialize(::particle * pparticle) override;


      /// Initialization routines
      virtual void set_vertex_source_code(const string & strVertexSourceCode);
      virtual void set_fragment_source_code(const string & strFragmentSourceCode);
      virtual void set_model_path(const ::payload & payloadFile);


      string get_font();
      ::e_status set_font(const string& strFont);

      virtual void set_update_shader();
      virtual void defer_update_shader();
      virtual ::e_status _update_shader();

      virtual void _001OnDraw(::draw2d::graphics_pointer & pgraphics);

//      virtual void _001OnDraw1Through3(::draw2d::graphics_pointer & pgraphics);

  //    virtual void _001OnDrawArcs(::draw2d::graphics_pointer & pgraphics, bool bPath);

//      virtual void draw_arc(::draw2d::graphics_pointer & pgraphics, int_rectangle & r, double dStart, double dAngle, bool bPath);

      virtual void on_layout(::draw2d::graphics_pointer & pgraphics) override;

      virtual void defer_prepare();

      virtual void _prepare();

      virtual void draw() override;


   };


} // namespace graphics3d_impact3d




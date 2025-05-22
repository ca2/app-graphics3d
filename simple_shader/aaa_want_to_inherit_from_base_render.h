#pragma once


namespace simple_shader
{


   class CLASS_DECL_APP_SIMPLE_SHADER render :
      virtual public __application_consumer
   {
   public:


      string                              m_strShaderPrefix;
      bool                                m_bUpdateShader;
      string                              m_strProjection;
      string                              m_strFragment;
      ::pointer<::gpu::context>           m_papplication;
      ::pointer<::gpu::program>           m_pprogram;
      rectangle                                m_rect;
      ::user::impact *                    m_pimpact;
      string                              m_strFont1;
      string_map < ::image::image_pointer >      m_pimagemap;
      ::image::image_pointer                     m_pimage1;
      ::image::image_pointer                     m_pimage2;
      ::color::hls                               m_hlsText;
      ::duration                              m_durationStart;
      ::gpu::enum_shader_source           m_eshadersource;

      render();
      virtual ~render();


      virtual ::e_status initialize(::particle * pparticle) override;

#ifdef _DEBUG
      virtual long long increment_reference_count() override;
      virtual long long decrement_reference_count() override;
#endif

      string get_font();
      ::e_status set_font(const string& strFont);

      virtual ::e_status update_shader();
      virtual void defer_update_shader();
      virtual ::e_status _update_shader();

      virtual void _001OnDraw(::draw2d::graphics_pointer & pgraphics);

      virtual void _001OnDraw1Through3(::draw2d::graphics_pointer & pgraphics);

      virtual void _001OnDrawArcs(::draw2d::graphics_pointer & pgraphics, bool bPath);

      virtual void draw_arc(::draw2d::graphics_pointer & pgraphics, rectangle & r, double dStart, double dAngle, bool bPath);

      virtual void on_layout(::draw2d::graphics_pointer & pgraphics);

      virtual void defer_load_fragment(const ::string & pszPath);


   };


} // namespace simple_shader



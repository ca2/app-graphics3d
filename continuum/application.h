#pragma once


#include "core/platform/application.h"


namespace app_graphics3d_continuum
{


   class CLASS_DECL_APP_GRAPHICS3D_CONTINUUM application :
      virtual public ::core::application
   {
   public:


      bool m_bAbsoluteMousePosition;


      ::pointer_array < ::user::check_box> m_checkboxaSkyBox;

      pane_impact* m_ppaneimpact;
      ::file::path                                    m_pathApplicationText;
      ::user::document* m_pdocMenu;
      //::pointer<::user::single_document_template>  m_ptemplateSimpleDrawingMain;
      //pane_impact *                                     m_ppaneimpact;
      impact *                                     m_pimpact;
      impact *                                     m_pimpactSwitcher;
      impact *                                     m_pimpactSkybox;
      main_scene *                                 m_pmainscene;
      main_scene *                                 m_pmainsceneSwitcher;
      main_scene *                                 m_pmainsceneSkybox;
      ::pointer < ::user::plain_edit >             m_peditYeeColor;



      application();
      ~application() override;


      __DECLARE_APPLICATION_RELEASE_TIME();


      virtual string preferred_experience() override;
      virtual void init_instance() override;
      virtual void term_application() override;

      virtual void on_request(::request* prequest) override;

#ifdef _DEBUG
      virtual long long increment_reference_count() override;
      virtual long long decrement_reference_count() override;
#endif
      void create_options_body(const ::atom & atom, ::user::interaction* pparent) override;


      bool is_absolute_mouse_position();
      //::string graphics3d_get_implementation_name() override;

      //::string draw2d_get_default_implementation_name() override;


   };


} // namespace app_graphics3d_continuum




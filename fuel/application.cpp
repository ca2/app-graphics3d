#include "framework.h"
#include "application.h"
#include "child_frame.h"
#include "document.h"
//#include "edit_impact.h"
#include "frame.h"
#include "impact.h"
//#include "main_impact.h"
#include "pane_impact.h"
//#include "toggle_impact.h"
//#include "top_impact.h"
//#include "window.h"
#include "acme/handler/request.h"
#include "acme/platform/system.h"
#include "base/user/user/single_document_template.h"


__IMPLEMENT_APPLICATION_RELEASE_TIME(app_graphics3d_fuel);
IMPLEMENT_APPLICATION_FACTORY(app_graphics3d_fuel);


namespace app_graphics3d_fuel
{


   // __IMPLEMENT_APPLICATION_RELEASE_TIME1(app_graphics3d_fuel));


   application::application()
   {

      m_bUseSwapChainWindow = true;
      //m_ptemplateFuelMain = nullptr;
      //m_ptemplateFuelImpact = nullptr;

      m_strAppId              = "app-graphics3d/fuel";
      m_strAppName            = "app-graphics3d/fuel";
      m_strBaseSupportId      = "app-graphics3d/fuel";
      m_bLicense              = false;

   }


   application::~application()
   {

   }


   void application::init_instance()
   {

      factory()->add_factory_item <::app_graphics3d_fuel::child_frame >();
      factory()->add_factory_item <::app_graphics3d_fuel::document >();
      factory()->add_factory_item <::app_graphics3d_fuel::frame >();
      //factory()->add_factory_item <::app_graphics3d_fuel::edit_impact >();
      factory()->add_factory_item <::app_graphics3d_fuel::impact >();
      //factory()->add_factory_item <::app_graphics3d_fuel::main_impact >();
      factory()->add_factory_item <::app_graphics3d_fuel::pane_impact >();
      //factory()->add_factory_item <::app_graphics3d_fuel::top_impact >();
      //factory()->add_factory_item <::app_graphics3d_fuel::toggle_impact >();


      ::core::application::init_instance();

      //if (!::aura::application::init_instance())
      //{

      //   return false;

      //}

      ///set_local_data();

      add_impact_system(
         "main", __initialize_new ::user::single_document_template(
                          "main",
                          ::type < ::app_graphics3d_fuel::document >(),
                          ::type < ::app_graphics3d_fuel::frame >(),       // top level SDI frame::windowing::window
                        ::type < ::app_graphics3d_fuel::impact >()));
      //::type < ::app_graphics3d_fuel::pane_impact >()));
      
      //add_document_template(pdocumenttemplate);
      //
      //m_ptemplateFuelMain = pdocumenttemplate;
      
      add_impact_system(
         "impact", __initialize_new ::user::single_document_template(
                          "main",
                          ::type < ::app_graphics3d_fuel::document >(),
                          ::type < ::app_graphics3d_fuel::child_frame >(),       // top level SDI frame::windowing::window
                          //::type < ::app_graphics3d_fuel::main_impact >()));
                     ::type < ::app_graphics3d_fuel::impact >()));
   
      //add_document_template(pdocumenttemplate);

      //m_ptemplateFuelImpact = pdocumenttemplate;

      //return true;

   }


   void application::term_instance()
   {

      ::aura::application::term_instance();
   }


   void application::on_request(::request * prequest)
   {

      //if (::is_set(prequest))
      //{

      //   string strMode = prequest->payload("mode");

      //   if (strMode == "window")
      //   {

      //      auto pwindow = __create_new < window >();

      //      //::user::system cs(
      //      //   WS_EX_LAYERED,
      //      //   NULL,
      //      //   "Fuel Lab - OpenGL Testing", 
      //      //   WS_VISIBLE,
      //      //   int_rectangle_dimension(100, 100, 900, 300));

      //      //pwindow->create_window_ex(cs);

      //      //pwindow->set_dim(100, 100, 900, 300);

      //      //pwindow->display();

      //      //pwindow->set_need_redraw();

      //      return;

      //   }


      //}


      if(impact_system("main")->get_document_count() == 0)
      {

         impact_system("main")->request(prequest);

      }

      //if(prequest->m_payloadFile.has_character())
      //{

      //   m_ptemplateFuelImpact->request(prequest);

      //}

   }


#ifdef _DEBUG


   long long application::increment_reference_count()
   {

      return ::object::increment_reference_count();

   }


   long long application::decrement_reference_count()
   {

      return ::object::decrement_reference_count();

   }


#endif


   ::string application::draw2d_get_default_implementation_name()
   {

      return system()->implementation_name("draw2d", "opengl");

   }


   //__namespace_application_factory("app-graphics3d/fuel");


} // namespace app_graphics3d_fuel








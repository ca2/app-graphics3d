#include "platform.h"
#include "application.h"
//#include "child_frame.h"
//#include "document.h"
//#include "edit_impact.h"
//#include "frame.h"
//#include "impact.h"
//#include "main_impact.h"
//#include "pane_impact.h"
//#include "toggle_impact.h"
//#include "top_impact.h"
//#include "window.h"
//#include "application.h"
#include "pane_impact.h"
#include "main_frame.h"
//#include "main_impact.h"
#include "impact.h"
#include "document.h"
#include "acme/handler/request.h"
#include "acme/platform/system.h"
#include "acme/prototype/prototype/url.h"
#include "apex/database/_binary_stream.h"
#include "acme/filesystem/filesystem/directory_context.h"
#include "acme/filesystem/filesystem/file_context.h"
#include "apex/interprocess/communication.h"
#include "berg/user/user/single_document_template.h"
//#include "app-core/hello_multiverse/render.h"
//#include "app-core/hello_multiverse/hello_multiverse.h"
#include "aura/user/user/user.h"
#include "acme/handler/request.h"
#include "acme/constant/impact.h"
#include "acme/platform/system.h"
#include "berg/user/user/single_document_template.h"


__IMPLEMENT_APPLICATION_RELEASE_TIME(app_graphics3d_fuel);
IMPLEMENT_APPLICATION_FACTORY(app_graphics3d_fuel);


namespace app_graphics3d_fuel
{


   // __IMPLEMENT_APPLICATION_RELEASE_TIME1(app_graphics3d_fuel_memory_opengl));


   application::application()
   {

      //m_bUseSwapChainWindow = false;
      //m_ptemplateFuelMain = nullptr;
      //m_ptemplateFuelImpact = nullptr;

      m_strAppId              = "app-graphics3d/fuel";
      m_strAppName            = "Fuel";
      m_strBaseSupportId      = "app-graphics3d/fuel";
      m_bLicense              = false;
      m_bGpu = true;

   }


   application::~application()
   {

   }




   void application::init_instance()
   {

      auto puser = user();

      puser->will_use_impact_hint("font_selection_impact");
      puser->will_use_impact_hint("color_selection_impact");


      factory()->add_factory_item <::app_graphics3d_fuel::pane_impact >();
      factory()->add_factory_item <::app_graphics3d_fuel::document >();
      factory()->add_factory_item <::app_graphics3d_fuel::main_frame >();
      factory()->add_factory_item <::app_graphics3d_fuel::impact >();
      //factory()->add_factory_item <::app_graphics3d_fuel::main_impact >();

      ::core::application::init_instance();

      add_matter_locator("app-core/hello_multiverse");

      //set_local_data();

      add_impact_system(
         "main", __initialize_new::user::single_document_template(
            "main",
            typeid(::app_graphics3d_fuel::document),
            typeid(::app_graphics3d_fuel::main_frame),       // top level SDI frame::windowing::window
            typeid(::app_graphics3d_fuel::pane_impact)));

      //add_document_template(psingledocumenttemplate);

      //constructø(m_ptemplateFlagMain, psingledocumenttemplate);

      //add_impact_system(
      //   "editable", __initialize_new::user::single_document_template(
      //      "editable",
      //      typeid(::app_graphics3d_fuel::document),
      //      typeid(main_frame),       // top level SDI frame::windowing::window
      //      typeid(main_impact)));

      //add_document_template(psingledocumenttemplate);

      //constructø(m_ptemplateFlagEditableImpact, psingledocumenttemplate);

      add_impact_system(
         "impact",
         __initialize_new::user::single_document_template(
            "html/frame",
            typeid(::app_graphics3d_fuel::document),
            typeid(simple_frame_window),       // top level SDI frame::windowing::window
            typeid(::app_graphics3d_fuel::impact)));

      //add_document_template(psingledocumenttemplate);

      //constructø(m_ptemplateFlagImpact, psingledocumenttemplate);

      //return true;


   }


   void application::term_instance()
   {

      ::aura::application::term_instance();

   }


   void application::on_request(::request * prequest)
   {

      ::file::path pathLastOpenedFile;

      if (prequest->has_file())
      {

         ::user::document * puserdocument;

         if (has_property("no_hello_edit"))
         {

            puserdocument = impact_system("impact")->get_document();
         }
         else
         {

            puserdocument = impact_system("editable")->get_document();

         }

         ::pointer<document> pdocument = puserdocument;

         //if (pdocument->try_load_image(prequest->m_payloadFile))
         //{

         //   pdocument->change_to_last_bilbo_impact(true);

         //   return;

         //}

         if (has_property("no_hello_edit"))
         {

            impact_system("impact")->request(prequest);

         }
         else
         {

            impact_system("impact")->request(prequest);

         }

         if (::user::__document(prequest) == nullptr)
         {

            return;

         }

      }
      else
      {

         impact_system("main")->request(prequest);

         if (::user::__document(prequest) == nullptr)
         {

            return;

         }

         if (!prequest->has_file() && !has_activation_message())
         {

            datastream()->get("last_opened_file", pathLastOpenedFile);

            auto strProtocol = ::url::get_protocol(pathLastOpenedFile);

            if (strProtocol != "app-core.flag" && !file()->exists(pathLastOpenedFile))
            {

               pathLastOpenedFile = directory()->document2() / "default.appcoreflag";

            }

         }

      }

      ::user::document * pdocument = ::user::__document(prequest);

      if (pdocument != nullptr)
      {

         auto strProtocol = ::url::get_protocol(pathLastOpenedFile);

         if (strProtocol == "app-core.flag")
         {

            m_pinterprocesscommunication2->_handle_uri(pathLastOpenedFile);

         }
         else if (file()->exists(pathLastOpenedFile))
         {

            fork([pdocument, pathLastOpenedFile]()
               {


                  preempt(1_s);

                  try
                  {

                     if (pdocument->m_path.is_empty())
                     {

                        pdocument->open_document(pathLastOpenedFile);

                     }

                  }
                  catch (...)
                  {


                  }

});


         }

         //if (pdocument->m_path.is_empty())
         //{

       //     show_country_flag("");

         //}

      }

   }



#ifdef _DEBUG

   long long application::increment_reference_count()
   {

      return ::bred::application::increment_reference_count();

   }


   long long application::decrement_reference_count()
   {

      return ::bred::application::decrement_reference_count();

   }

#endif

   ::string application::draw2d_get_default_implementation_name()
   {

      return ::bred::application::draw2d_get_default_implementation_name();

   }


   void application::create_options_impact(const ::atom & atom, ::user::interaction * pparent)
   {

      if (atom == APP_OPTIONS_IMPACT)
         ::bred::application::create_options_impact("options_impact_handler://gpu", pparent);
      else
         ::bred::application::create_options_impact(atom, pparent);

   }


   void application::on_after_create_form(const ::atom & atom, ::user::interaction * pparent)
   {

      ::bred::application::on_after_create_form(
         atom == APP_OPTIONS_IMPACT ? ::atom("options_impact_handler://gpu") : atom, pparent);

   }


} // namespace app_graphics3d_fuel








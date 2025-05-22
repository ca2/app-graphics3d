#include "framework.h"
#include "application.h"
#include "frame.h"
#include "main_frame.h"
#include "tab_impact.h"
#include "impact.h"
#include "document.h"
#include "acme/constant/id.h"
#include "acme/handler/request.h"
#include "base/user/user/single_document_template.h"


__IMPLEMENT_APPLICATION_RELEASE_TIME(app_simple_shader);
IMPLEMENT_APPLICATION_FACTORY(app_simple_shader);


namespace app_simple_shader
{


   // __IMPLEMENT_APPLICATION_RELEASE_TIME1(app_simple_shader));


   application::application() :
      m_textLoadFragmentPathPrefix(this, id_load_fragment_path_prefix)
   {
      
      m_bImaging = true;

      //m_ptemplateSimpleShaderMain = nullptr;

      m_ptabimpact = nullptr;

      m_strAppId = "app-simple/shader";

      m_strBaseSupportId = "app-simple/shader";

      m_strAppName = "Simple Shader";

      m_bLicense = false;

      m_strMainTitle = "Simple Drawing";

   }


   application::~application()
   {

   }




   //string application::get_hover_font() const
   //{

   //   return "Helvetica";

   //}


   void application::init_instance()
   {

      //set_local_data();

      //puser->will_use_impact_hint(FONTSEL_IMPACT);

      factory()->add_factory_item <::app_simple_shader::document >();
      factory()->add_factory_item <::app_simple_shader::frame >();
      factory()->add_factory_item <::app_simple_shader::main_frame >();
      //factory()->add_factory_item <::user::button_impact >();
      factory()->add_factory_item <::app_simple_shader::impact >();
      factory()->add_factory_item <::app_simple_shader::tab_impact >();

      //default_toggle_check_handling("simple_checkbox");

      //default_toggle_check_handling("no_client_frame");

      ::aura::application::init_instance();

      //if (!::aura::application::init_instance())
      //{

      //   return false;

      //}

      add_impact_system(
         "main", __initialize_new ::user::single_document_template(
                               "main",
                               ::type < document >(),
                               ::type < main_frame >(),
                               ::type < tab_impact >()));

      //add_document_template(pdoctemplate);

      //m_ptemplateSimpleShaderMain = pdoctemplate;

      //return true;

   }


   void application::term_application()
   {

      ::aura::application::term_application();

   }


   void application::on_request(::request * prequest)
   {

#if 0

#ifdef _DEBUG

      informationf("_DEBUG build? (basis)");

      ASSERT(false);

#else

      informationf("RELEASE build? (stage)");

      ASSERT(false);

#endif

#endif

      if (impact_system("main")->get_document_count() == 0)
      {

         if(prequest->m_egraphicsoutputpurpose & ::graphics::e_output_purpose_screen)
         {

            information() << "prequest->m_bMakeVisible";

         }
         else
         {

            information() << "NOT prequest->m_bMakeVisible";

         }

         impact_system("main")->request(prequest);

      }

      if (is_true("wfi_maximize"))
      {

         prequest->payload("document").cast < document >()->get_typed_impact < ::user::tab_impact >()->top_level_frame()->design_window_maximize();

      }

      informationf("\nfinished simple_shader::on_request");

   }


   string application::preferred_experience()
   {

      return ::aura::application::preferred_experience();

   }


//#ifdef _DEBUG
//
//
//   long long application::increment_reference_count()
//   {
//
//      return ::object::increment_reference_count();
//
//   }
//
//
//   long long application::decrement_reference_count()
//   {
//
//      return ::object::decrement_reference_count();
//
//   }
//
//
//#endif


   ::pointer<impact> application::create_simple_shader_impact(::user::impact* pimpactParent, ::user::impact_data * pimpactdata)
   {

      return pimpactParent->create_impact < impact >(pimpactdata);

   }


   //::atom application::translate_property_id(const ::atom & atom)
   //{

   //   return atom;

   //   //if(atom == "simple_checkbox")
   //   //{

   //   //   return id_simple_checkbox;

   //   //}
   //   //else if(atom == "simple_text")
   //   //{

   //   //   return id_simple_text;

   //   //}

   //   //return atom;

   //}



} // namespace simple_shader




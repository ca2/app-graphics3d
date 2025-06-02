#include "framework.h"
#include "application.h"
#include "document.h"
#include "frame.h"
#include "impact.h"
#include "pane_impact.h"
#include "main_frame.h"
#include "acme/handler/request.h"
#include "base/user/user/single_document_template.h"
#include "base/user/user/multiple_document_template.h"


__IMPLEMENT_APPLICATION_RELEASE_TIME(graphics3d_hello3d);
IMPLEMENT_APPLICATION_FACTORY(graphics3d_hello3d);


namespace graphics3d_hello3d
{


   // __IMPLEMENT_APPLICATION_RELEASE_TIME1(graphics3d_hello3d));


   application::application()
   {
      
      //m_ptemplateMain = nullptr;

      //m_ptemplateImpact = nullptr;

      m_ppaneimpact = nullptr;

      m_strAppId = "graphics3d/hello3d";

      m_strAppName = "Complex Impact 3D";

      m_strBaseSupportId = "graphics3d/hello3d";

      m_bLicense = false;

      m_strMainTitle = "Complex Impact 3D";

   }


   application::~application()
   {


   }


   void application::init_instance()
   {

//      set_local_data();

      factory()->add_factory_item<::graphics3d_hello3d::document >();
      factory()->add_factory_item<::graphics3d_hello3d::frame >();
      factory()->add_factory_item<::graphics3d_hello3d::main_frame >();
      //factory()->add_factory_item<::user::button_impact >();
      factory()->add_factory_item<::graphics3d_hello3d::impact >();
      factory()->add_factory_item<::graphics3d_hello3d::pane_impact >();

      ::core::application::init_instance();

      add_impact_system(
         "main", __initialize_new ::user::single_document_template(
                               "main",
                               ::type < ::user::document >(),
                               ::type < main_frame >(),
                               ::type < pane_impact >()));

      //m_ptemplateMain = pdoctemplate;

      //add_document_template(pdoctemplate);

      //auto pmultidoctemplate = __initialize_new ::user::multiple_document_template(
      //   "main",
      //   ::type < document >(),
      //   ::type < frame >(),
      //   ::type < impact >()));

      //m_ptemplateImpact = pmultidoctemplate;

      //add_document_template(pmultidoctemplate);

   }


   void application::term_application()
   {

      ::aura::application::term_application();

   }


   void application::on_request(::request * prequest)
   {


      if (impact_system("main")->get_document_count() == 0)
      {

         impact_system("main")->request(prequest);

      }
      else
      {

         //::file::path path = prequest->m_payloadFile;

         //m_ptemplateImpact->open_document_file(this, path);

      }

   }


   string application::preferred_experience()
   {

      return ::core::application::preferred_experience();

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


   ::pointer<impact> application::create_simple_shader_impact(::user::impact* pimpactParent, ::user::impact_data * pimpactdata)
   {

      return pimpactParent->create_impact < impact >(pimpactdata);

   }


} // namespace graphics3d_hello3d




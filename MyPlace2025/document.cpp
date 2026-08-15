#include "platform.h"
#include "document.h"
#include "impact.h"
#include "application.h"
#include "acme/filesystem/filesystem/file_context.h"


namespace app_graphics3d_MyPlace2025
{


   document::document()
   {

   }


   document::~document()
   {

   }


   bool document::on_new_document()
   {

      //::file::path path;
      //
      //::pointer < ::app_graphics3d_MyPlace2025::application > papp = application();
      //
      //path = papp->m_pathApplicationText;

      ////const char * pszPath = path.c_str();
      //
      //try
      //{

      //   file()->get_lines(m_straLine, path);
      //   
      //}
      //catch(...)
      //{
      // 
      //}


      //if (m_straLine.is_empty())
      //{
      //   
      //   try
      //   {
      //      
      //      file()->get_lines(m_straLine, "document://application/app-simple/application/application.txt");
      //      
      //   }
      //   catch (...)
      //   {
      //   
      //      
      //   }

      //}

      //   


      return true;
      //return ::user::document::on_new_document();

   }


   bool document::on_open_document(const ::payload & payloadFile)
   {

      auto pimpact = get_typed_impact < ::app_graphics3d_MyPlace2025::impact >();

      if(pimpact == NULL)
      {

         return true;

      }

      return true;

   }


#ifdef _DEBUG


   long long document::increment_reference_count()
   {

      return  ::user::document::increment_reference_count();

   }


   long long document::decrement_reference_count()
   {

      return  ::user::document::decrement_reference_count();

   }


#endif


} // namespace app_graphics3d_MyPlace2025




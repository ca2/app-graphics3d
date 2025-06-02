#include "framework.h"
#include "document.h"
#include "render.h"
#include "pane_impact.h"
#include "impact.h"
#include "acme/constant/id.h"
#include "acme/filesystem/filesystem/file_context.h"


namespace graphics3d_impact3d
{


   document::document()
   {

   }


   document::~document()
   {

   }


   //void document::assert_ok() const
   //{

   //   ::user::document::assert_ok();

   //}


   //void document::dump(dump_context & dumpcontext) const
   //{

   //   ::user::document::dump(dumpcontext);

   //}


   bool document::on_new_document()
   {

      return ::user::document::on_new_document();

   }


   bool document::on_open_document(const ::payload & payloadFile)
   {

      impact * pimpact = get_typed_impact < impact >();

      if(pimpact == NULL)
      {

         return true;

      }

      string strPath = payloadFile.as_file_path();

      __construct_new(m_prender);

      auto strVertexSourceCode = file()->as_string("matter://TransformVertexShader.vertexshader");

      auto strFragmentSourceCode = file()->as_string("matter://TextureFragmentShader.fragmentshader");

      m_prender->m_strMatrixUniform = "MVP";

      m_prender->m_strTextureUniform = "myTextureSampler";

      m_prender->set_vertex_source_code(strVertexSourceCode);

      m_prender->set_fragment_source_code(strFragmentSourceCode);

      m_prender->set_model_path(strPath);

      m_path = strPath;

      

      get_app()->m_ppaneimpact->set_current_tab_by_id(MAIN_IMPACT);

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


   //stream& document::write(::stream& stream) const
   //{

   //   return stream;

   //}


} // namespace graphics3d_impact3d



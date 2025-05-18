// From V0idsEmbrace@Twich continuum project
// by camilo on 2025-05-18 04:11 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "scene.h"


namespace cube
{


   scene::scene()
   {


   }

   scene::~scene()
   {


   }


   void scene::on_load_scene()
   {


   }





      void scene::add_object(::cube::application_object* papplicationobject)
      {

         m_pengine->m_mapObjects[papplicationobject->m_iId] = papplicationobject;

      }


} // namespace cube




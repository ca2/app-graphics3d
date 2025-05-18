// From V0idsEmbrace@Twich continuum project
// by camilo on 2025-05-18 04:11 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "application.h"
#include "engine.h"
#include "impact.h"
#include "scene.h"


namespace graphics3d
{


   scene::scene()
   {


   }

   scene::~scene()
   {


   }


   ::graphics3d::camera scene::get_default_camera()
   {

      return {};

   }


   void scene::on_load_scene()
   {


   }


   void scene::add_object(::graphics3d::scene_object *pobject)
   {

      m_mapObjects[pobject->m_iId] = pobject;

   }


   ::pointer<model> scene::create_model_from_file(const ::file::path& path)
   {

      auto pmodel = m_pimpact->m_pengine->create_model_from_file(path);

      return pmodel;

   }


   scene_object & scene::create_object(const ::file::path& path)
   {
      
      auto pmodel = create_model_from_file(path);
      
      auto pobject = __øcreate < ::graphics3d::scene_object>();

      add_object(pobject);

      pobject->m_pmodel = pmodel;

      return *pobject;

   }


} // namespace graphics3d




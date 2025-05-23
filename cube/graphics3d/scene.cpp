// From V0idsEmbrace@Twich continuum project
// by camilo on 2025-05-18 04:11 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "application.h"
#include "context.h"
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


   void scene::on_initialize_scene()
   {


   }


   void scene::initialize_scene(::graphics3d::engine * pengine)
   {

      m_pengine = pengine;

      on_initialize_scene();

   }


   ::pointer < ::graphics3d::camera > scene::get_default_camera()
   {

      return {};

   }


   void scene::on_load_scene(::gpu::context* pgpucontext)
   {


   }


   void scene::add_object(::graphics3d::scene_object *pobject)
   {

      m_mapObjects[pobject->m_iId] = pobject;

   }


   void scene::on_update_global_ubo(::gpu::context* pgpucontext)
   {



   }


   void scene::on_render(::gpu::context * pgpucontext)
   {


   }


   ::pointer<model> scene::create_tinyobjloader_model(const ::file::path& path)
   {

      auto pmodel = m_pengine->create_tinyobjloader_model(path);

      return pmodel;

   }


   scene_object & scene::tinyobjloader_object(const ::file::path& path)
   {
      
      auto pmodel = create_tinyobjloader_model(path);
      
      auto pobject = __øcreate < ::graphics3d::scene_object>();

      add_object(pobject);

      pobject->m_pmodel = pmodel;

      return *pobject;

   }


} // namespace graphics3d




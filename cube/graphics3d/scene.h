// From V0idsEmbrace@Twich continuum project
// by camilo on 2025-05-18 04:03 <3ThomasBorregaardSorensen!!
#pragma once


#include "apex/platform/app_consumer.h"
#include "app-cube/cube/graphics3d/scene_object.h"


namespace graphics3d
{


   class CLASS_DECL_CUBE scene :
      virtual public ::app_consumer < ::cube::application >
   {
   public:


      ::pointer < ::graphics3d::engine >     m_pengine;
      ::string                               m_strName;
      scene_object::map				            m_mapObjects;


      scene();
      ~scene() override;


      virtual void on_initialize_scene();
      virtual void initialize_scene(::graphics3d::engine * pengine);


      virtual ::pointer < ::graphics3d::camera > get_default_camera();


      virtual void on_load_scene(::graphics3d::context* pcontext);


      //virtual void handle_mouse_move(int x, int y);

      //virtual ::pointer<model> createModelFromFile(::cube::context* pcontext, const ::file::path& path);

      virtual void on_update_global_ubo();

      virtual void on_render(::graphics3d::context * pcontext);


      virtual ::pointer<::graphics3d::model> create_tinyobjloader_model(const ::file::path& path);
      virtual scene_object & tinyobjloader_object(const ::file::path& path);

      virtual void add_object(::graphics3d::scene_object* pobject);


   };


} // namespace graphics3d




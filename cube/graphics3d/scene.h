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


      ::pointer < ::cube::impact >     m_pimpact;
      ::string                         m_strName;
      scene_object::map				      m_mapObjects;


      scene();
      ~scene() override;


      virtual ::pointer < ::graphics3d::camera > get_default_camera();


      virtual void on_load_scene();


      //virtual void handle_mouse_move(int x, int y);

      //virtual ::pointer<model> createModelFromFile(::cube::context* pcontext, const ::file::path& path);


      


      virtual ::pointer<::graphics3d::model> create_model_from_file(const ::file::path& path);
      virtual scene_object & create_object(const ::file::path& path);

      virtual void add_object(::graphics3d::scene_object* pobject);


   };


} // namespace graphics3d




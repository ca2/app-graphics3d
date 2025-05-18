// From V0idsEmbrace@Twich continuum project
// by camilo on 2025-05-18 04:03 <3ThomasBorregaardSorensen!!
#pragma once


#include "apex/platform/app_consumer.h"
#include "scene_object.h"


namespace cube
{


   class CLASS_DECL_CUBE scene :
      virtual public ::app_consumer < application >
   {
   public:


      scene_object::map				m_mapObjects;

      scene();
      ~scene() override;


      virtual void on_load_scene();


      //virtual void handle_mouse_move(int x, int y);

      //virtual ::pointer<model> createModelFromFile(::cube::context* pcontext, const ::file::path& path);


      virtual void add_object(::cube::scene_object* pobject);



   };


} // namespace cube




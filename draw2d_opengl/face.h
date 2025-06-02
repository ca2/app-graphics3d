// Created by camilo with Mom and Bilbo support on
// 2025-06-01 23:20 <3ThomasBorregaardSorensen!!
#pragma once


#include "cube/draw2d/face.h"



namespace draw2d_opengl
{


   class CLASS_DECL_DRAW2D_OPENGL face:
      virtual public ::draw2d_gpu::face
   {
   public:

      
      unsigned int m_VAO, m_VBO;


      face();
      ~face() override;

      //virtual character& get_character(const ::scoped_string& scopedstr);


      virtual void create_character(::draw2d_gpu::character& ch, const ::scoped_string& scopedstr);

      void create_draw_buffers();

      void create_texture(::draw2d_gpu::character& ch, const unsigned char* p);


   };


} // namespace draw2d_opengl


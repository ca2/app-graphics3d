// Created by camilo on 2025-05-13 12:59 <3ThomasBorregaardSorensen!!
#pragma once


namespace opengl
{ 


   class CLASS_DECL_OPENGL_GRAPHICS3D_MYPLACE2025 application :
      virtual public ::particle
   {
   public:


      application();
      ~application() override;


      virtual void resize(int cx, int cy);


      //virtual void run_application();

   };


} // namespace opengl
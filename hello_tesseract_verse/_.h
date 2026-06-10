#pragma once


#include "core/_.h"
#include "app-graphics3d/graphics3d/_.h"


#if defined(_app_graphics3d_hello_tesseract_verse_project)
#define CLASS_DECL_APP_GRAPHICS3D_HELLO_TESSERACT_VERSE  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_APP_GRAPHICS3D_HELLO_TESSERACT_VERSE  CLASS_DECL_IMPORT
#endif


namespace app_graphics3d_hello_tesseract_verse
{

   class application;
   class pane_impact;
   class document;
   class impact;


   class main_scene;
   class simple_render_system;
   class point_light_system;


   class GlobalUbo;


   enum enum_mouse
   {
      e_mouse_none = 0,
      e_mouse_updateLook
   };
   enum enum_keyboard
   {
      e_keyboard_none = 0,
      e_keyboard_updateMovement,
      e_keyboard_spaceExplorer
   };



} // namespace flag







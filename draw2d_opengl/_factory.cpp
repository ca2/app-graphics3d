#include "framework.h"
#include "_opengl.h"
#include "image.h"
#include "graphics.h"
#include "bitmap.h"
#include "pen.h"
#include "brush.h"
#include "region.h"
#include "font.h"
#include "path.h"
#include "draw2d.h"
//#ifdef WINDOWS_DESKTOP
//#include "face_gdiplus.h"
//#else
//#include "face_freetype.h"
//#endif

__FACTORY_EXPORT void draw2d_opengl_factory(::factory::factory * pfactory)
{

   pfactory->add_factory_item < ::draw2d_opengl::image, ::image::image >();
   pfactory->add_factory_item < ::draw2d_opengl::graphics, ::draw2d::graphics >();
   pfactory->add_factory_item < ::draw2d_opengl::bitmap, ::draw2d::bitmap >();
   pfactory->add_factory_item < ::draw2d_opengl::pen, ::draw2d::pen >();
   pfactory->add_factory_item < ::draw2d_opengl::brush, ::draw2d::brush >();
   pfactory->add_factory_item < ::draw2d_opengl::region, ::draw2d::region >();
   pfactory->add_factory_item < ::draw2d_opengl::font, ::write_text::font >();
   pfactory->add_factory_item < ::draw2d_opengl::path, ::draw2d::path >();

//#ifdef WINDOWS_DESKTOP
//   pfactory->add_factory_item < ::draw2d_opengl::face_gdiplus, ::draw2d_gpu::face >();
//#else
//   pfactory->add_factory_item < ::draw2d_opengl::face_freetype, ::draw2d_gpu::face >();
//#endif


   pfactory->add_factory_item < ::draw2d_opengl::draw2d, ::draw2d::draw2d >();

}




#include "framework.h"
#include "image.h"
#include "graphics.h"
#include "bitmap.h"
#include "pen.h"
#include "brush.h"
#include "region.h"
#include "font.h"
#include "path.h"
#include "draw2d.h"


__FACTORY_EXPORT void draw2d_nanovg_factory(::factory::factory * pfactory)
{

   pfactory->add_factory_item < ::draw2d_nanovg::image, ::image::image >();
   pfactory->add_factory_item < ::draw2d_nanovg::graphics, ::draw2d::graphics >();
   pfactory->add_factory_item < ::draw2d_nanovg::bitmap, ::draw2d::bitmap >();
   pfactory->add_factory_item < ::draw2d_nanovg::pen, ::draw2d::pen >();
   pfactory->add_factory_item < ::draw2d_nanovg::brush, ::draw2d::brush >();
   pfactory->add_factory_item < ::draw2d_nanovg::region, ::draw2d::region >();
   pfactory->add_factory_item < ::draw2d_nanovg::font, ::write_text::font >();
   pfactory->add_factory_item < ::draw2d_nanovg::path, ::draw2d::path >();


   pfactory->add_factory_item < ::draw2d_nanovg::draw2d, ::draw2d::draw2d >();

}




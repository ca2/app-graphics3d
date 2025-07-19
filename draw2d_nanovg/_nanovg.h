// Created by camilo on 2023-04-27 17:06 <3ThomasBorregaardSorensen!!
#pragma once




#undef USUAL_OPERATING_SYSTEM_SUPPRESSIONS
#include "acme/_operating_system.h"

#include <nanovg.h>


#if defined(WINDOWS_DESKTOP)
#define VK_USE_PLATFORM_WIN32_KHR
#else
#error "wwwwwhat?!?!"
#endif


//#include </opengl.h>



#include "graphics.h"


#define VK2D_GRAPHICS(pgraphics) (dynamic_cast < ::draw2d_nanovg::graphics * > (pgraphics))
#define VK2D_HDC(pgraphics) (VK2D_GRAPHICS(pgraphics)->m_hdc)


namespace opengl
{


   void resize(const ::int_size & size, bool bYSwap);


   //https://community.khronos.org/t/draw-an-arc-in-opengl/57994/2
   void draw_arc(float cx, float cy, float r, float start_angle, float arc_angle, int num_segments);

} // namespace opengl



namespace draw2d_nanovg
{


   class draw2d;


} // namespace draw2d_nanovg


//void copy(nanovg_matrix_t* pcairomatrix, const ::geometry2d::matrix* pmatrix);




inline NVGcolor as_nvg_color(const ::color::color& color)
{

   NVGcolor nvgcolor;

   nvgcolor.r = color.f32_red();
   nvgcolor.g = color.f32_green();
   nvgcolor.b = color.f32_blue();
   nvgcolor.a = color.f32_opacity();

   return nvgcolor;

}


inline void _nvgcopy(float* p, const ::geometry2d::matrix * pmatrix)
{


   p[0] = (float) pmatrix->a1;
   p[1] = (float) pmatrix->b1;
   p[2] = (float) pmatrix->a2;
   p[3] = (float) pmatrix->b2;
   p[4] = (float) pmatrix->c1;
   p[5] = (float) pmatrix->c2;


}



#define __expand_nanovgmatrix(nanovgmatrix) \
   nanovgmatrix[0], nanovgmatrix[1], nanovgmatrix[2], nanovgmatrix[3], nanovgmatrix[4], nanovgmatrix[5]





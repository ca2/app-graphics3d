#include "framework.h"
#include "impact.h"
#include "pane_impact.h"
#include "acme/constant/message.h"
#include "acme/handler/topic.h"
#include "aura/graphics/draw2d/graphics.h"
#include "aura/graphics/draw2d/pen.h"
#include "aura/message/user.h"

//#include "aura/update.h"
//#include <math.h>
// http://stackoverflow.com/questions/4052940/how-to-make-an-opengl-rendering-context-with-transparent-background
//http://stackoverflow.com/users/176769/karlphillip
//#include <windows.h>
//#ifdef WINDOWS
//#include <windowsx.h>
//#endif

//
//#ifdef LINUX
//
//#define WIDTH 3200
//#define HEIGHT 1800f
//
//const int sbAttrib[] = {GLX_DOUBLEBUFFER, 0, GLX_RED_SIZE, 1,GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1,GLX_ALPHA_SIZE, 1, GLX_DEPTH_SIZE, 16,None};
//int pbAttrib[] = {GLX_PBUFFER_WIDTH, WIDTH,GLX_PBUFFER_HEIGHT, HEIGHT,GLX_PRESERVED_CONTENTS, True,None };
//
//#endif
//
//
////extern CLASS_DECL_AXIS thread_int_ptr < DWORD_PTR > t_time1;
//GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.00f };
//GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.00f };
//GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.00f };
////GLfloat LightPosition2[] = { -5.0f, -5.0f, 32.0f, 1.00f };

//#ifdef WINDOWS
//typedef BOOL(WINAPI * PFNWGLDESTROYPBUFFERARBPROC) (HPBUFFERARB hPbuffer);
//typedef BOOL(WINAPI * PFNWGLQUERYPBUFFERARBPROC) (HPBUFFERARB hPbuffer, int iAttribute, int *piValue);
//typedef HDC(WINAPI * PFNWGLGETPBUFFERDCARBPROC) (HPBUFFERARB hPbuffer);
//typedef HPBUFFERARB(WINAPI * PFNWGLCREATEPBUFFERARBPROC) (HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
//typedef int (WINAPI * PFNWGLRELEASEPBUFFERDCARBPROC) (HPBUFFERARB hPbuffer, HDC hDC);
//
//PFNWGLDESTROYPBUFFERARBPROC                       wglDestroyPbufferARB;
//PFNWGLQUERYPBUFFERARBPROC                         wglQueryPbufferARB;
//PFNWGLGETPBUFFERDCARBPROC                         wglGetPbufferDCARB;
//PFNWGLCREATEPBUFFERARBPROC                        wglCreatePbufferARB;
//PFNWGLRELEASEPBUFFERDCARBPROC                     wglReleasePbufferDCARB;
//
//
//typedef BOOL(WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, unsigned int nMaxFormats, int *piFormats, unsigned int *nNumFormats);
//typedef BOOL(WINAPI * PFNWGLGETPIXELFORMATATTRIBFVARBPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, unsigned int nAttributes, const int *piAttributes, FLOAT *pfValues);
//typedef BOOL(WINAPI * PFNWGLGETPIXELFORMATATTRIBIVARBPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, unsigned int nAttributes, const int *piAttributes, int *piValues);
//
//PFNWGLCHOOSEPIXELFORMATARBPROC                    wglChoosePixelFormatARB;
//PFNWGLGETPIXELFORMATATTRIBFVARBPROC               wglGetPixelFormatAttribfvARB;
//PFNWGLGETPIXELFORMATATTRIBIVARBPROC               wglGetPixelFormatAttribivARB;
//
//
//#endif

#include <assert.h>
#ifdef WINDOWS
#include <tchar.h>
#endif // WINDOWS
//#ifdef  assert
//#define verify(expr) if(!expr) assert(0)
//#else verify(expr) expr
//#endif






namespace app_graphics3d_fuel
{



   impact::impact()
   {
   }

      impact::~impact()
   {
   }

   //void impact::assert_ok() const
   //{
   //   user::box::assert_ok();
   //}

   //void impact::dump(dump_context & dumpcontext) const
   //{
   //   user::box::dump(dumpcontext);
   //}


   void impact::install_message_routing(::channel * pchannel)
   {

//      ::backimpact::user::impact::install_message_routing(pchannel);

      ::user::impact::install_message_routing(pchannel);

      MESSAGE_LINK(e_message_create, pchannel, this, &impact::on_message_create);
      MESSAGE_LINK(e_message_destroy, pchannel, this, &impact::on_message_destroy);
      MESSAGE_LINK(e_message_key_down, pchannel, this, &impact::on_message_key_down);
      MESSAGE_LINK(e_message_key_up, pchannel, this, &impact::on_message_key_up);

   }


   bool impact::keyboard_focus_is_focusable()
   {

      return true;

   }

   
   void impact::on_message_key_down(::message::message * pmessage)
   {

      auto pkey = pmessage->m_union.m_pkey;

      pmessage->m_bRet = true;

   }


   void impact::on_message_key_up(::message::message * pmessage)
   {

      auto pkey = pmessage->m_union.m_pkey;

      pmessage->m_bRet = true;

   }


   void impact::on_message_create(::message::message * pmessage)
   {

      ::pointer<::message::create>pcreate(pmessage);

      pcreate->previous();

      if (pcreate->m_bRet)
      {

         return;

      }





   }

   void impact::on_message_destroy(::message::message * pmessage)
   {


   }

   void impact::on_timer(timer * ptimer)
   {

   }



   void impact::handle(::topic * ptopic, ::handler_context * phandlercontext)
   {

      if (ptopic->id() == id_initial_update)
      {
         ::pointer<::userex::pane_tab_impact>ppaneimpact = get_typed_parent < ::userex::pane_tab_impact >();
         //if(ppaneimpact.is_set())
         //{
         //   ppaneimpact->m_pflagview = this;
         //}

      }
      __UNREFERENCED_PARAMETER(ptopic);
   }


   void impact::_001OnNcClip(::draw2d::graphics_pointer & pgraphics)
   {

      ::user::impact::_001OnNcClip(pgraphics);

   }


   void impact::_001OnClip(::draw2d::graphics_pointer & pgraphics)
   {

      ::user::impact::_001OnClip(pgraphics);

   }


   void impact::_001OnNcDraw(::draw2d::graphics_pointer & pgraphics)
   {

      ::user::impact::_001OnNcDraw(pgraphics);

   }


   void impact::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {

      //return;

      pgraphics->fill_solid_rectangle(::int_rectangle_dimension(10, 10, 100, 100), argb(128, 200, 220, 255));

      //return;

      pgraphics->fill_solid_rectangle(::int_rectangle_dimension(110, 100, 100, 100), argb(255, 200, 220, 255));

      auto ppen = __øcreate < ::draw2d::pen > ();

      ppen->create_solid(10, argb(200, 20, 155, 50));

      pgraphics->set(ppen);

      pgraphics->draw_line(::double_point( 0.0, 0.0 ), ::double_point(300.0, 300.0));

      pgraphics->fill_solid_rectangle(::int_rectangle_dimension(10, 150, 100, 100), argb(128, 100, 255, 120));

      pgraphics->fill_solid_rectangle(::int_rectangle_dimension(110, 150, 100, 100), argb(255, 100, 255, 120));

      auto ppen2 = __øcreate < ::draw2d::pen > ();

      ppen2->create_solid(5, argb(200, 20, 155, 50));

      pgraphics->set(ppen2);

      pgraphics->draw_line(::double_point(300.0, 0.0), ::double_point(100.0, 300.0));

   }


   void impact::_001OnNcPostDraw(::draw2d::graphics_pointer & pgraphics)
   {


   }


   ::user::document * impact::get_document()
   {

      return  (::user::impact::get_document());

   }


   void impact::on_layout(::draw2d::graphics_pointer & pgraphics)
   {

      auto rectangleX = this->rectangle();

      if (rectangleX.area() <= 0)
         return;

   }





} // namespace app_graphics3d_fuel



















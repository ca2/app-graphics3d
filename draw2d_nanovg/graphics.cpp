#include "framework.h"
#include "gpu_opengl/_gpu_opengl.h"
#include "_nanovg.h"
#include "draw2d.h"
#include "pen.h"
#include "font.h"
#include "brush.h"
#include "image.h"
#include "keep.h"
#include "path.h"
#include "color.h"
//
////  // Optional — depends on your conventions
//
#include "acme/parallelization/synchronous_lock.h"
#include "acme/parallelization/task.h"
#include "acme/platform/application.h"
#include "acme/platform/node.h"
#include "acme/prototype/geometry2d/_text_stream.h"
#include "acme/prototype/geometry2d/item.h"
#include "acme/prototype/mathematics/mathematics.h"
#include "acme/prototype/string/str.h"
#include "gpu_opengl/approach.h"
#include "gpu_opengl/renderer.h"
#include "gpu_opengl/texture.h"
#include "bred/gpu/context_lock.h"
#include "bred/gpu/cpu_buffer.h"
#include "bred/gpu/layer.h"
#include "bred/gpu/render.h"
#include "aura/graphics/draw2d/clip.h"
#include "aura/graphics/graphics/context.h"
#include "aura/graphics/image/drawing.h"
#include "aura/graphics/image/target.h"
#include "aura/graphics/write_text/font_enumeration_item.h"
#include "aura/user/user/interaction.h"
#include "bred/gpu/layer.h"
#include "bred/gpu/swap_chain.h"
#include "aura/graphics/write_text/text_out.h"
#include "aura/graphics/write_text/draw_text.h"
#include "acme/prototype/geometry2d/_defer_item.h"
#include "aura/graphics/write_text/_defer_geometry2d_item.h"

#if defined(WINDOWS_DESKTOP)
#include "windowing_win32/window.h"
#include <dwmapi.h>
#else
#include "aura/windowing/window.h"

#endif
#define NANOVG_GL3
#include <nanovg_gl.h>
#include <math.h>

////#include <vk/freeglut.h>
//#define VKAD_GLAPI_EXPORT

//#include "vkad_wgl.h"
////#define WGL_CONTEXT_FLAGS_ARB 0X2094
//#define WGL_CONTEXT_COREPROFILE_BIT_ARB 0x00000001
//#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126

//int  opengl_init();


//HGLRC initialize_opengl_version(HDC hdc, int iMajor, int iMinor);



namespace opengl
{

#include "line1.h"

}


#ifdef WINDOWS_DESKTOP


BOOL CALLBACK draw2d_nanovg_EnumFamCallBack(LPLOGFONT lplf, LPNEWTEXTMETRIC lpntm, unsigned int FontType, void * p);

#endif


class draw2d_nanovg_enum_fonts
{
public:


   ::write_text::font_enumeration_item_array& m_itema;


   draw2d_nanovg_enum_fonts(::write_text::font_enumeration_item_array& itema) :
      m_itema(itema)
   {

   }


};


namespace draw2d_nanovg
{


   static ::i64 performance_steady_nanoseconds()
   {

      return ::std::chrono::duration_cast<::std::chrono::nanoseconds>(
         ::std::chrono::steady_clock::now().time_since_epoch()).count();

   }


   graphics* thread_graphics()
   {

      return ::get_task()->payload("draw2d_nanovg::graphics").cast < graphics >();

   }


   void thread_graphics(graphics* pgraphics)
   {

      ::get_task()->payload("draw2d_nanovg::graphics") = pgraphics;

   }


   graphics::graphics()
   {

      m_bHadEndLayer = false;
      m_bHasCurrentPoint = false;
      //m_hrc = nullptr;
      //m_hwnd = nullptr;
      //m_hglrc = nullptr;
      //_m_bYFlip = true;
      m_pointTranslate = ::i32_point();
      m_bPrinting = false;
      m_pimageAlphaBlend = nullptr;
      m_size.set(0, 0);
      //m_hdc = nullptr;
      //m_hdcGraphics = nullptr;
      m_ewritetextrendering = ::write_text::e_rendering_anti_alias_grid_fit;
      //m_dFontFactor = 1.0;

   }


   //void graphics::assert_ok() const
   //{

   //   object::assert_ok();

   //}


   //void graphics::dump(dump_context& dumpcontext) const
   //{
   //   
   //   object::dump(dumpcontext);

   //   dumpcontext << "m_hdc = " << (iptr) m_hdc;
   //   dumpcontext << "\nm_hAttribDC = " << (iptr) m_hdc;
   //   dumpcontext << "\nm_bPrinting = " << m_bPrinting;

   //   dumpcontext << "\n";

   //}


   graphics::~graphics()
   {

      //opengl_delete_offscreen_buffer();

      DeleteDC();

   }

#ifdef _DEBUG

   long long graphics::increment_reference_count()
   {

      return ::particle::increment_reference_count();

   }


   long long graphics::decrement_reference_count()
   {

      return ::particle::decrement_reference_count();

   }


#endif

   //bool graphics::IsPrinting() const
   //{

   //   return m_bPrinting;

   //}


   bool graphics::CreateDC(const ::scoped_string& lpszDriverName, const ::scoped_string& lpszDeviceName, const char* lpszOutput, const void* lpInitData)
   {
      //return Attach(::CreateDC(lpszDriverName, lpszDeviceName, lpszOutput, (const DEVMODE*)lpInitData));
      return false;

   }


   bool graphics::CreateIC(const ::scoped_string& lpszDriverName, const ::scoped_string& lpszDeviceName, const char* lpszOutput, const void* lpInitData)
   {

      //return Attach(::CreateIC(lpszDriverName, lpszDeviceName, lpszOutput, (const DEVMODE*) lpInitData));
      return false;

   }


   void graphics::create_memory_graphics(const ::i32_size& size)
   {

      ::i32_size sizeMemory(size);

      if (sizeMemory.is_empty())
      {

         sizeMemory = { 1920, 1080 };

      }

      ::gpu::graphics::create_memory_graphics(sizeMemory);

   }


   void graphics::_create_memory_graphics(const ::i32_size& size)
   {

      auto puserinteraction = m_puserinteractionDraw2dGraphics;

      if (!puserinteraction)
      {

         puserinteraction = dynamic_cast < ::user::interaction * >(
            application()->m_pacmeuserinteractionMain.m_p);

      }

      if (!puserinteraction)
      {

         throw ::exception(
            error_wrong_state,
            "No main interaction is available for NanoVG memory graphics.");

      }

      auto pwindow = puserinteraction->window();

      if (!pwindow)
      {

         throw ::exception(
            error_wrong_state,
            "No window is available to acquire the OpenGL GPU device.");

      }

      m_puserinteractionDraw2dGraphics = puserinteraction;

      auto pgpuapproach = application()->get_gpu_approach();
      auto pgpudevice = pgpuapproach->get_gpu_device(pwindow);

      if (!pgpudevice)
      {

         throw ::exception(
            error_wrong_state,
            "Failed to acquire the OpenGL GPU device for NanoVG memory graphics.");

      }

      if (!context_lease())
      {

         auto contextlease = pgpudevice->acquire_draw2d_context(
            ::gpu::e_output_gpu_buffer,
            size);

         set_context_lease(::transfer(contextlease));

      }

      auto pgpucontextNew = gpu_context();

      if (!pgpucontextNew)
      {

         throw ::exception(
            error_wrong_state,
            "Failed to create the NanoVG offscreen GPU context.");

      }

      pgpucontextNew->m_pgpucompositor = this;

      m_sizeScaleOutput = { 1.0, -1.0 };
      m_pointTranslateOutput = { 0.0, (double)size.cy };
      m_size = size;
      m_sizeWindow = size;

      {

         ::gpu::context_lock contextlockNew(pgpucontextNew);

         pgpucontextNew->get_gpu_renderer();
         ::opengl::resize(size, false);

         if (!m_pdc)
         {

            m_pdc = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);

            if (!m_pdc)
            {

               throw ::exception(
                  error_failed,
                  "nvgCreateGL3 failed for NanoVG memory graphics.");

            }

         }

      }

   }


   void graphics::on_acquire_memory_graphics(
      ::image::image * pimage,
      const ::i32_size & size)
   {

      auto pgpucontext = gpu_context();

      if (!pgpucontext || !m_pdc)
      {

         context_lease().mark_damaged();

         throw ::exception(
            error_wrong_state,
            "NanoVG memory graphics has no warm GPU context.");

      }

      try
      {

         pgpucontext->send(
            [this, pgpucontext, pimage, size]()
            {

               ::gpu::graphics::on_acquire_memory_graphics(pimage, size);

               m_egraphics = ::e_graphics_draw;
               m_sizeScaleOutput = { 1.0, -1.0 };
               m_pointTranslateOutput = { 0.0, (double)size.cy };
               m_size = size;
               m_sizeWindow = size;

               ::gpu::context_lock contextlock(pgpucontext);

               pgpucontext->m_pgpucompositor = this;
               pgpucontext->on_resize(size);
               ::opengl::resize(size, false);

            });

      }
      catch (...)
      {

         context_lease().mark_damaged();

         throw;

      }

   }


   void graphics::on_release_memory_graphics()
   {

      auto pgpucontext = gpu_context();

      if (!pgpucontext)
      {

         context_lease().mark_damaged();

         throw ::exception(
            error_wrong_state,
            "NanoVG memory graphics lost its GPU context.");

      }

      try
      {

         pgpucontext->send(
            [this, pgpucontext]()
            {

               ::gpu::context_lock contextlock(pgpucontext);

               glFlush();
               ::opengl::check_error("");

               auto pgpuimage = dynamic_cast<::gpu::image *>(m_pimage);

               if (pgpuimage && pgpuimage->gpu_texture())
               {

                  pgpuimage->gpu_texture()->defer_fence();

               }

               ::gpu::graphics::on_release_memory_graphics();
               m_pimage = nullptr;

            });

      }
      catch (...)
      {

         context_lease().mark_damaged();

         throw;

      }

   }


   void graphics::create_window_graphics(::windowing::window* pwindow)
   {

      m_pwindow = pwindow;

      opengl_defer_create_window_context(pwindow);

      set_ok_flag();

   }


   void graphics::create_for_window_draw2d(::user::interaction* puserinteraction, const ::i32_size& size)
   {

      ::gpu::graphics::create_for_window_draw2d(puserinteraction, size);

      auto pwindow = puserinteraction->window();

      //opengl_defer_create_window_context(pwindow);

      auto psystem = system();

      auto pgpuapproach = application()->get_gpu_approach();

      auto pgpudevice = pgpuapproach->get_gpu_device(pwindow);

      auto pgpucontextNew = pgpudevice->main_draw2d_context();

      set_gpu_context(pgpucontextNew);

      auto pcontext = gpu_context();

      pcontext->m_pgpucompositor = this;

      pcontext->defer_create_window_context(pwindow);

      ::cast < ::gpu_opengl::context > pcontextOpengl = pcontext;
      ::cast < ::gpu_opengl::approach > papproachOpengl = pgpuapproach;

      ::gpu::context_lock contextlock(pcontext);

      //nanovg_device_create_info_t createinfo;
      //createinfo.samples = VK_SAMPLE_COUNT_1_BIT;
      //createinfo.deferredResolve = true;
      //createinfo.inst = papproachOpengl->m_vkinstance;
      //createinfo.phy = pcontextOpengl->m_pgpudevice->m_pphysicaldevice->m_physicaldevice;
      //createinfo.vkdev = pcontextOpengl->logicalDevice();
      //createinfo.qFamIdx = pcontextOpengl->m_pgpudevice->m_queuefamilyindexes.graphicsFamily;
      //createinfo.qIndex = 0;
      //createinfo.threadAware = false; /**< if true, mutex is created and guard device queue and caches access */

      if (m_pdc)
      {

         nvgDeleteGL3(m_pdc);

         m_pdc = nullptr;

      }

      m_pdc = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);

      if (!m_pdc)
      {

         throw ::exception(error_failed, "nvgCreateGL3 failed");

      }
      //m_pdc = nvgCreateGL3(NVG_DEBUG);

      //auto sizeWindow = pwindow->m_sizeWindow;

      //m_nanovgsurface = nanovg_surface_create(
      //   m_pdc,
      //   sizeWindow.cx,
      //   sizeWindow.cy
      //);

      //m_pdc = nanovg_create(m_nanovgsurface);
      //if (!m_pdc)
      //{

      //   throw ::exception(error_failed);

      //}

      defer_create_swap_chain(puserinteraction);

      //      ::opengl::resize(size);


      //if (m_papplication->m_gpu.m_bUseSwapChainWindow)
      //{

      //    auto pcontextMain = pgpudevice->main_context();

      //    auto pswapchain = pcontextMain->get_swap_chain();

      //    if (!pswapchain->m_bSwapChainInitialized)
      //    {

      //        pswapchain->initialize_swap_chain_window(pcontextMain, puserinteraction->window());

      //    }

      //}

      set_ok_flag();

   }


   void graphics::create_compatible_graphics(::draw2d::graphics* pgraphics)
   {

      ::gpu::graphics::create_compatible_graphics(pgraphics);
      //opengl_create_offscreen_buffer({ 920, 1080 });
      //opengl_create_offscreen_buffer(pgraphics->m_pimage->size());

   }


   //bool graphics::opengl_create_offscreen_buffer(const ::i32_size & sizePlacement)
   //{

   //   return opengl_create_offscreen_buffer(sizePlacement);

   //   //if (m_puserinteractionDraw2dGraphics)
   //   //{

   //   //   on_gpu_context_placement_change(rectanglePlacement, m_puserinteractionDraw2dGraphics->m_pacmewindowingwindow);
   //   //}
   //   //else
   //   //{

   //   //   on_gpu_context_placement_change(rectanglePlacement, nullptr);

   //   //}


   //   ////if (!draw2d_nanovg()->m_popenglcontext) {
   //   ////   informationf("MS GDI - RegisterClass failed");
   //   ////   informationf("last-error code: %d\n", GetLastError());
   //   ////   return false;
   //   ////}

   //   ////if (!m_pgpucontext)
   //   ////{

   //   ////   auto pgpuapproach = application()->get_gpu_approach();

   //   ////   if (!m_puserinteractionDraw2dGraphics)
   //   ////   {

   //   ////      m_puserinteractionDraw2dGraphics = dynamic_cast < ::user::interaction*>(application()->m_pacmeuserinteractionMain.m_p);

   //   ////   }

   //   ////   ASSERT(m_puserinteractionDraw2dGraphics);

   //   ////   auto pgpudevice = pgpuapproach->get_gpu_device();

   //   ////   m_pgpucontext = pgpudevice->start_cpu_buffer_context(this, {}, rectanglePlacement);

   //   ////}

   //   //auto pgpuapproach = application()->get_gpu_approach();

   //   //auto pgpudevice = pgpuapproach->get_gpu_device(m_puserinteractionDraw2dGraphics->m_pacmewindowingwindow);


   //   //::cast < ::gpu_opengl::context > pcontextOpengl = gpu_context();
   //   //::cast < ::gpu_opengl::approach > papproachOpengl = pgpuapproach;

   //   ////nanovg_device_create_info_t createinfo;
   //   ////createinfo.samples = VK_SAMPLE_COUNT_1_BIT;
   //   ////createinfo.deferredResolve = true;
   //   ////createinfo.inst = papproachOpengl->m_vkinstance;
   //   ////createinfo.phy = pcontextOpengl->m_pgpudevice->m_pphysicaldevice->m_physicaldevice;
   //   ////createinfo.vkdev = pcontextOpengl->logicalDevice();
   //   ////createinfo.qFamIdx = pcontextOpengl->m_pgpudevice->m_queuefamilyindexes.graphicsFamily;
   //   ////createinfo.qIndex = 0;
   //   ////createinfo.threadAware = false; /**< if true, mutex is created and guard device queue and caches access */

   //   ////m_pdc = nanovg_device_create(&createinfo);
   //   ////m_nanovgsurface = nanovg_surface_create(m_pdc, rectanglePlacement.width(),
   //   ////   rectanglePlacement.height());

   //   ////m_pdc = nanovg_create(m_nanovgsurface);
   //   ////if (!m_pgpucontext)
   //   ////{

   //   ////   return false;

   //   ////}


   //   ////      ::opengl::resize(size);


   //   ////}

   //   ////LPCTSTR lpClassName = L"draw2d_nanovg_offscreen_buffer_window";
   //   ////LPCTSTR lpWindowName = L"draw2d_nanovg_offscreen_buffer_window";
   //   //////unsigned int dwStyle = WS_CAPTION | WS_POPUPWINDOW; // | WS_VISIBLE
   //   ////unsigned int dwExStyle = 0;
   //   ////unsigned int dwStyle = WS_OVERLAPPEDWINDOW;
   //   ////dwStyle |= WS_POPUP;
   //   //////dwStyle |= WS_VISIBLE;
   //   //////dwStyle |= WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
   //   ////dwStyle &= ~WS_CAPTION;
   //   //////dwStyle = 0;
   //   ////dwStyle &= ~WS_THICKFRAME;
   //   ////dwStyle &= ~WS_BORDER;
   //   ////int x = 0;
   //   ////int y = 0;
   //   ////int nWidth = size.cx;
   //   ////int nHeight = size.cy;
   //   ////HWND hWndParent = nullptr;
   //   ////HMENU hMenu = nullptr;
   //   ///////HINSTANCE hInstance = psystem->m_hinstance;
   //   ////void * lpParam = nullptr;

   //   //////HWND window = CreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y,  nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
   //   ////HWND window = CreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, nullptr, lpParam);

   //   ////if (window == nullptr)
   //   ////{
   //   ////   informationf("MS GDI - CreateWindow failed");
   //   ////   informationf("last-error code: %d\n", GetLastError());
   //   ////   return false;
   //   ////}

   //   ////// create WGL context, make current

   //   ////PIXELFORMATDESCRIPTOR pixformat;
   //   ////int chosenformat;
   //   ////HDC hdc = GetDC(window);
   //   ////if (hdc == nullptr)
   //   ////{
   //   ////   informationf("MS GDI - GetDC failed");
   //   ////   informationf("last-error code: %d\n", GetLastError());
   //   ////   return false;
   //   ////}

   //   ////ZeroMemory(&pixformat, sizeof(pixformat));
   //   ////pixformat.nSize = sizeof(pixformat);
   //   ////pixformat.nVersion = 1;
   //   ////pixformat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
   //   ////pixformat.iPixelType = PFD_TYPE_RGBA;
   //   ////pixformat.cColorBits = 24;
   //   ////pixformat.cAlphaBits = 8;
   //   ////pixformat.cDepthBits = 24;
   //   ////pixformat.cStencilBits = 8;

   //   ////chosenformat = ChoosePixelFormat(hdc, &pixformat);
   //   ////if (chosenformat == 0)
   //   ////{
   //   ////   informationf("MS GDI - ChoosePixelFormat failed");
   //   ////   informationf("last-error code: %d\n", GetLastError());
   //   ////   return false;
   //   ////}

   //   ////bool spfok = SetPixelFormat(hdc, chosenformat, &pixformat);
   //   ////if (!spfok)
   //   ////{
   //   ////   informationf("MS GDI - SetPixelFormat failed");
   //   ////   informationf("last-error code: %d\n", GetLastError());
   //   ////   return false;
   //   ////}

   //   ////HGLRC hglrcTime = wglCreateContext(hdc);
   //   ////if (hglrcTime == nullptr)
   //   ////{
   //   ////   informationf("MS WGL - wglCreateContext failed");
   //   ////   informationf("last-error code: %d\n", GetLastError());
   //   ////   ReleaseDC(m_hwnd, m_hdc);
   //   ////   return false;
   //   ////}

   //   ////bool okMakeCurrent = wglMakeCurrent(hdc, hglrcTime);
   //   ////if (!okMakeCurrent)
   //   ////{
   //   ////   informationf("MS WGL - wglMakeCurrent failed");
   //   ////   informationf("last-error code: %d\n", GetLastError());
   //   ////   return false;
   //   ////}
   //   //////vkfwInit();
   //   ////// ... <snip> ... setup a window and a context
   //   ////
   //   ////auto wglCurrentContext = wglGetCurrentContext();

   //   ////// Load all Opengl functions using the vkfw loader function
   //   ////// If you use SDL you can use: https://wiki.libsdl.org/SDL_GL_GetProcAddress
   //   //////if (!vkadLoadGLLoader((VKADloadproc)vkfwGetProcAddress)) {
   //   //////   std::cout << "Failed to initialize Opengl context" << std::endl;
   //   //////   return -1;
   //   //////}
   //   ////if (!vkadLoadWGL(hdc))
   //   ////{
   //   ////   // Problem: vkewInit failed, something is seriously wrong.
   //   ////   informationf("vkadLoadWGL failed");
   //   ////   //return false;
   //   ////   //throw resource_exception();

   //   ////   return false;

   //   ////}
   //   ////int attribs[] =
   //   ////{
   //   ////   WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
   //   ////   WGL_CONTEXT_MINOR_VERSION_ARB, 1,
   //   ////   WGL_CONTEXT_FLAGS_ARB, 0,
   //   ////   WGL_CONTEXT_PROFILE_MASK_ARB,
   //   ////   WGL_CONTEXT_COREPROFILE_BIT_ARB, 0
   //   ////};

   //   //////PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
   //   //////wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

   //   ////auto hglrc =  wglCreateContextAttribsARB(hdc, 0, attribs);
   //   ////wglMakeCurrent(nullptr, nullptr);
   //   ////wglDeleteContext(hglrcTime);
   //   ////   wglMakeCurrent(hdc, m_hglrc);
   //   //////draw2d_nanovg()->defer_initialize_glew();
   //   ////
   //   //////draw2d_nanovg()->defer_initialize_glew();


   //   ////m_hwnd = window;
   //   ////m_hdc = hdc;
   //   ////m_hglrc = hglrc;
   //   ////m_size = size;

   //   //bool bYSwap = m_papplication->m_gpu.m_bUseSwapChainWindow;

   //   //::opengl::resize(rectanglePlacement.size(), bYSwap);

   //   //return true;

   //}


   //bool graphics::opengl_delete_offscreen_buffer()
   //{

   //   //if (m_hglrc == NULL && m_hdc == NULL && m_hwnd == NULL)
   //   //{

   //   //   return true;

   //   //}

   //   //wglMakeCurrent(nullptr, nullptr);
   //   //wglDeleteContext(m_hglrc);
   //   //::ReleaseDC(m_hwnd, m_hdc);
   //   //::DestroyWindow(m_hwnd);
   //   m_size.set(0, 0);
   //   //m_hglrc = NULL;
   //   //m_hwnd = NULL;
   //   //m_hdc = NULL;
   //   return true;

   //}


   bool graphics::opengl_defer_create_window_context(::windowing::window* pwindow)
   {

      auto psystem = system();

      auto pgpuapproach = application()->get_gpu_approach();

      auto pgpudevice = pgpuapproach->get_gpu_device(m_puserinteractionDraw2dGraphics->m_pacmewindowingwindow);

      auto pgpucontext = pgpudevice->main_context();

      ////if (!m_pgpucontext)
      ////{

      ////   m_pgpucontext = pgpudevice->start_swap_chain_context(this, pwindow);

      ////}


      //pgpucontext->defer_create_window_context(pwindow);

      ::cast < ::gpu_opengl::context > pcontextOpengl = pgpucontext;
      ::cast < ::gpu_opengl::approach > papproachOpengl = pgpuapproach;

      //nanovg_device_create_info_t createinfo;
      //createinfo.samples = VK_SAMPLE_COUNT_1_BIT;
      //createinfo.deferredResolve = true;
      //createinfo.inst = papproachOpengl->m_vkinstance;
      //createinfo.phy = pcontextOpengl->m_pgpudevice->m_pphysicaldevice->m_physicaldevice;
      //createinfo.vkdev = pcontextOpengl->logicalDevice();
      //createinfo.qFamIdx = pcontextOpengl->m_pgpudevice->m_queuefamilyindexes.graphicsFamily;
      //createinfo.qIndex = 0;
      //createinfo.threadAware = false; /**< if true, mutex is created and guard device queue and caches access */

      //m_pdc = nanovg_device_create(&createinfo);
      //m_nanovgsurface = nanovg_surface_create(m_pdc, pwindow->m_sizeWindow.cx,
      //   pwindow->m_sizeWindow.cy);

      //m_pdc = nanovg_create(m_nanovgsurface);
      //if (!m_pgpucontext)
      //{

      //   return false;

      //}


      //      ::opengl::resize(size);

      return true;

   }



   int graphics::ExcludeUpdateRgn(::user::interaction_base* pwindow)
   {
      // ASSERT(m_hdc != nullptr);
      //throw ::interface_only();
      ////return ::ExcludeUpdateRgn(m_hdc, WIN_WINDOW(pwindow)->get_handle());
      return 0;
   }

   //double graphics::GetDeviceCaps(double nIndex) const
   //{
   //   // ASSERT(m_hdc != nullptr);
   //   //return ::GetDeviceCaps(m_hdc, nIndex);
   //   return 0;
   //}

   i32_point graphics::GetBrushOrg() const
   {
      // ASSERT(m_hdc != nullptr);
      ::i32_point point;
      //VERIFY(::GetBrushOrgEx(m_hdc, &point));
      return point;
   }

   i32_point graphics::SetBrushOrg(int x, int y)
   {
      // ASSERT(m_hdc != nullptr);
      ::i32_point point;
      //VERIFY(::SetBrushOrgEx(m_hdc, x, y, &point));
      return point;
   }

   i32_point graphics::SetBrushOrg(const ::i32_point& point)
   {
      // ASSERT(m_hdc != nullptr);
      //VERIFY(::SetBrushOrgEx(m_hdc, point.x, point.y, &point));
      return point;
   }


   // int graphics::EnumObjects(int nObjectType, int (CALLBACK* lpfn)(void *, LPARAM), LPARAM lpData)
   // {
   //    // ASSERT(m_hdc != nullptr);
   //    //return ::EnumObjects(m_hdc, nObjectType, (GOBJENUMPROC)lpfn, lpData);
   //    return 0;
   // }

   ::draw2d::bitmap* graphics::SelectObject(::draw2d::bitmap* pbitmap)
   {

      if (m_pbitmap == pbitmap)
      {

         return m_pbitmap;

      }

      create_memory_graphics(pbitmap->get_size());

      //vkClear(VK_COLOR_BUFFER_BIT | VK_DEPTH_BUFFER_BIT);

      m_pbitmap = pbitmap;

      return m_pbitmap;

   }


   ::draw2d::object* graphics::SelectObject(::draw2d::object* pObject)
   {
      /*      // ASSERT(m_hdc != nullptr);
         if(pObject == nullptr)
            return nullptr;
         return SelectGdiObject(get_app(), m_hdc, pObject->get_os_data()); */
      return nullptr;
   }

   //HGDIOBJ graphics::SelectObject(HGDIOBJ hObject) // Safe for nullptr handles
   //{

   //   unsigned int uType = GetObjectType(hObject);

   //   if(uiType == OBJ_BITMAP)
   //   {

   //      HBITMAP hbitmap = (HBITMAP) hObject;

   //      if(m_pbitmap.is_null())
   //         m_pbitmap.create();

   //      if(m_pbitmap.is_null())
   //         return nullptr;

   //      //(dynamic_cast < ::draw2d_nanovg::bitmap * > (m_pbitmap.m_p))->m_pbitmap = ___new plusplus::Bitmap (hbitmap, nullptr);

   //      //if(m_pgraphics != nullptr)
   //      //{

   //      //   try
   //      //   {

   //      //      delete m_pgraphics;

   //      //   }
   //      //   catch(...)
   //      //   {

   //      //      informationf("graphics::SelectObject(HGDIOBJ) OBJ_BITMAP : Failed to delete plusplus::Graphics");

   //      //   }

   //      //   m_pgraphics = nullptr;

   //      //}

   //      //m_pgraphics = allocateø< plusplus::Graphics((plusplus::Bitmap *) m_pbitmap->get_os_data >());

   //      //m_pgraphics->SetPageUnit(plusplus::UnitPixel);

   //      //set_text_rendering_hint(::write_text::e_rendering_anti_alias_grid_fit);

   //      return hbitmap;

   //   }

   //   //*ASSERT(m_hdc == m_hdc); // ASSERT a simple graphics object
   //   //return (hObject != nullptr) ? ::SelectObject(m_hdc, hObject) : nullptr; */
   //   return nullptr;
   //}


   color32_t graphics::GetNearestColor(color32_t crColor) const
   {

      //return ::GetNearestColor(m_hdc, crColor);
      return color::transparent;

   }


   unsigned int graphics::RealizePalette()
   {
      //return ::RealizePalette(m_hdc);
      return 0;
   }

   void graphics::UpdateColors()
   {
      //::UpdateColors(m_hdc);
   }


   int graphics::GetPolyFillMode() const
   {
      //return ::GetPolyFillMode(m_hdc);
      return 0;
   }


   int graphics::GetROP2() const
   {
      //return ::GetROP2(m_hdc);
      return 0;
   }


   int graphics::GetStretchBltMode() const
   {
      //return ::GetStretchBltMode(m_hdc);
      return 0;
   }


   int graphics::GetMapMode() const
   {
      //return ::GetMapMode(m_hdc);
      return 0;
   }


   int graphics::GetGraphicsMode() const
   {
      //return ::GetGraphicsMode(m_hdc);
      return 0;
   }


   // bool graphics::GetWorldTransform(XFORM* pXform) const
   // {
   //
   //
   //
   //    //m_pgraphics->GetTransform(((graphics *)this)->m_pm);
   //
   //    //plusplus::REAL ::f64_rectangle[6];
   //
   //    //m_pm->GetElements(rectangle);
   //
   //    //pXform->eM11 = ::f64_rectangle[0];
   //    //pXform->eM12 = ::f64_rectangle[1];
   //    //pXform->eM21 = ::f64_rectangle[2];
   //    //pXform->eM22 = ::f64_rectangle[3];
   //
   //    //pXform->eDx = ::f64_rectangle[4];
   //    //pXform->eDy = ::f64_rectangle[5];
   //
   //
   //    return true;
   //
   // }

   i32_size graphics::get_context_extents() const
   {
      ::i32_size size;
      //::Get_wiewportExtEx(m_hdc, &size);
      return size;
   }

   i32_point graphics::GetWindowOrg() const
   {
      ::i32_point point;
      //::GetWindowOrgEx(m_hdc, &point);
      return point;
   }

   i32_size graphics::GetWindowExt() const
   {
      ::i32_size size;
      //::GetWindowExtEx(m_hdc, &size);
      return size;
   }


   //// non-virtual helpers calling virtual mapping functions
   //i32_point graphics::set_origin(const ::i32_point& point)
   //{

   //   return set_origin(point.x, point.y);

   //}

   //i32_size graphics::set_context_extents(const ::i32_size & size)
   //{
   //   return set_context_extents(size.cx, size.cy);
   //}

   i32_point graphics::SetWindowOrg(const ::i32_point& point)
   {
      return SetWindowOrg(point.x, point.y);
   }

   i32_size graphics::set_window_ext(const ::i32_size& size)
   {
      return set_window_ext(size.cx, size.cy);
   }

   void graphics::DPtoLP(::f64_point* lpPoints, ::collection::count nCount)
   {
      //::DPtoLP(m_hdc, lpPoints, (int) nCount);
   }

   void graphics::DPtoLP(::f64_rectangle* prectangle)
   {
      //::DPtoLP(m_hdc, (::f64_point *)rectangle, 2);
   }

   void graphics::LPtoDP(::f64_point* lpPoints, ::collection::count nCount)
   {
      //::LPtoDP(m_hdc, lpPoints, (int)  nCount);
   }

   void graphics::LPtoDP(::f64_rectangle* prectangle)
   {
      //::LPtoDP(m_hdc, (::f64_point *)rectangle, 2);
   }

   bool graphics::FillRgn(::draw2d::region* pRgn, ::draw2d::brush* pBrush)
   {

      //return ::FillRgn(m_hdc, (HRGN)pRgn->get_os_data(), (HBRUSH)pBrush->get_os_data()) != false;
      return false;

   }


   bool graphics::FrameRgn(::draw2d::region* pRgn, ::draw2d::brush* pBrush, int nWidth, int nHeight)
   {

      //return ::FrameRgn(m_hdc, (HRGN)pRgn->get_os_data(), (HBRUSH)pBrush->get_os_data(), nWidth, nHeight) != false;
      return false;

   }

   bool graphics::InvertRgn(::draw2d::region* pRgn)
   {

      // ASSERT(m_hdc != nullptr);

      //return ::InvertRgn(m_hdc, (HRGN)pRgn->get_os_data()) != false;
      return false;

   }

   //bool graphics::PaintRgn(::draw2d::region* pRgn)
   //{

   //   // ASSERT(m_hdc != nullptr);

   //   //return ::PaintRgn(m_hdc, (HRGN)pRgn->get_os_data())  != false;

   //   return ::draw2d::graphics::PaintRgn(pRgn);

   //}


   bool graphics::PtVisible(int x, int y) const
   {

      // ASSERT(m_hdc != nullptr);

      //return ::PtVisible(m_hdc, x, y) != false;
      //return ::draw2d::graphics::PtVisible(x, y);

      return true;

   }


   bool graphics::PtVisible(const ::i32_point& point) const
   {

      // ASSERT(m_hdc != nullptr);   // call virtual
      return PtVisible(point.x, point.y);

   }


   //bool graphics::rectVisible(const ::f64_rectangle & ::f64_rectangle) const
   //{

   //   // ASSERT(m_hdc != nullptr);

   //   //return ::rectVisible(m_hdc, &rectangle) != false;
   //   return false;

   //}


   //i32_point graphics::GetCurrentPosition() const
   //{
   //   // ASSERT(m_hdc != nullptr);
   //   //::i32_point point(m_po;
   //   //VERIFY(::GetCurrentPositionEx(m_hdc, &point));
   //   //return point;
   //   return ::draw2d::graphics::G;
   //}

   void graphics::polyline(const ::f64_point* lpPoints, ::collection::count nCount)
   {

      if (nCount <= 0)
      {

         //return true;

         return;

      }

      bool bOk1 = false;

      //plusplus::Point * ppoints = ___new plusplus::Point[nCount];

      //try
      //{

      //   for(double i = 0; i < nCount; i++)
      //   {
      //      ppoints[i].X = lpPoints[i].x;
      //      ppoints[i].Y = lpPoints[i].y;
      //   }

      //   bOk1 = m_pgraphics->DrawLines(vk2d_pen(),ppoints,(::double) nCount) == plusplus::Status::Ok;

      //}
      //catch(...)
      //{
      //}

      //try
      //{
      //   delete ppoints;
      //}
      //catch(...)
      //{
      //}


      //return bOk1;
   }


   void graphics::arc(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      double centerx = (x2 + x1) / 2.0;

      double centery = (y2 + y1) / 2.0;

      double radiusx = abs(x2 - x1) / 2.0;

      double radiusy = abs(y2 - y1) / 2.0;

      if (radiusx == 0.0 || radiusy == 0.0)
      {

         throw ::exception(error_invalid_empty_argument);

      }

      double start = atan2(y3 - centery, x3 - centerx);

      double end = atan2(y4 - centery, x4 - centerx);

      nanovg_keep keep(m_pdc);

      nvgTranslate(m_pdc, (float) centerx, (float)centery);

      nvgScale(m_pdc, (float)radiusx, (float)radiusy);

      nvgArc(m_pdc, 0.0f, 0.0f, 1.0f, (float)start, (float)end, NVG_CCW);

      draw();

   }


   //void graphics::arc(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4)
   //{

   //   double centerx    = (x2 + x1) / 2.0;
   //   double centery    = (y2 + y1) / 2.0;

   //   double start      = atan2(y3 - centery,x3 - centerx) * 180.0 / π;
   //   double end        = atan2(y4 - centery,x4 - centerx) * 180.0 / π;
   //   double sweep      = fabs(end - start);

   //   /*if(GetArcDirection() == AD_COUNTERCLOCKWISE)
   //   {
   //      sweep = -sweep;
   //   }
   //   */

   //   return Arc(x1,y1, x2-x1, y2-y1, start, sweep);

   //}


   void graphics::arc(double x1, double y1, double w, double h, double start, double extends)
   {

      //::plusplus::f64_rectangle ::f64_rectangle(x1,y1,w,h);

      //return m_pgraphics->DrawArc(vk2d_pen(),rectangle,(plusplus::REAL) start,(plusplus::REAL) extends) == plusplus::Status::Ok;

      //return true;

   }


   //void graphics::arc(double x1,double y1,double w,double h,double start,double extends)
   //{

   //   //::plusplus::rectF float_rectangle((plusplus::REAL) x1,(plusplus::REAL) y1,(plusplus::REAL) w,(plusplus::REAL) h);

   //   //return m_pgraphics->DrawArc(vk2d_pen(),float_rectangle,(plusplus::REAL) start,(plusplus::REAL) extends) == plusplus::Status::Ok;

   //   return true;

   //}


   bool graphics::fill(::draw2d::brush* pbrush, double xOrg, double yOrg)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      if (pbrush == nullptr || pbrush->m_ebrush == ::draw2d::e_brush_null)
      {

         return true;

      }

      if (_fill1(pbrush, xOrg, yOrg))
      {

         return false;

      }

      nvgFill(m_pdc);

      _fill2(pbrush, xOrg, yOrg);

      return true;

   }


   bool graphics::_fill1(::draw2d::brush* pbrush, double xOrg, double yOrg)
   {

      if (pbrush == nullptr || pbrush->m_ebrush == ::draw2d::e_brush_null)
      {

         return true;

      }

      //nanovg todo if (m_pregion.is_set() && !m_pregion.cast<region>()->is_simple_positive_region())
      //{

      //   nanovg_set_antialias(m_pdc, VKVG_ANTIALIAS_BEST);

      //   nanovg_push_group(m_pdc);

      //   _set(pbrush, xOrg, yOrg);

      //}
      //else
      if (pbrush->m_ebrush == ::draw2d::e_brush_linear_gradient_point_color)
      {

         float x0 = (float)(pbrush->m_point1.x - xOrg);

         float y0 = (float)(pbrush->m_point1.y - yOrg);

         float x1 = (float)(pbrush->m_point2.x - xOrg);

         float y1 = (float)(pbrush->m_point2.y - yOrg);

         //float xform[6];

         //nvgCurrentTransform(m_pdc, xform);

         //nvgTransformPoint(&x0, &y0, xform, x0, y0);
         //
         //nvgTransformPoint(&x1, &y1, xform, x1, y1);

         // Create a linear gradient paint
         NVGpaint gradient = nvgLinearGradient(
            m_pdc,
            x0, y0,         // start point (top of the rectangle)
            x1, y1,     // end point (bottom of the rectangle)
            as_nvg_color(pbrush->m_color1),    // start color (red)
            as_nvg_color(pbrush->m_color2)     // end color (blue)
         );

         nvgFillPaint(m_pdc, gradient);



         //VkvgPattern ppattern = nanovg_pattern_create_linear(x0, y0, x1, y1);

         //nanovg_pattern_add_color_stop(ppattern, 0., __expand_double_rgba(pbrush->m_color1));

         //nanovg_pattern_add_color_stop(ppattern, 1., __expand_double_rgba(pbrush->m_color2));

         return false;

      }
      else
      {

         _set(pbrush, xOrg, yOrg);

      }

      return false;

   }


   bool graphics::_set(::draw2d::brush* pbrush, double x, double y)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      if (pbrush->m_ebrush == ::draw2d::e_brush_radial_gradient_color)
      {

         auto radius = maximum(::abs(pbrush->m_size.cx), ::abs(pbrush->m_size.cy)) / 2.0;

         if (radius <= 0.0)
         {

            nvgFillColor(m_pdc, as_nvg_color(pbrush->m_color1));

         }
         else
         {

            auto paint = nvgRadialGradient(
               m_pdc,
               (float)(pbrush->m_point.x - x),
               (float)(pbrush->m_point.y - y),
               0.f,
               (float)radius,
               as_nvg_color(pbrush->m_color1),
               as_nvg_color(pbrush->m_color2));

            nvgFillPaint(m_pdc, paint);

         }

      }
      else if (pbrush->m_ebrush == ::draw2d::e_brush_linear_gradient_point_color)
      {

         //double x0 = pbrush->m_point1.x - x;

         //double y0 = pbrush->m_point1.y - y;

         //double x1 = pbrush->m_point2.x - x;

         //double y1 = pbrush->m_point2.y - y;

         //VkvgPattern ppattern = nanovg_pattern_create_linear(x0, y0, x1, y1);

         //nanovg_pattern_add_color_stop(ppattern, 0., __expand_double_rgba(pbrush->m_color1));

         //nanovg_pattern_add_color_stop(ppattern, 1., __expand_double_rgba(pbrush->m_color2));

         //nanovg_set_source(m_pdc, ppattern);

         //// nanovg_pattern_destroy(ppattern);


      }
      //else if (pbrush->m_ebrush == ::draw2d::e_brush_box_gradient)
      //{

      //   f64_rectangle outer(pbrush->m_point, pbrush->m_size);
      //   f64_rectangle inner(outer);
      //   inner.deflate(pbrush->m_dRadius);
      //   double K = 0.5522847498; // For HalfPi arc (90 degrees)
      //   double KR = K * pbrush->m_dRadius;


      //   //https://stackoverflow.com/questions/734076/how-to-best-approximate-a-geometrical-arc-with-a-bezier-curve
      //   //p0 = [0, radius]
      //   //p1 = [radius * K, radius]
      //   //p2 = [radius, radius * K]
      //   //p3 = [radius, 0]
      //   //where K is a so-called "magic number", which is an non-rational number. It can be approximated as follows:
      //   //K = 0.5522847498
      //   //https://stackoverflow.com/users/615243/nic

      //   // clockwise top-right
      //   //p0 = [innerright, outertop]
      //   //p1 = [innerright + radius * K, outertop]
      //   //p2 = [outerright, innertop - radius * K]
      //   //p3 = [outerright, innertop]
      //   //center = innerright, innertop


      //   VkvgPattern ppattern = nanovg_pattern_create_mesh();

      //   /* Add a Coons patch */
      //   nanovg_mesh_pattern_begin_patch(ppattern);
      //   nanovg_mesh_pattern_move_to(ppattern, inner.right, inner.top);
      //   nanovg_mesh_pattern_line_to(ppattern, inner.right, outer.top);
      //   nanovg_mesh_pattern_curve_to(ppattern, inner.right + KR, outer.top, outer.right, inner.top - KR, outer.right,
      //      inner.top);
      //   nanovg_mesh_pattern_line_to(ppattern, inner.right, inner.top);
      //   //nanovg_mesh_pattern_curve_to (pattern, 60,  30, 130,  60, 100, 100);
      //   //nanovg_mesh_pattern_curve_to (pattern, 60,  70,  30, 130,   0, 100);
      //   //nanovg_mesh_pattern_curve_to (pattern, 30,  70, -30,  30,   0, 0);
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 0, __expand_double_rgba(pbrush->m_color1));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 1, __expand_double_rgba(pbrush->m_color2));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 2, __expand_double_rgba(pbrush->m_color2));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 3, __expand_double_rgba(pbrush->m_color1));
      //   nanovg_mesh_pattern_end_patch(ppattern);
      //   int iStatus = nanovg_pattern_status(ppattern);


      //   nanovg_mesh_pattern_begin_patch(ppattern);
      //   nanovg_mesh_pattern_move_to(ppattern, inner.right, inner.top);
      //   nanovg_mesh_pattern_line_to(ppattern, outer.right, inner.top);
      //   nanovg_mesh_pattern_line_to(ppattern, outer.right, inner.bottom);
      //   nanovg_mesh_pattern_line_to(ppattern, inner.right, inner.bottom);
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 0, __expand_double_rgba(pbrush->m_color1));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 1, __expand_double_rgba(pbrush->m_color2));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 2, __expand_double_rgba(pbrush->m_color2));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 3, __expand_double_rgba(pbrush->m_color1));
      //   nanovg_mesh_pattern_end_patch(ppattern);

      //   ///* Add a Coons patch */
      //   //      nanovg_mesh_pattern_begin_patch (pattern);
      //   //      nanovg_mesh_pattern_move_to (pattern, 0, 0);
      //   //      nanovg_mesh_pattern_curve_to (pattern, 30, -30,  60,  30, 100, 0);
      //   //      nanovg_mesh_pattern_curve_to (pattern, 60,  30, 130,  60, 100, 100);
      //   //      nanovg_mesh_pattern_curve_to (pattern, 60,  70,  30, 130,   0, 100);
      //   //      nanovg_mesh_pattern_curve_to (pattern, 30,  70, -30,  30,   0, 0);
      //   //      nanovg_mesh_pattern_set_corner_color_rgb (pattern, 0, 1, 0, 0);
      //   //      nanovg_mesh_pattern_set_corner_color_rgb (pattern, 1, 0, 1, 0);
      //   //      nanovg_mesh_pattern_set_corner_color_rgb (pattern, 2, 0, 0, 1);
      //   //      nanovg_mesh_pattern_set_corner_color_rgb (pattern, 3, 1, 1, 0);
      //   //      nanovg_mesh_pattern_end_patch (pattern);

      //   ///* Add a Gouraud-shaded triangle */
      //   //      nanovg_mesh_pattern_begin_patch (pattern)
      //   //      nanovg_mesh_pattern_move_to (pattern, 100, 100);
      //   //      nanovg_mesh_pattern_line_to (pattern, 130, 130);
      //   //      nanovg_mesh_pattern_line_to (pattern, 130,  70);
      //   //      nanovg_mesh_pattern_set_corner_color_rgb (pattern, 0, 1, 0, 0);
      //   //      nanovg_mesh_pattern_set_corner_color_rgb (pattern, 1, 0, 1, 0);
      //   //      nanovg_mesh_pattern_set_corner_color_rgb (pattern, 2, 0, 0, 1);
      //   //      nanovg_mesh_pattern_end_patch (pattern)


      //            // clockwise bottom-right
      //            //p0 = [outerright, innerbottom]
      //            //p1 = [outerright, innerbottom + radius * K]
      //            //p2 = [innerright + radius * K, outerbottom]
      //            //p3 = [innerbottom, outerbottom]
      //            //center = innerright, innerbottom


      //   nanovg_mesh_pattern_begin_patch(ppattern);
      //   nanovg_mesh_pattern_move_to(ppattern, inner.right, inner.bottom);
      //   nanovg_mesh_pattern_line_to(ppattern, outer.right, inner.bottom);
      //   nanovg_mesh_pattern_curve_to(ppattern, outer.right, inner.bottom + KR, inner.right + KR, outer.bottom,
      //      inner.right, outer.bottom);
      //   nanovg_mesh_pattern_line_to(ppattern, inner.right, inner.bottom);
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 0, __expand_double_rgba(pbrush->m_color1));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 1, __expand_double_rgba(pbrush->m_color2));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 2, __expand_double_rgba(pbrush->m_color2));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 3, __expand_double_rgba(pbrush->m_color1));
      //   nanovg_mesh_pattern_end_patch(ppattern);


      //   nanovg_mesh_pattern_begin_patch(ppattern);
      //   nanovg_mesh_pattern_move_to(ppattern, inner.right, inner.bottom);
      //   nanovg_mesh_pattern_line_to(ppattern, inner.right, outer.bottom);
      //   nanovg_mesh_pattern_line_to(ppattern, inner.left, outer.bottom);
      //   nanovg_mesh_pattern_line_to(ppattern, inner.left, inner.bottom);
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 0, __expand_double_rgba(pbrush->m_color1));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 1, __expand_double_rgba(pbrush->m_color2));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 2, __expand_double_rgba(pbrush->m_color2));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 3, __expand_double_rgba(pbrush->m_color1));
      //   nanovg_mesh_pattern_end_patch(ppattern);


      //   nanovg_mesh_pattern_begin_patch(ppattern);
      //   nanovg_mesh_pattern_move_to(ppattern, inner.left, inner.top);
      //   nanovg_mesh_pattern_line_to(ppattern, inner.right, inner.top);
      //   nanovg_mesh_pattern_line_to(ppattern, inner.right, inner.bottom);
      //   nanovg_mesh_pattern_line_to(ppattern, inner.left, inner.bottom);
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 0, __expand_double_rgba(pbrush->m_color1));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 1, __expand_double_rgba(pbrush->m_color1));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 2, __expand_double_rgba(pbrush->m_color1));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 3, __expand_double_rgba(pbrush->m_color1));
      //   nanovg_mesh_pattern_end_patch(ppattern);

      //   // clockwise bottom-left
      //   //p0 = [innerleft, outerbottom]
      //   //p1 = [innerleft - radius * K, outerbottom]
      //   //p2 = [outerleft, innerbottom + radius * K]
      //   //p3 = [outerleft, innerbottom]
      //   //center = innerleft, innerbottom


      //   nanovg_mesh_pattern_begin_patch(ppattern);
      //   nanovg_mesh_pattern_move_to(ppattern, inner.left, inner.bottom);
      //   nanovg_mesh_pattern_line_to(ppattern, inner.left, outer.bottom);
      //   nanovg_mesh_pattern_curve_to(ppattern, inner.left - KR, outer.bottom, outer.left, inner.bottom + KR, outer.left,
      //      inner.bottom);
      //   nanovg_mesh_pattern_line_to(ppattern, inner.left, inner.bottom);
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 0, __expand_double_rgba(pbrush->m_color1));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 1, __expand_double_rgba(pbrush->m_color2));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 2, __expand_double_rgba(pbrush->m_color2));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 3, __expand_double_rgba(pbrush->m_color1));
      //   nanovg_mesh_pattern_end_patch(ppattern);


      //   nanovg_mesh_pattern_begin_patch(ppattern);
      //   nanovg_mesh_pattern_move_to(ppattern, inner.left, inner.top);
      //   nanovg_mesh_pattern_line_to(ppattern, outer.left, inner.top);
      //   nanovg_mesh_pattern_line_to(ppattern, outer.left, inner.bottom);
      //   nanovg_mesh_pattern_line_to(ppattern, inner.left, inner.bottom);
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 0, __expand_double_rgba(pbrush->m_color1));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 1, __expand_double_rgba(pbrush->m_color2));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 2, __expand_double_rgba(pbrush->m_color2));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 3, __expand_double_rgba(pbrush->m_color1));
      //   nanovg_mesh_pattern_end_patch(ppattern);


      //   // clockwise top-left
      //   //p0 = [outerleft, innertop]
      //   //p1 = [outerleft, innertop - radius * K]
      //   //p2 = [innerleft - radius * K, outertop]
      //   //p3 = [innerleft, outertop]
      //   //center = innerleft, innertop




      //   nanovg_mesh_pattern_begin_patch(ppattern);
      //   nanovg_mesh_pattern_move_to(ppattern, inner.left, inner.top);
      //   nanovg_mesh_pattern_line_to(ppattern, outer.left, inner.top);
      //   nanovg_mesh_pattern_curve_to(ppattern, outer.left, inner.top - KR, inner.left - KR, outer.top, inner.left,
      //      outer.top);
      //   nanovg_mesh_pattern_line_to(ppattern, inner.left, inner.top);
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 0, __expand_double_rgba(pbrush->m_color1));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 1, __expand_double_rgba(pbrush->m_color2));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 2, __expand_double_rgba(pbrush->m_color2));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 3, __expand_double_rgba(pbrush->m_color1));
      //   nanovg_mesh_pattern_end_patch(ppattern);


      //   nanovg_mesh_pattern_begin_patch(ppattern);
      //   nanovg_mesh_pattern_move_to(ppattern, inner.left, inner.top);
      //   nanovg_mesh_pattern_line_to(ppattern, inner.left, outer.top);
      //   nanovg_mesh_pattern_line_to(ppattern, inner.right, outer.top);
      //   nanovg_mesh_pattern_line_to(ppattern, inner.right, inner.top);
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 0, __expand_double_rgba(pbrush->m_color1));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 1, __expand_double_rgba(pbrush->m_color2));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 2, __expand_double_rgba(pbrush->m_color2));
      //   nanovg_mesh_pattern_set_corner_color_rgba(ppattern, 3, __expand_double_rgba(pbrush->m_color1));
      //   nanovg_mesh_pattern_end_patch(ppattern);


      //   nanovg_set_source(m_pdc, ppattern);


      //}
      //else if (pbrush->m_ebrush == ::draw2d::e_brush_pattern)
      //{

      //   if (pbrush->m_pimage.nok())
      //   {

      //      return false;

      //   }

      //   nanovg_surface_t* psurface = nanovg_get_target((nanovg_t*)pbrush->m_pimage->g()->get_os_data());

      //   if (psurface == nullptr)
      //   {

      //      return false;

      //   }

      //   VkvgPattern ppattern = nanovg_pattern_create_for_surface(psurface);

      //   nanovg_status_t status = nanovg_pattern_status(ppattern);

      //   if (status == VKVG_STATUS_SUCCESS)
      //   {

      //      nanovg_pattern_set_extend(ppattern, VKVG_EXTEND_REPEAT);

      //      nanovg_set_source(m_pdc, ppattern);

      //   }

      //   nanovg_pattern_destroy(ppattern);

      //   if (status != VKVG_STATUS_SUCCESS)
      //   {

      //      return false;

      //   }

      //}
      else
      {

         nvgFillColor(m_pdc, as_nvg_color(pbrush->m_color));

      }

      return true;

   }



   bool graphics::_fill2(::draw2d::brush* pbrush, double xOrg, double yOrg)
   {

      if (pbrush == nullptr || pbrush->m_ebrush == ::draw2d::e_brush_null)
      {

         return true;

      }

      //nanovg todo if (m_pregion.is_set() && !m_pregion.cast<region>()->is_simple_positive_region())
      //{

      //   nanovg_pop_group_to_source(m_pdc);

      //   m_pregion.cast<region>()->mask_fill(m_pdc);

      //}

      return true;

   }


   void graphics::fill_rectangle(const ::f64_rectangle& rectangle, const ::color::color& color)
   {

      ::draw2d::graphics::fill_rectangle(rectangle, color);

   }


   void graphics::fill_rectangle(const ::f64_rectangle& rectangle, ::draw2d::brush* pbrush)
   {

      if (!m_pdc)
      {

         throw ::exception(error_wrong_state);

      }

      nvgBeginPath(m_pdc);

      nvgRect(m_pdc,
         (float)rectangle.left,
         (float)rectangle.top,
         (float)(rectangle.right - rectangle.left),
         (float)(rectangle.bottom - rectangle.top));

      fill(pbrush);
      //vkBegin(VK_QUADS);

      //set(pbrush);

      //::opengl::vertex2f(rectangle);

      //vkEnd();

      ////return false;

   }



   void graphics::_set(const ::geometry2d::matrix& matrix)
   {

      if (m_pdc == nullptr)
      {

         throw ::exception(error_null_pointer);

      }

      float nanovgmatrix[6];

      _nvgcopy(nanovgmatrix, &matrix);


      nvgResetTransform(m_pdc);

      nvgTransform(m_pdc, __expand_nanovgmatrix(nanovgmatrix));

   }


   void graphics::frame_rectangle(const ::f64_rectangle& rectangleParam, ::draw2d::brush* pBrush)
   {

      //// ASSERT(m_hdc != nullptr);

      //::FrameRect(m_hdc,&rectangleParam,(HBRUSH)pBrush->get_os_data());

   }


   void graphics::invert_rectangle(const ::f64_rectangle& rectangleParam)
   {

      // ASSERT(m_hdc != nullptr);

      //::InvertRect(m_hdc,&rectangleParam);

   }


   //bool graphics::DrawIcon(double x, double y, ::image::icon * picon)
   //{

   //   // ASSERT(m_hdc != nullptr);

   //   if(picon == nullptr)
   //      return false;

   //   //return ::DrawIcon(m_hdc, x, y, (HICON) picon->m_picon) != false;

   //   return false;

   //}

   //bool graphics::DrawIcon(const ::i32_point & point, ::image::icon * picon)
   //{

   //   // ASSERT(m_hdc != nullptr);

   //   if(picon == nullptr)
   //      return false;

   //   //return ::DrawIcon(m_hdc, point.x, point.y, (HICON) picon->m_picon) != false;

   //   return false;

   //}

   //bool graphics::DrawIcon(double x, double y, ::image::icon * picon, double cx, double cy, unsigned int istepIfAniCur, HBRUSH hbrFlickerFreeDraw, unsigned int diFlags)
   //{

   //   //try
   //   //{

   //   //   if(picon == nullptr)
   //   //      return false;

   //   //   if(m_pgraphics == nullptr)
   //   //      return false;

   //   //   bool bOk = false;

   //   //   BITMAPINFO info;
   //   //   color32_t * pimage32;

   //   //   ZeroMemory(&info, sizeof (BITMAPINFO));

   //   //   info.bmiHeader.biSize          = sizeof (BITMAPINFOHEADER);
   //   //   info.bmiHeader.biWidth         = cx;
   //   //   info.bmiHeader.biHeight        = - cy;
   //   //   info.bmiHeader.biPlanes        = 1;
   //   //   info.bmiHeader.biBitCount      = 32;
   //   //   info.bmiHeader.biCompression   = BI_RGB;
   //   //   info.bmiHeader.biSizeImage     = cx * cy * 4;

   //   //   HBITMAP hbitmap = ::CreateDIBSection(nullptr, &info, DIB_RGB_COLORS, (void **) &pimage32, nullptr, 0);

   //   //   HDC hdc = ::CreateCompatibleDC(nullptr);

   //   //   HBITMAP hbitmapOld = (HBITMAP) ::SelectObject(hdc, hbitmap);

   //   //   ICONINFO ii;
   //   //   BITMAPINFO biC;
   //   //   BITMAPINFO biM;

   //   //   zero(biC);
   //   //   zero(biM);



   //   //   if(::GetIconInfo((HICON)picon->m_picon,&ii))
   //   //   {

   //   //      ::GetObject(ii.hbmColor,sizeof(biC),(void *)&biC);

   //   //      ::GetObject(ii.hbmMask,sizeof(biM),(void *)&biM);

   //   //   }



   //   //   if(!::DrawIconEx(hdc,0,0,(HICON)picon->m_picon,cx,cy,istepIfAniCur,nullptr,DI_IMAGE | DI_MASK))
   //   //   {

   //   //      informationf("nok");
   //   //   }
   //   //   else
   //   //   {

   //   //      bool bAllZeroAlpha = true;
   //   //      bool bTheresunsigned int = false;

   //   //      int area = cx * cy;

   //   //      color32_t * pc = pimage32;
   //   //      unsigned char * pA = &((unsigned char *) pimage32)[3];

   //   //      for(int i = 0; i < area; i++)
   //   //      {
   //   //         if(*pc != 0)
   //   //         {
   //   //            bTheresunsigned int = true;
   //   //         }
   //   //         if(*pA != 0)
   //   //         {
   //   //            bAllZeroAlpha = false;
   //   //            break;
   //   //         }
   //   //         pc++;
   //   //         pA +=4;
   //   //      }

   //   //      if(bAllZeroAlpha && bTheresunsigned int)
   //   //      {

   //   //         pc = pimage32;
   //   //         pA = &((unsigned char *)pimage32)[3];

   //   //         for(int i = 0; i < area; i++)
   //   //         {
   //   //            if(*pc != 0)
   //   //            {
   //   //               *pA = 255;
   //   //            }
   //   //            pc++;
   //   //            pA +=4;
   //   //         }
   //   //      }

   //   //      ::SelectObject(hdc, hbitmapOld);

   //   //      try
   //   //      {

   //   //         plusplus::Bitmap b(cx, cy, cx * 4 , PixelFormat32bppARGB, (unsigned char *) pimage32);

   //   //         bOk = m_pgraphics->DrawImage(&b, x, y, 0, 0, cx, cy, plusplus::UnitPixel) == plusplus::Ok;

   //   //      }
   //   //      catch(...)
   //   //      {
   //   //      }

   //   //   }

   //   //   ::DeleteDC(hdc);

   //   //   ::DeleteObject(hbitmap);

   //   //   return bOk;

   //   //}
   //   //catch(...)
   //   //{
   //   //   return false;
   //   //}

   //   //return ::DrawIconEx(m_hdc, x, y, picon->m_hicon, cx, cy, istepIfAniCur, hbrFlickerFreeDraw, diFlags);

   //   return true;

   //}

//   bool graphics::DrawState(const ::i32_point & point, const ::i32_size & size, HBITMAP hBitmap, unsigned int nFlags, HBRUSH hBrush)
//   {
//
//      // ASSERT(m_hdc != nullptr);
//      //return ::DrawState(m_hdc, hBrush, nullptr, (LPARAM)hBitmap, 0, point.x, point.y, size.cx, size.cy, nFlags|DST_BITMAP) != false;
//      return false;
//
//   }

//   bool graphics::DrawState(const ::i32_point & point, const ::i32_size & size, ::draw2d::bitmap* pBitmap, unsigned int nFlags, ::draw2d::brush* pBrush)
//   {
//
//      // ASSERT(m_hdc != nullptr);
//      //return ::DrawState(m_hdc, (HBRUSH)pBrush->get_os_data(), nullptr, (LPARAM)pBitmap->get_os_data(), 0, point.x, point.y, size.cx, size.cy, nFlags|DST_BITMAP) != false;
//
//      return false;
//
//   }
//
//   bool graphics::DrawState(const ::i32_point & point, const ::i32_size & size, HICON hIcon, unsigned int nFlags, HBRUSH hBrush)
//   {
//
//      // ASSERT(m_hdc != nullptr);
//      //return ::DrawState(m_hdc, hBrush, nullptr, (LPARAM)hIcon, 0, point.x, point.y, size.cx, size.cy, nFlags|DST_ICON) != false;
//
//      return false;
//
//   }
//
//
//   bool graphics::DrawState(const ::i32_point & point, const ::i32_size & size, HICON hIcon, unsigned int nFlags, ::draw2d::brush* pBrush)
//   {
//
//      // ASSERT(m_hdc != nullptr);
//      //return ::DrawState(m_hdc, (HBRUSH)pBrush->get_os_data(), nullptr, (LPARAM)hIcon, 0, point.x, point.y, size.cx, size.cy, nFlags|DST_ICON) != false;
//      return false;
//
//   }
//
//
//   bool graphics::DrawState(const ::i32_point & point, const ::i32_size & size, const ::string & lpszText, unsigned int nFlags, bool bPrefixText, double nTextLen, HBRUSH hBrush)
//   {
//
//      // ASSERT(m_hdc != nullptr);
//      //return ::DrawState(m_hdc, hBrush,  nullptr, (LPARAM)lpszText, (WPARAM)nTextLen, point.x, point.y, size.cx, size.cy, nFlags|(bPrefixText ? DST_PREFIXTEXT : DST_TEXT)) != false;
//      return false;
//
//   }
//
//
//   bool graphics::DrawState(const ::i32_point & point, const ::i32_size & size, const ::string & lpszText, unsigned int nFlags, bool bPrefixText, double nTextLen, ::draw2d::brush* pBrush)
//   {
//
//      // ASSERT(m_hdc != nullptr);
//      //return ::DrawState(m_hdc, (HBRUSH)pBrush->get_os_data(), nullptr, (LPARAM)lpszText, (WPARAM)nTextLen, point.x, point.y, size.cx, size.cy, nFlags|(bPrefixText ? DST_PREFIXTEXT : DST_TEXT)) != false;
//      return false;
//
//   }
//
//
//   bool graphics::DrawState(const ::i32_point & point, const ::i32_size & size, DRAWSTATEPROC lpDrawProc, LPARAM lData, unsigned int nFlags, HBRUSH hBrush)
//   {
//      // ASSERT(m_hdc != nullptr);
//      //return ::DrawState(m_hdc, hBrush,
//          //               lpDrawProc, lData, 0, point.x, point.y, size.cx, size.cy, nFlags|DST_COMPLEX) != false;
//      return false;
//
//   }
//
//
//   bool graphics::DrawState(const ::i32_point & point, const ::i32_size & size, DRAWSTATEPROC lpDrawProc, LPARAM lData, unsigned int nFlags, ::draw2d::brush* pBrush)
//   {
//      // ASSERT(m_hdc != nullptr);
//      //return ::DrawState(m_hdc, (HBRUSH)pBrush->get_os_data(),
//        //                 lpDrawProc, lData, 0, point.x, point.y, size.cx, size.cy, nFlags|DST_COMPLEX) != false;
//      return false;
//
//   }
//

//   bool graphics::DrawEdge(const ::f64_rectangle & rectangleParam,unsigned int nEdge,unsigned int nFlags)
//   {
//
//      // ASSERT(m_hdc != nullptr);
//      //return ::DrawEdge(m_hdc,(::f64_rectangle *)&rectangleParam,nEdge,nFlags) != false;
//      return false;
//
//   }
//

//   bool graphics::DrawFrameControl(const ::f64_rectangle & rectangleParam,unsigned int nType,unsigned int nState)
//   {
//      // ASSERT(m_hdc != nullptr);
//      //return ::DrawFrameControl(m_hdc,(::f64_rectangle *) &rectangleParam,nType,nState) != false;
//      return false;
//
//   }


   bool graphics::Chord(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
   {
      // ASSERT(m_hdc != nullptr);
      //return ::Chord(m_hdc, x1, y1, x2, y2, x3, y3, x4, y4) != false;
      return false;

   }


   bool graphics::Chord(const ::i32_rectangle& rectangleParam, const ::i32_point& pointStart, const ::i32_point& pointEnd)
   {

      // ASSERT(m_hdc != nullptr);
      //return ::Chord(m_hdc,rectangleParam.left,rectangleParam.top,
      //               rectangleParam.right,rectangleParam.bottom,ptStart.x,ptStart.y,
      //               ptEnd.x, ptEnd.y) != false;

      return false;

   }


   void graphics::DrawFocusRect(const ::i32_rectangle& rectangleParam)
   {
      // ASSERT(m_hdc != nullptr);
      //::DrawFocusRect(m_hdc,&rectangleParam);

   }


   //bool graphics::DrawEllipse(double x1, double y1, double x2, double y2)
   //{

   //   //set_smooth_mode(::draw2d::e_smooth_mode_high);

   //   //return (m_pgraphics->DrawEllipse(vk2d_pen(), x1, y1, x2 - x1, y2 - y1)) == plusplus::Status::Ok;

   //   return true;
   //}


   void graphics::draw_ellipse(const ::f64_rectangle& rectangleParam)
   {

      ::draw2d::graphics::draw_ellipse(rectangleParam);

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      //double centerx = (x2 + x1) / 2.0;

      //double centery = (y2 + y1) / 2.0;

      //double radiusx = fabs(x2 - x1) / 2.0;

      //double radiusy = fabs(y2 - y1) / 2.0;

      //if (radiusx == 0.0 || radiusy == 0.0)
      //{

      //   //return false;

      //   return;

      //}

      //nanovg_keep keep(m_pdc);

      //nanovg_new_sub_path(m_pdc);

      //nvgTranslate(m_pdc, centerx, centery);

      //if (m_ppen->m_epenalign == ::draw2d::e_pen_align_inset)
      //{

      //   nvgScale(m_pdc, radiusx - m_ppen->m_dWidth / 2.0, radiusy - m_ppen->m_dWidth / 2.0);

      //}
      //else
      //{

      //   nvgScale(m_pdc, radiusx, radiusy);

      //}

      //nvgArc(m_pdc, 0.0, 0.0, 1.0, 0.0, 2.0 * 3.1415);

      //keep.pulse();

      //_set(m_ppen);

      //nvgStroke(m_pdc);

      ////return true;

   }



   //bool graphics::FillEllipse(double x1, double y1, double x2, double y2)
   //{

   //   //set_smooth_mode(::draw2d::e_smooth_mode_high);

   //   //return (m_pgraphics->FillEllipse(vk2d_brush(), x1, y1, x2 - x1, y2 - y1)) == plusplus::Status::Ok;

   //   return true;

   //}


   void graphics::fill_ellipse(const ::f64_rectangle& rectangleParam)
   {

      ::draw2d::graphics::fill_ellipse(rectangleParam);
      //_synchronous_lock ml(::draw2d_nanovg::mutex());

      //double centerx = (x2 + x1) / 2.0;

      //double centery = (y2 + y1) / 2.0;

      //double radiusx = fabs(x2 - x1) / 2.0;

      //double radiusy = fabs(y2 - y1) / 2.0;

      //if (radiusx == 0.0 || radiusy == 0.0)
      //{

      //   //return false;

      //   return;

      //}

      //nanovg_keep keep(m_pdc);

      //_fill1();

      //nanovg_new_sub_path(m_pdc);

      //nvgTranslate(m_pdc, centerx, centery);

      //nvgScale(m_pdc, radiusx, radiusy);

      ////nvgArc(m_pdc, 0.0, 0.0, 1.0, 0.0, 2.0 * M_PI);
      //nvgArc(m_pdc, 0.0, 0.0, 1.0, 0.0, 6.28);

      //nanovg_fill(m_pdc);

      //_fill2();

      ////return true;

   }

   void graphics::fill_ellipse(double x1, double y1, double x2, double y2)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      double centerx = (x2 + x1) / 2.0;

      double centery = (y2 + y1) / 2.0;

      double radiusx = fabs(x2 - x1) / 2.0;

      double radiusy = fabs(y2 - y1) / 2.0;

      if (radiusx == 0.0 || radiusy == 0.0)
      {

         //return false;

         return;

      }

      if (!m_pbrush)
      {

         return;

      }

      if (m_pbrush->m_ebrush == ::draw2d::e_brush_null)
      {

         return;

      }

      if (m_pbrush->m_ebrush == ::draw2d::e_brush_solid
         && m_pbrush->m_color.is_transparent()
         && m_ealphamode == ::draw2d::e_alpha_mode_blend)
      {

         return;

      }

      nanovg_keep keep(m_pdc);

      _fill1();

      nvgBeginPath(m_pdc);

      //nvgTranslate(m_pdc, (float)centerx, (float)centery);

      //nvgScale(m_pdc, (float)radiusx, (float)radiusy);

      //nvgArc(m_pdc, 0.0, 0.0, 1.0, 0.0, 2.0 * M_PI);
      //nvgArc(m_pdc, (float)0.0, (float)0.0, (float)1.0, (float)0.0, (float)6.28, NVG_CCW);

      nvgEllipse(m_pdc, (float)centerx, (float)centery, (float)radiusx, (float)radiusy);

      //keep.pulse();

      _set(m_pbrush);

      nvgFill(m_pdc);

      _fill2();

      //return true;

   }


   void graphics::draw_ellipse(double x1, double y1, double x2, double y2)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      double centerx = (x2 + x1) / 2.0;

      double centery = (y2 + y1) / 2.0;

      double radiusx = fabs(x2 - x1) / 2.0;

      double radiusy = fabs(y2 - y1) / 2.0;

      if (radiusx == 0.0 || radiusy == 0.0)
      {

         //return false;

         return;

      }

      nanovg_keep keep(m_pdc);

      nvgBeginPath(m_pdc);

      //nvgTranslate(m_pdc, (float)centerx, (float)centery);

      //if (m_ppen->m_epenalign == ::draw2d::e_pen_align_inset)
      //{

      //   nvgScale(m_pdc, (float)(radiusx - m_ppen->m_dWidth / 2.0), (float)(radiusy - m_ppen->m_dWidth / 2.0));

      //}
      //else
      //{

      //   nvgScale(m_pdc, (float)radiusx, (float)radiusy);

      //}

      nvgEllipse(m_pdc, (float)centerx, (float)centery, (float)radiusx, (float)radiusy);

      //keep.pulse();

      _set(m_ppen);

      nvgStroke(m_pdc);

      //return true;

   }


   //bool graphics::draw_ellipse(const ::f64_rectangle & rectectParam)
   //{

   //   //set_smooth_mode(::draw2d::e_smooth_mode_high);

   //   //return (m_pgraphics->DrawEllipse(vk2d_pen(),(plusplus::REAL)rectangleParam.left,(plusplus::REAL)rectangleParam.top,
   //   //                                 (plusplus::REAL)(rectangleParam.right - rectangleParam.left),
   //   //                                 (plusplus::REAL)(rectangleParam.bottom - rectangleParam.top))) == plusplus::Status::Ok;

   //   return true;

   //}


   //bool graphics::FillEllipse(double x1,double y1,double x2,double y2)
   //{

   //   //set_smooth_mode(::draw2d::e_smooth_mode_high);

   //   //return (m_pgraphics->FillEllipse(vk2d_brush(),(plusplus::REAL)x1,(plusplus::REAL)y1,(plusplus::REAL)(x2 - x1),(plusplus::REAL)(y2 - y1))) == plusplus::Status::Ok;

   //   return true;

   //}


   //bool graphics::FillEllipse(const ::f64_rectangle & rectectParam)
   //{

   //   //set_smooth_mode(::draw2d::e_smooth_mode_high);

   //   //return (m_pgraphics->FillEllipse(vk2d_brush(),(plusplus::REAL)rectangleParam.left,(plusplus::REAL)rectangleParam.top,
   //   //                                 (plusplus::REAL)(rectangleParam.right - rectangleParam.left),
   //   //                                 (plusplus::REAL)(rectangleParam.bottom - rectangleParam.top))) == plusplus::Status::Ok;

   //   return true;

   //}


   //bool graphics::Pie(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
   //{
   //
   //   // ASSERT(m_hdc != nullptr);
   //   //return ::Pie(m_hdc, x1, y1, x2, y2, x3, y3, x4, y4) != false;
   //   return false;

   //}


   //bool graphics::Pie(const ::f64_rectangle & rectangleParam,const ::i32_point & pointStart,const ::i32_point & pointEnd)
   //{

   //   // ASSERT(m_hdc != nullptr);
   //   //return ::Pie(m_hdc, rectangleParam.left, rectangleParam.top,
   //   //             rectangleParam.right, rectangleParam.bottom, ptStart.x, ptStart.y,
   //   //             ptEnd.x, ptEnd.y) != false;

   //   return false;

   //}


   void graphics::fill_polygon(const ::f64_point* lpPoints, ::collection::count nCount)
   {

      if (!lpPoints || nCount < 3)
      {

         return;

      }

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      nvgBeginPath(m_pdc);
      nvgMoveTo(m_pdc, (float)lpPoints[0].x, (float)lpPoints[0].y);

      for (::collection::index i = 1; i < nCount; i++)
      {

         nvgLineTo(m_pdc, (float)lpPoints[i].x, (float)lpPoints[i].y);

      }

      nvgClosePath(m_pdc);

      fill();

      //   if(nCount <= 0)
      //      return true;

      //   bool bOk1 = false;

      //   plusplus::PointF * ppoints = ___new plusplus::PointF[nCount];

      //   try
      //   {

      //      for(double i = 0; i < nCount; i++)
      //      {
      //         ppoints[i].X = (plusplus::REAL) lpPoints[i].x;
      //         ppoints[i].Y = (plusplus::REAL) lpPoints[i].y;
      //      }

      //      m_pgraphics->SetInterpolationMode(plusplus::InterpolationModeHighQualityBicubic);

      //      set_smooth_mode(::draw2d::e_smooth_mode_high);


      //      bOk1 = m_pgraphics->FillPolygon(vk2d_brush(), ppoints, (::double) nCount, vk2d_get_fill_mode()) == plusplus::Status::Ok;

      //   }
      //   catch(...)
      //   {
      //   }

      //   try
      //   {
      //      delete ppoints;
      //   }
      //   catch(...)
      //   {
      //   }


      //   return bOk1;

      //return true;

   }


   //void graphics::fill_polygon(const ::f64_point* lpPoints,::collection::count nCount)
   //{

   //   //   if(nCount <= 0)
   //   //      return true;

   //   //   bool bOk1 = false;

   //   //   plusplus::Point * ppoints = ___new plusplus::Point[nCount];

   //   //   try
   //   //   {

   //   //      for(double i = 0; i < nCount; i++)
   //   //      {
   //   //         ppoints[i].X = lpPoints[i].x;
   //   //         ppoints[i].Y = lpPoints[i].y;
   //   //      }

   //   //      m_pgraphics->SetInterpolationMode(plusplus::InterpolationModeHighQualityBicubic);

   //   //      bOk1 = m_pgraphics->FillPolygon(vk2d_brush(), ppoints, (::double)  nCount, vk2d_get_fill_mode()) == plusplus::Status::Ok;

   //   //   }
   //   //   catch(...)
   //   //   {
   //   //   }

   //   //   try
   //   //   {
   //   //      delete ppoints;
   //   //   }
   //   //   catch(...)
   //   //   {
   //   //   }


   //   //   return bOk1;

   //   return true;

   //}


   void graphics::draw_polygon(const ::f64_point* lpPoints, ::collection::count nCount)
   {

      if (!lpPoints || nCount < 2)
      {

         return;

      }

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      nvgBeginPath(m_pdc);
      nvgMoveTo(m_pdc, (float)lpPoints[0].x, (float)lpPoints[0].y);

      for (::collection::index i = 1; i < nCount; i++)
      {

         nvgLineTo(m_pdc, (float)lpPoints[i].x, (float)lpPoints[i].y);

      }

      nvgClosePath(m_pdc);

      draw();

      //if (nCount <= 0)
      //   return true;

      //bool bOk1 = false;

      //plusplus::Point * ppoints = ___new plusplus::Point[nCount];

      //try
      //{

      //   for (double i = 0; i < nCount; i++)
      //   {
      //      ppoints[i].X = lpPoints[i].x;
      //      ppoints[i].Y = lpPoints[i].y;
      //   }

      //   m_pgraphics->SetInterpolationMode(plusplus::InterpolationModeHighQualityBicubic);

      //   set_smooth_mode(::draw2d::e_smooth_mode_high);

      //   bOk1 = m_pgraphics->DrawPolygon(vk2d_pen(), ppoints, (::double) nCount) == plusplus::Status::Ok;

      //}
      //catch (...)
      //{
      //}

      //try
      //{
      //   delete ppoints;
      //}
      //catch (...)
      //{
      //}


      //return bOk1;
      //return true;

   }


   //void graphics::draw_polygon(const ::f64_point* lpPoints,::collection::count nCount)
   //{

   //   //if(nCount <= 0)
   //   //   return true;

   //   //bool bOk1 = false;

   //   //plusplus::PointF * ppoints = ___new plusplus::PointF[nCount];

   //   //try
   //   //{

   //   //   for(double i = 0; i < nCount; i++)
   //   //   {
   //   //      ppoints[i].X = (plusplus::REAL) lpPoints[i].x;
   //   //      ppoints[i].Y = (plusplus::REAL) lpPoints[i].y;
   //   //   }

   //   //   m_pgraphics->SetInterpolationMode(plusplus::InterpolationModeHighQualityBicubic);

   //   //   bOk1 = m_pgraphics->DrawPolygon(vk2d_pen(),ppoints,(::double) nCount) == plusplus::Status::Ok;

   //   //}
   //   //catch(...)
   //   //{
   //   //}

   //   //try
   //   //{
   //   //   delete ppoints;
   //   //}
   //   //catch(...)
   //   //{
   //   //}


   //   //return bOk1;

   //   return true;

   //}


   void graphics::polygon(const ::f64_point* lpPoints, ::collection::count nCount)
   {

      if (!lpPoints || nCount < 3)
      {

         return;

      }

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      nvgBeginPath(m_pdc);
      nvgMoveTo(m_pdc, (float)lpPoints[0].x, (float)lpPoints[0].y);

      for (::collection::index i = 1; i < nCount; i++)
      {

         nvgLineTo(m_pdc, (float)lpPoints[i].x, (float)lpPoints[i].y);

      }

      nvgClosePath(m_pdc);

      fill();
      draw();

      //if(nCount <= 0)
      //   return true;

      //bool bOk1 = false;

      //bool bOk2 = false;

      //plusplus::Point * ppoints = ___new plusplus::Point[nCount];

      //try
      //{

      //   for(double i = 0; i < nCount; i++)
      //   {
      //      ppoints[i].X = lpPoints[i].x;
      //      ppoints[i].Y = lpPoints[i].y;
      //   }


      //   set_smooth_mode(::draw2d::e_smooth_mode_high);

      //   m_pgraphics->SetInterpolationMode(plusplus::InterpolationModeHighQualityBicubic);


      //   bOk1 = m_pgraphics->FillPolygon(vk2d_brush(), ppoints, (::double) nCount, vk2d_get_fill_mode()) == plusplus::Status::Ok;

      //   bOk2 = m_pgraphics->DrawPolygon(vk2d_pen(), ppoints, (::double) nCount) == plusplus::Status::Ok;

      //}
      //catch(...)
      //{
      //}

      //try
      //{
      //   delete ppoints;
      //}
      //catch(...)
      //{
      //}


      //return bOk1 && bOk2;

      //return true;

   }


   void graphics::poly_polygon(const ::f64_point* lpPoints, const int* lpPolyCounts, ::collection::count nCount)
   {

      if (!lpPoints || !lpPolyCounts || nCount <= 0)
      {

         return;

      }

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      nvgBeginPath(m_pdc);

      ::collection::index iPoint = 0;

      for (::collection::index iPolygon = 0; iPolygon < nCount; iPolygon++)
      {

         auto iPolygonCount = lpPolyCounts[iPolygon];

         if (iPolygonCount < 1)
         {

            continue;

         }

         nvgMoveTo(m_pdc, (float)lpPoints[iPoint].x, (float)lpPoints[iPoint].y);

         for (int i = 1; i < iPolygonCount; i++)
         {

            nvgLineTo(m_pdc, (float)lpPoints[iPoint + i].x, (float)lpPoints[iPoint + i].y);

         }

         nvgClosePath(m_pdc);

         iPoint += iPolygonCount;

      }

      fill();
      draw();

      // ASSERT(m_hdc != nullptr);


      //return ::PolyPolygon(m_hdc, lpPoints, (const ::double *) lpPolyCounts, (int) nCount) != false;
      //return false;

   }


   //bool graphics::f64_rectangle(double x1, double y1, double x2, double y2)
   //{

   //   //plusplus::rectF float_rectangle((plusplus::REAL) x1, (plusplus::REAL) y1, (plusplus::REAL) (x2 - x1), (plusplus::REAL) (y2 - y1));

   //   //bool bOk1 = m_pgraphics->FillRectangle(vk2d_brush(), float_rectangle) == plusplus::Status::Ok;

   //   //bool bOk2 = m_pgraphics->DrawRectangle(vk2d_pen(), float_rectangle) == plusplus::Status::Ok;

   //   //return bOk1 && bOk2;

   //   return true;

   //}


   //void graphics::polygon(const ::f64_point* lpPoints,::collection::count nCount)
   //{

   //   //if(nCount <= 0)
   //   //   return true;

   //   //bool bOk1 = false;

   //   //bool bOk2 = false;

   //   //plusplus::PointF * ppoints = ___new plusplus::PointF[nCount];

   //   //try
   //   //{

   //   //   for(double i = 0; i < nCount; i++)
   //   //   {
   //   //      ppoints[i].X = (plusplus::REAL) lpPoints[i].x;
   //   //      ppoints[i].Y = (plusplus::REAL) lpPoints[i].y;
   //   //   }

   //   //   m_pgraphics->SetInterpolationMode(plusplus::InterpolationModeHighQualityBicubic);

   //   //   if(m_ealphamode == ::draw2d::e_alpha_mode_set)
   //   //   {

   //   //      set_smooth_mode(::draw2d::e_smooth_mode_none);

   //   //   }
   //   //   else
   //   //   {

   //   //      set_smooth_mode(::draw2d::e_smooth_mode_high);

   //   //   }


   //   //   bOk1 = m_pgraphics->FillPolygon(vk2d_brush(),ppoints,(::double) nCount,vk2d_get_fill_mode()) == plusplus::Status::Ok;

   //   //   bOk2 = m_pgraphics->DrawPolygon(vk2d_pen(),ppoints,(::double) nCount) == plusplus::Status::Ok;

   //   //}
   //   //catch(...)
   //   //{
   //   //}

   //   //try
   //   //{
   //   //   delete ppoints;
   //   //}
   //   //catch(...)
   //   //{
   //   //}


   //   //return bOk1 && bOk2;

   //   return true;

   //}


   void graphics::rectangle(const ::f64_rectangle& rectangleParam)
   {

      fill_rectangle(rectangleParam);

      draw_rectangle(rectangleParam);

      //return ::f64_rectangle(rectangleParam.left, rectangleParam.top, rectangleParam.right, rectangleParam.bottom);

      //return bOk1 && bOk2;

   }


   void graphics::draw_rectangle(const ::f64_rectangle& rectangle, ::draw2d::pen* ppen)
   {

      if (!m_pdc)
      {

         throw ::exception(error_wrong_state);

      }

      nvgBeginPath(m_pdc);

      nvgRect(m_pdc,
         (float)rectangle.left,
         (float)rectangle.top,
         (float)(rectangle.right - rectangle.left),
         (float)(rectangle.bottom - rectangle.top));

      draw(ppen);
      //vkBegin(VK_QUADS);

      //set(pbrush);

      //::opengl::vertex2f(rectangle);

      //vkEnd();

      ////return false;

   }



   void graphics::draw_rectangle(const ::f64_rectangle& rectangle)
   {

      draw_rectangle(rectangle, m_ppen);

      //return true;

   }


   void graphics::fill_rectangle(const ::f64_rectangle& rectangle)
   {

      fill_rectangle(rectangle, m_pbrush);

   }


   //void graphics::round_rectangle(double x1, double y1, double x2, double y2, double x3, double y3)
   //{
   //
   //   //// ASSERT(m_hdc != nullptr);
   //   //return ::RoundRect(m_hdc, x1, y1, x2, y2, x3, y3) != false;

   //   //return false;

   //}


   void graphics::round_rectangle(const ::f64_rectangle& rectangleParam, const ::i32_point& point)
   {
      //// ASSERT(m_hdc != nullptr);
      //return ::RoundRect(m_hdc, rectangleParam.left, rectangleParam.top,
        //                 rectangleParam.right, rectangleParam.bottom, point.x, point.y) != false;

      //return false;

   }


   //   bool graphics::PatBlt(double x, double y, double nWidth, double nHeight)
   //   {
   //      //// ASSERT(m_hdc != nullptr);
   //      //return ::PatBlt(m_hdc, x, y, nWidth, nHeight, dwRop) != false;
   //
   //      return false;
   //
   //   }
   //
   //
   //   bool graphics::BitBltRaw(double x, double y, double nWidth, double nHeight, ::draw2d::graphics * pgraphicsSrc, double xSrc, double ySrc)
   //   {
   ////      if (m_pgraphics == nullptr)
   ////         return false;
   ////
   ////      if (::draw2d::graphics::BitBlt(x, y, nWidth, nHeight, pgraphicsSrc, xSrc, ySrc, dwRop))
   ////         return true;
   ////
   //      try
   //      {
   //
   //         if(pgraphicsSrc == nullptr)
   //            return false;
   //
   //         if (pgraphicsSrc->get_current_bitmap() == nullptr)
   //            return false;
   //
   //         if (pgraphicsSrc->get_current_bitmap()->get_os_data() == nullptr)
   //            return false;
   //
   //         ::pointer<bitmap>pbitmap = pgraphicsSrc->get_current_bitmap();
   //
   //         //pbitmap->create_texture(0);
   //
   //         //vkBegin(VK_QUADS);
   //         //// Front Face
   //         //vkTexCoord2f(0.0f, 0.0f); vkVertex3f(-1.0f, -1.0f, 1.0f);
   //         //vkTexCoord2f(1.0f, 0.0f); vkVertex3f(1.0f, -1.0f, 1.0f);
   //         //vkTexCoord2f(1.0f, 1.0f); vkVertex3f(1.0f, 1.0f, 1.0f);
   //         //vkTexCoord2f(0.0f, 1.0f); vkVertex3f(-1.0f, 1.0f, 1.0f);
   //
   //
   ////
   ////
   ////         return m_pgraphics->DrawImage(
   ////                   (plusplus::Bitmap *) pgraphicsSrc->get_current_bitmap()->get_os_data(),
   ////                   x, y, xSrc + pgraphicsSrc->get_origin().x, ySrc + pgraphicsSrc->get_origin().y, nWidth, nHeight, plusplus::UnitPixel) == plusplus::Status::Ok;
   ////
   //      }
   //      catch(...)
   //      {
   //         return false;
   //      }
   ////
   ////      //return ::BitBlt(m_hdc, x, y, nWidth, nHeight, VK2D_HDC(pgraphicsSrc), xSrc, ySrc);
   ////
   ////gdi_fallback:
   ////
   ////      HDC hdcDst = get_hdc();
   ////
   ////      if(hdcDst == nullptr)
   ////         return false;
   ////
   ////      HDC hdcSrc = VK2D_GRAPHICS(pgraphicsSrc)->get_hdc();
   ////
   ////      if(hdcSrc == nullptr)
   ////      {
   ////
   ////         release_hdc(hdcDst);
   ////
   ////         return false;
   ////
   ////      }
   ////
   ////      bool bOk = ::BitBlt(hdcDst, x, y, nWidth, nHeight, hdcSrc, x, y, dwRop) != false;
   ////
   ////      VK2D_GRAPHICS(pgraphicsSrc)->release_hdc(hdcSrc);
   ////
   ////      release_hdc(hdcDst);
   ////
   ////      return bOk;
   //
   //      return true;
   //
   //   }
   //
   //
   //   bool graphics::StretchBltRaw(double xDst, double yDst, double nDstWidth, double nDstHeight, ::draw2d::graphics * pgraphicsSrc, double xSrc, double ySrc, double nSrcWidth, double nSrcHeight)
   //   {
   //
   //      //if(pgraphicsSrc == nullptr)
   //      //   return false;
   //
   //      //plusplus::rectF dstRect((plusplus::REAL) xDst, (plusplus::REAL) yDst, (plusplus::REAL) nDstWidth, (plusplus::REAL) nDstHeight);
   //
   //      //plusplus::rectF srcRect((plusplus::REAL) xSrc, (plusplus::REAL) ySrc, (plusplus::REAL) nSrcWidth, (plusplus::REAL) nSrcHeight);
   //
   //      //if(pgraphicsSrc == nullptr || pgraphicsSrc->get_current_bitmap() == nullptr)
   //      //   return false;
   //
   //      //try
   //      //{
   //      //   return m_pgraphics->DrawImage((plusplus::Bitmap *) pgraphicsSrc->get_current_bitmap()->get_os_data(),  dstRect, srcRect, plusplus::UnitPixel) == plusplus::Status::Ok;
   //      //}
   //      //catch(...)
   //      //{
   //
   //      //}
   //
   //      //return false;
   //
   //      ////return ::StretchBlt(m_hdc, x, y, nWidth, nHeight, VK2D_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth, nSrcHeight);
   //
   //      return true;
   //
   //   }


   ::color::color graphics::GetPixel(int x, int y) const
   {
      // ASSERT(m_hdc != nullptr);
      //return ::GetPixel(m_hdc, x, y);
      return color::transparent;

   }


   ::color::color graphics::GetPixel(const ::i32_point& point) const
   {
      // ASSERT(m_hdc != nullptr);
      //return ::GetPixel(m_hdc, point.x, point.y);
      return color::transparent;

   }


   ::color::color graphics::SetPixel(double x, double y, ::color::color crColor)
   {

      return color::transparent;

   }


   ::color::color graphics::SetPixel(const ::i32_point& point, ::color::color crColor)
   {

      //fill_solid_rect_coord(point.x,point.y,1,1,crColor);

      return crColor;

   }


   //   bool graphics::FloodFill(double x, double y, color32_t crColor)
   //   {
   //
   //      // ASSERT(m_hdc != nullptr);
   //      //return ::FloodFill(m_hdc, x, y, crColor) != false;
   //      return 0;
   //
   //   }
   //
   //
   //   bool graphics::ExtFloodFill(double x, double y, color32_t crColor, unsigned int nFillType)
   //   {
   //
   //      // ASSERT(m_hdc != nullptr);
   //      //return ::ExtFloodFill(m_hdc, x, y, crColor, nFillType) != false;
   //
   //      return 0;
   //
   //   }


      //i32_size graphics::GetTabbedTextExtent(const ::string & lpszString, character_count nCount, ::collection::count nTabPositions, LPINT lpnTabStopPositions)
      //{

      //   // ASSERT(m_hdc != nullptr);

      //   return nullptr;

      //}


      //i32_size graphics::GetTabbedTextExtent(const ::string & str, ::collection::count nTabPositions, LPINT lpnTabStopPositions)
      //{

      //   // ASSERT(m_hdc != nullptr);

      //   return nullptr;

      //}


      //i32_size graphics::GetOutputTabbedTextExtent(const ::string & lpszString, character_count nCount, ::collection::count nTabPositions, LPINT lpnTabStopPositions)
      //{

      //   // ASSERT(m_hdc != nullptr);

      //   //return ::GetTabbedTextExtent(m_hdc, lpszString, (double) nCount, (int) nTabPositions, lpnTabStopPositions);
      //   return nullptr;

      //}


      //i32_size graphics::GetOutputTabbedTextExtent(const ::string & str, ::collection::count nTabPositions, LPINT lpnTabStopPositions)
      //{

      //   // ASSERT(m_hdc != nullptr);

      //   //return ::GetTabbedTextExtent(m_hdc, str, (double) str.length(), (int) nTabPositions, lpnTabStopPositions);
      //   return nullptr;

      //}


      //bool graphics::GrayString(::draw2d::brush* pBrush, bool (CALLBACK* lpfnOutput)(HDC, LPARAM, double), LPARAM lpData, double nCount,double x, double y, double nWidth, double nHeight)
      //{

      //   // ASSERT(m_hdc != nullptr);

      //   //return ::GrayString(m_hdc, (HBRUSH)pBrush->get_os_data(),(GRAYSTRINGPROC)lpfnOutput, lpData, nCount, x, y, nWidth, nHeight) != false;
      //   return false;

      //}


   unsigned int graphics::GetTextAlign()
   {

      // ASSERT(m_hdc != nullptr);

      //return ::GetTextAlign(m_hdc);
      return 0;

   }


   //double graphics::GetTextFace(::collection::count nCount, char * lpszFacename)
   //{

   //   // ASSERT(m_hdc != nullptr);

   //   //return ::GetTextFace(m_hdc, (int) nCount, lpszFacename);
   //   return -1;

   //}


   //double graphics::GetTextFace(string & rString)
   //{

   //   // ASSERT(m_hdc != nullptr);

   //   //double nResult = ::GetTextFace(m_hdc, 256, rString.GetBuffer(256));
   //   //rString.ReleaseBuffer();

   //   //return nResult;
   //   return -1;

   //}


   void graphics::get_text_metrics(::write_text::text_metric* lpMetrics)
   {

      _set(m_pfont);
      //if (!set(m_pfont))
      //{

      //   return false;

      //}
      


      float ascender = 0.f;

      float descender = 0.f;

      float lineh = 0.f;

      nvgTextMetrics(m_pdc, &ascender, &descender, &lineh);


      lpMetrics->m_dAscent = ascender;
      lpMetrics->m_dDescent = -descender;
      lpMetrics->m_dHeight = lineh;
      lpMetrics->m_dInternalLeading = 0.0;
      lpMetrics->m_dExternalLeading =
         maximum(0.0, (double)lineh - ((double)ascender - (double)descender));
       
//#if defined(WINDOWS_DESKTOP)
//      ::pointer<font>pfont = m_pfont;
//
//      TEXTMETRIC tm;
//
//      GetTextMetrics(pfont->m_hdcFont, &tm);
//
//      lpMetrics->m_dAscent = tm.tmAscent;
//      lpMetrics->m_dHeight = tm.tmHeight;
//      lpMetrics->m_dDescent = tm.tmDescent;
//
//#endif
      
      //lpMetrics->tmAveCharWidth = tm.tmAveCharWidth;

      //if (m_pgraphics == nullptr)
      //   return false;

      //graphics * pgraphics = ((graphics *)this);

      //if(pgraphics->vk2d_font() == nullptr)
      //   return false;

      //plusplus::Font * pfont = pgraphics->vk2d_font();

      //plusplus::FontFamily family;

      //pfont->GetFamily(&family);

      //::double iStyle = pfont->GetStyle();

      //double dHeight = family.GetEmHeight(iStyle);

      //double dSize = pfont->GetSize();

      //double dFontHeight = pfont->GetHeight((plusplus::REAL) pgraphics->get_dpiy());

      //lpMetrics->tmAscent              = (::double) (dSize * family.GetCellAscent(iStyle) / dHeight);
      //lpMetrics->tmDescent             = (::double) (dSize * family.GetCellDescent(iStyle) / dHeight);
      //lpMetrics->tmHeight              = (::double)dFontHeight;

      //double dLineSpacing = maximum(dFontHeight, dSize * family.GetLineSpacing(iStyle) / dHeight);

      //lpMetrics->tmInternalLeading     = (::double) (lpMetrics->tmAscent + lpMetrics->tmDescent - lpMetrics->tmHeight);
      //lpMetrics->tmExternalLeading     = (::double) (dLineSpacing - (lpMetrics->tmAscent + lpMetrics->tmDescent));

      //const plusplus::FontFamily * pfamilyMono = family.GenericMonospace();

      //::plusplus::Font font2(pfamilyMono, pfamilyMono->GetEmHeight(((graphics * )this)->vk2d_font()->GetStyle()));

      //wstring wstr(L"123AWZwmc123AWZwmcpQg");
      //plusplus::rectF ::f64_rectangle(0.f, 0.f, 1024.f * 1024.f, 1024.f * 1024.f);
      //plusplus::rectF rect2;
      //plusplus::PointF origin(0, 0);

      //m_pgraphics->MeasureString(wstr, (::double) wstr.get_length(), ((graphics * )this)->vk2d_font(), origin, &rectangle);


      ///*wstr = L"";
      //m_pgraphics->MeasureString(wstr.m_pwsz, -1, (plusplus::Font *) m_pfont->get_os_data(), origin, &rect2);*/

      //lpMetrics->tmAveCharWidth = (::double) (rectangle.Width * get_current_font()->m_dFontWidth / (double) wstr.get_length());

      //return true;

   }


   void graphics::get_output_text_metrics(::write_text::text_metric* lpMetrics)
   {

      // ASSERT(m_hdc != nullptr);

      //return ::GetTextMetricsW(m_hdc, lpMetrics) != false;

      //throw ::interface_only();

      //return false;

   }


   //double graphics::GetTextCharacterExtra()
   //{
   //   // ASSERT(m_hdc != nullptr);
   //   //return ::GetTextCharacterExtra(m_hdc);
   //   return 0;

   //}


   //bool graphics::GetCharWidth(unsigned int nFirstChar, unsigned int nLastChar, LPINT lpBuffer) const
   //{
   //   // ASSERT(m_hdc != nullptr);
   //   //return ::GetCharWidth(m_hdc, nFirstChar, nLastChar, lpBuffer) != false;

   //   return false;

   //}


   //bool graphics::GetOutputCharWidth(unsigned int nFirstChar, unsigned int nLastChar, LPINT lpBuffer) const
   //{
   //   // ASSERT(m_hdc != nullptr);
   //   //return ::GetCharWidth(m_hdc, nFirstChar, nLastChar, lpBuffer) != false;
   //   return false;

   //}


   unsigned int graphics::GetFontLanguageInfo() const
   {
      // ASSERT(m_hdc != nullptr);
      //return ::GetFontLanguageInfo(m_hdc);

      return 0;

   }


   //unsigned int graphics::GetCharacterPlacement(const ::string & lpString, character_count nCount, character_count nMaxExtent, LPGCP_RESULTS lpResults, unsigned int dwFlags) const
   //{

   //   // ASSERT(m_hdc != nullptr);

   //   // return ::GetCharacterPlacement(m_hdc, lpString, (int)nCount, (int)nMaxExtent, lpResults, dwFlags);
   //   return false;

   //}


   //unsigned int graphics::GetCharacterPlacement(string & str, character_count nMaxExtent, LPGCP_RESULTS lpResults, unsigned int dwFlags) const
   //{

   //   // ASSERT(m_hdc != nullptr);

   //   //return ::GetCharacterPlacement(m_hdc, (const ::string &)str, (double) str.length(), (int) nMaxExtent, lpResults, dwFlags);
   //   return false;

   //}


   //i32_size graphics::GetAspectRatioFilter() const
   //{
   //   // ASSERT(m_hdc != nullptr);
   //   ::i32_size size;
   //   // VERIFY(::GetAspectRatioFilterEx(m_hdc, &size));
   //   return size;

   //}


   //bool graphics::ScrollDC(double Δx, double Δy,
   //                        const ::f64_rectangle & lpRectScroll,const ::f64_rectangle & rectangleClip,
   //                        ::draw2d::region* pRgnUpdate, ::f64_rectangle * lpRectUpdate)
   //{
   //   
   //   // ASSERT(m_hdc != nullptr);
   //   //return ::ScrollDC(m_hdc,Δx,Δy,&rectangleClip,
   //     //                &rectangleClip, (HRGN)pRgnUpdate->get_os_data(), lpRectUpdate) != false;
   //   return 0;

   //}


   // Printer Escape Functions
   int graphics::Escape(int nEscape, int nCount, const ::scoped_string& lpszInData, void * lpOutData)
   {
      // ASSERT(m_hdc != nullptr);
      //return ::Escape(m_hdc, nEscape, nCount, lpszInData, lpOutData);
      return 0;

   }


   // graphics 3.1 Specific functions
   unsigned int graphics::SetBoundsRect(const ::f64_rectangle& rectangleBounds, unsigned int flags)
   {
      // ASSERT(m_hdc != nullptr);
      //return ::SetBoundsRect(m_hdc, &rectangleBounds, flags);
      return 0;

   }


   unsigned int graphics::GetBoundsRect(::f64_rectangle* rectangleBounds, unsigned int flags)
   {
      // ASSERT(m_hdc != nullptr);
      //return ::GetBoundsRect(m_hdc, rectangleBounds, flags);
      return 0;

   }


   void graphics::resize(const ::i32_size& sizeWindow)
   {

      m_sizeWindow = sizeWindow;

      bool bYSwap = m_papplication->m_gpu.m_bUseSwapChainWindow;

      ::opengl::resize(sizeWindow, bYSwap);

   }



   //bool graphics::ResetDC(const DEVMODE* lpDevMode)
   //{
   //   // ASSERT(m_hdc != nullptr);
   //   //return ::ResetDC(m_hdc, lpDevMode) != nullptr;
   //   return false;

   //}


   //unsigned int graphics::GetOutlineTextMetrics(unsigned int cbData, LPOUTLINETEXTMETRICW lpotm) const
   //{
   //   // ASSERT(m_hdc != nullptr);
   //   //return ::GetOutlineTextMetricsW(m_hdc, cbData, lpotm);
   //   return 0;
   //}


   //bool graphics::GetCharABCWidths(unsigned int nFirstChar, unsigned int nLastChar, LPABC lpabc) const
   //{
   //   // ASSERT(m_hdc != nullptr);
   //   //return ::GetCharABCWidths(m_hdc, nFirstChar, nLastChar, lpabc) != false;
   //   return false;

   //}


   //unsigned int graphics::GetFontData(unsigned int dwTable, unsigned int dwOffset, void * lpData,
   //                               unsigned int cbData) const
   //{
   //   // ASSERT(m_hdc != nullptr);
   //   //return ::GetFontData(m_hdc, dwTable, dwOffset, lpData, cbData);
   //   return 0;

   //}


   //double graphics::GetKerningPairs(double nPairs, LPKERNINGPAIR lpkrnpair) const
   //{
   //   // ASSERT(m_hdc != nullptr);
   //   //return ::GetKerningPairs(m_hdc, nPairs, lpkrnpair);
   //   return 0;

   //}


   //unsigned int graphics::GetGlyphOutline(unsigned int nChar, const ::e_align & ealign, const ::e_draw_text & edrawtext, LPGLYPHMETRICS lpgm,
   //                                   unsigned int cbBuffer, void * lpBuffer, const MAT2* lpmat2) const
   //{
   //   // ASSERT(m_hdc != nullptr);
   //   //return ::GetGlyphOutline(m_hdc, nChar, nFormat,
   //     //                       lpgm, cbBuffer, lpBuffer, lpmat2);
   //   return 0;

   //}


//    // ::user::document handling functions
//    int graphics::StartDoc(LPDOCINFO lpDocInfo)
//    {
//
//       //ASSERT(m_hdc != nullptr);
//
//       //return ::StartDoc(m_hdc, lpDocInfo);
//       return 0;
//
//    }
//
//
//    int graphics::StartPage()
//    {
//
//       //ASSERT(m_hdc != nullptr);
//
//       //::StartPage(m_hdc);
//
//       //m_pgraphics = ___new plusplus::Graphics (m_hdc);
//
//       //m_pgraphics->SetPageUnit(plusplus::UnitPixel);
//
//       return 1;
//
//    }
//
//
//    int graphics::EndPage()
//    {
//
//       /* ASSERT(m_hdc != nullptr);
//
//        delete m_pgraphics;
//
//       */
//       //return ::EndPage(m_hdc);
//       return 0;
//
//    }
//
//
//    int graphics::SetAbortProc(bool (CALLBACK* lpfn)(HDC, int))
//    {
//
//       //ASSERT(m_hdc != nullptr);
//
//       //return ::SetAbortProc(m_hdc, (ABORTPROC)lpfn);
//       return 0;
//
//    }
//
//
//    int graphics::AbortDoc()
//    {
//
//       //ASSERT(m_hdc != nullptr);
//
//       //return ::AbortDoc(m_hdc);
//       return 0;
//
//    }
//
//
//    int graphics::EndDoc()
//    {
//
//       //ASSERT(m_hdc != nullptr);
//
//       //return ::EndDoc(m_hdc);
//       return 0;
//
//    }


   //   bool graphics::MaskBlt(double x, double y, double nWidth, double nHeight, ::draw2d::graphics * pgraphicsSrc,
   //                          double xSrc, double ySrc, ::draw2d::bitmap& maskBitmap, double xMask, double yMask)
   //   {
   //      // ASSERT(m_hdc != nullptr);
   //      //return ::MaskBlt(m_hdc, x, y, nWidth, nHeight, VK2D_HDC(pgraphicsSrc),
   //        //               xSrc, ySrc,  (HBITMAP)maskBitmap.get_os_data(), xMask, yMask, dwRop) != false;
   //      return 0;
   //
   //   }
   //
   //
   //   bool graphics::PlgBlt(::f64_point * lpPoint, ::draw2d::graphics * pgraphicsSrc, double xSrc, double ySrc,
   //                         double nWidth, double nHeight, ::draw2d::bitmap& maskBitmap, double xMask, double yMask)
   //   {
   //
   //      /*try
   //      {
   //
   //         if(pgraphicsSrc == nullptr)
   //            return false;
   //
   //         if(pgraphicsSrc->get_current_bitmap() == nullptr)
   //            return false;
   //
   //         if(pgraphicsSrc->get_current_bitmap()->get_os_data() == nullptr)
   //            return false;
   //
   //         plusplus::Point p[3];
   //
   //         p[0].X = lpPoint[0].x;
   //         p[0].Y = lpPoint[0].y;
   //         p[1].X = lpPoint[1].x;
   //         p[1].Y = lpPoint[1].y;
   //         p[2].X = lpPoint[2].x;
   //         p[2].Y = lpPoint[2].y;
   //
   //         return m_pgraphics->DrawImage((plusplus::Bitmap *) pgraphicsSrc->get_current_bitmap()->get_os_data(), p, 3) == plusplus::Status::Ok;
   //
   //      }
   //      catch(...)
   //      {
   //         return false;
   //      }
   //      */
   //      return true;
   //
   //   }



   void graphics::SetPixelV(int x, int y, ::color::color crColor)
   {
      // ASSERT(m_hdc != nullptr);
      //return ::SetPixelV(m_hdc, x, y, crColor) != false;
      //return 0;

   }


   void graphics::SetPixelV(const ::i32_point& point, ::color::color crColor)
   {
      // ASSERT(m_hdc != nullptr);
      //return ::SetPixelV(m_hdc, point.x, point.y, crColor) != false;
      //return 0;

   }


   void graphics::angle_arc(double x, double y, double nRadius, ::f64_angle fStartAngle, ::f64_angle fSweepAngle)
   {
      // ASSERT(m_hdc != nullptr);
      //return ::AngleArc(m_hdc, x, y, nRadius, fStartAngle, fSweepAngle) != false;
      //return 0;

   }



   void graphics::arc_to(const ::i32_rectangle& rectangleParam, const ::i32_point& pointStart, const ::i32_point& pointEnd)
   {
      // ASSERT(m_hdc != nullptr);
      //return ArcTo(rectangleParam.left, rectangleParam.top, rectangleParam.right,
        //           rectangleParam.bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);

      //return false;

   }


   //int graphics::GetArcDirection() const
   //{
   //   // ASSERT(m_hdc != nullptr);
   //   // return ::GetArcDirection(m_hdc);
   //   return 0;

   //}


   void graphics::poly_polyline(const ::f64_point* lpPoints, const int* lpPolyPoints, ::collection::count nCount)
   {

      // ASSERT(m_hdc != nullptr);

      //return ::poly_polyline(m_hdc, lpPoints, (LPDWORD) lpPolyPoints, (unsigned int) nCount) != false;
      //return false;

   }


   // bool graphics::GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust) const
   // {
   //    // ASSERT(m_hdc != nullptr);
   //    //return ::GetColorAdjustment(m_hdc, lpColorAdjust) != false;
   //    return false;
   //
   // }


   ::draw2d::pen* graphics::get_current_pen()
   {

      return m_ppen;

   }

   ::draw2d::brush* graphics::get_current_brush()
   {

      return m_pbrush;

   }

   //::draw2d::palette_pointer graphics::get_current_palette()
   //{

   //   return (::draw2d::palette *)nullptr;

   //}

   ::write_text::font* graphics::get_current_font()
   {

      return m_pfont;

   }

   ::draw2d::bitmap* graphics::get_current_bitmap()
   {

      return m_pbitmap;

   }


   void graphics::poly_bezier(const ::f64_point* lpPoints, ::collection::count nCount)
   {

      // ASSERT(m_hdc != nullptr);

      //return ::poly_bezier(m_hdc, lpPoints, (unsigned int) nCount) != false;
      //return false;

   }


   int graphics::DrawEscape(int nEscape, int nInputSize, const ::scoped_string& lpszInputData)
   {

      // ASSERT(m_hdc != nullptr);

      //return ::DrawEscape(m_hdc, nEscape, nInputSize, lpszInputData);
      return 0;


   }


   int graphics::Escape(int nEscape, int nInputSize, const char* lpszInputData, int nOutputSize, char* lpszOutputData)
   {
      // ASSERT(m_hdc != nullptr);
      //return ::ExtEscape(m_hdc, nEscape, nInputSize, lpszInputData, nOutputSize, lpszOutputData);
      return 0;

   }


   //bool graphics::GetCharABCWidths(unsigned int nFirstChar, unsigned int nLastChar,
   //                                LPABCFLOAT lpABCF) const
   //{
   //   // ASSERT(m_hdc != nullptr);
   //   //return ::GetCharABCWidthsFloat(m_hdc, nFirstChar, nLastChar, lpABCF) != false;
   //   return false;
   //}


   //bool graphics::GetCharWidth(unsigned int nFirstChar, unsigned int nLastChar, float* lpFloatBuffer) const
   //{
   //   // ASSERT(m_hdc != nullptr);
   //   //return ::GetCharWidthFloat(m_hdc, nFirstChar, nLastChar, lpFloatBuffer) != false;
   //   return false;

   //}


   void graphics::abort_path()
   {
      //if(m_ppath != nullptr)
      //{
      //   delete m_ppath;
      //   m_ppath = nullptr;
      //}
      //return true;

   }


   void graphics::begin_path()
   {

      /*     if(m_ppath != nullptr)
              delete m_ppath;

           m_ppath = ___new plusplus::GraphicsPath ();
      */
      //      return m_ppath != nullptr;

            //return true;

   }


   void graphics::close_figure()
   {
      //ASSERT(m_ppath != nullptr);
      //return m_ppath->CloseFigure() == plusplus::Status::Ok;
      //return true;

   }


   void graphics::end_path()
   {

      //if(m_ppath == nullptr)
      //   return false;

      //m_ppathPaint   = m_ppath;
      //m_ppath        = nullptr;

      //return true;

   }


   void graphics::fill_path()
   {

      //return m_pgraphics->FillPath(vk2d_brush(), m_ppath) == plusplus::Status::Ok;
      //return true;

   }


   void graphics::flatten_path()
   {
      //      return m_ppath->Flatten() == plusplus::Status::Ok;
            //return true;

   }


   void graphics::stroke_and_fill_path()
   {


   }


   void graphics::stroke_path()
   {


   }


   void graphics::widen_path()
   {


   }


   float graphics::GetMiterLimit()
   {
      // ASSERT(m_hdc != nullptr);
      //float fMiterLimit;
      //VERIFY(::GetMiterLimit(m_hdc, &fMiterLimit));
      //::GetMiterLimit(m_hdc, &fMiterLimit);
      //;;; return fMiterLimit;
      return 0.f;
   }


   //void graphics::GetPath(::f64_point * lpPoints, unsigned char * lpTypes, ::collection::count nCount)
   //{

   //   // ASSERT(m_hdc != nullptr);

   //   //return ::GetPath(m_hdc, lpPoints, lpTypes, (int) nCount);

   //   ::GetPath(m_hdc, lpPoints, lpTypes, (int)nCount);

   //}


   void graphics::SetMiterLimit(float fMiterLimit)
   {
      // ASSERT(m_hdc != nullptr);
      ///return ::SetMiterLimit(m_hdc, fMiterLimit, nullptr) != false;
      //::SetMiterLimit(m_hdc, fMiterLimit, nullptr);

   }


   //void graphics::StrokeAndFillPath()
   //{

   //   //bool bOk1 = m_pgraphics->FillPath(vk2d_brush(), m_ppathPaint) == plusplus::Status::Ok;

   //   //bool bOk2 = m_pgraphics->DrawPath(vk2d_pen(), m_ppathPaint) == plusplus::Status::Ok;

   //   //return bOk1 && bOk2;

   //   //return true;

   //}


//   void graphics::StrokePath()
//   {
//
////      return m_pgraphics->DrawPath(vk2d_pen(), m_ppathPaint) == plusplus::Status::Ok;
//
//      //return true;
//
//   }


   //void graphics::WidenPath()
   //{

   //   //return m_ppath->Widen(vk2d_pen()) == plusplus::Status::Ok;

   //   //return true;

   //}


   void graphics::draw(::draw2d::path* ppath)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      if (!_set(ppath))
      {

         throw ::exception(error_failed);

      }

      draw();

   }


   void graphics::draw(::draw2d::path* ppath, ::draw2d::pen* ppen)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      if (!_set(ppath))
      {

         throw ::exception(error_failed);

      }

      //return
      draw(ppen);

   }

   bool graphics::_set(::draw2d::path* ppath)
   {

      if (::is_null(ppath) || ppath->m_itema.is_empty())
      {

         return false;

      }

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      nanovg_keep keep(m_pdc);

      nvgBeginPath(m_pdc);

      m_bHasCurrentPoint = false;

      //if (!m_bOutline)
      {

         //if (ppath->m_efillmode == ::draw2d::e_fill_mode_alternate)
         //{

         //   nanovg_set_fill_rule(m_pdc, VKVG_FILL_RULE_EVEN_ODD);

         //}
         //else
         //{

         //   nanovg_set_fill_rule(m_pdc, VKVG_FILL_RULE_NON_ZERO);

         //}

      }

      for (int i = 0; i < ppath->m_itema.get_count(); i++)
      {

         _set(ppath->m_itema[i]);

      }

      return true;

   }


   bool graphics::_set(::geometry2d::item* pitem)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      auto eitem = pitem->type();

      switch (eitem)
      {
      case ::draw2d::e_item_begin_figure:
         return _set(::draw2d::e_item_begin_figure);
      case ::draw2d::e_item_close_figure:
         return _set(::draw2d::e_item_close_figure);
      case ::draw2d::e_item_end_figure:
         return _set(::draw2d::e_item_end_figure);
      case ::draw2d::e_item_arc:
         return _set(pitem->cast <::geometry2d::arc_item>()->m_item);
         //case ::draw2d::e_item_line:
         //   return _set(pshape->shape < ::line > ());
      case ::draw2d::e_item_line:
         return _set(pitem->cast <::geometry2d::line_item>()->m_item);
         //case ::draw2d::e_item_lines:
         //   return _set(pshape->shape < ::lines > ());
      case ::draw2d::e_item_lines:
         return _set(pitem->cast <::geometry2d::lines_item>()->m_item);
         //case ::draw2d::e_item_rect:
         //   return _set(pshape->shape < ::i32_rectangle > ());
      case ::draw2d::e_item_rectangle:
         return _set(pitem->cast <::geometry2d::rectangle_item>()->m_item);
         //case ::draw2d::e_item_polygon:
         //   return _set(pshape->shape < ::int_polygon > ());
      case ::draw2d::e_item_ellipse:
         return _set(pitem->cast <::geometry2d::ellipse_item>()->m_item);
      case ::draw2d::e_item_polygon:
         return _set(pitem->cast <::geometry2d::polygon_item>()->m_polygon);
      case ::draw2d::e_item_text_out:
         return _set(pitem->cast <::geometry2d::text_out_item>()->m_item);
      case ::draw2d::e_item_draw_text:
         return _set(pitem->cast <::geometry2d::draw_text_item>()->m_item);
      default:
         throw "unexpected simple os graphics matter type";
      }

      return false;

   }


   bool graphics::_set(const ::draw2d::enum_item& eitem)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      if (eitem == ::draw2d::e_item_begin_figure)
      {

         //if (cairo_has_current_point(m_pdc))
         if(m_bHasCurrentPoint)
         {

            //cairo_new_sub_path(m_pdc);

            m_bHasCurrentPoint = false;

         }

         return true;

      }
      else if (eitem == ::draw2d::e_item_close_figure)
      {

         nvgClosePath(m_pdc);
         //cairo_close_path(m_pdc);

         //cairo_new_sub_path(m_pdc);

         return true;

      }
      else if (eitem == ::draw2d::e_item_end_figure)
      {

         return true;

      }
      else
      {

         return false;

      }

   }


   bool graphics::_set(const ::f64_arc& arc, const ::pointer<::draw2d::region>& pregion)
   {

      return _set(arc);

   }


   //bool _set(const ::line & line);
   //bool _set(const ::lines & lines);
   bool graphics::_set(const ::f64_rectangle& rectangle, const ::pointer<::draw2d::region>& pregion)
   {

      return _set(rectangle);

   }


   bool graphics::_set(const ::f64_ellipse& ellipse, const ::pointer<::draw2d::region>& pregion)
   {

      return _set(ellipse);

   }


   bool graphics::_set(const ::f64_polygon& polygon, const ::pointer<::draw2d::region>& pregion)
   {

      return _set(polygon);

   }


   bool graphics::_set(const ::write_text::text_out& textout, const ::pointer<::draw2d::region>& pregion)
   {

      return _set(textout);

   }


   bool graphics::_set(const ::write_text::draw_text& drawtext, const ::pointer<::draw2d::region>& pregion)
   {

      return _set(drawtext);

   }


   bool graphics::_set(const ::f64_arc& arc, const ::pointer<::draw2d::path>& ppath)
   {

      return _set(arc);

   }


   bool graphics::_set(const ::f64_line& line, const ::pointer<::draw2d::path>& ppath)
   {

      return _set(line);

   }


   bool graphics::_set(const ::f64_lines& lines, const ::pointer<::draw2d::path>& ppath)
   {

      return _set(lines);

   }


   bool graphics::_set(const ::f64_rectangle& rectangle, const ::pointer<::draw2d::path>& ppath)
   {

      return _set(rectangle);

   }


   bool graphics::_set(const ::f64_ellipse& ellipse, const ::pointer<::draw2d::path>& ppath)
   {

      return _set(ellipse);

   }


   bool graphics::_set(const ::f64_polygon& polygon, const ::pointer<::draw2d::path>& ppath)
   {

      return _set(polygon);

   }


   bool graphics::_set(const ::write_text::text_out& textout, const ::pointer<::draw2d::path>& ppath)
   {

      return _set(textout);

   }


   bool graphics::_set(const ::write_text::draw_text& drawtext, const ::pointer<::draw2d::path>& ppath)
   {

      return _set(drawtext);

   }


   bool graphics::_set(const ::f64_arc& arc)
   {

      if (arc.radius().cx <= 0.0000001)
      {

         return false;

      }

      if (arc.radius().cy <= 0.0000001)
      {

         return 0;

      }

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      nanovg_keep keep(m_pdc);

      nvgTranslate(m_pdc, (float)arc.center().x, (float)arc.center().y);

      nvgScale(m_pdc, 1.0, (float)(arc.radius().cy / arc.radius().cx));

      if (arc.m_angleExt > 0)
      {

         nvgArc(m_pdc, (float)0.0, (float)0.0, (float)arc.radius().cx, (float)arc.m_angleBeg, (float)arc.m_angleEnd2, NVG_CCW);

      }
      else
      {

         nvgArc(m_pdc, (float)0.0, (float)0.0, (float)arc.radius().cx, (float)arc.m_angleBeg, (float)arc.m_angleEnd2, NVG_CW);

      }

      return true;

   }


   //bool graphics::_set(const ::line & line)
   //{
   //
   //    _synchronous_lock ml(::draw2d_nanovg::mutex());
   //
   //    if (nanovg_has_current_point(m_pdc))
   //    {
   //
   //      double x;
   //
   //      double y;
   //
   //      nanovg_get_current_point (m_pdc, &x, &y);
   //
   //      if(x != line.m_p1.x || y != line.m_p1.y)
   //      {
   //
   //         nanovg_move_to(m_pdc, line.m_p1.x, line.m_p1.y);
   //
   //      }
   //      else
   //      {
   //
   //         nanovg_line_to(m_pdc, line.m_p1.x, line.m_p1.y);
   //
   //      }
   //
   //    }
   //    else
   //    {
   //
   //      nanovg_move_to(m_pdc, line.m_p1.x, line.m_p1.y);
   //
   //    }
   //
   //    nanovg_line_to(m_pdc, line.m_p2.x, line.m_p2.y);
   //
   //    return true;
   //
   //}


   bool graphics::_set(const ::f64_line& line)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      if (m_bHasCurrentPoint)
      {

         float x = (float)m_point.x;

         float y = (float)m_point.y;

         //nanovg_get_current_point(m_pdc, &x, &y);

         if (is_different(x, line.m_p1.x, 0.0001) || is_different(y, line.m_p1.y, 0.0001))
         {

            nvgLineTo(m_pdc, (float)line.m_p1.x, (float)line.m_p1.y);

         }

      }
      else
      {

         nvgMoveTo(m_pdc, (float)line.m_p1.x, (float)line.m_p1.y);

      }

      nvgLineTo(m_pdc, (float)line.m_p2.x, (float)line.m_p2.y);

      m_point.x = line.m_p2.x;

      m_point.y = line.m_p2.y;

      m_bHasCurrentPoint = true;

      return true;

   }


   bool graphics::_set(const ::i32_point_array& pointa)
   {

      if (pointa.get_count() <= 1)
      {

         return true;

      }

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      if (m_bHasCurrentPoint)
      {

         float x = (float)m_point.x;

         float y = (float)m_point.y;

         //nanovg_get_current_point(m_pdc, &x, &y);

         if (x != pointa[0].x || y != pointa[0].y)
         {

            nvgMoveTo(m_pdc, (float)pointa[0].x, (float)pointa[0].y);

         }
         else
         {

            nvgLineTo(m_pdc, (float)pointa[0].x, (float)pointa[0].y);

         }

      }
      else
      {

         nvgMoveTo(m_pdc, (float)pointa[0].x, (float)pointa[0].y);

      }

      for (::collection::index i = 1; i < pointa.get_count(); i++)
      {

         nvgLineTo(m_pdc, (float)pointa[i].x, (float)pointa[i].y);

      }

      return true;

   }


   bool graphics::_set(const ::f64_point_array& pointa)
   {

      if (pointa.get_count() <= 1)
      {

         return true;

      }

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      if (m_bHasCurrentPoint)
      {

         float x = (float)m_point.x;

         float y = (float)m_point.y;

         if (x != pointa[0].x || y != pointa[0].y)
         {

            nvgMoveTo(m_pdc, (float)pointa[0].x, (float)pointa[0].y);

         }
         else
         {

            nvgLineTo(m_pdc, (float)pointa[0].x, (float)pointa[0].y);

         }

      }
      else
      {

         nvgMoveTo(m_pdc, (float)pointa[0].x, (float)pointa[0].y);

      }

      for (::collection::index i = 1; i < pointa.get_count(); i++)
      {

         nvgLineTo(m_pdc, (float)pointa[i].x, (float)pointa[i].y);

      }

      return true;

   }


   //bool graphics::_set(const lines & lines)
   //{
   //
   //   if(lines.get_count() <= 1)
   //   {
   //
   //      return true;
   //
   //   }
   //
   //   _synchronous_lock ml(::draw2d_nanovg::mutex());
   //
   //   nanovg_new_sub_path(m_pdc);
   //
   //   _set((const ::i32_point_array &) lines);
   //
   //   return true;
   //
   //}


   bool graphics::_set(const f64_lines& lines)
   {

      if (lines.get_count() <= 1)
      {

         return true;

      }

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      nvgBeginPath(m_pdc);

      _set((const ::f64_point_array&)lines);

      return true;

   }


   //bool graphics::_set(const ::int_polygon & int_polygon)
   //{
   //
   //   if(int_polygon.get_count() <= 1)
   //   {
   //
   //      return true;
   //
   //   }
   //
   //   _synchronous_lock ml(::draw2d_nanovg::mutex());
   //
   //   nanovg_new_sub_path(m_pdc);
   //
   //   _set((const ::i32_point_array &) int_polygon);
   //
   //   nanovg_close_path(m_pdc);
   //
   //   return true;
   //
   //}


   bool graphics::_set(const ::f64_polygon& int_polygon)
   {

      if (int_polygon.get_count() <= 1)
      {

         return true;

      }

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      nvgBeginPath(m_pdc);

      _set((const ::f64_point_array&)int_polygon);

      nvgClosePath(m_pdc);

      return true;

   }


   //bool graphics::_set(const ::i32_rectangle & rectangle)
   //{
   //
   //    _synchronous_lock ml(::draw2d_nanovg::mutex());
   //
   //    nanovg_rectangle(
   //      m_pdc,
   //      rectangle.left,
   //      rectangle.top,
   //      rectangle.width(),
   //      rectangle.height());
   //
   //    return true;
   //
   //}


   bool graphics::_set(const ::f64_rectangle& rectangle)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      nvgRect(
         m_pdc,
         (float)rectangle.left,
         (float)rectangle.top,
         (float)rectangle.width(),
         (float)rectangle.height());

      return true;

   }


   bool graphics::_set(const ::f64_ellipse& ellipse)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      double Δx = ellipse.center_x();

      double Δy = ellipse.center_y();

      nvgTranslate(m_pdc, (float)Δx, (float)Δy);

      double rx = ellipse.width() / 2.0;

      double ry = ellipse.height() / 2.0;

      if (rx == 0.0 || ry == 0.0)
      {

         return false;

      }

      nvgScale(m_pdc, (float)rx, (float)ry);

      nvgArc(m_pdc, (float)0.0, (float)0.0, (float)1.0, (float)0.0, (float)(2.0 * π), NVG_CCW);

      nvgScale(m_pdc, (float)(1.0 / rx), (float)(1.0 / ry));

      nvgTranslate(m_pdc, (float)-Δx, (float)-Δy);

      return true;

   }


   bool graphics::_set(const ::write_text::text_out& textout)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      auto rectangle = ::f64_rectangle(textout.m_point, f64_size(65535.0, 65535.0));

      internal_draw_text(textout.m_strText, rectangle, e_align_top_left, e_draw_text_none);

      //#if defined(USE_PANGO)
      //
      //      internal_draw_text_pango(textout.m_strText, rectangle, e_align_top_left, e_draw_text_none,
      //                               &pango_nanovg_layout_path);
      //
      //#else
      //
      //      internal_draw_text(textout.m_strText, rectangle, e_align_top_left, e_draw_text_none, &nanovg_text_path);
      //
      //#endif

      //nanovg_status_t status = nanovg_status(m_pdc);

      //if (status != VKVG_STATUS_SUCCESS)
      //{

      //   const ::ansi_character* pszStatus = nanovg_status_to_string(status);

      //   informationf("nanovg error : graphics::set(string_path) %d %s", status, pszStatus);

      //}

      return true;

   }


   bool graphics::_set(const ::write_text::draw_text& drawtext)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      auto rectangle = drawtext.m_rectangle;

      internal_draw_text(drawtext.m_strText, rectangle, e_align_top_left, e_draw_text_none);

      //#if defined(USE_PANGO)
      //
      //      internal_draw_text_pango(drawtext.m_strText, rectangle, e_align_top_left, e_draw_text_none,
      //                               &pango_nanovg_layout_path);
      //
      //#else
      //
      //      internal_draw_text(drawtext.m_strText, rectangle, e_align_top_left, e_draw_text_none, &nanovg_text_path);
      //
      //#endif

      //nanovg_status_t status = nanovg_status(m_pdc);

      //if (status != VKVG_STATUS_SUCCESS)
      //{

      //   const ::ansi_character* pszStatus = nanovg_status_to_string(status);

      //   informationf("nanovg error : graphics::set(string_path) %d %s", status, pszStatus);

      //}

      return true;

   }


   //bool graphics::_set(::draw2d_nanovg::path::close * pclose)
   //{
   //
   //   return true;
   //
   //}


   //void graphics::set(const ::draw2d_nanovg::path::transfer & p)
   //{
   //
   //    _synchronous_lock ml(::draw2d_nanovg::mutex());
   //
   //    nanovg_move_to(m_pdc, p.m_x + 0.5, p.m_y + 0.5);
   //
   //    return true;
   //
   //}




   void graphics::fill(::draw2d::path* ppath)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      if (!_set(ppath))
      {

         throw ::exception(error_failed);

      }

      //return
      fill();

   }



   void graphics::fill(::draw2d::path* ppath, ::draw2d::brush* pbrush)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      if (!_set(ppath))
      {

         throw ::exception(error_failed);

      }

      //return
      fill(pbrush);

   }


   //bool graphics::fill_and_draw()
   //{

   //   _synchronous_lock ml(::draw2d_nanovg::mutex());

   //   bool bPen = m_ppen->m_epen != ::draw2d::e_pen_null;

   //   nanovg_keep keep(m_pdc);

   //   if (m_pbrush->m_ebrush != ::draw2d::e_brush_null)
   //   {

   //      _set(m_pbrush);

   //      set_alpha_mode(m_ealphamode);

   //      if (bPen)
   //      {

   //         nanovg_fill_preserve(m_pdc);

   //      }
   //      else
   //      {

   //         nanovg_fill(m_pdc);

   //      }

   //   }

   //   keep.pulse();

   //   if (bPen)
   //   {

   //      _set(m_ppen);

   //      set_alpha_mode(m_ealphamode);

   //      nvgStroke(m_pdc);

   //   }

   //   return true;

   //}


   //bool graphics::fill(::draw2d::brush* pbrush, double xOrg, double yOrg)
   //{

   //   _synchronous_lock ml(::draw2d_nanovg::mutex());

   //   if (pbrush == nullptr || pbrush->m_ebrush == ::draw2d::e_brush_null)
   //   {

   //      return true;

   //   }

   //   _fill1(pbrush, xOrg, yOrg);

   //   nanovg_fill(m_pdc);

   //   _fill2(pbrush, xOrg, yOrg);

   //   return true;

   //}


   //bool graphics::_fill1(::draw2d::brush* pbrush, double xOrg, double yOrg)
   //{

   //   if (pbrush == nullptr || pbrush->m_ebrush == ::draw2d::e_brush_null)
   //   {

   //      return true;

   //   }

   //   if (m_pregion.is_set() && !m_pregion.cast<region>()->is_simple_positive_region())
   //   {

   //      nanovg_set_antialias(m_pdc, VKVG_ANTIALIAS_BEST);

   //      nanovg_push_group(m_pdc);

   //      _set(pbrush, xOrg, yOrg);

   //   }
   //   else
   //   {

   //      _set(pbrush, xOrg, yOrg);

   //   }

   //   return true;

   //}


   //bool graphics::_fill2(::draw2d::brush* pbrush, double xOrg, double yOrg)
   //{

   //   if (pbrush == nullptr || pbrush->m_ebrush == ::draw2d::e_brush_null)
   //   {

   //      return true;

   //   }

   //   if (m_pregion.is_set() && !m_pregion.cast<region>()->is_simple_positive_region())
   //   {

   //      nanovg_pop_group_to_source(m_pdc);

   //      m_pregion.cast<region>()->mask_fill(m_pdc);

   //   }

   //   return true;

   //}


   bool graphics::fill(double xOrg, double yOrg)
   {

      return fill(m_pbrush, xOrg, yOrg);

   }


   bool graphics::_fill1(double xOrg, double yOrg)
   {

      return _fill1(m_pbrush, xOrg, yOrg);

   }


   bool graphics::_fill2(double xOrg, double yOrg)
   {

      return _fill2(m_pbrush, xOrg, yOrg);

   }


   bool graphics::draw(::draw2d::pen* ppen)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      if (ppen == nullptr || ppen->m_epen == ::draw2d::e_pen_null)
      {

         return true;

      }

      nanovg_keep keep(m_pdc);

      _set(ppen);

      nvgStroke(m_pdc);

      return true;

   }



   void graphics::draw()
   {

      draw(m_ppen);

   }



   //bool graphics::draw(::draw2d::pen* ppen)
   //{

   //   _synchronous_lock ml(::draw2d_nanovg::mutex());

   //   if (ppen == nullptr || ppen->m_epen == ::draw2d::e_pen_null)
   //   {

   //      return true;

   //   }

   //   //nanovg_keep keep(m_pdc);

   //   _set(ppen);

   //   nvgStroke(m_pdc);

   //   return true;

   //}


   bool graphics::_set(::draw2d::pen* ppen)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      if (ppen->m_epen == ::draw2d::e_pen_brush)
      {

         _set(ppen->m_pbrush);

      }
      else
      {

         nvgStrokeColor(m_pdc, as_nvg_color(ppen->m_color));

      }

      if (ppen->m_elinecapBeg == ::draw2d::e_line_cap_round
         && ppen->m_elinecapEnd == ::draw2d::e_line_cap_round)
      {

         nvgLineCap(m_pdc, NVG_ROUND);

      }
      else if (ppen->m_elinecapBeg == ::draw2d::e_line_cap_flat
         && ppen->m_elinecapEnd == ::draw2d::e_line_cap_flat)
      {

         nvgLineCap(m_pdc, NVG_BUTT);

      }

      nvgStrokeWidth(m_pdc, ppen->m_dWidth);

      return true;

   }




   //void graphics::AddMetaFileComment(unsigned int nDataSize, const unsigned char* pCommentData)
   //{
   //   // ASSERT(m_hdc != nullptr);
   //   //return ::GdiComment(m_hdc, nDataSize, pCommentData) != false;
   //   //return false;

   //}


   /*bool CALLBACK metaCallback(
      EmfPlusRecordType recordType,
      unsigned int flags,
      unsigned int dataSize,
      const uchar* pStr,
      void* callbackData)
   {
      // Play only EmfPlusRecordTypeFillEllipse records.
      if (recordType == EmfPlusRecordTypeFillEllipse)
      {
      // Explicitly cast callbackData as a metafile pointer, and use it to call
      // the PlayRecord method.
      static_cast < Metafile* > (callbackData)->PlayRecord(recordType, flags, dataSize, pStr);
      }
      return true;
   }

   VOID Example_EnumerateMetafile9(HDC hdc)
   {
      Graphics graphics(hdc);
      // Create a Metafile object from an existing disk metafile.
      Metafile* pMeta = ___new Metafile(L"SampleMetafile.emf", hdc);
      {
         // Fill a ::f64_rectangle and an ellipse in pMeta.
         Graphics metaGraphics(pMeta);
         metaGraphics.FillRectangle(&SolidBrush(Color(255, 0, 0, 0)), 0, 0, 100, 100);
     metaGraphics.FillEllipse(&SolidBrush(Color(255, 255, 0, 0)), 100, 0, 200, 100);
      }
      // Enumerate pMeta to the destination rectangle, passing pMeta as the callback data.
      graphics.EnumerateMetafile(
      pMeta,
      Rect(0, 0, 300, 50),
      metaCallback,
      pMeta);
      // Draw pMeta as an pimage->
      graphics.DrawImage(pMeta, Point(0, 150));
      delete pMeta;;
   }*/


   //bool graphics::PlayMetaFile(HENHMETAFILE hEnhMF, const ::f64_rectangle & rectangleBounds)
   //{

   //   //plusplus::rectF ::f64_rectangle((plusplus::REAL) rectangleBounds.left,(plusplus::REAL) rectangleBounds.top,(plusplus::REAL) width(rectangleBounds),(plusplus::REAL) height(rectangleBounds));

   //   //plusplus::Metafile* pMeta = ___new plusplus::Metafile (hEnhMF, false);

   //   ////m_pgraphcis->EnumerateMetafile(pMeta, rectangle, metaCallback, PMETAHEADER);

   //   //bool bOk = m_pgraphics->DrawImage(pMeta, rectangle) == plusplus::Status::Ok;

   //   //delete pMeta;

   //   //return bOk ? true : false;
   //   //return ::PlayEnhMetaFile(m_hdc, hEnhMF, lpBounds);

   //   return true;

   //}

   // true blend
   // COLOR_DEST = SRC_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA) * COLOR_DST

   // double blend
   // COLOR_DEST = SRC_ALPHA * BLEND_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA * BLEND_ALPHA) * COLOR_DST

// Thank you
// Jiju George T
// Web Developer
// India India
// Member


//   void graphics::alpha_blend(double xDest, double yDest, double nDestWidth, double nDestHeight, ::draw2d::graphics * pgraphicsSrc, double xSrc, double ySrc, double nSrcWidth, double nSrcHeight, double dRate)
//   {
//
////      if (m_pgraphics == nullptr)
////         return false;
////
////      if(m_pimageAlphaBlend->is_set())
////      {
////
////
////         ::f64_rectangle rectangleIntersect(m_pointAlphaBlend, m_pimageAlphaBlend->size());
////
////
////         ::image::image_pointer pimageWork = nullptr;
////         ::image::image_pointer pimageWork2 = nullptr;
//////         ::image::image_pointer pimageWork3 = nullptr;
////         ::image::image_pointer pimageWork4 = nullptr;
////
////
////         ::i32_point pointSrc(xSrc, ySrc);
////         ::i32_point ptDest(xDest, yDest);
////         ::i32_size size(nDestWidth, nDestHeight);
////
////
////
////         ::image::image_pointer pimage;
////         if(imageWork == nullptr)
////         {
////            pimage->create(this);
////            imageWork = pimage;
////         }
////         if(imageWork == nullptr)
////            return false;
////         if(!imageWork = create_image(size))
////            return false;
////         if(!imageWork.from(::i32_point(), pgraphicsSrc, pointSrc, size))
////            return false;
////
////
////
////
////         ::image::image_pointer pimage2;
////         if(imageWork2 == nullptr)
////         {
////            pimage2->create(this);
////            imageWork2 = pimage2;
////         }
////
////
////         ::image::image_pointer pimage4;
////         if(imageWork4 == nullptr)
////         {
////            image4.create(this);
////            imageWork4 = image4;
////         }
////         if(imageWork4 == nullptr)
////            return false;
////         if(!imageWork4 = create_image(size))
////            return false;
////
////
////         imageWork4.Fill(255, 0, 0, 0);
////
////         imageWork4.from(i32_point(maximum(0, m_pointAlphaBlend.x - xDest), maximum(0, m_pointAlphaBlend.y - yDest)),
////                         m_pimageAlphaBlend->get_graphics(), i32_point(maximum(0, xDest - m_pointAlphaBlend.x), maximum(0, yDest - m_pointAlphaBlend.y)), size);
////
////         imageWork.channel_multiply(::color::e_channel_opacity, imageWork4);
////
////
////         keep < image > keep(&m_pimageAlphaBlend, nullptr, m_pimageAlphaBlend, true);
////
////
////         return BitBlt(ptDest.x, ptDest.y, size.cx, size.cy, imageWork.get_graphics(), pointSrc.x, pointSrc.y);
////
////
////      }
////
////
////      float fA = (float) dRate;
////
////      plusplus::ColorMatrix matrix = {
////         1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
////         0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
////         0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
////         0.0f, 0.0f, 0.0f, fA  , 0.0f,
////         0.0f, 0.0f, 0.0f, 0.0f, 1.0f
////      };
////
////      plusplus::ImageAttributes attributes;
////
////      attributes.SetColorMatrix(&matrix, plusplus::ColorMatrixFlagsDefault, plusplus::ColorAdjustTypeBitmap);
////
////      plusplus::rectF dstRect((plusplus::REAL) xDest, (plusplus::REAL) yDest, (plusplus::REAL) nDestWidth, (plusplus::REAL) nDestHeight);
////
////      if(pgraphicsSrc == nullptr)
////         return false;
////
////      if(pgraphicsSrc->get_current_bitmap() == nullptr)
////         return false;
////
////      plusplus::Bitmap * pbitmap = nullptr;
////
////      try
////      {
////
////         pbitmap = (plusplus::Bitmap *) pgraphicsSrc->get_current_bitmap()->get_os_data();
////
////      }
////      catch(...)
////      {
////      }
////
////      if(pbitmap != nullptr)
////      {
////
////         m_pgraphics->DrawImage(pbitmap,dstRect,(plusplus::REAL) xSrc,(plusplus::REAL) ySrc,(plusplus::REAL) nSrcWidth,(plusplus::REAL) nSrcHeight,plusplus::UnitPixel,&attributes);
////
////      }
////
//      //return true;
//
//   }


   /*bool graphics::alpha_blend(double xDest, double yDest, double nDestWidth, double nDestHeight,
      ::draw2d::graphics * pgraphicsSrc, double xSrc, double ySrc, double nSrcWidth, double nSrcHeight, BLENDFUNCTION blend)
   {

      throw ::interface_only();
      //if(m_hdc == nullptr)
        // return false;


      if(m_pimageAlphaBlend->is_set())
      {


         ::f64_rectangle rectangleIntersect(m_pointAlphaBlend, m_pimageAlphaBlend->size());


         ::image::image_pointer pimageWork = nullptr;
         ::image::image_pointer pimageWork2 = nullptr;
   //         ::image::image_pointer pimageWork3 = nullptr;
         ::image::image_pointer pimageWork4 = nullptr;


         ::i32_point pointSrc(xSrc, ySrc);
         ::i32_point ptDest(xDest, yDest);
         ::i32_size size(nDestWidth, nDestHeight);



         ::image::image_pointer pimage;
         if(imageWork == nullptr)
         {
            pimage->create(this);
            imageWork = pimage;
         }
         if(imageWork == nullptr)
            return false;
         if(!imageWork = create_image(size))
            return false;
         if(!imageWork.from(::i32_point(), pgraphicsSrc, pointSrc, size))
            return false;




         ::image::image_pointer pimage2;
         if(imageWork2 == nullptr)
         {
            pimage2->create(this);
            imageWork2 = pimage2;
         }


         ::image::image_pointer pimage4;
         if(imageWork4 == nullptr)
         {
            image4.create(this);
            imageWork4 = image4;
         }
         if(imageWork4 == nullptr)
            return false;
         if(!imageWork4 = create_image(size))
            return false;


         imageWork4.Fill(255, 0, 0, 0);

         imageWork4.from(i32_point(maximum(0, m_pointAlphaBlend.x - xDest), maximum(0, m_pointAlphaBlend.y - yDest)),
            m_pimageAlphaBlend->get_graphics(), i32_point(maximum(0, xDest - m_pointAlphaBlend.x), maximum(0, yDest - m_pointAlphaBlend.y)), size);

         imageWork.channel_multiply(::color::e_channel_opacity, imageWork4);


         keep < image > keep(&m_pimageAlphaBlend, nullptr, m_pimageAlphaBlend, true);


         return psystem->imaging().true_blend(this, ptDest, size, imageWork.get_graphics(), pointSrc);


      }

      return ::AlphaBlend(m_hdc, xDest, yDest,
         nDestWidth, nDestHeight, VK2D_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth,
         nSrcHeight, blend) != false;
   }*/


   //bool graphics::TransparentBlt(double xDest, double yDest, double nDestWidth, double nDestHeight, ::draw2d::graphics * pgraphicsSrc, double xSrc, double ySrc, double nSrcWidth, double nSrcHeight, unsigned int crTransparent)
   //{

   //   // ASSERT(m_hdc != nullptr);

   //   //return ::TransparentBlt(m_hdc, xDest, yDest,  nDestWidth, nDestHeight, VK2D_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth,  nSrcHeight, crTransparent) != false;

   //   return true;

   //}


   //bool graphics::GradientFill(TRIVERTEX* pVertices, ULONG nVertices, void * pMesh, ULONG nMeshElements, unsigned int dwMode)
   //{

   //   // ASSERT(m_hdc != nullptr);

   //   //return ::GradientFill(m_hdc, pVertices, nVertices, pMesh, nMeshElements, dwMode) != false;

   //   return true;

   //}

   // This is aura API library.
   //
   //
   //
   //
   //
   //
   //
   //

   // // Always Inline. Functions only in Win98/Win2K or later
   //
   // inline color32_t graphics::GetDCBrushColor() const
   // {
   //    // ASSERT(m_hdc != nullptr);
   //    //return ::GetDCBrushColor(m_hdc);
   //    return color::transparent;
   // }


   //::gpu::frame* graphics::end_gpu_layer(::gpu::layer * pgpulayer)
   //{

   //   //nvgFlush(m_pdc);

   //   return ::gpu::graphics::end_gpu_layer(pgpulayer);

   //}


   // inline color32_t graphics::SetDCBrushColor(color32_t crColor)
   // {
   //    // ASSERT(m_hdc != nullptr);
   //    //return ::SetDCBrushColor(m_hdc, crColor);
   //    return color::transparent;
   //
   // }
   //
   //
   // inline color32_t graphics::GetDCPenColor() const
   // {
   //    // ASSERT(m_hdc != nullptr);
   //    //return ::GetDCPenColor(m_hdc);
   //    return color::transparent;
   //
   //
   // }
   //
   //
   // inline color32_t graphics::SetDCPenColor(color32_t crColor)
   // {
   //    // ASSERT(m_hdc != nullptr);
   //    //return ::SetDCPenColor(m_hdc, crColor);
   //    return color::transparent;
   //
   // }
   //
   //
   // inline bool graphics::GetCharABCWidthsI(unsigned int giFirst, unsigned int cgi, LPWORD pgi, LPABC lpabc) const
   // {
   //    // ASSERT(m_hdc != nullptr);
   //    //return ::GetCharABCWidthsI(m_hdc, giFirst, cgi, pgi, lpabc) != false;
   //    return false;
   //
   // }
   //
   //
   // inline bool graphics::GetCharWidthI(unsigned int giFirst, unsigned int cgi, LPWORD pgi, LPINT lpBuffer) const
   // {
   //    // ASSERT(m_hdc != nullptr);
   //    //return ::GetCharWidthI(m_hdc, giFirst, cgi, pgi, lpBuffer) != false;
   //    return false;
   //
   // }


   //inline bool graphics::GetTextExtentExPointI(LPWORD pgiIn, double cgi, double nMaxExtent, LPINT lpnFit, LPINT alpDx, LPSIZE32 LPSIZE32) const
   //{
   //   ENSURE(LPSIZE32 != nullptr);
   //   // ASSERT(m_hdc != nullptr);
   //   //return ::GetTextExtentExPointI(m_hdc, pgiIn, cgi, nMaxExtent, lpnFit, alpDx, LPSIZE32) != false;
   //   return false;

   //}


   //inline bool graphics::GetTextExtentPointI(LPWORD pgiIn, double cgi, LPSIZE32 LPSIZE32) const
   //{
   //   ENSURE(LPSIZE32 != nullptr);
   //   // ASSERT(m_hdc != nullptr);
   //   //return ::GetTextExtentPointI(m_hdc, pgiIn, cgi, LPSIZE32) != false;
   //   return false;

   //}


#define HIMETRIC_INCH   2540    // HIMETRIC units per inch

   void graphics::DPtoHIMETRIC(::f64_size* psize)
   {
      //      ASSERT(is_memory_segment_ok(LPSIZE32, sizeof(::i32_size)));
      //
      //      double nMapMode;
      //      if (this != nullptr && (nMapMode = GetMapMode()) < MM_ISOTROPIC &&
      //            nMapMode != MM_TEXT)
      //      {
      //         // when using a constrained map mode, map against physical inch
      //         ((::draw2d::graphics *)this)->SetMapMode(MM_HIMETRIC);
      //         DPtoLP(LPSIZE32);
      //         ((::draw2d::graphics *)this)->SetMapMode(nMapMode);
      //      }
      //      else
      //      {
      //         
      //         double cxPerInch, cyPerInch;
      //         if (this != nullptr)
      //         {
      //            ASSERT_VALID(this);
      //            // ASSERT(m_hdc != nullptr);  // no HDC attached or created?
      //            cxPerInch = GetDeviceCaps(LOGPIXELSX);
      //            cyPerInch = GetDeviceCaps(LOGPIXELSY);
      //         }
      //         else
      //         {

      //         }
      //         ASSERT(cxPerInch != 0 && cyPerInch != 0);
      //         LPSIZE32->cx = MulDiv(LPSIZE32->cx, HIMETRIC_INCH, cxPerInch);
      //         LPSIZE32->cy = MulDiv(LPSIZE32->cy, HIMETRIC_INCH, cyPerInch);
      //      }
   }


   void graphics::HIMETRICtoDP(::f64_size* psize)
   {
      //      ASSERT(is_memory_segment_ok(LPSIZE32, sizeof(::i32_size)));
      //
      //      double nMapMode;
      //      if (this != nullptr && (nMapMode = GetMapMode()) < MM_ISOTROPIC &&
      //            nMapMode != MM_TEXT)
      //      {
      //         // when using a constrained map mode, map against physical inch
      //         ((::draw2d::graphics *)this)->SetMapMode(MM_HIMETRIC);
      //         LPtoDP(LPSIZE32);
      //         ((::draw2d::graphics *)this)->SetMapMode(nMapMode);
      //      }
      //      else
      //      {
      //         
      //         double cxPerInch, cyPerInch;
      //         if (this != nullptr)
      //         {
      //            ASSERT_VALID(this);
      //            // ASSERT(m_hdc != nullptr);  // no HDC attached or created?
      //            cxPerInch = GetDeviceCaps(LOGPIXELSX);
      //            cyPerInch = GetDeviceCaps(LOGPIXELSY);
      //         }
      //         else
      //         {

      //         }
      //         ASSERT(cxPerInch != 0 && cyPerInch != 0);
      //         LPSIZE32->cx = MulDiv(LPSIZE32->cx, cxPerInch, HIMETRIC_INCH);
      //         LPSIZE32->cy = MulDiv(LPSIZE32->cy, cyPerInch, HIMETRIC_INCH);
      //      }
   }


   void graphics::LPtoHIMETRIC(::f64_size* psize)
   {
      //ASSERT(is_memory_segment_ok(LPSIZE32, sizeof(::i32_size)));

      //LPtoDP(LPSIZE32);
      //DPtoHIMETRIC(LPSIZE32);
   }


   void graphics::HIMETRICtoLP(::f64_size* psize)
   {
      //ASSERT(is_memory_segment_ok(LPSIZE32, sizeof(::i32_size)));

      //HIMETRICtoDP(LPSIZE32);
      //DPtoLP(LPSIZE32);

   }

//    /////////////////////////////////////////////////////////////////////////////
//    // special graphics drawing primitives/helpers
//
//    ::draw2d::brush* graphics::GetHalftoneBrush()
//    {
//       /*      ::aura::LockGlobals(CRIT_HALFTONEBRUSH);
//             if (gen_HalftoneBrush == nullptr)
//             {
//                unsigned short grayPattern[8];
//                for (double i = 0; i < 8; i++)
//                   grayPattern[i] = (unsigned short)(0x5555 << (i & 1));
//                HBITMAP grayBitmap = CreateBitmap(8, 8, 1, 1, grayPattern);
//                if (grayBitmap != nullptr)
//                {
//                   gen_HalftoneBrush = ::CreatePatternBrush(grayBitmap);
//                   ::DeleteObject(grayBitmap);
//                }
//             }
//             if (!gen_WingdixTerm)
//                gen_WingdixTerm = (char)!atexit(&__win_gdi_x_term);
//             ::aura::UnlockGlobals(CRIT_HALFTONEBRUSH);
//
//       //      return ::draw2d_nanovg::brush::from_handle(papp, gen_HalftoneBrush);*/
//       return nullptr;
//    }


   //void graphics::DrawDragRect(const ::f64_rectangle & rectangle, const ::i32_size & size, const ::f64_rectangle & lpRectLast, const ::i32_size & sizeLast, ::draw2d::brush* pBrush, ::draw2d::brush* pBrushLast)
   //{

   //   //throw ::interface_only();

   //   /*

   //         ASSERT(is_memory_segment_ok(rectangle, sizeof(::f64_rectangle), false));
   //         ASSERT(lpRectLast == nullptr ||
   //            is_memory_segment_ok(lpRectLast, sizeof(::f64_rectangle), false));

   //         // first, determine the update region and select it
   //         ::draw2d::region rgnNew;
   //         ::draw2d::region rgnOutside, rgnInside;
   //         rgnOutside.create_rect(rectangle);
   //         const ::f64_rectangle & rectangle = *rectangle;
   //         rectangle.inflate(-size.cx, -size.cy);
   //         rectangle.intersect(rectangle, rectangle);
   //         rgnInside.create_rect(rectangle);
   //         rgnNew.create_rect(0, 0, 0, 0);
   //         rgnNew.combine(&rgnOutside, &rgnInside, RGN_XOR);

   //         ::draw2d::brush* pBrushOld = nullptr;
   //         if (pBrush == nullptr)
   //         {
   //            pBrush = graphics::GetHalftoneBrush(this);
   //         }

   //         ENSURE(pBrush);

   //         if (pBrushLast == nullptr)
   //         {
   //            pBrushLast = pBrush;
   //         }

   //         ::draw2d::region rgnLast, rgnUpdate;
   //         if (lpRectLast != nullptr)
   //         {
   //            // find difference between ___new region and old region
   //            rgnLast.create_rect(0, 0, 0, 0);
   //            rgnOutside.SetRectRgn(lpRectLast);
   //            rectangle = *lpRectLast;
   //            rectangle.inflate(-sizeLast.cx, -sizeLast.cy);
   //            rectangle.intersect(rectangle, lpRectLast);
   //            rgnInside.SetRectRgn(rectangle);
   //            rgnLast.CombineRgn(&rgnOutside, &rgnInside, RGN_XOR);

   //            // only diff them if brushes are the same
   //            if (pBrush->get_os_data() == pBrushLast->get_os_data())
   //            {
   //               rgnUpdate.create_rect(0, 0, 0, 0);
   //               rgnUpdate.CombineRgn(&rgnLast, &rgnNew, RGN_XOR);
   //            }
   //         }
   //         if (pBrush->get_os_data() != pBrushLast->get_os_data() && lpRectLast != nullptr)
   //         {
   //            // brushes are different -- erase old region first
   //            SelectClipRgn(&rgnLast);
   //            get_clip_box(&rectangle);
   //            pBrushOld = SelectObject(pBrushLast);
   //            PatBlt(rectangle.left, rectangle.top, rectangle.width(), rectangle.height(), PATINVERT);
   //            SelectObject(pBrushOld);
   //            pBrushOld = nullptr;
   //         }

   //         // draw into the update/___new region
   //         SelectClipRgn(rgnUpdate.get_os_data() != nullptr ? &rgnUpdate : &rgnNew);
   //         get_clip_box(&rectangle);
   //         pBrushOld = SelectObject(pBrush);
   //         PatBlt(rectangle.left, rectangle.top, rectangle.width(), rectangle.height(), PATINVERT);

   //         // cleanup DC
   //         if (pBrushOld != nullptr)
   //            SelectObject(pBrushOld);
   //         SelectClipRgn(nullptr);

   //   */
   //}


   void graphics::text_out(double x, double y, const ::scoped_string& scopedstr)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      ::gpu::context_lock contextlock(gpu_context());

      nanovg_keep keep(m_pdc);

      auto rectangle = ::f64_rectangle(f64_point(x, y), f64_size(65535.0, 65535.0));

      internal_draw_text(scopedstr, rectangle, e_align_top_left, e_draw_text_none);

      //return true;

   }


   /*void graphics::FillSolidRect(const ::f64_rectangle & rectangle, color32_t clr)
   {
      ::SetBkColor(m_hdc, clr);
      ::ExtTextOut(m_hdc, 0, 0, ETO_OPAQUE, rectangle, nullptr, 0, nullptr);
   }*/

   /*void graphics::fill_rectangle(const f64_rectangle & rectangle, color32_t clr)
   {
      ::f64_rectangle rect32;
      copy(rect32, rectangle);
      FillSolidRect(rect32, clr);
   }
*/

/*

void graphics::FillSolidRect(double x, double y, double cx, double cy, color32_t clr)
{
   ::SetBkColor(m_hdc, clr);
   ::f64_rectangle rectangle(x, y, x + cx, y + cy);
   ::ExtTextOut(m_hdc, 0, 0, ETO_OPAQUE, &rectangle, nullptr, 0, nullptr);
}

*/


//void graphics::draw_inset_3d_rectangle(const ::f64_rectangle & rectangleParam,
//                          const ::color::color & clrTopLeft, const ::color::color & clrBottomRight,
//   const ::e_border & eborder)
//{

//   //draw3d_rectangle(rectangleParam.left, rectangleParam.top, rectangleParam.right - rectangleParam.left,
//     //         rectangleParam.bottom - rectangleParam.top, clrTopLeft, clrBottomRight);

//}

//
//void graphics::draw_inset_rectangle(const ::f64_rectangle & rectangle, const ::color::color & color, const ::e_border & eborder)
//{


//}


//void graphics::Draw3dRect(double x, double y, double cx, double cy,
//                          color32_t clrTopLeft, color32_t clrBottomRight)
//{
//   fill_solid_rect_coord(x, y, cx - 1, 1, clrTopLeft);
//   fill_solid_rect_coord(x, y, 1, cy - 1, clrTopLeft);
//   fill_solid_rect_coord(x + cx - 1, y, 1, cy, clrBottomRight);
//   fill_solid_rect_coord(x, y + cy - 1, cx, 1, clrBottomRight);
//}





//::draw2d::graphics * ::draw2d_nanovg::graphics::from_handle(HDC hDC)
//{
//hdc_map* pMap = ::windows_definition::MapHDC(true); //create map if not exist
//ASSERT(pMap != nullptr);
//      ::draw2d::graphics * pgraphics = (::draw2d::graphics *)pMap->from_handle(hDC);
   //    ASSERT(pgraphics == nullptr || (dynamic_cast<::draw2d_nanovg::graphics * >(pgraphics))->m_hdc == hDC);
   //  return pgraphics;
   // return nullptr;
   //}

   //bool graphics::Attach(HDC hdc)
   //{

   //   //if(hdc == nullptr)
   //   //   return false;

   //   //if(m_hdc == hdc)
   //   //{

   //   //   if(m_pgraphics != nullptr)
   //   //      return true;

   //   //}

   //   //if(hdc != nullptr)
   //   //{

   //   //   m_pgraphics = allocateø< ::plusplus::Graphics( >(HDC) hdc);

   //   //   m_pgraphics->SetPageUnit(plusplus::UnitPixel);

   //   //   set_text_rendering_hint(::write_text::e_rendering_anti_alias_grid_fit);

   //   //   m_hdc = (HDC) hdc;

   //   //}

   //   //return m_pgraphics != nullptr;

   //   return true;

   //   /*ASSERT(m_hdc == nullptr);      // only attach once, detach on destroy
   //   ASSERT(m_hdc == nullptr);    // only attach to an is_empty DC

   //   if (hDC == nullptr)
   //   {
   //      return false;
   //   }
   //   // remember early to avoid leak
   //   set_handle1(hDC);
   //   hdc_map* pMap = ::windows_definition::MapHDC(true); // create map if not exist
   //   ASSERT(pMap != nullptr);
   //   pMap->set_permanent(m_hdc, this);

   //   SetAttribDC(m_hdc);     // Default to same as output
   //   return true;*/
   //}


   //HDC graphics::Detach()
   //{

   //   //if(m_hdc == nullptr)
   //   //   return nullptr;

   //   //if(m_pgraphics == nullptr)
   //   //   return nullptr;

   //   //try
   //   //{

   //   //   delete m_pgraphics;

   //   //}
   //   //catch(...)
   //   //{

   //   //   informationf("graphics::Detach : Failed to delete plusplus::Graphics");

   //   //}

   //   //m_pgraphics = nullptr;

   //   //HDC hdc = m_hdc;
   //   //m_hdc = nullptr;

   //   //return hdc;

   //   return nullptr;

   //}


   //bool graphics::AttachPrinter(HDC hdc)
   //{

   //   //if (hdc == nullptr)
   //   //   return false;

   //   //if (m_hdc == hdc)
   //   //{

   //   //   return true;

   //   //}

   //   //if (hdc != nullptr)
   //   //{

   //   //   m_hdc = (HDC)hdc;

   //   //}

   //   //return m_hdc != nullptr;

   //   return true;

   //}


   //HDC graphics::DetachPrinter()
   //{

   //   //if (m_hdc == nullptr)
   //   //   return nullptr;

   //   //HDC hdc = m_hdc;

   //   //m_hdc = nullptr;

   //   //return hdc;

   //   return nullptr;

   //}


   void graphics::DeleteDC()
   {

      if (m_pdc)
      {

         auto pdc = m_pdc;

         m_pdc = nullptr;

         try
         {

            auto pcontext = gpu_context();

            if (pcontext)
            {

               ::gpu::context_lock contextlock(pcontext);

               nvgDeleteGL3(pdc);

            }
            else
            {

               nvgDeleteGL3(pdc);

            }

         }
         catch (...)
         {

         }

      }



      //HDC hdc = Detach();

      //if(hdc != nullptr)
      //{

      //   bool bDeleted = ::DeleteDC(hdc) != false;

      //   if(!bDeleted)
      //   {

      //      informationf("graphics::DeleteDC : Failed to delete GDI device context");

      //   }

      //}

      //if(m_pgraphics != nullptr)
      //{

      //   try
      //   {

      //      delete m_pgraphics;

      //   }
      //   catch(...)
      //   {

      //      informationf("graphics::DeleteDC : Failed to delete plusplus::Graphics");

      //   }

      //   m_pgraphics = nullptr;

      //}

      //if(m_ppath != nullptr)
      //{

      //   try
      //   {

      //      delete m_ppath;

      //   }
      //   catch(...)
      //   {

      //   }

      //   m_ppath = nullptr;

      //}

      //if(m_ppathPaint != nullptr)
      //{

      //   try
      //   {

      //      delete m_ppathPaint;

      //   }
      //   catch(...)
      //   {

      //   }

      //   m_ppathPaint = nullptr;

      //}

      //if (m_hdc != nullptr)
      //{

      //   //::ReleaseDC(m_hwnd, m_hdc);

      //}

      //return true;

   }


   //   void graphics::SetAttribDC(HDC hDC)  // set the Attribute DC
   //   {
   ////      set_handle2(hDC);
   //   }
   //
   //
   //   void graphics::SetOutputDC(HDC hDC)  // set the Output DC
   //   {
   //#ifdef _DEBUG
   //      /*      hdc_map* pMap = ::windows_definition::MapHDC();
   //            if (pMap != nullptr && pMap->lookup_permanent(m_hdc) == this)
   //            {
   //               information(trace_category_appmsg, 0, "Cannot set Output hDC on Attached graphics.\n");
   //               ASSERT(false);
   //            }*/
   //#endif
   //      //    set_handle1(hDC);
   //   }
   //
   //
   //   void graphics::ReleaseAttribDC()     // Release the Attribute DC
   //   {
   ////      set_handle2(nullptr);
   //   }
   //
   //
   //   void graphics::ReleaseOutputDC()     // Release the Output DC
   //   {
   //#ifdef _DEBUG
   //      /*      hdc_map* pMap = ::windows_definition::MapHDC();
   //            if (pMap != nullptr && pMap->lookup_permanent(m_hdc) == this)
   //            {
   //               information(trace_category_appmsg, 0, "Cannot Release Output hDC on Attached graphics.\n");
   //               ASSERT(false);
   //            }*/
   //#endif
   //      //set_handle1(nullptr);
   //   }

   //    /////////////////////////////////////////////////////////////////////////////
   //    // Out-of-line routines
   //
   // int graphics::StartDoc(const ::scoped_string& lpszDocName)
   // {
   //    //DOCINFO di;
   //    //memory_set(&di, 0, sizeof(DOCINFO));
   //    //di.cbSize = sizeof(DOCINFO);
   //    //di.lpszDocName = lpszDocName;
   //    //return StartDoc(&di);
   //    return -1;
   // }


   int graphics::save_graphics_context()
   {

      nvgSave(m_pdc);
      //      return m_pgraphics->Save();
      return 0;

   }


   void graphics::restore_graphics_context(int iSavedContext)
   {
      nvgRestore(m_pdc);
      //return m_pgraphics->Restore(nSavedDC) != false;
      //return true;

   }

   //   ::draw2d::object* graphics::SelectGdiObject(::particle * pparticle, HDC hDC, HGDIOBJ h)
      // {
   //      return ::draw2d_nanovg::object::from_handle(papp, ::SelectObject(hDC, h));
      //}
   ::draw2d_nanovg::draw2d* graphics::draw2d_nanovg()
   {

      return dynamic_cast <::draw2d_nanovg::draw2d*>(system()->draw2d());

   }

   ::draw2d::object* graphics::SelectStockObject(int nIndex)
   {
      /*      HGDIOBJ hObject = ::GetStockObject(nIndex);
            HGDIOBJ hOldObj = nullptr;
            ASSERT(hObject != nullptr);
            if(m_hdc != nullptr && m_hdc != m_hdc)
               hOldObj = ::SelectObject(m_hdc, hObject);
            if(m_hdc != nullptr)
               hOldObj = ::SelectObject(m_hdc, hObject);
            return ::draw2d_nanovg::object::from_handle(get_app(), hOldObj);*/

      return nullptr;

   }


   ::draw2d::pen* graphics::SelectObject(::draw2d::pen* pPen)
   {
      /*HGDIOBJ hOldObj = nullptr;
      if(pPen == nullptr)
         return nullptr;
      if(m_hdc != nullptr && m_hdc != m_hdc)
         hOldObj = ::SelectObject(m_hdc, pPen->get_os_data());
      if(m_hdc != nullptr)
         hOldObj = ::SelectObject(m_hdc, pPen->get_os_data());
      return dynamic_cast < pen * > (::draw2d_nanovg::object::from_handle(get_app(), hOldObj));*/
      m_ppen = pPen;
      return m_ppen;

   }


   ::draw2d::brush* graphics::SelectObject(::draw2d::brush* pBrush)
   {
      /*      HGDIOBJ hOldObj = nullptr;
            if(pBrush == nullptr)
               return nullptr;
            if(m_hdc != nullptr && m_hdc != m_hdc)
               hOldObj = ::SelectObject(m_hdc, pBrush->get_os_data());
            if(m_hdc != nullptr)
               hOldObj = ::SelectObject(m_hdc, pBrush->get_os_data());
            return dynamic_cast < ::draw2d::brush * > (::draw2d_nanovg::object::from_handle(get_app(), hOldObj));*/
      m_pbrush = pBrush;

      return m_pbrush;

   }


   ::write_text::font* graphics::SelectObject(::write_text::font* pfont)
   {
      /*      HGDIOBJ hOldObj = nullptr;
            if(pFont == nullptr)
               return nullptr;
            if(m_hdc != nullptr && m_hdc != m_hdc)
               hOldObj = ::SelectObject(m_hdc, pFont->get_os_data());
            if(m_hdc != nullptr)
               hOldObj = ::SelectObject(m_hdc, pFont->get_os_data());
            return dynamic_cast < ::write_text::font * > (::draw2d_nanovg::object::from_handle(get_app(), hOldObj));*/

            /*ASSERT(pFont != nullptr);

            if(pFont == nullptr)
               return nullptr;

            m_fontxyz = *pFont;
            return &m_fontxyz;*/

            //if(!set(pfont))
              // return nullptr;

      set(pfont);

      return m_pfont;

   }


   int graphics::SelectObject(::draw2d::region* pRgn)
   {
      int nRetVal = 1;
      //int nRetVal = GDI_ERROR;
      ////if(m_hdc != nullptr && m_hdc != m_hdc)
      //   nRetVal = (double)(iptr)::SelectObject(m_hdc, (HGDIOBJ) pRgn->get_os_data());
      //if(m_hdc != nullptr)
      //   nRetVal = (double)(iptr)::SelectObject(m_hdc, (HGDIOBJ) pRgn->get_os_data());
      return nRetVal;

   }


   ::draw2d::palette* graphics::SelectPalette(::draw2d::palette* pPalette, bool bForceBackground)
   {
      return nullptr;
      //      return dynamic_cast < ::draw2d::palette * > (::draw2d_nanovg::object::from_handle(get_app(), ::SelectPalette(m_hdc, (HPALETTE)pPalette->get_os_data(), bForceBackground)));
   }


   int graphics::SetPolyFillMode(int nPolyFillMode)
   {
      int nRetVal = 0;
      //if(m_hdc != nullptr && m_hdc != m_hdc)
      //   nRetVal = ::SetPolyFillMode(m_hdc, nPolyFillMode);
      //if(m_hdc != nullptr)
      //   nRetVal = ::SetPolyFillMode(m_hdc, nPolyFillMode);
      return nRetVal;

   }


   int graphics::SetROP2(int nDrawMode)
   {
      int nRetVal = 0;
      //if(m_hdc != nullptr && m_hdc != m_hdc)
      //   nRetVal = ::SetROP2(m_hdc, nDrawMode);
      //if(m_hdc != nullptr)
      //   nRetVal = ::SetROP2(m_hdc, nDrawMode);
      return nRetVal;

   }


   int graphics::set_interpolation_mode(int nStretchMode)
   {

      //if(m_pgraphics == nullptr)
      //{

      //   return -1;

      //}

      //if(nStretchMode == 0)
      //{
      //   m_pgraphics->SetInterpolationMode(plusplus::InterpolationModeNearestNeighbor);
      //}
      //else if(nStretchMode == e_interpolation_mode_high_quality_bicubic)
      //{
      //   m_pgraphics->SetInterpolationMode(plusplus::InterpolationModeHighQualityBicubic);
      //}
      //else
      //{
      //   m_pgraphics->SetInterpolationMode(plusplus::InterpolationModeLowQuality);
      //}
      //return 1;

      return true;

      /*double nRetVal = 0;
      if(m_hdc != nullptr && m_hdc != m_hdc)
         nRetVal = ::set_interpolation_mode(m_hdc, nStretchMode);
      if(m_hdc != nullptr)
         nRetVal = ::set_interpolation_mode(m_hdc, nStretchMode);
      return nRetVal;*/
   }


   int graphics::SetGraphicsMode(int iMode)
   {

      int nRetVal = 0;
      //if(m_hdc != nullptr && m_hdc != m_hdc)
      //{
      //   nRetVal = ::SetGraphicsMode(m_hdc, iMode);
      //}
      //if(m_hdc != nullptr)
      //{
      //   nRetVal = ::SetGraphicsMode(m_hdc, iMode);
      //}
      return nRetVal;

   }


   // bool graphics::SetWorldTransform(const XFORM* pXform)
   // {
   //
   //    //plusplus::REAL ::f64_rectangle[6];
   //
   //    //::f64_rectangle[0] = pXform->eDx;
   //    //::f64_rectangle[1] = pXform->eDy;
   //
   //    //::f64_rectangle[2] = pXform->eM11;
   //    //::f64_rectangle[3] = pXform->eM12;
   //    //::f64_rectangle[4] = pXform->eM21;
   //    //::f64_rectangle[5] = pXform->eM22;
   //
   //    //plusplus::Matrix m;
   //
   //    //m.SetElements(::f64_rectangle[2],::f64_rectangle[3],::f64_rectangle[4],::f64_rectangle[5],::f64_rectangle[0],::f64_rectangle[1]);
   //
   //    //m_pgraphics->SetTransform(&m);
   //
   //    return true;
   //
   // }
   //
   //
   // bool graphics::ModifyWorldTransform(const XFORM* pXform, unsigned int iMode)
   // {
   //    bool nRetVal = 0;
   //    //if(m_hdc != nullptr && m_hdc != m_hdc)
   //    //{
   //    //   nRetVal = ::ModifyWorldTransform(m_hdc, pXform, iMode) != false;
   //    //}
   //    //if(m_hdc != nullptr)
   //    //{
   //    //   nRetVal = ::ModifyWorldTransform(m_hdc, pXform, iMode) != false;
   //    //}
   //    return nRetVal;
   //
   // }


   int graphics::SetMapMode(int nMapMode)
   {
      int nRetVal = 0;
      //if(m_hdc != nullptr && m_hdc != m_hdc)
      //   nRetVal = ::SetMapMode(m_hdc, nMapMode);
      //if(m_hdc != nullptr)
      //   nRetVal = ::SetMapMode(m_hdc, nMapMode);
      return nRetVal;

   }


   //i32_point graphics::get_origin() const
   //{

   //   return ::draw2d::graphics::get_origin();

   //   //::i32_point point;
   //   //::GetContextOrgEx(m_hdc, &point);

   //   //if (m_pgraphics == nullptr)
   //   //{

   //   //   return ::i32_point();

   //   //}

   //   //plusplus::Point origin(0, 0);

   //   //m_pgraphics->TransformPoints(
   //   //   plusplus::CoordinateSpacePage,
   //   //   plusplus::CoordinateSpaceWorld,
   //   //   &origin,
   //   //   1);

   //   //return point((long long) origin.X, (long long) origin.Y);

   //   //return m_pointTranslate;

   //}


   ::gpu::texture* graphics::current_target_texture(::gpu::layer * pgpulayer)
   {

      auto pgpuimage = dynamic_cast < ::gpu::image * >(m_pimage);

      if (pgpuimage)
      {

         auto pgputexture = pgpuimage->gpu_texture();

         if (pgputexture)
         {

            return pgputexture;

         }

      }

      return ::gpu::graphics::current_target_texture(pgpulayer);

      //defer_constructø(m_ptextureCurrent);

      //::cast < ::gpu_opengl::texture > ptextureCurrent = m_ptextureCurrent;

      //auto vkimage = nanovg_surface_get_vk_image(m_nanovgsurface);

      //if (ptextureCurrent->m_vkimage != vkimage)
      //{

      //   if (ptextureCurrent->m_vkimage)
      //   {

      //      constructø(m_ptextureCurrent);

      //      ptextureCurrent = m_ptextureCurrent;

      //   }

      //   ptextureCurrent->_attach(vkimage, ::gpu::texture::e_type_image);

      //   ptextureCurrent->m_state.m_vkaccessflags = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
      //   ptextureCurrent->m_state.m_vkimagelayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
      //   ptextureCurrent->m_state.m_vkpipelinestageflags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

      //}

      //return m_ptextureCurrent;

   }


   bool graphics::is_gpu_oriented()
   {

      return true;

   }


   //   void graphics::_set(const ::geometry2d::matrix& matrix)
   //   {
   //
   //      thread_select();
   //
   //      //vkMatrixMode(VK_MODELVIEW);
   //      //vkLoadIdentity();
   //
   /////      VKdouble m[16];
   //
   //      //vkGetDoublev(VK_MODELVIEW_MATRIX, m);
   //
   //      //vkTranslatef(matrix.c1, matrix.c2, 0.f);
   //
   //      //vkGetDoublev(VK_MODELVIEW_MATRIX, m);
   //
   //
   //      double m[16];
   //
   //      m[0] = matrix.a1;
   //      m[1] = matrix.b1;
   //      //m[2] = matrix.c1;
   //      m[2] = 0.0;
   //      m[3] = 0.0;
   //
   //      m[4] = matrix.a2;
   //      m[5] = matrix.b2;
   //      //m[6] = matrix.c2;
   //      m[6] = 0.0;
   //      m[7] = 0.0;
   //
   //      m[8] = 0.0;
   //      m[9] = 0.0;
   //      m[10] = 1.0;
   //      m[11] = 0.0;
   //
   //      m[12] = matrix.c1;
   //      m[13] = matrix.c2;
   //      m[14] = 0.0;
   //      m[15] = 1.0;
   //
   //      //vkLoadMatrixd((const VKdouble *) m);
   //
   //      //return false;
   //
   //   }


      //i32_point graphics::set_origin(int x, int y)
      //{

      //   return ::draw2d::graphics::set_origin(x, y);

      //}


      //i32_point graphics::offset_origin(int nWidth, int nHeight)
      //{

      //   return ::draw2d::graphics::offset_origin(nWidth, nHeight);


      //}


      //i32_size graphics::set_context_extents(int x, int y)
      //{

      //   return ::draw2d::graphics::set_context_extents(x, y);

      //   //i32_size size(0, 0);
      //   ////if(m_hdc != nullptr && m_hdc != m_hdc)
      //   ////   ::Set_wiewportExtEx(m_hdc, x, y, &size);
      //   ////if(m_hdc != nullptr)
      //   ////   ::Set_wiewportExtEx(m_hdc, x, y, &size);
      //   //return size;

      //}


      //i32_size graphics::scale_context_extents(int xNum, int xDenom, int yNum, int yDenom)
      //{

      //   return ::draw2d::graphics::scale_context_extents(xNum, xDenom, yNum, yDenom);

      //   //i32_size size(0, 0);
      //   ////if(m_hdc != nullptr && m_hdc != m_hdc)
      //   ////   ::scale_context_extentsEx(m_hdc, xNum, xDenom, yNum, yDenom, &size);
      //   ////if(m_hdc != nullptr)
      //   ////   ::scale_context_extentsEx(m_hdc, xNum, xDenom, yNum, yDenom, &size);
      //   //return size;
      //}


   i32_point graphics::SetWindowOrg(int x, int y)
   {

      ::i32_point point;
      //if(m_hdc != nullptr && m_hdc != m_hdc)
      //   ::SetWindowOrgEx(m_hdc, x, y, &point);
      //if(m_hdc != nullptr)
      //   ::SetWindowOrgEx(m_hdc, x, y, &point);
      return point;

   }


   i32_point graphics::offset_window_org(int nWidth, int nHeight)
   {

      ::i32_point point;
      //if(m_hdc != nullptr && m_hdc != m_hdc)
      //   ::OffsetWindowOrgEx(m_hdc, nWidth, nHeight, &point);
      //if(m_hdc != nullptr)
      //   ::OffsetWindowOrgEx(m_hdc, nWidth, nHeight, &point);
      return point;

   }


   i32_size graphics::set_window_ext(int x, int y)
   {

      i32_size size(0, 0);
      //if(m_hdc != nullptr && m_hdc != m_hdc)
      //   ::SetWindowExtEx(m_hdc, x, y, &size);
      //if(m_hdc != nullptr)
      //   ::SetWindowExtEx(m_hdc, x, y, &size);
      return size;

   }


   i32_size graphics::scale_window_ext(int xNum, int xDenom, int yNum, int yDenom)
   {

      i32_size size(0, 0);
      //if(m_hdc != nullptr && m_hdc != m_hdc)
      //   ::ScaleWindowExtEx(m_hdc, xNum, xDenom, yNum, yDenom, &size);
      //if(m_hdc != nullptr)
      //   ::ScaleWindowExtEx(m_hdc, xNum, xDenom, yNum, yDenom, &size);
      return size;

   }


   int graphics::get_clip_box(::f64_rectangle* prectangle) const
   {

      //plusplus::f64_rectangle ::f64_rectangle;

      //m_pgraphics->GetClipBounds(rectangle);

      //rectangle.left = rectangle.X;
      //rectangle.top = rectangle.Y;
      //rectangle.right = rectangle.X + rectangle.Width;
      //rectangle.bottom = rectangle.Y + rectangle.Height;

      return 1;

   }


   int graphics::SelectClipRgn(::draw2d::region* pregion)
   {

      //if(pregion == nullptr)
      //{
      //   m_pgraphics->ResetClip();
      //}
      //else
      //{
      //   m_pgraphics->SetClip((plusplus::Region *) pregion->get_os_data());
      //}

      return 0;

      /*      double nRetVal = ERROR;
            if(m_hdc != nullptr && m_hdc != m_hdc)
               nRetVal = ::SelectClipRgn(m_hdc, pRgn == nullptr ? nullptr : (HRGN) pRgn->get_os_data());
            if(m_hdc != nullptr)
               nRetVal = ::SelectClipRgn(m_hdc, pRgn == nullptr ? nullptr : (HRGN) pRgn->get_os_data());
            return nRetVal;*/
   }


   int graphics::ExcludeClipRect(int x1, int y1, int x2, int y2)
   {

      //double nRetVal = ERROR;
      //if(m_hdc != nullptr && m_hdc != m_hdc)
      //   nRetVal = ::ExcludeClipRect(m_hdc, x1, y1, x2, y2);
      //if(m_hdc != nullptr)
      //   nRetVal = ::ExcludeClipRect(m_hdc, x1, y1, x2, y2);

      //return nRetVal;

      return 0;

   }


   int graphics::ExcludeClipRect(const ::i32_rectangle& rectangleParam)
   {

      //double nRetVal = ERROR;

      //if(m_hdc != nullptr && m_hdc != m_hdc)
      //   nRetVal = ::ExcludeClipRect(m_hdc, rectangleParam.left, rectangleParam.top,
      //                               rectangleParam.right, rectangleParam.bottom);
      //if(m_hdc != nullptr)
      //   nRetVal = ::ExcludeClipRect(m_hdc, rectangleParam.left, rectangleParam.top,
      //                               rectangleParam.right, rectangleParam.bottom);

      //return nRetVal;

      return 0;

   }


   int graphics::IntersectClipRect(int x1, int y1, int x2, int y2)
   {

      intersect_clip(::f64_rectangle(x1, y1, x2, y2));

      return 1;

   }


   int graphics::IntersectClipRect(const ::i32_rectangle& rectangleBounds)
   {

      intersect_clip(rectangleBounds);

      return 1;

   }


   int graphics::OffsetClipRgn(int x, int y)
   {

      int nRetVal = 0;

      //if(m_hdc != nullptr && m_hdc != m_hdc)
      //   nRetVal = ::OffsetClipRgn(m_hdc, x, y);
      //if(m_hdc != nullptr)
      //   nRetVal = ::OffsetClipRgn(m_hdc, x, y);

      return nRetVal;

   }


   int graphics::OffsetClipRgn(const ::i32_size& size)
   {

      int nRetVal = 0;

      //if(m_hdc != nullptr && m_hdc != m_hdc)
      //   nRetVal = ::OffsetClipRgn(m_hdc, size.cx, size.cy);
      //if(m_hdc != nullptr)
      //   nRetVal = ::OffsetClipRgn(m_hdc, size.cx, size.cy);

      return nRetVal;

   }


   unsigned int graphics::SetTextAlign(unsigned int nFlags)
   {
      //unsigned int nRetVal = GDI_ERROR;

      unsigned int nRetVal = 1;

      //if(m_hdc != nullptr && m_hdc != m_hdc)
      //   ::SetTextAlign(m_hdc, nFlags);
      //if(m_hdc != nullptr)
      //   nRetVal = ::SetTextAlign(m_hdc, nFlags);

      return nRetVal;

   }


   //int graphics::SetTextJustification(int nBreakExtra, int nBreakCount)
   //{
   // 
   //   double nRetVal = 0;

   //   //if(m_hdc != nullptr && m_hdc != m_hdc)
   //   //   nRetVal = ::SetTextJustification(m_hdc, nBreakExtra, nBreakCount);
   //   //if(m_hdc != nullptr)
   //   //   nRetVal = ::SetTextJustification(m_hdc, nBreakExtra, nBreakCount);

   //   return nRetVal;

   //}


   //double graphics::SetTextCharacterExtra(double nCharExtra)
   //{
   //   
   //   // ASSERT(m_hdc != nullptr);

   //   double nRetVal = 0x8000000;
   //   
   //   //if(m_hdc != nullptr && m_hdc != m_hdc)
   //   //   nRetVal = ::SetTextCharacterExtra(m_hdc, nCharExtra);
   //   //if(m_hdc != nullptr)
   //   //   nRetVal = ::SetTextCharacterExtra(m_hdc, nCharExtra);

   //   return nRetVal;

   //}


   //unsigned int graphics::SetMapperFlags(unsigned int dwFlag)
   //{
   //   
   //   // ASSERT(m_hdc != nullptr);

   //   unsigned int dwRetVal = GDI_ERROR;

   //   //if(m_hdc != nullptr && m_hdc != m_hdc)
   //   //   dwRetVal = ::SetMapperFlags(m_hdc, dwFlag);
   //   //if(m_hdc != nullptr)
   //   //   dwRetVal = ::SetMapperFlags(m_hdc, dwFlag);

   //   return dwRetVal;

   //}


   // typedef unsigned int (CALLBACK* __GDIGETLAYOUTPROC)(HDC);
   // typedef unsigned int (CALLBACK* __GDISETLAYOUTPROC)(HDC, unsigned int);

   unsigned int graphics::GetLayout() const
   {

      // HINSTANCE hInst = ::GetModuleHandleA("GDI32.DLL");
      //
      // ASSERT(hInst != nullptr);
      //
      // /*      unsigned int dwGetLayout = LAYOUT_LTR;
      //       __GDIGETLAYOUTPROC pfn;
      //       pfn = (__GDIGETLAYOUTPROC) GetProcAddress(hInst, "GetLayout");
      //       // if they API is available, just call it. If it is not
      //       // available, indicate an error.
      //       if (pfn != nullptr)
      //          dwGetLayout = (*pfn)(m_hdc);
      //       else
      //       {
      //          dwGetLayout = GDI_ERROR;
      //          set_last_error(ERROR_CALL_NOT_IMPLEMENTED);
      //       }*/
      //
      //       //return dwGetLayout;

      return 0;

   }


   unsigned int graphics::SetLayout(unsigned int dwSetLayout)
   {
      /*HINSTANCE hInst = ::GetModuleHandleA("GDI32.DLL");
      ASSERT(hInst != nullptr);
      unsigned int dwGetLayout = LAYOUT_LTR;
      __GDISETLAYOUTPROC pfn;
      pfn = (__GDISETLAYOUTPROC) GetProcAddress(hInst, "SetLayout");
      // If the API is availalbe, just call it. If it's not available,
      // setting anything other than LAYOUT_LTR is an error.
      if (pfn != nullptr)
         dwGetLayout = (*pfn)(m_hdc, dwSetLayout);
      else if (dwSetLayout != LAYOUT_LTR)
      {
         dwGetLayout = GDI_ERROR;
         set_last_error(ERROR_CALL_NOT_IMPLEMENTED);
      }
      return dwGetLayout;*/

      return 0;

   }
   /*
   void window::screen_to_client(::f64_rectangle * prectangle)
   {
   ASSERT(::IsWindow(m_hdc));
   ::screen_to_client(m_hdc, (::f64_point *)::f64_rectangle);
   ::screen_to_client(m_hdc, ((::f64_point *)::f64_rectangle)+1);
   if (GetExStyle() & WS_EX_LAYOUTRTL)
   ::f64_rectangle::swap_left_right(rectangle);
   }

   void window::client_to_screen(::f64_rectangle * prectangle)
   {
   ASSERT(::IsWindow(m_hdc));
   ::client_to_screen(m_hdc, (::f64_point *)::f64_rectangle);
   ::client_to_screen(m_hdc, ((::f64_point *)::f64_rectangle)+1);
   if (GetExStyle() & WS_EX_LAYOUTRTL)
   ::f64_rectangle::swap_left_right(rectangle);
   }*/


   /////////////////////////////////////////////////////////////////////////////
   // Advanced Win32 GDI functions

   void graphics::arc_to(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
   {
      // ASSERT(m_hdc != nullptr);
      bool bResult = false;
      //bool bResult = ::ArcTo(m_hdc, x1, y1, x2, y2, x3, y3, x4, y4) != false;
      //if (m_hdc != m_hdc)
      //{
      //   ::i32_point point;
      //   VERIFY(::GetCurrentPositionEx(m_hdc, &point));
      //   VERIFY(::MoveToEx(m_hdc, point.x, point.y, nullptr));
      //}
      //return bResult;
   }


   //double graphics::SetArcDirection(double nArcDirection)
   //{
   //   // ASSERT(m_hdc != nullptr);
   //   double nResult = 0;
   //   //if (m_hdc != m_hdc)
   //   //   nResult = ::SetArcDirection(m_hdc, nArcDirection);
   //   //if (m_hdc != nullptr)
   //   //   nResult = ::SetArcDirection(m_hdc, nArcDirection);
   //   return nResult;
   //}

   //bool graphics::PolyDraw(const ::f64_point* lpPoints, const unsigned char* lpTypes, ::collection::count nCount)
   //{

   //   // ASSERT(m_hdc != nullptr);

   //   bool bResult = false;
   //   //bool bResult = ::PolyDraw(m_hdc, lpPoints, lpTypes, (int) nCount) != false;

   //   //if (m_hdc != m_hdc)
   //   //{
   //   //   ::i32_point point;
   //   //   VERIFY(::GetCurrentPositionEx(m_hdc, &point));
   //   //   VERIFY(::MoveToEx(m_hdc, point.x, point.y, nullptr));
   //   //}
   //   return bResult;
   //}

   void graphics::polyline_to(const ::f64_point* lpPoints, ::collection::count nCount)
   {
      // ASSERT(m_hdc != nullptr);
      //bool bResult = false;
      //bool bResult = ::polyline_to(m_hdc, lpPoints, (unsigned int) nCount) != false;
      //if (m_hdc != m_hdc)
      //{
      //   ::i32_point point;
      //   VERIFY(::GetCurrentPositionEx(m_hdc, &point));
      //   VERIFY(::MoveToEx(m_hdc, point.x, point.y, nullptr));
      //}
      //return bResult;
   }


   // bool graphics::SetColorAdjustment(const COLORADJUSTMENT* lpColorAdjust)
   // {
   //    // ASSERT(m_hdc != nullptr);
   //    bool bResult = false;
   //    //if (m_hdc != m_hdc)
   //    //   bResult = ::SetColorAdjustment(m_hdc, lpColorAdjust) != false;
   //    //if (m_hdc != nullptr)
   //    //   bResult = ::SetColorAdjustment(m_hdc, lpColorAdjust) != false;
   //    return bResult;
   // }


   void graphics::poly_bezier_to(const ::f64_point* lpPoints, ::collection::count nCount)
   {
      // ASSERT(m_hdc != nullptr);
      //bool bResult = false;      //bool bResult = ::poly_bezier_to(m_hdc, lpPoints, (unsigned int) nCount) != false;
      //if (m_hdc != m_hdc)
      //{
      //   ::i32_point point;
      //   VERIFY(::GetCurrentPositionEx(m_hdc, &point));
      //   VERIFY(::MoveToEx(m_hdc, point.x, point.y, nullptr));
      //}
      //return bResult;
   }


   void graphics::SelectClipPath(int nMode)
   {
      // ASSERT(m_hdc != nullptr);

      //bool bResult = false;

      //// output DC always holds the current path
      //if (!::SelectClipPath(m_hdc, nMode))
      //   return false;

      //// transfer clipping region into the attribute DC
      //bool bResult = true;
      //if (m_hdc != m_hdc)
      //{
      //   HRGN hRgn = ::CreateRectRgn(0, 0, 0, 0);
      //   if (::GetClipRgn(m_hdc, hRgn) < 0 || !::SelectClipRgn(m_hdc, hRgn))
      //   {
      //      INFO(trace_category_appmsg, 0, "Error: unable to transfer clip region in graphics::SelectClipPath!\n");
      //      bResult = false;
      //   }
      //   ::DeleteObject(hRgn);
      //}
      //return bResult;
   }


   int graphics::SelectClipRgn(::draw2d::region* pregion, int nMode)
   {

      //if(pregion == nullptr)
      //{

      //   m_pgraphics->ResetClip();

      //}
      //else
      //{

      //   if(nMode == RGN_AND)
      //   {

      //      m_pgraphics->SetClip((plusplus::Region *) pregion->get_os_data(), plusplus::CombineModeIntersect);

      //   }
      //   else if(nMode == RGN_OR)
      //   {

      //      m_pgraphics->SetClip((plusplus::Region *) pregion->get_os_data(),plusplus::CombineModeUnion);

      //   }
      //   else if(nMode == RGN_XOR)
      //   {

      //      m_pgraphics->SetClip((plusplus::Region *) pregion->get_os_data(),plusplus::CombineModeXor);

      //   }
      //   else if(nMode == RGN_COPY)
      //   {

      //      m_pgraphics->SetClip((plusplus::Region *) pregion->get_os_data(),plusplus::CombineModeReplace);

      //   }
      //   else if(nMode == RGN_DIFF)
      //   {

      //      m_pgraphics->SetClip((plusplus::Region *) pregion->get_os_data(),plusplus::CombineModeExclude);

      //   }

      //}

      return 0;

   }

   //
   // /////////////////////////////////////////////////////////////////////////////
   // // Special handling for metafile playback
   //
   // double CALLBACK __enum_meta_file_procedure(HDC hDC,
   //    HANDLETABLE* pHandleTable, METARECORD* pMetaRec, double nHandles, LPARAM lParam)
   // {
   //    return 1;
   //    //      ::draw2d::graphics * pgraphics = (::draw2d::graphics *)lParam;
   //    //      ASSERT_VALID(pgraphics);
   //    //
   //    //      switch (pMetaRec->rdFunction)
   //    //      {
   //    //      // these records have effects different for each graphics derived class
   //    //      case META_SETMAPMODE:
   //    //         (dynamic_cast<::draw2d_nanovg::graphics * >(pgraphics))->SetMapMode((double)(short)pMetaRec->rdParm[0]);
   //    //         break;
   //    //      case META_SETWINDOWEXT:
   //    //         (dynamic_cast<::draw2d_nanovg::graphics * >(pgraphics))->set_window_ext(
   //    //         (double)(short)pMetaRec->rdParm[1], (double)(short)pMetaRec->rdParm[0]);
   //    //         break;
   //    //      case META_SETWINDOWORG:
   //    //         (dynamic_cast<::draw2d_nanovg::graphics * >(pgraphics))->SetWindowOrg(
   //    //         (double)(short)pMetaRec->rdParm[1], (double)(short)pMetaRec->rdParm[0]);
   //    //         break;
   //    //      case META_SETVIEWPORTEXT:
   //    //         (dynamic_cast<::draw2d_nanovg::graphics * >(pgraphics))->set_context_extents(
   //    //         (double)(short)pMetaRec->rdParm[1], (double)(short)pMetaRec->rdParm[0]);
   //    //         break;
   //    //      case META_SETVIEWPORTORG:
   //    //         (dynamic_cast<::draw2d_nanovg::graphics * >(pgraphics))->set_origin(
   //    //         (double)(short)pMetaRec->rdParm[1], (double)(short)pMetaRec->rdParm[0]);
   //    //         break;
   //    //      case META_SCALEWINDOWEXT:
   //    //         (dynamic_cast<::draw2d_nanovg::graphics * >(pgraphics))->scale_window_ext(
   //    //         (double)(short)pMetaRec->rdParm[3], (double)(short)pMetaRec->rdParm[2],
   //    //         (double)(short)pMetaRec->rdParm[1], (double)(short)pMetaRec->rdParm[0]);
   //    //         break;
   //    //      case META_SCALEVIEWPORTEXT:
   //    //         (dynamic_cast<::draw2d_nanovg::graphics * >(pgraphics))->scale_context_extents(
   //    //         (double)(short)pMetaRec->rdParm[3], (double)(short)pMetaRec->rdParm[2],
   //    //         (double)(short)pMetaRec->rdParm[1], (double)(short)pMetaRec->rdParm[0]);
   //    //         break;
   //    //      case META_OFFSETVIEWPORTORG:
   //    //         (dynamic_cast<::draw2d_nanovg::graphics * >(pgraphics))->offset_origin(
   //    //         (double)(short)pMetaRec->rdParm[1], (double)(short)pMetaRec->rdParm[0]);
   //    //         break;
   //    //      case META_SAVEDC:
   //    //         (dynamic_cast<::draw2d_nanovg::graphics * >(pgraphics))->SaveDC();
   //    //         break;
   //    //      case META_RESTOREDC:
   //    //         (dynamic_cast<::draw2d_nanovg::graphics * >(pgraphics))->RestoreDC((double)(short)pMetaRec->rdParm[0]);
   //    //         break;
   //    //      case META_SETBKCOLOR:
   //    //      {
   //    //         auto pbrush = createø < ::draw2d::brush >();
   //    //
   //    //         pbrush->create_solid(*(UNALIGNED color32_t*)& pMetaRec->rdParm[0]);
   //    //         (dynamic_cast<::draw2d_nanovg::graphics * >(pgraphics))->SelectObject(brush);
   //    //      }
   //    //      break;
   //    //      case META_SETTEXTCOLOR:
   //    //      {
   //    //         ::draw2d::brush_pointer brush((dynamic_cast<::draw2d_nanovg::graphics * >(pgraphics))->create_new, this, *(UNALIGNED color32_t*)&pMetaRec->rdParm[0]);
   //    //         (dynamic_cast<::draw2d_nanovg::graphics * >(pgraphics))->SelectObject(brush);
   //    //      }
   //    //      break;
   //    //
   //    //      // need to watch out for SelectObject(HFONT), for custom font mapping
   //    //      case META_SELECTOBJECT:
   //    //      {
   //    //         HGDIOBJ hObject = pHandleTable->objectHandle[pMetaRec->rdParm[0]];
   //    //         unsigned int nObjType = GetObjectType(hObject);
   //    //         if (nObjType == 0)
   //    //         {
   //    //            // object type is unknown, determine if it is a font
   //    //            HFONT hStockFont = (HFONT)::GetStockObject(SYSTEM_FONT);
   //    //            HFONT hFontOld = (HFONT)::SelectObject((dynamic_cast<::draw2d_nanovg::graphics * >(pgraphics))->m_hdc, hStockFont);
   //    //            HGDIOBJ hObjOld = ::SelectObject((dynamic_cast<::draw2d_nanovg::graphics * >(pgraphics))->m_hdc, hObject);
   //    //            if (hObjOld == hStockFont)
   //    //            {
   //    //               // got the stock object back, so must be selecting a font
   //    //               throw ::not_implemented();
   //    ////                  (dynamic_cast<::draw2d_nanovg::graphics * >(pgraphics))->SelectObject(::draw2d_nanovg::font::from_handle(pgraphics->get_app(), (HFONT)hObject));
   //    //               break;  // don't play the default record
   //    //            }
   //    //            else
   //    //            {
   //    //               // didn't get the stock object back, so restore everything
   //    //               ::SelectObject((dynamic_cast<::draw2d_nanovg::graphics * >(pgraphics))->m_hdc, hFontOld);
   //    //               ::SelectObject((dynamic_cast<::draw2d_nanovg::graphics * >(pgraphics))->m_hdc, hObjOld);
   //    //            }
   //    //            // and fall through to PlayMetaFileRecord...
   //    //         }
   //    //         else if (nObjType == OBJ_FONT)
   //    //         {
   //    //            // play back as graphics::SelectObject(::write_text::font*)
   //    ////               (dynamic_cast<::draw2d_nanovg::graphics * >(pgraphics))->SelectObject(::draw2d_nanovg::font::from_handle(pgraphics->get_app(), (HFONT)hObject));
   //    //            throw ::not_implemented();
   //    //            break;  // don't play the default record
   //    //         }
   //    //      }
   //    //      // fall through...
   //    //
   //    //      default:
   //    //         ::PlayMetaFileRecord(hDC, pHandleTable, pMetaRec, nHandles);
   //    //         break;
   //    //      }
   //    //
   //    //      return 1;
   // }


   //bool graphics::PlayMetaFile(HMETAFILE hMF)
   //{
   //   //if (::GetDeviceCaps(m_hdc, TECHNOLOGY) == DT_METAFILE)
   //   //{
   //   //   // playing metafile in metafile, just use aura windows API
   //   //   return ::PlayMetaFile(m_hdc, hMF) != false;
   //   //}

   //   //// for special playback, lParam == pgraphics
   //   //return ::EnumMetaFile(m_hdc, hMF, __enum_meta_file_procedure, (LPARAM)this) != false;
   //   return false;

   //}


   /////////////////////////////////////////////////////////////////////////////
   // Coordinate transforms


   void graphics::LPtoDP(::f64_size* LPSIZE32)
   {

      //ASSERT(is_memory_segment_ok(LPSIZE32, sizeof(::i32_size)));

      //i32_size sizeWinExt = GetWindowExt();
      //i32_size sizeVpExt = get_context_extents();
      //LPSIZE32->cx = MulDiv(LPSIZE32->cx, abs(sizeVpExt.cx), abs(sizeWinExt.cx));
      //LPSIZE32->cy = MulDiv(LPSIZE32->cy, abs(sizeVpExt.cy), abs(sizeWinExt.cy));

   }


   void graphics::DPtoLP(::f64_size* psize)
   {

      //ASSERT(is_memory_segment_ok(LPSIZE32, sizeof(::i32_size)));

      //i32_size sizeWinExt = GetWindowExt();
      //i32_size sizeVpExt = get_context_extents();
      //LPSIZE32->cx = MulDiv(LPSIZE32->cx, abs(sizeWinExt.cx), abs(sizeVpExt.cx));
      //LPSIZE32->cy = MulDiv(LPSIZE32->cy, abs(sizeWinExt.cy), abs(sizeVpExt.cy));

   }


   void graphics::draw_text(const ::scoped_string& scopedstr, const ::f64_rectangle& rectangle, const ::e_align& ealign, const ::e_draw_text& edrawtext)
   {

      ::gpu::context_lock contextlock(gpu_context());

      internal_draw_text(scopedstr, rectangle, ealign, edrawtext);

   }


   void graphics::draw_text(const ::scoped_string& scopedstr, const ::i32_rectangle& rectangle, const ::e_align& ealign, const ::e_draw_text& edrawtext)
   {

      ::gpu::context_lock contextlock(gpu_context());

      internal_draw_text(scopedstr, rectangle, ealign, edrawtext);
      //try
      //{

      //   if(m_pgraphics == nullptr)
      //      return false;

      //   /*

      //   switch(m_ewritetextrendering)
      //   {
      //   case ::write_text::e_rendering_anti_alias:
      //      m_pgraphics->SetCompositingMode(plusplus::CompositingModeSourceOver);
      //      m_pgraphics->SetTextRenderingHint(plusplus::TextRenderingHintAntiAlias);
      //      break;
      //   case ::write_text::e_rendering_anti_alias_grid_fit:
      //      m_pgraphics->SetCompositingMode(plusplus::CompositingModeSourceOver);
      //      m_pgraphics->SetTextRenderingHint(plusplus::TextRenderingHintAntiAliasGridFit);
      //      break;
      //   case ::write_text::e_rendering_single_bit_per_pixel:
      //      m_pgraphics->SetCompositingMode(plusplus::CompositingModeSourceOver);
      //      m_pgraphics->SetTextRenderingHint(plusplus::TextRenderingHintSingleBitPerPixel);
      //      break;
      //   case ::write_text::e_rendering_clear_type_grid_fit:
      //      m_pgraphics->SetCompositingMode(plusplus::CompositingModeSourceOver);
      //      m_pgraphics->SetTextRenderingHint(plusplus::TextRenderingHintClearTypeGridFit);
      //      break;
      //   }

      //   */
      //   m_pgraphics->SetCompositingMode(plusplus::CompositingModeSourceOver);
      //   m_pgraphics->SetTextRenderingHint(plusplus::TextRenderingHintAntiAlias);


      //}
      //catch(...)
      //{
      //}



      //plusplus::StringFormat format(plusplus::StringFormat::GenericTypographic());


      //format.SetFormatFlags(format.GetFormatFlags()
      //                      | plusplus::StringFormatFlagsNoClip | plusplus::StringFormatFlagsMeasureTrailingSpaces
      //                      | plusplus::StringFormatFlagsLineLimit);

      //if(nFormat & e_align_left)
      //{
      //   format.SetAlignment(plusplus::StringAlignmentNear);
      //}
      //else if(nFormat & e_align_right)
      //{
      //   format.SetAlignment(plusplus::StringAlignmentFar);
      //}
      //else if(nFormat & e_align_horizontal_center)
      //{
      //   format.SetAlignment(plusplus::StringAlignmentCenter);
      //}
      //else
      //{
      //   format.SetAlignment(plusplus::StringAlignmentNear);
      //}

      //if(nFormat & e_align_bottom)
      //{
      //   format.SetLineAlignment(plusplus::StringAlignmentFar);
      //}
      //else if(nFormat & e_align_top)
      //{
      //   format.SetLineAlignment(plusplus::StringAlignmentNear);
      //}
      //else if(nFormat & e_align_vertical_center)
      //{
      //   format.SetLineAlignment(plusplus::StringAlignmentCenter);
      //}
      //else
      //{
      //   format.SetLineAlignment(plusplus::StringAlignmentNear);
      //}

      //plusplus::REAL stops[] = {16,32,48, 64,80,96,112,128};

      //format.SetTabStops(0,8,stops);

      ////m_dFontSize             = pfontSrc.m_dFontSize;

      //plusplus::Matrix m;
      //m_pgraphics->GetTransform(&m);
      //plusplus::Matrix * pmNew = m.Clone();
      //try
      //{


      //   pmNew->Translate((plusplus::REAL) rectangleParam.left,(plusplus::REAL) rectangleParam.top);
      //   pmNew->Scale((plusplus::REAL) m_pfont->m_dFontWidth,(plusplus::REAL) 1.0,plusplus::MatrixOrderAppend);

      //   plusplus::rectF float_rectangle(0,0,(plusplus::REAL) ((rectangleParam.right - rectangleParam.left) * m_pfont->m_dFontWidth),(plusplus::REAL) (rectangleParam.bottom - rectangleParam.top));

      //   m_pgraphics->SetTransform(pmNew);

      //   m_pgraphics->DrawString(utf8_to_unicode(str),-1,vk2d_font(),float_rectangle,&format,vk2d_brush());
      //}
      //catch(...)
      //{
      //}

      //m_pgraphics->SetTransform(&m);

      //delete pmNew;

      //return 1;

   }


   //void graphics::draw_text_ex(const ::scoped_string & str, const ::i32_rectangle & rectangleParam, const ::e_align & ealign, const ::e_draw_text & edrawtext, LPDRAWTEXTPARAMS lpDTParams)
   //{

   //   // ASSERT(m_hdc != nullptr);

   //   // these flags would modify the string
   //   //ASSERT((nFormat & (DT_END_ELLIPSIS | DT_MODIFYSTRING)) != (DT_END_ELLIPSIS | DT_MODIFYSTRING));
   //   //ASSERT((nFormat & (DT_PATH_ELLIPSIS | DT_MODIFYSTRING)) != (DT_PATH_ELLIPSIS | DT_MODIFYSTRING));
   //   //wstring wstr = utf8_to_unicode(str);
   //   //return ::DrawTextExW(m_hdc,const_cast<wchar_t *>((const wchar_t *)wstr),(double)wcslen(wstr),(::f64_rectangle *) &rectangleParam,nFormat,lpDTParams);
   //   
   //   //return false;

   //}


   //void graphics::draw_text_ex(const ::scoped_string & str,const ::i32_rectangle & rectangleParam, const ::e_align & ealign, const ::e_draw_text & edrawtext,LPDRAWTEXTPARAMS lpDTParams)
   //{

   //   ::draw2d::graphics::draw_text_ex(str,rectangleParam,ealign, edrawtext,lpDTParams);

   //}

   ::f64_size graphics::get_text_extent(const ::scoped_string& scopedstr)
   {

      return get_text_extent(scopedstr, scopedstr.size());

   }


   //f64_size graphics::get_text_extent(const ::scoped_string & lpszString, character_count nCount, character_count iIndex)
   ::f64_size graphics::get_text_extent(const ::scoped_string& scopedstr, character_count iIndex)
   {

      string str(scopedstr.m_begin, minimum_non_negative(iIndex, scopedstr.size()));

      str = ::str::q_valid(str);

      if (str.is_empty())
      {

         return {};

      }

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      if (m_pfont.is_null())
      {

         //return false;

         throw ::exception(error_wrong_state);

      }

      if (m_pfont->m_dFontWidth <= 0.0)
      {

         throw ::exception(error_wrong_state);

      }

      if (iIndex < 0)
      {

         iIndex = (int)scopedstr.size();

      }

      if (not_found(str.find_first_character_in("\n\r")))
      {


#if defined(USE_PANGO)

         PangoFontDescription* pdesc = (PangoFontDescription*)m_pfont->get_os_data(this);

         if (::is_set(pdesc))
         {

            PangoLayout* playout;                            // layout for a paragraph of text

            playout = pango_nanovg_create_layout(m_pdc);                 // init pango layout ready for use

            pango_layout_set_text(playout, str.m_begin,
               str.size());          // sets the text to be associated with the layout (final arg is length, -1

            // to calculate automatically when passing a nul-terminated string)
            pango_layout_set_font_description(playout,
               pdesc);            // assign the previous font description to the layout

            pango_nanovg_update_layout(m_pdc,
               playout);                  // if the target surface or transformation properties of the nanovg instance

            // have changed, update the pango layout to reflect this
            int width = 0;

            int height = 0;

            PangoRectangle pos;

            pango_layout_index_to_pos(playout, str.size(), &pos);

            pango_layout_get_pixel_size(playout, &width, &height);

            //size.cx = ;

            //size.cy = height;

            g_object_unref(playout);                         // free the layout

            return { (double)pos.x / (double)PANGO_SCALE, (double)height };

         }
         else

#endif // USE_PANGO

         {

            nanovg_keep keep(m_pdc);

            _set(m_pfont);

            nvgTextAlign(m_pdc, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);

            auto x_advance = nvgTextBounds(m_pdc, 0.f, 0.f, str.m_begin, str.m_end, nullptr);

            //nanovg_font_extents_t fontextents;

            //nanovg_font_extents(m_pdc, &fontextents);

            float ascender = 0.f;

            float descender = 0.f;

            float lineh = 0.f;

            nvgTextMetrics(m_pdc, &ascender, &descender, &lineh);

            //size.cx = x;

            //size.cy = x;

            auto dWidth = (double)x_advance * m_pfont->m_dFontWidth;

            return { dWidth, lineh };

         }

         //return true;

         //xx

      }

      string_array straLines;

      straLines.add_lines(str, true);

      ::f64_size size;

      size.cx = 0.0;

      size.cy = 0.0;

      for (auto& strLine : straLines)
      {

         auto sizeLine = get_text_extent(strLine, strLine.length());

         size.cx = maximum(size.cx, sizeLine.cx);

      }

      _set(m_pfont);

      float lineh = 0.f;

      nvgTextMetrics(m_pdc, nullptr, nullptr, &lineh);

      size.cy = lineh * straLines.size();

      return size;

   }


   //f64_size graphics::get_text_extent(const ::string & lpszString, character_count nCount)
   //{

   //   ::f64_size size;

   //   if (!get_text_extent(size, lpszString, nCount, 0))
   //      return ::i32_size(0, 0);

   //   return ::i32_size(size.cx, size.cy);

   //}


  /* f64_size graphics::get_text_extent(const ::scoped_string & str)
   {

      ::f64_size size;

      if(!get_text_extent(size, str, (double) str.length(), 0))
         return ::i32_size(0, 0);

      return ::i32_size((long) size.cx, (long) size.cy);

   }*/


   //i32_size graphics::GetOutputTextExtent(const ::string & lpszString, character_count nCount)
   //{
   //   // ASSERT(m_hdc != nullptr);
   //   ::i32_size size;
   //   //string str(lpszString, nCount);
   //   //wstring wstr = utf8_to_unicode(str);
   //   //VERIFY(::GetTextExtentPoint32W(m_hdc, wstr, (double)wstr.get_length(), &size));
   //   return size;

   //}


   //i32_size graphics::GetOutputTextExtent(const ::string & str)
   //{

   //   // ASSERT(m_hdc != nullptr);

   //   ::i32_size size;

   //   wstring wstr = utf8_to_unicode(str);

   //   //::GetTextExtentPoint32W(m_hdc, wstr, (double)wstr.get_length(), &size);

   //   return size;

   //}

//
//   bool graphics::get_text_extent(f64_size & size, const ::string & lpszString, character_count nCount, character_count iIndex)
//   {
//
//      //// ASSERT(m_hdc != nullptr);
//
//      set(m_pfont);
//
//      ::pointer<font>pfont = m_pfont;
//
//      ::i32_size s;
//
//      wstring wstr = utf8_to_unicode(string(&lpszString[iIndex], nCount));
//
//      //if (!::GetTextExtentPoint32W(pfont->m_hdcFont, wstr, (double)wstr.get_length(), &s))
////         return false;
//
//      //// FreeType
//      //FT_Library ft;
//      //// All functions return a value different than 0 whenever an error occurred
//      //if (FT_Init_FreeType(&ft))
//      //   std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
//
//      //// Load font as face
//      //FT_Face face;
//      //if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
//      //   std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
//
//      //// Set i32_size to load vkyphs as
//      //FT_Set_Pixel_Sizes(face, 0, 48);
//
//      //// Disable unsigned char-alignment restriction
//      //vkPixelStorei(VK_UNPACK_ALIGNMENT, 1);
//
//      //// Load first 128 characters of ASCII set
//      //for (VKubyte c = 0; c < 128; c++)
//      //{
//      //   // Load character vkyph 
//      //   if (FT_Load_Char(face, c, FT_LOAD_RENDER))
//      //   {
//      //      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
//      //      continue;
//      //   }
//      //   // Generate texture
//      //   VKuint texture;
//      //   vkGenTextures(1, &texture);
//      //   vkBindTexture(VK_TEXTURE_2D, texture);
//      //   vkTexImage2D(
//      //      VK_TEXTURE_2D,
//      //      0,
//      //      VK_RED,
//      //      face->vkyph->bitmap.width,
//      //      face->vkyph->bitmap.rows,
//      //      0,
//      //      VK_RED,
//      //      VK_UNSIGNED_BYTE,
//      //      face->vkyph->bitmap.buffer
//      //   );
//      //   // Set texture options
//      //   vkTexParameteri(VK_TEXTURE_2D, VK_TEXTURE_WRAP_S, VK_CLAMP_TO_EDGE);
//      //   vkTexParameteri(VK_TEXTURE_2D, VK_TEXTURE_WRAP_T, VK_CLAMP_TO_EDGE);
//      //   vkTexParameteri(VK_TEXTURE_2D, VK_TEXTURE_MIN_FILTER, VK_LINEAR);
//      //   vkTexParameteri(VK_TEXTURE_2D, VK_TEXTURE_MAG_FILTER, VK_LINEAR);
//      //   // Now store character for later use
//      //   Character character = {
//      //       texture,
//      //       vkm::ivec2(face->vkyph->bitmap.width, face->vkyph->bitmap.rows),
//      //       vkm::ivec2(face->vkyph->bitmap_left, face->vkyph->bitmap_top),
//      //       face->vkyph->advance.x
//      //   };
//      //   Characters.insert(std::pair<VKchar, Character>(c, character));
//      //}
//      //vkBindTexture(VK_TEXTURE_2D, 0);
//      //// Destroy FreeType once we're finished
//      //FT_Done_Face(face);
//      //FT_Done_FreeType(ft);
//
//      size.cx = s.cx;
//
//      size.cy = s.cy;
//
//      return true;
//
//   }
//
//
//   bool graphics::get_text_extent(f64_size & size, const ::string & lpszString, character_count nCount)
//   {
//
//      //// ASSERT(m_hdc != nullptr);
//
//      set(m_pfont);
//
//      ::pointer<font>pfont = m_pfont;
//
//      ::i32_size s;
//
//      wstring wstr = utf8_to_unicode(lpszString, nCount);
//
//      //if (!::GetTextExtentPoint32W(pfont->m_hdcFont, wstr, (double)wstr.get_length(), &s))
//        // return false;
//
//      size.cx = s.cx;
//
//      size.cy = s.cy;
//
//      return true;
//
//   }
//
//
//   bool graphics::get_text_extent(f64_size & size, const ::string & str)
//   {
//
//      //// ASSERT(m_hdc != nullptr);
//
//      set(m_pfont);
//
//      ::pointer<font>pfont = m_pfont;
//
//      ::i32_size s;
//
//      wstring wstr = utf8_to_unicode(str);
//
//      //if (::GetTextExtentPoint32W(pfont->m_hdcFont, wstr, (double)wstr.get_length(), &s))
//        // return false;
//
//      size.cx = s.cx;
//
//      size.cy = s.cy;
//
//      return true;
//
//   }


   //void graphics::fill_rectangle(const f64_rectangle& rectangle, color32_t color32)
   //{

   //   try
   //   {

   //      if (m_pbitmap.is_set())
   //      {


   //         ::i32_size s = m_pbitmap.cast < bitmap>()->m_sizeOut;

   //         if (s.area() <= 0)
   //         {

   //            return;

   //         }

   //      }

   //      vkBegin(VK_QUADS);

   //      ::opengl::color(color32);

   //      ::opengl::vertex2f(rectangle);

   //      vkEnd();

   //   }
   //   catch(...)
   //   {
   //      
   //   }

   //}


   void graphics::draw_line(const i32_point& point1, const i32_point& point2, ::draw2d::pen* ppen)
   {

      ::opengl::line(point1.x, point1.y, point2.x, point2.y, (float)(ppen->m_dWidth),
         ppen->m_color.f32_red(), ppen->m_color.f32_green(),
         ppen->m_color.f32_blue(),
         ppen->m_color.f32_opacity(), 0.f, 0.f, true);

      /*vkLineWidth(ppen->m_dWidth);

      vkBegin(VK_LINES);

      ::opengl::color(ppen->m_color);

      vkVertex2f(point1.x, point1.y);
      vkVertex2f(point2.x, point2.y);

      vkEnd();*/

      m_point.x = point2.x;
      m_point.y = point2.y;

      //return true;

   }


   //void graphics::set_current_point(double x, double y)
   //{

   //   ::gpu::graphics::set_current_point(x, y);

   //}


   void graphics::line(double x1, double y1, double x2, double y2)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      nvgBeginPath(m_pdc);

      nvgMoveTo(m_pdc, (float)x1, (float)y1);

      nvgLineTo(m_pdc, (float) x2, (float) y2);

      draw();

      m_point.x = x2;

      m_point.y = y2;

   }


   void graphics::internal_draw_text(const block& block, const ::f64_rectangle& rectangle, const ::e_align& ealign,
      const ::e_draw_text& edrawtext)
   {


      //      _synchronous_lock ml(::draw2d_nanovg::mutex());

      auto pfont = m_pfont;

      if (::is_null(pfont))
      {

         throw ::exception(error_null_pointer);

      }

      if (pfont->m_fontsize.is_null_or_negative() || pfont->m_dFontWidth <= 0.0)
      {

         throw ::exception(error_wrong_state);

      }

#if defined(USE_PANGO)

      if (::is_set(pfont->get_os_data(this, 0)))
      {

         return internal_draw_text_pango(pfont, block, rectangle, ealign, edrawtext, &pango_nanovg_show_layout);


      }

#endif

      //if (::is_set(pfont->get_os_data(this, 1)))
//      if (::is_set(pfont->get_os_data(this, 0)))
  //    {

        // return internal_draw_text_nanovg(block, rectangle, ealign, edrawtext, &nanovg_show_text);

    //  }
      //else if (::is_set(pfont->get_os_data(this, 1)))
      //{

      //return internal_draw_text_nanovg(block, rectangle, ealign, edrawtext, &nanovg_show_text);
      return internal_draw_text_nanovg(block, rectangle, ealign, edrawtext);

      //}

      //throw ::exception(error_null_pointer);

   }


   void graphics::internal_draw_text_nanovg(const ::block& block, const ::f64_rectangle& rectangle,
      const ::e_align& ealign, const ::e_draw_text& edrawtext)
   {

      string str((const char*)block.data(), block.size());

      str = ::str::q_valid(str);

      if (str.is_empty())
      {

         return;

      }

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      if (m_pfont.is_null())
      {

         throw ::exception(error_null_pointer);

      }

      if (m_pfont->m_dFontWidth <= 0.0)
      {

         throw ::exception(error_wrong_state);

      }

      nanovg_keep keep(m_pdc);

      _set(m_pfont);

      float lineh = 0.f;

      nvgTextMetrics(m_pdc, nullptr, nullptr, &lineh);

      if (edrawtext & e_draw_text_expand_tabs)
      {

         str.replace_with("        ", "\t");

      }
      else
      {

         str.replace_with(" ", "\t");

      }

      if (edrawtext & e_draw_text_single_line)
      {

         str.replace_with("", "\n");

         str.replace_with("", "\r");

      }

      string_array stra;

      stra.add_lines(str);

      if (!stra.has_element())
      {

         return;

      }

      double x;

      int iHorizontalAlign;

      if (ealign & e_align_right)
      {

         iHorizontalAlign = NVG_ALIGN_RIGHT;

         x = rectangle.right;

      }
      else if (ealign & e_align_horizontal_center)
      {

         iHorizontalAlign = NVG_ALIGN_CENTER;

         x = rectangle.center_x();

      }
      else
      {

         iHorizontalAlign = NVG_ALIGN_LEFT;

         x = rectangle.left;

      }

      double y;

      int iVerticalAlign;

      if (ealign & e_align_baseline)
      {

         iVerticalAlign = NVG_ALIGN_BASELINE;

         y = rectangle.top;

      }
      else if (ealign & e_align_vertical_center)
      {

         iVerticalAlign = NVG_ALIGN_MIDDLE;

         y = rectangle.center_y() - lineh * (stra.size() - 1) / 2.0;

      }
      else if (ealign & e_align_bottom)
      {

         iVerticalAlign = NVG_ALIGN_BOTTOM;

         y = rectangle.bottom - lineh * (stra.size() - 1);

      }
      else
      {

         iVerticalAlign = NVG_ALIGN_TOP;

         y = rectangle.top;

      }

      if (!(ealign & e_align_baseline))
      {

         // Match draw2d's reference top-origin placement while preserving NanoVG's line metrics.
         y += nvgTextBaselineOffset(m_pdc);

      }

      nvgTextAlign(m_pdc, iHorizontalAlign | iVerticalAlign);

      if (m_pfont->m_dFontWidth != 1.0)
      {

         nvgTranslate(m_pdc, (float)x, 0.f);
         nvgScale(m_pdc, (float)m_pfont->m_dFontWidth, (float)1.0);
         nvgTranslate(m_pdc, (float)-x, 0.f);

      }

      //if (m_pbrush.is_set())
      //{

      //    set_os_color(m_pbrush->m_color);

      //}

      _fill1();

      for (auto& strLine : stra)
      {

         //nanovg_move_to(m_pdc, rectangle.left + Δx, rectangle.top + Δy + e.ascent + sz.cy * (i) / stra.get_size());

         //nvgMoveTo(m_pdc, , );

         if (strLine.contains("ø"))
         {

            nvgText(m_pdc, (float)x, (float)y, strLine.m_begin, strLine.m_end);

         }
         else
         {
            
            nvgText(m_pdc, (float)x, (float)y, strLine.m_begin, strLine.m_end);

         }

         //nanovg_status_t status = nanovg_status(m_pdc);

         //if (status != VKVG_STATUS_SUCCESS)
         //{

         //   const ::ansi_character* pszStatus = nanovg_status_to_string(status);

         //   informationf("nanovg error : graphics::draw_text %d %s", status, pszStatus);

         //}

         y += lineh;

      }

      _fill2();

      //return 1;

   }


   bool graphics::_set(::write_text::font* pfontParam)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      if (::is_null(pfontParam))
      {

         return false;

      }

      //::cast < ::draw2d_nanovg::font > pnanovgdraw2dfont = pfontParam;

      if (::is_null(pfontParam))
      {

         return false;

      }

      //auto posdata = pfontParam->get_os_data(this, 1);

      //if (::is_null(posdata))
      //{

      //   return false;

      //}

      ::string strFamilyName = pfontParam->m_pfontfamily->family_name(this);

      defer_load_font_by_family_name(strFamilyName);

      nvgFontFace(m_pdc, strFamilyName);

      //nanovg_font_face_t* pfontface = (nanovg_font_face_t*)posdata;

      //nanovg_set_font_face(m_pdc, pfontface);

      float fPreferredDpiX = 96.0f;

      float fPreferredDpiY = 96.0f;

      float fPreferredDensity = 1.0f;

      float fDenominatorDpi;

#ifdef __ANDROID__

      fDenominatorDpi = 160.0;

#elif defined(MACOS)

      fDenominatorDpi = 72.0;

#else

#define VKVG_USING_FREE_TYPE

      //#ifdef VKVG_USING_FREE_TYPE

               //fDenominatorDpi = 96.0;

      //#else

      fDenominatorDpi = 72.0;

      //#endif

#endif

      //double dFontScaler = 1.0;


      //if (::is_set(m_pdraw2dhost))
      //{

      //   dFontScaler = m_pdraw2dhost->font_scaler();

      //}

      if (::is_set(m_puserinteractionDraw2dGraphics))
      {

         fPreferredDpiX = m_puserinteractionDraw2dGraphics->preferred_dpi_x();

         fPreferredDpiY = m_puserinteractionDraw2dGraphics->preferred_dpi_y();

         fPreferredDensity = m_puserinteractionDraw2dGraphics->preferred_density();

      }

      float fDpi = maximum(fPreferredDpiX, fPreferredDpiY);

      float fDensity = fPreferredDensity;

      if (pfontParam->m_fontsize.eunit() == ::e_unit_pixel)
      {

         //nanovg_set_font_size(m_pdc, pfontParam->m_dFontSize * dFontScaler * fDensity);

         nvgFontSize(m_pdc, (float)(pfontParam->m_fontsize.as_f64() * fDensity));

      }
      else
      {

         //nanovg_set_font_size(m_pdc, pfontParam->m_dFontSize * dFontScaler * fPreferredDpiX / fDenominatorDpi);

         auto dFontSize = pfontParam->m_fontsize.as_f64();

         double dSize = dFontSize * fPreferredDpiX / fDenominatorDpi;

         nvgFontSize(m_pdc, (float)dSize);

      }

      m_pfontDevice = m_pfont;

      return true;

   }


   void graphics::set(::draw2d::region* pregion)
   {

      //return ::success;

   }


   void graphics::set(::draw2d::pen* ppen)
   {

      m_ppen = ppen;
      //vkLineWidth(ppen->m_dWidth);

      //::opengl::color(ppen->m_color);

      //return ::success;

   }



   void graphics::set(::draw2d::brush* pbrush)
   {

      ::draw2d::graphics::set(pbrush);
      //::opengl::color(pbrush->m_color);

      //return ::success;

   }



   void graphics::set(::write_text::font* pfont)
   {

      if (m_pfont != pfont)
      {

         ::string strFontName = pfont->m_pfontfamily->family_name(this);

         auto pszFontName = strFontName.c_str();

         ::draw2d::graphics::set(pfont);

      }

      //if (::is_null(pfont))
      //{

      //   //return ::error_failed;
      //   return;

      //}

      //pfont->get_os_data(this);

      ////return ::success;

   }


   void graphics::set(::draw2d::bitmap* pbitmap)
   {

      //return ::success;

   }


   ::draw2d::object* graphics::set_stock_object(int nIndex)
   {

      return nullptr;

   }


   void graphics::set_smooth_mode(::draw2d::enum_smooth_mode esmoothmode)
   {

      //if(m_esmoothmode == esmoothmode)
      //   return;

      //::draw2d::graphics::set_smooth_mode(esmoothmode);

      //try
      //{

      //   if(m_pgraphics == nullptr)
      //      return;

      //   if(m_esmoothmode == ::draw2d::e_smooth_mode_none)
      //   {

      //      m_pgraphics->SetSmoothingMode(plusplus::SmoothingModeNone);

      //   }
      //   else if(m_esmoothmode == ::draw2d::e_smooth_mode_antialias)
      //   {

      //      m_pgraphics->SetSmoothingMode(plusplus::SmoothingModeHighSpeed);

      //   }
      //   else if(m_esmoothmode == ::draw2d::e_smooth_mode_high)
      //   {

      //      m_pgraphics->SetSmoothingMode(plusplus::SmoothingModeHighQuality);

      //   }

      //}
      //catch(...)
      //{
      //return true;
      //}

   }


   void graphics::set_alpha_mode(::draw2d::enum_alpha_mode ealphamode)
   {

      try
      {
         if (m_ealphamode != ealphamode)
         {
            //if(m_pgraphics == nullptr)
            //   return;

            ::draw2d::graphics::set_alpha_mode(ealphamode);
            if (m_ealphamode == ::draw2d::e_alpha_mode_blend)
            {
               //vkColorMask(false, false, false, true);
               //vkColorMask(true, true, true, false);
               //vkBlendFunc(VK_SRC_ALPHA, VK_ONE_MINUS_SRC_ALPHA);
               //vkBlendFunc(VK_SRC_ALPHA, VK_SRC_ALPHA);
               //vkBlendFunc(VK_ONE, VK_ONE_MINUS_SRC_ALPHA);
               //vkBlendFunc(VK_ZERO, VK_SRC_ALPHA);
               //vkBlendEquationSeparate(VK_FUNC_ADD, VK_FUNC_ADD);
               // vkBlendFuncSeparate(VK_SRC_ALPHA, VK_ONE_MINUS_SRC_ALPHA, VK_ONE, VK_ZERO);
               //vkEnable(VK_BLEND);
               //vkDisable(VK_DEPTH_TEST);
               //vkDepthFunc(VK_NEVER);

               nvgGlobalCompositeOperation(m_pdc, NVG_SOURCE_OVER);
            }
            else if (m_ealphamode == ::draw2d::e_alpha_mode_set)
            {
               //vkEnable(VK_BLEND);
               //vkBlendFunc(VK_ONE, VK_ZERO);
               nvgGlobalCompositeOperation(m_pdc, NVG_COPY);
            }

         }

      }
      catch (...)
      {
         //return false;

      }

      //return true;

   }


   void graphics::_draw_nanovg_image(
      int iImage,
      const ::i32_size & sizeImage,
      const ::f64_rectangle & rectangleTarget,
      const ::image::image_drawing_options & imagedrawingoptions,
      const ::f64_point & pointSrc)
   {

      nanovg_keep keep(m_pdc);

      auto paint = nvgImagePattern(
         m_pdc,
         (float)(rectangleTarget.left - pointSrc.x),
         (float)(rectangleTarget.top - pointSrc.y),
         (float)sizeImage.cx,
         (float)sizeImage.cy,
         0.f,
         iImage,
         imagedrawingoptions.opacity().f32_opacity());

      nvgBeginPath(m_pdc);
      nvgRect(
         m_pdc,
         (float)rectangleTarget.left,
         (float)rectangleTarget.top,
         (float)rectangleTarget.width(),
         (float)rectangleTarget.height());
      nvgFillPaint(m_pdc, paint);
      nvgFill(m_pdc);

   }


   void graphics::reset_gpu_image_performance_diagnostics()
   {

      auto bEnabled = m_papplication
         && m_papplication->m_gpu.m_bPerformanceDiagnostics.load(
            ::std::memory_order_relaxed);
      auto iIntervalMilliseconds = m_papplication
         ? m_papplication->m_gpu.m_iPerformanceDiagnosticsIntervalMilliseconds.load(
            ::std::memory_order_relaxed)
         : 1'000;

      iIntervalMilliseconds = maximum(
         100,
         minimum(60'000, iIntervalMilliseconds));

      m_uPerformanceGpuImageDraws.store(0, ::std::memory_order_relaxed);
      m_uPerformanceCpuFallbackDraws.store(0, ::std::memory_order_relaxed);
      m_uPerformanceWrapperCreations.store(0, ::std::memory_order_relaxed);
      m_uPerformanceWrapperDeletions.store(0, ::std::memory_order_relaxed);
      m_uPerformancePendingFenceWaits.store(0, ::std::memory_order_relaxed);
      m_uPerformanceFenceWaitMicroseconds.store(0, ::std::memory_order_relaxed);
      m_uPerformanceWrapperMicroseconds.store(0, ::std::memory_order_relaxed);
      m_iPerformanceNextReportNanoseconds.store(
         performance_steady_nanoseconds()
            + (::i64)iIntervalMilliseconds * 1'000'000,
         ::std::memory_order_relaxed);
      m_bPerformanceDiagnosticsEnabledLast.store(
         bEnabled,
         ::std::memory_order_relaxed);
      m_uPerformanceDiagnosticsGenerationLast.store(
         m_papplication
            ? m_papplication->m_gpu.m_uPerformanceDiagnosticsGeneration.load(
               ::std::memory_order_relaxed)
            : 0,
         ::std::memory_order_relaxed);

   }


   void graphics::record_gpu_image_fast_path(
      bool bWaitedForFence,
      ::u64 uFenceMicroseconds,
      ::u64 uWrapperMicroseconds)
   {

      m_uPerformanceGpuImageDraws.fetch_add(1, ::std::memory_order_relaxed);
      m_uPerformanceWrapperCreations.fetch_add(1, ::std::memory_order_relaxed);
      m_uPerformanceWrapperDeletions.fetch_add(1, ::std::memory_order_relaxed);
      m_uPerformanceWrapperMicroseconds.fetch_add(
         uWrapperMicroseconds,
         ::std::memory_order_relaxed);

      if (bWaitedForFence)
      {

         m_uPerformancePendingFenceWaits.fetch_add(
            1,
            ::std::memory_order_relaxed);
         m_uPerformanceFenceWaitMicroseconds.fetch_add(
            uFenceMicroseconds,
            ::std::memory_order_relaxed);

      }

      report_gpu_image_performance_diagnostics_if_due();

   }


   void graphics::record_gpu_image_cpu_fallback()
   {

      if (!m_papplication
         || !m_papplication->m_gpu.m_bPerformanceDiagnostics.load(
            ::std::memory_order_relaxed))
      {

         return;

      }

      auto uGeneration =
         m_papplication->m_gpu.m_uPerformanceDiagnosticsGeneration.load(
            ::std::memory_order_relaxed);

      if (uGeneration != m_uPerformanceDiagnosticsGenerationLast.load(
         ::std::memory_order_relaxed))
      {

         reset_gpu_image_performance_diagnostics();

      }

      m_uPerformanceCpuFallbackDraws.fetch_add(
         1,
         ::std::memory_order_relaxed);
      report_gpu_image_performance_diagnostics_if_due();

   }


   void graphics::report_gpu_image_performance_diagnostics_if_due()
   {

      if (!m_papplication
         || !m_papplication->m_gpu.m_bPerformanceDiagnostics.load(
            ::std::memory_order_relaxed))
      {

         return;

      }

      auto iNowNanoseconds = performance_steady_nanoseconds();
      auto iDeadlineNanoseconds = m_iPerformanceNextReportNanoseconds.load(
         ::std::memory_order_relaxed);

      if (iNowNanoseconds < iDeadlineNanoseconds)
      {

         return;

      }

      auto iIntervalMilliseconds = maximum(
         100,
         minimum(
            60'000,
            m_papplication->m_gpu.m_iPerformanceDiagnosticsIntervalMilliseconds.load(
               ::std::memory_order_relaxed)));
      auto iNextNanoseconds = iNowNanoseconds
         + (::i64)iIntervalMilliseconds * 1'000'000;

      if (!m_iPerformanceNextReportNanoseconds.compare_exchange_strong(
         iDeadlineNanoseconds,
         iNextNanoseconds,
         ::std::memory_order_relaxed))
      {

         return;

      }

      auto uGpuDraws = m_uPerformanceGpuImageDraws.exchange(
         0,
         ::std::memory_order_relaxed);
      auto uCpuFallbacks = m_uPerformanceCpuFallbackDraws.exchange(
         0,
         ::std::memory_order_relaxed);
      auto uWrapperCreations = m_uPerformanceWrapperCreations.exchange(
         0,
         ::std::memory_order_relaxed);
      auto uWrapperDeletions = m_uPerformanceWrapperDeletions.exchange(
         0,
         ::std::memory_order_relaxed);
      auto uPendingFenceWaits = m_uPerformancePendingFenceWaits.exchange(
         0,
         ::std::memory_order_relaxed);
      auto uFenceWaitMicroseconds = m_uPerformanceFenceWaitMicroseconds.exchange(
         0,
         ::std::memory_order_relaxed);
      auto uWrapperMicroseconds = m_uPerformanceWrapperMicroseconds.exchange(
         0,
         ::std::memory_order_relaxed);

      information() << "[gpu.performance.nanovg_image] gpu_draws=" << uGpuDraws
         << " cpu_fallbacks=" << uCpuFallbacks
         << " wrapper_creates=" << uWrapperCreations
         << " wrapper_deletes=" << uWrapperDeletions
         << " pending_fence_waits=" << uPendingFenceWaits
         << " fence_wait_us=" << uFenceWaitMicroseconds
         << " wrapper_us=" << uWrapperMicroseconds;

   }


   bool graphics::_draw_gpu_image(
      const ::f64_rectangle & rectangleTarget,
      ::image::image * pimage,
      const ::image::image_drawing_options & imagedrawingoptions,
      const ::f64_point & pointSrc)
   {

      auto pgpuimage = dynamic_cast < ::gpu::image * >(pimage);

      if (!pgpuimage)
      {

         return false;

      }

      auto pgputexture = dynamic_cast < ::gpu_opengl::texture * >(
         pgpuimage->gpu_texture());

      if (!pgputexture || !pgputexture->m_gluTextureID)
      {

         throw ::exception(
            error_wrong_state,
            "NanoVG GPU image has no compatible OpenGL texture.");

      }

      auto pgpucontextTexture = pgputexture->context();
      auto pgpucontextCurrent = gpu_context();

      if (!pgpucontextTexture || !pgpucontextCurrent ||
          pgpucontextTexture->m_pgpudevice != pgpucontextCurrent->m_pgpudevice)
      {

         throw ::exception(
            error_wrong_state,
            "NanoVG GPU image belongs to a different GPU device.");

      }

      auto bPerformanceDiagnostics = m_papplication
         && m_papplication->m_gpu.m_bPerformanceDiagnostics.load(
            ::std::memory_order_relaxed);
      auto bPendingFence = false;
      auto uFenceMicroseconds = (::u64)0;

      if (bPerformanceDiagnostics)
      {

         auto uGeneration =
            m_papplication->m_gpu.m_uPerformanceDiagnosticsGeneration.load(
               ::std::memory_order_relaxed);

         if (uGeneration != m_uPerformanceDiagnosticsGenerationLast.load(
            ::std::memory_order_relaxed))
         {

            reset_gpu_image_performance_diagnostics();

         }

         bPendingFence = pgputexture->has_pending_fence();

         if (bPendingFence)
         {

            auto timeFenceStart = ::std::chrono::steady_clock::now();
            pgputexture->wait_fence();
            uFenceMicroseconds = (::u64)::std::chrono::duration_cast<
               ::std::chrono::microseconds>(
                  ::std::chrono::steady_clock::now() - timeFenceStart).count();

         }
         else
         {

            pgputexture->wait_fence();

         }

      }
      else
      {

         pgputexture->wait_fence();

      }

      _synchronous_lock synchronouslock(::draw2d_nanovg::mutex());

      auto sizeImage = pgpuimage->size();
      auto timeWrapperStart = ::std::chrono::steady_clock::time_point{};

      if (bPerformanceDiagnostics)
      {

         timeWrapperStart = ::std::chrono::steady_clock::now();

      }

      auto iImage = nvglCreateImageFromHandleGL3(
         m_pdc,
         pgputexture->m_gluTextureID,
         sizeImage.cx,
         sizeImage.cy,
         NVG_IMAGE_NODELETE |
            NVG_IMAGE_PREMULTIPLIED |
            NVG_IMAGE_FLIPY);
      auto uWrapperMicroseconds = (::u64)0;

      if (bPerformanceDiagnostics)
      {

         uWrapperMicroseconds = (::u64)::std::chrono::duration_cast<
            ::std::chrono::microseconds>(
               ::std::chrono::steady_clock::now() - timeWrapperStart).count();

      }

      if (iImage == 0)
      {

         throw ::exception(
            error_failed,
            "NanoVG failed to wrap the OpenGL GPU image texture.");

      }

      _draw_nanovg_image(
         iImage,
         sizeImage,
         rectangleTarget,
         imagedrawingoptions,
         pointSrc);

      if (bPerformanceDiagnostics)
      {

         timeWrapperStart = ::std::chrono::steady_clock::now();

      }

      nvgDeleteImage(m_pdc, iImage);

      if (bPerformanceDiagnostics)
      {

         uWrapperMicroseconds += (::u64)::std::chrono::duration_cast<
            ::std::chrono::microseconds>(
               ::std::chrono::steady_clock::now() - timeWrapperStart).count();
         record_gpu_image_fast_path(
            bPendingFence,
            uFenceMicroseconds,
            uWrapperMicroseconds);

      }

      return true;

   }


   void graphics::_draw_raw(
      const ::f64_rectangle & rectangleTarget,
      ::image::image * pimage,
      const ::image::image_drawing_options & imagedrawingoptions,
      const ::f64_point & pointSrc)
   {

      if (!m_pdc || !pimage || rectangleTarget.is_empty() || pimage->is_empty())
      {

         return;

      }

      pimage->defer_update_image();

      if (_draw_gpu_image(
         rectangleTarget,
         pimage,
         imagedrawingoptions,
         pointSrc))
      {

         return;

      }

      record_gpu_image_cpu_fallback();

      pimage->map();

      auto sizeImage = pimage->size();

      ::memory memoryRgba;

      memoryRgba.set_size(sizeImage.area() * 4);

      auto ptarget = memoryRgba.data();
      auto colorindexes = pimage->color_indexes();

      for (int y = 0; y < sizeImage.cy; y++)
      {

         auto psource = pimage->line_data(y);

         for (int x = 0; x < sizeImage.cx; x++)
         {

            *ptarget++ = psource->u8_red(colorindexes);
            *ptarget++ = psource->u8_green(colorindexes);
            *ptarget++ = psource->u8_blue(colorindexes);
            *ptarget++ = psource->u8_opacity(colorindexes);

            psource++;

         }

      }

      _synchronous_lock synchronouslock(::draw2d_nanovg::mutex());

      auto iImage = nvgCreateImageRGBA(
         m_pdc,
         sizeImage.cx,
         sizeImage.cy,
         NVG_IMAGE_PREMULTIPLIED,
         memoryRgba.data());

      if (iImage == 0)
      {

         return;

      }

      _draw_nanovg_image(
         iImage,
         sizeImage,
         rectangleTarget,
         imagedrawingoptions,
         pointSrc);

      nvgDeleteImage(m_pdc, iImage);

   }


   void graphics::set_text_rendering_hint(::write_text::enum_rendering etextrendering)
   {

      m_ewritetextrendering = etextrendering;

      //return true;

   }


   //   void * graphics::get_os_data() const
   //   {
   //
   ////      return (void *) m_pgraphics;
   //
   //      if (!m_hglrc)
   //      {
   //         return (void *) (iptr)1;
   //      }
   //
   //      return m_hglrc;
   //
   //   }


      //void graphics::fill_rectangle(const f64_rectangle& rectangle, color32_t color32)
      //{



      //}

   //   void * graphics::get_os_data_ex(int i) const
   //   {
   //
   //      if(i == 1)
   //      {
   ////         return m_pgraphics->GetHDC();
   //      }
   //      else
   //      {
   //         return ::draw2d::graphics::get_os_data(i);
   //      }
   //
   //      return nullptr;
   //
   //   }


   //   void graphics::release_os_data_ex(int i,void * p)
   //   {
   //      if(i == 1)
   //      {
   ////         m_pgraphics->ReleaseHDC((HDC)p);
   //      }
   //      else
   //      {
   //
   //         //::draw2d::graphics::release_os_data(i, p);
   //
   //      }
   //
   //   }


      /*HDC graphics::get_handle() const
      {

         return m_hdc;

      }


      HDC graphics::m_hdc const
      {

         return get_handle();

      }


      HDC graphics::m_hdc const
      {
         return get_handle();
      }
   */

   //bool graphics::attach(void * pdata)
   //{

   //   //if(m_pgraphics != nullptr)
   //   //{

   //   //   try
   //   //   {

   //   //      delete m_pgraphics;

   //   //   }
   //   //   catch(...)
   //   //   {

   //   //      informationf("graphics::attach : Failed to delete plusplus::Graphics");

   //   //   }

   //   //   m_pgraphics = nullptr;

   //   //}

   //   //m_pgraphics = (plusplus::Graphics *) pdata;

   //   return false;

   //}


   void* graphics::detach()
   {

      //plusplus::Graphics * pgraphics = m_pgraphics;

      //m_pgraphics = nullptr;

      //m_hdc = nullptr;

      //return pgraphics;

      return nullptr;

   }

   //plusplus::Font * graphics::vk2d_font()
   //{
   //   if(m_pfont.is_null())
   //   {
   //      m_pfont.create(this);
   //      if(m_pfont.is_set())
   //      {
   //         m_pfont->m_powner = this;
   //      }
   //   }
   //   if(m_pfont.is_null())
   //   {
   //      return nullptr;
   //   }
   //   return (plusplus::Font *) m_pfont->get_os_data();
   //}

   //plusplus::Brush * graphics::vk2d_brush()
   //{
   //   if(m_pbrush.is_null())
   //   {
   //      m_pbrush.create(this);
   //      if(m_pbrush.is_set())
   //      {
   //         m_pbrush->m_powner = this;
   //      }
   //   }
   //   if(m_pbrush.is_null())
   //   {
   //      return nullptr;
   //   }
   //   return (plusplus::Brush *) m_pbrush->get_os_data();
   //}

   //plusplus::Pen * graphics::vk2d_pen()
   //{

   //   synchronous_lock synchronouslock(this->synchronization());

   //   if(m_ppen.is_null())
   //   {
   //      m_ppen.create(this);
   //      if(m_ppen.is_set())
   //      {
   //         m_ppen->m_powner = this;
   //      }
   //   }
   //   if(m_ppen.is_null())
   //   {
   //      return nullptr;
   //   }
   //   return (plusplus::Pen *) m_ppen->get_os_data();
   //}

   //plusplus::FillMode graphics::vk2d_get_fill_mode()
   //{
   //   return plusplus::FillModeWinding;
   //}


   bool graphics::blur(bool bExpand, double dRadius, const ::i32_rectangle& rectangleParam)
   {

      //   // Commented Out for Running in cosan machine running Windows 2008

      //   //if(m_pbitmap.is_null() || m_pbitmap->get_os_data() == nullptr)
      //   //   return false;

      //   //plusplus::BlurParams myBlurParams;

      //   //myBlurParams.expandEdge    = bExpand ? 1 : 0;
      //   //myBlurParams.radius        = (float) dRadius;

      //   //plusplus::Blur myBlur;
      //   //myBlur.SetParameters(&myBlurParams);

      //   //

      //   //plusplus::Matrix m;
      //   //m_pgraphics->GetTransform(&m);

      //   //plusplus::PointF points[2];

      //   //points[0].X    = (plusplus::REAL) rectangleParam.left;
      //   //points[0].Y    = (plusplus::REAL) rectangleParam.top;
      //   //points[1].X    = (plusplus::REAL) rectangleParam.right;
      //   //points[1].Y    = (plusplus::REAL) rectangleParam.bottom;

      //   //m.TransformPoints(points, 2);

      //   ////plusplus::rectF float_rectangle(points[0].X, points[0].Y, points[1].X - points[0].X, points[1].Y - points[0].Y);

      //   //::f64_rectangle ::f64_rectangle;

      //   //rectangle.left      = (::double) points[0].X;
      //   //rectangle.top       = (::double) points[0].Y;
      //   //rectangle.right     = (::double) points[1].X;
      //   //rectangle.bottom    = (::double) points[1].Y;

      //   //plusplus::Bitmap * pbitmap = ((plusplus::Bitmap *) m_pbitmap->get_os_data());

      //   //pbitmap->ApplyEffect(&myBlur, &rectangle);

      //   return true;

      return true;
   }


   double graphics::get_dpix() const
   {

      //if (m_pgraphics == nullptr)
      //   return 96.0;

      //return m_pgraphics->GetDpiX();

      return 96.0;

   }


   double graphics::get_dpiy() const
   {

      //if (m_pgraphics == nullptr)
      //   return 96.0;

      //return m_pgraphics->GetDpiY();

      return 96.0;

   }


   void graphics::flush()
   {

      synchronous_lock synchronouslock(this->synchronization());

      //      m_pgraphics->Flush();

            //return true;

   }


   void graphics::sync_flush()
   {

      synchronous_lock synchronouslock(this->synchronization());

      //m_pgraphics->Flush(plusplus::FlushIntentionSync);

      //return true;

   }


   //HDC graphics::get_hdc()
   //{

   //   if(m_hdc != nullptr)
   //      return m_hdc;

   //   ////if(m_pgraphics == nullptr)
   //   //   return nullptr;

   //   //return m_pgraphics->GetHDC();

   //   return nullptr;

   //}

   //void graphics::release_hdc(HDC hdc)
   //{

   //   if(m_hdc != nullptr)
   //      return;

   //   //m_pgraphics->ReleaseHDC(hdc);

   //}


   //void graphics::enum_fonts(::write_text::font_enumeration_item_array& itema)
   //{

   //   return;

   //   synchronous_lock synchronouslock(this->synchronization());

   //   draw2d_nanovg_enum_fonts fonts(itema);

   //   //HDC hdc = ::CreateCompatibleDC(nullptr);

   //   //::EnumFontFamilies(hdc,(LPCTSTR)nullptr,(FONTENUMPROC)draw2d_nanovg_EnumFamCallBack,(LPARAM)&fonts);

   //   //::DeleteDC(hdc);

   //   // straPath = stra;

   //}


   //bool graphics::prefer_mapped_image_on_mix()
   //{

   //   return true;

   //}


   void graphics::create_window_graphics(const ::operating_system::window & window)
   {

      // http://stackoverflow.com/questions/4052940/how-to-make-an-opengl-rendering-context-with-transparent-background
      //

      //PIXELFORMATDESCRIPTOR pfd =
      //{
      //   sizeof(PIXELFORMATDESCRIPTOR),
      //   1,                                // Version Number
      //   PFD_DRAW_TO_WINDOW |         // Format Must Support Window
      //   PFD_SUPPORT_OPENGL |         // Format Must Support Opengl
      //   PFD_SUPPORT_COMPOSITION |         // Format Must Support Composition
      //   PFD_DOUBLEBUFFER,                 // Must Support Double Buffering
      //   PFD_TYPE_RGBA,                    // Request An RGBA Format
      //   32,                               // Select Our Color Depth
      //   0, 0, 0, 0, 0, 0,                 // Color Bits Ignored
      //   8,                                // An Alpha Buffer
      //   0,                                // Shift Bit Ignored
      //   0,                                // No Accumulation Buffer
      //   0, 0, 0, 0,                       // Accumulation Bits Ignored
      //   24,                               // 16Bit Z-Buffer (Depth Buffer)
      //   8,                                // Some Stencil Buffer
      //   0,                                // No Auxiliary Buffer
      //   PFD_MAIN_PLANE,                   // Main Drawing Layer
      //   0,                                // Reserved
      //   0, 0, 0                           // Layer Masks Ignored
      //};


      //DWM_BLURBEHIND bb = { 0 };
      ////HRGN hRgn = CreateRectRgn(0, 0, -1, -1);
      ////bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
      //bb.dwFlags = DWM_BB_ENABLE;
      ////bb.hRgnBlur = hRgn;
      //bb.fEnable = true;
      //DwmEnableBlurBehindWindow(wnd, &bb);


      //m_hdc = GetDC(wnd);
      //int PixelFormat = ChoosePixelFormat(m_hdc, &pfd);
      //if (PixelFormat == 0)
      //{
      //   ASSERT(0);
      //   return false;
      //}

      //BOOL bResult = SetPixelFormat(m_hdc, PixelFormat, &pfd);
      //if (bResult == false)
      //{
      //   ASSERT(0);
      //   return false;
      //}

      //m_hglrc = wglCreateContext(m_hdc);
      //if (!m_hglrc)
      //{
      //   ASSERT(0);
      //   return false;
      //}

//      return true;
      //return false;

   }


   //oswindow graphics::get_window_handle() const
   //{

   //   if (::is_null(m_puserinteractionDraw2dGraphics))
   //   {

   //      return nullptr;

   //   }

   //   return m_puserinteractionDraw2dGraphics->get_handle();

   //}


   void graphics::is_valid_update_window_thread()
   {

      //return ::GetCurrentThreadId() == psystem->get_twf()->m_uiThread;
      //return ::GetCurrentThreadId() == ::GetWindowThreadProcessId(m_hwnd, NULL);

   }


   //void graphics::_add_clipping_shape(const ::f64_rectangle & rectangle, ::draw2d::region * pregion)
   //{


   //}


   void graphics::send(const ::procedure& procedure)
   {

      ::gpu::graphics::send(procedure);

      //auto pgpucontext = gpu_context();

      //pgpucontext->_send(procedure);

   }


   void graphics::on_start_layer(::gpu::layer* pgpulayer)
   {

      //glClearColor(r, g, b, a);  // Set background color
      //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  // Clear buffers
      auto pgpucontext = gpu_context();

      pgpucontext->clear(pgpucontext->current_target_texture(::gpu::current_layer()), ::color::transparent);

      ::cast < ::gpu::layer > playerPrevious = pgpucontext->m_pgpudevice->get_previous_layer(pgpulayer);

      if (playerPrevious)
      {

         nvgBeginFrame(m_pdc, pgpucontext->m_rectangle.width(),
            pgpucontext->m_rectangle.height(), 1.f);

      }

      ////if (m_pdevicecontext && m_bInLayer)
      ////if (m_pdevicecontext)
      //{

      //   m_pdirect2d->m_pd2d1multithread->Enter();

      //   m_pdevicecontext->BeginDraw();

      //   m_pdevicecontext->Clear();

      //}

   }


   void graphics::on_end_layer(::gpu::layer* pgpulayer)
   {

      nvgEndFrame(m_pdc);

      auto pgpuimage = dynamic_cast < ::gpu::image * >(m_pimage);

      if (pgpuimage && pgpuimage->gpu_texture())
      {

         pgpuimage->gpu_texture()->defer_fence();

      }

      glFlush();
      ::opengl::check_error("");

      m_bHadEndLayer = true;

      //glClearColor(r, g, b, a);  // Set background color
      //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  // Clear buffers
      //auto pgpucontext = gpu_context();

      //pgpucontext->clear(::color::transparent);

      ////if (m_pdevicecontext && m_bInLayer)
      ////if (m_pdevicecontext)
      //{

      //   m_pdirect2d->m_pd2d1multithread->Enter();

      //   m_pdevicecontext->BeginDraw();

      //   m_pdevicecontext->Clear();

      //}

   }


   void graphics::start_layer(bool bFirstLayer)
   {

      thread_select();

      m_bHadEndLayer = false;

      ::gpu::context_lock contextlock(gpu_context());

      ::gpu::graphics::start_layer(bFirstLayer);

      //auto pgraphics = pgraphicscontext->draw2d_graphics();

      if (m_egraphics == e_graphics_draw)
      {

         auto size = total_size();

         ::i32_rectangle rectangle;

         rectangle.set_size(size);

         {

            ::string strMessage;

            strMessage.formatf("ø on_begin_draw");

            glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0, GL_DEBUG_SEVERITY_NOTIFICATION,
                                 -1, strMessage);
         }

         {

            ::string strMessage;

            ::cast<::gpu_opengl::texture> ptexture = gpu_context()->current_target_texture(::gpu::current_layer());

            auto uTexture = ptexture->m_gluTextureID;

            auto uFbo = ptexture->frame_buffer_object();

            strMessage.formatf("ø texture=%d fbo=%d", uTexture, uFbo);

            glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0, GL_DEBUG_SEVERITY_NOTIFICATION,
                                 -1, strMessage);
         }

         {

            GLint drawFbo = 0;
            glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFbo);

            GLint readFbo = 0;
            glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFbo);

            ::string strMessage;

            strMessage.formatf("ø drawFbo=%d readFbo=%d", drawFbo, readFbo);

            glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0, GL_DEBUG_SEVERITY_NOTIFICATION,
                                 -1, strMessage);
         }

         nvgBeginFrame(m_pdc, (float)size.width(), (float)size.height(), 1.0f);

         set_alpha_mode(::draw2d::e_alpha_mode_set);

         fill_rectangle(rectangle, ::color::transparent);

         set_alpha_mode(::draw2d::e_alpha_mode_blend);

         nvgResetScissor(m_pdc);

         reset_clip();

         reset_impact_area();

         update_matrix();

      }

      // bool bYSwap = m_papplication->m_gpu.m_bUseSwapChainWindow;

      //::opengl::resize(rectangle.size(), bYSwap);

      // m_z = 0.f;

      // auto pgpucontext = gpu_context();

      // if (!pgpucontext->m_pgpurenderer)
      //{

      //   constructø(pgpucontext->m_pgpurenderer);

      //   pgpucontext->m_eoutput = ::gpu::e_output_gpu_buffer;

      //   pgpucontext->m_escene = ::gpu::e_scene_2d;

      //   pgpucontext->m_pgpurenderer->initialize_gpu_renderer(pgpucontext);

      //}

      ////if (m_callbackImage32CpuBuffer)
      ////{

      ////   m_pgpucontext->m_callbackImage32CpuBuffer = m_callbackImage32CpuBuffer;

      ////}

   }


   //void graphics::start_layer(::e_graphics egraphics)
   //{

   //   thread_select();

   //   m_bHadEndLayer = false;

   //   ::gpu::context_lock contextlock(gpu_context());

   //   ::gpu::graphics::on_begin_draw();

   //   if (m_egraphics & e_graphics_draw)
   //   {

   //      auto size = total_size();

   //      ::i32_rectangle rectangle;

   //      rectangle.set_size(size);

   //      {

   //         ::string strMessage;

   //         strMessage.formatf("ø on_begin_draw");

   //         glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION,
   //            GL_DEBUG_TYPE_MARKER,
   //            0,
   //            GL_DEBUG_SEVERITY_NOTIFICATION,
   //            -1,
   //            strMessage);

   //      }

   //      {

   //         ::string strMessage;

   //         ::cast < ::gpu_opengl::texture > ptexture = gpu_context()->current_target_texture(::gpu::current_layer());

   //         auto uTexture = ptexture->m_gluTextureID;

   //         auto uFbo = ptexture->frame_buffer_object();

   //         strMessage.formatf("ø texture=%d fbo=%d", uTexture, uFbo);

   //         glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION,
   //            GL_DEBUG_TYPE_MARKER,
   //            0,
   //            GL_DEBUG_SEVERITY_NOTIFICATION,
   //            -1,
   //            strMessage);

   //      }

   //      {

   //         GLint drawFbo = 0;
   //         glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFbo);

   //         GLint readFbo = 0;
   //         glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFbo);

   //         ::string strMessage;

   //         strMessage.formatf("ø drawFbo=%d readFbo=%d", drawFbo, readFbo);

   //         glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION,
   //            GL_DEBUG_TYPE_MARKER,
   //            0,
   //            GL_DEBUG_SEVERITY_NOTIFICATION,
   //            -1,
   //            strMessage);

   //      }

   //      nvgBeginFrame(m_pdc, (float) size.width(), (float) size.height(), 1.0f);

   //      set_alpha_mode(::draw2d::e_alpha_mode_set);

   //      fill_rectangle(rectangle, ::color::transparent);

   //      set_alpha_mode(::draw2d::e_alpha_mode_blend);

   //      nvgResetScissor(m_pdc);

   //      reset_clip();

   //      reset_impact_area();

   //   }

   //   //bool bYSwap = m_papplication->m_gpu.m_bUseSwapChainWindow;

   //   //::opengl::resize(rectangle.size(), bYSwap);

   //   //m_z = 0.f;

   //   //auto pgpucontext = gpu_context();

   //   //if (!pgpucontext->m_pgpurenderer)
   //   //{

   //   //   constructø(pgpucontext->m_pgpurenderer);

   //   //   pgpucontext->m_eoutput = ::gpu::e_output_gpu_buffer;

   //   //   pgpucontext->m_escene = ::gpu::e_scene_2d;

   //   //   pgpucontext->m_pgpurenderer->initialize_gpu_renderer(pgpucontext);

   //   //}

   //   ////if (m_callbackImage32CpuBuffer)
   //   ////{

   //   ////   m_pgpucontext->m_callbackImage32CpuBuffer = m_callbackImage32CpuBuffer;

   //   ////}

   //}


   //void graphics::defer_add_gpu_render(::gpu::render * pgpurender)
   //{

   //   m_pgpucontext->m_rendera.add_unique(pgpurender);

   //}


   void graphics::initialize(::particle* pparticle)
   {

      ::draw2d::graphics::initialize(pparticle);

      //   ::gpu::renderer::initialize(pparticle);

   }


   void graphics::end_layer(bool bClosingLayer)
   {

      if (m_egraphics == e_graphics_draw)
      {

         auto pgpucontext = gpu_context();

         ::gpu::context_lock contextlock(pgpucontext);

         //nanovg_surface_resolve(m_nanovgsurface);

         //m_pgpucontext->m_prenderer->on_end_draw();

         //::f64_rectangle r{ 0.0, 0.0, 1920.0, 1080.0 };

         //fill_solid_rectangle(r, argb(255, 100, 200, 240));

         //::f64_rectangle r2{ 100.0, 100.0, 192.0, 198.0 };

         //set_alpha_mode(::draw2d::e_alpha_mode_blend);

         //fill_solid_rectangle(r2, argb(155, 120, 40, 100));

         //nanovg_flush(m_pdc);

         //defer_constructø(m_ptextureCurrent);

         //::cast < ::gpu_opengl::texture > ptextureEndDraw = m_ptextureEndDraw;

         //ptextureEndDraw->m_vkimage = nanovg_surface_get_vk_image(m_nanovgsurface);



         ::i32_rectangle rectangle;

         if (m_puserinteractionDraw2dGraphics && !m_puserinteractionDraw2dGraphics->host_rectangle().size().is_empty())
         {

            rectangle = m_puserinteractionDraw2dGraphics->host_rectangle();

         }
         else
         {

            rectangle = { 0, 0, 1920, 1080 };

         }

         //if (!m_pgpucontextOutput)
         //{

         //   constructø(m_pgpucontextOutput);

         //   ::cast < ::windowing::window > pwindow = m_puserinteractionDraw2dGraphics->m_pacmewindowingwindow;

         //   //m_pgpucontextOutput->create

         //   //m_pgpucontextOutput = m_papplication->get_gpu_approach()->get_gpu_device(m_puserinteractionDraw2dGraphics->m_pacmewindowingwindow)->create_window_context(pwindow);

         //   //m_pgpucontextOutput->create_window_buffer(pwindow);

         //}

         if (!m_bHadEndLayer)
         {

            nvgEndFrame(m_pdc);

         }

         {

            {

               ::string strMessage;

               strMessage.formatf("ø on_end_draw");

               glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION,
                  GL_DEBUG_TYPE_MARKER,
                  0,
                  GL_DEBUG_SEVERITY_NOTIFICATION,
                  -1,
                  strMessage);

            }

            ::string strMessage;

            ::cast < ::gpu_opengl::texture > ptexture = gpu_context()->current_target_texture(::gpu::current_layer());

            auto uTexture = ptexture->m_gluTextureID;

            auto uFbo = ptexture->frame_buffer_object();

            strMessage.formatf("ø texture=%d fbo=%d", uTexture, uFbo);

            glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION,
               GL_DEBUG_TYPE_MARKER,
               0,
               GL_DEBUG_SEVERITY_NOTIFICATION,
               -1,
               strMessage);

         }

         {

            GLint drawFbo = 0;
            glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFbo);

            GLint readFbo = 0;
            glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFbo);

            ::string strMessage;

            strMessage.formatf("ø drawFbo=%d readFbo=%d", drawFbo, readFbo);

            glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION,
               GL_DEBUG_TYPE_MARKER,
               0,
               GL_DEBUG_SEVERITY_NOTIFICATION,
               -1,
               strMessage);

         }


         //::cast < ::gpu_opengl::renderer > prenderer = m_pgpucontextOutput->get_gpu_renderer();

         ////m_pgpucontext->m_eoutput = ::gpu::e_output_gpu_buffer;

         //prenderer->_on_graphics_end_draw(m_ptextureEndDraw, rectangle);

         ////prenderer->_blend_image(vkimage, rectangle);



      }

      ////vkPushMatrix();

      ////vkColor3f(0, 1, 1);
      //vkBegin(VK_TRIANGLES);                              // Drawing Using Triangles
      //

      //vkColor4f(1.0f, 0.0f, 0.0f, 0.5f);                      // Set The Color To Red
      //vkVertex3f(100.0f, -2000.0f, 0.0f);                  // Top
      //

      //vkColor3f(0.0f, 1.0f, 0.0f);                      // Set The Color To Green
      //vkVertex3f(0.0f, 200.0f, 0.0f);                  // Bottom Left


      //vkColor3f(0.0f, 0.0f, 1.0f);                      // Set The Color To Blue
      //vkVertex3f(2000.0f, 2000.0f, 0.0f);                  // Bottom Right

      //vkEnd();

      //vkPopMatrix();


      //vkFlush();
      //vkFinish();
      //vkDisable(VK_BLEND);




      //SwapBuffers(m_hdc);

      //m_pgpucontextOpengl->render

      //dr();

      if (m_papplication->m_gpu.m_bUseSwapChainWindow)
      {

         //m_pgpucontext->swap_buffers();

         //m_pwindow->m_timeLastDrawGuard1.Now();

      }
      else
      {

         //m_pgpucontext->swap_buffers();

      //}
      //else
      //{

         //read_to_cpu_buffer();

         //m_pimage->map();

         //m_pimage->copy(m_pgpucontext->m_pcpubuffer->m_pimagetarget->m_pimage);

      }


      ::gpu::graphics::end_layer(bClosingLayer);

   }


   void graphics::on_present()
   {

      //if (m_papplication->m_gpu.m_bUseSwapChainWindow)
      //{

      //   m_pgpucontext->swap_buffers();

      //}

   }


   void graphics::thread_select()
   {

      if (thread_graphics() == this)
      {

         return;
      }

      //wglMakeCurrent(m_hdc, m_hglrc);

      //m_pgpucontext->make_current();

      thread_graphics(this);

   }


   bool graphics::_is_ok() const
   {

      //return ::is_set(this) & ::is_set(m_hglrc);

      return ::is_set(this) && m_pgpucontextCompositor2;

   }




   void graphics::intersect_clip(const ::f64_rectangle& rectangle)
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      nvgIntersectScissor(
         m_pdc,
         (float)rectangle.left,
         (float)rectangle.top,
         (float)rectangle.width(),
         (float)rectangle.height());


   }


   void graphics::intersect_clip(::draw2d::clip_group *pclipgroup)
   {

      ::f64_rectangle rectangleBounds;

      bool bHasBounds = false;

      for (auto &pclipitem: *pclipgroup)
      {

         if (!pclipitem)
         {

            continue;

         }

         ::f64_rectangle rectangleItem;

         switch (pclipitem->clip_item_type())
         {
         case ::draw2d::e_clip_item_rectangle:
            rectangleItem = dynamic_cast<::draw2d::clip_rectangle*>(pclipitem.m_p)->m_item;
            break;
         case ::draw2d::e_clip_item_ellipse:
            rectangleItem = dynamic_cast<::draw2d::clip_ellipse*>(pclipitem.m_p)->m_item;
            break;
         case ::draw2d::e_clip_item_polygon:
            rectangleItem = dynamic_cast<::draw2d::clip_polygon*>(pclipitem.m_p)->m_item.bounding_rect();
            break;
         default:
            continue;
         }

         if (bHasBounds)
         {

            rectangleBounds.unite(rectangleItem);

         }
         else
         {

            rectangleBounds = rectangleItem;

            bHasBounds = true;

         }

      }

      // NanoVG exposes rectangular scissoring only, so use the bounds of this
      // union-of-shapes clip group and intersect it with prior clip groups.
      if (bHasBounds)
      {

         intersect_clip(rectangleBounds);

      }



   }


   void graphics::reset_clip()
   {

      _synchronous_lock ml(::draw2d_nanovg::mutex());

      nvgResetScissor(m_pdc);

   }



   void graphics::_vk_rectangle(const ::f64_rectangle& rectangle)
   {

      //vkVertex2f((VKfloat)rectangle.left, (VKfloat)rectangle.top);
      //vkVertex2f((VKfloat)rectangle.right, (VKfloat)rectangle.top);
      //vkVertex2f((VKfloat)rectangle.right, (VKfloat)rectangle.bottom);
      //vkVertex2f((VKfloat)rectangle.left, (VKfloat)rectangle.bottom);

      struct Vertex {
         floating_sequence2 pos;
         floating_sequence3 color;
      };

      ::array_base<Vertex> vertexes = {
          {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}}, // Bottom-left
          {{ 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}}, // Bottom-right
          {{ 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}}, // Top-right
          {{-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}, // Top-left
      };

      ::array_base<unsigned short> indexes = {
          0, 1, 2, 2, 3, 0 // Two triangles
      };

   }


   void graphics::defer_load_font_by_family_name(const ::scoped_string& scopedstrName)
   {

      ::draw2d_nanovg::get()->defer_load_font_by_family_name(m_pdc, scopedstrName);

   }



} // namespace draw2d_nanovg



#ifdef WINDOWS_DESKTOP

BOOL CALLBACK draw2d_nanovg_EnumFamCallBack(LPLOGFONT lplf, LPNEWTEXTMETRIC lpntm, unsigned int FontType, void * p)
{

   draw2d_nanovg_enum_fonts* pfonts = (draw2d_nanovg_enum_fonts*)p;

   if (FontType & RASTER_FONTTYPE)
   {

   }
   else if (FontType & TRUETYPE_FONTTYPE)
   {

      pfonts->m_itema.add(allocateø::write_text::font_enumeration_item(lplf->lfFaceName));

   }
   else
   {

   }

   return true;

}


#endif


namespace opengl
{


   void resize(const ::i32_size& size, bool bYSwap)
   {

      //double d = 200.0 / 72.0;

      //double d = 1.0;

      ////vkViewport(0, 0, size.cx * d, size.cy * d);
      //vkViewport(0, 0, size.cx, size.cy);

      //vkMatrixMode(VK_PROJECTION);
      //vkLoadIdentity();
      ////vkOrtho(0, size.cx * d, size.cy * d, 0.0f, 000.0f, 1000.0f);
      ////vkOrtho(0, size.cx * d, size.cy * d, 0.0f, 000.0f, 1000.0f);
      //////vkOrtho(0, size.cx * d, 0.0f, size.cy * d, 000.0f, 1000.0f);
      ////vkOrtho(0, size.cx, size.cy, 0.0f, -1000.0f, 1000.0f);
      //vkOrtho(0.f, size.cx, 0.f, -size.cy, -1.0f, 1.0f);


      //vkMatrixMode(VK_PROJECTION);
      //vkLoadIdentity();
      //if (bYSwap)
      //{
      //   vkOrtho(0.0f, size.cx, size.cy, 0, -1.0f, 1.0f);  // Flip Y
      //}
      //else
      //{
      //   vkOrtho(0.0f, size.cx, 0, size.cy, -1.0f, 1.0f);  // Flip Y
      //}
      ////auto left = 0.;
      ////auto right = (double) size.cx;
      ////auto bottom = 0.;
      ////auto top = (double)size.cy;
      ////double dFar = 1.0;
      ////double dNear = -1.0;
      ////double tx = -(right + left) / (right - left);
      ////double ty = -(top + bottom) / (top - bottom);
      ////double tz = -(dFar + dNear) / (dFar - dNear);
      ////double a[] =
      ////{
      ////   2.0/(right - left),0.0,0.0,tx,
      ////   0.0,2.0/(top - bottom),0.0,ty,
      ////   0.0,0.0,-2.0/(dFar-dNear),tz,
      ////   0.0,0.0,0.0,1.0
      ////};
      ////vkMultMatrixd(a);

      ////vkMatrixMode(VK_MODELVIEW);
      ////vkLoadIdentity();


      ////vkuOrtho2D(0.f, size.cx, 0.f, size.cy);
      ////vkMatrixMode(VK_MODELVIEW);
      ////vkLoadIdentity();

      //// Clear
      ////vkClearColor(1.0f, 1.0f, 1.0f, 1.0f);
      ////vkClear(VK_COLOR_BUFFER_BIT | VK_DEPTH_BUFFER_BIT);

      //// Translate to inside of pixel (otherwise inaccuracies can occur on certain vk implementations)
      ////if (Opengl::accuracyTweak())
      //vkTranslatef(0.5f, 0.5f, 0);

   }

   //https://community.khronos.org/t/draw-an-arc-in-opengl/57994/2
   inline void draw_arc(float cx, float cy, float r, float start_angle, float arc_angle, int num_segments)
   {
      float theta = arc_angle / float(num_segments - 1);//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open

      float tangetial_factor = tanf(theta);

      float radial_factor = cosf(theta);


      float x = r * cosf(start_angle);//we now start at the start angle
      float y = r * sinf(start_angle);

      //vkBegin(VK_LINE_STRIP);//since the arc is not a closed curve, this is a strip now
      //for (int ii = 0; ii < num_segments; ii++)
      //{
      //   vkVertex2f(x + cx, y + cy);

      //   float tx = -y;
      //   float ty = x;

      //   x += tx * tangetial_factor;
      //   y += ty * tangetial_factor;

      //   x *= radial_factor;
      //   y *= radial_factor;
      //}
      //vkEnd();
   }




} // namespace opengl




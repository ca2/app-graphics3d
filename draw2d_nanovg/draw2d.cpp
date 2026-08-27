#include "platform.h"
#include "gpu_opengl/_gpu_opengl.h"
#include "draw2d.h"
#include "acme/exception/resource.h"
#include "acme/filesystem/filesystem/file_context.h"
#include "acme/platform/application.h"
#include "acme/platform/node.h"
#include "acme/prototype/prototype/memory.h"
#include "fontstash.h"
#include "aura/graphics/image/image.h"

//CLASS_DECL_DRAW2D_NANOVG void initialize_opengl();
//CLASS_DECL_DRAW2D_NANOVG void terminate_opengl();


namespace draw2d_nanovg
{


   //draw2d::private_font::private_font()
   //{


   //}


   //draw2d::private_font::~private_font()
   //{



   //}
   ::draw2d_nanovg::draw2d* g_pdraw2dnanovg = nullptr;

   ::draw2d_nanovg::draw2d* get()
   {
      return g_pdraw2dnanovg;

   }


   ::particle* mutex()
   {

      return get()->m_pmutex;

   }

   draw2d::draw2d()
   {
      g_pdraw2dnanovg = this;
      //m_atomClass = NULL;
      //m_bGladInitialized = false;

   }


   draw2d::~draw2d()
   {

      //m_mapPrivateFont.clear();

      //terminate_opengl();

   }


   ::draw2d::graphics_pointer draw2d::do_allocation_strategy(::acme::user::interaction * pacmeuserinteractionAffinity, ::image::image * pimage,
                                                          const ::i32_size & size)
   {

      if (::is_set(pimage))
      {

         auto pgraphicsOwned = pimage->m_pgraphicsOwned;

         if (pgraphicsOwned)
         {

            return pgraphicsOwned;

         }

         auto pdraw2dgraphics = allocate_graphics(pacmeuserinteractionAffinity);

         pdraw2dgraphics->update_as_image_render_target(pimage);

         return pdraw2dgraphics;

         //auto pdraw2dbitmap = pimage->get_bitmap();

         //if (::is_set(pdraw2dbitmap))
         //{

         //   pdraw2dgraphics->create_bitmap_graphics(pdraw2dbitmap);
         //}
         //else
         //{

         //   throw ::exception(error_wrong_state);
         //}
      }
      else
      {
         auto pdraw2dgraphics = allocate_graphics(pacmeuserinteractionAffinity);
         pdraw2dgraphics->create_memory_graphics(size, pacmeuserinteractionAffinity);
         return pdraw2dgraphics;
      }



      //return ::draw2d::draw2d::do_allocation_strategy(pdraw2dhost, pimage, size);

      //auto pdraw2dgraphics = create_memory_graphics(pdraw2dhost, size);

      //if (::is_set(pimage))
      //{

      //   pimage->create_from_graphics(pdraw2dgraphics);

      //}

      //return pdraw2dgraphics;

   }


   void draw2d::initialize(::particle * pparticle)
   {

      //auto estatus = 

      ::gpu::draw2d::initialize(pparticle);

      m_bNanoVGGeometryAntialias =
         !m_papplication->m_gpu.m_bMultisample;

      //application()->create_gpu();
      //if (!estatus)
      //{

      //   return estatus;

      //}

      //estatus = 

      m_pmutex = node()->create_mutex();
      opengl_init();



      defer_create_synchronization();
      //initialize_opengl();

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //return estatus;

   }


   bool draw2d::write_text_supports_raster_fonts()
   {

      return false;

   }


   bool draw2d::write_text_supports_legacy_gdi_fonts()
   {

      return false;

   }


   //bool draw2d::graphics_context_supports_single_buffer_mode()
   //{

   //   return true;

   //}


   bool draw2d::graphics_context_does_full_redraw()
   {

      return true;

   }


   //void draw2d::defer_initialize_glad()
   //{

   //   if (m_bGladInitialized)
   //   {

   //      return;

   //   }



   //   if(!vkadLoadGL())
   //   {
   //      // Problem: vkewInit failed, something is seriously wrong.
   //      informationf("vkadLoadGL failed");
   //      //return false;
   //      throw resource_exception();
   //   }

   //   m_bGladInitialized = true;


   //}


   string draw2d::write_text_get_default_implementation_name()
   {

      return "win32";

   }





   //draw2d::private_font * draw2d::get_file_private_font(::platform::context * pcontext, const ::file::path & path)
   //{

   //   auto & pwritetextfont = m_mapPrivateFont[path];

   //   if (::is_set(pwritetextfont))
   //   {

   //      return pwritetextfont;

   //   }

   //   construct_newø(pwritetextfont);

   //   pwritetextfont->m_pcollection = ___new Gdiplus::PrivateFontCollection();

   //   auto pmemory = system()->draw2d()->write_text()->get_file_memory(pcontext, path);

   //   if (pmemory->has_data())
   //   {

   //      pwritetextfont->m_pcollection->AddMemoryFont(pmemory->data(), (INT)pmemory->size());

   //      auto & fontCollection = *pwritetextfont->m_pcollection;

   //      auto iFamilyCount = fontCollection.GetFamilyCount();

   //      pwritetextfont->m_familya.set_size(iFamilyCount);

   //      fontCollection.GetFamilies(iFamilyCount, pwritetextfont->m_familya.data(), &pwritetextfont->m_iFamilyCount);

   //      pwritetextfont->m_familya.set_size(iFamilyCount);

   //      for (int iFamily = 0; iFamily < iFamilyCount; iFamily++)
   //      {

   //         if (pwritetextfont->m_familya[iFamily].GetLastStatus() != Gdiplus::Ok)
   //         {

   //            warningf("font family nok");

   //         }

   //      }

   //   }

   //   return pwritetextfont;

   //}


   // LRESULT CALLBACK opengl_window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
   // {
   //    return DefWindowProc(hwnd, message, wparam, lparam);
   // }


   int  draw2d::opengl_init()
   {

      //constructø(m_popenglcontext);

      //if (m_atomClass)
      //{

      //   return true;

      //}

      //WNDCLASSW wc;
      //ZeroMemory(&wc, sizeof(wc));
      //wc.style = CS_OWNDC;
      //wc.lpfnWndProc = opengl_window_proc;
      //wc.hInstance = (HINSTANCE) system()->m_hinstanceThis;
      //wc.lpszClassName = L"draw2d_nanovg_offscreen_buffer_window";
      //m_atomClass = RegisterClassW(&wc);

      //if (!m_atomClass)
      //{

      //   return false;

      //}

      //defer_initialize_glad();

      return true;

   }


   ::string draw2d::defer_load_font(NVGcontext * pdc, ::write_text::font * pwritetextfont)
   {

      _synchronous_lock lock(m_pmutex);

      ::write_text::font_face_request request;
      request.m_strFamily = pwritetextfont->family_name();
      request.m_fontweight = pwritetextfont->m_fontweight;
      request.m_bItalic = pwritetextfont->m_bItalic;

      ::string strFontKey;
      strFontKey.formatf(
         "family=%s;weight=%d;italic=%d",
         request.m_strFamily.c_str(),
         request.m_fontweight.as_i32(),
         request.m_bItalic ? 1 : 0);

      auto iFont = nvgFindFont(pdc, strFontKey);

      if (iFont >= 0)
      {

         return strFontKey;

      }

      ::write_text::font_face_source source;

      if (system()->draw2d()->write_text()->resolve_font_face(source, request))
      {

         iFont = nvgCreateFontAtIndex(
            pdc,
            strFontKey,
            source.m_path,
            source.m_iFaceIndex);

      }

      if (iFont < 0)
      {

         const auto bExists = source.m_path.has_character() && file()->exists(source.m_path);
         const auto strExtension = ::string(source.m_path.final_extension());
         ::string strMessage;

         strMessage.formatf(
            "NanoVG failed to load the requested font into the current graphics context. "
            "family=\"%s\" weight=%d italic=%s resolved_family=\"%s\" "
            "path=\"%s\" face_index=%d exists=%s extension=\"%s\".",
            request.m_strFamily.c_str(),
            request.m_fontweight.as_i32(),
            request.m_bItalic ? "true" : "false",
            source.m_strResolvedFamily.c_str(),
            source.m_path.c_str(),
            source.m_iFaceIndex,
            bExists ? "true" : "false",
            strExtension.c_str());

         information() << "[draw2d_nanovg.font] " << strMessage;

         throw ::exception(
            error_failed,
            strMessage);

      }

      return strFontKey;

   }



} // namespace draw2d_nanovg




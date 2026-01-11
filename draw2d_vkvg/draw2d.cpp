#include "framework.h"
#include "draw2d.h"
#include "acme/exception/resource.h"
#include "acme/platform/application.h"
#include "acme/platform/node.h"
#include "acme/prototype/prototype/memory.h"


//CLASS_DECL_DRAW2D_VKVG void initialize_vulkan();
//CLASS_DECL_DRAW2D_VKVG void terminate_vulkan();


namespace draw2d_vkvg
{


   //draw2d::private_font::private_font()
   //{


   //}


   //draw2d::private_font::~private_font()
   //{



   //}
   ::draw2d_vkvg::draw2d* g_pdraw2dvkvg = nullptr;

   ::draw2d_vkvg::draw2d* get()
   {
      return g_pdraw2dvkvg;

   }


   ::particle* mutex()
   {

      return get()->m_pmutex;

   }

   draw2d::draw2d()
   {
      g_pdraw2dvkvg = this;
      //m_atomClass = NULL;
      //m_bGladInitialized = false;

   }


   draw2d::~draw2d()
   {

      //m_mapPrivateFont.clear();

      //terminate_vulkan();

   }


   void draw2d::initialize(::particle * pparticle)
   {

      //auto estatus = 

      ::gpu::draw2d::initialize(pparticle);


      //application()->create_gpu();
      //if (!estatus)
      //{

      //   return estatus;

      //}

      //estatus = 

      m_pmutex = node()->create_mutex();
      vulkan_init();



      defer_create_synchronization();
      //initialize_vulkan();

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //return estatus;

   }


   bool draw2d::graphics_context_supports_single_buffer_mode()
   {

      return true;

   }


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

   //   auto & pfont = m_mapPrivateFont[path];

   //   if (::is_set(pfont))
   //   {

   //      return pfont;

   //   }

   //   øconstruct_new(pfont);

   //   pfont->m_pcollection = ___new Gdiplus::PrivateFontCollection();

   //   auto pmemory = system()->draw2d()->write_text()->get_file_memory(pcontext, path);

   //   if (pmemory->has_data())
   //   {

   //      pfont->m_pcollection->AddMemoryFont(pmemory->data(), (INT)pmemory->size());

   //      auto & fontCollection = *pfont->m_pcollection;

   //      auto iFamilyCount = fontCollection.GetFamilyCount();

   //      pfont->m_familya.set_size(iFamilyCount);

   //      fontCollection.GetFamilies(iFamilyCount, pfont->m_familya.data(), &pfont->m_iFamilyCount);

   //      pfont->m_familya.set_size(iFamilyCount);

   //      for (int iFamily = 0; iFamily < iFamilyCount; iFamily++)
   //      {

   //         if (pfont->m_familya[iFamily].GetLastStatus() != Gdiplus::Ok)
   //         {

   //            warningf("font family nok");

   //         }

   //      }

   //   }

   //   return pfont;

   //}

#if defined(WINDOWS_DESKTOP)
   LRESULT CALLBACK vulkan_window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
   {
      return DefWindowProc(hwnd, message, wparam, lparam);
   }
#endif

   int  draw2d::vulkan_init()
   {

      //øconstruct(m_pvulkancontext);

      //if (m_atomClass)
      //{

      //   return true;

      //}

      //WNDCLASSW wc;
      //ZeroMemory(&wc, sizeof(wc));
      //wc.style = CS_OWNDC;
      //wc.lpfnWndProc = vulkan_window_proc;
      //wc.hInstance = (HINSTANCE) system()->m_hinstanceThis;
      //wc.lpszClassName = L"draw2d_vkvg_offscreen_buffer_window";
      //m_atomClass = RegisterClassW(&wc);

      //if (!m_atomClass)
      //{

      //   return false;

      //}

      //defer_initialize_glad();

      return true;

   }


   void draw2d::defer_load_font_by_family_name(VkvgContext pdc, const ::scoped_string& scopedstrName)
   {

      _synchronous_lock lock(m_pmutex);

      auto& font = m_mapFont[scopedstrName];

      if (!font.m_bLoaded)
      {

         font.m_bLoaded = true;

         ::file::path pathFont = node()->get_font_path_from_name(scopedstrName);

         vkvg_load_font_from_path(pdc, pathFont, scopedstrName);

         auto status = vkvg_status(pdc);

         if (status)
         {

            warning() << "oh no";

         }

      }

   }



} // namespace draw2d_vkvg




#include "platform.h"
#include "draw2d.h"
#include "acme/filesystem/filesystem/file_context.h"
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


   const void * draw2d::get_gpu_physical_device_features(void * p)
   {

      auto prequiredFeatures = (VkPhysicalDeviceFeatures *) p;

      const void * vkvgPNext =
         vkvg_get_device_requirements(
            prequiredFeatures);

      return vkvgPNext;

   }




   void draw2d::get_required_gpu_device_extensions(::u64 uPhysicalDevice, ::array<const char *> & pszaRequiredDeviceExtensions)
   {

      VkPhysicalDevice vkphysicaldevice = (VkPhysicalDevice)uPhysicalDevice;
      uint32_t extensionCount = 0;

      vkvg_get_required_device_extensions(
         vkphysicaldevice,
         nullptr,
         &extensionCount);

      ::array<const char *> vkvgExtensions;
      vkvgExtensions.set_size(extensionCount);
      

      vkvg_get_required_device_extensions(
         vkphysicaldevice,
         vkvgExtensions.data(),
         &extensionCount);

      

      for (int i = 0; i < vkvgExtensions.size(); i++)
      {

         auto ext = vkvgExtensions[i];

         if (ext)
         {

            bool bFound = false;

            for(int j = 0; j < pszaRequiredDeviceExtensions.size(); j++)
            {

               auto existing = pszaRequiredDeviceExtensions[j];

               if (existing)
               {

                  if (!strcmp(existing, ext))
                  {

                     bFound = true;

                     break;

                  }

               }
            
            }

            if (!bFound)
            {

               pszaRequiredDeviceExtensions.add(ext);

            }

         }

      }

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

#if defined(WINDOWS_DESKTOP)
   LRESULT CALLBACK vulkan_window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
   {
      return DefWindowProc(hwnd, message, wparam, lparam);
   }
#endif

   int  draw2d::vulkan_init()
   {

      //constructø(m_pvulkancontext);

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


   bool draw2d::write_text_supports_raster_fonts()
   {

      // This backend resolves file-backed outline faces, not GDI bitmap fonts.
      return false;

   }


   bool draw2d::write_text_supports_legacy_gdi_fonts()
   {

      // Keep legacy GDI-only faces out of the shared Windows enumeration.
      return false;

   }


   ::string draw2d::defer_load_font(
      VkvgContext pdc,
      VkvgDevice pdevice,
      ::write_text::font * pwritetextfont)
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

      ::string strDeviceFontKey;
      strDeviceFontKey.formatf("device=%p;%s", pdevice, strFontKey.c_str());

      auto& font = m_mapFont[strDeviceFontKey];

      if (!font.m_bLoaded)
      {

         ::write_text::font_face_source source;

         if (!system()->draw2d()->write_text()->resolve_font_face(source, request))
         {

            ::string strMessage;

            strMessage.formatf(
               "VKVG could not resolve the requested font face. family=\"%s\" weight=%d italic=%s.",
               request.m_strFamily.c_str(),
               request.m_fontweight.as_i32(),
               request.m_bItalic ? "true" : "false");

            information() << "[draw2d_vkvg.font] " << strMessage;

            throw ::exception(error_failed, strMessage);

         }

         vkvg_load_font_from_path(pdc, source.m_path, strFontKey);

         auto status = vkvg_status(pdc);

         if (status != VKVG_STATUS_SUCCESS)
         {

            const auto bExists = source.m_path.has_character() && file()->exists(source.m_path);
            const auto strExtension = ::string(source.m_path.final_extension());
            ::string strMessage;

            strMessage.formatf(
               "VKVG failed to load the requested font face. family=\"%s\" weight=%d italic=%s "
               "resolved_family=\"%s\" path=\"%s\" face_index=%d exists=%s extension=\"%s\" status=%d.",
               request.m_strFamily.c_str(),
               request.m_fontweight.as_i32(),
               request.m_bItalic ? "true" : "false",
               source.m_strResolvedFamily.c_str(),
               source.m_path.c_str(),
               source.m_iFaceIndex,
               bExists ? "true" : "false",
               strExtension.c_str(),
               (int)status);

            information() << "[draw2d_vkvg.font] " << strMessage;

            throw ::exception(error_failed, strMessage);

         }

         font.m_bLoaded = true;

      }

      return strFontKey;

   }



} // namespace draw2d_vkvg




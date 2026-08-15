#include "platform.h"
#include "draw2d.h"
///#include "gpu_directx12/directx12.h"
#include "acme/platform/application.h"
#include "aura/windowing/window.h"


namespace draw2d_directx12
{


   draw2d::draw2d()
   {

      defer_create_synchronization();

   }


   draw2d::~draw2d()
   {

      //::directx12::finalize();

   }


   void draw2d::initialize(::particle * pparticle)
   {

      //auto estatus = 
      
      ::draw2d::draw2d::initialize(pparticle);

      //if (!estatus)
      //{

      //   return estatus;

      //}
   
      /*::directx12::defer_initialize(this);*/

      //estatus = initialize_gdiplus();

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //return estatus;
   
   }



   string draw2d::write_text_get_default_implementation_name()
   {

      return system()->implementation_name("write_text", "directx12");

   }


   bool draw2d::lock_device()
   {

      //auto pdirectx12 = m_pgpudevice;

      //if (!pdirectx12)
      //{

      //   return false;

      //}

      //auto pmultithread = pdirectx12->m_pd2d1multithread.m_p;

      //if (!pmultithread)
      //{

      //   return false;

      //}

      //pmultithread->Enter();

      return true;

   }


   void draw2d::unlock_device()
   {
      
      //auto pdirectx12 = m_pgpudevice;

      //auto pmultithread = pdirectx12->m_pd2d1multithread.m_p;

      //pmultithread->Leave();

   }


   //void draw2d::on_create_window(::windowing::window* pwindow)
   //{

   //   ::gpu::draw2d::on_create_window(pwindow);

   //   ///auto rectangleWindow = pwindow->get_window_rectangle();
   //   
   //   //::directx12::defer_initialize(pwindow, rectangleWindow);

   //}


   void draw2d::adjust_composited_window_styles(unsigned int& nExStyle, unsigned int& nStyle)
   {

      nExStyle |= WS_EX_LAYERED;

      if (m_papplication->m_gpu.m_bUseSwapChainWindow)
      {

         nExStyle |= WS_EX_NOREDIRECTIONBITMAP;

      }

   }


} // namespace draw2d_directx12




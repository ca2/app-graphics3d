#include "platform.h"
#include "buffer.h"
#include "acme/parallelization/synchronous_lock.h"
#include "aura/graphics/image/image.h"
#include "aura/graphics/image/aaa_target.h"
#include "bred/gpu/context.h"


namespace gpu_vulkan
{


   buffer::buffer()
   {

   }


   buffer::~buffer()
   {

   }


   void buffer::gpu_read()
   {

      _synchronous_lock synchronouslock(this->synchronization());

      if (m_ppixmap.nok())
      {

         return;

      }

      //m_pixmap.map();

      auto cx = m_ppixmap->width();

      auto cy = m_ppixmap->height();

      //auto sizeNeeded = cx * cy * 4;

      //m_pixmap.create(m_memory, sizeNeeded);
      
      auto data = m_ppixmap->data();
      
      {

         auto dst = (unsigned char *)data;
         auto size = cx * cy;

         while (size > 0)
         {
            dst[0] = byte_clip(((int)dst[0] * (int)dst[3]) / 255);
            dst[1] = byte_clip(((int)dst[1] * (int)dst[3]) / 255);
            dst[2] = byte_clip(((int)dst[2] * (int)dst[3]) / 255);
            dst += 4;
            size--;
         }

      }

      //::copy_image32(m_pixmap.m_pimage32,
      //   cx, cy,
      //   m_pixmap.m_iScan,
      //   (const ::image32_t*) data, cx * 4);

   }


   void buffer::gpu_write()
   {

      synchronous_lock synchronouslock(this->synchronization());

      if (m_ppixmap.nok())
      {

         return;

      }

// //     m_pixmap.map();
//      //
////      glDrawPixels(
////         m_pixmap.m_size.cx, m_pixmap.m_size.cy,
////         GL_BGRA,
////         GL_UNSIGNED_BYTE,
////         m_pixmap.m_pimage32Raw);
//      
//      glTexImage2D(GL_TEXTURE_2D, 0, 0, 0,
//                   m_pixmap.m_size.cx, m_pixmap.m_size.cy,
//                   GL_RGBA, GL_UNSIGNED_BYTE,
//                   m_pixmap.m_pimage32Raw);

   }


} // namespace gpu_vulkan




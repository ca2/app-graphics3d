//// From gpu_opengl/pixmap.cpp by
////  camilo on 2025-07-29 07:57 <3ThomasBorregaardSørensen!!
//// From bred/gpu/pixmap.cpp by
////  camilo on 2025-07-01 14:39 <3ThomasBorregaardSørensen!!
#include "framework.h"
//#include "pixmap.h"
//
//
//namespace gpu_vulkan
//{
//
//
//   pixmap::pixmap()
//   {
//
//   }
//
//
//   pixmap::~pixmap()
//   {
//
//   }
//
//
//   void pixmap::initialize_gpu_pixmap(::gpu::renderer* pgpurenderer, const ::i32_size& m_size)
//   {
//
//      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//      // generate texture
//         //unsigned int texture;
//      glGenTextures(1, &m_gluTexture);
//      glBindTexture(GL_TEXTURE_2D, m_gluTexture); // You must bind before setting parameters
//
//      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//
//   }
//
//
//   void pixmap::set_pixels(const void* data, int w, int h)
//   {
//
//      glBindTexture(GL_TEXTURE_2D, m_gluTexture);
//
//      glTexImage2D(
//         GL_TEXTURE_2D,
//         0,
//         GL_BGRA,
//         w,
//         h,
//         0,
//         GL_BGRA,
//         GL_UNSIGNED_BYTE,
//         data
//      );
//      
//      glBindTexture(GL_TEXTURE_2D, 0);
//
//   }
//
//
//   void pixmap::bind_texture(::gpu::shader* pgpushader)
//   {
//
//      glBindTexture(GL_TEXTURE_2D, m_gluTexture);
//      ::opengl::check_error("");
//
//   }
//
//   
//   void pixmap::unbind_texture(::gpu::shader* pgpushader)
//   {
//
//      glBindTexture(GL_TEXTURE_2D, 0);
//      ::opengl::check_error("");
//
//   }
//
//
//   VkImageView pixmap::get_image_view()
//   {
//
//      if (m_vkimageview)
//      {
//
//         return m_vkimageview;
//
//      }
//
//      ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;
//      VkImageViewType viewType;
//      viewType = VK_IMAGE_VIEW_TYPE_2D;
//
//      unsigned int uLayerCount;
//         uLayerCount = 1;
//
//      VkImageViewCreateInfo viewInfo = {
//          .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
//          .image = m_vkimage,
//          .viewType = viewType,
//          .format = m_vkformat = pcontext->m_formatImageDefault,
//          .subresourceRange = {
//              .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
//              .baseMipLevel = 0,
//              .levelCount = 1,
//              .baseArrayLayer = 0,
//              .layerCount = uLayerCount
//          }
//      };
//
//      VkCheckResult(vkCreateImageView(pcontext->logicalDevice(), &viewInfo, NULL, &m_vkimageview));
//
//      return m_vkimageview;
//
//   }
//
//
//} // namespace gpu_vulkan
//
//

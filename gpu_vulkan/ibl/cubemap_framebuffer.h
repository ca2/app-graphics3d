// From github:/tristancalderbank/OpenGL-PBR-Renderer/cubemap_framebuffer.h by
// camilo on 2025-09-26 19:52 <3ThomasBorregaardSorensen!!
// From gpu_opengl/ibl/cubemap_framebuffer.h by
// camilo on 2025-10-10 22:15 <3ThomasBorregaardSorensen!!
#pragma once


#include "gpu/ibl/cubemap_framebuffer.h"


namespace gpu_vulkan
{


   namespace ibl
   {
      /**
       * Framebuffer for rendering to faces of a cubemap.
       */
      class cubemap_framebuffer :
         virtual public ::gpu::ibl::cubemap_framebuffer
      {
      public:


         //int m_iWidth, m_iHeight;
         //unsigned int m_uFramebufferId;
         //unsigned int m_uDepthRenderbufferId;
         //unsigned int m_uCubemapTextureId;

         struct render_pass_t
         {

            VkFramebuffer m_framebuffera[6] = {};

         };

         struct framebuffer_cube
         {

            VkFramebuffer m_framebuffera[6] = {};

         };

         map<VkRenderPass, framebuffer_cube> m_mapFramebufferCube;

         VkImageView       m_imageviewa[6] = {};

         map<::gpu_vulkan::render_pass *, render_pass_t> m_mapRenderPass;



         cubemap_framebuffer();


         ~cubemap_framebuffer() override;


         //virtual void initialize_cubemap_framebuffer(int width, int height);


         void on_initialize_cubemap_framebuffer() override;

         void bind(::gpu::command_buffer *pgpucommandbuffer) override;


         void generateMipmap();


         /**
          * Set which cube face texture to render to.
          * @param index
          */
         void setCubeFace(unsigned int index);

         VkFramebuffer framebuffer(::gpu_vulkan::render_pass *prenderpass, int iFace);
         VkFramebuffer _framebuffer(::gpu_vulkan::render_pass *prenderpass, int iFace);

         //unsigned int getCubemapTextureId();


      };


   } // namespace ibl


} // namespace gpu_vulkan

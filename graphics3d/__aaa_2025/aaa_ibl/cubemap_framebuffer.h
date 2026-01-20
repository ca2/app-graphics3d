// From github:/tristancalderbank/OpenGL-PBR-Renderer/cubemap_framebuffer.h by
// camilo on 2025-09-26 19:52 <3ThomasBorregaardSorensen!!
#pragma once


namespace gpu
{


   namespace ibl
   {
      /**
       * Framebuffer for rendering to faces of a cubemap.
       */
      class CLASS_DECL_GPU cubemap_framebuffer :
         virtual public ::particle
      {
      public:


         //int m_uWidth, m_uHeight;
         ::pointer < ::gpu::texture > m_ptexture;
         // unsigned int m_uFramebufferId;
         // unsigned int m_uDepthRenderbufferId;
         // unsigned int m_uCubemapTextureId;

         cubemap_framebuffer();


         ~cubemap_framebuffer() override;


         virtual void initialize_cubemap_framebuffer(int width, int height);


         virtual void on_initialize_cubemap_framebuffer();


         virtual void bind();


         virtual void generateMipmap();


         /**
          * Set which cube face texture to render to.
          * @param index
          */
         virtual void setCubeFace(unsigned int index);


         //virtual unsigned int getCubemapTextureId();


      };


   } // namespace ibl


} // namespace gpu

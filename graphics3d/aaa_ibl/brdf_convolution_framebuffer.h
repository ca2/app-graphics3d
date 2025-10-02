// From github:/tristancalderbank/OpenGL-PBR-Renderer/brdf_convolution_framebuffer.h by
// camilo on 2025-09-26 19:50 <3ThomasBorregaardSorensen!!
#pragma once


namespace gpu
{


   namespace ibl
   {

      /**
       * Framebuffer for rendering brdf convolution map.
       */
      class CLASS_DECL_GPU brdf_convolution_framebuffer
         :
         virtual public ::particle
      {
      public:


         //unsigned int m_uWidth, m_uHeight;

         ::pointer < ::gpu::texture > m_ptexture;

         // unsigned int m_uFramebufferId;
         // unsigned int m_uDepthRenderbufferId;
         // unsigned int m_uColorTextureId;

         brdf_convolution_framebuffer();


         ~brdf_convolution_framebuffer() override;


         virtual void initialize_BrdfConvolutionFramebuffer(unsigned int width, unsigned int height);


         virtual void on_initialize_BrdfConvolutionFramebuffer();


         /**
          * Activate this framebuffer for drawing.
          */
         virtual void bind();


         /**
          * Get the width.
          * @return
          */
         unsigned int getWidth();


         /**
          * Get the height.
          * @return
          */
         unsigned int getHeight();


         /**
          * Get the GL color texture id.
          * @return
          */
         //unsigned int getColorTextureId();

      };


   } // namespace ibl


} // namespace gpu

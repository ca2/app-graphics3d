#pragma once


#include "acme/_operating_system.h"
#include "gpu_directx12/context.h"
////#include "aura/os/_os.h"
/////#include "glad_egl.h"
//
//#include <EGL/egl.h>
//#include <EGL/eglext.h>


namespace directx12
{


   class CLASS_DECL_GPU_DIRECTX12 context_egl :
      virtual public ::directx12::context
   {
   public:


      EGLDisplay                       m_display;
      EGLConfig                        m_config;
      EGLContext                       m_context;
      EGLSurface                       m_surface;


      context_egl();
      ~context_egl() override;


      void create_context() override;


      void _create_offscreen_buffer(const ::int_size& size) override;
      void resize_offscreen_buffer(const ::int_size& size) override;
      void destroy_offscreen_buffer() override;

      void make_current() override;

      string get_shader_version_text() override;

      void _translate_shader(string_array& stra) override;


   };


} // namespace directx12




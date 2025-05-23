#pragma once


////#include "acme/prototype/prototype/object.h"


namespace gpu
{


   class CLASS_DECL_CUBE approach :
      virtual public ::object
   {
   public:

//
//#ifdef WINDOWS_DESKTOP
//
//      ATOM                          m_atomClass;
//
//#endif
//      bool                          m_bGlewInit;
//
//
//      //HGLRC                         m_hrc;
//      //HDC                           m_hdc;
//      //HWND                          m_hwnd;
//      //::int_size                        m_sizeNew;
//      //::int_size                        m_size;
//      //HDC                           m_hdcGraphics;
//      //float                         m_z;
//      //int_point                         m_pointTranslate;
//      //::pointer<::opengl::buffer>  m_pbuffer;
//      //::pointer<::opengl::program> m_pprogram;
//      //unsigned int                  VAO;
//      //unsigned int                  VBO;

      approach();
      ~approach() override;


      virtual void initialize(::particle * pparticle) override;


      virtual void initialize_gpu_approach();


      virtual ::file::path shader_path(const ::file::path& pathShader);


      virtual void defer_shader_memory(::memory & memory, const ::file::path& pathShader);


      virtual ::pointer < ::gpu::context > create_context(::particle * pparticle, ::gpu::enum_output eoutput);


      virtual void defer_init_gpu_library();

      //virtual void draw();
      //virtual void start();
      //virtual void render();

      //virtual bool create_offscreen_buffer(const ::int_size& size);
      //virtual bool destroy_offscreen_buffer();


      //virtual void make_current();


      virtual ::gpu::payload load_dds(const ::scoped_string & scopedstrImagePath);
      virtual void create_global_ubo(::gpu::context* pgpucontext, int iSize, int iFrameCount);
      virtual void update_global_ubo(::gpu::context* pgpucontext, int iFrameIndex, const ::block& block);


   };


} // namespace gpu





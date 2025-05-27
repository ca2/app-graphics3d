#pragma once


////#include "acme/exception/exception.h"
#include "acme/prototype/geometry2d/size.h"
#include "acme/prototype/prototype/memory.h"
#include "aura/graphics/draw3d/matrix.h"
#include "apex/parallelization/thread.h"
#include "app-cube/cube/gpu/shader.h"
#include "acme/graphics/image/image32.h"


namespace gpu
{


   class renderer;

   class image_data
   {
   public:


      ::memory       m_memory;

      int m_iWidth;
      int m_iHeight;

      int width() const { return m_iWidth; }
      int height() const { return m_iHeight; }
      const void * data() const { return m_memory.data(); }
      void * data() { return m_memory.data(); }

      image_data() :m_iWidth(0), m_iHeight(0) {}
      image_data(const image_data & imagedata) :
         m_iWidth(imagedata.m_iWidth),
         m_iHeight(imagedata.m_iHeight),
         m_memory(imagedata.m_memory)
      {


      }
      image_data(image_data && imagedata) :
         m_iWidth(imagedata.m_iWidth),
         m_iHeight(imagedata.m_iHeight),
         m_memory(::transfer(imagedata.m_memory))
      {


      }

   };


   class CLASS_DECL_CUBE context :
      virtual public ::thread
   {
   public:


      //bool m_bOffscreen = true;

      enum_output m_eoutput;


      ::pointer<::gpu::device>               m_pgpudevice;
      
      ::int_size                             m_sizeNew;
      ::int_size                             m_size;
      float                                  m_z;
      int_point                              m_pointTranslate;
      ::pointer<::gpu::cpu_buffer>           m_pcpubuffer;
      ::pointer<::gpu::shader>               m_pshader;
      bool                                   m_bCreated;
      ::draw3d::matrix                       m_matrixProjection;
      ::draw3d::matrix                       m_matrixImpact;
      ::pointer < ::gpu::renderer >          m_prenderer;
      ::pointer < ::graphics3d::engine >     m_pengine;
      ::pointer_array < ::gpu::render >      m_rendera;

      ::image32_callback                     m_callbackImage32CpuBuffer;

      //::pointer < ::mutex >                  m_pmutexOffscreen;
      //::int_size                             m_sizeOffscreen;
      //int                                    m_iScanOffscreen;
      //::memory                               m_memoryOffscreen;


      context();
      ~context() override;


      virtual void start_gpu_context(const start_context_t & startcontext);

      virtual void initialize_gpu_context(const start_context_t& startcontext);

      virtual void on_create_context(const start_context_t& startcontext);

      bool task_iteration() override;

      virtual void lock_context();
      virtual void unlock_context();

      //virtual void initialize(::particle * pparticle) override;


      virtual ::pointer<::gpu::shader> create_shader(
         const ::file::path& pathVert,
         const ::file::path& pathFrag,
         const ::array<::gpu::shader::enum_descriptor_set_slot>& eslota = {},
         const ::particle_pointer& pLocalDescriptorSet = {},
         const ::particle_pointer& pVertexInput = {},
         const ::gpu::property* pproperties = nullptr,
         ::gpu::shader::enum_flag eflag = ::gpu::shader::e_flag_none);


      virtual ::gpu::renderer* get_renderer();

      virtual ::int_size size();


      virtual string _001GetIntroProjection();
      virtual string _001GetIntroFragment();

      virtual bool is_mesa();

      inline const ::draw3d::matrix & view_matrix() const { return m_matrixImpact; }
      inline ::draw3d::matrix & view_matrix() { return m_matrixImpact; }
      inline const ::draw3d::matrix & projection_matrix() const { return m_matrixProjection; }
      inline ::draw3d::matrix & projection_matrix() { return m_matrixProjection; }

      virtual void start_drawing();
      virtual void global_transform();
      virtual void draw();
      virtual void render();


      virtual void set_bitmap_1(::image::image *pimage);

      virtual void swap_buffers();


      virtual void create_window_buffer(void * pHwnd);
      virtual void _create_window_buffer(void * pHwnd);
      virtual void create_offscreen_buffer(const ::int_size& size);
      virtual void _create_offscreen_buffer(const ::int_size& size);

      virtual void defer_create_window_context(::windowing::window * pwindow);
      virtual void _defer_create_window_context(::windowing::window * pwindow);

      virtual void resize_offscreen_buffer(const ::int_size& size);
      virtual void destroy_offscreen_buffer();
      
      virtual void prepare_for_gpu_read();

      virtual void translate_shader(string & str);

      virtual void _translate_shader(string_array& stra);

      virtual void make_current();

      virtual string load_fragment(const ::string & pszPath, enum_shader_source & eshadersource);

      virtual string get_shader_version_text();

      virtual void set_matrix_uniform(const ::gpu::payload & payloadMatrix);


      virtual class image_data image24(const ::payload & payloadFile);
      virtual class image_data image32(const ::payload & payloadFile);


      virtual void clear(const ::color::color& color);


      virtual void create_global_ubo(int iSize, int iFrameCount);
      virtual void update_global_ubo(const ::block& block);



   };

   class context_lock
   {
   public:
      
      context *      m_papplication;
      ::e_status     m_estatus;
      critical_section m_cs;
      
      context_lock(context * pgpucontext):
         m_estatus(error_failed),
         m_papplication(pgpucontext)
      {
         
         if(m_papplication)
         {

            m_cs.lock();
            try
            {

               m_papplication->lock_context();

               m_estatus = success;

            }
            catch (const ::exception& exception)
            {

               m_estatus = exception.m_estatus;

            }
            
         }
         
      }
      
      ~context_lock()
      {
         
         if(m_papplication)
         {
         
            if(m_estatus)
            {
            
               m_papplication->unlock_context();
               
            }
            m_cs.unlock();
         }
         
      }


   };



} // namespace gpu





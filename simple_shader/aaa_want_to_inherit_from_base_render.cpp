#include "framework.h"

#include <math.h>


void image_gl_set(image* pimage);


namespace simple_shader
{


   render::render()
   {

      m_bUpdateShader = false;

      m_strShaderPrefix = "default";

   }


   render::~render()
   {

   }


   ::e_status render::initialize(::particle * pparticle)
   {

      auto estatus = ::object::initialize(pparticle);

      if (!estatus)
      {

         return estatus;

      }

      initialize_application_consumer();

      m_papplication = psystem->get_gpu()->create_context();

      if (!m_papplication)
      {

         return ::error_failed;

      }

      estatus = m_papplication->initialize(this);

      if (!estatus)
      {

         return estatus;

      }

      return estatus;

   }


#ifdef _DEBUG


   long long render::increment_reference_count()
   {

      return ::object::increment_reference_count();

   }


   long long render::decrement_reference_count()
   {

      return ::object::decrement_reference_count();

   }


#endif


   void render::on_layout(::draw2d::graphics_pointer & pgraphics)
   {

      if (m_rect.area() <= 0)
      {

         return;

      }

      if (m_papplication)
      {

         m_papplication->resize_offscreen_buffer(m_rect.size());

      }
      
   }


   void render::defer_load_fragment(const ::string & pszPathPrefix)
   {

      if (string(pszPathPrefix) == "default")
      {

         m_strProjection.empty();

         m_strFragment.empty();

         defer_update_shader();

         return;

      }

      ::file::listing listing(get_context());

      listing.ls_pattern("dropbox://shader/simple shader/", { "*.frag" });

      for (auto& path : listing)
      {

         if (path.name().case_insensitive_begins(pszPathPrefix))
         {

            m_strFragment = m_papplication->load_fragment(path, m_eshadersource);

            m_bUpdateShader = true;

            break;

         }

      }

   }


   ::e_status render::update_shader()
   {

      m_bUpdateShader = true;

      return ::success;

   }


   void render::defer_update_shader()
   {

      if (m_bUpdateShader)
      {

         m_bUpdateShader = false;

         _update_shader();

      }

   }


   ::e_status render::_update_shader()
   {

      ::e_status estatus = ::success_none;

      if(m_strProjection.is_empty())
      {

         m_strProjection = m_papplication->_001GetIntroProjection();

      }

      if(m_strFragment.is_empty())
      {

         m_strFragment = m_papplication->_001GetIntroFragment();

      }

      color32_t crText = argb(255, 55, 210, 120);

      string strProjection = m_strProjection;

      string strFragment = m_strFragment;

      ::gpu::context_lock lock(m_papplication);

      m_papplication->make_current();

      if (!m_pprogram)
      {

         estatus = __øconstruct(m_pprogram);

      }

      m_papplication->translate_shader(strProjection);

      m_papplication->translate_shader(strFragment);

      if (::succeeded(estatus))
      {

         estatus = m_pprogram->create_program(strProjection, strFragment);

      }

      ::color::color color(crText);

      string strDataId;

      strDataId = m_pimpact->id();

      m_papplication->m_pprogram = m_pprogram;

      m_papplication->draw();

      //unsigned int texture1;

      //glGenTextures(1, &texture1);

      ////            glActiveTexture(GL_TEXTURE0);

      //glBindTexture(GL_TEXTURE_2D, texture1);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      m_durationStart.Now();

      return ::success;

   }


   void render::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {
      
      ::gpu::context_lock lock(m_papplication);

      if (m_strShaderPrefix.has_character())
      {

         defer_load_fragment(m_strShaderPrefix);

         m_strShaderPrefix.empty();

      }

      defer_update_shader();

      if (m_papplication &&
        ::is_set(m_papplication->m_pprogram) &&
       m_papplication->m_pbuffer && ::is_ok(m_papplication->m_pbuffer->m_pimage))
      {
         
         {

            ::gpu::context_lock lock(m_papplication);
            
            m_papplication->make_current();

            m_papplication->start();

            {

               float x = (float) psession->get_cursor_position().x();

               float y = (float) psession->get_cursor_position().y();

               m_papplication->m_pprogram->m_pshader->setVec2("mouse", x, y);
               m_papplication->m_pprogram->m_pshader->setVec2("iMouse", x, y);

            }

            {

               float cx = (float) m_papplication->m_pbuffer->m_pimage->width();

               float cy = (float) m_papplication->m_pbuffer->m_pimage->height();

               m_papplication->m_pprogram->m_pshader->setVec2("resolution", cx, cy);
               m_papplication->m_pprogram->m_pshader->setVec2("iResolution", cx, cy);

            }

            {

               double dElapsed = __double(m_durationStart.elapsed());

               double dTime = dElapsed / 1000.0;

               float time = (float) dTime;

               m_papplication->m_pprogram->m_pshader->setFloat("time", time);
               m_papplication->m_pprogram->m_pshader->setFloat("iTime", time);

            }

            m_papplication->render();

            m_papplication->prepare_for_gpu_read();

            m_papplication->m_pbuffer->gpu_read();
            
         }
      
         
#if !defined(__APPLE__)
         ::geometry2d::matrix matrixOriginal;
         pgraphics->get(matrixOriginal);
         ::geometry2d::matrix matrix(matrixOriginal);
         matrix.scale(1.0, -1.0);
         matrix.translate(0, m_rect.height());
         pgraphics->set(matrix);
#endif

         pgraphics->draw(m_rect, m_papplication->m_pbuffer->m_pimage);

#if !defined(__APPLE__)
         pgraphics->set(matrixOriginal);
#endif
      }

      //_001OnDraw1Through3(pgraphics);

   }


   ::e_status render::set_font(const ::string & strFont)
   {

      if (strFont.is_empty())
      {

         return error_bad_argument;

      }

      string strDataId;

      strDataId = m_pimpact->id();

      if (!papp->datastream()->set(strDataId + ".font_family", m_strFont1))
      {

         return error_failed;

      }

      m_strFont1 = strFont;

      return ::success;

   }


   string render::get_font()
   {

      string strFont;

      strFont = papp->get_hover_font();

      if (strFont.has_character())
      {

         return strFont;

      }

      if (m_strFont1.is_empty())
      {

         string strDataId;

         strDataId = m_pimpact->id();

         if (!papp->datastream()->get(strDataId + ".font_family", m_strFont1)
            || m_strFont1.is_empty())
         {

            m_strFont1 = pnode->font_name(e_font_sans_ex);

         }

      }

      return m_strFont1;

   }


   string application::get_main_title() const
   {

      string strSimple(m_strSimple);

      if (strSimple.trimmed().has_character())
      {

         return m_strSimple;

      }
      else
      {

         return m_strMainTitle;

      }

   }


} // namespace simple_shader




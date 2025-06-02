#include "framework.h"
#include "render.h"
#include "application.h"
#include "acme/prototype/mathematics/trigonometry.h"
#include "aura/graphics/gpu/context.h"
#include "aura/graphics/gpu/program.h"
#include "aura/graphics/gpu/shader.h"
//#include "aura/graphics/draw2d.h"
#define GLAD_GLAPI_EXPORT
#include <glad.h>
#include <math.h>


CLASS_DECL_ACME ::color::color dk_red(); // <3 tbs

//#include "this_part_of_code"

namespace graphics3d_hello3d
{


   void render::on_draw_triangle(gpu::context * pgpucontext)
   {

      if (!m_pgpuprogram)
      {

         __defer_construct(m_pgpuprogram);

         ::string strVertex = R"====(
#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)====";

         ::string strFragment = R"====(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
)====";

         m_pgpuprogram->create_program(this, strVertex, strFragment);

         glGenVertexArrays(1, &m_VAO);

         glBindVertexArray(m_VAO);

         glGenBuffers(1, &m_VBO);

         glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
         if (m_iRender % 2 == 1)
         {
            float vertices[] = {
          -0.5f, -0.5f, 0.0f,
           0.5f, -0.5f, 0.0f,
           0.0f,  0.5f, 0.0f,
          0.25f, 0.5f, 0.0f,
           0.5f, 0.5f, 0.0f,
           0.375f,  0.25f, 0.0f
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

         }
         else
         {

            float vertices[] = {
          -0.5f, -0.5f, 0.0f,
           0.5f, -0.5f, 0.0f,
           0.0f,  0.5f, 0.0f
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

         }


         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
         glEnableVertexAttribArray(0);

      }



      m_pgpuprogram->m_pshader->use();
      glBindVertexArray(m_VAO);

      if (m_iRender % 2 == 1)
      {
         glDrawArrays(GL_TRIANGLES, 0, 6);

      }
      else
      {
         glDrawArrays(GL_TRIANGLES, 0, 3);

      }


   }
   //void render::_001OnDraw1Through3(::draw2d::graphics_pointer & pgraphics)
   //{

   //   //string strFontFamily = get_font();

   //   //auto ppen = __øcreate < ::draw2d::pen > ();

   //   //auto pbrush = __øcreate < ::draw2d::brush >();

   //   //auto pfont = __øcreate < ::write_text::font > ();

   //   //if (m_iDrawing == 3)
   //   //{

   //   //   if (!m_pimage1)
   //   //   {

   //   //      m_pimage1.create();

   //   //      fork([this]()
   //   //         {

   //   //            m_pimage1 = get_image("matter://pat1.jpg");

   //   //            if (m_pimage1.ok())
   //   //            {

   //   //               __øconstruct(m_pimage2);

   //   //               m_pimage2->copy_from(m_pimage1);

   //   //               if (m_pimage2.ok())
   //   //               {

   //   //                  m_pimage2->transform(e_image_grayscale);

   //   //                  m_pimpact->set_need_redraw();

   //   //                  m_pimpact->post_redraw();

   //   //               }

   //   //            }

   //   //         });

   //   //   }


   //   //}

   //   //int_rectangle rectangle;

   //   //int iSize = minimum(m_rectangle.width(), m_rectangle.height());

   //   //iSize = iSize * 3 / 4;

   //   //rectangle.set_size(iSize, iSize);

   //   //rectangle.Align(e_align_center, m_rectangle);

   //   //rectangle.offset_x(-iSize / 5 * 3);

   //   //rectangle.offset_x(iSize / 5 * m_iDrawing);
   //   //
   //   //::double_size size(0., 0.);
   //   //
   //   //bool bDrawText = true;
   //   //
   //   //string strTitle;
   //   //
   //   //if(bDrawText)
   //   //{

   //   //   pfont->create_pixel_font(strFontFamily, 100.0, 800);

   //   //   pgraphics->selectFont(font);

   //   //   strTitle = papp->get_main_title();

   //   //   size = pgraphics->get_text_extent(strTitle);

   //   //   if (!size.is_empty())
   //   //   {

   //   //      if (m_iDrawing == 1)
   //   //      {

   //   //         pfont->create_pixel_font(strFontFamily, rectangle.height() * 80.0 / size.get_maximum_dimension(), 800);

   //   //      }
   //   //      else
   //   //      {

   //   //         pfont->create_pixel_font(strFontFamily, rectangle.height() * 160.0 / size.get_maximum_dimension(), 800);

   //   //      }

   //   //   }

   //   //   pgraphics->selectFont(font);

   //   //   size = pgraphics->get_text_extent(strTitle);
   //   //      
   //   //}

   //   //auto& echeckSimple = papp->m_echeckSimple;

   //   //if (__bool(echeckSimple))
   //   //{

   //   //   ppen->create_null();

   //   //}
   //   //else
   //   //{

   //   //   ppen->create_solid(4.0, argb(255, 50, 180, 255));

   //   //}

   //   //if (m_iDrawing == 3 && m_pimage1.ok())
   //   //{

   //   //   pbrush->CreatePatternBrush(m_pimage1);

   //   //}
   //   //else
   //   //{

   //   //   if (__bool(papp->m_echeckSimple))
   //   //   {

   //   //      pbrush->create_solid(argb(255, 255, 255, 200));

   //   //   }
   //   //   else
   //   //   {

   //   //      pbrush->CreateLinearGradientBrush(rectangle.top_left(), rectangle.bottom_right(), argb(255, 255, 255, 200), argb(255, 255, 125, 100));

   //   //   }

   //   //}

   //   //pgraphics->SelectObject(pen);

   //   //pgraphics->SelectObject(brush);

   //   //pgraphics->ellipse(rectangle);

   //   //::int_rectangle rectangleText;

   //   //rectangleText.set_size(size);

   //   //rectangleText.inflate(10, 10);

   //   //rectangleText.Align(e_align_center, rectangle);

   //   //pgraphics->set_alpha_mode(::draw2d::e_alpha_mode_blend);

   //   //if (m_iDrawing == 1)
   //   //{

   //   //   pbrush->create_solid(m_hlsText);

   //   //}
   //   //else if (m_iDrawing == 3)
   //   //{

   //   //   if (__bool(papp->m_echeckSimple))
   //   //   {

   //   //      pbrush->create_solid(m_hlsText);

   //   //   }
   //   //   else
   //   //   {

   //   //      if (m_pimage2.ok())
   //   //      {

   //   //         pbrush->CreatePatternBrush(m_pimage2);

   //   //      }
   //   //      else
   //   //      {

   //   //         pbrush->create_solid(m_hlsText);

   //   //      }

   //   //   }

   //   //}
   //   //else
   //   //{

   //   //   if (__bool(papp->m_echeckSimple))
   //   //   {

   //   //      pbrush->create_solid(m_hlsText);

   //   //   }
   //   //   else
   //   //   {

   //   //      pbrush->CreateLinearGradientBrush(rectangleText.top_left(), rectangleText.bottom_right(), m_hlsText, argb(255, 255, 255, 200));

   //   //   }

   //   //}

   //   //pgraphics->SelectObject(brush);

   //   //if(bDrawText)
   //   //{
   //   //
   //   //   pgraphics->draw_text(strTitle, rectangleText, e_align_center);
   //   //   
   //   //}

   //}


   void render::on_draw_triangle2(gpu::context * pgpucontext)
   {

      if (!m_pgpuprogram)
      {

         __defer_construct(m_pgpuprogram);

         ::string strVertex = R"====(
#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)====";

         ::string strFragment = R"====(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
)====";

         m_pgpuprogram->create_program(this, strVertex, strFragment);

      }

      if (!m_pprogram2)
      {

         __defer_construct(m_pprogram2);

         ::string strVertex = R"====(
#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)====";

         ::string strFragment = R"====(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);
} 
)====";

         m_pprogram2->create_program(this, strVertex, strFragment);

         {

            glGenVertexArrays(1, &m_VAO);

            glBindVertexArray(m_VAO);

            glGenBuffers(1, &m_VBO);

            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f,
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(0);


         }
         {

            glGenVertexArrays(1, &m_VAO2);

            glBindVertexArray(m_VAO2);

            glGenBuffers(1, &m_VBO2);

            glBindBuffer(GL_ARRAY_BUFFER, m_VBO2);
            float vertices[] = {
         0.25f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            0.375f, 0.25f, 0.0f
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(0);


         }

      }

      m_pgpuprogram->m_pshader->use();
      glBindVertexArray(m_VAO);

      glDrawArrays(GL_TRIANGLES, 0, 3);


      m_pprogram2->m_pshader->use();
      glBindVertexArray(m_VAO2);

      glDrawArrays(GL_TRIANGLES, 0, 3);


   }


   void render::on_draw_triangle_uniform(gpu::context * pgpucontext)
   {

      if (!m_pgpuprogram)
      {

         __defer_construct(m_pgpuprogram);

         ::string strVertex = R"====(
#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)====";

         ::string strFragment = R"====(
#version 330 core
out vec4 FragColor;
uniform vec4 ourColor; // we set this variable in the OpenGL code.
void main()
{
    FragColor = ourColor;
} 
)====";

         m_pgpuprogram->create_program(this, strVertex, strFragment);

         glGenVertexArrays(1, &m_VAO);

         glBindVertexArray(m_VAO);

         glGenBuffers(1, &m_VBO);

         glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
         if (m_iRender % 2 == 1)
         {
            float vertices[] = {
          -0.5f, -0.5f, 0.0f,
           0.5f, -0.5f, 0.0f,
           0.0f,  0.5f, 0.0f,
          0.25f, 0.5f, 0.0f,
           0.5f, 0.5f, 0.0f,
           0.375f,  0.25f, 0.0f
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

         }
         else
         {

            float vertices[] = {
          -0.5f, -0.5f, 0.0f,
           0.5f, -0.5f, 0.0f,
           0.0f,  0.5f, 0.0f
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

         }


         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
         glEnableVertexAttribArray(0);

      }

      float greenValue = ((float)::trigonometry::sin(double_second()) / 2.0f) + 0.5f;
      // int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
      m_pgpuprogram->m_pshader->use();
      m_pgpuprogram->m_pshader->setVec4("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
         // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
      glBindVertexArray(m_VAO);

      if (m_iRender % 2 == 1)
      {
         glDrawArrays(GL_TRIANGLES, 0, 6);

      }
      else
      {
         glDrawArrays(GL_TRIANGLES, 0, 3);

      }


   }


   void render::on_draw_triangle_more_attributes(gpu::context * pgpucontext)
   {

      if (!m_pgpuprogram)
      {

         __defer_construct(m_pgpuprogram);

         ::string strVertex = R"====(
#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
  
out vec3 ourColor; // output a color to the fragment shader

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor; // set ourColor to the input color we got from the vertex data
}       
)====";

         ::string strFragment = R"====(
#version 330 core
out vec4 FragColor;  
in vec3 ourColor;
  
void main()
{
    FragColor = vec4(ourColor, 1.0);
}
)====";

         m_pgpuprogram->create_program(this, strVertex, strFragment);

         glGenVertexArrays(1, &m_VAO);

         glBindVertexArray(m_VAO);

         glGenBuffers(1, &m_VBO);

         glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
         float vertices[] = {
            // positions         // colors
             0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
             0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
         };
         glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
         glEnableVertexAttribArray(0);
         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
         glEnableVertexAttribArray(1);


      }


      m_pgpuprogram->m_pshader->use();
      glBindVertexArray(m_VAO);

      glDrawArrays(GL_TRIANGLES, 0, 3);


   }


} // namespace simple_shader




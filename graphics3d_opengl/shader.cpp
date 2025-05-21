#include "framework.h"
#include "context.h"
#include "glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "acme/filesystem/filesystem/file_context.h"
#include "shader.h"
#include "app-cube/cube/impact.h"
#include <fstream>
#include <sstream>
#include <iostream>


namespace graphics3d_opengl
{

   // Constructor to load and compile shaders
   shader::shader()
   {



   }
   
   
   void shader::on_initialize_shader()
   {
      //initialize(pparticle);
      // Load and compile vertex and fragment shaders
      auto strVert = file()->as_string(m_pcontext->m_pimpact->m_pengine->_translate_shader_path(m_pathVert));
      auto strFrag = file()->as_string(m_pcontext->m_pimpact->m_pengine->_translate_shader_path(m_pathFrag));

      GLuint vertexShader = compile_shader(GL_VERTEX_SHADER, strVert);
      GLuint fragmentShader = compile_shader(GL_FRAGMENT_SHADER, strFrag);

      // Link shaders into a shader program
      link_shaders(vertexShader, fragmentShader);

      // Clean up shaders (they are no longer needed once linked into a program)
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);
   }

   // Destructor to clean up shader program
   shader::~shader()
   {
      glDeleteProgram(m_ProgramID);
   }

   // Bind the shader program
   void shader::bind() const
   {
      glUseProgram(m_ProgramID);
   }

   // Unbind the shader program
   void shader::unbind() const
   {
      glUseProgram(0);
   }
   void shader::_set_int(const char * name, int i) const 
   {
      //Bind();  // Ensure the shader program is bound
      GLint location = glGetUniformLocation(m_ProgramID, name);
      if (location == -1) {
         std::cerr << "Uniform " << name << " not found!" << std::endl;
         return;
      }
      glUniform1i(location, i);
   } 
   void shader::_set_float(const char * name, float value) const
   {
      //Bind();  // Ensure the shader program is bound
      GLint location = glGetUniformLocation(m_ProgramID, name);
      if (location == -1) {
         std::cerr << "Uniform " << name << " not found!" << std::endl;
         return;
      }
      glUniform1f(location, value);
   }
   void shader::_set_vec2(const char* name, const glm::vec2& value) const {
      //Bind();  // Ensure the shader program is bound
      GLint location = glGetUniformLocation(m_ProgramID, name);
      if (location == -1) {
         std::cerr << "Uniform " << name << " not found!" << std::endl;
         return;
      }
      glUniform2f(location, value.x, value.y);
   }
   void shader::_set_vec3(const char * name, const glm::vec3& value) const {
      //Bind();  // Ensure the shader program is bound
      GLint location = glGetUniformLocation(m_ProgramID, name);
      if (location == -1) {
         std::cerr << "Uniform " << name << " not found!" << std::endl;
         return;
      }
      glUniform3f(location, value.x, value.y, value.z);
   }
   void shader::_set_vec4(const char* name, const glm::vec4& value) const {
      //Bind();  // Ensure the shader program is bound
      GLint location = glGetUniformLocation(m_ProgramID, name);
      if (location == -1) {
         std::cerr << "Uniform " << name << " not found!" << std::endl;
         return;
      }
      glUniform4f(location, value.x, value.y, value.z, value.w);
   }

   void shader::_set_mat2(const char* name, const glm::mat2& matrix) const
   {
      GLint location = glGetUniformLocation(m_ProgramID, name);
      if (location == -1) {
         std::cerr << "Uniform " << name << " not found!" << std::endl;
         return;
      }
      glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
   }


   void shader::_set_mat3(const char *  name, const glm::mat3& matrix) const
   {
      GLint location = glGetUniformLocation(m_ProgramID, name);
      if (location == -1) {
         std::cerr << "Uniform " << name << " not found!" << std::endl;
         return;
      }
      glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
   }

   void shader::_set_mat4(const char * name, const glm::mat4& matrix) const
   {
      GLint location = glGetUniformLocation(m_ProgramID, name);
      if (location == -1) {
         std::cerr << "Uniform " << name << " not found!" << std::endl;
         return;
      }
      glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
   }


   void shader::push_properties()
   {

      auto p = m_properties.m_pproperties;
      int iLen = 0;

      while (true)
      {

         if (::is_null(p->m_pszName))
         {

            break;

         }

         switch (p->m_etype)
         {
         case ::gpu::e_type_int:
            _set_int(p->m_pszName, *(int*)(m_properties.data() + iLen));
            break;
         case ::gpu::e_type_float:
            _set_float(p->m_pszName, *(float*)(m_properties.data() + iLen));
            break;
         case ::gpu::e_type_seq2:
            _set_vec2(p->m_pszName, *(glm::vec2*)(m_properties.data() + iLen));
            break;
         case ::gpu::e_type_seq3:
            _set_vec3(p->m_pszName, *(glm::vec3*)(m_properties.data() + iLen));
            break;
         case ::gpu::e_type_seq4:
            _set_vec4(p->m_pszName, *(glm::vec4*)(m_properties.data() + iLen));
            break;
         case ::gpu::e_type_mat2:
            _set_mat2(p->m_pszName, *(glm::mat2*)(m_properties.data() + iLen));
            break;
         case ::gpu::e_type_mat3:
            _set_mat3(p->m_pszName, *(glm::mat3*)(m_properties.data() + iLen));
            break;
         case ::gpu::e_type_mat4:
            _set_mat4(p->m_pszName, *(glm::mat4*)(m_properties.data() + iLen));
               break;

         }
         
         auto iLenItem = ::gpu::get_type_size(p->m_etype);

         iLen += iLenItem;

         p++;

      }




   }

   //// Read shader source code from file
   //std::string shader::ParseShader(const std::string& filePath)
   //{
   //   //std::ifstream file(filePath);
   //   //std::stringstream stream;

   //   //if (file.is_open())
   //   //{
   //     // stream << file.rdbuf();
   //      //file.close();
   //   //}
   //   //else
   //   //{
   //     // std::cerr << "Failed to open shader file: " << filePath << std::endl;
   //   //}
   //   return file()->as_string(filePath.c_str()).c_str();
   //   //return //stream.str();
   //}

   // Compile a shader of a given type
   GLuint shader::compile_shader(GLenum type, const ::scoped_string & scopedstrSource)
   {
      GLuint shaderID = glCreateShader(type);
      auto sourceCStr = scopedstrSource.begin();
      GLint i = scopedstrSource.size();
      glShaderSource(shaderID, 1, &sourceCStr, &i);
      glCompileShader(shaderID);

      // Check for compilation errors
      GLint success;
      glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
      if (!success)
      {
         GLint logSize;
         glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logSize);
         std::string log(logSize, '\0');
         glGetShaderInfoLog(shaderID, logSize, &logSize, &log[0]);
         std::cerr << "shader compilation failed:\n" << log << std::endl;
      }

      return shaderID;
   }

   // Link vertex and fragment shaders into a shader program
   void shader::link_shaders(GLuint vertexShader, GLuint fragmentShader)
   {
      m_ProgramID = glCreateProgram();
      glAttachShader(m_ProgramID, vertexShader);
      glAttachShader(m_ProgramID, fragmentShader);
      glLinkProgram(m_ProgramID);

      // Check for linking errors
      GLint success;
      glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
      if (!success)
      {
         GLint logSize;
         glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &logSize);
         std::string log(logSize, '\0');
         glGetProgramInfoLog(m_ProgramID, logSize, &logSize, &log[0]);
         std::cerr << "Program linking failed:\n" << log << std::endl;
      }
   }

   void shader::draw()
   {

      glDrawArrays(GL_TRIANGLES, 0, 6);
  }



} // namespace graphics3d_opengl



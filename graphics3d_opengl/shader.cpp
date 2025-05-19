#include "framework.h"
#include "glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "acme/filesystem/filesystem/file_context.h"
#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>


namespace graphics3d_opengl
{

   // Constructor to load and compile shaders
   shader::shader(::particle * pparticle, const std::string& vertexPath, const std::string& fragmentPath)
   {
      initialize(pparticle);
      // Load and compile vertex and fragment shaders
      std::string vertexCode = ParseShader(vertexPath);
      std::string fragmentCode = ParseShader(fragmentPath);

      GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexCode);
      GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentCode);

      // Link shaders into a shader program
      LinkShaders(vertexShader, fragmentShader);

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
   void shader::Bind() const
   {
      glUseProgram(m_ProgramID);
   }

   // Unbind the shader program
   void shader::Unbind() const
   {
      glUseProgram(0);
   }

   void shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) const {
      GLint location = glGetUniformLocation(m_ProgramID, name.c_str());
      if (location == -1) {
         std::cerr << "Uniform " << name << " not found!" << std::endl;
         return;
      }
      glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
   }


   // Read shader source code from file
   std::string shader::ParseShader(const std::string& filePath)
   {
      //std::ifstream file(filePath);
      //std::stringstream stream;

      //if (file.is_open())
      //{
        // stream << file.rdbuf();
         //file.close();
      //}
      //else
      //{
        // std::cerr << "Failed to open shader file: " << filePath << std::endl;
      //}
      return file()->as_string(filePath.c_str()).c_str();
      //return //stream.str();
   }

   // Compile a shader of a given type
   GLuint shader::CompileShader(GLenum type, const std::string& source)
   {
      GLuint shaderID = glCreateShader(type);
      const char* sourceCStr = source.c_str();
      glShaderSource(shaderID, 1, &sourceCStr, nullptr);
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
   void shader::LinkShaders(GLuint vertexShader, GLuint fragmentShader)
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

   void shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) const {
      Bind();  // Ensure the shader program is bound
      GLint location = glGetUniformLocation(m_ProgramID, name.c_str());

      glUniform3f(location, v0, v1, v2);
   }
   void shader::setVec3(const std::string& name, const glm::vec3& value) const {
      Bind();  // Ensure the shader program is bound
      GLint location = glGetUniformLocation(m_ProgramID, name.c_str());
      if (location == -1) {
         std::cerr << "Uniform " << name << " not found!" << std::endl;
         return;
      }
      glUniform3f(location, value.x, value.y, value.z);
   }
   void shader::setFloat(const std::string& name, float value) const {
      Bind();  // Ensure the shader program is bound
      GLint location = glGetUniformLocation(m_ProgramID, name.c_str());
      if (location == -1) {
         std::cerr << "Uniform " << name << " not found!" << std::endl;
         return;
      }
      glUniform1f(location, value);
   }



} // namespace graphics3d_opengl



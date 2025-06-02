#pragma once
//
//
//#include "cube/gpu/shader.h"
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
////#include "GLFW/glfw3.h"
//#include <string>
//
//
//namespace graphics3d_opengl
//{
//
//   class shader :
//      virtual public ::graphics3d::shader
//   {
//   public:
//
//
//      //shader(::particle * pparticle, const std::string& vertexPath, const std::string& fragmentPath);
//      shader();
//      ~shader();
//
//
//      void on_initialize_shader() override;
//
//      void bind() const override;
//      void unbind() const override;
//
// 
//
//      GLuint GetProgramID() const { return m_ProgramID; }
//
//   //private:
//      GLuint m_ProgramID;
//      //std::string ParseShader(const std::string& filePath);
//      GLuint compile_shader(GLenum type, const ::scoped_string & scopedstrSource);
//      void link_shaders(GLuint vertexShader, GLuint fragmentShader);
//
//      void _set_int(const char* name, int value) const;
//      void _set_float(const char* name, float value) const;
//      void _set_vec2(const char* name, const glm::vec2& value) const;
//      void _set_vec3(const char* name, const glm::vec3& value) const;
//      void _set_vec4(const char* name, const glm::vec4& value) const;
//      void _set_mat2(const char* name, const glm::mat2& matrix) const;
//      void _set_mat3(const char* name, const glm::mat3& matrix) const;
//      void _set_mat4(const char* name, const glm::mat4& matrix) const;
//
//      void push_properties() override;
//
//      void draw() override;
//
//
//   };
//
//
//
//} // namespace graphics3d_opengl
//
//
//
//

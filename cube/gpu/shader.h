#pragma once


#include "app-cube/cube/gpu/properties.h"


namespace gpu
{

   enum enum_shader
   {

      shader_fragment,
      shader_projection,

   };

   class CLASS_DECL_CUBE shader :
      virtual public ::matter
   {
   public:
      
      enum enum_flag
      {
         e_flag_none = 0,
         e_flag_clear_default_bindings_and_attributes_descriptions = 1,

      };


      //unsigned int               m_uId;

      string                     m_strError;

      //string_map < payload >     m_mapLayout;

      ::pointer < context >      m_pgpucontext;

      ::file::path               m_pathVertex;
      ::file::path               m_pathFragment;
      ::file::path               m_pathGeometry;

      memory                     m_memoryVertex;
      memory                     m_memoryFragment;
      memory                     m_memoryGeometry;

      ::gpu::properties          m_properties;

      enum_flag                  m_eflag;

      shader();
      ~shader() override;

      
      virtual void initialize_shader(
         ::gpu::context* pgpucontext,
         const ::file::path & pathVertex, 
         const ::file::path & pathFragment, 
         const ::gpu::property* pproperties = nullptr,
         enum_flag eflag = e_flag_none);

      virtual void initialize_shader_with_block(
         ::gpu::context* pgpucontext,
         const ::block & blockVertex, 
         const ::block & blockFragment,
         const ::gpu::property* pproperties = nullptr,
         enum_flag eflag = e_flag_none);

      virtual void on_initialize_shader();

      //virtual void use();

      //virtual void setBool(const ::scoped_string & scopedstrName, bool value);

      //virtual void setInt(const ::scoped_string & scopedstrName, int value);

      //virtual void setFloat(const ::scoped_string & scopedstrName, float value);

      ////virtual void setVec2(const ::scoped_string & scopedstrName, const glm::vec2& value);
      //virtual void setVec2(const ::scoped_string & scopedstrName, float x, float y);

      ////virtual void setVec3(const ::scoped_string & scopedstrName, const glm::vec3& value);
      //virtual void setVec3(const ::scoped_string & scopedstrName, float x, float y, float z);

      ////virtual void setVec4(const ::scoped_string & scopedstrName, const glm::vec4& value);
      //virtual void setVec4(const ::scoped_string & scopedstrName, float x, float y, float z, float w);

      //virtual void setMat2(const ::scoped_string & scopedstrName, const float p[2*2]);
      //virtual void setMat3(const ::scoped_string & scopedstrName, const float p[3*3]);
      //virtual void setMat4(const ::scoped_string & scopedstrName, const float p[4*4]);

      //virtual shader * create_shader(const ::string & pszVertex, enum_shader eshader);

      //static void shader_compile_errors(GLuint shader, enum_shader eshader, string & strSummary);

      //static void program_compile_errors(GLuint program, string & strSummary);


      //virtual payload * get_uniform(const ::string & strName);

      //virtual ::gpu::payload * get_payload(const ::scoped_string & scopedstrName);
      virtual void draw();

      //virtual void on_initialize_shader();


      virtual void bind();
      virtual void unbind();


      virtual void push_properties();


      virtual void set_bool(const ::scoped_string& scopedstrName, bool value);

      virtual void set_int(const ::scoped_string& scopedstrName, int value);

      virtual void set_float(const ::scoped_string& scopedstrName, float value);

      virtual void set_vec2(const ::scoped_string& scopedstrName, float x, float y);
      virtual void set_vec2(const ::scoped_string& scopedstrName, const ::glm::vec2& a);

      virtual void set_vec3(const ::scoped_string& scopedstrName, float x, float y, float z);
      virtual void set_vec3(const ::scoped_string& scopedstrName, const ::glm::vec3& a);

      virtual void set_vec4(const ::scoped_string& scopedstrName, float x, float y, float z, float w);
      virtual void set_vec4(const ::scoped_string& scopedstrName, const ::glm::vec4& a);

      virtual void set_mat2(const ::scoped_string& scopedstrName, const ::glm::mat2& a);
      virtual void set_mat3(const ::scoped_string& scopedstrName, const ::glm::mat3& a);
      virtual void set_mat4(const ::scoped_string& scopedstrName, const ::glm::mat4& a);


   };


} // namespace gpu





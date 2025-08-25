// Co-creating with V0idsEmbrace@Twitch with
// camilo on 2025-05-19 04:59 <3ThomasBorregaardSorensen!!
#pragma once


#include "bred/gpu/shader.h"


namespace gpu_vulkan
{

   
   //class shader_texture;


   //class shader_sampler :
   //   virtual public ::particle
   //{
   //public:

   //   shader* m_pshader;

   //   //VkImageView srcView, dstView;
   //   VkSampler m_vksampler;
   //   unsigned int m_uSet;
   //   VkRenderPass m_vkrenderpass2;
   //   VkDescriptorSetLayout m_descriptorsetlayout;
   //   //VkPipelineLayout pipelineLayout;
   //   // VkPipeline pipeline;
   //   ///VkDescriptorPool descPool;
   //   ///VkDescriptorSet descSet;
   //   //::pointer<::gpu::shader>                        m_pshader;
   //   ::pointer<::gpu_vulkan::descriptor_set_layout>  m_psetdescriptorlayout;
   //   ::pointer <::gpu_vulkan::descriptor_pool>       m_pdescriptorpool;

   //   map < VkImage, ::pointer<shader_texture> >      m_shadertexturemap;

   //   //::map < VkImage, image > m_mapImage;

   //   shader_sampler();
   //   ~shader_sampler() override;

   //   
   //   VkRenderPass get_render_pass();

   //};


   //class shader_texture :
   //   virtual public ::particle
   //{
   //public:


   //   shader_sampler * m_pshadersampler;

   //   ::pointer < texture > m_ptexture;

   //   VkImageView       m_vkimageview;
   //   map < VkRenderPass, VkFramebuffer >    m_mapVkFramebuffer2;
   //   VkDescriptorSet   m_vkdescriptorset;
   //   //VkPipelineLayout		      m_vkpipelinelayout = nullptr;

   //   shader_texture();
   //   ~shader_texture() override;

   //   void initialize_shader_texture(shader_sampler* pshadersampler, texture* ptexture);

   //   VkImageView get_image_view();
   //   VkFramebuffer get_framebuffer(::gpu_vulkan::render_pass * prenderpass);

   //};

   class CLASS_DECL_GPU_VULKAN shader :
      virtual public ::gpu::shader
   {
   public:


      ::pointer < pipeline > m_ppipeline;



      //bool m_bClearColor;
      //::color::color m_colorClear;

      int m_iColorAttachmentCount = 1;
      //VkPrimitiveTopology m_vktopology;

      ::comparable_array<VkDynamicState> m_dynamicstateaEnable;

      //VkDescriptorSet   m_vkdescriptorset;
      ::pointer<::gpu_vulkan::descriptor_set_layout>  m_psetdescriptorlayout;
      ::pointer <::gpu_vulkan::descriptor_pool>       m_pdescriptorpool;



      //::pointer < shader_sampler >     m_pshadersampler;
      ::pointer < ::gpu_vulkan::shader > m_pshaderPresent;
      VkRenderPass            m_vkrenderpassCurrent;


      shader();
      ~shader();


      bool need_rebuild() override;

      //void bind(auto pcommandbuffer);

      ///static void default_pipeline_configuration(pipeline_configuration& configInfo);
   ///private:
///		static ::array<char> readFile(const ::string& filepath);

      /*void create_graphics_pipeline(
         const ::string& vertFilepath,
         const ::string& fragFilepath,
         const pipeline_configuration& configInfo);

      void create_shader_module(const ::block& block, VkShaderModule* shaderModule);*/

      /*
      VkPipeline graphicsPipeline;
      VkShaderModule vertShaderModule;
      VkShaderModule fragShaderModule;*/
      // VkPipelineLayout		m_vkpipelinelayout;


      //void _create_pipeline_layout(int iPushPropertiesSize);

      virtual render_pass* render_pass2();
      virtual void create_descriptor_layout();

      void draw() override;

      bool has_sampler();
      //class shader_sampler * shader_sampler();
      //class shader_texture * shader_texture(::gpu::texture* ptexture, bool bSampler);

      //void initialize_shader(
      //	::gpu::context* pgpucontext,
      //	const ::file::path& pathVert,
      //	const ::file::path& pathFrag,
      //	const ::gpu::property* pproperties) override;

      void on_initialize_shader() override;

      virtual void _create_pipeline();

      void bind(::gpu::texture* pgputextureTarget, ::gpu::texture* pgputextureSource) override;
      void bind(::gpu::texture* pgputextureTarget) override;
      void bind() override;
      void unbind() override;
      virtual void _bind(::gpu::texture* pgputextureTarget);
      virtual void _bind();


      void push_properties() override;


      void set_push_properties(const ::block& block) override;


      void bind_source(::gpu::texture * ptexture, int iSlot) override;


   };


} // namespace gpu_vulkan 




//#pragma once
//
//
//#include "bred/gpu/shader.h"
//
//
//namespace gpu_vulkan
//{
//   //enum enum_type
//   //{
//   //   e_type_int,
//   //   e_type_float,
//
//   //};
//
//   class CLASS_DECL_GPU_VULKAN shader :
//      virtual public ::gpu::shader
//   {
//   public:
//
//      //struct payload
//      //{
//
//      //   int m_iOffset;
//      //   enum_type m_etype;
//
//      //};
//
//      memory   m_memoryPushConstants;
//      //__creatable_from_base(shader, ::gpu::shader);
//      
//      shader();
//      ~shader() override;
//
//      
//      virtual void create_shader(const ::string & pszVertex, const ::string & pszFragment, const ::string & pszGeometry = nullptr) override;
//
//      virtual void use() override;
//
//      virtual void setBool(const ::scoped_string & scopedstrName, bool value) override;
//
//      virtual void setInt(const ::scoped_string & scopedstrName, int value) override;
//
//      virtual void setFloat(const ::scoped_string & scopedstrName, float value) override;
//
//      //virtual void setVec2(const ::scoped_string & scopedstrName, const glm::vec2& value) override;
//      virtual void setVec2(const ::scoped_string & scopedstrName, float x, float y) override;
//
//      //virtual void setVec3(const ::scoped_string & scopedstrName, const glm::vec3& value) override;
//      virtual void setVec3(const ::scoped_string & scopedstrName, float x, float y, float z) override;
//
//      //virtual void setVec4(const ::scoped_string & scopedstrName, const glm::vec4& value) override;
//      virtual void setVec4(const ::scoped_string & scopedstrName, float x, float y, float z, float w) override;
//
//      //virtual void setMat2(const ::scoped_string & scopedstrName, const glm::mat2& mat) override;
//      //virtual void setMat3(const ::scoped_string & scopedstrName, const glm::mat3& mat) override;
//      //virtual void setMat4(const ::scoped_string & scopedstrName, const glm::mat4& mat) override;
//      virtual void setMat2(const ::scoped_string & scopedstrName, const float p[2*2]) override;
//      virtual void setMat3(const ::scoped_string & scopedstrName, const float p[3*3]) override;
//      virtual void setMat4(const ::scoped_string & scopedstrName, const float p[4*4]) override;
//
//
//
//      unsigned int create_shader(const ::string & pszVertex, VkShaderStageFlagBits type);
//
//      virtual void shader_compile_errors(int shader, VkShaderStageFlagBits type, string & strSummary);
//
//      virtual void program_compile_errors(int program, string & strSummary);
//      //::gpu::uniform get_uniform(const ::string & strUniform) override;
//
//   };
//
//
//} // namespace gpu_vulkan
//
//
//

// From directx2 texture by camilo on 2025-06-16 02:11 <3ThomasBorregaardSørensen!!
// Created by camilo on 2025-06-08 18:11 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/texture.h"


namespace gpu_vulkan
{


   class CLASS_DECL_GPU_VULKAN texture :
      virtual public ::gpu::texture
   {
   //protected:


      //texture* _get_depth_texture();


   public:


      struct
      {

         int m_bForOnAfterDoneFrameStep : 1;

         void set_new_texture()
         {
            memset(this, 0xff, sizeof(*this));

         }


      }new_texture;

      struct state_t
      {
         VkAccessFlags              m_vkaccessflags;
         VkImageLayout              m_vkimagelayout;
         VkPipelineStageFlags       m_vkpipelinestageflags;

         state_t() {}

         state_t(
            VkAccessFlags              vkaccessflags,
            VkImageLayout              vkimagelayout,
            VkPipelineStageFlags       vkpipelinestageflags
         ) :
            m_vkimagelayout(vkimagelayout),
            m_vkaccessflags(vkaccessflags),
            m_vkpipelinestageflags(vkpipelinestageflags)
         {


         }



      };


      class scoped_state
      {
      public:
         ::gpu_vulkan::command_buffer* m_pcommandbuffer;
         texture* m_ptexture;
         state_t m_state;

         scoped_state(::gpu_vulkan::command_buffer* pcommandbuffer, texture* ptexture, state_t state) :
            m_pcommandbuffer(pcommandbuffer),
            m_ptexture(ptexture),
            m_state(state)

         {

            m_state = m_ptexture->m_state;

         }
         ~scoped_state()
         {

            m_ptexture->_set_state(m_pcommandbuffer, m_state);

         }

      };

      struct shader_t
      {

         bool m_bNew = true;
         VkDescriptorSet m_vkdescriptorset = VK_NULL_HANDLE;


      };


      bool                       m_bOwnImage;
      VkImage                    m_vkimage;
      VkFormat                   m_vkformat;
      VkDeviceMemory             m_vkdevicememory;
      state_t                    m_state;
      int                        m_mipsLevel;
      //VkImage                    m_vkimageDepth;
      //VkDeviceMemory             m_vkdevicememoryDepth;
      VkImageView                m_vkimageview;
      //VkImageView                m_vkimageviewDepth;
      map<VkRenderPass, VkFramebuffer >             m_mapFramebuffer;
      map<::gpu_vulkan::shader *, shader_t >             m_mapShader;


      texture();
      ~texture() override;


      void initialize_image_texture(::gpu::renderer* prenderer, const ::int_rectangle& rectangleTarget, bool bWithDepth, ::pixmap* ppixmap = nullptr, enum_type etype = e_type_image) override;
      void initialize_depth_texture(::gpu::renderer* pgpurenderer, const ::int_rectangle& rectangleTarget) override;
      //void blend(::gpu::texture* ptexture, const ::int_rectangle& rectangleTarget) override;
      //void TransitionImageLayout(
      //   VkImageLayout newLayout,
      //   uint32_t    layerCount);

      void _set_state(::gpu_vulkan::command_buffer * pcommandbuffer, 
         state_t state);
      scoped_state _scoped_state(::gpu_vulkan::command_buffer* pcommandbuffer,
         state_t state)
      {

         auto stateRestore = m_state;

         _set_state(pcommandbuffer, state);

         return { pcommandbuffer, this, stateRestore };

        

      }

      void _attach(VkImage vkimage, enum_type etype);
      virtual unsigned int _get_layer_count();
      virtual VkImageViewType _get_image_view_type();

      void destroy();

      VkImageView get_image();

      VkImageView get_image_view();

      VkImage get_depth_image();

      VkImageView get_depth_image_view();


      VkDescriptorSet descriptor_set(::gpu_vulkan::shader* pshader);

      VkFramebuffer get_framebuffer(::gpu_vulkan::render_pass * prenderpass);

      // VkFramebuffer create_framebuffer(VkRenderPass renderpass);
      void _LoadCubeMap(::pixmap* ppixmap);
   };


} // namespace gpu_vulkan




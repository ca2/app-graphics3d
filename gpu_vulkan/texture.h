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

      bool                       m_bOwnImage;
      VkImage                    m_vkimage;
      VkDeviceMemory             m_vkdevicememory;
      state_t                    m_state;
      //VkImage                    m_vkimageDepth;
      //VkDeviceMemory             m_vkdevicememoryDepth;
      VkImageView                m_vkimageview;
      //VkImageView                m_vkimageviewDepth;
      map<VkRenderPass, VkFramebuffer >             m_mapFramebuffer;


      texture();
      ~texture() override;


      void initialize_image_texture(::gpu::renderer* prenderer, const ::int_rectangle& rectangleTarget, bool bWithDepth);
      void initialize_depth_texture(::gpu::renderer* pgpurenderer, const ::int_rectangle& rectangleTarget) override;
      //void blend(::gpu::texture* ptexture, const ::int_rectangle& rectangleTarget) override;

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


      void destroy();

      VkImageView get_image();

      VkImageView get_image_view();

      VkImage get_depth_image();

      VkImageView get_depth_image_view();

      VkFramebuffer get_framebuffer(VkRenderPass vkrenderpass);

      // VkFramebuffer create_framebuffer(VkRenderPass renderpass);

   };


} // namespace gpu_vulkan




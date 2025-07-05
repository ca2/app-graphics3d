// From directx2 texture by camilo on 2025-06-16 02:11 <3ThomasBorregaardSørensen!!
// Created by camilo on 2025-06-08 18:11 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/texture.h"


namespace gpu_vulkan
{


   class CLASS_DECL_GPU_VULKAN texture :
      virtual public ::gpu::texture
   {
   protected:


      texture* _get_depth_texture();


   public:


      struct
      {

         int m_bForOnAfterDoneFrameStep : 1;

         void set_new_texture()
         {
            memset(this, 0xff, sizeof(*this));

         }


      }new_texture;


      VkImage                    m_vkimage;
      VkDeviceMemory             m_vkdevicememory;
      VkImageLayout              m_vkimagelayout;
      VkAccessFlags              m_vkaccessflags;
      VkPipelineStageFlags       m_vkpipelinestageflags;
      //VkImage                    m_vkimageDepth;
      //VkDeviceMemory             m_vkdevicememoryDepth;
      VkImageView                m_vkimageview;
      //VkImageView                m_vkimageviewDepth;
      map<VkRenderPass, VkFramebuffer >             m_mapFramebuffer;
      ::pointer < texture >      m_ptextureDepth;    
      
      texture();
      ~texture() override;


      void initialize_image_texture(::gpu::renderer* prenderer, const ::int_rectangle& rectangleTarget, bool bWithDepth);

      //void blend(::gpu::texture* ptexture, const ::int_rectangle& rectangleTarget) override;

      void _new_state(::gpu_vulkan::command_buffer * pcommandbuffer, 
         VkAccessFlags accessflags,
         VkImageLayout newLayout,
         VkPipelineStageFlags pipelineStageFlags);

      VkImageView get_image();

      VkImageView get_image_view();

      VkImage get_depth_image();

      VkImageView get_depth_image_view();

      VkFramebuffer get_framebuffer(VkRenderPass vkrenderpass);

      // VkFramebuffer create_framebuffer(VkRenderPass renderpass);

   };


} // namespace gpu_vulkan




// From directx2 texture by camilo on 2025-06-16 02:11 <3ThomasBorregaardSørensen!!
// Created by camilo on 2025-06-08 18:11 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/texture.h"


namespace gpu_vulkan
{


   class CLASS_DECL_GPU_VULKAN texture :
      virtual public ::gpu::texture
   {
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

      
      texture();
      ~texture() override;


      void initialize_gpu_texture(::gpu::renderer* prenderer, const ::int_size & size);

      void blend(::gpu::texture* ptexture, const ::int_rectangle& rectangleTarget) override;

   };


} // namespace gpu_vulkan




// Created by camilo on 2025-06-08 18:14 < 3ThomasBorregaardSørensen!!
#include "framework.h"
#include "texture.h"
#include "renderer.h"


namespace gpu_vulkan
{


   texture::texture()
   {

      new_texture.set_new_texture();

   }


   texture::~texture()
   {

   }


   void texture::initialize_gpu_texture(::gpu::renderer* prenderer, const ::int_size& size) //, bool bCreateRenderTargetView, bool bCreateShaderResourceView)
   {

      ::gpu::texture::initialize_gpu_texture(prenderer, size);


   }


   void texture::blend(::gpu::texture* ptexture, const ::int_rectangle& rectangleTarget)
   {

      

   }


} // namespace gpu_vulkan





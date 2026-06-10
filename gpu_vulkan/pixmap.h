//// From gpu_opengl/pixmap.h by
////  camilo on 2025-07-29 07:57 <3ThomasBorregaardSørensen!!
//// From bred/gpu/pixmap.h by
////  camilo on 2025-07-01 14:36 <3ThomasBorregaardSørensen!!
//#pragma once
//
//
//#include "bred/gpu/pixmap.h"
//
//
//namespace gpu_vulkan
//{
//
//
//   class CLASS_DECL_GPU_VULKAN pixmap :
//      virtual public ::gpu::pixmap
//   {
//   public:
//
//
//      VkImageView       m_vkimageview;
//
//
//      pixmap();
//      ~pixmap() override;
//
//
//      // virtual ::i32_size size();
//
//
//      //void initialize_gpu_pixmap(::gpu::texture * pgpurenderer, const ::i32_rectangle & rectangle) override;
//
//      void on_initialize_gpu_pixmap() override;
//
//
//      void set_pixels(const void* data, int w, int h) override;
//      //virtual void merge_layers(::pointer_array < ::gpu::layer >* playera);
//      //virtual void blend(::gpu::layer * pgpulayer);
//      //virtual void blend(::gpu::texture* ptexture);
//
//
//      void bind_texture(::gpu::shader* pgpushader) override;
//      void unbind_texture(::gpu::shader* pgpushader) override;
//
//      virtual VkImageView get_image_view();
//
//
//   };
//
//
//} // namespace gpu

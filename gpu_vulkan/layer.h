// From bred/gpu/layer.h by camilo on 2025-06-27 22:44 <3ThomasBorregaardSørensen!!
// Created by camilo on 2025-06-12 20:58 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/layer.h"


namespace gpu_vulkan
{


   class CLASS_DECL_GPU_VULKAN layer :
      virtual public ::gpu::layer
   {
   public:

      VkSemaphore          m_vksemaphoreRenderFinished;

      //int m_iFrameIndex = -1;
      //int m_iLayerIndex = -1;
      //::pointer < renderer >        m_pgpurenderer;
      ////::pointer < renderer >        m_pgpurendererTarget;
      ////::int_rectangle m_rectangleTarget;
      //::pointer_array <texture >    m_texturea;
      //::pointer_array <texture >    m_textureaSource;
      //::pointer < ::gpu::frame > m_pgpuframe;
      ////::comptr < ID3D12Resource > m_presource;

      //::pointer < command_buffer >  m_pcommandbufferLayer;

      layer();
      ~layer() override;


      void initialize_gpu_layer(::gpu::renderer* pgpurenderer, int iFrameIndex, int iLayerIndex) override;


   };


} // namespace gpu_vulkan




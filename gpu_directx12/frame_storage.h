// Created by camilo on 2025-08-16 01:27 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/frame_storage.h"


namespace gpu_directx12
{



   class CLASS_DECL_GPU_DIRECTX12 frame_storage :
      virtual public ::gpu::frame_storage
   {
   public:


      //struct resource
      //{
      //   ::comptr<ID3D12Resource> m_presource;
      //   unsigned char * m_pMap = nullptr;

      //};



      frame_storage();
      ~frame_storage() override;


      void map_allocate(::gpu::memory_buffer* pgpumemorybuffer, int size) override;


   };



} // namespace gpu_directx12




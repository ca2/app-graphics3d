// Created by camilo on 2025-08-16 01:27 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "context.h"
#include "device.h"
#include "frame_storage.h"
#include "memory_buffer.h"


namespace gpu_directx12
{


   frame_storage::frame_storage()
   {


   }


   frame_storage::~frame_storage()
   {


   }


   void frame_storage::map_allocate(::gpu::memory_buffer* pgpumemorybuffer, int size)
   {

      ::gpu::frame_storage::map_allocate(pgpumemorybuffer, size);

   }



} // namespace gpu_directx12




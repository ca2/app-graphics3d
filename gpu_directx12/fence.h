// Dropped in by camilo on 2026-06-08 00:40 <3ThomasBorregaardSørensen!! Mummi!! Bilbo!!
#pragma once


#include <d3d12.h>
#include <wrl/client.h>
#include "bred/gpu/fence.h"


namespace gpu_directx12
{


   class fence : virtual public ::gpu::fence
   {
   public:


      ::comptr<ID3D12Fence> m_pfence;
      HANDLE m_hevent;
      UINT64 m_uFenceValue;


      fence();
      ~fence() override;


      void initialize_gpu_fence(::gpu::context *pgpucontext, bool bCreateSignaled) override;

      void reset_gpu_fence() override;

      void wait_gpu_fence() override;

      void signal(ID3D12CommandQueue *pcommandqueue);
   };


} // namespace gpu_directx12

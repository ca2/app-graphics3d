// Created by camilo on 2026-06-11.
#pragma once


#include <deque>
#include <mutex>
#include "bred/gpu/semaphore.h"


namespace gpu_directx12
{


   class semaphore :
      virtual public ::gpu::semaphore
   {
   public:


      ::comptr<ID3D12Fence> m_pfence;
      std::mutex m_mutex;
      UINT64 m_uNextFenceValue;
      std::deque<UINT64> m_signalaPendingWait;
      std::deque<UINT64> m_waitaPendingSignal;


      semaphore();
      ~semaphore() override;


      void initialize_gpu_semaphore(::gpu::context *pgpucontext) override;

      void wait(ID3D12CommandQueue *pcommandqueue);
      void signal(ID3D12CommandQueue *pcommandqueue);


   };


} // namespace gpu_directx12

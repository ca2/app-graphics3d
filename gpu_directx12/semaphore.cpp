// Created by camilo on 2026-06-11.
#include "platform.h"
#include "semaphore.h"
#include "device.h"
#include "queue.h"
#include "bred/gpu/context.h"


namespace gpu_directx12
{


   semaphore::semaphore() :
      m_uNextFenceValue(1)
   {


   }


   semaphore::~semaphore()
   {


   }


   void semaphore::initialize_gpu_semaphore(::gpu::context *pgpucontext)
   {

      ::gpu::semaphore::initialize_gpu_semaphore(pgpucontext);

      ::cast<::gpu_directx12::device> pdevice = m_pgpucontext->m_pgpudevice;

      HRESULT hresult = pdevice->m_pd3d12device->CreateFence(
         0,
         D3D12_FENCE_FLAG_NONE,
         IID_PPV_ARGS(&m_pfence));

      ::defer_throw_hresult(hresult);

   }


   void semaphore::_wait(ID3D12CommandQueue *pcommandqueue)
   {

      UINT64 uFenceValue;

      {

         std::scoped_lock lock(m_mutex);

         if (m_signalaPendingWait.empty())
         {

            uFenceValue = m_uNextFenceValue++;
            m_waitaPendingSignal.push_back(uFenceValue);

         }
         else
         {

            uFenceValue = m_signalaPendingWait.front();
            m_signalaPendingWait.pop_front();

         }

      }

      HRESULT hresult = pcommandqueue->Wait(m_pfence, uFenceValue);

      ::defer_throw_hresult(hresult);

   }


   void semaphore::_signal(ID3D12CommandQueue *pcommandqueue)
   {

      UINT64 uFenceValue;

      {

         std::scoped_lock lock(m_mutex);

         if (m_waitaPendingSignal.empty())
         {

            uFenceValue = m_uNextFenceValue++;
            m_signalaPendingWait.push_back(uFenceValue);

         }
         else
         {

            uFenceValue = m_waitaPendingSignal.front();
            m_waitaPendingSignal.pop_front();

         }

      }

      HRESULT hresult = pcommandqueue->Signal(m_pfence, uFenceValue);

      ::defer_throw_hresult(hresult);

   }


   void semaphore::wait(::gpu::queue * pgpuqueue)
   {

      ::cast < ::gpu_directx12::queue > pqueue = pgpuqueue;


      _wait(pqueue->m_pd3d12commandqueue);


      //::cast<::gpu_directx12::semaphore> psemaphore = pgpusemaphore;
//
    ///  pgpusemaphore->wait(m_pgpuqueue);

  //    psemaphore->wait(m_pcommandqueue);

   }


   void semaphore::signal(::gpu::queue * pgpuqueue)
   {

      ::cast < ::gpu_directx12::queue > pqueue = pgpuqueue;


      _signal(pqueue->m_pd3d12commandqueue);


      //::cast<::gpu_directx12::semaphore> psemaphore = pgpusemaphore;
//
    ///  pgpusemaphore->wait(m_pgpuqueue);

  //    psemaphore->wait(m_pcommandqueue);

   }


} // namespace gpu_directx12

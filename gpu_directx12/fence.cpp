// Dropped in by camilo on 2026-06-08 00:41 <3ThomasBorregaardSørensen!! Mummi!! Bilbo!!
#include "platform.h"
#include "fence.h"
#include "device.h"
#include "queue.h"
#include "bred/gpu/context.h"


namespace gpu_directx12
{


   fence::fence()
   {

      m_hevent = nullptr;
      //m_uFence = 0;
   }


   fence::~fence()
   {

      if (m_hevent)
      {

         ::CloseHandle(m_hevent);

         m_hevent = nullptr;
      }

      //m_pfence->Reset();
   }


   void fence::initialize_gpu_fence(::gpu::context *pgpucontext, bool bCreateSignaled)
   {

      ::gpu::fence::initialize_gpu_fence(pgpucontext, bCreateSignaled);

      ::cast<::gpu_directx12::device> pdevice = pgpucontext->m_pgpudevice;

      m_uFence = 1;

      const UINT64 uInitialValue = bCreateSignaled ? m_uFence : 0;

      HRESULT hresult = pdevice->m_pd3d12device->CreateFence(uInitialValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pfence));

      if (FAILED(hresult))
      {

         throw ::exception(error_failed);
      }

      m_hevent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);

      if (!m_hevent)
      {

         throw ::exception(error_failed);

      }

   }


   void fence::reset_gpu_fence() { ++m_uFence; }


   void fence::_signal(ID3D12CommandQueue *pcommandqueue)
   {

      HRESULT hresult = pcommandqueue->Signal(m_pfence, m_uFence);

      if (FAILED(hresult))
      {

         throw ::exception(error_failed);

      }

   }


   bool fence::has_finished()
   {

      if (m_pfence->GetCompletedValue() >= m_uFence)
      {

         return true;

      }

      return false;

   }


   void fence::wait_gpu_fence()
   {

      if (has_finished())
      {

         return;

      }


      ::ResetEvent(m_hevent);

      HRESULT hresult = m_pfence->SetEventOnCompletion(m_uFence, m_hevent);

      if (FAILED(hresult))
      {

         throw ::exception(error_failed);

      }

      ::WaitForSingleObject(m_hevent, INFINITE);

   }


   void fence::signal_gpu_fence(::gpu::queue * pgpuqueue)
   {

      ::cast < ::gpu_directx12::queue > pqueue = pgpuqueue;

      _signal(pqueue->m_pd3d12commandqueue);

   }


} // namespace gpu_directx12

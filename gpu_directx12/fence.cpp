// Dropped in by camilo on 2026-06-08 00:41 <3ThomasBorregaardSørensen!! Mummi!! Bilbo!!
#include "framework.h"
#include "fence.h"
#include "bred/gpu/context.h"
#include "device.h"


namespace gpu_directx12
{


   fence::fence()
   {

      m_hevent = nullptr;
      m_uFenceValue = 0;
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

      ::cast<::gpu_directx12::device> pdevice = m_pgpucontext->m_pgpudevice;

      m_uFenceValue = 1;

      const UINT64 uInitialValue = bCreateSignaled ? m_uFenceValue : 0;

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


   void fence::reset_gpu_fence() { ++m_uFenceValue; }


   void fence::signal(ID3D12CommandQueue *pcommandqueue)
   {

      HRESULT hresult = pcommandqueue->Signal(m_pfence, m_uFenceValue);

      if (FAILED(hresult))
      {

         throw ::exception(error_failed);
      }
   }


   void fence::wait_gpu_fence()
   {

      if (m_pfence->GetCompletedValue() >= m_uFenceValue)
      {

         return;
      }

      HRESULT hresult = m_pfence->SetEventOnCompletion(m_uFenceValue, m_hevent);

      if (FAILED(hresult))
      {

         throw ::exception(error_failed);
      }

      ::WaitForSingleObject(m_hevent, INFINITE);
   }


} // namespace gpu_directx12

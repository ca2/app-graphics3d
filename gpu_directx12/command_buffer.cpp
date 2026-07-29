// Created by camilo on 2025-06-23 00:16 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "approach.h"
#include "command_buffer.h"
#include "depth_stencil.h"
#include "descriptors.h"
#include "fence.h"
#include "frame.h"
#include "renderer.h"
#include "semaphore.h"
#include "texture.h"
#include "offscreen_render_target_view.h"
#include "physical_device.h"
#include "swap_chain.h"
#include "initializers.h"
#include "bred/gpu/aaa_cpu_buffer.h"
#include "bred/gpu/layer.h"
//#include "bred/gpu/render_state.h"
#include "gpu_directx12/shader.h"
#include "acme/parallelization/synchronous_lock.h"
#include "acme/platform/application.h"
#include "aura/graphics/image/aaa_target.h"
#include "aura/user/user/interaction.h"
#include "aura/windowing/window.h"


using namespace directx12;



namespace gpu_directx12
{


   command_buffer::command_buffer()
   {

      m_fenceValue = 0;

      m_hFenceEvent = nullptr;

   }


   command_buffer::~command_buffer()
   {

      if (m_hFenceEvent)
      {

         ::CloseHandle(m_hFenceEvent);

         m_hFenceEvent = nullptr;

      }

   }


   void command_buffer::initialize_command_buffer(::gpu::render_target* pgpurendertarget, ::gpu::queue * pqueue, ::gpu::enum_command_buffer ecommandbuffer)
   {
      
      ::gpu::command_buffer::initialize_command_buffer(pgpurendertarget, pqueue, ecommandbuffer);

      ::cast < renderer > prenderer = pgpurendertarget->m_pgpurenderer;

      ::cast < context > pcontext = prenderer->m_pgpucontext;

      if (ecommandbuffer == ::gpu::e_command_buffer_graphics)
      {

         _initialize_command_buffer(
            pcontext->command_queue(),
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            prenderer);

      }
      else if (ecommandbuffer == ::gpu::e_command_buffer_copy)
      {

         _initialize_command_buffer(
            pcontext->copy_command_queue(),
            D3D12_COMMAND_LIST_TYPE_COPY,
            prenderer);

      }

   }


   void command_buffer::_initialize_command_buffer(ID3D12CommandQueue * pcommandqueue, D3D12_COMMAND_LIST_TYPE ecommandlisttype, ::gpu_directx12::renderer* prenderer)
   {

      if (::is_null(pcommandqueue))
      {

         throw ::exception(error_wrong_state);

      }

      m_prenderer = prenderer;

      m_pcommandqueue = pcommandqueue;

      m_ecommandlisttype = ecommandlisttype;

      // D3D12_COMMAND_LIST_TYPE_DIRECT
      // D3D12_COMMAND_LIST_TYPE_DIRECT

      ::cast<gpu_directx12::device> pdevice = prenderer->m_pgpucontext->m_pgpudevice;

      HRESULT hr = pdevice->m_pd3d12device->CreateCommandAllocator(
         m_ecommandlisttype,  // Type: DIRECT for graphics
         __interface_of(m_pcommandallocator)
      );

      ::defer_throw_hresult(hr);

      //auto& pcommandlist = m_framea.element_at_grow(iFrame);
      // 4. Create command list (can be reused)
      ::defer_throw_hresult(pdevice->m_pd3d12device->CreateCommandList(
         0,
         m_ecommandlisttype,
         m_pcommandallocator, // initial allocator
         nullptr, // No PSO yet
         __interface_of(m_pcommandlist)
      ));

      ::defer_throw_hresult(m_pcommandlist->Close()); // Must be closed before Reset()

      HRESULT hrCreateFeence =
         pdevice->m_pd3d12device->CreateFence(m_fenceValue, D3D12_FENCE_FLAG_NONE,
            __interface_of(m_pfence));

      ::defer_throw_hresult(hrCreateFeence);

      // 5. Create fence + event for GPU sync
      m_hFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

      ///m_fenceValue = 1;

   }


   void command_buffer::begin_command_buffer(bool bOneTime)
   {

      reset();

      m_estate = ::gpu::command_buffer::e_state_recording;

   }


   void command_buffer::_copy_buffer(d3d12_resource *pd3d12resourceTargetBuffer,
                                     d3d12_resource *pd3d12resourceSourceBuffer, memsize size)
   {
      pd3d12resourceTargetBuffer->_set_state(this, D3D12_RESOURCE_STATE_COPY_DEST);
      if (!pd3d12resourceSourceBuffer->m_bUpload)
      {
         pd3d12resourceSourceBuffer->_set_state(this, D3D12_RESOURCE_STATE_COPY_SOURCE);
      }


      m_pcommandlist->CopyBufferRegion(pd3d12resourceTargetBuffer->m_presource, 0, pd3d12resourceSourceBuffer->m_presource, 0,
                                       size);



   }


   void command_buffer::_copy_resource(texture * ptextureTarget, texture * ptextureSource)
   {



            m_pcommandlist->CopyResource(ptextureTarget->m_pd3d12resourceTexture->m_presource, ptextureSource->m_pd3d12resourceTexture->m_presource);


   }


   void command_buffer::submit_command_buffer(::gpu::layer* pgpulayer)
   {

      ::cast < ::gpu_directx12::device > pdevice = m_prenderer->m_pgpucontext->m_pgpudevice;
      
      //informationf("Going to close Command List : 0x%016llx", m_pcommandlist.m_p);

      HRESULT hrCloseCommandList = m_pcommandlist->Close();

      pdevice->defer_throw_hresult(hrCloseCommandList);

      for (auto &pgpusemaphore : m_semaphoreaWait)
      {

         ::cast<::gpu_directx12::semaphore> psemaphore = pgpusemaphore;

         psemaphore->wait(m_pcommandqueue);

      }

      ID3D12CommandList* ppCommandLists[] = { m_pcommandlist };

      m_pcommandqueue->ExecuteCommandLists(1, ppCommandLists);

      for (auto &pgpusemaphore : m_semaphoreaSignal)
      {

         ::cast<::gpu_directx12::semaphore> psemaphore = pgpusemaphore;

         psemaphore->signal(m_pcommandqueue);

      }

      ::cast<::gpu_directx12::fence> pfence = m_pgpufence;

      if (pfence)
      {

         pfence->signal(m_pcommandqueue);

      }

      m_estate = ::gpu::command_buffer::e_state_submitted;

   }


   void command_buffer::wait_commands_to_execute()
   {

      ::cast < ::gpu_directx12::device > pdevice = m_prenderer->m_pgpucontext->m_pgpudevice;

      //UINT64 uploadFenceValue = ++m_fenceValue;

      m_fenceValue++;

      auto hrSignalCommandQueue = m_pcommandqueue->Signal(m_pfence, m_fenceValue);

      pdevice->defer_throw_hresult(hrSignalCommandQueue);

      wait_for_gpu();

   }


   void command_buffer::wait_for_gpu()
   {

      //const UINT64 fenceValue = ++m_fenceValue;

      //prenderer->m_pcommandqueue->Signal(m_pfence, m_fenceValue);

      if (m_pfence->GetCompletedValue() < m_fenceValue)
      {

         ::ResetEvent(m_hFenceEvent);

         m_pfence->SetEventOnCompletion(m_fenceValue, m_hFenceEvent);

         ::WaitForSingleObject(m_hFenceEvent, INFINITE);

      }

      //m_iCurrentFrame2 = (m_iCurrentFrame2 + 1) % get_frame_count();


   }


   void command_buffer::reset()
   {

      auto pcommandallocator = m_pcommandallocator;

      HRESULT hrResetCommandAllocator = pcommandallocator->Reset();

      ::defer_throw_hresult(hrResetCommandAllocator);

      m_pcommandlist->Reset(pcommandallocator, nullptr);

   }


   bool command_buffer::has_finished()
   {

      return m_pfence->GetCompletedValue() >= m_fenceValue;

   }



} // namespace gpu_directx12




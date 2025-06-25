// Created by camilo on 2025-06-23 00:16 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "approach.h"
#include "command_buffer.h"
#include "depth_stencil.h"
#include "descriptors.h"
#include "frame.h"
#include "renderer.h"
#include "texture.h"
#include "offscreen_render_target_view.h"
#include "physical_device.h"
#include "swap_chain.h"
#include "initializers.h"
#include "bred/gpu/cpu_buffer.h"
#include "bred/gpu/layer.h"
#include "bred/gpu/render_state.h"
#include "gpu_directx12/shader.h"
#include "acme/parallelization/synchronous_lock.h"
#include "acme/platform/application.h"
#include "aura/graphics/image/target.h"
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


   void command_buffer::initialize_command_buffer(D3D12_COMMAND_LIST_TYPE ecommandlisttype, ::gpu_directx12::renderer* prenderer)
   {

      m_prenderer = prenderer;

      //m_pcommandqueue = pcommandqueue;

      m_ecommandlisttype = ecommandlisttype;

      // D3D12_COMMAND_LIST_TYPE_DIRECT
      // D3D12_COMMAND_LIST_TYPE_DIRECT

      ::cast<gpu_directx12::device> pdevice = prenderer->m_pgpucontext->m_pgpudevice;

      HRESULT hr = pdevice->m_pdevice->CreateCommandAllocator(
         m_ecommandlisttype,  // Type: DIRECT for graphics
         __interface_of(m_pcommandallocator)
      );

      ::defer_throw_hresult(hr);

      //auto& pcommandlist = m_framea.element_at_grow(iFrame);
      // 4. Create command list (can be reused)
      ::defer_throw_hresult(pdevice->m_pdevice->CreateCommandList(
         0,
         m_ecommandlisttype,
         m_pcommandallocator, // initial allocator
         nullptr, // No PSO yet
         __interface_of(m_pcommandlist)
      ));

      ::defer_throw_hresult(m_pcommandlist->Close()); // Must be closed before Reset()

      HRESULT hrCreateFeence =
         pdevice->m_pdevice->CreateFence(0, D3D12_FENCE_FLAG_NONE,
            __interface_of(m_pfence));

      ::defer_throw_hresult(hrCreateFeence);

      // 5. Create fence + event for GPU sync
      m_hFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

   }


   //::pointer <command_buffer >renderer::beginSingleTimeCommands()
   //{

   //   ::pointer <command_buffer > pcommandbuffer;

   //   __defer_construct_new(pcommandbuffer);

   //   ::cast < device > pdevice= m_pgpucontext->m_pgpudevice;

   //   pcommandbuffer->initialize_command_buffer(pdevice);

   //   return pcommandbuffer;

   //}


   void command_buffer::submit_command_buffer(ID3D12CommandQueue * pcommandqueue)
   {

      ::cast < ::gpu_directx12::device > pdevice = m_prenderer->m_pgpucontext->m_pgpudevice;

      HRESULT hrCloseCommandList = m_pcommandlist->Close();

      pdevice->defer_throw_hresult(hrCloseCommandList);

      ID3D12CommandList* ppCommandLists[] = { m_pcommandlist };

      pcommandqueue->ExecuteCommandLists(1, ppCommandLists);


   }


   void command_buffer::wait_commands_to_execute(ID3D12CommandQueue* pcommandqueue)
   {

      ::cast < ::gpu_directx12::device > pdevice = m_prenderer->m_pgpucontext->m_pgpudevice;

      UINT64 uploadFenceValue = ++m_fenceValue;

      HRESULT hrSignalCommandQueue = pcommandqueue->Signal(m_pfence, uploadFenceValue);

      pdevice->defer_throw_hresult(hrSignalCommandQueue);

   }


   void command_buffer::wait_for_gpu()
   {

      //const UINT64 fenceValue = ++m_fenceValue;

      //prenderer->m_pcommandqueue->Signal(m_pfence, m_fenceValue);

      if (m_pfence->GetCompletedValue() < m_fenceValue)
      {

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




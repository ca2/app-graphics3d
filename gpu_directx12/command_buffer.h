// Created by camilo on 2025-06-23 00:14 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/command_buffer.h"


namespace gpu_directx12
{


   class command_buffer :
      virtual public ::gpu::command_buffer
   {
   public:


      ::comptr<ID3D12Fence>                     m_pfence;
      UINT64                                    m_fenceValue;
      HANDLE                                    m_hFenceEvent;
      ::comptr<ID3D12CommandAllocator >         m_pcommandallocator;
      ::comptr < ID3D12GraphicsCommandList >    m_pcommandlist;
      ::comptr < ID3D12CommandQueue >           m_pcommandqueue;
      ::pointer < ::gpu_directx12::renderer >   m_prenderer;
      D3D12_COMMAND_LIST_TYPE                   m_ecommandlisttype;

      ::array <comptr <IUnknown > >             m_comptraHold;


      command_buffer();
      ~command_buffer() override;

      void initialize_command_buffer(::gpu::render_target* prendertarget,::gpu::queue * pqueue, ::gpu::enum_command_buffer ecommandbuffer) override;
      virtual void _initialize_command_buffer(ID3D12CommandQueue * pcommandqueue, D3D12_COMMAND_LIST_TYPE ecommandlisttype, ::gpu_directx12::renderer* prenderer);

      void submit_command_buffer(::gpu::layer* pgpulayer) override;

      void wait_commands_to_execute() override;

      virtual void wait_for_gpu();

      virtual void reset();

      virtual bool has_finished();

      void begin_command_buffer(bool bOneTime) override;

      virtual void _copy_buffer(d3d12_resource *pd3d12resourceTargetBuffer, d3d12_resource *pd3d12resourceSourceBuffer,
                        memsize size);

      virtual void _copy_resource(texture *ptextureTarget, texture *ptextureSource);

   };


} // namespace gpu_directx12




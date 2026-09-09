// Created by camilo on 2025-06-23 00:14 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/command_buffer.h"


namespace gpu_directx12
{


   class command_buffer :
      virtual public ::gpu::command_buffer
   {
   public:


      //::comptr<ID3D12Fence>                     m_pfence;
      
      //HANDLE                                    m_hFenceEvent;
      ::comptr<ID3D12CommandAllocator >         m_pcommandallocator;
      ::comptr < ID3D12GraphicsCommandList >    m_pcommandlist;
      // D3D12 has no RSGet* calls. All viewport/scissor writes must use the
      // setters below so scoped restoration sees the actual recorded state.
      D3D12_VIEWPORT m_viewporta[D3D12_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE]{};
      D3D12_RECT m_scissora[D3D12_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE]{};
      UINT m_uViewportCount = 0;
      UINT m_uScissorCount = 0;
      unsigned long long m_uRasterizerStateGeneration = 0;
      //::comptr < ID3D12CommandQueue >           m_pcommandqueue;
      ::pointer < ::gpu_directx12::renderer >   m_prenderer;
      

      ::array <comptr <IUnknown > >             m_comptraHold;

      bool                                      m_bDeviceDescriptorHeapSet = false;


      command_buffer();
      ~command_buffer() override;

      void initialize_command_buffer(::gpu::render_target* prendertarget,::gpu::queue * pqueue, ::gpu::enum_command_buffer ecommandbuffer) override;
      ///virtual void _initialize_command_buffer(ID3D12CommandQueue * pcommandqueue, D3D12_COMMAND_LIST_TYPE ecommandlisttype, ::gpu_directx12::renderer* prenderer);

      virtual void _defer_set_device_descriptor_heaps();

      virtual void _clear(::gpu::texture * pgputexture, const ::i32_rectangle & rectangle, const ::color::color & color);

      void clear(::gpu::texture * pgputexture, const ::color::color & color) override;

      void submit_command_buffer(::gpu::layer* pgpulayer) override;

      void wait_commands_to_execute() override;

      virtual void wait_for_gpu();

      virtual void reset();

      void set_viewports(UINT count, const D3D12_VIEWPORT * pviewports);
      void set_scissor_rects(UINT count, const D3D12_RECT * prects);

      virtual bool has_finished();

      void begin_command_buffer(bool bOneTime) override;

      virtual void _copy_buffer(d3d12_resource *pd3d12resourceTargetBuffer, d3d12_resource *pd3d12resourceSourceBuffer,
                        memsize size);

      virtual void _copy_resource(texture *ptextureTarget, texture *ptextureSource);
      virtual void _copy_texture_region(texture * ptextureTarget, texture * ptextureSource);

   };


} // namespace gpu_directx12




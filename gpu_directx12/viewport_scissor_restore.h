#pragma once

#include "command_buffer.h"
#include "bred/gpu/context.h"

namespace gpu_directx12
{
   // Thread-confined to the caller's recording scope; destroy before submission.
   class CLASS_DECL_GPU_DIRECTX12 viewport_scissor_restore :
      virtual public ::gpu::viewport_scissor_restore
   {
   public:
      ::pointer<::gpu_directx12::command_buffer> m_pcommandbuffer;
      ::comptr<ID3D12GraphicsCommandList> m_pcommandlist;
      D3D12_VIEWPORT m_viewporta[D3D12_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE]{};
      D3D12_RECT m_scissora[D3D12_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE]{};
      UINT m_uViewportCount = 0;
      UINT m_uScissorCount = 0;
      unsigned long long m_uRasterizerStateGeneration = 0;

      viewport_scissor_restore();
      viewport_scissor_restore(const viewport_scissor_restore &) = delete;
      viewport_scissor_restore & operator=(const viewport_scissor_restore &) = delete;
      ~viewport_scissor_restore() override;

      void initialize(::gpu::command_buffer * pcommandbuffer) override;
   };
}

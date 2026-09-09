#include "platform.h"
#include "viewport_scissor_restore.h"

namespace gpu_directx12
{
   viewport_scissor_restore::viewport_scissor_restore()
   {
   }

   void viewport_scissor_restore::initialize(::gpu::command_buffer * pgpucommandbuffer)
   {
      ::cast<::gpu_directx12::command_buffer> pcommandbuffer = pgpucommandbuffer;
      if (!pcommandbuffer)
         throw ::exception(error_bad_argument, "Viewport/scissor restore requires a DirectX 12 command buffer");
      if (m_pcommandbuffer || !pcommandbuffer->m_pcommandlist ||
          pcommandbuffer->m_estate != ::gpu::command_buffer::e_state_recording)
         throw ::exception(error_wrong_state, "Viewport/scissor restore requires a fresh guard and recording buffer");

      m_pcommandbuffer = pcommandbuffer;
      m_pcommandlist = pcommandbuffer->m_pcommandlist;
      m_uRasterizerStateGeneration = pcommandbuffer->m_uRasterizerStateGeneration;
      m_uViewportCount = pcommandbuffer->m_uViewportCount;
      m_uScissorCount = pcommandbuffer->m_uScissorCount;
      for (UINT i = 0; i < m_uViewportCount; ++i)
         m_viewporta[i] = pcommandbuffer->m_viewporta[i];
      for (UINT i = 0; i < m_uScissorCount; ++i)
         m_scissora[i] = pcommandbuffer->m_scissora[i];
   }

   viewport_scissor_restore::~viewport_scissor_restore()
   {
      if (!m_pcommandbuffer ||
          m_pcommandbuffer->m_estate != ::gpu::command_buffer::e_state_recording ||
          m_pcommandbuffer->m_uRasterizerStateGeneration != m_uRasterizerStateGeneration ||
          m_pcommandbuffer->m_pcommandlist.m_p != m_pcommandlist.m_p)
         return;

      // The setters also restore the tracked state, allowing nested guards.
      // Unlike D3D11, D3D12 has no separate RSSetState/ScissorEnable to save.
      m_pcommandbuffer->set_viewports(m_uViewportCount, m_viewporta);
      m_pcommandbuffer->set_scissor_rects(m_uScissorCount, m_scissora);
   }
}

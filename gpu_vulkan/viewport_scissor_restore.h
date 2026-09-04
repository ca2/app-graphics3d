// Created by camilo on 2026-09-02.
#pragma once


#include "command_buffer.h"
#include "bred/gpu/context.h"


namespace gpu_vulkan
{


   class CLASS_DECL_GPU_VULKAN viewport_scissor_restore :
      virtual public ::gpu::viewport_scissor_restore
   {
   public:


      ::pointer < ::gpu_vulkan::command_buffer > m_pcommandbuffer;
      VkViewport                                 m_vkviewport;
      VkRect2D                                   m_vkrect2dScissor;
      bool                                       m_bViewportSet;
      bool                                       m_bScissorSet;


      viewport_scissor_restore()
      {

         m_vkviewport = {};
         m_vkrect2dScissor = {};
         m_bViewportSet = false;
         m_bScissorSet = false;

      }


      void initialize(
         ::gpu::command_buffer * pgpucommandbuffer) override
      {

         auto pcommandbuffer =
            dynamic_cast < ::gpu_vulkan::command_buffer * >(
               pgpucommandbuffer);

         if (::is_null(pcommandbuffer))
         {

            return;

         }

         m_pcommandbuffer = pcommandbuffer;
         m_vkviewport = pcommandbuffer->m_vkviewport;
         m_vkrect2dScissor = pcommandbuffer->m_vkrect2dScissor;
         m_bViewportSet = pcommandbuffer->m_bViewportSet;
         m_bScissorSet = pcommandbuffer->m_bScissorSet;

      }


      ~viewport_scissor_restore() override
      {

         auto pcommandbuffer = m_pcommandbuffer;

         if (::is_null(pcommandbuffer)
            || pcommandbuffer->m_vkcommandbuffer == VK_NULL_HANDLE
            || pcommandbuffer->m_estate != ::gpu::command_buffer::e_state_recording)
         {

            return;

         }

         if (m_bViewportSet)
         {

            vkCmdSetViewport(
               pcommandbuffer->m_vkcommandbuffer,
               0,
               1,
               &m_vkviewport);

            pcommandbuffer->m_vkviewport = m_vkviewport;

         }

         pcommandbuffer->m_bViewportSet = m_bViewportSet;

         if (m_bScissorSet)
         {

            vkCmdSetScissor(
               pcommandbuffer->m_vkcommandbuffer,
               0,
               1,
               &m_vkrect2dScissor);

            pcommandbuffer->m_vkrect2dScissor = m_vkrect2dScissor;

         }

         pcommandbuffer->m_bScissorSet = m_bScissorSet;

      }


      viewport_scissor_restore(
         const viewport_scissor_restore &) = delete;

      viewport_scissor_restore & operator = (
         const viewport_scissor_restore &) = delete;


   };


} // namespace gpu_vulkan


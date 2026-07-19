// Created by camilo on 2025-12-11 16:03 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "binding.h"
#include "block.h"
#include "buffer.h"
#include "command_buffer.h"
#include "context.h"
#include "memory_buffer.h"
#include "render_target.h"
#include "renderer.h"


namespace gpu_vulkan
{


   block::block() {}


   block::~block() {
   
      if (m_uboBuffers.has_element())
      {
         
         if (((::uptr)m_uboBuffers[0]->m_pbuffer->m_vkbuffer & 0xffff) == 0x019b)
         {

            information("~block (m_vkbuffer & 0xffff) == 0x019b");

         }

      }

   
   }


   //void block::initialize_gpu_block(::gpu::context *pgpucontext)
   //{

   //   ::gpu::block::initialize_gpu_block(pgpucontext);

   //}


   void block::create_gpu_block(::gpu::context *pgpucontext)
   {

      ::gpu::block::create_gpu_block(pgpucontext);

      auto prendertarget = pgpucontext->m_pgpurenderer->render_target();

      auto iFrameCount = prendertarget->m_pgpurenderer->m_pgpucontext->m_pgpudevice->get_frame_count();
      
      m_uboBuffers.set_size(iFrameCount);
      
      ::array_base<VkDescriptorBufferInfo> bufferinfoa;

      int iBufferSize = this->size(false);
      
      for (int i = 0; i < m_uboBuffers.size(); i++)
      {
      
         defer_construct_newø(m_uboBuffers[i]);
      
         m_uboBuffers[i]->_initialize_buffer(pgpucontext, iBufferSize,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

         if (((::uptr)m_uboBuffers[i]->m_pbuffer->m_vkbuffer & 0xffff) == 0x019b)
         {

            information("(m_vkbuffer & 0xffff) == 0x019b");

         }
      
         m_uboBuffers[i]->map(0, m_uboBuffers[i]->m_size);
      
         bufferinfoa.add(m_uboBuffers[i]->descriptorInfo());
      
         m_uboBuffers[i]->unmap();

      }
      
      // pbindingset->defer_create_update_descriptor_set(pgpucommandbuffer, m_descriptorsetsGlobal, bufferinfoa);
      
      // auto globalSetLayout = m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();

   }


   VkDescriptorSet block::descriptor_set(::gpu::binding_slot_set * pgpubindingslotset,
                                         ::gpu::command_buffer *pgpucommandbuffer)
   {

      ::cast<::gpu_vulkan::binding_slot_set> pbindingslotset = pgpubindingslotset;

      auto &vkdescriptorseta = pbindingslotset->descriptor_set(pgpucommandbuffer);

      auto iFrameIndex = pgpucommandbuffer->m_iCommandBufferFrameIndex2;

      if (iFrameIndex < 0 || iFrameIndex >= vkdescriptorseta.size())
      {

         throw ::exception(error_wrong_state, "invalid Vulkan descriptor-set frame index");

      }

      return vkdescriptorseta[iFrameIndex];

   }


   void block::update_frame(gpu::renderer* pgpurenderer)
   {

      auto prendertarget = pgpurenderer->render_target();

      auto iFrameIndex = prendertarget->m_pgpurenderer->m_pgpucontext->m_pgpudevice->get_frame_index3();

      if (iFrameIndex < 0 || iFrameIndex >= m_uboBuffers.size())
      {

         return;
      }

      if (!m_uboBuffers[iFrameIndex])
      {

         return;
      }

      m_uboBuffers[iFrameIndex]->map();

      m_uboBuffers[iFrameIndex]->writeToBuffer(this->data(false));

      m_uboBuffers[iFrameIndex]->flush();

      m_uboBuffers[iFrameIndex]->unmap();

   }


   VkDescriptorBufferInfo block::descriptor_info(::gpu::command_buffer *pgpucommandbuffer)
   {

      return descriptor_info(pgpucommandbuffer->m_iCommandBufferFrameIndex2);

   }


   VkDescriptorBufferInfo block::descriptor_info(::collection::index iFrameIndex)
   {

      if (iFrameIndex < 0 || iFrameIndex >= m_uboBuffers.size() || !m_uboBuffers[iFrameIndex])
      {

         throw ::exception(error_wrong_state, "invalid Vulkan uniform-buffer frame index");

      }

      VkDescriptorBufferInfo bufferinfo;

      auto &pbuffer = m_uboBuffers[iFrameIndex];

      bufferinfo.buffer = pbuffer->m_pbuffer->m_vkbuffer;
      bufferinfo.offset = 0;
      bufferinfo.range = pbuffer->m_pbuffer->m_size;

      return bufferinfo;
   }



} // namespace gpu_vulkan



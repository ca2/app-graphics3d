// Created by camilo on 2025-12-11 08:28 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "binding.h"
#include "block.h"
#include "command_buffer.h"
#include "descriptors.h"
#include "render_target.h"
#include "renderer.h"
#include "app-graphics3d/gpu_vulkan/texture.h"
#include "acme/prototype/prototype/address_object.h"


namespace gpu_vulkan
{


   ::gpu_vulkan::descriptor_set_layout *binding_set::descriptor_set_layout(::gpu::command_buffer *pgpucommandbuffer)
   {

      if (!m_pdescriptorsetlayout1)
      {

         defer_update_binding_set(pgpucommandbuffer);

      }

      return m_pdescriptorsetlayout1;

   }


   void binding_set::defer_update_binding_set(::gpu::command_buffer * pgpucommandbuffer)
   {
      
      if (!m_pdescriptorsetlayout1)
      {

         create_descriptor_set_layout(pgpucommandbuffer);

      }

   }


   void binding_set::create_descriptor_set_layout(::gpu::command_buffer *pgpucommandbuffer)
   {

      auto pcontext = pgpucommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext;

      auto builder = ::gpu_vulkan::descriptor_set_layout::Builder(pcontext);

      for (auto &pbinding: *this)
      {

         auto flags = (VkShaderStageFlags)0;

         if (!pbinding->m_bVertexShader && !pbinding->m_bFragmentShader)
         {
            if (pbinding->is_image_sampler())
            {
               pbinding->m_bFragmentShader = true;
            }
            else
            {
               pbinding->m_bVertexShader = true;
               pbinding->m_bFragmentShader = true;
            }
         }

         if (pbinding->m_bVertexShader)
         {

            flags |= VK_SHADER_STAGE_VERTEX_BIT;
         }

         if (pbinding->m_bFragmentShader)
         {

            flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
         }

         VkDescriptorType type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

         if (pbinding->is_image_sampler())
         {

            type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
         }

         builder.addBinding(pbinding->m_iSlot, type, flags);
      }

      auto pdescriptorsetlayout = builder.build();

      m_pdescriptorsetlayout1 = pdescriptorsetlayout;

   }


   //void binding_set::defer_create_update_descriptor_set(::gpu::command_buffer * pgpucommandbuffer,
   //                                                     ::array<VkDescriptorSet> &descriptorseta,
   //                                                     ::array_base<VkDescriptorBufferInfo> &bufferinfoa)
   //{

   //   for (int i = 0; i < bufferinfoa.size(); i++)
   //   {

   //      descriptor_writer(*m_pdescriptorsetlayout1, *m_pdescriptorpool)
   //         .writeBuffer(0, bufferinfoa[i])
   //         .build(descriptorseta.ø(i));
   //   }

   //}


   binding_slot_set::binding_slot_set()
   {


         }
   binding_slot_set ::~binding_slot_set()
   {



   }



   ::array<VkDescriptorSet> & binding_slot_set::descriptor_set(::gpu::command_buffer * pgpucommandbuffer)
   {

      if (!m_pvkdescriptorseta)
      {

         construct_newø(m_pvkdescriptorseta);

         auto iFrameCount =
            pgpucommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext->m_pgpudevice->get_frame_count();

         m_pvkdescriptorseta->set_size(iFrameCount);

         ::cast<::gpu_vulkan::binding_set> pbindingset = m_pbindingset;

         for (int i = 0; i < iFrameCount; i++)
         {

            ::array_base<address_object < VkDescriptorImageInfo>> vkdescriptorimageinfoa;

            ::array_base<address_object<VkDescriptorBufferInfo>> vkdescriptorbufferinfoa;

            auto pdescriptosetlayout = pbindingset->descriptor_set_layout(pgpucommandbuffer);

            auto pdescriptorpool = this->descriptor_pool(pgpucommandbuffer);

            auto vkdescriptorwriter = descriptor_writer(*pdescriptosetlayout, *pdescriptorpool);

            auto iSlotCount = this->size();

            for (int iSlot = 0; iSlot < iSlotCount; iSlot++)
            {

               auto &bindingslot = this->element_at(iSlot);

               if (bindingslot.m_ptexture)
               {

                  ::cast<::gpu_vulkan::texture> ptexture = bindingslot.m_ptexture;

                  auto &vkdescriptorimageinfo = *vkdescriptorimageinfoa.add_new().m_p;

                  vkdescriptorimageinfo = ptexture->descriptor_info();

               }
               else if (bindingslot.m_pblock)
               {

                  ::cast<::gpu_vulkan::block> pblock = bindingslot.m_pblock;

                  auto &vkdescriptorbufferinfo = *vkdescriptorbufferinfoa.add_new().m_p;

                  vkdescriptorbufferinfo = pblock->descriptor_info(i);

               }

            }

            int iImageInfo = 0;
            int iBufferInfo = 0;

            for (int iSlot = 0; iSlot < iSlotCount; iSlot++)
            {

               auto &bindingslot = this->element_at(iSlot);

               if (bindingslot.m_ptexture)
               {

                  auto &vkdescriptorimageinfo = *vkdescriptorimageinfoa.element_at(iImageInfo++).m_p;

                  vkdescriptorwriter.writeImage(iSlot, &vkdescriptorimageinfo);

               }
               else if (bindingslot.m_pblock)
               {

                  auto &vkdescriptorbufferinfo = *vkdescriptorbufferinfoa.element_at(iBufferInfo++).m_p;

                  vkdescriptorwriter.writeBuffer(iSlot, &vkdescriptorbufferinfo);

               }

            }

            VkDescriptorSet vkdescriptorset = VK_NULL_HANDLE;
               
            vkdescriptorwriter.build(vkdescriptorset);

            (*m_pvkdescriptorseta)[i] = vkdescriptorset;

         }

      }

      return *m_pvkdescriptorseta;

   }


   //void binding_set::defer_create_update_descriptor_set(::gpu::command_buffer * pgpucommandbuffer,
   //                                                  ::array<VkDescriptorSet> & descriptorseta)

   //{



   //   }


//      //m_psetdescriptorlayout =
//      //
//      //      .addBinding(m_bindingUbo.m_uBinding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
//      //                  VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
//      //      .addBinding(m_bindingSampler.m_uBinding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
//      //                  VK_SHADER_STAGE_FRAGMENT_BIT)
//      //      .build();

//      int iFrameCount = m_pgpurenderer->m_pgpurendertarget->get_frame_count();

//      auto pdescriptorpoolbuilder = allocateø::gpu_vulkan::descriptor_pool::Builder();

//      pdescriptorpoolbuilder->initialize_builder(m_pgpurenderer->m_pgpucontext);
//      pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
//      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, iFrameCount * 10);
//      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

//      m_pdescriptorpool = pdescriptorpoolbuilder->build();
//   }
//   else
//   {

//      m_psetdescriptorlayout =
//         ::gpu_vulkan::descriptor_set_layout::Builder(m_pgpurenderer->m_pgpucontext)
//            .addBinding(m_bindingSampler.m_uBinding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
//                        VK_SHADER_STAGE_FRAGMENT_BIT)
//            .build();

//      int iFrameCount = m_pgpurenderer->m_pgpurendertarget->get_frame_count();

//      auto pdescriptorpoolbuilder = allocateø::gpu_vulkan::descriptor_pool::Builder();

//      pdescriptorpoolbuilder->initialize_builder(m_pgpurenderer->m_pgpucontext);
//      pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
//      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

//      m_pdescriptorpool = pdescriptorpoolbuilder->build();
//   }
//}
// else if (m_bindingSampler.is_set())
//{

//   {

//      m_psetdescriptorlayout =
//         ::gpu_vulkan::descriptor_set_layout::Builder(m_pgpurenderer->m_pgpucontext)
//            .addBinding(m_bindingSampler.m_uBinding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
//                        VK_SHADER_STAGE_FRAGMENT_BIT)
//            .build();

//      int iFrameCount = m_pgpurenderer->m_pgpurendertarget->get_frame_count();

//      auto pdescriptorpoolbuilder = allocateø::gpu_vulkan::descriptor_pool::Builder();

//      pdescriptorpoolbuilder->initialize_builder(m_pgpurenderer->m_pgpucontext);
//      pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
//      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

//      m_pdescriptorpool = pdescriptorpoolbuilder->build();
//   }
//}
// else if (m_bindingCubeSampler.is_set())
//{

//   {

//      m_psetdescriptorlayout =
//         ::gpu_vulkan::descriptor_set_layout::Builder(m_pgpurenderer->m_pgpucontext)
//            .addBinding(m_bindingCubeSampler.m_uBinding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
//                        VK_SHADER_STAGE_FRAGMENT_BIT)
//            .build();

//      int iFrameCount = m_pgpurenderer->m_pgpurendertarget->get_frame_count();

//      auto pdescriptorpoolbuilder = allocateø::gpu_vulkan::descriptor_pool::Builder();

//      pdescriptorpoolbuilder->initialize_builder(m_pgpurenderer->m_pgpucontext);
//      pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
//      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

//      m_pdescriptorpool = pdescriptorpoolbuilder->build();
//   }
//}

//_create_pipeline_layout((int)m_propertiesPush.m_memory.size());

// ::cast < context > pgpucontext = m_pgpurenderer->m_pgpucontext;
//
// ::cast < device > pgpudevice = pgpucontext->m_pgpudevice;


// for (int i = 0; i < this->size(); i++)
//{


//
//      .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
//      .build();
//}


   ::gpu_vulkan::descriptor_pool *binding_slot_set::descriptor_pool(::gpu::command_buffer *pgpucommandbuffer)
   {

         if (!m_pdescriptorpool)
         {

            defer_update_binding_slot_set(pgpucommandbuffer);
         }

         return m_pdescriptorpool;
      }


      void binding_slot_set::defer_update_binding_slot_set(::gpu::command_buffer *pgpucommandbuffer)
      {
         // pdescriptorsetlayout->m_iIndex = pbindingset.m_iSet;

         if (m_pdescriptorpool)
         {
            
            return;

         }

         auto pgpucontext = pgpucommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext;

         auto pdescriptorpoolbuilder = allocateø::gpu_vulkan::descriptor_pool::Builder();

         int iFrameCount =
            pgpucommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext->m_pgpudevice->get_frame_count();

         pdescriptorpoolbuilder->initialize_builder(pgpucontext);

         ::collection::count iMaxSets = iFrameCount * m_pbindingset->size() * 2;
         pdescriptorpoolbuilder->setMaxSets(iMaxSets);

         ::collection::count iUniformBuffer = iFrameCount * m_pbindingset->uniform_buffer_count() * 2;
         if (iUniformBuffer > 0)
         {
            pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, iUniformBuffer);
         }
         ::collection::count iImageSampler = iFrameCount * m_pbindingset->image_sampler_count() * 2;
         if (iImageSampler > 0)
         {
            pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iImageSampler);
         }

         auto pdescriptorpool = pdescriptorpoolbuilder->build();

         // pdescriptorpool->m_iIndex = pbindingset.m_iSet;

         // m_descriptorpoola.ø(pdescriptorpool->m_iIndex) = pdescriptorpool;

         m_pdescriptorpool = pdescriptorpool;

      }


} // namespace gpu_vulkan

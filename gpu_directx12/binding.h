//
// From gpu_directx11/binding.h by camilo on 2026-01-08 01:03 <3ThomasBorregaardSørensen!!
// From gpu_vulkan/binding.h by camilo on 2025-12-11 17:26 <3ThomasBorregaardSørensen!!
//
#pragma once


#include "bred/gpu/binding.h"


namespace gpu_directx12
{

   class binding : virtual public ::gpu::binding
   {
   public:
   };


   class binding_set : virtual public ::gpu::binding_set
   {
   public:


      int m_iXYZ_Index = -1;
      int m_iXYZ_IndexSampler = -1;

      //::array_base < ::comptr<ID3D12DescriptorHeap> > m_heapa1;
      //::array_base < ::comptr<ID3D12DescriptorHeap> > m_heapaSampler1;
      //int m_iHeapIndex = 0;
      //int m_iHeapSamplerIndex = 0;

      // ::pointer < ::gpu_vulkan::descriptor_set_layout > m_pdescriptorsetlayout1;
      // ::pointer <::gpu_vulkan::descriptor_pool> m_pdescriptorpool;


      // virtual ::gpu_vulkan::descriptor_set_layout *descriptor_set_layout(::gpu::command_buffer * pgpucommandbuffer);
      //
      // virtual void defer_update_binding_set(::gpu::command_buffer *pgpucommandbuffer);
      //
      // virtual void create_descriptor_set_layout(::gpu::command_buffer *pgpucommandbuffer);
      //
      //
      // virtual void defer_create_update_descriptor_set(::gpu::command_buffer *pgpucommandbuffer,
      //                                                 ::array_base<VkDescriptorSet> &descriptorseta,
      //                                                 ::array_base<VkDescriptorBufferInfo> &bufferinfoa);
      //
      // virtual void defer_create_update_descriptor_set(::gpu::command_buffer *pgpucommandbuffer,
      //                                                 ::array_base<VkDescriptorSet> &descriptorseta);
   };


   // class CLASS_DECL_BRED binding_set_array : virtual public ::pointer_array<binding_set>
   //{
   // public:
   //    bool has_global_ubo() const;
   // };


} // namespace gpu_directx12

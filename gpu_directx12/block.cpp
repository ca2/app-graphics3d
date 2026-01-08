// From gpu_directx11/block.cpp by camilo on 2025-12-11 19:04 <3ThomasBorregaardSørensen!!
#include "framework.h"
//#include "binding.h"
#include "block.h"
//#include "command_buffer.h"
#include "context.h"
//#include "render_target.h"
#include "renderer.h"


namespace gpu_directx12
{


   block::block() {}


   block::~block() {}


   void block::create_gpu_block(::gpu::context *pgpucontext)
   {

      ::gpu::block::create_gpu_block(pgpucontext);

   }


   ID3D12Resource * block::resource()
   {

      if (m_presource)
      {

         return m_presource;

      }

      int iBufferSize = this->size(false);

      ::cast < renderer > prenderer = m_pgpucontext->m_pgpurenderer;

      ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

      CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);

      CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(
         ::directx12::Align256(iBufferSize));

      pgpudevice->m_pdevice->CreateCommittedResource(
         &heapProps,
         D3D12_HEAP_FLAG_NONE,
         &bufferDesc,
         D3D12_RESOURCE_STATE_GENERIC_READ,
         nullptr,
         __interface_of(m_presource));

      D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};

      cbvDesc.BufferLocation = m_presource->GetGPUVirtualAddress();

      cbvDesc.SizeInBytes = ::directx12::Align256(iBufferSize); // must be 256-byte aligned

      auto handle = prenderer->m_pheapCbv->GetCPUDescriptorHandleForHeapStart();

      pgpudevice->m_pdevice->CreateConstantBufferView(&cbvDesc, handle);

      CD3DX12_RANGE readRange(0, 0);

      m_presource->Map(
         0, &readRange,
         &m_pData);

      return m_presource;

   }


//    VkDescriptorSet block::descriptor_set(::gpu::binding_set_pointer pgpubindingset,
//                                          ::gpu::command_buffer *pgpucommandbuffer)
//    {
//
//       if (m_vkdescriptorset)
//       {
//
//          return m_vkdescriptorset;
//
//       }
//
// //      VkDescriptorBufferInfo bufferinfo;
//
//       ::cast<::gpu_vulkan::context> pcontext = m_pgpucontext;
//
//       //bufferinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//       //bufferinfo.imageView = get_image_view();
//       //bufferinfo.sampler = pcontext->_001VkSampler();
//
//       unsigned int uSamplerBinding = 0;
//
//       //auto pgpubindingset = pshader->get_first_image_sampler_binding_set();
//
//       ::cast<::gpu_vulkan::binding_set> pbindingset = pgpubindingset.m_pbindingset;
//
//       auto playout = pbindingset->descriptor_set_layout(pgpucommandbuffer);
//
//       auto ppool = pbindingset->m_pdescriptorpool;
//
//       auto iFrameIndex = pgpucommandbuffer->m_pgpurendertarget->get_frame_index();
//
//       auto bufferInfo = m_uboBuffers[iFrameIndex]->descriptorInfo();
//
//       //   descriptor_writer(*globalSetLayout, *m_pglobalpool)
//       //      .writeBuffer(0, &bufferInfo)
//       //      .build(globalDescriptorSets[i]);
//       ::gpu_vulkan::descriptor_writer(*playout, *ppool)
//          .writeBuffer(uSamplerBinding, &bufferInfo)
//          .build(m_vkdescriptorset);
//
//       return m_vkdescriptorset;
//
//    }


   void block::update_frame(::gpu::renderer * pgpurenderer)
   {

      auto pgpurendertarget = pgpurenderer->render_target();

         auto iFrameIndex = pgpurendertarget->get_frame_index();


         //MyGlobalData globalData = { /* your values */ };
         ::cast < renderer > prenderer = pgpurenderer;
         //      UINT8* mappedPtr = nullptr;
         //    D3D12_RANGE readRange = {}; // no read access
         //  m_uboBuffers[iFrameIndex]->m_presourceBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mappedPtr));
         //memcpy(mappedPtr, block.data(), block.size());


         resource();

         auto pData = m_pData;
         memcpy(pData, this->data(false), this->size(false));


         //m_uboBuffers[iFrameIndex]->m_presourceBuffer->Unmap(0, nullptr);

         //m_uboBuffers[iFrameIndex]->writeToBuffer(block.data());

         //m_uboBuffers[iFrameIndex]->flush();

         //ID3D11Buffer* globalUBOBuffer = nullptr;
         //D3D11_BUFFER_DESC cbd = {};
         //cbd.Usage = D3D11_USAGE_DYNAMIC;
         //cbd.ByteWidth = block;
         //cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
         //cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

         //device->CreateBuffer(&cbd, nullptr, &globalUBOBuffer);
         //if (m_pbufferGlobalUbo)
         //{

         //   D3D11_MAPPED_SUBRESOURCE mapped;
         //   m_pcontext->Map(m_pbufferGlobalUbo, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
         //   memcpy(mapped.pData, block.data(), block.size());
         //   m_pcontext->Unmap(m_pbufferGlobalUbo, 0);


         //}

         //m_pbufferGlobalUbo
         //m_pbufferGlobalUbo

   }


} // namespace gpu_directx12



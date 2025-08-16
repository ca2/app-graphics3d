// Created by camilo on 2025-08-16 01:27 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "context.h"
#include "device.h"
#include "frame_storage.h"
#include "memory_buffer.h"


namespace gpu_directx12
{


   frame_storage::frame_storage()
   {


   }


   frame_storage::~frame_storage()
   {


   }


   void frame_storage::map_allocate(::gpu::memory_buffer* pgpumemorybuffer, int size)
   {

      if (size > m_iBufferSize)
      {

         throw ::exception(error_wrong_state);

      }

      bool bDiscard;

      if (size > m_iBufferSize - m_iBufferOffset)
      {

         bDiscard = true;

         m_iBuffer++;

      }
      else
      {

         bDiscard = false;

      }

      auto& resource = m_resourcea.ø(m_iBuffer);

      if (!resource.m_presource)
      {

         ::cast < ::gpu_directx12::device > pgpudevice = m_pgpudevice;

         auto pd3d12device = pgpudevice->m_pdevice;

         UINT bufSize = m_iBufferSize; // number of vertices the buffer can hold

         // Describe heap (UPLOAD so CPU can write directly)
         D3D12_HEAP_PROPERTIES heapProps = {};
         heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

         D3D12_RESOURCE_DESC desc = {};
         desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
         desc.Width = bufSize;
         desc.Height = 1;
         desc.DepthOrArraySize = 1;
         desc.MipLevels = 1;
         desc.SampleDesc.Count = 1;
         desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

         // Create upload buffer
         pd3d12device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            __interface_of(resource.m_presource));

         // Persistent map
         resource.m_presource->Map(0, nullptr, reinterpret_cast<void**>(&resource.m_pMap));

         //m_currentOffset = 0;

         bDiscard = true;

      }

      //m_iBufferOffset = m_iBufferOffset;

      //m_pmodelbuffer->m_strDebugString.formatf("24*th:%d", m_iBufferOffset / 24);

      //m_pbuffer = pbuffer;
      //buffer buffer;

      ::cast < memory_buffer > pmemorybuffer = pgpumemorybuffer;

      pmemorybuffer->m_presource = resource.m_presource;
      pmemorybuffer->m_iBufferOffset = m_iBufferOffset;
      pmemorybuffer->m_iSizeMapped = size;
      pmemorybuffer->m_pMap = ((unsigned char*)resource.m_pMap) + m_iBufferOffset;

      m_iBufferOffset += size;

      //return buffer;

   }



} // namespace gpu_directx12




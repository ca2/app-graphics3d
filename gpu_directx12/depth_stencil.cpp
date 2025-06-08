// Created by camilo on 2025-06-08 20:00 < 3ThomasBorregaardSørensen!!
#include "framework.h"
#include "depth_stencil.h"
#include "renderer.h"


namespace gpu_directx12
{


   depth_stencil::depth_stencil()
   {

      //new_depth_stencil.set_new_depth_stencil();

   }


   depth_stencil::~depth_stencil()
   {

   }


   void depth_stencil::initialize_depth_stencil(::gpu::renderer* prenderer, const ::int_size& size)
   {

      m_prenderer = prenderer;

      ::cast < ::gpu_directx12::device > pdevice = prenderer->m_pgpucontext->m_pgpudevice;

      D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
      dsvHeapDesc.NumDescriptors = 1;
      dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
      dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
      pdevice->m_pdevice->CreateDescriptorHeap(&dsvHeapDesc, __interface_of(m_pheap));

      // 2. Describe depth stencil resource
      D3D12_RESOURCE_DESC depthDesc = {};
      depthDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
      depthDesc.Width = size.width();
      depthDesc.Height = size.height();
      depthDesc.MipLevels = 1;
      depthDesc.DepthOrArraySize = 1;
      depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
      depthDesc.SampleDesc.Count = 1;
      depthDesc.SampleDesc.Quality = 0;
      depthDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
      depthDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

      D3D12_CLEAR_VALUE depthClearValue = {};
      depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;
      depthClearValue.DepthStencil.Depth = 1.0f;
      depthClearValue.DepthStencil.Stencil = 0;

      CD3DX12_HEAP_PROPERTIES heapproperties(D3D12_HEAP_TYPE_DEFAULT);

      // 3. Create depth stencil resource
      pdevice->m_pdevice->CreateCommittedResource(
         &heapproperties,
         D3D12_HEAP_FLAG_NONE,
         &depthDesc,
         D3D12_RESOURCE_STATE_DEPTH_WRITE,
         &depthClearValue,
         __interface_of(m_presource));

      // 4. Create DSV
      D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
      dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
      dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
      dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

      m_handle = m_pheap->GetCPUDescriptorHandleForHeapStart();

      pdevice->m_pdevice->CreateDepthStencilView(m_presource, &dsvDesc, m_handle);


   }


} // namespace gpu_directx12





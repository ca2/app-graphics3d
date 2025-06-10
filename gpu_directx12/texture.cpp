// Created by camilo on 2025-06-08 18:14 < 3ThomasBorregaardSørensen!!
#include "framework.h"
#include "texture.h"
#include "renderer.h"


namespace gpu_directx12
{


   texture::texture()
   {

      new_texture.set_new_texture();

   }


   texture::~texture()
   {

   }


   void texture::initialize_texture(::gpu::renderer* prenderer, const ::int_size& size) //, bool bCreateRenderTargetView, bool bCreateShaderResourceView)
   {

      m_prenderer = prenderer;

      DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM;
      // 1. Create the texture resource
      D3D12_RESOURCE_DESC textureDesc = {};
      textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
      textureDesc.Width = size.width();
      textureDesc.Height = size.height();
      textureDesc.MipLevels = 1;
      textureDesc.DepthOrArraySize = 1;
      textureDesc.Format = format;
      textureDesc.SampleDesc.Count = 1;
      textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
      textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

      D3D12_CLEAR_VALUE clearValue = {};
      clearValue.Format = format;
      clearValue.Color[0] = 0.5f *0.5f;
      clearValue.Color[1] = 0.95f * 0.5f;
      clearValue.Color[2] = 0.9f * 0.5f;
      clearValue.Color[3] = 0.5f;

      //clearValue.Color = { 0.5f, 0.75f, 0.9f, 0.5f };

      ::cast < ::gpu_directx12::device > pdevice = prenderer->m_pgpucontext->m_pgpudevice;

      CD3DX12_HEAP_PROPERTIES heapproperties(D3D12_HEAP_TYPE_DEFAULT);

      HRESULT hrCreateCommittedResource = pdevice->m_pdevice->CreateCommittedResource(
         &heapproperties,
         D3D12_HEAP_FLAG_NONE,
         &textureDesc,
         D3D12_RESOURCE_STATE_RENDER_TARGET,
         &clearValue,
         __interface_of(m_presource));

      pdevice->defer_throw_hresult(hrCreateCommittedResource);

      m_estate = D3D12_RESOURCE_STATE_RENDER_TARGET;

      new_texture.set_new_texture();

      //if (bCreateRenderTargetView)
      //{

      //   //// 2. Create RTV descriptor heap
      //   //D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
      //   //rtvHeapDesc.NumDescriptors = 1;
      //   //rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
      //   //rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
      //   //HRESULT hrCreateDescriptorHeap = pdevice->m_pdevice->CreateDescriptorHeap(&rtvHeapDesc, __interface_of(m_pheapRenderTargetView));

      //   //pdevice->defer_throw_hresult(hrCreateDescriptorHeap);

      //   //// 3. Create RTV
      //   //m_handleRenderTargetView = m_pheapRenderTargetView->GetCPUDescriptorHandleForHeapStart();
      //   CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

      //   pdevice->m_pdevice->CreateRenderTargetView(m_presource, nullptr, m_handleRenderTargetView);

      //}

      //if (bCreateShaderResourceView)
      //{

      //   //// 4. Create SRV descriptor heap
      //   //D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
      //   //srvHeapDesc.NumDescriptors = 1;
      //   //srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
      //   //srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
      //   //
      //   //HRESULT hrCreateDescriptorHeap = pdevice->m_pdevice->CreateDescriptorHeap(&srvHeapDesc, __interface_of(m_pheapShaderResourceView));

      //   //pdevice->defer_throw_hresult(hrCreateDescriptorHeap);

      //   //// 5. Create SRV
      //   //D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
      //   //srvDesc.Format = format;
      //   //srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
      //   //srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
      //   //srvDesc.Texture2D.MostDetailedMip = 0;
      //   //srvDesc.Texture2D.MipLevels = 1;

      //   //m_handleShaderResourceView = m_pheapShaderResourceView->GetCPUDescriptorHandleForHeapStart();

      //   //pdevice->m_pdevice->CreateShaderResourceView(m_presource, &srvDesc, m_handleShaderResourceView);

      //}

   }


   void texture::_new_state(ID3D12GraphicsCommandList* pcommandlist, D3D12_RESOURCE_STATES estateNew)
   {

      if (m_estate != estateNew)
      {

         // Transition to copy source
         D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            m_presource,
            m_estate, // Adjust if needed
            estateNew
         );

         pcommandlist->ResourceBarrier(1, &barrier);

         m_estate = estateNew;

         //pcommandBuffer->submit_command_buffer();

      }

   }


} // namespace gpu_directx12





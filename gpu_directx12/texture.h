// Created by camilo on 2025-06-08 18:11 < 3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/texture.h"
#include <d3d11.h>
#include <d2d1_1.h>


namespace gpu_directx12
{


   class CLASS_DECL_GPU_DIRECTX12 texture :
      virtual public ::gpu::texture
   {
   public:


      struct
      {

         int m_bForOnAfterDoneFrameStep : 1;

         void set_new_texture()
         {
            memset(this, 0xff, sizeof(*this));

         }


      }new_texture;

      
      ::comptr<ID3D12Resource>         m_presource;
      ::comptr<ID3D12Resource>         m_presourceDepthStencilView;
      ::comptr<ID3D12DescriptorHeap>   m_pheapRenderTargetView;
      D3D12_RESOURCE_STATES            m_estate;
      D3D12_CPU_DESCRIPTOR_HANDLE      m_handleRenderTargetView;
      ::comptr<ID3D12DescriptorHeap>   m_pheapShaderResourceView;
      D3D12_CPU_DESCRIPTOR_HANDLE      m_handleShaderResourceView;
      ::comptr<ID3D12DescriptorHeap>   m_pheapDepthStencilView;
      D3D12_CPU_DESCRIPTOR_HANDLE      m_handleDepthStencilView;
      ::comptr<ID3D12DescriptorHeap>   m_pheapSampler;
      D3D12_CPU_DESCRIPTOR_HANDLE      m_handleSampler;
      //UINT m_rtvDescriptorSize;
      //bool m_bRenderTarget;
      bool m_bShaderResource;

      class d3d11 :
         virtual public ::particle
      {
      public:

         ID3D11Resource* m_d3d11wrappedresources[1];

         //::comptr<ID3D12Resource> d3d12Resource;
         ::comptr<ID3D11Resource> wrappedResource;
         ::comptr<IDXGISurface> dxgiSurface;
         ::comptr<ID2D1Bitmap1> d2dBitmap;
         HANDLE sharedHandle = NULL;

         d3d11() {}
         ~d3d11() override {}



      };


      ::pointer < d3d11 > m_pd3d11;

      texture();
      ~texture() override;


      void initialize_gpu_texture(::gpu::renderer* prenderer, const ::int_rectangle & rectangle) override;

      void _new_state(ID3D12GraphicsCommandList* pcommandlist, D3D12_RESOURCE_STATES estate);

      class d3d11* d3d11();

      void blend(::gpu::texture* ptexture) override;


      void create_render_target();

      void create_shader_resource();

      void create_depth_resources();

      virtual void _initialize_gpu_texture(::gpu::renderer* prenderer, UINT uCurrentBufferIndex, IDXGISwapChain3* pdxgiswapchain);

   };


   class texture_guard
   {
   public:

      texture* m_ptexture;
      ID3D12GraphicsCommandList* m_pcommandlist;
      D3D12_RESOURCE_STATES m_estateOld;

      texture_guard(ID3D12GraphicsCommandList* pcommandlist, texture* ptexture, D3D12_RESOURCE_STATES estate)
      {

         m_ptexture = ptexture;
         m_pcommandlist = pcommandlist;
         m_estateOld = m_ptexture->m_estate;

         m_ptexture->_new_state(m_pcommandlist, estate);

      }

      ~texture_guard()
      {

         m_ptexture->_new_state(m_pcommandlist, m_estateOld);

      }

   };



} // namespace gpu_directx12




// Created by camilo on 2025-06-04 23:08 <3ThomasBorregaardSørensen!!
#pragma once


#include "direct2d/_.h"
#include "direct2d/swap_chain.h"
#include "draw2d_direct2d/_.h"
#include "draw2d_direct2d/swap_chain.h"
#include "gpu_directx12/swap_chain.h"
#include "gpu_directx12/texture.h"


#include <d3d11on12.h>


namespace gpu_directx12
{


   class CLASS_DECL_GPU_DIRECTX12 direct2d_draw2d_swap_chain :
      virtual public ::draw2d_direct2d::swap_chain,
      virtual public ::gpu_directx12::swap_chain
   {
   public:




      //::pointer < swap_chain_texture > m_ptextureRenderTargetBackBuffer;
      //::comptr<ID3D12Resource> m_pd3d12resourceBackBuffer;
      //::comptr<ID3D11Resource> m_pd3d11resourceWrapped;

      //::comptr<IDXGISurface> m_pdxgisurface;

      //::comptr<ID2D1Bitmap1> m_pd2dbitmap;

      ::comptr<ID2D1DeviceContext> d2dContext;


      ::pointer < ::gpu_directx12::device > m_pgpudevice;

      //::comptr<ID3D11Device> m_pd3d11device;
      //::comptr<ID3D11DeviceContext> m_pd3d11context;
      ::comptr<ID3D11On12Device> m_pd3d11on12;
      //::comptr <ID3D11Resource > m_presourceWrappedRTV;
      //::comptr<IDXGISurface> m_pdxgisurface;
      //::comptr<ID3D12CommandQueue>                                m_pcommandqueue;
      //::pointer < ::draw2d_direct2d::graphics > m_pgraphics;
      //::pointer < ::gpu_directx12::context > m_pgpucontext;
      //::comptr <IDXGIDevice> m_pdxgidevice;
      //ID3D11Resource* resources[1];
      //::comptr<ID2D1Device> m_pd2d1device;
      //::comptr<ID2D1Bitmap1> m_pd2d1bitmap;
      //::comptr<ID2D1DeviceContext> m_pd2d1devicecontext;

      //::comptr<ID3D11Texture2D> m_ptextureShared;
      //VkSwapchainKHR       m_vkswapchain;
      //uint32_t             currentImageIndex;
      //::int_size m_size;

      //::comptr<ID3D11SamplerState> m_psamplerstateLinear;
      // ::comptr<ID3D11ShaderResourceView> m_pshaderresourceviewShader;
      //::pointer < ::gpu_directx12::shader > m_pshaderSwapChainOutput;
      ::comptr<ID3D12DescriptorHeap> m_srvHeap;
      ::comptr<ID3D12DescriptorHeap> m_samplerHeap;

      ::comptr<ID3D12Resource> m_renderTargets[2];
      ::comptr<ID3D12CommandAllocator> m_commandAllocator;
      //::comptr<ID3D12CommandQueue> m_commandQueue;
      ::comptr<ID3D12RootSignature> m_rootSignature;
      ::comptr<ID3D12DescriptorHeap> m_rtvHeap;
      ::comptr<ID3D12PipelineState> m_pipelineState;
      ::comptr<ID3D12GraphicsCommandList> m_commandList;
      ::comptr<ID3D12Resource> m_vertexBuffer;
      D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
      UINT m_srvDescriptorSize = 0;

      // Synchronization objects.
      UINT m_frameIndex;
      HANDLE m_fenceEvent;
      ::comptr<ID3D12Fence> m_fence;
      UINT64 m_fenceValue;

      direct2d_draw2d_swap_chain();
      ~direct2d_draw2d_swap_chain() override;


      void initialize_gpu_swap_chain(::gpu::device* pgpudevice, ::windowing::window* pwindow) override;

      void on_before_begin_draw_frame(::draw2d_gpu::graphics* pgraphics) override;
      void on_after_end_draw_frame(::draw2d_gpu::graphics* pgraphics) override;

      ::gpu_directx12::texture* current_texture() override;
      ::gpu_directx12::depth_stencil* current_depth_stencil() override;
      //virtual void _defer_d3d11on12_wrapped_resources();
      //void initialize_direct2d_draw2d_gpu_swap_chain(::gpu::device* pgpudevice, ::windowing::window* pwindow) override;


      void endDraw(::draw2d_gpu::graphics* pgraphics, ::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc) override;


      void present() override;

      virtual void _create_offscreen_graphics_for_swap_chain_blitting(::gpu_directx12::context *pcontext, ::draw2d_direct2d::graphics* pgraphics, const ::int_size& size = {});

   };


} // namespace gpu_directx12



// Created by camilo on 2025-06-04 23:08 <3ThomasBorregaardSørensen!!
#pragma once


#include "direct2d/_.h"
#include "direct2d/swap_chain.h"
#include "draw2d_direct2d/_.h"
#include "draw2d_direct2d/swap_chain.h"


#include <d3d11on12.h>


namespace gpu_directx12
{


   class CLASS_DECL_GPU_DIRECTX12 direct2d_draw2d_swap_chain :
      virtual public ::draw2d_direct2d::swap_chain
   {
   public:

      ::pointer < ::gpu_directx12::device > m_pgpudevice;

      //::comptr<ID3D11Device> m_pd3d11device;
      //::comptr<ID3D11DeviceContext> m_pd3d11context;
      ::comptr<ID3D11On12Device> m_pd3d11on12;
      ::comptr <ID3D11Resource > m_presourceWrappedRTV;
      //::comptr<IDXGISurface> m_pdxgisurface;
      ::comptr<ID3D12CommandQueue>                                m_pcommandqueue;
      //::pointer < ::draw2d_direct2d::graphics > m_pgraphics;
      //::pointer < ::gpu_directx12::context > m_pgpucontext;
      //::comptr <IDXGIDevice> m_pdxgidevice;
      ID3D11Resource* resources[1];
      //::comptr<ID2D1Device> m_pd2d1device;
      //::comptr<ID2D1Bitmap1> m_pd2d1bitmap;
      //::comptr<ID2D1DeviceContext> m_pd2d1devicecontext;

      ::comptr<IDXGISwapChain> m_pdxgiswapchain;

      ::comptr<ID3D11Texture2D> m_ptextureShared;
      //VkSwapchainKHR       m_vkswapchain;
      //uint32_t             currentImageIndex;
      //::int_size m_size;

      ::comptr<ID3D11SamplerState> m_psamplerstateLinear;
      ::comptr<ID3D11ShaderResourceView> m_pshaderresourceviewShader;



      direct2d_draw2d_swap_chain();
      ~direct2d_draw2d_swap_chain() override;


      void initialize_direct2d_draw2d_gpu_swap_chain(::gpu::device * pgpudevice, ::windowing::window * pwindow) override;



      void endDraw(::draw2d_gpu::graphics* pgraphics, ::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc) override;


   };


} // namespace gpu_directx12



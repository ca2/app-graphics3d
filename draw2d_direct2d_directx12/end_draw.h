// Created by camilo on 2025-06-04 23:08 <3ThomasBorregaardSørensen!!
#pragma once


#include "aura/graphics/gpu/end_draw.h"
//#include "direct2d_directx12/graphics.h"

#include <d3d11on12.h>


namespace draw2d_direct2d_directx12
{


   class CLASS_DECL_DRAW2D_DIRECT2D_DIRECTX12 swap_chain_end_draw :
      virtual public ::draw2d_gpu::end_draw
   {
   public:



      ::comptr<ID3D11Device> m_pd3d11device;
      ::comptr<ID3D11DeviceContext> m_pd3d11context;
      ::comptr<ID3D11On12Device> m_pd3d11on12;
      ::comptr <ID3D11Resource > m_presourceWrappedRTV;
      ::comptr<IDXGISurface> m_pdxgisurface;
      ::comptr<ID3D12CommandQueue>                                m_pcommandqueue;
      ::pointer < ::draw2d_direct2d_directx11::graphics > m_pgraphics;
      ::pointer < ::gpu_directx12::context > m_pgpucontext;
      ::comptr<ID2D1DeviceContext> m_pd2d1devicecontext;
      ::comptr <IDXGIDevice> m_pdxgidevice;
      //::comptr <IDXGIDevice> m_pdxgidevice;
      ID3D11Resource* resources[1];
      ::comptr<ID2D1Device> m_pd2d1device;

      ::comptr<ID2D1DeviceContext> m_pd2d1context;

      //::comptr<IDXGISurface> m_pdxgisurface;

      ::comptr<ID2D1Bitmap1> m_pd2d1bitmap;

      //::pointer < ::gpu_directx12::shader > m_pshader;

      swap_chain_end_draw();
      ~swap_chain_end_draw() override;


      void initialize_end_draw(::draw2d_gpu::graphics * pgraphics) override;



      void endDraw(::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc) override;


   };


} // namespace draw2d_direct2d_directx12




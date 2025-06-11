// From application_object by camilo on 2025-05-17 01:10 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "buffer.h"
#include "engine.h"
#include "frame.h"
#include "input.h"
#include "offscreen_render_pass.h"
#include "swap_chain_render_pass.h"
#include "aura/platform/application.h"
#include "aura/user/user/graphics3d.h"
#include "gpu_directx12/approach.h"
#include "gpu_directx12/context.h"
#include "gpu_directx12/descriptors.h"
#include "gpu_directx12/direct2d_draw2d_swap_chain.h"
#include "gpu_directx12/offscreen_render_target_view.h"
#include "gpu_directx12/renderer.h"
#include "gpu_directx12/texture.h"
#include "draw2d_direct2d/_.h"
//#include "direct2d/draw2d_connector.h"
#include "draw2d_direct2d/graphics.h"
#include "aura/graphics/graphics3d/camera.h"
#include "aura/graphics/graphics3d/scene.h"
#include "aura/windowing/window.h"
//#include "aura/graphics/graphics3d/system/simple_render_system.h"
//#include "aura/graphics/graphics3d/system/point_light_system.h"
#include "acme/platform/application.h"
#include "apex/database/client.h"
#include "apex/database/stream.h"
//#include "cube/container.h"
#include <chrono>


namespace graphics3d_directx12
{


   void engine::on_after_done_frame_step2(::draw2d::graphics_pointer& pgraphics)
   {

      ::cast < ::draw2d_direct2d::graphics > pgraphics2d = pgraphics;

      if (!pgraphics2d)
      {

         return;

      }

      ::cast< ::gpu_directx12::context > pgpucontext = m_pgpucontext;
      ::cast< ::gpu_directx12::renderer > prenderer = pgpucontext->m_pgpurenderer;
      auto prendertargetview = prenderer->m_prendertargetview;
      ::cast < ::gpu_directx12::offscreen_render_target_view > poffscreenrendertargetview = prendertargetview;
      ::cast< ::gpu_directx12::device > pgpudevice = pgpucontext->m_pgpudevice;

      //::cast < ::draw2d_direct2d_directx12::swap_chain_end_draw > penddraw = pgraphics2d->m_penddraw;

      //ID3D11Device* device = pgpudevice->m_pdevice;
      //ID3D11DeviceContext* context = pgpucontext->m_pcontext;
      auto offscreenTexture = poffscreenrendertargetview->current_texture()->m_presource.m_p;
      //if (!device || !context || !offscreenTexture)
      if (!offscreenTexture)
      {
         throw ::exception(error_wrong_state);
      }

      auto pswapchain = pgpudevice->m_pswapchain;
      if (!pswapchain)
      {

         throw ::exception(error_wrong_state);
      }

      if (poffscreenrendertargetview->current_texture()->new_texture.m_bForOnAfterDoneFrameStep)
      {

         if (m_presourceWrappedD3D11Resource)
         {

            // Tell D3D11On12 we’re done using the wrapped resource
            ID3D11Resource* resources[] = { m_presourceWrappedD3D11Resource.m_p };
            pswapchain->m_pd3d11on12->ReleaseWrappedResources(resources, _countof(resources));

         }

         poffscreenrendertargetview->current_texture()->new_texture.m_bForOnAfterDoneFrameStep = false;

         D3D11_RESOURCE_FLAGS d3d11Flags = {};
         d3d11Flags.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

         ::defer_throw_hresult(pswapchain->m_pd3d11on12->CreateWrappedResource(
            offscreenTexture,
            &d3d11Flags,
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT,
            __interface_of(m_presourceWrappedD3D11Resource)
         ));
         //ComPtr<IDXGISurface> dxgiSurface;
         ::defer_throw_hresult(m_presourceWrappedD3D11Resource.as(m_pdxgisurface));
         D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
         );

         ::defer_throw_hresult(pswapchain->m_pd2d1devicecontext->CreateBitmapFromDxgiSurface(
            m_pdxgisurface, &bitmapProps, &m_pd2dbitmap));

      }

      pswapchain->m_pd2d1devicecontext->SetTarget(m_pd2dbitmap);
      pswapchain->m_pd2d1devicecontext->BeginDraw();

      /*ComPtr<ID2D1Bitmap1> d2dBitmap;
      ThrowIfFailed(d2dContext->CreateBitmapFromDxgiSurface(
         dxgiSurface.Get(), &bitmapProps, &d2dBitmap));

      d2dContext->SetTarget(d2dBitmap.Get());
      d2dContext->BeginDraw();*/


      auto puserinteration = pgraphics2d->m_puserinteraction;

      auto r = pgpucontext->m_rectangle;


      ::int_rectangle rHost = r;
      if (puserinteration)
      {

         auto pwindow = puserinteration->window();

         if (pwindow)
         {

            rHost = pwindow->get_window_rectangle();

         }

      }
      int iBottom = r.bottom();

      int iHostBottom = rHost.height();

      int iTop = r.top();

      int iNewTop = iHostBottom - iBottom;
      // Example draw:
      pswapchain->m_pd2d1devicecontext->DrawImage(
         m_pd2dbitmap,
         D2D1::Point2F(0.f, 0.f),
         D2D1::RectF(r.left(), iNewTop, r.width(), r.height()),
         D2D1_INTERPOLATION_MODE_LINEAR,
         D2D1_COMPOSITE_MODE_SOURCE_OVER);

      ::defer_throw_hresult(pswapchain->m_pd2d1devicecontext->EndDraw());


      pswapchain->m_pd3d11context->Flush();

      //D3D11_TEXTURE2D_DESC texDesc = {};
      //texDesc.Width = width;
      //texDesc.Height = height;
      //texDesc.MipLevels = 1;
      //texDesc.ArraySize = 1;
      //texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      //texDesc.SampleDesc.Count = 1;
      //texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
      //texDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;

      // ... Create texture using device->CreateTexture2D

      //   // 2. Wrap the texture in a DXGI surface
      //   comptr<IDXGISurface> dxgiSurface;
      //   offscreenTexture->QueryInterface(IID_PPV_ARGS(&dxgiSurface));

      //   // 3. Create the Direct2D bitmap
      //   D2D1_BITMAP_PROPERTIES1 bitmapProps =
      //      D2D1::BitmapProperties1(
      //         D2D1_BITMAP_OPTIONS_NONE,
      //         D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
      //      );

      //   comptr<ID2D1Bitmap1> bitmap;
      //   pgraphics2d->m_pdevicecontext->CreateBitmapFromDxgiSurface(
      //      dxgiSurface,
      //      &bitmapProps,
      //      &bitmap
      //   );

      //   // 4. Draw into the D2D1RenderTarget
      //   //d2dDeviceContext->BeginDraw();

      //   auto r = pgpucontext->m_rectangle;

      //   //pgraphics2d->m_pdevicecontext->DrawBitmap(
      //   //   bitmap,
      //   //   D2D1::RectF(r.left(), r.top(), r.width(), r.height()),
      //   //   1.0f, // opacity
      //   //   D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
      //   //   nullptr // source rect (optional)
      //   //);

      //   auto puserinteration = pgraphics2d->m_puserinteraction;

      //   


      //      ::int_rectangle rHost = r;
      //      if (puserinteration)
      //      {

      //         auto pwindow = puserinteration->window();

      //         if (pwindow)
      //         {

      //            rHost = pwindow->get_window_rectangle();

      //         }

      //      }
      //      int iBottom= r.bottom();

      //      int iHostBottom = rHost.height();

      //      int iTop = r.top();

      //      int iNewTop = iHostBottom - iBottom;

      //   pgraphics2d->m_pdevicecontext->DrawImage(
      //      bitmap,
      //      D2D1::Point2F(0.f, 0.f),
      //      D2D1::RectF(r.left(), iNewTop, r.width(), r.height()),
      //      D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
      //      D2D1_COMPOSITE_MODE_SOURCE_OVER);

      //}

   }


} // namespace graphics3d_directx12




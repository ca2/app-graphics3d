// Created by camilo on 2025-06-04 23:11 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "direct2d_draw2d_swap_chain.h"
#include "offscreen_render_target_view.h"
#include "draw2d_direct2d/graphics.h"
#include "direct2d/direct2d.h"
#include "aura/windowing/window.h"
#include "gpu_directx12/context.h"
#include "gpu_directx12/renderer.h"
#include "gpu_directx12/swap_chain.h"
#include "gpu_directx12/texture.h"
#include "aura/graphics/gpu/graphics.h"


namespace gpu_directx12
{


   direct2d_draw2d_swap_chain::direct2d_draw2d_swap_chain()
   {

   }


   direct2d_draw2d_swap_chain::~direct2d_draw2d_swap_chain()
   {


   }


   void direct2d_draw2d_swap_chain::initialize_direct2d_draw2d_gpu_swap_chain(::gpu::device* pgpudevice, ::windowing::window* pwindow)
   {


      m_pgpudevice = ::gpu::swap_chain::m_pgpudevice;
      
      //m_pgraphics = pgraphics;

      //m_pgpucontext = pgraphics->m_pgpucontext;

      //D3D11_TEXTURE2D_DESC desc = {};
      //desc.Width = g_width;
      //desc.Height = g_height;
      //desc.MipLevels = 1;
      //desc.ArraySize = 1;
      //desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      //desc.SampleDesc.Count = 1;
      //desc.Usage = D3D11_USAGE_DEFAULT;
      //desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

      //g_device->CreateTexture2D(&desc, nullptr, &g_d2dSharedTex);
      //g_device->CreateShaderResourceView(g_d2dSharedTex.Get(), nullptr, &g_srv);

      //ComPtr<IDXGIDevice> dxgiDevice;
      //g_device.As(&dxgiDevice);

      //D2D1_FACTORY_OPTIONS opts = {};
      //D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, opts, &g_d2dFactory);
      //g_d2dFactory->CreateDevice(dxgiDevice.Get(), &g_d2dDevice);

      //comptr < ID2D1DeviceContext > pdevicecontextTemplate;

      //HRESULT  hr = m_pgraphics->m_pdevice->CreateDeviceContext(
      //   //D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
      //   D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
      //   &m_pdevicecontext);

      //if (FAILED(hr))
      //{

      //   throw ::hresult_exception(hr);

      //}

      // g_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &g_d2dContext);
      ::cast < ::gpu_directx12::device > pdevice = m_pgpudevice;

      auto pd3d12device = pdevice->m_pdevice;

     /* HRESULT hrD3D11On12 = D3D11On12CreateDevice(
         m_pdevice,
         D3D11_CREATE_DEVICE_BGRA_SUPPORT,
         nullptr, 0,
         (IUnknown* const*)m_pcommandqueue.pp(),
         1,
         0,
         &m_pd3d11device,
         &m_pd3d11context,
         nullptr
      );

      ::defer_throw_hresult(hrD3D11On12);*/


      //d3d11Device.As(&d3d11On12); // Query interface

      HRESULT hrD3D11On12 = D3D11On12CreateDevice(
         pd3d12device,
         D3D11_CREATE_DEVICE_BGRA_SUPPORT,
         nullptr,
         0,
         (IUnknown * const *) pdevice->m_pcommandqueue.pp(),
         1,
         0,
         &m_pd3d11device,
         &m_pd3d11context,
         nullptr
      );

      ::defer_throw_hresult(hrD3D11On12);

      //::defer_throw_hresult(m_pd3d11device.As(&m_pd3d11on12)); // Query interface


      //::comptr<ID3D11Resource> wrappedRTV;


      ::defer_throw_hresult(m_pd3d11device.as(m_pd3d11on12)); // Query interface

      ::defer_throw_hresult(m_pd3d11device.as(m_pdxgidevice_2));


      ::defer_throw_hresult(m_pdxgiswapchain3.as(m_pdxgiswapchain1));
      
      //D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(
      //   D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
      //   D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
      //);

      //ComPtr<ID2D1Bitmap1> d2dTargetBitmap;
      //d2dDeviceContext->CreateBitmapFromDxgiSurface(
      //   dxgiSurface.Get(),
      //   &bitmapProps,
      //   &d2dTargetBitmap
      //);

      //// Set as render target
      //d2dDeviceContext->SetTarget(d2dTargetBitmap.Get());


      //::cast < gpu_directx12::renderer > pgpurendererOutput = m_pgraphics->end_draw_renderer_output();

      //::cast < gpu_directx12::swap_chain_render_target_view > pswapchainrendertargetview = pgpurendererOutput->m_prendertargetview;

      //pswapchainrendertargetview->m_ptextureShared.as(m_pdxgisurface);

      //D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(
      //   D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
      //   D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
      //);

      //::cast < ::gpu_directx12::device > pgpudevice = m_pgpucontext->m_pgpudevice;

      //m_pd2d1devicecontext = ::direct2d_directx12::get()->create_d2d1_device_context(nullptr, {});

      //m_pd2d1devicecontext->CreateBitmapFromDxgiSurface(m_pdxgisurface, bitmapProps, &m_pd2d1bitmap);
      //
      //m_pd2d1devicecontext->SetTarget(m_pd2d1bitmap);

      //g_d2dContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), &props, &g_d2dBitmap);
      //g_d2dContext->SetTarget(g_d2dBitmap.Get());

      //m_pgpucontext = pgraphics->m_pgpucontext;
      
      //m_pgpucontext = pgpucontextParam;

      //::direct2d_directx12_lock lock;

      //auto pdirect2d_directx12 = ::direct2d_directx12::get();

      ////::user::interaction* puserinteraction = m_puserinteraction;

      //::user::interaction* puserinteraction = nullptr;

      //if (!puserinteraction)
      //{

      //   puserinteraction = dynamic_cast <::user::interaction*>(m_papplication->m_pacmeuserinteractionMain.m_p);

      //}

      //auto pwindow = puserinteraction->window();

      //auto rectanglePlacement = pwindow->get_window_rectangle();

      //m_pd2d1device = pdirect2d_directx12->d2d1_device(pwindow, rectanglePlacement);

      //comptr < ID2D1DeviceContext > pdevicecontextTemplate;

      //m_pd2d1device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_pd2d1context);

      //ComPtr<IDXGISurface> dxgiSurface;
      //sharedTexture.As(&dxgiSurface);

      //D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(
      //   D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
      //   D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
      //);

      //ComPtr<ID2D1Bitmap1> d2dBitmap;
      //d2dContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), &bitmapProps, &d2dBitmap);

      //d2dContext->SetTarget(d2dBitmap.Get());
                  //ComPtr<ID2D1DeviceContext> d2dContext;
            //d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dContext);
            //::cast < ::gpu_directx12::device > pgpudevice = m_pgpucontext->m_pgpudevice;

   }


   void direct2d_draw2d_swap_chain::endDraw(::draw2d_gpu::graphics * pgraphics, ::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc)
   {

      ::cast < renderer > pgpurendererSrc = prendererSrc;

      ::cast < offscreen_render_target_view > poffscreenrendertargetview = pgpurendererSrc->m_prendertargetview;

      if (poffscreenrendertargetview->current_texture()->new_texture.m_bForOnAfterDoneFrameStep)
      {

         poffscreenrendertargetview->current_texture()->new_texture.m_bForOnAfterDoneFrameStep = false;

         D3D11_RESOURCE_FLAGS d3d11Flags = {};
         d3d11Flags.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

         auto offscreenTexture = poffscreenrendertargetview->current_texture()->m_presource.m_p;

         ::defer_throw_hresult(m_pd3d11on12->CreateWrappedResource(
            offscreenTexture,
            &d3d11Flags,
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT,
            __interface_of(m_presourceWrappedD3D11Resource)
         ));

         ::defer_throw_hresult(m_presourceWrappedD3D11Resource.as(m_pdxgisurface));

         D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
         );

         ::defer_throw_hresult(m_pd2d1devicecontext->CreateBitmapFromDxgiSurface(
            m_pdxgisurface, &bitmapProps, &m_pd2dbitmap));

      }

      ID3D11Resource* resources[] = { m_presourceWrappedD3D11Resource.m_p };

      m_pd3d11on12->AcquireWrappedResources(resources, _countof(resources));

      try
      {

         ::draw2d_direct2d::swap_chain::endDraw(pgraphics, puserinteraction, prendererSrc);

      }
      catch (...)
      {


      }

      m_pd3d11on12->ReleaseWrappedResources(resources, _countof(resources));


//      ::cast < ::draw2d_direct2d::graphics > pgraphics2d = pgraphics;
//
//      if (!pgraphics2d)
//      {
//
//         return;
//
//      }
//
//      ::cast< ::gpu_directx12::context > pgpucontext = m_pgpucontext;
//      ::cast< ::gpu_directx12::renderer > prenderer = pgpucontext->m_pgpurenderer;
//      auto prendertargetview = prenderer->m_prendertargetview;
//      ::cast < ::gpu_directx12::offscreen_render_target_view > poffscreenrendertargetview = prendertargetview;
//      ::cast< ::gpu_directx12::device > pgpudevice = pgpucontext->m_pgpudevice;
//
//      //::cast < ::draw2d_direct2d_directx12::swap_chain_end_draw > penddraw = pgraphics2d->m_penddraw;
//
//      //ID3D11Device* device = pgpudevice->m_pdevice;
//      //ID3D11DeviceContext* context = pgpucontext->m_pcontext;
//      auto offscreenTexture = poffscreenrendertargetview->current_texture()->m_presource.m_p;
//      //if (!device || !context || !offscreenTexture)
//      if (!offscreenTexture)
//      {
//         throw ::exception(error_wrong_state);
//      }
//
//      auto pswapchain = pgpudevice->m_pswapchain;
//
//      if (!pswapchain)
//      {
//
//         throw ::exception(error_wrong_state);
//      }
//
//      if (poffscreenrendertargetview->current_texture()->new_texture.m_bForOnAfterDoneFrameStep)
//      {
//
//         if (m_presourceWrappedD3D11Resource)
//         {
//
//            // Tell D3D11On12 we’re done using the wrapped resource
//            ID3D11Resource* resources[] = { m_presourceWrappedD3D11Resource.m_p };
//            m_pd3d11on12->ReleaseWrappedResources(resources, _countof(resources));
//
//         }
//
//         poffscreenrendertargetview->current_texture()->new_texture.m_bForOnAfterDoneFrameStep = false;
//
//         D3D11_RESOURCE_FLAGS d3d11Flags = {};
//         d3d11Flags.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
//
//         ::defer_throw_hresult(m_pd3d11on12->CreateWrappedResource(
//            offscreenTexture,
//            &d3d11Flags,
//            D3D12_RESOURCE_STATE_RENDER_TARGET,
//            D3D12_RESOURCE_STATE_PRESENT,
//            __interface_of(m_presourceWrappedD3D11Resource)
//         ));
//         //ComPtr<IDXGISurface> dxgiSurface;
//         ::defer_throw_hresult(m_presourceWrappedD3D11Resource.as(m_pdxgisurface));
//         D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(
//            D2D1_BITMAP_OPTIONS_TARGET,
//            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
//         );
//
//         ::defer_throw_hresult(m_pd2d1devicecontext->CreateBitmapFromDxgiSurface(
//            m_pdxgisurface, &bitmapProps, &m_pd2dbitmap));
//
//      }
//
//      m_pd2d1devicecontext->SetTarget(m_pd2dbitmap);
//      m_pd2d1devicecontext->BeginDraw();
//
//      /*ComPtr<ID2D1Bitmap1> d2dBitmap;
//      ThrowIfFailed(d2dContext->CreateBitmapFromDxgiSurface(
//         dxgiSurface.Get(), &bitmapProps, &d2dBitmap));
//
//      d2dContext->SetTarget(d2dBitmap.Get());
//      d2dContext->BeginDraw();*/
//
//
//      auto puserinteration = pgraphics2d->m_puserinteraction;
//
//      auto r = pgpucontext->m_rectangle;
//
//
//      ::int_rectangle rHost = r;
//      if (puserinteration)
//      {
//
//         auto pwindow = puserinteration->window();
//
//         if (pwindow)
//         {
//
//            rHost = pwindow->get_window_rectangle();
//
//         }
//
//      }
//      int iBottom = r.bottom();
//
//      int iHostBottom = rHost.height();
//
//      int iTop = r.top();
//
//      int iNewTop = iHostBottom - iBottom;
//      // Example draw:
//      m_pd2d1devicecontext->DrawImage(
//         m_pd2dbitmap,
//         D2D1::Point2F(0.f, 0.f),
//         D2D1::RectF(r.left(), iNewTop, r.width(), r.height()),
//         D2D1_INTERPOLATION_MODE_LINEAR,
//         D2D1_COMPOSITE_MODE_SOURCE_OVER);
//
//      ::defer_throw_hresult(m_pd2d1devicecontext->EndDraw());
//
//      //::gpu_directx12
//
//         //auto pdevice = pswapchain->_get_d
//
//         m_pd3d11context->Flush();
//
//      //D3D11_TEXTURE2D_DESC texDesc = {};
//      //texDesc.Width = width;
//      //texDesc.Height = height;
//      //texDesc.MipLevels = 1;
//      //texDesc.ArraySize = 1;
//      //texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
//      //texDesc.SampleDesc.Count = 1;
//      //texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
//      //texDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;
//
//      // ... Create texture using device->CreateTexture2D
//
//      //   // 2. Wrap the texture in a DXGI surface
//      //   comptr<IDXGISurface> dxgiSurface;
//      //   offscreenTexture->QueryInterface(IID_PPV_ARGS(&dxgiSurface));
//
//      //   // 3. Create the Direct2D bitmap
//      //   D2D1_BITMAP_PROPERTIES1 bitmapProps =
//      //      D2D1::BitmapProperties1(
//      //         D2D1_BITMAP_OPTIONS_NONE,
//      //         D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
//      //      );
//
//      //   comptr<ID2D1Bitmap1> bitmap;
//      //   pgraphics2d->m_pdevicecontext->CreateBitmapFromDxgiSurface(
//      //      dxgiSurface,
//      //      &bitmapProps,
//      //      &bitmap
//      //   );
//
//      //   // 4. Draw into the D2D1RenderTarget
//      //   //d2dDeviceContext->BeginDraw();
//
//      //   auto r = pgpucontext->m_rectangle;
//
//      //   //pgraphics2d->m_pdevicecontext->DrawBitmap(
//      //   //   bitmap,
//      //   //   D2D1::RectF(r.left(), r.top(), r.width(), r.height()),
//      //   //   1.0f, // opacity
//      //   //   D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
//      //   //   nullptr // source rect (optional)
//      //   //);
//
//      //   auto puserinteration = pgraphics2d->m_puserinteraction;
//
//      //   
//
//
//      //      ::int_rectangle rHost = r;
//      //      if (puserinteration)
//      //      {
//
//      //         auto pwindow = puserinteration->window();
//
//      //         if (pwindow)
//      //         {
//
//      //            rHost = pwindow->get_window_rectangle();
//
//      //         }
//
//      //      }
//      //      int iBottom= r.bottom();
//
//      //      int iHostBottom = rHost.height();
//
//      //      int iTop = r.top();
//
//      //      int iNewTop = iHostBottom - iBottom;
//
//      //   pgraphics2d->m_pdevicecontext->DrawImage(
//      //      bitmap,
//      //      D2D1::Point2F(0.f, 0.f),
//      //      D2D1::RectF(r.left(), iNewTop, r.width(), r.height()),
//      //      D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
//      //      D2D1_COMPOSITE_MODE_SOURCE_OVER);
//
//      //}
//
////
////      auto pcurrentcontext = m_pgpudevice->current_context();
////
////      ::cast < ::gpu_directx12::renderer > prenderer = pcurrentcontext->m_pgpurenderer;
////      ///auto pcommandqueue = prenderer->m_pcommandqueue;
////
////      D3D11_RESOURCE_FLAGS flags = {};
////      flags.BindFlags = D3D11_BIND_RENDER_TARGET;
////
////      ::defer_throw_hresult(m_pd3d11on12->CreateWrappedResource(
////         prenderer->m_prendertargetview->current_texture()->m_presource,
////         &flags,
////         D3D12_RESOURCE_STATE_RENDER_TARGET,
////         D3D12_RESOURCE_STATE_PRESENT,
////         __interface_of(m_presourceWrappedRTV)
////      ));
////
////      //      ::defer_throw_hresult(m_presourceWrappedRTV.as(m_pdxgisurface_2)); // Get IDXGISurface
////
////
////      resources[0] = { m_presourceWrappedRTV.m_p };
////      m_pd3d11on12->AcquireWrappedResources(resources, _countof(resources));
////
////
////
////      ::draw2d_direct2d::swap_chain::endDraw(pgraphics, puserinteraction, prendererSrc);
////
////      m_pd3d11on12->ReleaseWrappedResources(resources, _countof(resources));
////     
//////      ::cast < ::draw2d_direct2d::graphics > pgraphicsDirect2d = pgraphics;
//////
//////      comptr < ID2D1Bitmap > pd2d1bitmap;
//////
//////      pgraphicsDirect2d->m_pbitmaprendertarget->GetBitmap(&pd2d1bitmap);
//////
//////      pgraphicsDirect2d->m_pdevicecontext->SetTarget(nullptr);
//////
//////      m_pd2d1devicecontext->BeginDraw();
//////
//////      D2D1_RECT_F rectfDib1;
//////
//////      // Save original transform
//////      D2D1_MATRIX_3X2_F originalTransform;
//////      m_pd2d1devicecontext->GetTransform(&originalTransform);
//////
//////      // Flip vertically: scale Y by -1 and translate down
//////      D2D1_MATRIX_3X2_F flipY =
//////         D2D1::Matrix3x2F::Scale(1.0f, -1.0f) *
//////         D2D1::Matrix3x2F::Translation(0.0f, m_pgpucontext->m_rectangle.height());
//////
//////      m_pd2d1devicecontext->SetTransform(flipY * originalTransform);
//////
//////
//////      rectfDib1.left = 0;
//////      rectfDib1.top = 0;
//////      rectfDib1.right = m_pgpucontext->m_rectangle.width();
//////      rectfDib1.bottom = m_pgpucontext->m_rectangle.height();
//////      D2D1_COLOR_F color;
//////
//////      copy(color, ::rgba(0.75, 0.75, 0.40, 0.5));
//////
//////      comptr< ID2D1SolidColorBrush> pd2d1solidcolorbrush1;
//////
//////      m_pd2d1devicecontext->CreateSolidColorBrush(color, &pd2d1solidcolorbrush1);
//////      //D2D1_RECT_F r1;
//////
//////      //r1.left = 500;
//////      //r1.top = 300;
//////      //r1.right = 600;
//////      //r1.bottom = 400;
//////
//////      //m_pd2d1devicecontext->FillRectangle(r1, pd2d1solidcolorbrush1);
//////
//////
//////
//////      m_pd2d1devicecontext->DrawImage(
//////         pd2d1bitmap,
//////         D2D1::Point2F(0.f, 0.f),
//////         rectfDib1,
//////         D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
//////         D2D1_COMPOSITE_MODE_SOURCE_COPY);
//////
//////      // Restore transform
//////      m_pd2d1devicecontext->SetTransform(originalTransform);
//////      //D2D1_RECT_F r2;
//////
//////      //r2.left = 100;
//////      //r2.top = 100;
//////      //r2.right = 200;
//////      //r2.bottom = 500;
//////
//////      //comptr< ID2D1SolidColorBrush> pd2d1solidcolorbrush;
//////
//////      //D2D1_COLOR_F color1;
//////
//////      //copy(color1, ::rgba(0.5, 0.75, 0.90, 0.5));
//////
//////      //m_pd2d1devicecontext->CreateSolidColorBrush(color1, &pd2d1solidcolorbrush);
//////
//////      //m_pd2d1devicecontext->DrawRectangle(r2, pd2d1solidcolorbrush);
//////
//////
//////      m_pd2d1devicecontext->EndDraw();
//////
//////      m_pgraphics->m_pdevicecontext->SetTarget(pd2d1bitmap);
//////
//////      //::cast < gpu_directx12::renderer > pgpurendererOutput = m_pgraphics->end_draw_renderer_output();
//////
//////      //::cast < gpu_directx12::swap_chain_render_target_view > pswapchainrendertargetview = pgpurendererOutput->m_prendertargetview;
//////
//////      //pswapchainrendertargetview->endDraw();
//////
//////
//////      //d2dDeviceContext->BeginDraw();
////////// ... Direct2D drawing calls ...
//////      pgraphicsDirect2d->m_pdevicecontext->EndDraw();
//////      m_pd3d11context->Flush(); // Ensure rendering is completed
////
////
////      
////      
////
//

   }


   void direct2d_draw2d_swap_chain::present()
   {

      ::draw2d_direct2d::swap_chain::present();

   }


} // namespace gpu_directx12




#include "framework.h"
#include "pen.h"
#include "graphics.h"
#include "direct2d_directx12/direct2d_directx12.h"


namespace draw2d_direct2d_directx12
{


   pen::pen()
   {

      m_pthis = this;
      m_bMetroColor     = false;
      //m_colorMetro = {};

   }


   pen::~pen()
   {

      destroy();

   }


   //void pen::dump(dump_context & dumpcontext) const
   //{

   //   ::draw2d::object::dump(dumpcontext);

   //}


   //comptr < ID2D1StrokeStyle1 > pen::_create_stroke_style(::draw2d::graphics * pgraphicsParam)
   //{

   //   bool bProperties = false;

   //   D2D1_STROKE_STYLE_PROPERTIES1 properties1{};

   //   if (m_elinecapBeg == ::draw2d::e_line_cap_round)
   //   {

   //      properties1.startCap = D2D1_CAP_STYLE_ROUND;

   //      bProperties = true;

   //   }


   //   if (m_elinecapEnd == ::draw2d::e_line_cap_round)
   //   {

   //      properties1.endCap = D2D1_CAP_STYLE_ROUND;

   //      bProperties = true;

   //   }


   //   if (!bProperties)
   //   {

   //      return nullptr;

   //   }

   //   comptr < ID2D1StrokeStyle1 > pstrokestyle;

   //   HRESULT hr = ::direct2d_directx12::factory()->CreateStrokeStyle(&properties1, nullptr, 0, &pstrokestyle);

   //   return pstrokestyle;

   //}

   void pen::create(::draw2d::graphics* pgraphicsParam, char iCreate)
   {

      auto pgraphics = __graphics(pgraphicsParam);

      if((!m_bMetroColor || m_colorMetro != m_color) || m_pbrush == nullptr)
      {

         D2D1_COLOR_F color;

         copy(color, m_color);

         pgraphics->m_prendertarget->CreateSolidColorBrush(color, &m_pbrush);

         if(m_pbrush != nullptr)
         {

            m_osdata[0] = (ID2D1Brush *) m_pbrush;
            
            m_colorMetro       = m_color;

            m_bMetroColor   = true;
            
         }

         m_pstrokestyle = _create_stroke_style(pgraphics,
            m_elinecapBeg,
            m_elinecapEnd);

         if (m_pstrokestyle != nullptr)
         {

            m_osdata[1] = (ID2D1StrokeStyle1 *)m_pstrokestyle;

         }


      }

      //return m_osdata[0] != nullptr;

   }


   void pen::destroy()
   {

      destroy_os_data();

      ::draw2d::pen::destroy();

   }



   void pen::destroy_os_data()
   {

      m_pbrush = nullptr;

      m_pstrokestyle = nullptr;

      object::destroy_os_data();

   }

   HRESULT pen::s_RenderPatternToCommandList(ID2D1RenderTarget * pgraphics,D2D1_COLOR_F *pcr)
   {

      HRESULT hr = S_OK;

      //pgraphics->BeginDraw();

      pgraphics->Clear(pcr);

      ID2D1SolidColorBrush * pbr = nullptr;

      //hr = pgraphics->CreateSolidColorBrush(*pcr, &pbr);

      //pgraphics->DrawRectangle(D2D1::RectF(0.f, 0.f, 256.f, 256.f), pbr, 0.f);

      //pbr->Release();

      ///hr = pgraphics->EndDraw();

      return hr;

   }

   HRESULT pen::s_CreatePatternBrush(ID2D1DeviceContext *pDeviceContext, D2D1_COLOR_F * pcr, ID2D1ImageBrush **ppImageBrush)
   {

      HRESULT hrEndDraw = pDeviceContext->EndDraw();

      HRESULT hr = S_OK;
      ID2D1Image *pOldTarget = nullptr;
      pDeviceContext->GetTarget(&pOldTarget);

      ID2D1CommandList *pCommandList = nullptr;
      hr = pDeviceContext->CreateCommandList(&pCommandList);

      if(SUCCEEDED(hr))
      {
         pDeviceContext->SetTarget(pCommandList);
         hr = s_RenderPatternToCommandList(pDeviceContext,pcr);
      }

      pDeviceContext->SetTarget(pOldTarget);

      ID2D1ImageBrush *pImageBrush = nullptr;

      if(SUCCEEDED(hr))
      {
         hr = pCommandList->Close();
      }

      if(SUCCEEDED(hr))
      {

         D2D1_IMAGE_BRUSH_PROPERTIES props;

         props.sourceRectangle.left = 0.f;
         props.sourceRectangle.top = 0.f;
         props.sourceRectangle.right = 256.f;
         props.sourceRectangle.bottom = 256.f;

         props.extendModeX = D2D1_EXTEND_MODE_WRAP;
         props.extendModeY = D2D1_EXTEND_MODE_WRAP;

         props.interpolationMode = D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR;

         hr = pDeviceContext->CreateImageBrush(
              pCommandList,&props,nullptr,&pImageBrush);
      }

      // Fill a int_rectangle with the image brush.
      /*if (SUCCEEDED(hr))
      {
      pDeviceContext->fill_rectangle(
      D2D1::RectF(0, 0, 100, 100), pImageBrush);
      }*/

      //pImageBrush->Release();
      pCommandList->Release();
      pOldTarget->Release();

      if(hrEndDraw == S_OK)
      {
         pDeviceContext->BeginDraw();
      }

      if(SUCCEEDED(hr))
      {

         *ppImageBrush = pImageBrush;

      }
      else
      {

         *ppImageBrush = nullptr;
      }

      return hr;
   }

} // namespace draw2d_direct2d_directx12




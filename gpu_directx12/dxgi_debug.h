// Created by camilo on 2021-06-05 23:48 BRT <3ThomasBS_!!
#pragma once


#if defined(_DEBUG)


#include <DXGIDebug.h>


namespace directx12
{


   class CLASS_DECL_GPU_DIRECTX12 dxgi_debug :
      virtual public ::matter
   {
   public:


      comptr < IDXGIDebug1 >     m_pdxgidebug1;

      comptr < IDXGIDebug >      m_pdxgidebug;

      static dxgi_debug *        s_pdxgidebug;


      dxgi_debug();


      virtual void debug();


   };


} // namespace directx12


#endif // !defined(UNIVERSAL_WINDOWS)




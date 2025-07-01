// From gpu_directx11/input_layout.h by
// camilo on 2025-06-29 06:04 <3ThomasBorregaardSørensen!!
// From gpu_vulkan/input_layout.h by camilo on 2025-06-29 03:49 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/input_layout.h"


namespace gpu_directx12
{


   class CLASS_DECL_GPU_DIRECTX12 input_layout :
      virtual public ::gpu::input_layout
   {
   public:


      ::array < D3D12_INPUT_ELEMENT_DESC > m_desca;



      input_layout();
      ~input_layout();


      //void on_initialize_input_layout() override;



      D3D12_INPUT_LAYOUT_DESC _get_d3d12_input_layout_desc();


   };


} // namespace gpu_directx11




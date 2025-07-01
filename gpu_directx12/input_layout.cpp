// From gpu_directx11/input_layout.cpp by
// camilo on 2025-06-29 06:06 <3ThomasBorregaardSørensen!!
// From gpu_vulkan/input_layout.cpp by camilo on 2025-06-29 03:55 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "context.h"
#include "device.h"
#include "input_layout.h"


namespace gpu_directx12
{



   inline const char* input_layout_semantic_name_from_gpu_property_name(const ::scoped_string& scopedstr)
   {

      if (scopedstr.case_insensitive_equals("position"))
      {

         return "POSITION";

      }
      else if (scopedstr.case_insensitive_equals("color"))
      {

         return "COLOR";

      }
      else if (scopedstr.case_insensitive_equals("normal"))
      {

         return "NORMAL";

      }
      else if (scopedstr.case_insensitive_equals("uv"))
      {

         return "TEXCOORD";

      }
      else
      {

         throw ::exception(error_not_implemented, "please implement this missing implementation");

      }

   }




   inline DXGI_FORMAT input_layout_format_from_gpu_property_type(const ::gpu::enum_type& etype)
   {

      switch (etype)
      {
      case ::gpu::e_type_seq2:
         return DXGI_FORMAT_R32G32_FLOAT;
      case ::gpu::e_type_seq3:
         return DXGI_FORMAT_R32G32B32_FLOAT;
      case ::gpu::e_type_seq4:
         return DXGI_FORMAT_R32G32B32A32_FLOAT;
      default:
         throw ::exception(error_not_implemented, "please implement this missing implementation");
      }

   }




   inline int input_layout_aligned_property_size(int i)
   {

      return (i + 3) & ~3;

   }


   input_layout::input_layout()
   {


   }


   input_layout::~input_layout()
   {


   }


   D3D12_INPUT_LAYOUT_DESC input_layout::_get_d3d12_input_layout_desc()
   {

      auto countInputLayout = this->count();

      if (countInputLayout > 0)
      {

         int iSemanticIndex = 0;
         int iInputSlot = 0;
         D3D12_INPUT_CLASSIFICATION classification = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
         UINT DataStepRate = 0;
         int iOffset = 0;
         int iNextOffset = 0;

         for (::collection::index iInputLayout = 0; iInputLayout < countInputLayout; iInputLayout++)
         {

            auto pproperty = this->m_pproperties + iInputLayout;

            auto name = pproperty->m_pszName;
            auto type = pproperty->m_etype;
            auto offset = iNextOffset;
            iNextOffset = offset + input_layout_aligned_property_size(pproperty->get_item_size());

            D3D12_INPUT_ELEMENT_DESC desc{};

            desc.SemanticName = input_layout_semantic_name_from_gpu_property_name(name);
            desc.SemanticIndex = iSemanticIndex;
            desc.Format = input_layout_format_from_gpu_property_type(type);
            desc.InputSlot = iInputSlot;
            desc.AlignedByteOffset = offset;
            desc.InputSlotClass = classification;
            desc.InstanceDataStepRate = DataStepRate;

            m_desca.add(desc);

         }

         //// Input layout
         //D3D12_INPUT_ELEMENT_DESC layout[] = {
         //    {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
         //    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
         //};

      }
     /* else
      {

         UINT uOffset0 = offsetof(gpu::Vertex, position);
         UINT uOffset1 = offsetof(gpu::Vertex, color);
         UINT uOffset2 = offsetof(gpu::Vertex, normal);
         UINT uOffset3 = offsetof(gpu::Vertex, uv);


         layout.add({ "POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, uOffset0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
         if (m_iVertexLevel == 2)
         {
            layout.add({ "COLOR"    , 0, DXGI_FORMAT_R32G32B32A32_FLOAT , 0, uOffset1, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
         }
         else
         {
            layout.add({ "COLOR"    , 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, uOffset1, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });


         }

         if (m_iVertexLevel > 2 || m_iVertexLevel < 0)
         {
            layout.add({ "NORMAL"   , 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, uOffset2, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
            layout.add({ "TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT    , 0, uOffset3, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

         }

      }*/

      return { m_desca.data(), (UINT) m_desca.size() };

   }


} // namespace gpu_directx12




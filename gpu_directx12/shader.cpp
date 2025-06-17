#include "framework.h"
// Co-creating with V0idsEmbrace@Twitch with
// camilo on 2025-05-19 04:59 <3ThomasBorregaardSorensen!!
#include "approach.h"
#include "buffer.h"
#include "context.h"
#include "shader.h"
#include "descriptors.h"
#include "renderer.h"
#include "texture.h"
#include "offscreen_render_target_view.h"
#include "bred/gpu/types.h"
#include "acme_windows_common/hresult_exception.h"
#include <d3dcompiler.h>
//#include "bred/user/user/graphics3d.h"



namespace gpu_directx12
{


   shader::shader()
   {

   }


   shader::~shader()
   {

   }


   ::comptr < ID3DBlob> shader::create_vertex_shader_blob(const ::block& block)
   {

      comptr <ID3DBlob> pblobShader;
      comptr <ID3DBlob> pblobError;

      auto data = (const char*)block.data();

      auto size = block.size();

      HRESULT hr = D3DCompile(
         data,            // pointer to shader source
         size,             // size of shader source
         nullptr,                       // optional source name
         nullptr,                       // macro definitions
         nullptr,                       // include handler
         "main",                      // entry point
         "vs_5_0",                      // target profile (e.g., vs_5_0, ps_5_0)
         0,                             // compile flags
         0,                             // effect flags
         &pblobShader,                   // compiled shader
         &pblobError                     // error messages
      );

      if (FAILED(hr))
      {

         if (pblobError)
         {

            ::string strError((const char*)pblobError->GetBufferPointer(),
               pblobError->GetBufferSize());

            throw ::exception(error_failed);

         }

      }

      return pblobShader;

   }


   ::comptr < ID3DBlob> shader::create_pixel_shader_blob(const ::block& block)
   {

      comptr <ID3DBlob> pblobShader;
      comptr <ID3DBlob> pblobError;

      auto data = (const char*)block.data();

      auto size = block.size();

      HRESULT hr = D3DCompile(
         data,            // pointer to shader source
         size,             // size of shader source
         nullptr,                       // optional source name
         nullptr,                       // macro definitions
         nullptr,                       // include handler
         "main",                      // entry point
         "ps_5_0",                      // target profile (e.g., vs_5_0, ps_5_0)
         0,                             // compile flags
         0,                             // effect flags
         &pblobShader,                   // compiled shader
         &pblobError                     // error messages
      );

      if (FAILED(hr))
      {

         if (pblobError)
         {

            ::string strError((const char*)pblobError->GetBufferPointer(),
               pblobError->GetBufferSize());

            throw ::exception(error_failed);

         }
      }

      return pblobShader;

   }


   void shader::create_root_signature()
   {

      ::cast < ::gpu_directx12::device > pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      ::cast < ::gpu_directx12::context > pcontext = m_pgpurenderer->m_pgpucontext;

      int i = 0;
      // Create a root signature consisting of a descriptor table with a single CBV.
      {
         D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

         // This is the highest version the sample supports. If CheckFeatureSupport succeeds, the HighestVersion returned will not be greater than this.
         featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

         if (FAILED(pgpudevice->m_pdevice->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
         {
            featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
         }

         ::array < CD3DX12_DESCRIPTOR_RANGE1> ranges;
         ::array<CD3DX12_ROOT_PARAMETER1> rootParameters;

         int iCount = 0;
         if (m_edescriptorsetslota.contains(e_descriptor_set_slot_global))
         {
            iCount++;

         }
         if (m_edescriptorsetslota.contains(e_descriptor_set_slot_local))
         {

            iCount++;

         }
         if (m_edescriptorsetslota.contains(e_descriptor_set_shader_resource_view_and_sampler))
         {

            iCount+=2;

         }
         ranges.set_size(iCount);
         rootParameters.set_size(iCount);
         //ranges.set_size(1);
         //rootParameters.set_size(1);
         ////ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, iCount, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
         //ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, iCount, 0);
         //rootParameters[0].InitAsDescriptorTable(1, &ranges[i],
         //   D3D12_SHADER_VISIBILITY_ALL); //|              D3D12_SHADER_VISIBILITY_PIXEL);

         if (m_edescriptorsetslota.contains(e_descriptor_set_slot_global))
         {

            ranges[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, i, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
            rootParameters[i].InitAsDescriptorTable(1, &ranges[i],
               D3D12_SHADER_VISIBILITY_ALL); //|              D3D12_SHADER_VISIBILITY_PIXEL);
            i++;

         }

         if (m_edescriptorsetslota.contains(e_descriptor_set_slot_local))
         {

            //if (m_pLocalDescriptorSet)
            //{

            //   ::cast < ::gpu_directx12::set_descriptor_layout > pset = m_pLocalDescriptorSet;

            //   auto setLayout = pset->getDescriptorSetLayout();

            //   descriptorSetLayouts.add(setLayout);

            //}

      //      rootParameters.element_at_grow(1).InitAsConstantBufferView(1); // b1: ObjectMatrices
            //ranges[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, i, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
            rootParameters[i].InitAsConstantBufferView(1);
            i++;

         }

         if (m_edescriptorsetslota.contains(e_descriptor_set_shader_resource_view_and_sampler))
         {
            // Range for SRV (Texture2D)
            ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,     // Range type
               1,                                    // Number of descriptors
               0,                                    // Base shader register (t0)
               0,                                    // Register space
               D3D12_DESCRIPTOR_RANGE_FLAG_NONE,    // Flags
               D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND);

            // Range for Sampler
            ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, // Range type
               1,                                    // Number of descriptors
               0,                                    // Base shader register (s0)
               0,                                    // Register space
               D3D12_DESCRIPTOR_RANGE_FLAG_NONE,    // Flags
               D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND);

            //CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc;
            //rootSigDesc.Init(1, &param, 0, nullptr,
            //   D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
             // Root parameter for SRV table
            rootParameters[0].InitAsDescriptorTable(1,              // Number of ranges
               &ranges[0],       // Range array
               D3D12_SHADER_VISIBILITY_PIXEL); // Only pixel shader needs this

            // Root parameter for Sampler table
            rootParameters[1].InitAsDescriptorTable(1,              // Number of ranges
               &ranges[1],       // Range array
               D3D12_SHADER_VISIBILITY_PIXEL); // Only pixel shader needs this

         }



         // Allow input layout and deny uneccessary access to certain pipeline stages.
         D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
            D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
            D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
            D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
            D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
         //|  D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

         CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
         rootSignatureDesc.Init_1_1(rootParameters.size(), rootParameters.data(), 0, nullptr, rootSignatureFlags);

         ::comptr<ID3DBlob> signature;
         ::comptr<ID3DBlob> error;
         HRESULT hrSerializeVersionedRootSignature = D3DX12SerializeVersionedRootSignature(
            &rootSignatureDesc, featureData.HighestVersion, &signature, &error);
         if (FAILED(hrSerializeVersionedRootSignature))
         {
            if (error)
            {
               ::OutputDebugStringA((char*)error->GetBufferPointer());
            }
            throw ::exception(error_failed, "Failed to serialize root signature");
         }

         HRESULT hrCreateRootSignature = pgpudevice->m_pdevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), __interface_of(m_prootsignature));
         ::defer_throw_hresult(hrCreateRootSignature);
      }

      //::cast < ::gpu_directx12::device > pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      //::cast < ::gpu_directx12::context > pcontext = m_pgpurenderer->m_pgpucontext;

      //   ::array <CD3DX12_ROOT_PARAMETER> rootParameters;

      //   if (m_edescriptorsetslota.contains(e_descriptor_set_slot_global))
      //   {

      //      rootParameters.element_at_grow(0).InitAsConstantBufferView(0); // b0: GlobalUbo

      //   }

      //   if (m_edescriptorsetslota.contains(e_descriptor_set_slot_local))
      //   {

      //      //if (m_pLocalDescriptorSet)
      //      //{

      //      //   ::cast < ::gpu_directx12::set_descriptor_layout > pset = m_pLocalDescriptorSet;

      //      //   auto setLayout = pset->getDescriptorSetLayout();

      //      //   descriptorSetLayouts.add(setLayout);

      //      //}

      //      rootParameters.element_at_grow(1).InitAsConstantBufferView(1); // b1: ObjectMatrices

      //   }
      //   //rootParameters[0].InitAsConstantBufferView(0);

      //   CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc;
      //   rootSigDesc.Init(rootParameters.size(), rootParameters.data(),
      //      0, nullptr,
      //      D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

      //   ::comptr<ID3DBlob> serializedRootSig = nullptr;
      //   ::comptr<ID3DBlob> errorBlob = nullptr;
      //   HRESULT hr = D3D12SerializeRootSignature(
      //      &rootSigDesc,
      //      D3D_ROOT_SIGNATURE_VERSION_1,
      //      &serializedRootSig,
      //      &errorBlob
      //   );

      //   if (FAILED(hr))
      //   {
      //      if (errorBlob)
      //      {
      //         ::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
      //      }
      //      throw ::exception(error_failed, "Failed to serialize root signature");
      //   }

      //   //      com_ptr<ID3D12RootSignature> rootSignature;

      //   HRESULT hrCreateRootSignature = pgpudevice->m_pdevice->CreateRootSignature(
      //      0,
      //      serializedRootSig->GetBufferPointer(),
      //      serializedRootSig->GetBufferSize(),
      //      __interface_of(m_prootsignature)
      //      );
      //   ::defer_throw_hresult(hrCreateRootSignature);

      //return rootSignature;
   }


   inline const char* input_layout_semantic_name_from_gpu_property_name(const ::scoped_string& scopedstr)
   {

      if (scopedstr.case_insensitive_equals("position"))
      {

         return "POSITION";

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


   inline DXGI_FORMAT input_layout_format_from_gpu_property_type(const ::gpu::enum_type & etype)
   {

      switch(etype)
      {
      case ::gpu::e_type_seq2:
         return DXGI_FORMAT_R32G32_FLOAT;
      default:
         throw ::exception(error_not_implemented, "please implement this missing implementation");
      }

   }


   inline int input_layout_aligned_property_size(int i)
   {

      return (i + 3) & ~3;

   }

   void shader::create_vertex_and_pixel_shader(const ::block& blockVertex, const ::block& blockPixel)
   {

      ::array < D3D12_INPUT_ELEMENT_DESC > layout;

      auto countInputLayout = m_propertiesInputLayout.count();

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

            auto pproperty = m_propertiesInputLayout.m_pproperties + iInputLayout;

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

            layout.add(desc);

         }

         //// Input layout
         //D3D12_INPUT_ELEMENT_DESC layout[] = {
         //    {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
         //    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
         //};

      }
      else
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

      }

      auto data = layout.data();

      auto size = layout.size();

      auto pblobVertex = create_vertex_shader_blob(blockVertex);

      auto pblobPixel = create_pixel_shader_blob(blockPixel);

      ::cast < ::gpu_directx12::device > pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      ::cast < ::gpu_directx12::context > pcontext = m_pgpurenderer->m_pgpucontext;

      create_root_signature();

      // Describe and create the graphics pipeline state object (PSO).
      D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
      psoDesc.InputLayout = { data, (UINT)size };
      psoDesc.pRootSignature = m_prootsignature;
      psoDesc.VS = CD3DX12_SHADER_BYTECODE(pblobVertex);
      psoDesc.PS = CD3DX12_SHADER_BYTECODE(pblobPixel);
      psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
      psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
      psoDesc.SampleMask = UINT_MAX;
      psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
      psoDesc.NumRenderTargets = 1;
      psoDesc.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM;
      psoDesc.SampleDesc.Count = 1;

      if (m_bDisableDepthTest)
      {
         
         psoDesc.DepthStencilState.DepthEnable = FALSE;
         psoDesc.DepthStencilState.StencilEnable = FALSE;

      }
      else
      {

         D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
         depthStencilDesc.DepthEnable = TRUE;
         depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
         depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
         depthStencilDesc.StencilEnable = FALSE;
         psoDesc.DepthStencilState = depthStencilDesc;
         psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT; // must match your DSV

      }

      HRESULT hrCreateGraphicsPipelineState =
         pgpudevice->m_pdevice->CreateGraphicsPipelineState(&psoDesc, __interface_of(m_ppipelinestate));

      ::defer_throw_hresult(hrCreateGraphicsPipelineState);

   }
      

   void shader::on_initialize_shader()
   {

      ::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::cast <device> pgpudevice = pgpucontext->m_pgpudevice;

      ::cast <renderer> prenderer = m_pgpurenderer;

      ::cast < shader_vertex_input > pshadervertexinput = m_pVertexInput;

      pgpudevice->defer_shader_memory(m_memoryVertex, m_pathVertex);

      pgpudevice->defer_shader_memory(m_memoryFragment, m_pathFragment);

      create_vertex_and_pixel_shader(m_memoryVertex, m_memoryFragment);

      if (m_edescriptorsetslota.contains(e_descriptor_set_slot_local))
      {
         int iNumberOfObjects = 10;
         UINT constantBufferSize = ::directx12::Align256(m_propertiesPush.size()) * iNumberOfObjects;    // CB size is required to be 256-byte aligned.
         CD3DX12_HEAP_PROPERTIES heapproperties(D3D12_HEAP_TYPE_UPLOAD);
         auto resourcedesc = CD3DX12_RESOURCE_DESC::Buffer(constantBufferSize);
         HRESULT hrCreateCommittedResource = pgpudevice->m_pdevice->CreateCommittedResource(
            &heapproperties,
            D3D12_HEAP_FLAG_NONE,
            &resourcedesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            __interface_of(m_presourcePushProperties));
         pgpudevice->defer_throw_hresult(hrCreateCommittedResource);


         UINT descriptorSize = pgpudevice->m_pdevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

         D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
         cbvDesc.BufferLocation = m_presourcePushProperties->GetGPUVirtualAddress();
         cbvDesc.SizeInBytes = constantBufferSize;
         int iDescriptor = 1; // 1 - second descriptor, first descriptor (0) is the global ubo
         CD3DX12_CPU_DESCRIPTOR_HANDLE objectCBHandle(prenderer->m_pheapCbv->GetCPUDescriptorHandleForHeapStart(), iDescriptor, descriptorSize);
         pgpudevice->m_pdevice->CreateConstantBufferView(&cbvDesc, objectCBHandle);

      }

   }


   void shader::bind(::gpu::texture* pgputextureTarget, ::gpu::texture* pgputextureSource)
   {

      bind();

   }


   void shader::bind()
   {

      ::cast < ::gpu_directx12::renderer > prenderer = m_pgpurenderer;

      ::cast < ::gpu_directx12::context > pcontext = prenderer->m_pgpucontext;

      auto pcommandbuffer = prenderer->getCurrentCommandBuffer2();

      auto pcommandlist = pcommandbuffer->m_pcommandlist;

      pcommandlist->SetPipelineState(m_ppipelinestate);  // ID3D12PipelineState*

      if (m_prootsignature)
      {

         pcommandlist->SetGraphicsRootSignature(m_prootsignature);

      }

      if (m_edescriptorsetslota.contains(e_descriptor_set_slot_global))
      {

         auto iFrameIndex =m_pgpurenderer->get_frame_index();

         pcommandlist->SetGraphicsRootDescriptorTable(0, prenderer->m_pheapCbv->GetGPUDescriptorHandleForHeapStart());

      }

      m_iPush = 0;

   }


   void shader::unbind()
   {

   }


   void shader::push_properties()
   {

      ::cast < renderer > prenderer = m_pgpurenderer;

      if (m_propertiesPush.size() <= 0)
      {

         return;

      }

      ::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::cast <device> pgpudevice = pgpucontext->m_pgpudevice;

      auto iSetSize = ::directx12::Align256(m_propertiesPush.size());
      CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
      UINT8* pPushProperties;
      auto hrMap = m_presourcePushProperties->Map(0, &readRange, (void**) & pPushProperties);
      defer_throw_hresult(hrMap);
      memcpy(pPushProperties + ::directx12::Align256(m_propertiesPush.size())*m_iPush, m_propertiesPush.data(), m_propertiesPush.size());
      m_presourcePushProperties->Unmap(0, nullptr);

      auto pcommandbuffer = prenderer->getCurrentCommandBuffer2();

      
      auto address = m_presourcePushProperties->GetGPUVirtualAddress() + m_iPush* ::directx12::Align256(m_propertiesPush.size()) ;
      pcommandbuffer->m_pcommandlist->SetGraphicsRootConstantBufferView(1, address);
      m_iPush++;

   }


   void shader::draw()
   {

      ::cast < ::gpu_directx12::renderer > prenderer = m_pgpurenderer;

      ::cast < ::gpu_directx12::context > pcontext = prenderer->m_pgpucontext;

      auto pcommandbuffer = prenderer->getCurrentCommandBuffer2();

      auto pcommandlist = pcommandbuffer->m_pcommandlist;

      pcommandlist->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

      pcommandlist->DrawInstanced(6, 1, 0, 0);

   }


} // namespace gpu_directx12



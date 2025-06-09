#include "framework.h"
// Co-creating with V0idsEmbrace@Twitch with
// camilo on 2025-05-19 04:59 <3ThomasBorregaardSorensen!!
#include "approach.h"
#include "buffer.h"
#include "shader.h"
#include "context.h"
#include "descriptors.h"
#include "renderer.h"
#include "texture.h"
#include "offscreen_render_target_view.h"
#include "aura/graphics/gpu/types.h"
#include "acme_windows_common/hresult_exception.h"
#include <d3dcompiler.h>
//#include "aura/user/user/graphics3d.h"



namespace gpu_directx12
{


   shader::shader()
   {

      //m_vktopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

   }


   shader::~shader()
   {

      ::cast < context > pgpucontext = m_pgpurenderer->m_pgpucontext;

      // vkDestroyPipelineLayout(pgpucontext->logicalDevice(), m_vkpipelinelayout, nullptr);

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
         ranges.set_size(iCount);
         rootParameters.set_size(iCount);
         //ranges.set_size(1);
         //rootParameters.set_size(1);
         //ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, iCount, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
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



   void shader::create_vertex_and_pixel_shader(const ::block& blockVertex, const ::block& blockPixel)
   {


      ::array < D3D12_INPUT_ELEMENT_DESC > layout;

      UINT uOffset0 = offsetof(gpu::Vertex, position);
      UINT uOffset1 = offsetof(gpu::Vertex, color);
      UINT uOffset2 = offsetof(gpu::Vertex, normal);
      UINT uOffset3 = offsetof(gpu::Vertex, uv);

      layout.add({ "POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, uOffset0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
      layout.add({ "COLOR"    , 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, uOffset1, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
      layout.add({ "NORMAL"   , 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, uOffset2, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
      layout.add({ "TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT    , 0, uOffset3, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });


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

      ///auto pblobShader = create_vertex_shader_blob(block);

      //::cast < device > pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      //auto hresult = pgpudevice->m_pdevice->CreateVertexShader(
      //   pblobShader->GetBufferPointer(),
      //   pblobShader->GetBufferSize(),
      //   nullptr,
      //   &m_pvertexshader
      //);

      //if (FAILED(hresult))
      //{

      //   throw ::hresult_exception(hresult);

      //}

      //::array < D3D11_INPUT_ELEMENT_DESC > layout;

      //UINT uOffset0 = offsetof(gpu::Vertex, position);
      //UINT uOffset1 = offsetof(gpu::Vertex, color);
      //UINT uOffset2 = offsetof(gpu::Vertex, normal);
      //UINT uOffset3 = offsetof(gpu::Vertex, uv);
      //
      //layout.add({ "POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, uOffset0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
      //layout.add({ "COLOR"    , 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, uOffset1, D3D11_INPUT_PER_VERTEX_DATA, 0 });
      //layout.add({ "NORMAL"   , 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, uOffset2, D3D11_INPUT_PER_VERTEX_DATA, 0 });
      //layout.add({ "TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT    , 0, uOffset3, D3D11_INPUT_PER_VERTEX_DATA, 0 });
      //

      //auto data = layout.data();
      //auto size = layout.size();
      //
      ////ID3D11InputLayout* inputLayout = nullptr;
      //hresult = pgpudevice->m_pdevice->CreateInputLayout(
      //   data,
      //   size,
      //   pblobShader->GetBufferPointer(),
      //   pblobShader->GetBufferSize(),
      //   &m_pinputlayout
      //);

      //if (FAILED(hresult))
      //{

      //   throw ::hresult_exception(hresult);

      //}
   }


   //void shader::create_pixel_shader(const ::block& block)
   //{

   //   auto pblobShader = create_pixel_shader_blob(block);

   //   ::cast < device > pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

   //   pgpudevice->m_pdevice->CreatePixelShader(
   //      pblobShader->GetBufferPointer(),
   //      pblobShader->GetBufferSize(),
   //      nullptr,
   //      &m_ppixelshader
   //   );

   //}


   //void shader::_create_pipeline_layout(int iSize)
   //{

   //   ::cast < context > pgpucontext = m_pgpurenderer->m_pgpucontext;

   //   ::cast < device > pgpudevice = pgpucontext->m_pgpudevice;

   //   //VkPushConstantRange pushConstantRange{};
   //   //pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
   //   //pushConstantRange.offset = 0;
   //   ////pushConstantRange.size = sizeof(PointLightPushConstants);
   //   //pushConstantRange.size = iSize;

   //   //::array<VkDescriptorSetLayout> descriptorSetLayouts;

   //   //if (m_edescriptorsetslota.contains(e_descriptor_set_slot_global))
   //   //{

   //   //   auto globalSetLayout = pgpucontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();

   //   //   descriptorSetLayouts.add(globalSetLayout);

   //   //}

   //   //if (m_pLocalDescriptorSet)
   //   //{

   //   //   ::cast < ::gpu_directx12::set_descriptor_layout > pset = m_pLocalDescriptorSet;

   //   //   auto setLayout = pset->getDescriptorSetLayout();

   //   //   descriptorSetLayouts.add(setLayout);

   //   //}

   //   //VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
   //   //pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
   //   //pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
   //   //pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();

   //   //if (iSize > 0)
   //   //{
   //   //   pipelineLayoutInfo.pushConstantRangeCount = 1;
   //   //   pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
   //   //}
   //   //else
   //   //{
   //   //   pipelineLayoutInfo.pushConstantRangeCount = 0;
   //   //   pipelineLayoutInfo.pPushConstantRanges = NULL;


   //   //}

   //   ////pipelineLayoutInfo.pPushConstantRanges = nullptr;

   //   //if (vkCreatePipelineLayout(
   //   //   pgpucontext->logicalDevice(),
   //   //   &pipelineLayoutInfo,
   //   //   nullptr,
   //   //   &m_vkpipelinelayout) !=
   //   //   VK_SUCCESS)
   //   //{

   //   //   throw ::exception(error_failed, "failed to create pipeline layout!");

   //   //}

   //}


   void shader::on_initialize_shader()
   {

      //_create_pipeline_layout(m_properties.m_memory.size());

      //__construct_new(m_ppipeline);

      ::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::cast <device> pgpudevice = pgpucontext->m_pgpudevice;

      ::cast <renderer> prenderer = m_pgpurenderer;

      //PipelineConfigInfo pipelineConfig{};

      ::cast < shader_vertex_input > pshadervertexinput = m_pVertexInput;


      pgpudevice->defer_shader_memory(m_memoryVertex, m_pathVertex);
      pgpudevice->defer_shader_memory(m_memoryFragment, m_pathFragment);


      create_vertex_and_pixel_shader(m_memoryVertex, m_memoryFragment);
      //create_pixel_shader(m_memoryFragment);



      //if (pshadervertexinput)
      //{

      //   pipelineConfig.bindingDescriptions.copy(pshadervertexinput->m_bindings);
      //   pipelineConfig.attributeDescriptions.copy(pshadervertexinput->m_attribs);

      //}

      ///pipeline::defaultPipelineConfigInfo(pipelineConfig);

      //if (m_eflag & e_flag_clear_default_bindings_and_attributes_descriptions)
      //{

      //   pipelineConfig.attributeDescriptions.clear();
      //   pipelineConfig.bindingDescriptions.clear();

      //}
      //if (m_bDepthTestButNoDepthWrite)
      //{
      //   pipelineConfig.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
      //   pipelineConfig.depthStencilInfo.depthTestEnable = VK_TRUE;
      //   pipelineConfig.depthStencilInfo.depthWriteEnable = VK_FALSE;
      //   pipelineConfig.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
      //      pipelineConfig.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
      //   pipelineConfig.depthStencilInfo.stencilTestEnable = VK_FALSE;


      //}
      //else if (m_bDisableDepthTest)
      //{

      //   pipelineConfig.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
      //   pipelineConfig.depthStencilInfo.depthTestEnable = VK_FALSE;
      //   pipelineConfig.depthStencilInfo.depthWriteEnable = VK_FALSE;
      //   pipelineConfig.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_ALWAYS; // doesn't matter since test is disabled
      //   pipelineConfig.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
      //   pipelineConfig.depthStencilInfo.stencilTestEnable = VK_FALSE;

      //}
      //pipelineConfig.colorBlendAttachments.clear();
      //if (m_bAccumulationEnable)
      //{

      //   VkPipelineColorBlendAttachmentState state;
      //   state.blendEnable = VK_TRUE;
      //   state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // use alpha blending
      //   state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
      //   state.colorBlendOp = VK_BLEND_OP_ADD;
      //   state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
      //   state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
      //   state.alphaBlendOp = VK_BLEND_OP_ADD;
      //   state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      //      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
      //   pipelineConfig.colorBlendAttachments.add(state);
      //   if (m_iColorAttachmentCount > 1)
      //   {

      //      state.blendEnable = VK_TRUE;
      //      state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // use alpha blending
      //      state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
      //      state.colorBlendOp = VK_BLEND_OP_ADD;
      //      state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
      //      state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
      //      state.alphaBlendOp = VK_BLEND_OP_ADD;
      //      state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT;
      //      pipelineConfig.colorBlendAttachments.add(state);
      //   }

      //   pipelineConfig.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
      //   pipelineConfig.colorBlendInfo.logicOpEnable = VK_FALSE;
      //   pipelineConfig.colorBlendInfo.attachmentCount = pipelineConfig.colorBlendAttachments.get_count();
      //   pipelineConfig.colorBlendInfo.pAttachments = pipelineConfig.colorBlendAttachments.data();

      //}
      //else if (m_bEnableBlend)
      //{

      //   for (int i = 0; i < m_iColorAttachmentCount; i++)
      //   {
      //      VkPipelineColorBlendAttachmentState state;
      //      state.blendEnable = VK_TRUE;
      //      state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
      //      state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
      //      state.colorBlendOp = VK_BLEND_OP_ADD;
      //      state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
      //      state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
      //      state.alphaBlendOp = VK_BLEND_OP_ADD;
      //      //state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // use alpha blending
      //      //state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
      //      //state.colorBlendOp = VK_BLEND_OP_ADD;
      //      //state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
      //      //state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
      //      //state.alphaBlendOp = VK_BLEND_OP_ADD;
      //      state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      //         VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
      //      pipelineConfig.colorBlendAttachments.add(state);
      //   }

      //   pipelineConfig.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
      //   pipelineConfig.colorBlendInfo.logicOpEnable = VK_FALSE;
      //   pipelineConfig.colorBlendInfo.attachmentCount = pipelineConfig.colorBlendAttachments.get_count();
      //   pipelineConfig.colorBlendInfo.pAttachments = pipelineConfig.colorBlendAttachments.data();

      //}
      //else
      //{

      //   for (int i = 0; i < m_iColorAttachmentCount; i++)
      //   {
      //      VkPipelineColorBlendAttachmentState state;
      //      state.colorWriteMask =
      //         VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
      //         VK_COLOR_COMPONENT_A_BIT;
      //      state.blendEnable = VK_FALSE;
      //      state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
      //      state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
      //      state.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
      //      state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
      //      state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
      //      state.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional
      //      pipelineConfig.colorBlendAttachments.add(state);
      //   }

      //   pipelineConfig.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
      //   pipelineConfig.colorBlendInfo.logicOpEnable = VK_FALSE;
      //   pipelineConfig.colorBlendInfo.attachmentCount = pipelineConfig.colorBlendAttachments.get_count();
      //   pipelineConfig.colorBlendInfo.pAttachments = pipelineConfig.colorBlendAttachments.data();

      //}

      //pipelineConfig.inputAssemblyInfo.topology = m_vktopology;
      //pipelineConfig.dynamicStateEnables.append_unique(m_dynamicstateaEnable);
      //pipelineConfig.dynamicStateInfo.dynamicStateCount = pipelineConfig.dynamicStateEnables.size();
      //auto prendertargetview = prenderer->m_prendertargetview;
      //pipelineConfig.renderPass = prendertargetview->m_vkrendertargetview;
      //pipelineConfig.pipelineLayout = m_vkpipelinelayout;


      /*m_ppipeline->initialize_pipeline(m_pgpurenderer,
         m_memoryVertex,
         m_memoryFragment,
         pipelineConfig);
*/

   }




   void shader::bind()
   {

      ::cast < ::gpu_directx12::renderer > prenderer = m_pgpurenderer;

      ::cast < ::gpu_directx12::context > pcontext = prenderer->m_pgpucontext;

      auto pcommandbuffer = prenderer->getCurrentCommandBuffer2();

      auto pcommandlist = pcommandbuffer->m_pcommandlist;

      // 1. Set the pipeline state object
      pcommandlist->SetPipelineState(m_ppipelinestate);  // ID3D12PipelineState*

      auto& ptexture = prenderer->m_prendertargetview->m_texturea[prenderer->get_frame_index()];

      //auto pheap = ptexture->m_pheapRenderTargetView;

      //// Bind descriptor heap
      //ID3D12DescriptorHeap* heaps[] = { pheap };

      //pcommandlist->SetDescriptorHeaps(1, heaps);

      if (m_prootsignature)
      {

         pcommandlist->SetGraphicsRootSignature(m_prootsignature);

      }

      if (m_edescriptorsetslota.contains(e_descriptor_set_slot_global)
         || m_edescriptorsetslota.contains(e_descriptor_set_slot_local))
      {

         auto iFrameIndex = pcontext->m_pgpurenderer->get_frame_index();

         /*auto pbuffer = pcontext->m_uboBuffers[iFrameIndex];

         auto pheap = pbuffer->m_pheap;

         ID3D12DescriptorHeap* heaps[] = { pheap };

         pcommandlist->SetDescriptorHeaps(1, heaps);*/


      }

      if (m_edescriptorsetslota.contains(e_descriptor_set_slot_global))
      {

         auto iFrameIndex = pcontext->m_pgpurenderer->get_frame_index();

         ///auto pbuffer = pcontext->m_uboBuffers[iFrameIndex];

         //auto pheap = pbuffer->m_pheap;

         //ID3D12DescriptorHeap* heaps[] = { pheap };

         //pcommandlist->SetDescriptorHeaps(1, heaps);

         //pcommandlist->SetGraphicsRootConstantBufferView(1, pbuffer->m_presourceBuffer->GetGPUVirtualAddress());
         //auto pbuffer = prenderer->m_presourceConstantBuffer;
         //auto gpuaddress = pbuffer->GetGPUVirtualAddress();

         // Set root descriptor table (assumes root parameter 0 is CBV)
         pcommandlist->SetGraphicsRootDescriptorTable(0, prenderer->m_cbvHeap->GetGPUDescriptorHandleForHeapStart());      
      }


      //// Push constants
      //pcommandlist->SetGraphicsRoot32BitConstants(0, 4, color, 0);

      //if (m_presourceConstantBuffer)
      //{
      //   // Bind constant buffer
      //   pcommandlist->SetGraphicsRootConstantBufferView(1, m_presourceConstantBuffer->GetGPUVirtualAddress());

      //}

      //::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      //::cast <device> pgpudevice = pgpucontext->m_pgpudevice;

      //::cast <renderer> prenderer = m_pgpurenderer;

      //pgpucontext->m_pgpucontext->IASetInputLayout(m_pinputlayout);

      ////defer_throw_hresult(hr1);

      //pgpucontext->m_pgpucontext->VSSetShader(m_pvertexshader, nullptr, 0);

      /////defer_throw_hresult(hr2);

      //pgpucontext->m_pcontext->PSSetShader(m_ppixelshader, nullptr, 0);

      //if (prenderer)
      //{

      //   auto pgpurendertargetview = prenderer->m_prendertargetview;

      //   if (pgpurendertargetview)
      //   {

      //      ::cast < offscreen_render_target_view > poffscreenrendertargetview = pgpurendertargetview;

      //      if (poffscreenrendertargetview)
      //      {

      //         auto pshaderresourceview = poffscreenrendertargetview->m_pshaderresourceview;

      //         if (pshaderresourceview)
      //         {

      //            pgpucontext->m_pcontext->PSSetShaderResources(0, 1, pshaderresourceview.pp());

      //         }

      //      }

      //   }


      //}

      ////defer_throw_hresult(hr3);

      ///*auto commandBuffer = prenderer->getCurrentCommandBuffer();

      //m_ppipeline->bind(commandBuffer);

      //if (m_edescriptorsetslota.contains(e_descriptor_set_slot_global))
      //{

      //   auto globalDescriptorSet = pgpucontext->getGlobalDescriptorSet(prenderer);

      //   vkCmdBindDescriptorSets(
      //      commandBuffer,
      //      VK_PIPELINE_BIND_POINT_GRAPHICS,
      //      m_vkpipelinelayout,
      //      0,
      //      1,
      //      &globalDescriptorSet,
      //      0,
      //      nullptr);

      //}*/

   }


   void shader::unbind()
   {

      ::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::cast <device> pgpudevice = pgpucontext->m_pgpudevice;

      ::cast <renderer> prenderer = m_pgpurenderer;

      //pgpucontext->m_pcontext->IASetInputLayout(m_pinputlayout);

      //defer_throw_hresult(hr1);

      //if (m_pvertexshader)
      //{

      //   pgpucontext->m_pcontext->VSSetShader(nullptr, nullptr, 0);

      //}

      ///defer_throw_hresult(hr2);

      //if (m_ppixelshader)
      //{

      //   pgpucontext->m_pcontext->PSSetShader(m_ppixelshader, nullptr, 0);

      //}

      //if (prenderer)
      //{

      //   auto pgpurendertargetview = prenderer->m_prendertargetview;

      //   if (pgpurendertargetview)
      //   {

      //      ::cast < offscreen_render_target_view > poffscreenrendertargetview = pgpurendertargetview;

      //      if (poffscreenrendertargetview)
      //      {

      //         auto pshaderresourceview = poffscreenrendertargetview->m_pshaderresourceview;

      //         if (pshaderresourceview)
      //         {

      //            ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
      //            pgpucontext->m_pcontext->PSSetShaderResources(0, 1, nullSRV);

      //         }

      //      }

      //   }


      //}

      //::cast < ::gpu_directx12::renderer > prenderer = m_pgpurenderer;

      //::cast < ::gpu_directx12::context > pcontext = prenderer->m_pgpucontext;

      ///auto pcommandlist = prenderer->getCurrentCommandBuffer();

      // 1. Set the pipeline state object
      //pcommandlist->SetPipelineState(nullptr);  // ID3D12PipelineState*


   }


   void shader::push_properties()
   {

      ::cast < renderer > prenderer = m_pgpurenderer;

      //   auto commandBuffer = prenderer->getCurrentCommandBuffer();

      //   vkCmdPushConstants(
      //      commandBuffer,
      //      m_vkpipelinelayout,
      //      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
      //      0,
      //      m_properties.size(),
      //      m_properties.data());

      if (m_properties.size() <= 0)
      {

         return;

      }

      ::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::cast <device> pgpudevice = pgpucontext->m_pgpudevice;

      auto iSetSize = ::directx12::Align256(m_properties.size());

      //auto& iSizePushConstants = m_iaSizePushConstants.element_at_grow(m_pgpurenderer->get_frame_index());
      //auto& presourceConstantBuffer = m_resourceaConstantBuffer.element_at_grow(m_pgpurenderer->get_frame_index());
      //auto& pData = m_mapaConstantBuffer.element_at_grow(m_pgpurenderer->get_frame_index());
      //if (iSetSize != iSizePushConstants || !presourceConstantBuffer)
      //{

      //   if (presourceConstantBuffer && pData)
      //   {

      //      presourceConstantBuffer->Unmap(0, nullptr);

      //   }

      //   presourceConstantBuffer.Release();

      //   ////using namespace Microsoft::WRL;

      //   //// -- Root Parameters: [0] Push constants, [1] Constant buffer
      //   //CD3DX12_ROOT_PARAMETER rootParams[2];
      //   //rootParams[0].InitAsConstants(4, 0); // float4 color -> b0
      //   //rootParams[1].InitAsConstantBufferView(1); // matrix -> b1

      //   //CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc;
      //   //rootSigDesc.Init(_countof(rootParams), rootParams, 0, nullptr,
      //   //   D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

      //   //::comptr<ID3DBlob> pblobSignature;
      //   //::comptr<ID3DBlob> pblobSignatureError;
      //   //D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pblobSignature, &pblobSignatureError);
      //   //pgpudevice->m_pdevice->CreateRootSignature(0, 
      //   //   pblobSignature->GetBufferPointer(), 
      //   //   pblobSignature->GetBufferSize(),
      //   //   __interface_of(m_prootsignature));

      //   iSizePushConstants = iSetSize;

      //   // -- Constant buffer (256 bytes aligned)
      //   CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
      //   CD3DX12_RESOURCE_DESC bufDesc = CD3DX12_RESOURCE_DESC::Buffer(iSetSize);

      //   pgpudevice->m_pdevice->CreateCommittedResource(
      //      &heapProps,
      //      D3D12_HEAP_FLAG_NONE,
      //      &bufDesc,
      //      D3D12_RESOURCE_STATE_GENERIC_READ,
      //      nullptr,
      //      __interface_of(presourceConstantBuffer));

      //   auto iFrameIndex = pgpucontext->m_pgpurenderer->get_frame_index();

      //   //auto pbuffer = pgpucontext->m_uboBuffers[iFrameIndex];

      //   //auto pheap = pbuffer->m_pheap;

      //   D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
      //   cbvDesc.BufferLocation = presourceConstantBuffer->GetGPUVirtualAddress();
      //   cbvDesc.SizeInBytes = iSetSize; // must be 256-byte aligned

      //   //pgpudevice->m_pdevice->CreateConstantBufferView(&cbvDesc, pheap->GetCPUDescriptorHandleForHeapStart());

      //   ///m_pbufferPushConstants.Release();

      //   //D3D11_BUFFER_DESC cbDesc = {};
      //   //cbDesc.ByteWidth = (m_properties.size() + 15) & ~15;
      //   //cbDesc.Usage = D3D11_USAGE_DYNAMIC;
      //   //cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
      //   //cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

      //   //auto pdevice = pgpudevice->m_pdevice;

      //   //HRESULT hr = pdevice->CreateBuffer(&cbDesc, nullptr, &m_pbufferPushConstants);

      //   //if (FAILED(hr))
      //   //{

      //   //   throw ::hresult_exception(hr);

      //   //}

      //   

      //   //UINT8* pData = nullptr;
      //   D3D12_RANGE range = { 0, 0 }; // We don’t intend to read from it
      //   presourceConstantBuffer->Map(0, &range, reinterpret_cast<void**>(&pData));


      //}

      //::cast < renderer > prenderer = m_pgpurenderer;
      //int iAddress = prenderer->m_iPushPropertiesAddress;

      //pCommandList->SetGraphicsRootConstantBufferView(0, pFrame->CameraAddress);
      memcpy(prenderer->m_pPushProperties, m_properties.data(), m_properties.size());

      //::cast < renderer > prenderer = m_pgpurenderer;

      auto pcommandbuffer = prenderer->getCurrentCommandBuffer2();

      pcommandbuffer->m_pcommandlist->SetGraphicsRootConstantBufferView(1, prenderer->m_presourcePushProperties->GetGPUVirtualAddress());


      //PushConstants pc = { XMFLOAT4(1, 0, 0, 1), currentTime };

      //D3D11_MAPPED_SUBRESOURCE mapped;
      //HRESULT hrMap = pgpucontext->m_pcontext->Map(m_pbufferPushConstants, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

      //if (FAILED(hrMap))
      //{

      //   throw ::hresult_exception(hrMap);

      //}

      //memcpy(mapped.pData, m_properties.data(), m_properties.size());
      //pgpucontext->m_pcontext->Unmap(m_pbufferPushConstants, 0);


      //pgpucontext->m_pcontext->VSSetConstantBuffers(0, 1, pgpucontext->m_pbufferGlobalUbo.pp());
      //pgpucontext->m_pcontext->PSSetConstantBuffers(0, 1, pgpucontext->m_pbufferGlobalUbo.pp());


      //auto pVS = m_pbufferPushConstants.m_p;
      //pgpucontext->m_pcontext->VSSetConstantBuffers(1, 1, &pVS);
      //auto pPS = m_pbufferPushConstants.m_p;
      //pgpucontext->m_pcontext->PSSetConstantBuffers(1, 1, &pPS);



   }


   void shader::draw()
   {


      ::cast < ::gpu_directx12::renderer > prenderer = m_pgpurenderer;

      ::cast < ::gpu_directx12::context > pcontext = prenderer->m_pgpucontext;

      auto pcommandbuffer = prenderer->getCurrentCommandBuffer2();

      auto pcommandlist = pcommandbuffer->m_pcommandlist;

      //::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;
      pcommandlist->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

      pcommandlist->DrawInstanced(6, 1, 0, 0);

      //   ::cast < renderer > prenderer = m_pgpurenderer;

      //   auto commandBuffer = prenderer->getCurrentCommandBuffer();

      //   vkCmdDraw(commandBuffer, 6, 1, 0, 0);

   }


} // namespace gpu_directx12



//#include "shader.h"
//
//
//namespace gpu_directx12
//{
//
//
//   const char* shader_type_c_str(VkShaderStageFlagBits etype)
//   {
//
//      switch (etype)
//      {
//      case VK_SHADER_STAGE_VERTEX_BIT:
//         return "VERTEX";
//      case VK_SHADER_STAGE_FRAGMENT_BIT:
//         return "FRAGMENT";
//#if !defined(__APPLE__) && !defined(__ANDROID__)
//      case VK_SHADER_STAGE_GEOMETRY_BIT:
//         return "GEOMETRY";
//#endif
//      default:
//         return "(Unknown Shader Type)";
//      }
//
//   }
//
//
//   shader::shader()
//   {
//
//
//   }
//
//
//   shader::~shader()
//   {
//
//
//   }
//
//
//   unsigned int shader::create_shader(const ::string & strSource, VkShaderStageFlagBits type)
//   {
//
//      //unsigned int uShader;
//
//      //uShader = glCreateShader(type);
//
//      //if(!uShader)
//      //{
//
//      //   GLenum eerror = glGetError();
//      //   
//      //   auto errString = directx12_error_string(eerror);
//
//      //   informationf("error %d \"%s\"", eerror, errString);
//
//      //   return ::error_failed;
//
//      //}
//
//      //const char * sza[1];
//
//      //sza[0] = strSource;
//
//      //glShaderSource(uShader, 1, sza, NULL);
//
//      //glCompileShader(uShader);
//
//      //information() << "compiling shader : " << sza[0];
//
//      ////string strSummary;
//
//      ////::e_status estatus =
//      //
//      //shader_compile_errors(uShader, type, m_strError);
//
//      ////if (!estatus)
//      ////{
//
//      ////   return 0;
//
//      ////}
//      //m_strError.empty();
//
//      //return uShader;
//
//      return 0;
//
//
//   }
//
//
//   void shader::create_shader(const ::string & pszVertex, const ::string & pszFragment, const ::string & pszGeometry)
//   {
//
//      unsigned int uVertex = create_shader(pszVertex, VK_SHADER_STAGE_VERTEX_BIT);
//
//      unsigned int uFragment = create_shader(pszFragment, VK_SHADER_STAGE_FRAGMENT_BIT);
//      
//#if !defined(__APPLE__) && !defined(__ANDROID__)
//
//      unsigned int uGeometry = 0;
//
//      bool bGeometry = pszGeometry.trimmed().has_character();
//
//      if (bGeometry)
//      {
//
//         uGeometry = create_shader(pszGeometry, VK_SHADER_STAGE_GEOMETRY_BIT);
//
//      }
//      
//#endif
//
////      m_uId = glCreateProgram();
////
////      glAttachShader(m_uId, uVertex);
////
////      glAttachShader(m_uId, uFragment);
////      
////#if !defined(__APPLE__) && !defined(__ANDROID__)
////
////      if (bGeometry)
////      {
////
////         glAttachShader(m_uId, uGeometry);
////
////      }
////      
////#endif
////
////      glLinkProgram(m_uId);
////      
////      // Validate program
//////      glValidateProgram(m_uId);
////
////      string strSummary;
////
////      ///::e_status estatus =
////      
////      program_compile_errors(m_uId, strSummary);
////
////      //if(estatus.succeeded())
////      //{
////
////      //   return estatus;
////
////      //}
////
////      // delete the shaders
////      // they're linked into our program
////
////      glDeleteShader(uVertex);
////
////      glDeleteShader(uFragment);
////
////#if !defined(__APPLE__) && !defined(__ANDROID__)
////
////      if (bGeometry)
////      {
////
////         glDeleteShader(uGeometry);
////
////      }
////      
////#endif
////
//      //return ::success;
//
//   }
//
//
//   // activate the shader
//   // ------------------------------------------------------------------------
//   void shader::use()
//   {
//      
//      //glUseProgram(m_uId);
//      
//   }
//
//
//   void shader::setBool(const ::scoped_string & scopedstrName, bool b)
//   {
//
//      setInt(scopedstrName, b != false);
//
//      //GLint i = glGetUniformLocation(m_uId, pszName);
//
//      //glUniform1i(i, (int)value);
//
//   }
//
//
//   void shader::setInt(const ::scoped_string & scopedstrName, int i)
//   {
//
//      auto p = get_payload(scopedstrName);
//
//      if(!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, &i, sizeof(int));
//
//      //GLint i = glGetUniformLocation(m_uId, pszName);
//
//      //glUniform1i(i, value);
//
//      //int i = value != 0;
//      //m_memoryPushConstants.append(&i, sizeof(int));
//
//
//   }
//
//
//   void shader::setFloat(const ::scoped_string & scopedstrName, float f)
//   {
//
//      //GLint i = glGetUniformLocation(m_uId, pszName);
//
//      //glUniform1f(i, value);
//      //int i = value != 0;
//      auto p = get_payload(scopedstrName);
//
//      if (!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, &f, sizeof(float));
//
//   }
//
//
////   void shader::setVec2(const ::scoped_string & scopedstrName, const glm::vec2& value)
////   {
////
////      GLint i = glGetUniformLocation(m_uId, pszName);
////
////      glUniform2fv(i, 1, &value[0]);
////
////   }
//
//
//   void shader::setVec2(const ::scoped_string & scopedstrName, float x, float y)
//   {
//
//      auto p = get_payload(scopedstrName);
//
//      if (!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, &x, sizeof(float));
//      m_memoryPushConstants.set_at(p->m_iOffset + sizeof(float), &y, sizeof(float));
//
//   }
//
//
////   void shader::setVec3(const ::scoped_string & scopedstrName, const glm::vec3& value)
////   {
////
////      GLint i = glGetUniformLocation(m_uId, pszName);
////
////      glUniform3fv(i, 1, &value[0]);
////
////   }
//
//
//   void shader::setVec3(const ::scoped_string & scopedstrName, float x, float y, float z)
//   {
//      
//      auto p = get_payload(scopedstrName);
//
//      if (!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, &x, sizeof(float));
//      m_memoryPushConstants.set_at(p->m_iOffset + sizeof(float), &y, sizeof(float));
//      m_memoryPushConstants.set_at(p->m_iOffset + 2*sizeof(float), &z, sizeof(float));
//      
//   }
//
//
////   void shader::setVec4(const ::scoped_string & scopedstrName, const glm::vec4& value)
////   {
////
////      GLint i = glGetUniformLocation(m_uId, pszName);
////
////      glUniform4fv(i, 1, &value[0]);
////
////   }
//
//
//   void shader::setVec4(const ::scoped_string & scopedstrName, float x, float y, float z, float w)
//   {
//
//      auto p = get_payload(scopedstrName);
//
//      if (!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, &x, sizeof(float));
//      m_memoryPushConstants.set_at(p->m_iOffset + sizeof(float), &y, sizeof(float));
//      m_memoryPushConstants.set_at(p->m_iOffset + 2 * sizeof(float), &z, sizeof(float));
//      m_memoryPushConstants.set_at(p->m_iOffset + 3 * sizeof(float), &w, sizeof(float));
//
//   }
//
//
//   void shader::setMat2(const ::scoped_string & scopedstrName, const float a[2*2])
//   {
//
//      auto p = get_payload(scopedstrName);
//
//      if (!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, (void *) a, 2*2*sizeof(float));
//
//   }
//
//
//   void shader::setMat3(const ::scoped_string & scopedstrName, const float a[3*3])
//   {
//
//      auto p = get_payload(scopedstrName);
//
//      if (!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, a, 3 * 3 * sizeof(float));
//
//   }
//
//
//   void shader::setMat4(const ::scoped_string & scopedstrName, const float a[4*4])
//   {
//
//      auto p = get_payload(scopedstrName);
//
//      if (!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, a, 4 * 4 * sizeof(float));
//
//   }
//
//
//   void shader::shader_compile_errors(int shader, VkShaderStageFlagBits type, string & strSummary)
//   {
//
//      //::e_status estatus = ::success;
//
//      //GLint success = 0;
//
//      //GLchar infoLog[1024];
//
//      //glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//
//      //if (success)
//      //{
//
//      //   strSummary.formatf("SUCCESS::SHADER_COMPILATION (%s) \n -- --------------------------------------------------- -- \n", shader_type_c_str(type));
//
//      //}
//      //else
//      //{
//
//      //   glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
//
//      //   const char * psz = shader_type_c_str(type);
//
//      //   const char * pszLog = infoLog;
//
//      //   strSummary.formatf("error::SHADER_COMPILATION_ERROR of type: %s \n %s \n -- --------------------------------------------------- -- \n", psz, pszLog);
//
//      //   warning() << strSummary;
//
//      //   throw ::exception(error_failed, "Shader Compilation Error", strSummary);
//
//      //}
//
//      //information() << strSummary;
//
//      ////return estatus;
//
//   }
//
//
//   void shader::program_compile_errors(int program, string & strSummary)
//   {
//
//      ////::e_status estatus = ::success;
//
//      //GLint success;
//
//      //GLchar infoLog[1024];
//      //
//      //glGetProgramiv(program, GL_LINK_STATUS, &success);
//
//      //if (success)
//      //{
//
//      //   strSummary.formatf("SUCCESS::PROGRAM_LINKING \n -- --------------------------------------------------- -- \n");
//
//      //}
//      //else
//      //{
//
//      //   glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
//
//      //   strSummary.formatf("error::PROGRAM_LINKING_ERROR : \n %s \n -- --------------------------------------------------- -- \n", infoLog);
//
//      //   throw ::exception(error_failed);
//
//      //}
//
//      //information() << strSummary;
//
//      ////return estatus;
//
//   }
//
//
//   //::gpu::payload * shader::get_payload(const ::scoped_string & scopedstrPayload)
//   //{
//   //   
//   //   auto uniform = glGetUniformLocation(m_uId, strUniform);
//
//   //   return uniform;
//
//   //}
//
//
//} // namespace gpu_directx12
//
//
//

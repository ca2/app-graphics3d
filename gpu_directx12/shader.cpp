#include "framework.h"
// Co-creating with V0idsEmbrace@Twitch with
// camilo on 2025-05-19 04:59 <3ThomasBorregaardSorensen!!
#include "approach.h"
#include "binding.h"
#include "block.h"
//#include "buffer.h"
#include "command_buffer.h"
#include "context.h"
#include "descriptors.h"
#include "input_layout.h"
#include "offscreen_render_target_view.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "bred/gpu/binding.h"
#include "bred/gpu/layer.h"
#include "bred/gpu/types.h"
#include "acme/operating_system/windows_common/com/hresult_exception.h"
#include <d3dcompiler.h>

#include "bred/graphics3d/engine.h"
#include "bred/graphics3d/immersion_layer.h"
#include "bred/graphics3d/scene_base.h"
//#include "bred/user/user/graphics3d.h"





namespace gpu_directx12
{


   ::comptr<ID3DBlob> create_computer_shader_blob(const ::block &block)
   {

      comptr<ID3DBlob> pblobShader;
      comptr<ID3DBlob> pblobError;

      auto data = (const char *)block.data();

      auto size = block.size();

      HRESULT hr = D3DCompile(data, // pointer to shader source
                              size, // size of shader source
                              nullptr, // optional source name
                              nullptr, // macro definitions
                              nullptr, // include handler
                              "main", // entry point
                              "cs_5_0", // target profile (e.g., vs_5_0, ps_5_0)
                              0, // compile flags
                              0, // effect flags
                              &pblobShader, // compiled shader
                              &pblobError // error messages
      );

      if (FAILED(hr))
      {

         if (pblobError)
         {

            ::string strError((const char *)pblobError->GetBufferPointer(), pblobError->GetBufferSize());

            throw ::exception(error_failed);
         }
      }

      return pblobShader;
   }


   shader::shader()
   { m_dxgiformatRenderTargetView = DXGI_FORMAT_UNKNOWN;
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

         ///int i = 0;
         // Create a root signature consisting of a descriptor table with a single CBV.
         {
            D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

            // This is the highest version the sample supports. If CheckFeatureSupport succeeds, the HighestVersion returned will not be greater than this.
            featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

            if (FAILED(pgpudevice->m_pd3d12device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
            {
               featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
            }

            ::block_array<CD3DX12_DESCRIPTOR_RANGE1, 64> ranges;
            ::block_array<CD3DX12_ROOT_PARAMETER1, 64> rootParameters;

            // Static sampler at s0, space0
            //::array< CD3DX12_STATIC_SAMPLER_DESC > staticSamplers(pre_allocate_t{}, 64);
            // Versioned root signature

            //ranges.m_countAddUp = 64;
            //rootParameters.m_countAddUp = 64;
            //staticSamplers.m_countAddUp = 64;


            //int iCount = 0;
            //if (m_edescriptorsetslota.contains(e_descriptor_set_slot_global))
            //{
            //   iCount++;

            //}
            //if (m_edescriptorsetslota.contains(e_descriptor_set_slot_local))
            //{

            //   iCount++;

            //}
            //if (m_edescriptorsetslota.contains(e_descriptor_set_shader_resource_view_and_sampler))
            //{

            //   iCount += 2;

            //}
            //ranges.set_size(iCount);
            //rootParameters.set_size(iCount);
            //ranges.set_size(1);
            //rootParameters.set_size(1);
            ////ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, iCount, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
            //ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, iCount, 0);
            //rootParameters[0].InitAsDescriptorTable(1, &ranges[i],
            //   D3D12_SHADER_VISIBILITY_ALL); //|              D3D12_SHADER_VISIBILITY_PIXEL);

            if (m_pbindingslotseta && m_pbindingslotseta->has_global_ubo())
            {

               //ranges.øadd().Init(
               //   D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 
               //   1, 
               //   ranges.get_size(), 
               //   0, 
               //   D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);

               rootParameters.add_new().InitAsConstantBufferView(
                  0,
                  0,
                  D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC,
                  D3D12_SHADER_VISIBILITY_ALL); //|              D3D12_SHADER_VISIBILITY_PIXEL);

            }

            if (m_propertiesPushShared.m_pproperties)
            {

                              UINT ShaderRegister;

                     if (m_iPushConstants < 0)
                     {

                        ShaderRegister = 1;

                     }
                     else
                     {

                        ShaderRegister = m_iPushConstants;

                     }
                     rootParameters.add_new().InitAsConstantBufferView(
                ShaderRegister,
                    0, // space
                        D3D12_ROOT_DESCRIPTOR_FLAG_NONE,
                        D3D12_SHADER_VISIBILITY_ALL
                        );

                     m_iPushConstantsBufferIndex = rootParameters.get_upper_bound();
            }

            if (m_pbindingslotseta)
            {

               update_binding_slots();

               for (auto &pbindingslotset: *m_pbindingslotseta)
               {

                  if (pbindingslotset->m_pbindingset->first()->is_global_ubo())
                  {

                     // auto pscene = pgpucontext->m_pengine->m_pimmersionlayer->m_pscene;

                     // auto pblockGlobalUbo1 = pscene->global_ubo1(pgpucontext);
                     ////    auto globalSetLayout =
                     ////    pgpucontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();
                     ////    pipelineconfiguration.aaadescriptorSetLayouts.add(globalSetLayout);
                     // auto pgpubindingsetGlobalUbo = pgpucontext->global_ubo1_binding_set();

                     //::cast<::gpu_vulkan::binding_set> pbindingsetGlobalUbo = pgpubindingsetGlobalUbo;

                     // auto pdescriptorsetlayout = pbindingsetGlobalUbo->descriptor_set_layout(pgpucommandbuffer);

                     // descriptorsetlayouta.ø(pbindingslotset->m_iSet) = pdescriptorsetlayout;

                     continue;
                  }

                  ::cast<::gpu_directx12::binding_set> pbindingset = pbindingslotset->m_pbindingset;

                  int iBaseRegister1 = maximum(0, pbindingset->first()->m_iBindingPoint2);

                  auto &srvRange = ranges.add_new();
                  srvRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
                                pbindingset->get_count(), // t0..t4
                                iBaseRegister1 // base register t0
                  );

                  auto &samplerRange = ranges.add_new();
                  samplerRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER,
                                    1, // s0
                                    iBaseRegister1);

                  rootParameters.add_new().InitAsDescriptorTable(1, &srvRange, D3D12_SHADER_VISIBILITY_PIXEL);

                  pbindingset->m_iXYZ_Index = rootParameters.get_upper_bound();

                  rootParameters.add_new().InitAsDescriptorTable(1, &samplerRange, D3D12_SHADER_VISIBILITY_PIXEL);


                  pbindingset->m_iXYZ_IndexSampler = rootParameters.get_upper_bound();

                  //               CD3DX12_ROOT_SIGNATURE_DESC rsDesc(_countof(params), params, 0, nullptr,
                  //                                                D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
               }
            }

         //   if (m_edescriptorsetslota.contains(e_descriptor_set_slot_local))
         //   {



         //      //if (m_pLocalDescriptorSet)
         //      //{

         //      //   ::cast < ::gpu_directx12::descriptor_set_layout > pset = m_pLocalDescriptorSet;

         //      //   auto setLayout = pset->getDescriptorSetLayout();

         //      //   aaadescriptorSetLayouts.add(setLayout);

         //      //}

         ////      rootParameters.element_at_grow(1).InitAsConstantBufferView(1); // b1: ObjectMatrices
         //      //ranges[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, i, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);

         //      UINT ShaderRegister;

         //      if (m_iPushConstants < 0)
         //      {

         //         ShaderRegister = 1;

         //      }
         //      else
         //      {

         //         ShaderRegister = m_iPushConstants;

         //      }
         //      rootParameters.add_new().InitAsConstantBufferView(ShaderRegister);

         //      m_iPushConstantsBufferIndex = rootParameters.get_upper_bound();



         //   }

            //if (m_bindingSampler.is_set() || m_bindingCubeSampler.is_set())

            //if (has_image_sampler())
            //{

            //   auto pbindingslot = get_first_image_sampler_binding_slot();

            //   UINT ShaderRegister;

            //   ShaderRegister = pbindingslot->m_pbinding->m_iSlot;

            //   //if (m_bindingCubeSampler.is_set())
            //   //{

            //   //   ShaderRegister = m_bindingCubeSampler.m_uBinding;

            //   //}
            //   //else
            //   //{

            //   //   ShaderRegister = m_bindingSampler.m_uBinding;

            //   //}
            //   //
            //   //CD3DX12_DESCRIPTOR_RANGE texRange;
            //   ranges.add_new().Init(
            //      D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 
            //      1, 
            //      ShaderRegister, 
            //      0,
            //      D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);

            //   rootParameters.add_new().InitAsDescriptorTable(
            //      1,
            //      &ranges.element_at(ranges.get_upper_bound()),
            //      D3D12_SHADER_VISIBILITY_PIXEL);

            //   m_iShaderResourceViewDescriptorTableRootParameterIndex = rootParameters.get_upper_bound();
            //   //staticSamplers.add(CD3DX12_STATIC_SAMPLER_DESC(0,                                  // register s0
            //   //   D3D12_FILTER_MIN_MAG_MIP_LINEAR,
            //   //   D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            //   //   D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            //   //   D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            //   //   0.0f,
            //   //   1,
            //   //   D3D12_COMPARISON_FUNC_ALWAYS,
            //   //   D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK,
            //   //   0.0f, D3D12_FLOAT32_MAX,
            //   //   D3D12_SHADER_VISIBILITY_PIXEL,
            //   //   0                                   // register space 0
            //   //));

            //   //CD3DX12_DESCRIPTOR_RANGE samplerRange;
            //   ranges.add_new().Init(
            //      D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER,
            //      1, 
            //      ShaderRegister); // s0
            //   rootParameters.add_new().InitAsDescriptorTable(
            //      1,
            //      &ranges.last(), 
            //      D3D12_SHADER_VISIBILITY_PIXEL);

            //   m_iSamplerDescriptorTableRootParameterIndex = rootParameters.get_upper_bound();
            //   //CD3DX12_ROOT_PARAMETER rootParams[2];
            //   
            ////   CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc;
            ////   rootSigDesc.Init(_countof(rootParams), rootParams, 0, nullptr,
            ////      D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

            ////::comptr<ID3DBlob> pblobSignatureBlob;
            ////::comptr<ID3DBlob> pblobError;
            ////D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
            ////   &pblobSignatureBlob, &pblobError);

            ////pgpudevice->m_pd3d12device->CreateRootSignature(0, pblobSignatureBlob->GetBufferPointer(),
            ////   pblobSignatureBlob->GetBufferSize(), __interface_of(m_prootsignature));
            ////   )

            //}

       //     if (m_edescriptorsetslota.contains(e_descriptor_set_shader_resource_view_and_sampler))
       //     {
       //        // Range for SRV (Texture2D)
       //        rootParameters.øadd().InitAsShaderResourceView(
       //           0,
       //           0,
       //           D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC,
       //           D3D12_SHADER_VISIBILITY_PIXEL);
       //        //ranges.øadd().Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,     // Range type
       //        //   1,                                    // Number of descriptors
       //        //   0,                                    // Base shader register (t0)
       //        //   0,                                    // Register space
       //        //   D3D12_DESCRIPTOR_RANGE_FLAG_NONE,    // Flags
       //        //   D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND);
       //        //CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc;
       ////rootSigDesc.Init(1, &param, 0, nullptr,
       ////   D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
       // // Root parameter for SRV table
       //        //rootParameters.øadd().InitAsDescriptorTable(1,              // Number of ranges
       //        //   &ranges.last(),       // Range array
       //        //   D3D12_SHADER_VISIBILITY_PIXEL); // Only pixel shader needs this

       //        // Range for Sampler
       //        //ranges.øadd().Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, // Range type
       //        //   1,                                    // Number of descriptors
       //        //   0,                                    // Base shader register (s0)
       //        //   0,                                    // Register space
       //        //   D3D12_DESCRIPTOR_RANGE_FLAG_NONE,    // Flags
       //        //   D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND);

       //
       //        // Root parameter for Sampler table
       //        //rootParameters.øadd().InitAsDescriptorTable(1,              // Number of ranges
       //        //   &ranges.last(),       // Range array
       //        //   D3D12_SHADER_VISIBILITY_PIXEL); // Only pixel shader needs this

       //     }



            // Allow input layout and deny uneccessary access to certain pipeline stages.
            D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
               D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
               D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
               D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
               D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
            //|  D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

            auto uRootParametersSize = (UINT) rootParameters.size();
            auto pRootParametersData = rootParameters.data();

            //auto uSamplersSize = (UINT)staticSamplers.size();
            //auto pSamplersData = staticSamplers.data();

            CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
            rootSignatureDesc.Init_1_1(
               uRootParametersSize, pRootParametersData,
               //uSamplersSize, pSamplersData,
               0, nullptr,
               rootSignatureFlags);

            ::comptr<ID3DBlob> psignature;
            ::comptr<ID3DBlob> perror;
            HRESULT hrSerializeVersionedRootSignature = D3DX12SerializeVersionedRootSignature(
               &rootSignatureDesc, featureData.HighestVersion, &psignature, &perror);
            if (FAILED(hrSerializeVersionedRootSignature))
            {

               ::string strError;
               if (perror)
               {
                  strError = (char*)perror->GetBufferPointer();
                  ::OutputDebugStringA(strError);
               }
               throw ::exception(error_failed, "Failed to serialize root signature");
            }

            HRESULT hrCreateRootSignature = 
               pgpudevice->m_pd3d12device->CreateRootSignature(
                  0, 
                  psignature->GetBufferPointer(),
                  psignature->GetBufferSize(),
                  __interface_of(m_prootsignature));

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

      //      //   ::cast < ::gpu_directx12::descriptor_set_layout > pset = m_pLocalDescriptorSet;

      //      //   auto setLayout = pset->getDescriptorSetLayout();

      //      //   aaadescriptorSetLayouts.add(setLayout);

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

      //   HRESULT hrCreateRootSignature = pgpudevice->m_pd3d12device->CreateRootSignature(
      //      0,
      //      serializedRootSig->GetBufferPointer(),
      //      serializedRootSig->GetBufferSize(),
      //      __interface_of(m_prootsignature)
      //      );
      //   ::defer_throw_hresult(hrCreateRootSignature);

      //return rootSignature;

         auto pgpurendertarget = m_pgpurenderer->render_target();

         if (m_pbindingslotseta)
         {
            int iFrameCount = pgpurendertarget->m_pgpurenderer->m_pgpucontext->m_pgpudevice->get_frame_count();
            for (int iFrame = 0; iFrame < iFrameCount; iFrame++)
            {
               // auto &pheap = pbindingset->m_heapa1.ø(iFrame);
               auto &pheap = m_heapa1.atø(iFrame);
               if (!pheap)
               {


                  D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
                  srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
                  srvHeapDesc.NumDescriptors = m_iHeapCount;
                  srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

                  pgpudevice->m_pd3d12device->CreateDescriptorHeap(&srvHeapDesc, __interface_of(pheap));
               }

               // auto &pheapSampler = pbindingset->m_heapaSampler1.ø(iFrame);
               auto &pheapSampler = m_heapaSampler1.atø(iFrame);
               if (!pheapSampler)
               {

                  D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
                  srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
                  srvHeapDesc.NumDescriptors = m_iHeapSamplerCount;
                  srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

                  pgpudevice->m_pd3d12device->CreateDescriptorHeap(&srvHeapDesc, __interface_of(pheapSampler));
               }
            }
         //UINT inc = pgpudevice->m_pd3d12device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

         //int iFrameCount = pcontext->m_pgpurenderer->m_pgpurendertarget->get_frame_count();
         //for (int iSet = 0; iSet < m_pbindingslotseta->size(); iSet++)
         //{

         //   ::cast<::gpu_directx12::binding_set> pbindingset = binding_set(iSet);

         //   if (pbindingset->first()->is_image_sampler())
         //   {

         //      for (int iFrame = 0; iFrame < iFrameCount; iFrame++)
         //      {
         //         //auto &pheap = pbindingset->m_heapa1.ø(iFrame);
         //         auto &pheap = m_heapa1.ø(iFrame);
         //         if (!pheap)
         //         {


         //            D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
         //            srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
         //            srvHeapDesc.NumDescriptors = m_iHeapCount;
         //            srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

         //            pgpudevice->m_pd3d12device->CreateDescriptorHeap(&srvHeapDesc, __interface_of(pheap));
         //         }

         //         //auto &pheapSampler = pbindingset->m_heapaSampler1.ø(iFrame);
         //         auto &pheapSampler = m_heapaSampler1.ø(iFrame);
         //         if (!pheapSampler)
         //         {

         //            D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
         //            srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
         //            srvHeapDesc.NumDescriptors = m_iHeapSamplerCount;
         //            srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

         //            pgpudevice->m_pd3d12device->CreateDescriptorHeap(&srvHeapDesc, __interface_of(pheapSampler));
         //         }
         //      }
         //   }
         }
         
   }
      

           
         //   if (pbindingset->m_iXYZ_Index >= 0)
         //   {

         //      pcommandlist->SetGraphicsRootDescriptorTable(
         //         pbindingset->m_iXYZ_Index,
         //         pbindingset->m_pheap1->GetGPUDescriptorHandleForHeapStart()); // t0
         //   }
         //   if (pbindingset->m_iXYZ_IndexSampler >= 0)
         //   {
         //      pcommandlist->SetGraphicsRootDescriptorTable(
         //         pbindingset->m_iXYZ_IndexSampler,
         //         pbindingset->m_pheapSampler1->GetGPUDescriptorHandleForHeapStart()); // t0
         //   }
         //}
   //}


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


   inline DXGI_FORMAT input_layout_format_from_gpu_property_type(const ::gpu::enum_type & etype)
   {

      switch(etype)
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



   //void shader::create_vertex_and_pixel_shader(const ::block& blockVertex, const ::block& blockPixel)
   //{

   //   //::array < D3D12_INPUT_ELEMENT_DESC > layout;

   //   //auto countInputLayout = m_propertiesInputLayout.count();

   //   //if (countInputLayout > 0)
   //   //{

   //   //   int iSemanticIndex = 0;
   //   //   int iInputSlot = 0;
   //   //   D3D12_INPUT_CLASSIFICATION classification = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
   //   //   UINT DataStepRate = 0;
   //   //   int iOffset = 0;
   //   //   int iNextOffset = 0;

   //   //   for (::collection::index iInputLayout = 0; iInputLayout < countInputLayout; iInputLayout++)
   //   //   {

   //   //      auto pproperty = m_propertiesInputLayout.m_pproperties + iInputLayout;

   //   //      auto name = pproperty->m_pszName;
   //   //      auto type = pproperty->m_etype;
   //   //      auto offset = iNextOffset;
   //   //      iNextOffset = offset + input_layout_aligned_property_size(pproperty->get_item_size());

   //   //      D3D12_INPUT_ELEMENT_DESC desc{};

   //   //      desc.SemanticName = input_layout_semantic_name_from_gpu_property_name(name);
   //   //      desc.SemanticIndex = iSemanticIndex;
   //   //      desc.Format = input_layout_format_from_gpu_property_type(type);
   //   //      desc.InputSlot = iInputSlot;
   //   //      desc.AlignedByteOffset = offset;
   //   //      desc.InputSlotClass = classification;
   //   //      desc.InstanceDataStepRate = DataStepRate;

   //   //      layout.add(desc);

   //   //   }

   //   //   //// Input layout
   //   //   //D3D12_INPUT_ELEMENT_DESC layout[] = {
   //   //   //    {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
   //   //   //    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
   //   //   //};

   //   //}
   //  /* else
   //   {

   //      UINT uOffset0 = offsetof(gpu::Vertex, position);
   //      UINT uOffset1 = offsetof(gpu::Vertex, color);
   //      UINT uOffset2 = offsetof(gpu::Vertex, normal);
   //      UINT uOffset3 = offsetof(gpu::Vertex, uv);


   //      layout.add({ "POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, uOffset0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
   //      if (m_iVertexLevel == 2)
   //      {
   //         layout.add({ "COLOR"    , 0, DXGI_FORMAT_R32G32B32A32_FLOAT , 0, uOffset1, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
   //      }
   //      else
   //      {
   //         layout.add({ "COLOR"    , 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, uOffset1, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });


   //      }

   //      if (m_iVertexLevel > 2 || m_iVertexLevel < 0)
   //      {
   //         layout.add({ "NORMAL"   , 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, uOffset2, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
   //         layout.add({ "TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT    , 0, uOffset3, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

   //      }

   //   }*/

   //   //auto data = layout.data();

   //   //auto size = layout.size();

   //   ::cast < input_layout > pinputlayout = m_pinputlayout;

   //   auto pblobVertex = create_vertex_shader_blob(blockVertex);

   //   auto pblobPixel = create_pixel_shader_blob(blockPixel);

   //   ::cast < ::gpu_directx12::device > pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

   //   ::cast < ::gpu_directx12::context > pcontext = m_pgpurenderer->m_pgpucontext;

   //   create_root_signature();

   //   // Describe and create the graphics pipeline state object (PSO).
   //   D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
   //   if (pinputlayout)
   //   {
   //      psoDesc.InputLayout = pinputlayout->_get_d3d12_input_layout_desc();
   //   }
   //   psoDesc.pRootSignature = m_prootsignature;
   //   psoDesc.VS = CD3DX12_SHADER_BYTECODE(pblobVertex);
   //   psoDesc.PS = CD3DX12_SHADER_BYTECODE(pblobPixel);
   //   CD3DX12_RASTERIZER_DESC rasterizerdesc(D3D12_DEFAULT);

   //   rasterizerdesc.CullMode = as_d3d12_cull_mode(m_ecullmode);

   //   psoDesc.RasterizerState = rasterizerdesc;

   //   if (m_bEnableBlend)
   //   {
   //      D3D12_BLEND_DESC blendDesc = {};
   //      blendDesc.AlphaToCoverageEnable = FALSE;
   //      blendDesc.IndependentBlendEnable = FALSE;

   //      D3D12_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
   //      rtBlendDesc.BlendEnable = TRUE;
   //      rtBlendDesc.SrcBlend = D3D12_BLEND_ONE;
   //      rtBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
   //      rtBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
   //      rtBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
   //      rtBlendDesc.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
   //      rtBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
   //      rtBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

   //      blendDesc.RenderTarget[0] = rtBlendDesc;

   //      //D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
   //      // ... (fill in shaders, input layout, root signature, etc.)
   //      psoDesc.BlendState = blendDesc;
   //      psoDesc.NumRenderTargets = 1;
   //      psoDesc.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM;
   //   }
   //   else
   //   {
   //      psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
   //   }
   //   psoDesc.SampleMask = UINT_MAX;
   //   psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
   //   psoDesc.NumRenderTargets = 1;
   //   psoDesc.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM;
   //   psoDesc.SampleDesc.Count = 1;

   //   if (m_bDisableDepthTest)
   //   {
   //      
   //      D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
   //      depthStencilDesc.DepthEnable = FALSE;
   //      depthStencilDesc.StencilEnable = FALSE;
   //      psoDesc.DepthStencilState = depthStencilDesc;

   //   }
   //   else
   //   {

   //      D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
   //      depthStencilDesc.DepthEnable = TRUE;
   //      if (m_bDepthTestButNoDepthWrite)
   //      {
   //         depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
   //      }
   //      else
   //      {
   //         depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
   //      }
   //      if (m_bLequalDepth)
   //      {
   //         depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
   //      }
   //      else
   //      {
   //         depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;

   //      }
   //      depthStencilDesc.StencilEnable = FALSE;
   //      psoDesc.DepthStencilState = depthStencilDesc;
   //      psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT; // must match your DSV

   //   }

   //   HRESULT hrCreateGraphicsPipelineState =
   //      pgpudevice->m_pd3d12device->CreateGraphicsPipelineState(&psoDesc, __interface_of(m_ppipelinestate));

   //   ::defer_throw_hresult(hrCreateGraphicsPipelineState);

   //}


   void shader::_defer_set_current_pipeline(::gpu::command_buffer * pgpucommandbuffer, ::gpu::texture * pgputexture)
   {
      ::cast<::gpu_directx12::texture> ptexture = pgputexture;
      if (m_ppipelinestate &&
         m_dxgiformatRenderTargetView == ptexture->m_resourcedesc.Format &&
         m_bPipelineBlendEnabled == m_bEnableBlend)
      {

         return;

      }

      if (ptexture->m_resourcedesc.Format == DXGI_FORMAT_UNKNOWN)
      {

         ptexture->m_resourcedesc = ptexture->m_pd3d12resourceTexture->m_presource->GetDesc();
      }

      m_dxgiformatRenderTargetView = ptexture->m_resourcedesc.Format;

      m_ppipelinestate.release();
      
      //::array < D3D12_INPUT_ELEMENT_DESC > layout;

      // auto countInputLayout = m_propertiesInputLayout.count();

      // if (countInputLayout > 0)
      //{

      //   int iSemanticIndex = 0;
      //   int iInputSlot = 0;
      //   D3D12_INPUT_CLASSIFICATION classification = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
      //   UINT DataStepRate = 0;
      //   int iOffset = 0;
      //   int iNextOffset = 0;

      //   for (::collection::index iInputLayout = 0; iInputLayout < countInputLayout; iInputLayout++)
      //   {

      //      auto pproperty = m_propertiesInputLayout.m_pproperties + iInputLayout;

      //      auto name = pproperty->m_pszName;
      //      auto type = pproperty->m_etype;
      //      auto offset = iNextOffset;
      //      iNextOffset = offset + input_layout_aligned_property_size(pproperty->get_item_size());

      //      D3D12_INPUT_ELEMENT_DESC desc{};

      //      desc.SemanticName = input_layout_semantic_name_from_gpu_property_name(name);
      //      desc.SemanticIndex = iSemanticIndex;
      //      desc.Format = input_layout_format_from_gpu_property_type(type);
      //      desc.InputSlot = iInputSlot;
      //      desc.AlignedByteOffset = offset;
      //      desc.InputSlotClass = classification;
      //      desc.InstanceDataStepRate = DataStepRate;

      //      layout.add(desc);

      //   }

      //   //// Input layout
      //   //D3D12_INPUT_ELEMENT_DESC layout[] = {
      //   //    {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
      //   //    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
      //   //};

      //}
      /* else
       {

          UINT uOffset0 = offsetof(gpu::Vertex, position);
          UINT uOffset1 = offsetof(gpu::Vertex, color);
          UINT uOffset2 = offsetof(gpu::Vertex, normal);
          UINT uOffset3 = offsetof(gpu::Vertex, uv);


          layout.add({ "POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, uOffset0,
       D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }); if (m_iVertexLevel == 2)
          {
             layout.add({ "COLOR"    , 0, DXGI_FORMAT_R32G32B32A32_FLOAT , 0, uOffset1,
       D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
          }
          else
          {
             layout.add({ "COLOR"    , 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, uOffset1,
       D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });


          }

          if (m_iVertexLevel > 2 || m_iVertexLevel < 0)
          {
             layout.add({ "NORMAL"   , 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, uOffset2,
       D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }); layout.add({ "TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT    , 0,
       uOffset3, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

          }

       }*/

      // auto data = layout.data();

      // auto size = layout.size();

      ::cast<input_layout> pinputlayout = m_pinputlayout;

      auto pblobVertex = create_vertex_shader_blob(m_memoryVertex);

      auto pblobPixel = create_pixel_shader_blob(m_memoryFragment);

      ::cast<::gpu_directx12::device> pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      ::cast<::gpu_directx12::context> pcontext = m_pgpurenderer->m_pgpucontext;

      create_root_signature();

      // Describe and create the graphics pipeline state object (PSO).
      D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
      if (pinputlayout)
      {
         psoDesc.InputLayout = pinputlayout->_get_d3d12_input_layout_desc();
      }
      psoDesc.pRootSignature = m_prootsignature;
      psoDesc.VS = CD3DX12_SHADER_BYTECODE(pblobVertex);
      psoDesc.PS = CD3DX12_SHADER_BYTECODE(pblobPixel);
      CD3DX12_RASTERIZER_DESC rasterizerdesc(D3D12_DEFAULT);

      rasterizerdesc.CullMode = as_d3d12_cull_mode(m_ecullmode);

      psoDesc.RasterizerState = rasterizerdesc;

      if (m_bEnableBlend)
      {
         D3D12_BLEND_DESC blendDesc = {};
         blendDesc.AlphaToCoverageEnable = FALSE;
         blendDesc.IndependentBlendEnable = FALSE;

         D3D12_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
         rtBlendDesc.BlendEnable = TRUE;
         rtBlendDesc.SrcBlend = D3D12_BLEND_ONE;
         rtBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
         rtBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
         rtBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
         rtBlendDesc.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
         rtBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
         rtBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

         blendDesc.RenderTarget[0] = rtBlendDesc;

         // D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
         //  ... (fill in shaders, input layout, root signature, etc.)
         psoDesc.BlendState = blendDesc;
         psoDesc.NumRenderTargets = 1;
         psoDesc.RTVFormats[0] = m_dxgiformatRenderTargetView;
      }
      else
      {
         psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
      }
      psoDesc.SampleMask = UINT_MAX;
      psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
      psoDesc.NumRenderTargets = 1;
      psoDesc.RTVFormats[0] = m_dxgiformatRenderTargetView;
      psoDesc.SampleDesc.Count = 1;

      if (m_bDisableDepthTest)
      {

         D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
         depthStencilDesc.DepthEnable = FALSE;
         depthStencilDesc.StencilEnable = FALSE;
         psoDesc.DepthStencilState = depthStencilDesc;
      }
      else
      {

         D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
         depthStencilDesc.DepthEnable = TRUE;
         if (m_bDepthTestButNoDepthWrite)
         {
            depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
         }
         else
         {
            depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
         }
         if (m_bLequalDepth)
         {
            depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
         }
         else
         {
            depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
         }
         depthStencilDesc.StencilEnable = FALSE;
         psoDesc.DepthStencilState = depthStencilDesc;
         psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT; // must match your DSV
      }

      HRESULT hrCreateGraphicsPipelineState =
         pgpudevice->m_pd3d12device->CreateGraphicsPipelineState(&psoDesc, __interface_of(m_ppipelinestate));

      ::defer_throw_hresult(hrCreateGraphicsPipelineState);

      m_bPipelineBlendEnabled = m_bEnableBlend;

            //create_vertex_and_pixel_shader(m_memoryVertex, m_memoryFragment);

      // if (m_edescriptorsetslota.contains(e_descriptor_set_slot_local))
      //{
      //
      if (m_propertiesPushShared.size(true) > 0 && m_iPush >= m_iPushMax)
      {
         _on_more_push();
      }
   }
      

   void shader::on_initialize_shader()
   {

      ::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::cast <device> pgpudevice = pgpucontext->m_pgpudevice;

      ::cast <renderer> prenderer = m_pgpurenderer;

      ::cast < input_layout > pinputlayout = m_pinputlayout;

      pgpudevice->defer_shader_memory(m_memoryVertex, m_pathVertex);

      pgpudevice->defer_shader_memory(m_memoryFragment, m_pathFragment);



   }

   void shader::_on_more_push()
   {

      ::cast<renderer> prenderer = m_pgpurenderer;
            ::cast<context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::cast<device> pgpudevice = pgpucontext->m_pgpudevice;

               int iNumberOfObjects = 256;
      UINT constantBufferSize = ::directx12::Align256((UINT)m_propertiesPushShared.size(false)) *
                                iNumberOfObjects; // CB size is required to be 256-byte aligned.
      CD3DX12_HEAP_PROPERTIES heapproperties(D3D12_HEAP_TYPE_UPLOAD);
      auto resourcedesc = CD3DX12_RESOURCE_DESC::Buffer(constantBufferSize);
         HRESULT hrCreateCommittedResource = pgpudevice->m_pd3d12device->CreateCommittedResource(
            &heapproperties,
            D3D12_HEAP_FLAG_NONE,
            &resourcedesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            __interface_of(m_resourceaPushProperties.atø(m_iPush/256)));
         pgpudevice->defer_throw_hresult(hrCreateCommittedResource);


         UINT descriptorSize = pgpudevice->m_pd3d12device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

         D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
         cbvDesc.BufferLocation = m_resourceaPushProperties[m_iPush/256]->GetGPUVirtualAddress();
         cbvDesc.SizeInBytes = constantBufferSize;
         int iDescriptor = 1; // 1 - second descriptor, first descriptor (0) is the global ubo
         CD3DX12_CPU_DESCRIPTOR_HANDLE objectCBHandle(prenderer->m_pheapCbv->GetCPUDescriptorHandleForHeapStart(), iDescriptor, descriptorSize);
         pgpudevice->m_pd3d12device->CreateConstantBufferView(&cbvDesc, objectCBHandle);

         m_iPushMax += iNumberOfObjects;

   }


   //void shader::bind(::gpu::command_buffer * pgpucommandbuffer, ::gpu::texture *pgputextureTarget, ::gpu::texture *pgputextureSource)
   //{

   //   bind(pgpucommandbuffer, pgputextureTarget);

   //   bind_source(pgpucommandbuffer, pgputextureSource, 0);

   //}


   void shader::bind(::gpu::command_buffer *pgpucommandbuffer, ::gpu::texture *pgputextureTarget)
   {
      _defer_set_current_pipeline(pgpucommandbuffer, pgputextureTarget);

      _bind(pgpucommandbuffer);


      ::cast < ::gpu_directx12::texture > ptextureDst = pgputextureTarget;

      if (!ptextureDst->m_pheapRenderTargetView)
      {

         ptextureDst->create_render_target();

      }

      ::cast < renderer > prenderer = m_pgpurenderer;

      ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

      //::cast < command_buffer > pcommandbuffer = prenderer->getCurrentCommandBuffer2(::gpu::current_layer());
      if (ptextureDst->m_iTextureSerial == 0xd)
      {

         warning("m_iTextureSerial == 0xd");

      }
      ptextureDst->set_state(pcommandbuffer, ::gpu::e_texture_state_color_attachment);

      auto pcommandlist = pcommandbuffer->m_pcommandlist;

      D3D12_CPU_DESCRIPTOR_HANDLE handlea[] = {ptextureDst->current_layer().m_handleRenderTargetView};

      D3D12_CPU_DESCRIPTOR_HANDLE depth[1]{};
      D3D12_CPU_DESCRIPTOR_HANDLE * dpth=nullptr;
      if (ptextureDst->m_pheapDepthStencilView
         && !m_bDisableDepthTest)
      {
         depth[0] = ptextureDst->m_handleDepthStencilView;
         dpth = depth;
      }

      //if(ptextureDst->m_ph)

      pcommandlist->OMSetRenderTargets(
         1,
         handlea,
         FALSE,
         dpth
      );


   }


      void shader::defer_bind_frame_buffer_layer(::gpu::command_buffer *pgpucommandbuffer,
                                              ::gpu::texture *pgputextureTarget)
   {

      ::cast<texture> ptexture = pgputextureTarget;

      if (ptexture->m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
      {

         if (ptexture->m_iCurrentLayer >= 0)
         {

                  ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

            auto pcommandlist = pcommandbuffer->m_pcommandlist;

      D3D12_CPU_DESCRIPTOR_HANDLE handlea[] = {ptexture->current_layer().m_handleRenderTargetView};

            D3D12_CPU_DESCRIPTOR_HANDLE depth[1]{};
            D3D12_CPU_DESCRIPTOR_HANDLE *dpth = nullptr;
            if (ptexture->m_pheapDepthStencilView && !m_bDisableDepthTest)
            {
               depth[0] = ptexture->m_handleDepthStencilView;
               dpth = depth;
            }

            // if(ptextureDst->m_ph)

            pcommandlist->OMSetRenderTargets(1, handlea, FALSE, dpth);

         }
      }
   }

      void shader::on_bind_already_bound(::gpu::command_buffer *pgpucommandbuffer, ::gpu::texture *pgputextureTarget)
   {

      defer_bind_frame_buffer_layer(pgpucommandbuffer, pgputextureTarget);
   }

   void shader::bind_source(::gpu::command_buffer *pgpucommandbuffer, ::gpu::texture *ptextureSource, int iSlot)
   {

      int iFrameIndex = pgpucommandbuffer->m_iCommandBufferFrameIndex2;

      if (ptextureSource == m_pgputextureBound && 
         iFrameIndex == m_iFrameBound &&
         pgpucommandbuffer->m_iSerial == m_iCommandBufferSerialSourceBound)
      {

         return;

      }

      ::cast < ::gpu_directx12::texture > ptextureSrc = ptextureSource;

      if (!ptextureSrc->m_pheapShaderResourceView)
      {

         ptextureSrc->create_shader_resource();

      }

      ::cast < ::gpu_directx12::renderer > prenderer = m_pgpurenderer;

      //::cast < command_buffer > pcommandbuffer = prenderer->getCurrentCommandBuffer2(::gpu::current_layer());


      ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

      if (ptextureSrc->m_iTextureSerial == 0xd)
      {

         warning("ptextureSrc->m_iTextureSerial == 0xd");

      }

      ptextureSrc->set_state(pcommandbuffer, ::gpu::e_texture_state_shader_read);

      auto pcommandlist = pcommandbuffer->m_pcommandlist;

//      if (m_edescriptorsetslota.contains(e_descriptor_set_slot_global))
//      {
//
////         pcommandlist->SetGraphicsRootConstantBufferView(0, your_cbv_gpu_address);
//         pcommandlist->SetGraphicsRootDescriptorTable(1,
//            ptextureSrc->m_pheapShaderResourceView->GetGPUDescriptorHandleForHeapStart()); // t0
//         pcommandlist->SetGraphicsRootDescriptorTable(2,
//            ptextureSrc->m_pheapSampler->GetGPUDescriptorHandleForHeapStart()); // s0
//      }
//      else if (m_edescriptorsetslota.contains(e_descriptor_set_slot_local))
//      {
//
//         UINT RootParameterIndex;
//
//         if (m_iPushConstants < 0)
//         {
//
//            RootParameterIndex = 1;
//
//         }
//         else
//         {
//
//            RootParameterIndex = m_iPushConstants;
//
//         }
//
//         pcommandlist->SetGraphicsRootDescriptorTable(RootParameterIndex,
//            ptextureSrc->m_pheapShaderResourceView->GetGPUDescriptorHandleForHeapStart()); // t0
//         pcommandlist->SetGraphicsRootDescriptorTable(RootParameterIndex,
//            ptextureSrc->m_pheapSampler->GetGPUDescriptorHandleForHeapStart()); // s0
//
//      }
//      else
//      {
//
//      }
      //if (m_bindingCubeSampler.is_set())
      //{
      //   pcommandlist->SetGraphicsRootDescriptorTable(
      //      m_bindingCubeSampler.m_uBinding,
      //      ptextureSrc->m_pheapShaderResourceView->GetGPUDescriptorHandleForHeapStart()); // t0
      //   pcommandlist->SetGraphicsRootDescriptorTable(
      //      m_bindingCubeSampler.m_uBinding,
      //      ptextureSrc->m_pheapSampler->GetGPUDescriptorHandleForHeapStart()); // s0

      //}
      //else if (m_bindingSampler.is_set())
      //{

      //   pcommandlist->SetGraphicsRootDescriptorTable(
      //      m_bindingSampler.m_uBinding,
      //      ptextureSrc->m_pheapShaderResourceView->GetGPUDescriptorHandleForHeapStart()); // t0
      //   pcommandlist->SetGraphicsRootDescriptorTable(
      //      m_bindingSampler.m_uBinding,
      //      ptextureSrc->m_pheapSampler->GetGPUDescriptorHandleForHeapStart()); // s0

      //}

      
      //::cast<renderer> prenderer = m_pgpurenderer;

//      ::cast<::gpu_directxvulkan::shader> pshader = prenderer->m_pgpucontext->m_pshaderBound;

      //::cast<command_buffer> pcommandbuffer = ::gpu::current_command_buffer();
      //::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

      unsigned int uSet = 0;

      auto pbindingslot = get_first_image_sampler_binding_slot();

      uSet = pbindingslot->m_iSet;

      pbindingslot->m_ptexture = ptextureSrc;

      // if (m_bindingSampler.is_set())
      //    uSet = m_bindingSampler.m_uSet;
      // else if (m_bindingCubeSampler.is_set())
      //    uSet = m_bindingCubeSampler.m_uSet;

      //auto vkdescriptorset = ptexture->_001DescriptorSet(this, pgpucommandbuffer);

      //if ((((::uptr)vkdescriptorset) & 0xffff) == 0x357)
      //{

      //   // vkdescriptorset = ptexture->descriptor_set(this, pgpucommandbuffer);
      //}

      //// Bind pipeline and descriptor sets
      ////      vkCmdBindPipeline(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
      ////    vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, ...);
      //vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer,
      //                        VK_PIPELINE_BIND_POINT_GRAPHICS, // Bind point
      //                        pshader->m_ppipelineCurrent->_pipeline_layout(), // Layout used when pipeline was created
      //                        uSet, // First set (set = 0)
      //                        1, // Descriptor set count
      //                        &vkdescriptorset, // Pointer to descriptor set
      //                        0, // Dynamic offset count
      //                        NULL // Dynamic offsets
      //);


      ::cast<::gpu_directx12::binding_set> pbindingset = binding_set(uSet);
      ::cast<context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::cast<device> pgpudevice = pgpucontext->m_pgpudevice;
      if (pbindingset->m_iXYZ_Index >= 0)
      {


         //auto &pheap = pbindingset->m_heapa1[iFrame];
         auto &pheap = m_heapa1[iFrameIndex];

         // pbindingset->m_pheap1 Should had been created at create_root_signature()
         ASSERT(pheap);

         D3D12_CPU_DESCRIPTOR_HANDLE h = pheap->GetCPUDescriptorHandleForHeapStart();
         UINT inc = pgpudevice->m_pd3d12device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
         auto h2 = ptextureSrc->m_handleShaderResourceView;
         auto h1 = h;
         //auto iHeapIndex = pbindingset->m_iHeapIndex++;
         auto iHeapIndex = m_iHeapIndex++;
         auto iHeapIndex1 = iHeapIndex % m_iHeapCount;
         auto base = iHeapIndex1 * inc;
         h1.ptr += base;
         pgpudevice->m_pd3d12device->CopyDescriptorsSimple(1, h1, h2, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
         auto hGpu = pheap->GetGPUDescriptorHandleForHeapStart();
         hGpu.ptr += base;

         pcommandlist->SetGraphicsRootDescriptorTable(
            pbindingset->m_iXYZ_Index,
            hGpu); // t0
      }

      if (pbindingset->m_iXYZ_IndexSampler >= 0)
      {
         //auto &pheapSampler = pbindingset->m_heapaSampler1[iFrame];
         auto &pheapSampler = m_heapaSampler1[iFrameIndex];
         // pbindingset->m_pheapSampler1 Should had been created at create_root_signature()
         ASSERT(pheapSampler);
         //{
         //   D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
         //   srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
         //   srvHeapDesc.NumDescriptors = 1;
         //   srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

         //   pgpudevice->m_pd3d12device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&pbindingset->m_pheapSampler1));
            UINT inc = pgpudevice->m_pd3d12device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
            D3D12_CPU_DESCRIPTOR_HANDLE h = pheapSampler->GetCPUDescriptorHandleForHeapStart();
            auto h2 = ptextureSrc->m_handleSampler;
            auto h1 = h;
            //auto iHeapIndex = pbindingset->m_iHeapSamplerIndex++;
            auto iHeapIndex = m_iHeapSamplerIndex++;
            auto iHeapIndex1 = iHeapIndex % m_iHeapSamplerCount;
            auto base = iHeapIndex1 * inc;
            h1.ptr += base;
            pgpudevice->m_pd3d12device->CopyDescriptorsSimple(1, h1, h2, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

         //}
            auto hGpu = pheapSampler->GetGPUDescriptorHandleForHeapStart();
            hGpu.ptr += base;

         pcommandlist->SetGraphicsRootDescriptorTable(
            pbindingset->m_iXYZ_IndexSampler,
            hGpu); // t0
      }

      //if (pbindingset->m_iXYZ_Index >= 0 || pbindingset->m_iXYZ_IndexSampler >= 0)
      //{

      //   //::array<ID3D12DescriptorHeap *> heapa;

      //   //for (int i = 0; i < binding_slot_set_array()->size(); i++)
      //   //{
      //   //   auto pbindingslotset = binding_slot_set_array()->element_at(i);

      //   //   if (pbindingslotset->m_pbindingset->first()->is_image_sampler())
      //   //   {
      //   //      // heapa.add(ptextureDst->m_pheapRenderTargetView);
      //   //      ::cast<::gpu_directx12::binding_set> pbindingset = pbindingslotset->m_pbindingset;
      //   //   
      //   //      if (pbindingset->m_iXYZ_Index >= 0)
      //   //      {
      //   //         heapa.add(pbindingset->m_pheap1);
      //   //      }
      //   //      if (pbindingset->m_iXYZ_IndexSampler)
      //   //      {
      //   //         heapa.add(pbindingset->m_pheapSampler1);
      //   //      }
      //   //   }
      //   //}


      //   //pcommandlist->SetDescriptorHeaps((UINT)heapa.size(), heapa.data());


      //   if (pbindingset->m_iXYZ_IndexSampler >= 0)
      //   {
      //      pcommandlist->SetGraphicsRootDescriptorTable(
      //         pbindingset->m_iXYZ_IndexSampler,
      //         pbindingset->m_pheapSampler1->GetGPUDescriptorHandleForHeapStart()); // t0
      //   }

      //}

      //if (m_iShaderResourceViewDescriptorTableRootParameterIndex>=0)
      //{
      //      pcommandlist->SetGraphicsRootDescriptorTable(
      //         m_iShaderResourceViewDescriptorTableRootParameterIndex,
      //         ptextureSrc->m_pheapShaderResourceView->GetGPUDescriptorHandleForHeapStart()); // t0


      //}
      //if (m_iSamplerDescriptorTableRootParameterIndex>=0)
      //{
      //   pcommandlist->SetGraphicsRootDescriptorTable(
      //      m_iSamplerDescriptorTableRootParameterIndex,
      //      ptextureSrc->m_pheapSampler->GetGPUDescriptorHandleForHeapStart()); // t0


      //}

      m_pgputextureBound = ptextureSource;

      m_iFrameBound = iFrameIndex;

      m_iCommandBufferSerialSourceBound = pgpucommandbuffer->m_iSerial;

   }


   void shader::_bind(::gpu::command_buffer *pgpucommandbuffer)
   {

      bool bNewFrame = m_iFrameBound != pgpucommandbuffer->m_iCommandBufferFrameIndex2;

      m_iFrameBound = pgpucommandbuffer->m_iCommandBufferFrameIndex2;

      ::cast < ::gpu_directx12::renderer > prenderer = m_pgpurenderer;

      ::cast < ::gpu_directx12::context > pcontext = prenderer->m_pgpucontext;

      //::cast < command_buffer > pcommandbuffer = prenderer->getCurrentCommandBuffer2(::gpu::current_layer());
      ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

      auto pcommandlist = pcommandbuffer->m_pcommandlist;

      pcommandlist->SetPipelineState(m_ppipelinestate);  // ID3D12PipelineState*

      if (m_prootsignature)
      {

         pcommandlist->SetGraphicsRootSignature(m_prootsignature);

      }

      m_iCommandBufferSerialPipelineBound = pgpucommandbuffer->m_iSerial;

      auto iFrameIndex = pgpucommandbuffer->m_iCommandBufferFrameIndex2;

      //if (m_edescriptorsetslota.contains(e_descriptor_set_slot_global))
      if (has_global_ubo())
      {


         auto pscene = pcontext->m_pengine->m_pimmersionlayer->m_pscene;

         auto pgpublockGlobalUbo1 = pscene->global_ubo1(pcontext);

         ::cast<::gpu_directx12::block > pblockGlobalUbo1 = pgpublockGlobalUbo1;

         //pcommandlist->SetGraphicsRootDescriptorTable(0, prenderer->m_pheapCbv->GetGPUDescriptorHandleForHeapStart());
         pcommandlist->SetGraphicsRootConstantBufferView(
            0, pblockGlobalUbo1->m_pd3d12resourceBlock->gpu_address());

      }

      if (bNewFrame)
      {

         m_iPush = 0;

         m_iHeapIndex = 0;

      }

      if (m_etopology == ::gpu::e_topology_triangle_strip)
      {

         pcommandlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

      }
      else if (m_etopology == ::gpu::e_topology_triangle_list)
      {

         pcommandlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

      }

      if (m_pbindingslotseta)
      {

         ::array<ID3D12DescriptorHeap *> heapa;

         auto &pheap = m_heapa1[iFrameIndex];
         heapa.add(pheap);

         auto &pheapSampler = m_heapaSampler1[iFrameIndex];
         heapa.add(pheapSampler);

         //for (int i = 0; i < binding_slot_set_array()->size(); i++)
         //{
         //   auto pbindingslotset = binding_slot_set_array()->element_at(i);

         //   if (pbindingslotset->m_pbindingset->first()->is_image_sampler())
         //   {
         //      // heapa.add(ptextureDst->m_pheapRenderTargetView);
         //      ::cast<::gpu_directx12::binding_set> pbindingset = pbindingslotset->m_pbindingset;

         //      //auto &pheap = pbindingset->m_heapa1[iFrame];
         //      auto &pheap = m_heapa1[iFrame];
         //      if (pbindingset->m_iXYZ_Index >= 0 && pheap)
         //      {
         //         heapa.add(pheap);
         //      
         //      }
         //      auto &pheapSampler = pbindingset->m_heapaSampler1[iFrame];
         //      if (pbindingset->m_iXYZ_IndexSampler)
         //      {
         //         heapa.add(pheapSampler);
         //      }
         //   }
         //}

         if (heapa.has_element())
         {
            pcommandlist->SetDescriptorHeaps((UINT)heapa.size(), heapa.data());
         }
      }


   }


   void shader::unbind(::gpu::command_buffer *pgpucommandbuffer)
   {

      ::gpu::shader::unbind(pgpucommandbuffer);

   }


   void shader::push_properties(::gpu::command_buffer *pgpucommandbuffer)
   {

      ::cast < renderer > prenderer = m_pgpurenderer;

      if (m_propertiesPushShared.size(false) <= 0)
      {

         return;

      }

      if (m_iPush >= m_iPushMax)
      {

         _on_more_push();

      }

      ::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::cast <device> pgpudevice = pgpucontext->m_pgpudevice;

      auto iSetSize = ::directx12::Align256((UINT)m_propertiesPushShared.size(false));
      CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
      UINT8* pPushProperties;
      auto hrMap = m_resourceaPushProperties[m_iPush/256]->Map(0, &readRange, (void **)&pPushProperties);
      defer_throw_hresult(hrMap);
      memcpy(pPushProperties + ::directx12::Align256((UINT)m_propertiesPushShared.size(false)) * (m_iPush % 256),
             m_propertiesPushShared.data(false), m_propertiesPushShared.size(false));
      m_resourceaPushProperties[m_iPush/256]->Unmap(0, nullptr);

      //::cast < command_buffer > pcommandbuffer = prenderer->getCurrentCommandBuffer2(::gpu::current_layer());

      ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;


      int iBlockSize = ::directx12::Align256((UINT)m_propertiesPushShared.size(false));
      int iSize = (m_iPush %256) * iBlockSize;
      m_strPushConstantsDebugging.formatf("%d,%d:%d", iSize, iBlockSize, iSize/iBlockSize);
      auto address = m_resourceaPushProperties[m_iPush/256]->GetGPUVirtualAddress() + iSize;
      pcommandbuffer->m_pcommandlist->SetGraphicsRootConstantBufferView(m_iPushConstantsBufferIndex, address);
      m_iPush++;

   }


   void shader::draw()
   {

      ::cast < ::gpu_directx12::renderer > prenderer = m_pgpurenderer;

      ::cast < ::gpu_directx12::context > pcontext = prenderer->m_pgpucontext;

      ::cast < command_buffer > pcommandbuffer = prenderer->getCurrentCommandBuffer2(::gpu::current_layer());

      auto pcommandlist = pcommandbuffer->m_pcommandlist;

      pcommandlist->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

      pcommandlist->DrawInstanced(6, 1, 0, 0);

   }


    void shader::bind_slot_set(::gpu::command_buffer *pgpucommandbuffer, int iSet,
                              ::gpu::binding_slot_set *pgpubindingslotset)
   {

      //::cast<::gpu_directx12::binding_slot_set> pbindingslotset = pgpubindingslotset;

      //::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

      //::cast<renderer> prenderer = m_pgpurenderer;

      //::cast<::gpu_directx12::shader> pshader = prenderer->m_pgpucontext->m_pshaderBound;

      // auto &vkdescriptorseta = pbindingslotset->descriptor_set(pgpucommandbuffer);

      int iFrameIndex = pgpucommandbuffer->m_iCommandBufferFrameIndex2;

      // auto vkpipelinelayout = pshader->m_ppipelineCurrent->_pipeline_layout();

      // VkDescriptorSet vkdescriptorset = vkdescriptorseta[iFrame];

      // VkDescriptorSet vkdescriptorsetaBind[1];

      // vkdescriptorsetaBind[0] = vkdescriptorset;

      // vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer,
      //                         VK_PIPELINE_BIND_POINT_GRAPHICS, // Bind point
      //                         vkpipelinelayout, // Layout used when pipeline was created
      //                         iSet, // First set (set = 0)
      //                         1, // Descriptor set count
      //                         vkdescriptorsetaBind, // Pointer to descriptor set
      //                         0, // Dynamic offset count
      //                         NULL // Dynamic offsets
      //);


      ::cast<::gpu_directx12::command_buffer> pcommandbuffer = pgpucommandbuffer;
      ::cast<::gpu_directx12::device> pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      ::cast<::gpu_directx12::context> pcontext = m_pgpurenderer->m_pgpucontext;
      ::cast<::gpu_directx12::binding_set> pbindingset = binding_set(iSet);
      // if (pgpubindingslotset->m_pbindingset->has_s)
          
      //UINT inc = pgpudevice->m_pd3d12device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
      if (pbindingset->first()->is_image_sampler())
      {


         //auto &pheap = pbindingset->m_heapa1[iFrame];
         auto &pheap = m_heapa1[iFrameIndex];
         // pbindingset->m_pheap1 Should had been created at create_root_signature()
         ASSERT(pheap);
         {

            // D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
            // srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            // srvHeapDesc.NumDescriptors = pbindingset->size();
            // srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            int base1 = -1;
            // pgpudevice->m_pd3d12device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&pbindingset->m_pheap1));
            //auto &pheap = pbindingset->m_heapa[iFrame];
            D3D12_CPU_DESCRIPTOR_HANDLE h = pheap->GetCPUDescriptorHandleForHeapStart();
            for (int i = 0; i < pgpubindingslotset->size(); i++)
            {
               auto &bindingslot = pgpubindingslotset->element_at(i);
               if (bindingslot.m_pbinding->is_image_sampler())
               {
                  ::cast<::gpu_directx12::texture> ptexture = bindingslot.m_ptexture;
                  if (!ptexture->m_pheapShaderResourceView)
                  {

                     ptexture->create_shader_resource();
                  }
                  ptexture->set_state(pgpucommandbuffer, ::gpu::e_texture_state_shader_read);
                  UINT inc =
                     pgpudevice->m_pd3d12device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
                  auto h2 = ptexture->m_handleShaderResourceView;
                  auto h1 = h;
                  //auto iHeapIndex = pbindingset->m_iHeapIndex++;
                  auto iHeapIndex = m_iHeapIndex++;
                  auto iHeapIndex1 = iHeapIndex % m_iHeapCount;
                  auto base = iHeapIndex1 * inc;
                  h1.ptr += base;
                  if (base1 < 0)
                  {

                     base1 = base;

                  }
                  pgpudevice->m_pd3d12device->CopyDescriptorsSimple(1, h1, h2, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
               }

               
            }
            auto hGpu = pheap->GetGPUDescriptorHandleForHeapStart();
            hGpu.ptr += base1;

            pcommandbuffer->m_pcommandlist->SetGraphicsRootDescriptorTable(
               pbindingset->m_iXYZ_Index,
               hGpu); // t0
         }

         //auto &pheapSampler = pbindingset->m_heapaSampler1[iFrame];
         auto &pheapSampler = m_heapaSampler1[iFrameIndex];
         ASSERT(pheapSampler);
         {

            //   D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
            //   srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
            //   srvHeapDesc.NumDescriptors = 1;
            //   srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

            //   pgpudevice->m_pd3d12device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&pbindingset->m_pheapSampler1));

            //auto &pheapSampler = pbindingset->m_heapaSampler[iFrame];
            auto &bindingslot = pgpubindingslotset->element_at(0);
            ::cast<::gpu_directx12::texture> ptexture = bindingslot.m_ptexture;
            // if (!ptexture->m_pheapSampler)
            //{

            //   ptexture->create_sm();
            //}
            auto inc = pgpudevice->m_pd3d12device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
            D3D12_CPU_DESCRIPTOR_HANDLE h = pheapSampler->GetCPUDescriptorHandleForHeapStart();
            auto h2 = ptexture->m_handleSampler;
            auto h1 = h;
            //auto iHeapIndex = pbindingset->m_iHeapSamplerIndex++;
            auto iHeapIndex = m_iHeapSamplerIndex++;
            auto iHeapIndex1 = iHeapIndex % m_iHeapSamplerCount;
            auto base = iHeapIndex1 * inc;
            h1.ptr += base;
            pgpudevice->m_pd3d12device->CopyDescriptorsSimple(1, h1, h2, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
            auto hGpu = pheapSampler->GetGPUDescriptorHandleForHeapStart();
            hGpu.ptr += base;
            pcommandbuffer->m_pcommandlist->SetGraphicsRootDescriptorTable(
               pbindingset->m_iXYZ_IndexSampler,
                             hGpu                                              ); // t0
         }

      }


   }

         

         
      


//  D3D12_CPU_DESCRIPTOR_HANDLE h = srvHeap->GetCPUDescriptorHandleForHeapStart();

      //if (bAlbedo)
      //{
      //   device->CopyDescriptorsSimple(1, h, pAlbedo->m_srvCpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
      //}
      //h.ptr += inc;

      //if (bMetallicRoughness)
      //{
      //   device->CopyDescriptorsSimple(1, h, pMR->m_srvCpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
      //}
      //h.ptr += inc;
   //}

} // namespace gpu_directx12



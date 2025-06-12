// Created by camilo on 2025-06-11 02:34 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "renderer.h"
#include "swap_chain.h"


#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
const char* fullscreen_vertex_shader = R"shader(// fullscreen_vs.hlsl
      struct VSOut {
         float4 pos : SV_POSITION;
         float2 uv : TEXCOORD0;
      };

      VSOut main(uint vid : SV_VertexID) {
         float2 verts[3] = {
             float2(-1, -1),
             float2(-1, +3),
             float2(+3, -1)
         };
         float2 uvs[3] = {
             float2(0, 1),
             float2(0, -1),
             float2(2, 1)
         };

         VSOut o;
         o.pos = float4(verts[vid], 0, 1);
         o.uv = 0.5 * (verts[vid] + 1.0);
         return o;
      }
)shader";

const char* fullscreen_pixel_shader = R"shader(// fullscreen_ps.hlsl
Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_Target {
    return tex.Sample(samp, uv);
}
)shader";



namespace gpu_directx12
{


   swap_chain::swap_chain()
   {


   }


   swap_chain::~swap_chain()
   {

   }



	void swap_chain::initialize_render_target_view(renderer* pgpurenderer, const ::int_size& size, ::pointer <render_target_view>previous)
	{

		render_target_view::initialize_render_target_view(pgpurenderer, size, previous);

		//      clear_flag(e_flag_success);
	}


	void swap_chain::init()
	{

		m_pgpurenderer->restart_frame_counter();

//		::cast < ::gpu_directx12::device > pgpudevice = m_pgpudevice;

		//m_pdxgiswapchain1.as(m_pdxgiswapchain3);

		comptr<ID3DBlob> vsBlob;
		comptr<ID3DBlob> psBlob;
		comptr<ID3DBlob> errorBlob;

		HRESULT hr = D3DCompile(
			fullscreen_vertex_shader, strlen(fullscreen_vertex_shader),
			nullptr,                       // optional source name
			nullptr,                       // macro definitions
			nullptr,                       // include handler

			"main", "vs_5_0",
			0, 0,
			&vsBlob, &errorBlob
		);
		if (FAILED(hr)) {
			if (errorBlob) OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			return;
		}

		// Pixel Shader
		hr = D3DCompile(
			fullscreen_pixel_shader, strlen(fullscreen_vertex_shader),
			nullptr,                       // optional source name
			nullptr,                       // macro definitions
			nullptr,                       // include handler

			"main", "ps_5_0",
			0, 0,
			&psBlob, &errorBlob
		);
		if (FAILED(hr)) {
			if (errorBlob) OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			return;
		}

		//pgpudevice->m_pdevice->CreateVertexShader(
		//	vsBlob->GetBufferPointer(),
		//	vsBlob->GetBufferSize(),
		//	nullptr,
		//	&m_pvertexshaderFullscreen
		//);

		//pgpudevice->m_pdevice->CreatePixelShader(
		//	psBlob->GetBufferPointer(),
		//	psBlob->GetBufferSize(),
		//	nullptr,
		//	&m_ppixelshaderFullscreen
		//);

		//D3D11_SAMPLER_DESC sampDesc = {};
		//sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		//sampDesc.AddressU = sampDesc.AddressV = sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

		//pgpudevice->m_pdevice->CreateSamplerState(&sampDesc, &m_psamplerstateLinear);
				// Define the vertex input layout.
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			 { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			 { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};
		//::cast < ::gpu_directx12::context > pcontext = m_pgpucontext;
		::cast < ::gpu_directx12::device > pdevice = m_pgpudevice;
		// Describe and create the graphics pipeline state object (PSO).
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = m_prootsignature;
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob);
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob);
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;
		::defer_throw_hresult(pdevice->m_pdevice->CreateGraphicsPipelineState(&psoDesc, __interface_of(m_ppipelinestate)));


	}


   void swap_chain::endDraw(::draw2d_gpu::graphics* pgraphics, ::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc)
   {




   }


   void swap_chain::present()
   {


   }


} // namespace gpu_directx12




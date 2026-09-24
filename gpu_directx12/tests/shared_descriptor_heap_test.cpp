#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <vector>
#include <fstream>
#include <iterator>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "../pixel_upload.h"
using Microsoft::WRL::ComPtr;
using namespace gpu_directx12::detail;
static void require(bool value, const char * message)
{
   if (!value) { std::fprintf(stderr, "%s\n", message); std::exit(1); }
}
static void check(HRESULT hr) { require(SUCCEEDED(hr), "D3D12 operation failed"); }
static std::string source(const char * name)
{
   std::ifstream file(std::string("source/app-graphics3d/gpu_directx12/") + name, std::ios::binary);
   require(file.good(), "Run test from main workspace");
   return {std::istreambuf_iterator<char>(file), {}};
}
int main()
{
   const auto deviceCode = source("device.cpp");
   require(deviceCode.find("D3D12_MAX_SHADER_VISIBLE_SAMPLER_HEAP_SIZE") != std::string::npos &&
      deviceCode.find("desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;") != std::string::npos,
      "Device sampler heap has wrong type or capacity");
   require(deviceCode.find("m_textureTables.find(key)") != std::string::npos &&
      deviceCode.find("m_handleLinearClampSampler") != std::string::npos &&
      deviceCode.find("capacity - used") != std::string::npos,
      "Immutable descriptor caching/capacity validation missing");
   const auto shaderCode = source("shader.cpp");
   const auto bindingAt = shaderCode.find("void shader::bind_slot_set(");
   require(bindingAt != std::string::npos, "Image binding missing");
   const auto binding = shaderCode.substr(bindingAt, shaderCode.find("\n   }", bindingAt) - bindingAt);
   require(binding.find("_texture_table(textures)") != std::string::npos &&
      binding.find("CopyDescriptorsSimple") == std::string::npos &&
      binding.find("m_heapaSampler1") == std::string::npos,
      "Image binding still uses old per-frame heaps or copies shader-visible descriptors");
   require(source("renderer.cpp").find("SetDescriptorHeaps") == std::string::npos,
      "Renderer bypasses device heap binding");
   const auto commandCode = source("command_buffer.cpp");
   const auto resetAt = commandCode.find("void command_buffer::reset()");
   require(commandCode.find("m_bDeviceDescriptorHeapSet = false;", resetAt) != std::string::npos,
      "Command-list reset must invalidate heap binding");

   ComPtr<ID3D12Debug> debug;
   check(D3D12GetDebugInterface(IID_PPV_ARGS(&debug))); debug->EnableDebugLayer();
   ComPtr<IDXGIFactory4> factory;
   check(CreateDXGIFactory2(0, IID_PPV_ARGS(&factory)));
   ComPtr<IDXGIAdapter> warp;
   check(factory->EnumWarpAdapter(IID_PPV_ARGS(&warp)));
   ComPtr<ID3D12Device> device;
   check(D3D12CreateDevice(warp.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));
   ComPtr<ID3D12InfoQueue> messages; check(device.As(&messages));
   D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
   heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
   heapDesc.NumDescriptors = 163840;
   heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
   ComPtr<ID3D12DescriptorHeap> views, samplers;
   check(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&views)));
   heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
   heapDesc.NumDescriptors = D3D12_MAX_SHADER_VISIBLE_SAMPLER_HEAP_SIZE;
   check(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&samplers)));
   D3D12_SAMPLER_DESC sampler{};
   sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
   sampler.AddressU = sampler.AddressV = sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
   sampler.MaxAnisotropy = 1; sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
   sampler.MaxLOD = D3D12_FLOAT32_MAX;
   device->CreateSampler(&sampler, samplers->GetCPUDescriptorHandleForHeapStart());
   const UINT stride = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
   auto cpu = [&](UINT index) { auto h = views->GetCPUDescriptorHandleForHeapStart(); h.ptr += (SIZE_T)index * stride; return h; };
   auto gpu = [&](UINT index) { auto h = views->GetGPUDescriptorHandleForHeapStart(); h.ptr += (UINT64)index * stride; return h; };
   ComPtr<ID3D12CommandAllocator> allocator;
   check(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator)));
   ComPtr<ID3D12GraphicsCommandList> commands;
   check(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator.Get(), nullptr, IID_PPV_ARGS(&commands)));
   D3D12_HEAP_PROPERTIES heap{}; heap.Type = D3D12_HEAP_TYPE_DEFAULT;
   heap.CreationNodeMask = heap.VisibleNodeMask = 1;
   D3D12_RESOURCE_DESC desc{};
   desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
   desc.Width = desc.Height = desc.DepthOrArraySize = desc.MipLevels = desc.SampleDesc.Count = 1;
   desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
   ComPtr<ID3D12Resource> textures[2], uploads[2];
   const unsigned char colors[2][4] = {{255, 0, 0, 255}, {0, 255, 0, 255}};
   for (UINT i = 0; i < 2; ++i)
   {
      check(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc,
         D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, nullptr, IID_PPV_ARGS(&textures[i])));
      D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint{}; UINT subresource = 0;
      check(create_pixel_upload(device.Get(), desc, colors[i], 1, 1, 4, 4, 0, 0, 0, 0,
         &uploads[i], footprint, subresource));
      record_pixel_upload(commands.Get(), textures[i].Get(), uploads[i].Get(), footprint, 0, 0, 0,
         D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
      D3D12_SHADER_RESOURCE_VIEW_DESC srv{};
      srv.Format = desc.Format; srv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
      srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
      srv.Texture2D.MipLevels = 1;
      device->CreateShaderResourceView(textures[i].Get(), &srv, cpu(i));
      device->CreateShaderResourceView(textures[i].Get(), &srv, cpu(4 - i));
   }
   desc = {};
   desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
   desc.Width = 8 * sizeof(float); desc.Height = desc.DepthOrArraySize = desc.MipLevels = desc.SampleDesc.Count = 1;
   desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR; desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
   ComPtr<ID3D12Resource> output, readback;
   check(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc,
      D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr, IID_PPV_ARGS(&output)));
   D3D12_UNORDERED_ACCESS_VIEW_DESC uav{};
   uav.ViewDimension = D3D12_UAV_DIMENSION_BUFFER; uav.Buffer.NumElements = 2;
   uav.Buffer.StructureByteStride = 4 * sizeof(float);
   device->CreateUnorderedAccessView(output.Get(), nullptr, &uav, cpu(2));
   heap.Type = D3D12_HEAP_TYPE_READBACK; desc.Flags = D3D12_RESOURCE_FLAG_NONE;
   check(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc,
      D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&readback)));
   D3D12_DESCRIPTOR_RANGE ranges[3]{};
   ranges[0] = {D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 0, 0, 0};
   ranges[1] = {D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0, 0, 0};
   ranges[2] = {D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0, 0, 0};
   D3D12_ROOT_PARAMETER parameters[4]{};
   for (UINT i = 0; i < 3; ++i)
   {
      parameters[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
      parameters[i].DescriptorTable = {1, &ranges[i]};
   }
   parameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
   parameters[3].Constants = {0, 0, 1};
   D3D12_ROOT_SIGNATURE_DESC rootDesc{4, parameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_NONE};
   ComPtr<ID3DBlob> rootBlob, errors, code;
   check(D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootBlob, &errors));
   ComPtr<ID3D12RootSignature> root;
   check(device->CreateRootSignature(0, rootBlob->GetBufferPointer(), rootBlob->GetBufferSize(), IID_PPV_ARGS(&root)));
   const char shader[] =
      "Texture2D<float4> a:register(t0), b:register(t1); SamplerState s:register(s0);"
      "RWStructuredBuffer<float4> o:register(u0); cbuffer C:register(b0){uint index;};"
      "[numthreads(1,1,1)] void main(){o[index]=a.SampleLevel(s,float2(.5,.5),0)+2*b.SampleLevel(s,float2(.5,.5),0);}";
   check(D3DCompile(shader, sizeof(shader) - 1, nullptr, nullptr, nullptr, "main", "cs_5_0", 0, 0, &code, &errors));
   D3D12_COMPUTE_PIPELINE_STATE_DESC pipelineDesc{};
   pipelineDesc.pRootSignature = root.Get(); pipelineDesc.CS = {code->GetBufferPointer(), code->GetBufferSize()};
   ComPtr<ID3D12PipelineState> pipeline;
   check(device->CreateComputePipelineState(&pipelineDesc, IID_PPV_ARGS(&pipeline)));
   ID3D12DescriptorHeap * heaps[] = {views.Get(), samplers.Get()};
   commands->SetDescriptorHeaps(2, heaps);
   commands->SetPipelineState(pipeline.Get()); commands->SetComputeRootSignature(root.Get());
   commands->SetComputeRootDescriptorTable(1, samplers->GetGPUDescriptorHandleForHeapStart());
   commands->SetComputeRootDescriptorTable(2, gpu(2));
   for (UINT i = 0; i < 2; ++i)
   {
      commands->SetComputeRootDescriptorTable(0, gpu(i ? 3 : 0));
      commands->SetComputeRoot32BitConstant(3, i, 0);
      commands->Dispatch(1, 1, 1);
   }
   D3D12_RESOURCE_BARRIER barrier{};
   barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
   barrier.Transition = {output.Get(), D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
      D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE};
   commands->ResourceBarrier(1, &barrier);
   commands->CopyResource(readback.Get(), output.Get()); check(commands->Close());
   D3D12_COMMAND_QUEUE_DESC queueDesc{}; ComPtr<ID3D12CommandQueue> queue;
   check(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&queue)));
   ID3D12CommandList * lists[] = {commands.Get()}; queue->ExecuteCommandLists(1, lists);
   ComPtr<ID3D12Fence> fence; check(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
   check(queue->Signal(fence.Get(), 1));
   HANDLE event = CreateEventW(nullptr, FALSE, FALSE, nullptr); require(event != nullptr, "Event creation failed");
   check(fence->SetEventOnCompletion(1, event));
   require(WaitForSingleObject(event, 10000) == WAIT_OBJECT_0, "Shared descriptor test timed out"); CloseHandle(event);
   void * mapped = nullptr; check(readback->Map(0, nullptr, &mapped));
   const float expected[8] = {1, 2, 0, 3, 2, 1, 0, 3};
   for (int i = 0; i < 8; ++i) require(std::fabs(static_cast<float *>(mapped)[i] - expected[i]) < .001f,
      "Contiguous descriptor tables aliased or sampled the wrong textures");
   const D3D12_RANGE noWrite{0, 0}; readback->Unmap(0, &noWrite);
   for (UINT64 i = 0; i < messages->GetNumStoredMessages(); ++i)
   {
      SIZE_T size = 0; check(messages->GetMessage(i, nullptr, &size));
      std::vector<unsigned char> storage(size); auto message = reinterpret_cast<D3D12_MESSAGE *>(storage.data());
      check(messages->GetMessage(i, message, &size));
      require(message->Severity > D3D12_MESSAGE_SEVERITY_ERROR, message->pDescription);
   }
   std::puts("D3D12 shared descriptor heaps passed: contiguous multi-texture tables, immutable bindings, shared sampler and debug validation.");
}

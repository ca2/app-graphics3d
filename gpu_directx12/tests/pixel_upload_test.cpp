#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include "../pixel_upload.h"
// _d3dx12.h also contains an unrelated ca2 cull-mode adapter. Supply its
// enum for this standalone native test; all upload/D3D12 code is production code.
namespace gpu { enum enum_cull_mode { e_cull_mode_none, e_cull_mode_front, e_cull_mode_back }; }
#include "../static_pixel_upload.h"
#include "../texture_copy.h"

using Microsoft::WRL::ComPtr;
using namespace gpu_directx12::detail;
static void require(bool condition, const char * message)
{
   if (!condition) { std::fprintf(stderr, "%s\n", message); std::exit(1); }
}
static void check(HRESULT hr) { require(SUCCEEDED(hr), "D3D12 operation failed"); }

int main()
{
   // Native tests cannot instantiate the full application. Check the production
   // routing as well, so correct barriers are not sent to the asset COPY list.
   std::ifstream textureFile("source/app-graphics3d/gpu_directx12/texture.cpp", std::ios::binary);
   require(textureFile.good(), "Run the upload test from the main workspace");
   const std::string textureCode{std::istreambuf_iterator<char>(textureFile), {}};
   const auto initialBegin = textureCode.find("void texture::_upload_initial_texture_data(");
   const auto initialEnd = textureCode.find("void texture::static_upload_buffer::initialize_static_upload_buffer", initialBegin);
   require(initialBegin != std::string::npos && initialEnd != std::string::npos, "Initial upload route missing");
   const auto initialUpload = textureCode.substr(initialBegin, initialEnd - initialBegin);
   require(initialUpload.find("graphics_queue(), ::gpu::e_command_buffer_graphics") != std::string::npos,
      "Initial upload must explicitly select the graphics queue");
   const auto uploadAt = initialUpload.find("->update_with_texture_data(");
   const auto commitAt = initialUpload.find(".commit();");
   require(uploadAt != std::string::npos && commitAt != std::string::npos && uploadAt < commitAt,
      "Initial upload must submit before returning");
   const auto createAt = textureCode.find("void texture::_create_texture(");
   const auto createEnd = textureCode.find("new_texture.set_new_texture();", createAt);
   require(createAt != std::string::npos && createEnd != std::string::npos, "Texture creation route missing");
   const auto creation = textureCode.substr(createAt, createEnd - createAt);
   require(creation.find("_upload_initial_texture_data(texturedata);") != std::string::npos &&
      creation.find("getLoadAssetsCommandBuffer();") == std::string::npos,
      "Texture initialization must use the dedicated graphics upload route");

   const auto glyphAt = textureCode.find("void texture::set_pixels(bool bSync,");
   const auto glyphEnd = textureCode.find("////void texture::set_pixels", glyphAt);
   require(glyphAt != std::string::npos && glyphEnd != std::string::npos, "Glyph upload route missing");
   const auto glyphUpload = textureCode.substr(glyphAt, glyphEnd - glyphAt);
   require(glyphUpload.find("_get_upload_buffer()") == std::string::npos &&
      glyphUpload.find("_write_pixels(bSync, data,") != std::string::npos &&
      glyphUpload.find("rectangle.top_left(), true);") != std::string::npos,
      "Glyph upload must complete in shader-read state, not use frame-end staging");
   const auto writeAt = textureCode.find("void texture::_write_pixels(");
   require(writeAt != std::string::npos && writeAt < glyphAt, "Pixel upload implementation missing");
   const auto write = textureCode.substr(writeAt, glyphAt - writeAt);
   const auto copyAt = write.find("detail::record_pixel_upload(");
   const auto readyAt = write.find("set_state(pcommandbuffer, ::gpu::e_texture_state_shader_read);");
   const auto submitAt = write.find("commands.commit();");
   require(copyAt != std::string::npos && readyAt != std::string::npos && submitAt != std::string::npos &&
      copyAt < readyAt && readyAt < submitAt, "Glyph sampling transition must be submitted with its upload");

   ComPtr<ID3D12Debug> debug;
   check(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)));
   debug->EnableDebugLayer();
   ComPtr<IDXGIFactory4> factory;
   check(CreateDXGIFactory2(0, IID_PPV_ARGS(&factory)));
   ComPtr<IDXGIAdapter> warp;
   check(factory->EnumWarpAdapter(IID_PPV_ARGS(&warp)));
   ComPtr<ID3D12Device> device;
   check(D3D12CreateDevice(warp.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));
   ComPtr<ID3D12InfoQueue> messages;
   check(device.As(&messages));
   D3D12_COMMAND_QUEUE_DESC queueDesc{};
   ComPtr<ID3D12CommandQueue> queue;
   check(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&queue)));
   ComPtr<ID3D12Fence> fence;
   check(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
   HANDLE event = CreateEventW(nullptr, FALSE, FALSE, nullptr);
   require(event != nullptr, "CreateEvent failed");
   UINT64 serial = 0;

   for (auto format : {DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_R8G8_UNORM,
                      DXGI_FORMAT_R32G32B32A32_FLOAT})
   for (int mip : {0, 1})
   {
      const int bpp = (int)pixel_upload_bytes(format);
      D3D12_RESOURCE_DESC desc{};
      desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
      desc.Width = 26; desc.Height = 18; desc.DepthOrArraySize = 2; desc.MipLevels = 2;
      desc.Format = format; desc.SampleDesc.Count = 1;
      const auto previous = format == DXGI_FORMAT_B8G8R8A8_UNORM
         ? D3D12_RESOURCE_STATE_RENDER_TARGET : (format == DXGI_FORMAT_R8G8_UNORM
            ? D3D12_RESOURCE_STATE_COPY_DEST : D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
      if (previous == D3D12_RESOURCE_STATE_RENDER_TARGET) desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
      D3D12_HEAP_PROPERTIES heap{};
      heap.Type = D3D12_HEAP_TYPE_DEFAULT;
      heap.CreationNodeMask = heap.VisibleNodeMask = 1;
      ComPtr<ID3D12Resource> texture;
      check(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc, previous,
         nullptr, IID_PPV_ARGS(&texture)));
      ComPtr<ID3D12CommandAllocator> allocator;
      check(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator)));
      ComPtr<ID3D12GraphicsCommandList> commands;
      check(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator.Get(), nullptr, IID_PPV_ARGS(&commands)));

      const int w = (int)desc.Width >> mip, h = desc.Height >> mip;
      std::vector<unsigned char> zero((size_t)w * h * bpp, 0);
      ComPtr<ID3D12Resource> initial;
      D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint{};
      UINT subresource = 0;
      check(create_pixel_upload(device.Get(), desc, zero.data(), w, h, w * bpp, bpp,
         0, 0, mip, 1, initial.GetAddressOf(), footprint, subresource));
      record_pixel_upload(commands.Get(), texture.Get(), initial.Get(), footprint, subresource, 0, 0, previous);

      const int patchW = 5, patchH = 3, x = w - patchW, y = h - patchH;
      const int scan = patchW * bpp + 7; // Deliberately padded and unaligned CPU rows.
      std::vector<unsigned char> pixels((patchH - 1) * scan + patchW * bpp, 0xcc);
      for (int row = 0; row < patchH; ++row)
         for (int byte = 0; byte < patchW * bpp; ++byte)
            pixels[row * scan + byte] = (unsigned char)(1 + row * 29 + byte);
      ComPtr<ID3D12Resource> patch;
      check(create_pixel_upload(device.Get(), desc, pixels.data(), patchW, patchH, scan, bpp,
         x, y, mip, 1, patch.GetAddressOf(), footprint, subresource));
      require(footprint.Footprint.RowPitch % D3D12_TEXTURE_DATA_PITCH_ALIGNMENT == 0, "Unaligned GPU row pitch");
      require(subresource == (UINT)mip + desc.MipLevels, "Wrong mip/layer subresource");
      record_pixel_upload(commands.Get(), texture.Get(), patch.Get(), footprint, subresource, x, y, previous);

      // Invalid inputs must fail before allocating or recording any GPU work.
      ComPtr<ID3D12Resource> invalid;
      require(create_pixel_upload(device.Get(), desc, pixels.data(), patchW, patchH, 1, bpp,
         x, y, mip, 1, invalid.GetAddressOf(), footprint, subresource) == E_INVALIDARG, "Short stride accepted");
      require(create_pixel_upload(device.Get(), desc, pixels.data(), patchW, patchH, scan, bpp,
         x + 1, y, mip, 1, invalid.GetAddressOf(), footprint, subresource) == E_INVALIDARG, "Out-of-bounds upload accepted");
      require(create_pixel_upload(device.Get(), desc, pixels.data(), patchW, patchH, scan, bpp + 1,
         x, y, mip, 1, invalid.GetAddressOf(), footprint, subresource) == E_INVALIDARG, "Wrong pixel size accepted");
      require(create_pixel_upload(device.Get(), desc, pixels.data(), patchW, patchH, scan, bpp,
         -1, y, mip, 1, invalid.GetAddressOf(), footprint, subresource) == E_INVALIDARG, "Negative offset accepted");

      UINT64 bytes = 0;
      device->GetCopyableFootprints(&desc, subresource, 1, 0, &footprint, nullptr, nullptr, &bytes);
      D3D12_RESOURCE_DESC readDesc{};
      readDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
      readDesc.Width = bytes; readDesc.Height = readDesc.DepthOrArraySize = readDesc.MipLevels = 1;
      readDesc.SampleDesc.Count = 1; readDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
      heap.Type = D3D12_HEAP_TYPE_READBACK;
      ComPtr<ID3D12Resource> readback;
      check(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &readDesc,
         D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&readback)));
      D3D12_RESOURCE_BARRIER barrier{};
      barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
      // Like the glyph-upload wrapper, finalize for sampling in the same
      // submission, including a newly created COPY_DEST atlas.
      if (previous != D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE)
      {
         barrier.Transition = {texture.Get(), subresource, previous, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE};
         commands->ResourceBarrier(1, &barrier);
      }
      barrier.Transition = {texture.Get(), subresource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COPY_SOURCE};
      commands->ResourceBarrier(1, &barrier);
      D3D12_TEXTURE_COPY_LOCATION src{}, dst{};
      src.pResource = texture.Get(); src.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX; src.SubresourceIndex = subresource;
      dst.pResource = readback.Get(); dst.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT; dst.PlacedFootprint = footprint;
      commands->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
      check(commands->Close());
      ID3D12CommandList * lists[] = {commands.Get()};
      queue->ExecuteCommandLists(1, lists);
      check(queue->Signal(fence.Get(), ++serial));
      check(fence->SetEventOnCompletion(serial, event));
      require(WaitForSingleObject(event, 10000) == WAIT_OBJECT_0, "GPU upload timed out");
      void * mapped = nullptr;
      D3D12_RANGE range{0, (SIZE_T)bytes};
      check(readback->Map(0, &range, &mapped));
      for (int row = 0; row < h; ++row)
         for (int col = 0; col < w; ++col)
            for (int channel = 0; channel < bpp; ++channel)
            {
               const auto actual = static_cast<unsigned char *>(mapped)[footprint.Offset +
                  (size_t)row * footprint.Footprint.RowPitch + col * bpp + channel];
               const auto expected = row >= y && col >= x
                  ? pixels[(row - y) * scan + (col - x) * bpp + channel] : 0;
               require(actual == expected, "Pixel mismatch (row pitch, offset, channel order or untouched region)");
            }
      const D3D12_RANGE noWrite{0, 0};
      readback->Unmap(0, &noWrite);
   }
   // Static upload: base level of six faces with multiple mips. Verify every
   // face, including padding, and require restoration from RT/read/copy states.
   for (UINT faceCount : {1u, 6u})
   for (auto previous : {D3D12_RESOURCE_STATE_RENDER_TARGET,
                         D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COPY_DEST})
   {
      D3D12_RESOURCE_DESC desc{};
      desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
      desc.Width = 13; desc.Height = 13; desc.DepthOrArraySize = (UINT16)faceCount; desc.MipLevels = 3;
      desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; desc.SampleDesc.Count = 1;
      desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
      D3D12_HEAP_PROPERTIES heap{};
      heap.Type = D3D12_HEAP_TYPE_DEFAULT; heap.CreationNodeMask = heap.VisibleNodeMask = 1;
      ComPtr<ID3D12Resource> target;
      check(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc, previous, nullptr, IID_PPV_ARGS(&target)));
      const auto bytes = GetRequiredIntermediateSize(target.Get(), 0, desc.MipLevels * desc.DepthOrArraySize);
      auto bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(bytes);
      heap.Type = D3D12_HEAP_TYPE_UPLOAD;
      ComPtr<ID3D12Resource> upload;
      check(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &bufferDesc,
         D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&upload)));
      ComPtr<ID3D12CommandAllocator> allocator;
      check(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator)));
      ComPtr<ID3D12GraphicsCommandList> commands;
      check(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator.Get(), nullptr, IID_PPV_ARGS(&commands)));
      std::vector<unsigned char> pixels[6];
      D3D12_SUBRESOURCE_DATA data[6]{};
      constexpr int scan = 13 * 4 + 9;
      for (int face = 0; face < faceCount; ++face)
      {
         pixels[face].resize(scan * 13, 0xcd);
         for (int y = 0; y < 13; ++y)
            for (int x = 0; x < 13 * 4; ++x)
               pixels[face][y * scan + x] = (unsigned char)(face * 31 + y * 7 + x);
         data[face] = {pixels[face].data(), scan, scan * 13};
      }
      require(update_static_pixels(device.Get(), commands.Get(), target.Get(), upload.Get(), faceCount - 1, data, previous) == E_INVALIDARG,
         "Static upload accepted missing cube face");
      data[faceCount - 1].RowPitch = 1;
      require(update_static_pixels(device.Get(), commands.Get(), target.Get(), upload.Get(), faceCount, data, previous) == E_INVALIDARG,
         "Static upload accepted short stride");
      data[faceCount - 1].RowPitch = scan;
      data[faceCount - 1].pData = nullptr;
      require(update_static_pixels(device.Get(), commands.Get(), target.Get(), upload.Get(), faceCount, data, previous) == E_INVALIDARG,
         "Static upload accepted null pixels");
      data[faceCount - 1].pData = pixels[faceCount - 1].data();
      check(update_static_pixels(device.Get(), commands.Get(), target.Get(), upload.Get(), faceCount, data, previous));

      // Regression: the platform asset loader uses COPY lists. Reject those
      // before any illegal RT/SRV transition reaches the debug layer.
      ComPtr<ID3D12CommandAllocator> copyAllocator;
      check(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COPY, IID_PPV_ARGS(&copyAllocator)));
      ComPtr<ID3D12GraphicsCommandList> copyCommands;
      check(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COPY, copyAllocator.Get(), nullptr,
         IID_PPV_ARGS(&copyCommands)));
      require(update_static_pixels(device.Get(), copyCommands.Get(), target.Get(), upload.Get(),
         faceCount, data, previous) == E_INVALIDARG, "Static upload accepted a COPY command list");
      check(copyCommands->Close());

      auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(target.Get(), previous, D3D12_RESOURCE_STATE_COPY_SOURCE);
      commands->ResourceBarrier(1, &barrier);
      ComPtr<ID3D12Resource> readbacks[6];
      D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprints[6]{};
      for (UINT face = 0; face < faceCount; ++face)
      {
         UINT64 readBytes = 0;
         device->GetCopyableFootprints(&desc, face * desc.MipLevels, 1, 0, &footprints[face], nullptr, nullptr, &readBytes);
         auto readDesc = CD3DX12_RESOURCE_DESC::Buffer(readBytes);
         heap.Type = D3D12_HEAP_TYPE_READBACK;
         check(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &readDesc,
            D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&readbacks[face])));
         auto src = CD3DX12_TEXTURE_COPY_LOCATION(target.Get(), face * desc.MipLevels);
         auto dst = CD3DX12_TEXTURE_COPY_LOCATION(readbacks[face].Get(), footprints[face]);
         commands->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
      }
      check(commands->Close());
      ID3D12CommandList * lists[] = {commands.Get()};
      queue->ExecuteCommandLists(1, lists);
      check(queue->Signal(fence.Get(), ++serial));
      check(fence->SetEventOnCompletion(serial, event));
      require(WaitForSingleObject(event, 10000) == WAIT_OBJECT_0, "Static upload timed out");
      for (UINT face = 0; face < faceCount; ++face)
      {
         void * mapped = nullptr;
         check(readbacks[face]->Map(0, nullptr, &mapped));
         for (int y = 0; y < 13; ++y)
            require(std::memcmp(static_cast<unsigned char *>(mapped) + y * footprints[face].Footprint.RowPitch,
               pixels[face].data() + y * scan, 13 * 4) == 0, "Static cube face pixels or mip indexing wrong");
         const D3D12_RANGE noWrite{0, 0};
         readbacks[face]->Unmap(0, &noWrite);
      }
   }
   // Icon-list preservation: grow in both directions, and clip a shrinking
   // destination. Validate native bytes and restoration from sampling/RT states.
   for (bool grow : {false, true})
   {
      D3D12_RESOURCE_DESC sourceDesc{};
      sourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
      sourceDesc.Width = 13; sourceDesc.Height = 11;
      sourceDesc.DepthOrArraySize = sourceDesc.MipLevels = 1;
      sourceDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      sourceDesc.SampleDesc.Count = 1;
      sourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
      auto targetDesc = sourceDesc;
      targetDesc.Width = grow ? 29 : 7; targetDesc.Height = grow ? 17 : 5;
      CD3DX12_HEAP_PROPERTIES heap(D3D12_HEAP_TYPE_DEFAULT);
      ComPtr<ID3D12Resource> source, target;
      check(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &sourceDesc,
         D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr, IID_PPV_ARGS(&source)));
      check(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &targetDesc,
         D3D12_RESOURCE_STATE_RENDER_TARGET, nullptr, IID_PPV_ARGS(&target)));
      ComPtr<ID3D12CommandAllocator> allocator;
      check(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator)));
      ComPtr<ID3D12GraphicsCommandList> commands;
      check(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator.Get(), nullptr, IID_PPV_ARGS(&commands)));
      std::vector<unsigned char> pixels(13 * 11 * 4);
      for (size_t i = 0; i < pixels.size(); ++i) pixels[i] = (unsigned char)(i * 17 + 31);
      ComPtr<ID3D12Resource> upload;
      D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint{};
      UINT subresource = 0;
      check(create_pixel_upload(device.Get(), sourceDesc, pixels.data(), 13, 11, 13 * 4, 4,
         0, 0, 0, 0, &upload, footprint, subresource));
      record_pixel_upload(commands.Get(), source.Get(), upload.Get(), footprint, 0, 0, 0,
         D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
      D3D12_DESCRIPTOR_HEAP_DESC rtvDesc{};
      rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; rtvDesc.NumDescriptors = 1;
      ComPtr<ID3D12DescriptorHeap> rtv;
      check(device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&rtv)));
      auto handle = rtv->GetCPUDescriptorHandleForHeapStart();
      device->CreateRenderTargetView(target.Get(), nullptr, handle);
      const float transparent[4]{};
      commands->ClearRenderTargetView(handle, transparent, 0, nullptr);
      require(copy_texture_region(nullptr, target.Get(), source.Get(),
         D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE) == E_INVALIDARG,
         "Texture preservation accepted a missing command list");
      check(copy_texture_region(commands.Get(), target.Get(), source.Get(),
         D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
      auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(target.Get(),
         D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_SOURCE);
      commands->ResourceBarrier(1, &barrier);
      UINT64 readBytes = 0;
      device->GetCopyableFootprints(&targetDesc, 0, 1, 0, &footprint, nullptr, nullptr, &readBytes);
      auto bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(readBytes);
      heap.Type = D3D12_HEAP_TYPE_READBACK;
      ComPtr<ID3D12Resource> readback;
      check(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &bufferDesc,
         D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&readback)));
      auto from = CD3DX12_TEXTURE_COPY_LOCATION(target.Get(), 0);
      auto to = CD3DX12_TEXTURE_COPY_LOCATION(readback.Get(), footprint);
      commands->CopyTextureRegion(&to, 0, 0, 0, &from, nullptr);
      check(commands->Close());
      ID3D12CommandList * lists[] = {commands.Get()};
      queue->ExecuteCommandLists(1, lists);
      check(queue->Signal(fence.Get(), ++serial));
      check(fence->SetEventOnCompletion(serial, event));
      require(WaitForSingleObject(event, 10000) == WAIT_OBJECT_0, "Texture preservation timed out");
      void * mapped = nullptr;
      check(readback->Map(0, nullptr, &mapped));
      for (UINT y = 0; y < targetDesc.Height; ++y)
         for (UINT x = 0; x < targetDesc.Width; ++x)
            for (UINT channel = 0; channel < 4; ++channel)
            {
               const auto expected = x < 13 && y < 11 ? pixels[(y * 13 + x) * 4 + channel] : 0;
               const auto actual = static_cast<unsigned char *>(mapped)[y * footprint.Footprint.RowPitch + x * 4 + channel];
               require(actual == expected, "Preserved icon pixels or new transparent area incorrect");
            }
      const D3D12_RANGE noWrite{0, 0};
      readback->Unmap(0, &noWrite);
   }
   CloseHandle(event);
   for (UINT64 i = 0; i < messages->GetNumStoredMessages(); ++i)
   {
      SIZE_T size = 0;
      check(messages->GetMessage(i, nullptr, &size));
      std::vector<unsigned char> storage(size);
      auto message = reinterpret_cast<D3D12_MESSAGE *>(storage.data());
      check(messages->GetMessage(i, message, &size));
      require(message->Severity > D3D12_MESSAGE_SEVERITY_ERROR, message->pDescription);
   }
   std::puts("D3D12 WARP pixel uploads passed: padded rows, offsets, untouched pixels, mip/layer, formats, validation and state restoration.");
}

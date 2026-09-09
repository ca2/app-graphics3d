// Standalone WARP/debug-layer regression for the production factory-lock policy.
// Run from the main workspace. Link d3d12.lib d3d11.lib dxgi.lib d2d1.lib ole32.lib.
#include <windows.h>
#include <d3d12.h>
#include <d3d11on12.h>
#include <d3d11sdklayers.h>
#include <d2d1_1.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <atomic>
#include <thread>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

using Microsoft::WRL::ComPtr;
static void require(bool ok, const char * message)
{
   if (!ok) { std::fprintf(stderr, "%s\n", message); std::exit(1); }
}
static void check(HRESULT hr)
{
   if (FAILED(hr)) { std::fprintf(stderr, "HRESULT 0x%08lx\n", hr); std::exit(1); }
}
struct factory_lock
{
   ID2D1Multithread * lock;
   explicit factory_lock(ID2D1Multithread * value) : lock(value) { lock->Enter(); }
   ~factory_lock() { lock->Leave(); }
   factory_lock(const factory_lock &) = delete;
   factory_lock & operator=(const factory_lock &) = delete;
};
static std::string source(const char * path)
{
   std::ifstream file(path, std::ios::binary);
   require(file.good(), "Run from the main workspace");
   return { std::istreambuf_iterator<char>(file), {} };
}
static void verify_production_scopes()
{
   const auto code = source("source/app-graphics3d/gpu_directx12/context.cpp");
   const auto begin = code.find("void context::on_start_layer(");
   const auto end = code.find("void context::on_end_layer(", begin);
   require(begin != std::string::npos && end != std::string::npos, "Layer hooks missing");
   const auto acquire = code.substr(begin, end - begin);
   const auto release = code.substr(end, code.find("m_iResourceWrappingCount--;", end) - end);
   const std::string lock = "::direct2d_lock interoplock(::direct2d::get());";
   const auto acquired = acquire.find("pinterop->m_pd3d11on12->AcquireWrappedResources(");
   require(acquired != std::string::npos && acquire.find(lock) < acquired,
      "Layer acquire must take the actual D2D factory lock");
   const auto locked = release.find(lock);
   const auto released = release.find("pinterop->m_pd3d11on12->ReleaseWrappedResources(");
   const auto flushed = release.find("pinterop->m_pd3d11devicecontextMain->Flush();");
   require(locked < released && released < flushed && flushed != std::string::npos,
      "Release and flush must be protected and ordered");
   require(release.find('}', locked) > flushed, "Release/flush must share one lock scope");
   const auto device = source("source/app-graphics3d/gpu_directx12/device.cpp");
   const auto deviceBegin = device.find("class device::d3d11on12 * device::d3d11on12()");
   require(device.find(lock, deviceBegin) < device.find("defer_construct_new", deviceBegin),
      "Interop device publication must be locked");
   const auto texture = source("source/app-graphics3d/gpu_directx12/texture.cpp");
   const auto textureBegin = texture.find("class texture::d3d11* texture::d3d11()");
   require(texture.find(lock, textureBegin) < texture.find("if (!m_pd3d11)", textureBegin),
      "Wrapped texture publication must be locked");
}

int main()
{
   verify_production_scopes();
   check(CoInitializeEx(nullptr, COINIT_MULTITHREADED));
   ComPtr<ID3D12Debug> debug;
   check(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)));
   debug->EnableDebugLayer();
   ComPtr<IDXGIFactory4> dxgi;
   check(CreateDXGIFactory2(0, IID_PPV_ARGS(&dxgi)));
   ComPtr<IDXGIAdapter> warp;
   check(dxgi->EnumWarpAdapter(IID_PPV_ARGS(&warp)));
   ComPtr<ID3D12Device> device;
   check(D3D12CreateDevice(warp.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));
   D3D12_COMMAND_QUEUE_DESC queueDesc = {};
   ComPtr<ID3D12CommandQueue> queue;
   check(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&queue)));
   IUnknown * queues[] = { queue.Get() };
   ComPtr<ID3D11Device> device11;
   ComPtr<ID3D11DeviceContext> immediate;
   check(D3D11On12CreateDevice(device.Get(), D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
      nullptr, 0, queues, 1, 0, &device11, &immediate, nullptr));
   ComPtr<ID3D11On12Device> interop;
   check(device11.As(&interop));
   ComPtr<IDXGIDevice> dxgiDevice;
   check(device11.As(&dxgiDevice));
   D2D1_FACTORY_OPTIONS options = {};
   options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
   ComPtr<ID2D1Factory1> factory;
   check(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, options, factory.GetAddressOf()));
   ComPtr<ID2D1Multithread> multithread;
   check(factory.As(&multithread));
   require(multithread->GetMultithreadProtected(), "D2D factory must be multithreaded");
   ComPtr<ID2D1Device> d2dDevice;
   check(factory->CreateDevice(dxgiDevice.Get(), &d2dDevice));
   ComPtr<ID2D1DeviceContext> layer, worker;
   check(d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &layer));
   check(d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &worker));

   D3D12_HEAP_PROPERTIES heap = {};
   heap.Type = D3D12_HEAP_TYPE_DEFAULT;
   D3D12_RESOURCE_DESC desc = {};
   desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
   desc.Width = desc.Height = 64;
   desc.DepthOrArraySize = desc.MipLevels = 1;
   desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
   desc.SampleDesc.Count = 1;
   desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
   ComPtr<ID3D12Resource> resource;
   check(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc,
      D3D12_RESOURCE_STATE_RENDER_TARGET, nullptr, IID_PPV_ARGS(&resource)));
   D3D11_RESOURCE_FLAGS flags = {};
   flags.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
   ComPtr<ID3D11Resource> wrapped;
   check(interop->CreateWrappedResource(resource.Get(), &flags, D3D12_RESOURCE_STATE_RENDER_TARGET,
      D3D12_RESOURCE_STATE_RENDER_TARGET, IID_PPV_ARGS(&wrapped)));
   ComPtr<IDXGISurface> surface;
   check(wrapped.As(&surface));
   auto props = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET,
      D2D1::PixelFormat(desc.Format, D2D1_ALPHA_MODE_PREMULTIPLIED));
   ComPtr<ID2D1Bitmap1> layerBitmap, iconBitmap, readback;
   check(layer->CreateBitmapFromDxgiSurface(surface.Get(), &props, &layerBitmap));
   check(worker->CreateBitmap(D2D1::SizeU(64, 64), nullptr, 0, &props, &iconBitmap));
   props.bitmapOptions = D2D1_BITMAP_OPTIONS_CPU_READ | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
   check(worker->CreateBitmap(D2D1::SizeU(64, 64), nullptr, 0, &props, &readback));
   layer->SetTarget(layerBitmap.Get());
   worker->SetTarget(iconBitmap.Get());
   std::atomic<bool> start{false};
   std::thread shell([&]
   {
      check(CoInitializeEx(nullptr, COINIT_MULTITHREADED));
      while (!start.load()) std::this_thread::yield();
      for (int i = 0; i < 1000; ++i)
      {
         worker->BeginDraw();
         worker->Clear(D2D1::ColorF(D2D1::ColorF::Red));
         if ((i % 4) == 0) std::this_thread::yield();
         check(worker->EndDraw());
      }
      CoUninitialize();
   });
   start = true;
   ID3D11Resource * resources[] = { wrapped.Get() };
   for (int i = 0; i < 1000; ++i)
   {
      { factory_lock lock(multithread.Get()); interop->AcquireWrappedResources(resources, 1); }
      layer->BeginDraw();
      layer->Clear(D2D1::ColorF(D2D1::ColorF::Blue));
      if ((i % 4) == 0) std::this_thread::yield();
      check(layer->EndDraw());
      {
         factory_lock lock(multithread.Get());
         interop->ReleaseWrappedResources(resources, 1);
         immediate->Flush();
      }
   }
   shell.join();
   worker->SetTarget(nullptr);
   layer->SetTarget(nullptr);
   check(readback->CopyFromBitmap(nullptr, iconBitmap.Get(), nullptr));
   D2D1_MAPPED_RECT mapped = {};
   check(readback->Map(D2D1_MAP_OPTIONS_READ, &mapped));
   for (UINT y = 0; y < 64; ++y)
      for (UINT x = 0; x < 64; ++x)
      {
         const auto pixel = mapped.bits + y * mapped.pitch + x * 4;
         require(pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 255 && pixel[3] == 255,
            "Worker icon pixels were corrupted");
      }
   check(readback->Unmap());
   // Complete submitted work without holding the factory lock. Queue/fence
   // waits are deliberately outside the interop critical sections.
   ComPtr<ID3D12Fence> fence;
   check(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
   check(queue->Signal(fence.Get(), 1));
   HANDLE completed = CreateEvent(nullptr, FALSE, FALSE, nullptr);
   require(completed != nullptr, "CreateEvent failed");
   check(fence->SetEventOnCompletion(1, completed));
   require(WaitForSingleObject(completed, 30000) == WAIT_OBJECT_0, "GPU work did not complete");
   CloseHandle(completed);
   ComPtr<ID3D11InfoQueue> info;
   check(device11.As(&info));
   for (UINT64 i = 0; i < info->GetNumStoredMessagesAllowedByRetrievalFilter(); ++i)
   {
      SIZE_T size = 0;
      check(info->GetMessage(i, nullptr, &size));
      std::vector<unsigned char> bytes(size);
      auto message = reinterpret_cast<D3D11_MESSAGE *>(bytes.data());
      check(info->GetMessage(i, message, &size));
      require(message->Severity != D3D11_MESSAGE_SEVERITY_ERROR &&
         message->Severity != D3D11_MESSAGE_SEVERITY_CORRUPTION, message->pDescription);
   }
   ComPtr<ID3D12InfoQueue> info12;
   check(device.As(&info12));
   for (UINT64 i = 0; i < info12->GetNumStoredMessagesAllowedByRetrievalFilter(); ++i)
   {
      SIZE_T size = 0;
      check(info12->GetMessage(i, nullptr, &size));
      std::vector<unsigned char> bytes(size);
      auto message = reinterpret_cast<D3D12_MESSAGE *>(bytes.data());
      check(info12->GetMessage(i, message, &size));
      require(message->Severity != D3D12_MESSAGE_SEVERITY_ERROR &&
         message->Severity != D3D12_MESSAGE_SEVERITY_CORRUPTION, message->pDescription);
   }
   std::puts("PASS: 1000 layer handoffs + 1000 concurrent icon draws; icon pixels intact, no D3D11/D3D12 errors.");
}

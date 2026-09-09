#pragma once

#include <d3d12.h>
#include <cstring>

namespace gpu_directx12::detail
{
   // Raw pixel upload: no color conversion, compression or planar formats.
   inline UINT pixel_upload_bytes(DXGI_FORMAT format)
   {
      switch (format)
      {
      case DXGI_FORMAT_R8_UNORM: return 1;
      case DXGI_FORMAT_R8G8_UNORM: return 2;
      case DXGI_FORMAT_B8G8R8A8_UNORM:
      case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
      case DXGI_FORMAT_B8G8R8A8_TYPELESS:
      case DXGI_FORMAT_R8G8B8A8_UNORM:
      case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
      case DXGI_FORMAT_R8G8B8A8_TYPELESS: return 4;
      case DXGI_FORMAT_R16G16B16A16_FLOAT: return 8;
      case DXGI_FORMAT_R32G32B32A32_FLOAT: return 16;
      default: return 0;
      }
   }

   // Returns one owned COM reference. Each call gets independent staging storage.
   inline HRESULT create_pixel_upload(ID3D12Device * device, const D3D12_RESOURCE_DESC & target,
      const void * data, int width, int height, int scan, int bytesPerPixel,
      int x, int y, int mip, int layer, ID3D12Resource ** upload,
      D3D12_PLACED_SUBRESOURCE_FOOTPRINT & footprint, UINT & subresource)
   {
      if (!upload) return E_POINTER;
      *upload = nullptr;
      if (!device || !data) return E_POINTER;
      if (width <= 0 || height <= 0 || x < 0 || y < 0 || mip < 0 || layer < 0 ||
          mip >= target.MipLevels || mip >= 64 || layer >= target.DepthOrArraySize ||
          target.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE2D || target.SampleDesc.Count != 1 ||
          (target.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) ||
          bytesPerPixel <= 0 || pixel_upload_bytes(target.Format) != (UINT)bytesPerPixel)
         return E_INVALIDARG;

      const UINT64 rowBytes = (UINT64)width * (UINT)bytesPerPixel;
      UINT64 mipWidth = target.Width >> mip, mipHeight = (UINT64)target.Height >> mip;
      if (!mipWidth) mipWidth = 1;
      if (!mipHeight) mipHeight = 1;
      if (scan <= 0 || (UINT64)scan < rowBytes ||
          (UINT64)x + (UINT)width > mipWidth || (UINT64)y + (UINT)height > mipHeight ||
          (UINT64)(height - 1) * (UINT)scan + rowBytes > (SIZE_T)-1)
         return E_INVALIDARG;

      auto region = target;
      region.Width = (UINT)width;
      region.Height = (UINT)height;
      region.DepthOrArraySize = 1;
      region.MipLevels = 1;
      region.Alignment = 0;
      region.Flags = D3D12_RESOURCE_FLAG_NONE;
      region.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
      UINT rows = 0;
      UINT64 rowSize = 0, totalBytes = 0;
      device->GetCopyableFootprints(&region, 0, 1, 0, &footprint, &rows, &rowSize, &totalBytes);
      if (!totalBytes || totalBytes == (UINT64)-1 || totalBytes > (SIZE_T)-1 ||
          rows != (UINT)height || rowSize != rowBytes || footprint.Footprint.RowPitch < rowBytes)
         return E_INVALIDARG;

      D3D12_HEAP_PROPERTIES heap{};
      heap.Type = D3D12_HEAP_TYPE_UPLOAD;
      heap.CreationNodeMask = heap.VisibleNodeMask = 1;
      D3D12_RESOURCE_DESC buffer{};
      buffer.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
      buffer.Width = totalBytes;
      buffer.Height = buffer.DepthOrArraySize = buffer.MipLevels = 1;
      buffer.SampleDesc.Count = 1;
      buffer.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
      ID3D12Resource * staging = nullptr;
      auto hr = device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &buffer,
         D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&staging));
      if (FAILED(hr)) return hr;

      void * mapped = nullptr;
      const D3D12_RANGE noRead{0, 0};
      hr = staging->Map(0, &noRead, &mapped);
      if (FAILED(hr)) { staging->Release(); return hr; }
      auto destination = static_cast<unsigned char *>(mapped) + footprint.Offset;
      auto source = static_cast<const unsigned char *>(data);
      for (UINT row = 0; row < rows; ++row)
         std::memcpy(destination + (SIZE_T)row * footprint.Footprint.RowPitch,
            source + (SIZE_T)row * scan, (SIZE_T)rowBytes);
      const D3D12_RANGE written{(SIZE_T)footprint.Offset,
         (SIZE_T)(footprint.Offset + (rows - 1) * (UINT64)footprint.Footprint.RowPitch + rowBytes)};
      staging->Unmap(0, &written);
      subresource = (UINT)mip + (UINT)layer * target.MipLevels;
      *upload = staging;
      return S_OK;
   }

   inline void record_pixel_upload(ID3D12GraphicsCommandList * commands, ID3D12Resource * target,
      ID3D12Resource * upload, const D3D12_PLACED_SUBRESOURCE_FOOTPRINT & footprint,
      UINT subresource, UINT x, UINT y, D3D12_RESOURCE_STATES previousState)
   {
      D3D12_RESOURCE_BARRIER barrier{};
      barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
      barrier.Transition.pResource = target;
      barrier.Transition.Subresource = subresource;
      barrier.Transition.StateBefore = previousState;
      barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
      if (previousState != D3D12_RESOURCE_STATE_COPY_DEST)
         commands->ResourceBarrier(1, &barrier);

      D3D12_TEXTURE_COPY_LOCATION destination{};
      destination.pResource = target;
      destination.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
      destination.SubresourceIndex = subresource;
      D3D12_TEXTURE_COPY_LOCATION source{};
      source.pResource = upload;
      source.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
      source.PlacedFootprint = footprint;
      commands->CopyTextureRegion(&destination, x, y, 0, &source, nullptr);

      // The tracked state remains valid: the copy leaves the subresource as found.
      if (previousState != D3D12_RESOURCE_STATE_COPY_DEST)
      {
         barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
         barrier.Transition.StateAfter = previousState;
         commands->ResourceBarrier(1, &barrier);
      }
   }
}

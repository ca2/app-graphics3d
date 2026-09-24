#pragma once

#include "pixel_upload.h"

namespace gpu_directx12::detail
{
   // Preserve the top-left overlap of every matching mip/layer, without
   // resizing/filtering. Both resources leave the copy in their original state.
   inline HRESULT copy_texture_region(ID3D12GraphicsCommandList * commands,
      ID3D12Resource * target, ID3D12Resource * source,
      D3D12_RESOURCE_STATES targetState, D3D12_RESOURCE_STATES sourceState)
   {
      if (!commands || commands->GetType() != D3D12_COMMAND_LIST_TYPE_DIRECT || !target || !source)
         return E_INVALIDARG;
      if (target == source) return S_OK;
      const auto dst = target->GetDesc();
      const auto src = source->GetDesc();
      if (dst.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE2D || src.Dimension != dst.Dimension ||
          src.Format != dst.Format || !pixel_upload_bytes(src.Format) ||
          src.SampleDesc.Count != 1 || dst.SampleDesc.Count != 1 ||
          src.DepthOrArraySize != dst.DepthOrArraySize || src.MipLevels != dst.MipLevels ||
          (src.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) ||
          (dst.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL))
         return E_INVALIDARG;
      auto transition = [commands](ID3D12Resource * resource, D3D12_RESOURCE_STATES before,
                                   D3D12_RESOURCE_STATES after)
      {
         if (before == after) return;
         D3D12_RESOURCE_BARRIER barrier{};
         barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
         barrier.Transition = {resource, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, before, after};
         commands->ResourceBarrier(1, &barrier);
      };
      transition(target, targetState, D3D12_RESOURCE_STATE_COPY_DEST);
      transition(source, sourceState, D3D12_RESOURCE_STATE_COPY_SOURCE);
      for (UINT layer = 0; layer < src.DepthOrArraySize; ++layer)
         for (UINT mip = 0; mip < src.MipLevels; ++mip)
         {
            UINT64 width = (src.Width < dst.Width ? src.Width : dst.Width) >> mip;
            UINT height = (src.Height < dst.Height ? src.Height : dst.Height) >> mip;
            D3D12_BOX box{0, 0, 0, (UINT)(width ? width : 1), height ? height : 1, 1};
            D3D12_TEXTURE_COPY_LOCATION from{}, to{};
            from.pResource = source;
            from.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
            from.SubresourceIndex = layer * src.MipLevels + mip;
            to.pResource = target;
            to.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
            to.SubresourceIndex = layer * dst.MipLevels + mip;
            commands->CopyTextureRegion(&to, 0, 0, 0, &from, &box);
         }
      transition(source, D3D12_RESOURCE_STATE_COPY_SOURCE, sourceState);
      transition(target, D3D12_RESOURCE_STATE_COPY_DEST, targetState);
      return S_OK;
   }
}

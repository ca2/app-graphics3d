#pragma once

#include "pixel_upload.h"
#include "_d3dx12.h"

namespace gpu_directx12::detail
{
   // Each input is mip zero of one array slice/cube face, not a consecutive mip.
   // Validate every face before recording any copy. Restore the original state
   // so the resource's CPU-side state tracker remains accurate.
   inline HRESULT update_static_pixels(ID3D12Device * device, ID3D12GraphicsCommandList * commands,
      ID3D12Resource * target, ID3D12Resource * upload, UINT count,
      const D3D12_SUBRESOURCE_DATA * data, D3D12_RESOURCE_STATES previousState)
   {
      if (!device || !commands || !target || !upload || !data) return E_POINTER;
      // Reject before recording: COPY/COMPUTE lists cannot support every state
      // this helper restores. Callers must use the graphics queue.
      if (commands->GetType() != D3D12_COMMAND_LIST_TYPE_DIRECT) return E_INVALIDARG;
      const auto desc = target->GetDesc();
      const auto staging = upload->GetDesc();
      const auto bytesPerPixel = pixel_upload_bytes(desc.Format);
      if (desc.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE2D || desc.SampleDesc.Count != 1 ||
          !desc.MipLevels || !bytesPerPixel || (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) ||
          count == 0 || count > 6 || count != desc.DepthOrArraySize ||
          staging.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER)
         return E_INVALIDARG;

      D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprints[6]{};
      UINT rows[6]{};
      UINT64 rowBytes[6]{}, sizes[6]{};
      UINT64 end = 0;
      for (UINT face = 0; face < count; ++face)
      {
         const UINT64 offset = (end + D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT - 1) &
            ~(UINT64)(D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT - 1);
         device->GetCopyableFootprints(&desc, face * desc.MipLevels, 1, offset,
            &footprints[face], &rows[face], &rowBytes[face], &sizes[face]);
         if (!sizes[face] || sizes[face] == (UINT64)-1 || offset > staging.Width ||
             sizes[face] > staging.Width - offset || rows[face] != desc.Height ||
             rowBytes[face] != desc.Width * bytesPerPixel || !data[face].pData ||
             data[face].RowPitch <= 0 || (UINT64)data[face].RowPitch < rowBytes[face] ||
             data[face].SlicePitch <= 0 ||
             (UINT64)data[face].RowPitch > (UINT64)(SIZE_T)-1 / desc.Height)
            return E_INVALIDARG;
         const UINT64 extent = (rows[face] - 1) * (UINT64)data[face].RowPitch + rowBytes[face];
         if ((UINT64)data[face].SlicePitch < extent) return E_INVALIDARG;
         end = offset + sizes[face];
      }

      D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
         target, previousState, D3D12_RESOURCE_STATE_COPY_DEST);
      if (previousState != D3D12_RESOURCE_STATE_COPY_DEST)
         commands->ResourceBarrier(1, &barrier);

      HRESULT result = S_OK;
      for (UINT face = 0; face < count; ++face)
      {
         if (!UpdateSubresources(commands, target, upload, face * desc.MipLevels, 1,
             sizes[face], &footprints[face], &rows[face], &rowBytes[face], &data[face]))
         {
            result = E_FAIL;
            break;
         }
      }

      if (previousState != D3D12_RESOURCE_STATE_COPY_DEST)
      {
         barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
         barrier.Transition.StateAfter = previousState;
         commands->ResourceBarrier(1, &barrier);
      }
      return result;
   }
}

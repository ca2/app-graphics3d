#pragma once


#include <cstddef>
#include <cstring>


namespace gpu_vulkan
{


   enum class enum_cube_map_copy_operation
   {

      copy_unchanged,
      rotate_180,

   };


   struct cube_map_face_rule
   {

      int m_iSourceFace;
      enum_cube_map_copy_operation m_eoperation;

   };


   template < typename PIXEL >
   void pack_cube_map_for_vulkan(
      PIXEL *pdestination,
      int width,
      int height,
      const PIXEL *const psourcefaces[6],
      const int sourcescana[6])
   {

      constexpr cube_map_face_rule vulkanCubeMapFaceRules[6] =
      {

         {0, enum_cube_map_copy_operation::rotate_180},     // Positive X
         {1, enum_cube_map_copy_operation::rotate_180},     // Negative X
         {3, enum_cube_map_copy_operation::copy_unchanged}, // Positive Y
         {2, enum_cube_map_copy_operation::copy_unchanged}, // Negative Y
         {5, enum_cube_map_copy_operation::rotate_180},     // Positive Z
         {4, enum_cube_map_copy_operation::rotate_180},     // Negative Z

      };

      const auto destinationScan = static_cast<std::size_t>(width) * sizeof(PIXEL);
      const auto destinationFaceSize = destinationScan * static_cast<std::size_t>(height);
      auto *pdestinationBytes = reinterpret_cast<std::byte *>(pdestination);

      for (int destinationFace = 0; destinationFace < 6; ++destinationFace)
      {

         const auto &rule = vulkanCubeMapFaceRules[destinationFace];
         const auto *psourceBytes =
            reinterpret_cast<const std::byte *>(psourcefaces[rule.m_iSourceFace]);
         auto *pdestinationFace =
            pdestinationBytes + destinationFaceSize * destinationFace;

         for (int y = 0; y < height; ++y)
         {

            const auto sourceY =
               rule.m_eoperation == enum_cube_map_copy_operation::rotate_180
                  ? height - y - 1
                  : y;
            const auto *psourceRow = reinterpret_cast<const PIXEL *>(
               psourceBytes + static_cast<std::size_t>(sourceY) * sourcescana[rule.m_iSourceFace]);
            auto *pdestinationRow = reinterpret_cast<PIXEL *>(
               pdestinationFace + static_cast<std::size_t>(y) * destinationScan);

            if (rule.m_eoperation == enum_cube_map_copy_operation::rotate_180)
            {

               for (int x = 0; x < width; ++x)
               {

                  pdestinationRow[x] = psourceRow[width - x - 1];

               }

            }
            else
            {

               std::memcpy(pdestinationRow, psourceRow, destinationScan);

            }

         }

      }

   }


} // namespace gpu_vulkan

#include "../cube_map_upload.h"

#include <array>
#include <cassert>
#include <cstdint>


int main()
{

   using pixel_t = std::uint32_t;

   std::array<std::array<pixel_t, 4>, 6> source{};

   for (std::size_t face = 0; face < source.size(); ++face)
   {

      source[face] = {
         static_cast<pixel_t>(face * 100 + 1),
         static_cast<pixel_t>(face * 100 + 2),
         static_cast<pixel_t>(face * 100 + 3),
         static_cast<pixel_t>(face * 100 + 4),
      };

   }

   const auto sourceBefore = source;
   const pixel_t *sourceFaces[6];
   int sourceScans[6];

   for (std::size_t face = 0; face < source.size(); ++face)
   {

      sourceFaces[face] = source[face].data();
      sourceScans[face] = 2 * static_cast<int>(sizeof(pixel_t));

   }

   std::array<pixel_t, 24> destination{};

   gpu_vulkan::pack_cube_map_for_vulkan(
      destination.data(), 2, 2, sourceFaces, sourceScans);

   constexpr std::array<int, 6> sourceFaceIndices{0, 1, 3, 2, 5, 4};
   constexpr std::array<bool, 6> rotate180{true, true, false, false, true, true};

   for (std::size_t destinationFace = 0;
        destinationFace < sourceFaceIndices.size();
        ++destinationFace)
   {

      const auto &sourceFace = source[sourceFaceIndices[destinationFace]];
      const auto destinationOffset = destinationFace * 4;

      if (rotate180[destinationFace])
      {

         assert(destination[destinationOffset + 0] == sourceFace[3]);
         assert(destination[destinationOffset + 1] == sourceFace[2]);
         assert(destination[destinationOffset + 2] == sourceFace[1]);
         assert(destination[destinationOffset + 3] == sourceFace[0]);

      }
      else
      {

         assert(destination[destinationOffset + 0] == sourceFace[0]);
         assert(destination[destinationOffset + 1] == sourceFace[1]);
         assert(destination[destinationOffset + 2] == sourceFace[2]);
         assert(destination[destinationOffset + 3] == sourceFace[3]);

      }

   }

   assert(source == sourceBefore);

   return 0;

}

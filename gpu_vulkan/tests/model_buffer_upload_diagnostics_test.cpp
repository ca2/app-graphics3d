#include "../model_buffer_upload_diagnostics.h"

#include <array>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


namespace
{


   std::string read_text_file(const char * pszPath)
   {

      std::ifstream stream(pszPath, std::ios::binary);
      std::ostringstream output;
      output << stream.rdbuf();
      return output.str();

   }


}


int main()
{

   const std::array<std::uint32_t, 6> indexes{0, 1, 2, 2, 3, 0};

   const auto valid = gpu_vulkan::inspect_model_buffer_upload(
      4,
      44,
      4 * 44,
      indexes.size(),
      sizeof(indexes[0]),
      indexes.size() * sizeof(indexes[0]),
      indexes.data());

   assert(valid.m_uRequiredVertexBytes == 4 * 44);
   assert(valid.m_uRequiredIndexBytes == indexes.size() * sizeof(indexes[0]));
   assert(valid.m_uMaximumIndex == 3);
   assert(valid.m_bVertexBufferLargeEnough);
   assert(valid.m_bIndexBufferLargeEnough);
   assert(valid.m_bIndexesInVertexRange);
   assert(valid.is_valid());

   const auto undersizedIndexBuffer = gpu_vulkan::inspect_model_buffer_upload(
      4,
      44,
      4 * 44,
      indexes.size(),
      sizeof(indexes[0]),
      indexes.size() * sizeof(indexes[0]) - 1,
      indexes.data());

   assert(!undersizedIndexBuffer.m_bIndexBufferLargeEnough);
   assert(!undersizedIndexBuffer.is_valid());

   const std::array<std::uint32_t, 3> outOfRangeIndexes{0, 1, 4};

   const auto outOfRange = gpu_vulkan::inspect_model_buffer_upload(
      4,
      44,
      4 * 44,
      outOfRangeIndexes.size(),
      sizeof(outOfRangeIndexes[0]),
      outOfRangeIndexes.size() * sizeof(outOfRangeIndexes[0]),
      outOfRangeIndexes.data());

   assert(outOfRange.m_uMaximumIndex == 4);
   assert(!outOfRange.m_bIndexesInVertexRange);
   assert(!outOfRange.is_valid());

   constexpr std::size_t stoneSphereVertexCount = 4'394;
   constexpr std::size_t stoneSphereIndexCount = 26'352;
   constexpr std::size_t vertexTypeSize = 44;

   std::vector<std::uint32_t> stoneSphereIndexes(stoneSphereIndexCount);
   for (std::size_t i = 0; i < stoneSphereIndexes.size(); ++i)
   {

      stoneSphereIndexes[i] = static_cast<std::uint32_t>(i % stoneSphereVertexCount);

   }

   const auto stoneSphere = gpu_vulkan::inspect_model_buffer_upload(
      stoneSphereVertexCount,
      vertexTypeSize,
      stoneSphereVertexCount * vertexTypeSize,
      stoneSphereIndexes.size(),
      sizeof(stoneSphereIndexes[0]),
      stoneSphereIndexes.size() * sizeof(stoneSphereIndexes[0]),
      stoneSphereIndexes.data());

   assert(stoneSphere.m_uMaximumIndex == stoneSphereVertexCount - 1);
   assert(stoneSphere.is_valid());

   const auto meshImplementation = read_text_file("gpu_vulkan/gltf/mesh.cpp");
   const auto inspectPosition = meshImplementation.find("inspect_model_buffer_upload(");
   const auto logPosition = meshImplementation.find("gpu_vulkan direct mesh upload diagnostic:", inspectPosition);
   const auto validationPosition = meshImplementation.find("if (!diagnostic.is_valid())", inspectPosition);
   const auto drawPosition = meshImplementation.find("vkCmdDrawIndexed(", inspectPosition);

   assert(inspectPosition != std::string::npos);
   assert(logPosition != std::string::npos);
   assert(validationPosition != std::string::npos);
   assert(drawPosition != std::string::npos);
   assert(inspectPosition < logPosition);
   assert(logPosition < validationPosition);
   assert(validationPosition < drawPosition);

   return 0;

}

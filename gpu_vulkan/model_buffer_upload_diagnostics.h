#pragma once


#include <cstddef>
#include <cstdint>
#include <limits>


namespace gpu_vulkan
{


   struct model_buffer_upload_diagnostic
   {


      std::uint64_t m_uRequiredVertexBytes = 0;
      std::uint64_t m_uRequiredIndexBytes = 0;
      std::uint64_t m_uMaximumIndex = 0;
      bool m_bVertexBufferLargeEnough = false;
      bool m_bIndexBufferLargeEnough = false;
      bool m_bIndexTypeSupported = false;
      bool m_bIndexesInVertexRange = false;


      bool is_valid() const
      {

         return m_bVertexBufferLargeEnough
            && m_bIndexBufferLargeEnough
            && m_bIndexTypeSupported
            && m_bIndexesInVertexRange;

      }


   };


   template < typename INDEX >
   inline std::uint64_t maximum_model_index(const void *pIndexes, std::size_t uIndexCount)
   {

      const auto *pIndex = static_cast<const INDEX *>(pIndexes);
      std::uint64_t uMaximumIndex = 0;

      for (std::size_t i = 0; i < uIndexCount; ++i)
      {

         if (static_cast<std::uint64_t>(pIndex[i]) > uMaximumIndex)
         {

            uMaximumIndex = static_cast<std::uint64_t>(pIndex[i]);

         }

      }

      return uMaximumIndex;

   }


   inline model_buffer_upload_diagnostic inspect_model_buffer_upload(
      std::size_t uVertexCount,
      std::size_t uVertexTypeSize,
      std::size_t uVertexBufferBytes,
      std::size_t uIndexCount,
      std::size_t uIndexTypeSize,
      std::size_t uIndexBufferBytes,
      const void *pIndexes)
   {

      model_buffer_upload_diagnostic diagnostic;

      diagnostic.m_uRequiredVertexBytes =
         static_cast<std::uint64_t>(uVertexCount) * static_cast<std::uint64_t>(uVertexTypeSize);
      diagnostic.m_uRequiredIndexBytes =
         static_cast<std::uint64_t>(uIndexCount) * static_cast<std::uint64_t>(uIndexTypeSize);
      diagnostic.m_bVertexBufferLargeEnough =
         static_cast<std::uint64_t>(uVertexBufferBytes) >= diagnostic.m_uRequiredVertexBytes;
      diagnostic.m_bIndexBufferLargeEnough =
         static_cast<std::uint64_t>(uIndexBufferBytes) >= diagnostic.m_uRequiredIndexBytes;

      if (uIndexCount == 0)
      {

         diagnostic.m_bIndexTypeSupported =
            uIndexTypeSize == sizeof(std::uint8_t)
            || uIndexTypeSize == sizeof(std::uint16_t)
            || uIndexTypeSize == sizeof(std::uint32_t);
         diagnostic.m_bIndexesInVertexRange = true;

         return diagnostic;

      }

      if (!pIndexes)
      {

         return diagnostic;

      }

      switch (uIndexTypeSize)
      {
      case sizeof(std::uint8_t):
         diagnostic.m_bIndexTypeSupported = true;
         diagnostic.m_uMaximumIndex = maximum_model_index<std::uint8_t>(pIndexes, uIndexCount);
         break;
      case sizeof(std::uint16_t):
         diagnostic.m_bIndexTypeSupported = true;
         diagnostic.m_uMaximumIndex = maximum_model_index<std::uint16_t>(pIndexes, uIndexCount);
         break;
      case sizeof(std::uint32_t):
         diagnostic.m_bIndexTypeSupported = true;
         diagnostic.m_uMaximumIndex = maximum_model_index<std::uint32_t>(pIndexes, uIndexCount);
         break;
      default:
         return diagnostic;
      }

      diagnostic.m_bIndexesInVertexRange = diagnostic.m_uMaximumIndex < uVertexCount;

      return diagnostic;

   }


} // namespace gpu_vulkan

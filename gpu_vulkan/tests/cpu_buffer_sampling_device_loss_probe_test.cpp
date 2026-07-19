#include <fstream>
#include <iterator>
#include <sstream>
#include <string>


namespace
{


   std::string read_file(const char *path)
   {

      std::ifstream stream(path, std::ios::binary);

      return {
         std::istreambuf_iterator<char>(stream),
         std::istreambuf_iterator<char>()};

   }


   bool contains_uncommented_call(const std::string & source, const std::string & call)
   {

      std::istringstream stream(source);
      std::string line;

      while (std::getline(stream, line))
      {

         const auto firstNonWhitespace = line.find_first_not_of(" \t\r");

         if (firstNonWhitespace != std::string::npos
            && line.compare(firstNonWhitespace, call.size(), call) == 0)
         {

            return true;

         }

      }

      return false;

   }


} // namespace


int main()
{

   const auto rendererHeader = read_file("gpu_vulkan/renderer.h");
   const auto rendererImplementation = read_file("gpu_vulkan/renderer.cpp");
   const auto fenceImplementation = read_file("gpu_vulkan/fence.cpp");
   const auto textureHeader = read_file("gpu_vulkan/texture.h");
   const auto textureImplementation = read_file("gpu_vulkan/texture.cpp");
   const auto bufferHeader = read_file("gpu_vulkan/buffer.h");
   const auto bufferImplementation = read_file("gpu_vulkan/buffer.cpp");
   const auto contextImplementation = read_file("gpu_vulkan/context.cpp");

   if (rendererHeader.find("probe_queue_submission") == std::string::npos)
   {

      return 1;

   }

   const auto probeCall = rendererImplementation.find("m_pcpubuffersampler->probe_queue_submission()");
   const auto sampleCall = rendererImplementation.find("m_pcpubuffersampler->sample(ptexture, ecompletion)");

   if (probeCall == std::string::npos || sampleCall == std::string::npos || probeCall >= sampleCall)
   {

      return 1;

   }

   if (rendererImplementation.find("gpu_vulkan cpu sample empty queue probe succeeded") == std::string::npos)
   {

      return 1;

   }

   if (fenceImplementation.find("gpu_vulkan fence wait: result={}") == std::string::npos)
   {

      return 1;

   }

   const auto sampleBegin = rendererImplementation.find("void renderer::cpu_buffer_sampler::sample(");
   const auto sampleEnd = rendererImplementation.find(
      "void renderer::cpu_buffer_sampler::probe_queue_submission()",
      sampleBegin);

   if (sampleBegin == std::string::npos || sampleEnd == std::string::npos)
   {

      return 1;

   }

   const auto sampleImplementation =
      rendererImplementation.substr(sampleBegin, sampleEnd - sampleBegin);

   const auto sourceTransition = rendererImplementation.find("cpu_sample_stage_source_transition");
   const auto bufferFillProbe = rendererImplementation.find("cpu_sample_stage_buffer_fill_probe");
   const auto bufferCopy = rendererImplementation.find("cpu_sample_stage_buffer_copy");
   const auto hostVisibility = rendererImplementation.find("cpu_sample_stage_host_visibility");

   if (sourceTransition == std::string::npos
      || bufferFillProbe == std::string::npos
      || bufferCopy == std::string::npos
      || hostVisibility == std::string::npos
      || !(sourceTransition < bufferFillProbe
         && bufferFillProbe < bufferCopy
         && bufferCopy < hostVisibility))
   {

      return 1;

   }

   if (rendererHeader.find("::pointer_array < buffer >") == std::string::npos
      || rendererImplementation.find("VK_BUFFER_USAGE_TRANSFER_DST_BIT") == std::string::npos
      || rendererImplementation.find("VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT") == std::string::npos
      || rendererImplementation.find("VK_MEMORY_PROPERTY_HOST_COHERENT_BIT") == std::string::npos
      || rendererImplementation.find("source_usage={}") == std::string::npos
      || !contains_uncommented_call(sampleImplementation, "vkCmdFillBuffer(")
      || !contains_uncommented_call(sampleImplementation, "vkCmdCopyImageToBuffer(")
      || !contains_uncommented_call(sampleImplementation, "vkCmdPipelineBarrier("))
   {

      return 1;

   }

   if (contains_uncommented_call(sampleImplementation, "vkCmdCopyImage(")
      || contains_uncommented_call(sampleImplementation, "vkCmdClearColorImage(")
      || rendererImplementation.find("iWidth * 4") == std::string::npos)
   {

      return 1;

   }

   if (textureHeader.find("VkImageUsageFlags") == std::string::npos
      || textureHeader.find("m_vkimageusageflags") == std::string::npos
      || textureImplementation.find("m_vkimageusageflags = imagecreateinfo.usage") == std::string::npos)
   {

      return 1;

   }

   if (bufferHeader.find("m_vkdevicesizeAllocation") == std::string::npos
      || bufferHeader.find("m_vkdevicesizeAlignment") == std::string::npos
      || bufferHeader.find("m_uMemoryTypeBits") == std::string::npos
      || bufferHeader.find("m_uMemoryTypeIndex") == std::string::npos
      || bufferHeader.find("m_vkmemorypropertyflagsRequested") == std::string::npos
      || bufferHeader.find("m_vkmemorypropertyflagsSelected") == std::string::npos
      || bufferImplementation.find("m_pMapped = nullptr") == std::string::npos)
   {

      return 1;

   }

   if (contextImplementation.find("auto vkresultBind = vkBindBufferMemory(") == std::string::npos
      || contextImplementation.find("vkresultBind != VK_SUCCESS") == std::string::npos)
   {

      return 1;

   }

   if (rendererImplementation.find("destination_allocation_bytes={}") == std::string::npos
      || rendererImplementation.find("destination_alignment={}") == std::string::npos
      || rendererImplementation.find("destination_memory_type_bits={}") == std::string::npos
      || rendererImplementation.find("destination_memory_type_index={}") == std::string::npos
      || rendererImplementation.find("destination_memory_flags_requested={}") == std::string::npos
      || rendererImplementation.find("destination_memory_flags_selected={}") == std::string::npos)
   {

      return 1;

   }

   return 0;

}

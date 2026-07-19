#include <fstream>
#include <iterator>
#include <string>


namespace
{


   std::string read_file(const char * path)
   {

      std::ifstream stream(path, std::ios::binary);

      return {
         std::istreambuf_iterator<char>(stream),
         std::istreambuf_iterator<char>()};

   }


   std::string function_text(
      const std::string & source,
      const std::string & signature,
      const std::string & nextSignature)
   {

      const auto begin = source.find(signature);
      const auto end = source.find(nextSignature, begin);

      if (begin == std::string::npos || end == std::string::npos)
      {

         return {};

      }

      return source.substr(begin, end - begin);

   }


} // namespace


int main()
{

   const auto rendererHeader = read_file("gpu_vulkan/renderer.h");
   const auto rendererImplementation = read_file("gpu_vulkan/renderer.cpp");

   if (rendererHeader.find("void send_sample(const ::i32_size & sizeSample);") == std::string::npos)
   {

      return 1;

   }

   const auto sampleImplementation = function_text(
      rendererImplementation,
      "void renderer::sample()",
      "VkRenderPass renderer::getRenderPass() const");
   const auto sendSampleImplementation = function_text(
      rendererImplementation,
      "void renderer::cpu_buffer_sampler::send_sample(",
      "void renderer::sample()");

   if (sampleImplementation.empty() || sendSampleImplementation.empty())
   {

      return 1;

   }

   const auto textureAcquisition = sampleImplementation.find("current_texture(::gpu::current_layer())");
   const auto sampledExtent = sampleImplementation.find("const auto sizeSample = ptexture->rectangle().size();");
   const auto bufferUpdate = sampleImplementation.find("m_pcpubuffersampler->update(sizeSample);");
   const auto sampleSend = sampleImplementation.find("m_pcpubuffersampler->send_sample(sizeSample);");

   if (textureAcquisition == std::string::npos
      || sampledExtent == std::string::npos
      || bufferUpdate == std::string::npos
      || sampleSend == std::string::npos
      || !(textureAcquisition < sampledExtent
         && sampledExtent < bufferUpdate
         && bufferUpdate < sampleSend)
      || sampleImplementation.find("m_pgpucontext->m_rectangle.size()") != std::string::npos)
   {

      return 1;

   }

   if (sendSampleImplementation.find("const ::i32_size & sizeSample") == std::string::npos
      || sendSampleImplementation.find("const int iWidth = sizeSample.width();") == std::string::npos
      || sendSampleImplementation.find("const int iHeight = sizeSample.height();") == std::string::npos
      || sendSampleImplementation.find("m_pcontext->m_rectangle") != std::string::npos)
   {

      return 1;

   }

   return 0;

}

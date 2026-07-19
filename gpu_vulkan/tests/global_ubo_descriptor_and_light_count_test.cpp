#include <cassert>
#include <fstream>
#include <sstream>
#include <string>


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

   const auto bindingImplementation = read_text_file("gpu_vulkan/binding.cpp");
   const auto descriptorLoop = bindingImplementation.find("for (int i = 0; i < iFrameCount; i++)");
   const auto descriptorInfo = bindingImplementation.find("descriptor_info(i)", descriptorLoop);
   const auto descriptorBuild = bindingImplementation.find("vkdescriptorwriter.build", descriptorLoop);

   assert(descriptorLoop != std::string::npos);
   assert(descriptorInfo != std::string::npos);
   assert(descriptorBuild != std::string::npos);
   assert(descriptorLoop < descriptorInfo);
   assert(descriptorInfo < descriptorBuild);

   const auto blockHeader = read_text_file("gpu_vulkan/block.h");
   const auto blockImplementation = read_text_file("gpu_vulkan/block.cpp");

   assert(blockHeader.find("VkDescriptorSet m_vkdescriptorset") == std::string::npos);
   assert(blockImplementation.find("pbindingslotset->descriptor_set(pgpucommandbuffer)") != std::string::npos);

   const auto commandBufferImplementation = read_text_file("../app/bred/gpu/command_buffer.cpp");
   const auto genericLayerImplementation = read_text_file("../app/bred/gpu/layer.cpp");
   const auto vulkanLayerImplementation = read_text_file("gpu_vulkan/layer.cpp");

   assert(commandBufferImplementation.find("m_iCommandBufferFrameIndex2 = -1;") != std::string::npos);
   assert(genericLayerImplementation.find("pcommandbufferLayer->m_iCommandBufferFrameIndex2 = iFrameIndex;") != std::string::npos);
   assert(vulkanLayerImplementation.find("pcommandbufferLayer->m_iCommandBufferFrameIndex2 = iFrameIndex;") != std::string::npos);

   const auto shader = read_text_file("graphics3d_vulkan/render_system/shader/wavefront.frag");
   const auto clampedLightCount = shader.find("clamp(globalUbo.numLights, 0, 10)");
   const auto lightingLoop = shader.find("i < lightCount", clampedLightCount);

   assert(clampedLightCount != std::string::npos);
   assert(lightingLoop != std::string::npos);
   assert(clampedLightCount < lightingLoop);
   assert(shader.find("const int lightCount = 6") == std::string::npos);

   return 0;

}

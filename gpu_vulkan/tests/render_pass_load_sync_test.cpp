#include "../render_pass_load_sync.h"

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

   assert(gpu_vulkan::color_attachment_access(false) == VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
   assert(gpu_vulkan::color_attachment_access(true)
      == (VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT));

   assert(gpu_vulkan::depth_attachment_access(false) == VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
   assert(gpu_vulkan::depth_attachment_access(true)
      == (VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT));

   const auto rendererImplementation = read_text_file("gpu_vulkan/renderer.cpp");
   const auto renderPassImplementation = read_text_file("gpu_vulkan/render_pass.cpp");

   assert(rendererImplementation.find("color_attachment_access(true)") != std::string::npos);
   assert(rendererImplementation.find("depth_attachment_access(true)") != std::string::npos);
   assert(renderPassImplementation.find("dependency.dstAccessMask = color_attachment_access(bLoadExisting);")
      != std::string::npos);
   assert(renderPassImplementation.find("dependency.dstAccessMask |= depth_attachment_access(bLoadExisting);")
      != std::string::npos);

   return 0;

}

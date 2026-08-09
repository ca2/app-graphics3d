#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>


int main()
{

   const auto sourcePath = std::filesystem::path(__FILE__).parent_path().parent_path() / "graphics.cpp";
   std::ifstream source(sourcePath);

   if (!source)
   {

      std::cerr << "Unable to open " << sourcePath << '\n';
      return 1;

   }

   const std::string text((std::istreambuf_iterator<char>(source)), std::istreambuf_iterator<char>());
   const auto functionBegin = text.find("void graphics::end_layer(bool bClosingLayer)");
   const auto functionEnd = text.find("void graphics::on_present()", functionBegin);

   if (functionBegin == std::string::npos || functionEnd == std::string::npos)
   {

      std::cerr << "Unable to locate draw2d_vkvg::graphics::end_layer.\n";
      return 1;

   }

   const auto function = text.substr(functionBegin, functionEnd - functionBegin);
   const auto flush = function.find("vkvg_flush(vkvgcontext);");
   const auto handoff = function.find("::gpu::graphics::end_layer(bClosingLayer);");
   const auto unlock = function.find("m_queuehostcalllock.unlock();");

   if (flush == std::string::npos || handoff == std::string::npos || unlock == std::string::npos ||
       !(flush < handoff && handoff < unlock))
   {

      std::cerr << "The shared Vulkan queue lock must cover VKVG flush and the completed layer-copy handoff.\n";
      return 1;

   }

   return 0;

}

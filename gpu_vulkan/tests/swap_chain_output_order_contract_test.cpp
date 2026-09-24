#include <cassert>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>


namespace
{


   std::string read(const std::filesystem::path & path)
   {

      std::ifstream stream(path);
      assert(stream);
      return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};

   }


   std::string section(const std::string & text, const std::string & begin, const std::string & end)
   {

      const auto first = text.find(begin);
      const auto last = text.find(end, first);
      assert(first != std::string::npos);
      assert(last != std::string::npos);
      return text.substr(first, last - first);

   }


} // namespace


int main()
{

   const auto gpuVulkan = std::filesystem::path(__FILE__).parent_path().parent_path();
   const auto contextSource = read(gpuVulkan / "context.cpp");
   const auto endDraw = section(
      contextSource,
      "void context::draw2d_on_end_draw(::gpu::graphics *pgpugraphics)",
      "void context::copy(::gpu::command_buffer *");

   assert(endDraw.find("swap_buffers()") == std::string::npos);

   return 0;

}

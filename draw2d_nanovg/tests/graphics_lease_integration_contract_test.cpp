#include <cassert>
#include <fstream>
#include <iterator>
#include <string>


namespace
{


   std::string read_file(const char * pszPath)
   {

      std::ifstream stream(pszPath, std::ios::binary);

      return {
         std::istreambuf_iterator<char>(stream),
         std::istreambuf_iterator<char>()};

   }


   std::string section(
      const std::string & source,
      const std::string & beginMarker,
      const std::string & endMarker)
   {

      const auto begin = source.find(beginMarker);
      const auto end = source.find(endMarker, begin);

      assert(begin != std::string::npos);
      assert(end != std::string::npos);
      assert(begin < end);

      return source.substr(begin, end - begin);

   }


} // namespace


int main()
{

   const auto gpuGraphicsHeader = read_file("../app/bred/gpu/graphics.h");
   const auto nanovgHeader = read_file("draw2d_nanovg/graphics.h");
   const auto nanovgSource = read_file("draw2d_nanovg/graphics.cpp");

   const auto nanovgCreate = section(
      nanovgSource,
      "void graphics::_create_memory_graphics(",
      "void graphics::create_window_graphics(");
   const auto nanovgRelease = section(
      nanovgSource,
      "void graphics::on_release_memory_graphics()",
      "void graphics::create_window_graphics(");

   assert(gpuGraphicsHeader.find("::gpu::context_lease m_contextlease") != std::string::npos);
   assert(nanovgCreate.find("acquire_draw2d_context") != std::string::npos);
   assert(nanovgCreate.find("create_draw2d_context") == std::string::npos);
   assert(nanovgHeader.find("on_acquire_memory_graphics") != std::string::npos);
   assert(nanovgHeader.find("on_release_memory_graphics") != std::string::npos);
   assert(nanovgRelease.find("defer_fence") != std::string::npos);
   assert(nanovgRelease.find("m_pimage = nullptr") != std::string::npos);

   return 0;

}

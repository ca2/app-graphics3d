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


} // namespace


int main()
{

   const auto header = read_file("draw2d_nanovg/draw2d.h");
   const auto source = read_file("draw2d_nanovg/draw2d.cpp");
   const auto graphics = read_file("draw2d_nanovg/graphics.cpp");

   assert(header.find("bool m_bNanoVGGeometryAntialias") !=
      std::string::npos);
   assert(header.find("int nanovg_create_flags() const;") !=
      std::string::npos);
   assert(source.find(
      "m_bNanoVGGeometryAntialias =\r\n"
      "         !m_papplication->m_gpu.m_bMultisample;") !=
      std::string::npos);
   assert(graphics.find("int draw2d::nanovg_create_flags() const") !=
      std::string::npos);
   assert(graphics.find("NVG_STENCIL_STROKES | NVG_DEBUG") !=
      std::string::npos);
   assert(graphics.find("if (m_bNanoVGGeometryAntialias)") !=
      std::string::npos);
   assert(graphics.find("iFlags |= NVG_ANTIALIAS;") !=
      std::string::npos);
   assert(graphics.find(
      "nvgCreateGL3(::draw2d_nanovg::get()->nanovg_create_flags())") !=
      std::string::npos);
   assert(graphics.find(
      "nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG)") ==
      std::string::npos);

   return 0;

}

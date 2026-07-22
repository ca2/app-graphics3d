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

   const auto header = read_file("draw2d_nanovg/draw2d.h");
   const auto source = read_file("draw2d_nanovg/draw2d.cpp");
   const auto graphicsSource = read_file("draw2d_nanovg/graphics.cpp");
   const auto loadFont = section(
      source,
      "::string draw2d::defer_load_font(",
      "} // namespace draw2d_nanovg");

   assert(header.find("m_mapFont") == std::string::npos);
   assert(header.find("m_bLoaded") == std::string::npos);

   assert(header.find("defer_load_font(NVGcontext * pdc, ::write_text::font * pfont)") != std::string::npos);
   assert(loadFont.find("resolve_font_face") != std::string::npos);
   assert(loadFont.find("node()->get_font_path_from_name") == std::string::npos);

   const auto findFont = loadFont.find("nvgFindFont(pdc, strFontKey)");
   const auto createFont = loadFont.find("nvgCreateFontAtIndex(", findFont);
   const auto validateFont = loadFont.find("if (iFont < 0)", createFont);

   assert(findFont != std::string::npos);
   assert(createFont != std::string::npos);
   assert(validateFont != std::string::npos);
   assert(findFont < createFont);
   assert(createFont < validateFont);
   assert(loadFont.find("error_failed", validateFont) != std::string::npos);
   assert(loadFont.find("family=\\\"") != std::string::npos);
   assert(loadFont.find("resolved_family=\\\"") != std::string::npos);
   assert(loadFont.find("path=\\\"") != std::string::npos);
   assert(loadFont.find("face_index=") != std::string::npos);
   assert(loadFont.find("exists=") != std::string::npos);
   assert(loadFont.find("extension=\\\"") != std::string::npos);
   assert(loadFont.find("return strFontKey") != std::string::npos);

   assert(graphicsSource.find("nvgFontFace(m_pdc, strFontKey)") != std::string::npos);

   return 0;

}

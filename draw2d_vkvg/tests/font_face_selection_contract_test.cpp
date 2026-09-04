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

   const auto draw2dHeader = read_file("draw2d_vkvg/draw2d.h");
   const auto draw2dSource = read_file("draw2d_vkvg/draw2d.cpp");
   const auto graphicsSource = read_file("draw2d_vkvg/graphics.cpp");

   assert(draw2dHeader.find("defer_load_font(") != std::string::npos);
   assert(draw2dHeader.find("VkvgDevice pdevice") != std::string::npos);
   assert(draw2dSource.find("font_face_request request") != std::string::npos);
   assert(draw2dSource.find("request.m_fontweight = pwritetextfont->m_fontweight") != std::string::npos);
   assert(draw2dSource.find("request.m_bItalic = pwritetextfont->m_bItalic") != std::string::npos);
   assert(draw2dSource.find("resolve_font_face(source, request)") != std::string::npos);
   assert(draw2dSource.find("strDeviceFontKey.formatf(\"device=%p;%s\"") != std::string::npos);
   assert(draw2dSource.find("m_mapFont[strDeviceFontKey]") != std::string::npos);
   assert(draw2dSource.find("vkvg_load_font_from_path(pdc, source.m_path, strFontKey)") != std::string::npos);
   assert(graphicsSource.find("vkvg_select_font_face(vkvgcontext, strFontKey)") != std::string::npos);
   assert(graphicsSource.find("vkvg_font_extents(vkvgcontext, &fontextents)") != std::string::npos);
   assert(graphicsSource.find("GetTextMetrics(pwritetextfont->m_hdcFont, &tm)") == std::string::npos);

   return 0;

}

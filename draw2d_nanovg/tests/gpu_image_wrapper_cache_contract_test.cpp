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

   const auto header = read_file("draw2d_nanovg/graphics.h");
   const auto source = read_file("draw2d_nanovg/graphics.cpp");

   assert(header.find("struct nanovg_gpu_image_wrapper_cache_entry") !=
      std::string::npos);
   assert(header.find("m_iTextureSerial") != std::string::npos);
   assert(header.find("m_uOpenGlTexture") != std::string::npos);
   assert(header.find("m_size") != std::string::npos);
   assert(header.find("m_iNanovgImage") != std::string::npos);
   assert(header.find("::pointer < ::gpu::texture >") !=
      std::string::npos);
   assert(header.find("m_pgputexture") !=
      std::string::npos);
   assert(header.find("m_uLastUsedFrame") != std::string::npos);
   assert(header.find("m_nanovgGpuImageWrapperCache") != std::string::npos);
   assert(header.find("acquire_nanovg_gpu_image_wrapper(") !=
      std::string::npos);

   const auto acquire = section(
      source,
      "int graphics::acquire_nanovg_gpu_image_wrapper(",
      "bool graphics::_draw_gpu_image(");
   assert(acquire.find("m_iTextureSerial == pgputexture->m_iTextureSerial") !=
      std::string::npos);
   assert(acquire.find("m_uOpenGlTexture == pgputexture->m_gluTextureID") !=
      std::string::npos);
   assert(acquire.find("m_size.cx == sizeImage.cx") != std::string::npos);
   assert(acquire.find("m_size.cy == sizeImage.cy") != std::string::npos);
   assert(acquire.find("nvglCreateImageFromHandleGL3(") !=
      std::string::npos);
   assert(acquire.find("NVG_IMAGE_NODELETE") != std::string::npos);
   assert(acquire.find("entry.m_pgputexture = pgputexture;") !=
      std::string::npos);
   assert(acquire.find("m_nanovgGpuImageWrapperCache.push_back(") !=
      std::string::npos);

   const auto gpuPath = section(
      source,
      "bool graphics::_draw_gpu_image(",
      "void graphics::_draw_raw(");
   assert(gpuPath.find("acquire_nanovg_gpu_image_wrapper(") !=
      std::string::npos);
   assert(gpuPath.find("nvgDeleteImage(") == std::string::npos);
   assert(gpuPath.find("->map(") == std::string::npos);
   assert(gpuPath.find("read_pixels(") == std::string::npos);

   return 0;

}

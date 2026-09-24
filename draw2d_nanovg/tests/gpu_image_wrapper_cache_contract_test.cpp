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

   assert(header.find("s_uNanovgGpuImageWrapperStaleFrames = 120") !=
      std::string::npos);
   assert(header.find("s_zNanovgGpuImageWrapperPreferredMaximum = 512") !=
      std::string::npos);
   assert(header.find("m_uPerformanceWrapperCacheHits") !=
      std::string::npos);
   assert(header.find("m_uPerformanceWrapperCacheMisses") !=
      std::string::npos);
   assert(header.find("m_uPerformanceWrapperEvictions") !=
      std::string::npos);

   const auto maintain = section(
      source,
      "void graphics::maintain_nanovg_gpu_image_wrapper_cache()",
      "void graphics::clear_nanovg_gpu_image_wrapper_cache()");
   assert(maintain.find(
      "entry.m_uLastUsedFrame == m_uNanovgGpuImageWrapperFrameSerial") !=
      std::string::npos);
   assert(maintain.find("if (!bUsedThisFrame") != std::string::npos);
   assert(maintain.find("s_uNanovgGpuImageWrapperStaleFrames") !=
      std::string::npos);
   assert(maintain.find("s_zNanovgGpuImageWrapperPreferredMaximum") !=
      std::string::npos);
   assert(maintain.find("nvgDeleteImage(m_pdc, entry.m_iNanovgImage);") !=
      std::string::npos);

   const auto onEndLayer = section(
      source,
      "void graphics::on_end_layer(",
      "void graphics::start_layer(");
   const auto endFrame = onEndLayer.find("nvgEndFrame(m_pdc);");
   const auto maintainAfterFlush = onEndLayer.find(
      "maintain_nanovg_gpu_image_wrapper_cache();", endFrame);
   const auto advanceFrame = onEndLayer.find(
      "++m_uNanovgGpuImageWrapperFrameSerial;", maintainAfterFlush);
   assert(endFrame != std::string::npos);
   assert(maintainAfterFlush != std::string::npos);
   assert(advanceFrame != std::string::npos);
   assert(endFrame < maintainAfterFlush);
   assert(maintainAfterFlush < advanceFrame);

   const auto windowCreation = section(
      source,
      "void graphics::create_for_window_draw2d(",
      "void graphics::create_compatible_graphics(");
   const auto deleteWindowContext = windowCreation.find("nvgDeleteGL3(m_pdc);");
   const auto clearWindowCache = windowCreation.find(
      "clear_nanovg_gpu_image_wrapper_cache();", deleteWindowContext);
   assert(deleteWindowContext != std::string::npos);
   assert(clearWindowCache != std::string::npos);

   const auto deleteDc = section(
      source,
      "void graphics::DeleteDC()",
      "int graphics::save_graphics_context()");
   assert(deleteDc.find("clear_nanovg_gpu_image_wrapper_cache();") !=
      std::string::npos);

   assert(source.find("wrapper_cache_hits=") != std::string::npos);
   assert(source.find("wrapper_cache_misses=") != std::string::npos);
   assert(source.find("wrapper_evictions=") != std::string::npos);
   assert(source.find("wrapper_cached=") != std::string::npos);

   return 0;

}

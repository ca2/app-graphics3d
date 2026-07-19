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
   const auto textureHeader = read_file("../app/gpu_opengl/texture.h");
   const auto textureSource = read_file("../app/gpu_opengl/texture.cpp");

   assert(header.find("bool _draw_gpu_image(") != std::string::npos);
   assert(header.find("void _draw_nanovg_image(") != std::string::npos);

   const auto drawRaw = section(
      source,
      "void graphics::_draw_raw(",
      "void graphics::set_text_rendering_hint(");
   const auto update = drawRaw.find("pimage->defer_update_image();");
   const auto gpuDispatch = drawRaw.find("_draw_gpu_image(", update);
   const auto cpuMap = drawRaw.find("pimage->map();", gpuDispatch);
   const auto cpuUpload = drawRaw.find("nvgCreateImageRGBA(", cpuMap);
   assert(update != std::string::npos);
   assert(gpuDispatch != std::string::npos);
   assert(cpuMap != std::string::npos);
   assert(cpuUpload != std::string::npos);
   assert(update < gpuDispatch);
   assert(gpuDispatch < cpuMap);
   assert(cpuMap < cpuUpload);

   const auto gpuPath = section(
      source,
      "bool graphics::_draw_gpu_image(",
      "void graphics::_draw_raw(");
   assert(gpuPath.find("dynamic_cast < ::gpu::image * >") !=
      std::string::npos);
   assert(gpuPath.find("dynamic_cast < ::gpu_opengl::texture * >") !=
      std::string::npos);
   assert(gpuPath.find("wait_fence();") != std::string::npos);
   assert(gpuPath.find("nvglCreateImageFromHandleGL3(") !=
      std::string::npos);
   assert(gpuPath.find("NVG_IMAGE_NODELETE") != std::string::npos);
   assert(gpuPath.find("NVG_IMAGE_PREMULTIPLIED") != std::string::npos);
   assert(gpuPath.find("NVG_IMAGE_FLIPY") != std::string::npos);
   assert(gpuPath.find("nvgDeleteImage(m_pdc, iImage);") !=
      std::string::npos);
   assert(gpuPath.find("->map(") == std::string::npos);
   assert(textureHeader.find("bool has_pending_fence() const;") !=
      std::string::npos);
   assert(textureSource.find(
      "return m_glsyncGpuCommandsCompleteFence != nullptr;") !=
      std::string::npos);
   assert(gpuPath.find("has_pending_fence()") <
      gpuPath.find("wait_fence();"));
   assert(gpuPath.find("record_gpu_image_fast_path(") !=
      std::string::npos);
   assert(source.find("gpu.performance.nanovg_image") !=
      std::string::npos);
   assert(drawRaw.find("record_gpu_image_cpu_fallback();") < cpuMap);

   return 0;

}

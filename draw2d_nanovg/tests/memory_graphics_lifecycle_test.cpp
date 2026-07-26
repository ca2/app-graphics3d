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

   assert(header.find(
      "void _create_memory_graphics(const ::i32_size & size) override;") !=
      std::string::npos);

   const auto publicCreation = section(
      source,
      "void graphics::create_memory_graphics(",
      "void graphics::_create_memory_graphics(");

   const auto fallback = publicCreation.find("sizeMemory = { 1920, 1080 };");
   const auto delegation = publicCreation.find(
      "::gpu::graphics::create_memory_graphics(sizeMemory);");

   assert(fallback != std::string::npos);
   assert(delegation != std::string::npos);
   assert(fallback < delegation);
   assert(publicCreation.find("opengl_create_offscreen_buffer") ==
      std::string::npos);

   const auto backendCreation = section(
      source,
      "void graphics::_create_memory_graphics(",
      "void graphics::create_window_graphics(");

   const auto deleteOldNanoVg = backendCreation.find("nvgDeleteGL3(pdcOld);");
   const auto acquireGpuContext = backendCreation.find("acquire_gpu_context(");
   const auto createGpuContext = backendCreation.find("create_draw2d_context(");
   const auto gpuBufferOutput = backendCreation.find(
      "::gpu::e_output_gpu_buffer", acquireGpuContext);
   const auto assignContext = backendCreation.find(
      "set_context_lease(::transfer(contextlease));", acquireGpuContext);
   const auto assignCompositor = backendCreation.find(
      "pgpucontextNew->m_pgpucompositor = this;", assignContext);
   const auto ensureRenderer = backendCreation.find(
      "pgpucontextNew->get_gpu_renderer();", assignCompositor);
   const auto createNanoVg = backendCreation.find(
      "nvgCreateGL3(::draw2d_nanovg::get()->nanovg_create_flags())",
      ensureRenderer);

   assert(deleteOldNanoVg == std::string::npos);
   assert(acquireGpuContext != std::string::npos);
   assert(createGpuContext == std::string::npos);
   assert(gpuBufferOutput != std::string::npos);
   assert(assignContext != std::string::npos);
   assert(assignCompositor != std::string::npos);
   assert(ensureRenderer != std::string::npos);
   assert(createNanoVg != std::string::npos);
   assert(acquireGpuContext < gpuBufferOutput);
   assert(gpuBufferOutput < assignContext);
   assert(assignContext < assignCompositor);
   assert(assignCompositor < ensureRenderer);
   assert(ensureRenderer < createNanoVg);
   assert(backendCreation.find("if (!m_pdc)", ensureRenderer) < createNanoVg);
   assert(backendCreation.find("m_sizeScaleOutput = { 1.0, -1.0 };") !=
      std::string::npos);
   assert(backendCreation.find(
      "m_pointTranslateOutput = { 0.0, (double)size.cy };") !=
      std::string::npos);
   assert(backendCreation.find("opengl_create_offscreen_buffer") ==
      std::string::npos);

   const auto bitmapSelection = section(
      source,
      "::draw2d::bitmap* graphics::SelectObject(::draw2d::bitmap* pbitmap)",
      "::draw2d::object* graphics::SelectObject(::draw2d::object* pObject)");

   assert(bitmapSelection.find("create_memory_graphics(pbitmap->get_size());") !=
      std::string::npos);
   assert(bitmapSelection.find("opengl_create_offscreen_buffer") ==
      std::string::npos);
   assert(bitmapSelection.find("opengl_delete_offscreen_buffer") ==
      std::string::npos);

   return 0;

}

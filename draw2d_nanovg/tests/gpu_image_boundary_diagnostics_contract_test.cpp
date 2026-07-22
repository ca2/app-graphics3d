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

   assert(header.find("m_uPerformanceRenderedTextureDiagnostics") !=
      std::string::npos);
   assert(header.find("m_uPerformanceSampledTextureDiagnostics") !=
      std::string::npos);
   assert(header.find("diagnose_rendered_gpu_image(") !=
      std::string::npos);
   assert(header.find("diagnose_sampled_gpu_image(") !=
      std::string::npos);

   assert(source.find("m_bPerformanceDiagnostics.load(") !=
      std::string::npos);
   assert(source.find("uDiagnosticIndex >= 8") !=
      std::string::npos);
   assert(source.find("pgputexture->read_pixels(&pixmap);") !=
      std::string::npos);
   assert(source.find("[gpu.performance.nanovg_image_boundary] stage=render") !=
      std::string::npos);
   assert(source.find("[gpu.performance.nanovg_image_boundary] stage=sample") !=
      std::string::npos);

   const auto onEndLayer = section(
      source,
      "void graphics::on_end_layer(",
      "void graphics::start_layer(");
   const auto bindTarget = onEndLayer.find(
      "pgputextureTarget->bind_render_target();");
   const auto endFrame = onEndLayer.find(
      "nvgEndFrame(m_pdc);", bindTarget);
   const auto gpuImageGuard = onEndLayer.find(
      "dynamic_cast < ::gpu::image * >(m_pimage)", endFrame);
   const auto diagnoseRender = onEndLayer.find(
      "diagnose_rendered_gpu_image(", gpuImageGuard);
   assert(bindTarget != std::string::npos);
   assert(endFrame != std::string::npos);
   assert(gpuImageGuard != std::string::npos);
   assert(diagnoseRender != std::string::npos);
   assert(bindTarget < endFrame);
   assert(endFrame < gpuImageGuard);
   assert(gpuImageGuard < diagnoseRender);

   const auto publicEndLayer = section(
      source,
      "void graphics::end_layer(bool bClosingLayer)",
      "void graphics::on_present()");
   assert(publicEndLayer.find("nvgEndFrame(m_pdc);") == std::string::npos);
   assert(publicEndLayer.find("diagnose_rendered_gpu_image(") ==
      std::string::npos);
   assert(publicEndLayer.find(
      "::gpu::graphics::end_layer(bClosingLayer);") != std::string::npos);

   const auto gpuPath = source.find("bool graphics::_draw_gpu_image(");
   const auto diagnoseSample = source.find(
      "diagnose_sampled_gpu_image(", gpuPath);
   const auto wrapper = source.find(
      "nvglCreateImageFromHandleGL3(", diagnoseSample);
   assert(gpuPath != std::string::npos);
   assert(diagnoseSample != std::string::npos);
   assert(wrapper != std::string::npos);
   assert(gpuPath < diagnoseSample);
   assert(diagnoseSample < wrapper);

   return 0;

}

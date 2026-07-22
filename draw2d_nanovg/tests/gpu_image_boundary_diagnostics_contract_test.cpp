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

   assert(header.find("reserve_rendered_gpu_image_diagnostic()") !=
      std::string::npos);
   assert(header.find("diagnose_gpu_image_target_state(") !=
      std::string::npos);
   const auto renderedDiagnosticDeclaration = header.find(
      "void diagnose_rendered_gpu_image(");
   assert(renderedDiagnosticDeclaration != std::string::npos);
   assert(header.find("::i64 iDiagnosticIndex", renderedDiagnosticDeclaration) !=
      std::string::npos);

   assert(source.find(
      "[gpu.performance.nanovg_image_boundary] stage=target_state") !=
      std::string::npos);
   assert(source.find("wglGetCurrentContext()") != std::string::npos);
   assert(source.find("wglGetCurrentDC()") != std::string::npos);
   assert(source.find("GL_DRAW_FRAMEBUFFER_BINDING") != std::string::npos);
   assert(source.find("GL_VIEWPORT") != std::string::npos);
   assert(source.find("context_match=") != std::string::npos);
   assert(source.find("framebuffer_match=") != std::string::npos);

   const auto onEndLayer = section(
      source,
      "void graphics::on_end_layer(",
      "void graphics::start_layer(");
   const auto bindTarget = onEndLayer.find(
      "pgputextureTarget->bind_render_target();");
   const auto endFrame = onEndLayer.find(
      "nvgEndFrame(m_pdc);", bindTarget);
   const auto gpuImageGuard = onEndLayer.find(
      "dynamic_cast < ::gpu::image * >(m_pimage)");
   const auto diagnoseRender = onEndLayer.find(
      "diagnose_rendered_gpu_image(", endFrame);
   assert(bindTarget != std::string::npos);
   assert(endFrame != std::string::npos);
   assert(gpuImageGuard != std::string::npos);
   assert(diagnoseRender != std::string::npos);
   assert(gpuImageGuard < bindTarget);
   assert(bindTarget < endFrame);
   assert(endFrame < diagnoseRender);

   const auto reserveDiagnostic = onEndLayer.find(
      "reserve_rendered_gpu_image_diagnostic()");
   const auto readFramebufferBefore = onEndLayer.find(
      "GL_DRAW_FRAMEBUFFER_BINDING", reserveDiagnostic);
   const auto diagnoseTargetState = onEndLayer.find(
      "diagnose_gpu_image_target_state(", bindTarget);
   const auto endFrameWithDiagnostic = onEndLayer.find(
      "nvgEndFrame(m_pdc);", diagnoseTargetState);
   const auto diagnoseRenderWithIndex = onEndLayer.find(
      "diagnose_rendered_gpu_image(", endFrameWithDiagnostic);
   const auto diagnoseRenderIndex = onEndLayer.find(
      "iDiagnosticIndex", diagnoseRenderWithIndex);
   assert(reserveDiagnostic != std::string::npos);
   assert(readFramebufferBefore != std::string::npos);
   assert(diagnoseTargetState != std::string::npos);
   assert(endFrameWithDiagnostic != std::string::npos);
   assert(diagnoseRenderWithIndex != std::string::npos);
   assert(diagnoseRenderIndex != std::string::npos);
   assert(reserveDiagnostic < readFramebufferBefore);
   assert(readFramebufferBefore < bindTarget);
   assert(bindTarget < diagnoseTargetState);
   assert(diagnoseTargetState < endFrameWithDiagnostic);
   assert(endFrameWithDiagnostic < diagnoseRenderWithIndex);
   assert(diagnoseRenderWithIndex < diagnoseRenderIndex);

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

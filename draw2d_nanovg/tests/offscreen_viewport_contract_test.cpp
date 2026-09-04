#include <cassert>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>


namespace
{


   std::string read(const std::filesystem::path & path)
   {

      std::ifstream stream(path);
      assert(stream);
      return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};

   }


} // namespace


int main()
{

   const auto draw2dNanovg = std::filesystem::path(__FILE__).parent_path().parent_path();
   const auto source = read(draw2dNanovg / "graphics.cpp");

   const auto beginDrawBegin = source.find(
      "void graphics::begin_draw(bool bExternalRendering");
   const auto beginDrawEnd = source.find("void graphics::end_draw()", beginDrawBegin);
   const auto beginFrameBegin = source.find("void graphics::_nvg_begin_frame(");
   const auto endFrameBegin = source.find(
      "void graphics::_nvg_end_frame(bool bExternalRendering)");
   const auto endFrameEnd = source.find(
      "::gpu::texture_site* graphics::current_target_texture", endFrameBegin);

   assert(beginDrawBegin != std::string::npos);
   assert(beginDrawEnd != std::string::npos);
   assert(beginFrameBegin != std::string::npos);
   assert(endFrameBegin != std::string::npos);
   assert(endFrameEnd != std::string::npos);

   const auto beginDraw = source.substr(beginDrawBegin, beginDrawEnd - beginDrawBegin);
   const auto beginFrame = source.substr(beginFrameBegin, endFrameBegin - beginFrameBegin);
   const auto endFrame = source.substr(endFrameBegin, endFrameEnd - endFrameBegin);

   assert(beginDraw.find("auto sizeTarget = pgputextureTarget->size();") !=
      std::string::npos);
   assert(beginDraw.find(
      "_nvg_begin_frame(bExternalRendering, pgputextureTarget, sizeTarget);") !=
      std::string::npos);
   assert(source.find(
      "_nvg_end_frame(m_bNvgBeginFrameExternalRendering);") !=
      std::string::npos);
   assert(beginFrame.find("if (bExternalRendering)") != std::string::npos);
   assert(beginFrame.find(
      "glViewport(0, 0, m_sizeNvgBeginFrame.cx, m_sizeNvgBeginFrame.cy);") !=
      std::string::npos);
   assert(endFrame.find(
      "glViewport(0, 0, m_sizeNvgBeginFrame.cx, m_sizeNvgBeginFrame.cy);") !=
      std::string::npos);
   assert(beginFrame.find("getCurrentCommandBuffer2") != std::string::npos);
   assert(endFrame.find("getCurrentCommandBuffer2") != std::string::npos);
   assert(endFrame.find("pgpucontext->input_placement()") == std::string::npos);
   assert(endFrame.find("pgpucontext->m_sizeRaw") == std::string::npos);

   return 0;

}

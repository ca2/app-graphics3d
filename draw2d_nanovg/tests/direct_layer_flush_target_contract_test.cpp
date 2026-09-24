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
   const auto targetBegin = source.find(
      "::gpu::texture_site* graphics::current_target_texture(::gpu::layer * pgpulayer)");
   const auto targetEnd = source.find("bool graphics::is_gpu_oriented()", targetBegin);
   const auto setTargetBegin = source.find(
      "void graphics::set_target_image(::image::image * pimage)");
   const auto setTargetEnd = source.find(
      "::draw2d::pen* graphics::get_current_pen()", setTargetBegin);
   const auto begin = source.find("void graphics::on_end_layer(::gpu::layer* pgpulayer)");
   const auto end = source.find("void graphics::begin_draw()", begin);

   assert(targetBegin != std::string::npos);
   assert(targetEnd != std::string::npos);
   assert(setTargetBegin != std::string::npos);
   assert(setTargetEnd != std::string::npos);
   assert(begin != std::string::npos);
   assert(end != std::string::npos);

   const auto currentTarget = source.substr(targetBegin, targetEnd - targetBegin);
   const auto setTarget = source.substr(setTargetBegin, setTargetEnd - setTargetBegin);
   const auto onEndLayer = source.substr(begin, end - begin);

   const auto activeLayer = currentTarget.find(
      "if (pgpulayer && pgpulayer->m_bIncludeInFrameComposition)");
   const auto activeLayerTexture = currentTarget.find(
      "pgpucontext->get_gpu_renderer()->current_render_target_texture(pgpulayer)",
      activeLayer);
   const auto memoryImage = currentTarget.find("::cast < ::gpu::image > pgpuimage =m_pimage;");

   assert(activeLayer != std::string::npos);
   assert(activeLayerTexture != std::string::npos);
   assert(memoryImage != std::string::npos);
   assert(activeLayer < activeLayerTexture);
   assert(activeLayerTexture < memoryImage);

   const auto compositionLayerGuard = setTarget.find(
      "if (pgpulayer && pgpulayer->m_bIncludeInFrameComposition)");
   const auto skipImageTarget = setTarget.find("return;", compositionLayerGuard);
   const auto imageTarget = setTarget.find(
      "::cast < ::draw2d_nanovg::image > popenglimage = pimage;");

   assert(compositionLayerGuard != std::string::npos);
   assert(skipImageTarget != std::string::npos);
   assert(imageTarget != std::string::npos);
   assert(compositionLayerGuard < skipImageTarget);
   assert(skipImageTarget < imageTarget);

   assert(onEndLayer.find(
      "if (!pgputexturesiteTarget || !pgputexturesiteTarget->gpu_texture())") !=
      std::string::npos);
   assert(onEndLayer.find(
      "auto pgputextureTarget = pgputexturesiteTarget->gpu_texture();") !=
      std::string::npos);
   assert(onEndLayer.find(
      "auto pgputextureTarget = m_pgputexturesiteTarget->gpu_texture();") ==
      std::string::npos);

   return 0;

}

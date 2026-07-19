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

   const auto header = read_file("draw2d_nanovg/image.h");
   const auto imageSource = read_file("draw2d_nanovg/image.cpp");
   const auto graphicsSource = read_file("draw2d_nanovg/graphics.cpp");
   const auto gpuImageSource = read_file("../app/bred/gpu/image.cpp");

   assert(header.find("virtual public ::gpu::image") != std::string::npos);
   assert(header.find("void map(") == std::string::npos);
   assert(header.find("void unmap(") == std::string::npos);

   const auto create = section(
      imageSource,
      "void image::create(const ::i32_size& size",
      "bool image::host(");
   const auto createGraphics = create.find("create_memory_graphics(size);");
   const auto initializeGpuImage = create.find("initialize_gpu_image(");
   assert(createGraphics != std::string::npos);
   assert(initializeGpuImage != std::string::npos);
   assert(createGraphics < initializeGpuImage);
   assert(create.find("create_bitmap") == std::string::npos);
   assert(create.find("::pixmap::initialize") == std::string::npos);

   const auto getGraphics = section(
      imageSource,
      "::draw2d::graphics * image::_get_graphics() const",
      "double image::pi()");
   assert(getGraphics.find("return m_pgraphics;") != std::string::npos);
   assert(getGraphics.find("m_pbitmap") == std::string::npos);

   const auto getGpuGraphics = section(
      gpuImageSource,
      "::draw2d::graphics * image::get_graphics() const",
      "::gpu::texture * image::gpu_texture() const");
   const auto unmapBeforeGraphics = getGpuGraphics.find("unmap();");
   const auto returnGpuGraphics = getGpuGraphics.find("return _get_graphics();");
   assert(unmapBeforeGraphics != std::string::npos);
   assert(returnGpuGraphics != std::string::npos);
   assert(unmapBeforeGraphics < returnGpuGraphics);

   const auto destroy = section(
      imageSource,
      "void image::destroy()",
      "bool image::from(");
   assert(destroy.find("::gpu::image::destroy();") != std::string::npos);

   const auto target = section(
      graphicsSource,
      "::gpu::texture* graphics::current_target_texture(",
      "bool graphics::is_gpu_oriented()");
   const auto imageCast = target.find("dynamic_cast < ::gpu::image * >");
   const auto imageTexture = target.find("gpu_texture()", imageCast);
   const auto fallback = target.find(
      "::gpu::graphics::current_target_texture(pgpulayer)", imageTexture);
   assert(imageCast != std::string::npos);
   assert(imageTexture != std::string::npos);
   assert(fallback != std::string::npos);
   assert(imageCast < imageTexture);
   assert(imageTexture < fallback);

   const auto endLayer = section(
      graphicsSource,
      "void graphics::on_end_layer(",
      "void graphics::start_layer(");
   assert(endLayer.find("gpu_texture()") != std::string::npos);
   assert(endLayer.find("defer_fence();") != std::string::npos);

   return 0;

}

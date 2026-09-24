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

   const auto header = read_file("draw2d_nanovg/draw2d.h");
   const auto source = read_file("draw2d_nanovg/draw2d.cpp");
   const auto graphicsHeader = read_file("draw2d_nanovg/graphics.h");
   const auto graphics = read_file("draw2d_nanovg/graphics.cpp");

   assert(header.find("bool m_bNanoVGGeometryAntialias") !=
      std::string::npos);
   assert(header.find("int nanovg_create_flags() const;") !=
      std::string::npos);
   assert(source.find(
      "m_bNanoVGGeometryAntialias =\r\n"
      "         !m_papplication->m_gpu.m_bMultisample;") !=
      std::string::npos);
   assert(graphics.find("int draw2d::nanovg_create_flags() const") !=
      std::string::npos);
   assert(graphics.find("NVG_STENCIL_STROKES | NVG_DEBUG") !=
      std::string::npos);
   assert(graphics.find("if (m_bNanoVGGeometryAntialias)") !=
      std::string::npos);
   assert(graphics.find("iFlags |= NVG_ANTIALIAS;") !=
      std::string::npos);
   assert(graphics.find(
      "nvgCreateGL3(::draw2d_nanovg::get()->nanovg_create_flags())") !=
      std::string::npos);
   assert(graphics.find(
      "nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG)") ==
      std::string::npos);
   assert(graphicsHeader.find(
      "void prepare_nanovg_render_target(::gpu::texture * pgputexture);") !=
      std::string::npos);
   assert(graphics.find("create_depth_resources();") !=
      std::string::npos);
   assert(graphics.find("glClearStencil(0);") != std::string::npos);
   assert(graphics.find("glClear(GL_STENCIL_BUFFER_BIT);") !=
      std::string::npos);

   const auto prepareTarget = section(
      graphics,
      "void graphics::prepare_nanovg_render_target(",
      "void graphics::on_start_layer(");
   assert(prepareTarget.find("frame_buffer_object()") !=
      std::string::npos);
   assert(prepareTarget.find("glViewport(") != std::string::npos);
   assert(prepareTarget.find("GL_STENCIL_WRITEMASK") !=
      std::string::npos);
   assert(prepareTarget.find("GL_STENCIL_CLEAR_VALUE") !=
      std::string::npos);
   assert(prepareTarget.find("GL_SCISSOR_TEST") != std::string::npos);
   assert(prepareTarget.find("glStencilMask(0xffffffffu);") !=
      std::string::npos);
   assert(prepareTarget.find("glDisable(GL_SCISSOR_TEST);") !=
      std::string::npos);
   assert(prepareTarget.find("glClearStencil(iStencilClearValue);") !=
      std::string::npos);
   assert(prepareTarget.find("GL_COLOR_BUFFER_BIT") == std::string::npos);
   assert(prepareTarget.find("GL_DEPTH_BUFFER_BIT") == std::string::npos);

   const auto onStartLayer = section(
      graphics,
      "void graphics::on_start_layer(",
      "void graphics::on_end_layer(");
   const auto beginDraw = section(
      graphics,
      "void graphics::begin_draw()",
      "void graphics::end_draw()");
   const auto startLayer = section(
      graphics,
      "void graphics::start_layer(bool bFirstLayer)",
      "//void graphics::start_layer(::e_graphics");
   for (const auto * path : {&onStartLayer, &beginDraw, &startLayer})
   {

      const auto prepare = path->find("prepare_nanovg_render_target(");
      const auto begin = path->find("nvgBeginFrame(");

      assert(prepare != std::string::npos);
      assert(begin != std::string::npos);
      assert(prepare < begin);

   }

   assert(beginDraw.find(
      "auto pgputextureTarget = current_target_texture(") !=
      std::string::npos);
   assert(beginDraw.find("if (m_pimage)") == std::string::npos);

   return 0;

}

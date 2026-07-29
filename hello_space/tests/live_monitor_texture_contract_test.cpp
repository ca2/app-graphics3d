#include <cassert>
#include <fstream>
#include <iterator>
#include <string>


namespace
{


   std::string read_file(const char *pszPath)
   {

      std::ifstream stream(pszPath, std::ios::binary);

      assert(stream);

      return {
         std::istreambuf_iterator<char>(stream),
         std::istreambuf_iterator<char>()};

   }


   std::string section(
      const std::string &source,
      const std::string &beginMarker,
      const std::string &endMarker)
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

   const auto sceneSource = read_file("hello_space/main_scene.cpp");
   const auto renderSystemSource = read_file(
      "../app/bred/graphics3d/render_system/texture_render_system.cpp");
   const auto overlayFragmentSource =
      read_file("hello_space/opengl/overlay1.frag");
   const auto overlayFragmentHeaderSource =
      read_file("hello_space/opengl/overlay1.frag.h");

   const auto beforeRender = section(
      sceneSource,
      "void main_scene::on_before_render(",
      "void main_scene::on_render(");

   const auto resolve = beforeRender.find(
      "pgpucontext->copy(m_pgputextureMonitor2, "
      "m_pgputextureMonitorMultisample, nullptr);");
   const auto beginMonitorRender = beforeRender.find(
      "pgpucommandbuffer->begin_render(m_pgpushaderBlend, "
      "m_pgputextureMonitorMultisample);");
   const auto monitorRectangle = beforeRender.find(
      "auto rectangleMonitor = "
      "m_pgputextureMonitorMultisample->rectangle();",
      beginMonitorRender);
   const auto setViewport = beforeRender.find(
      "pgpucommandbuffer->set_viewport(rectangleMonitor);",
      monitorRectangle);
   const auto setScissor = beforeRender.find(
      "pgpucommandbuffer->set_scissor(rectangleMonitor);",
      setViewport);
   const auto bindBackgroundTexture = beforeRender.find(
      "m_pgputextureHelloMultiverseScreen->binding_slot_set(",
      setScissor);
   const auto drawMonitor = beforeRender.find(
      "pgpucommandbuffer->draw_vertexes(3);",
      bindBackgroundTexture);
   const auto sceneRenderableGuard =
      beforeRender.find("if (m_prenderable)", resolve);
   const auto renderableLookup = beforeRender.find(
      "auto prenderableMonitor = m_prenderable->renderable();",
      sceneRenderableGuard);
   const auto renderableGuard =
      beforeRender.find("if (prenderableMonitor)", renderableLookup);
   const auto publish = beforeRender.find(
      "prenderableMonitor->m_ptextureTexture = "
      "m_pgputextureMonitor2;",
      renderableGuard);

   assert(resolve != std::string::npos);
   assert(beginMonitorRender != std::string::npos);
   assert(monitorRectangle != std::string::npos);
   assert(setViewport != std::string::npos);
   assert(setScissor != std::string::npos);
   assert(bindBackgroundTexture != std::string::npos);
   assert(drawMonitor != std::string::npos);
   assert(beginMonitorRender < monitorRectangle);
   assert(monitorRectangle < setViewport);
   assert(setViewport < setScissor);
   assert(setScissor < bindBackgroundTexture);
   assert(bindBackgroundTexture < drawMonitor);
   assert(sceneRenderableGuard != std::string::npos);
   assert(renderableLookup != std::string::npos);
   assert(renderableGuard != std::string::npos);
   assert(publish != std::string::npos);
   assert(resolve < sceneRenderableGuard);
   assert(sceneRenderableGuard < renderableLookup);
   assert(renderableLookup < renderableGuard);
   assert(renderableGuard < publish);
   assert(beforeRender.find(
      "m_ptextureTexture = m_pgputextureMonitorMultisample") ==
      std::string::npos);

   assert(renderSystemSource.find(
      "prenderable->m_ptextureTexture->binding_slot_set(") !=
      std::string::npos);

   for (const auto *pszOverlaySource : {
      overlayFragmentSource.c_str(),
      overlayFragmentHeaderSource.c_str()})
   {

      const std::string overlaySource(pszOverlaySource);
      const std::string backgroundBlock =
         "vec2 backgroundUv = vec2(\r\n"
         "        viewportUv.x,\r\n"
         "        1.0 - viewportUv.y);\r\n"
         "\r\n"
         "    vec4 backgroundColor =\r\n"
         "        texture(backgroundTexture, backgroundUv);";
      const std::string overlayPlacementDeclaration =
         "vec2 overlayPlacementUv = viewportUv;";
      const std::string insideOverlayBlock =
         "bool insideOverlay =\r\n"
         "        overlayPlacementUv.x >= overlayTopLeft.x &&\r\n"
         "        overlayPlacementUv.y >= overlayTopLeft.y &&\r\n"
         "        overlayPlacementUv.x <= overlayBottomRight.x &&\r\n"
         "        overlayPlacementUv.y <= overlayBottomRight.y;";
      const std::string overlayLocalBlock =
         "vec2 overlayLocalUv =\r\n"
         "        (overlayPlacementUv - overlayTopLeft) /\r\n"
         "        rectangleSize;";
      const std::string overlaySampleBlock =
         "vec2 overlayUv = vec2(\r\n"
         "        overlayLocalUv.x,\r\n"
         "        1.0 - overlayLocalUv.y);\r\n"
         "\r\n"
         "    vec4 overlayColor =\r\n"
         "        texture(overlayTexture, overlayUv);";

      assert(overlaySource.find(backgroundBlock) != std::string::npos);
      assert(overlaySource.find(overlayPlacementDeclaration) !=
         std::string::npos);
      assert(overlaySource.find(insideOverlayBlock) != std::string::npos);
      assert(overlaySource.find(overlayLocalBlock) != std::string::npos);
      assert(overlaySource.find(overlaySampleBlock) != std::string::npos);
      assert(overlaySource.find("viewportTopLeftUv") == std::string::npos);
      assert(overlaySource.find("overlayTopLeftUv") == std::string::npos);
      assert(overlaySource.find("texture(backgroundTexture, viewportUv)") ==
         std::string::npos);

   }

   return 0;

}

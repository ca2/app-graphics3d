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

   const auto sceneSource = read_file("continuum/main_scene.cpp");
   const auto renderSystemSource = read_file(
      "../app/bred/graphics3d/render_system/texture_render_system.cpp");

   const auto beforeRender = section(
      sceneSource,
      "void main_scene::on_before_render(",
      "void main_scene::on_render(");

   const auto resolve = beforeRender.find(
      "pgpucontext->copy(m_pgputextureMonitor2, "
      "m_pgputextureMonitorMultisample, nullptr);");
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

   return 0;

}

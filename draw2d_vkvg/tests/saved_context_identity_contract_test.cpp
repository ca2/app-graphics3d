#include <cassert>
#include <fstream>
#include <sstream>
#include <string>


static std::string read_file(const char * path)
{

   std::ifstream stream(path, std::ios::binary);
   assert(stream);
   std::ostringstream output;
   output << stream.rdbuf();
   return output.str();

}


static std::string section(
   const std::string & source,
   const std::string & beginMarker,
   const std::string & endMarker)
{

   const auto begin = source.find(beginMarker);
   assert(begin != std::string::npos);
   const auto end = source.find(endMarker, begin + beginMarker.size());
   assert(end != std::string::npos);
   return source.substr(begin, end - begin);

}


int main()
{

   const auto header = read_file(
      "source/app-graphics3d/draw2d_vkvg/graphics.h");
   const auto source = read_file(
      "source/app-graphics3d/draw2d_vkvg/graphics.cpp");
   const auto save = section(
      source,
      "int graphics::save_graphics_context()",
      "void graphics::restore_graphics_context(");
   const auto restore = section(
      source,
      "void graphics::restore_graphics_context(",
      "::draw2d_vkvg::draw2d* graphics::draw2d_vkvg()");
   const auto prepare = section(
      source,
      "void graphics::prepare_vkvg_render_target(",
      "void graphics::maintain_vkvg_direct_target_cache()");
   const auto maintain = section(
      source,
      "void graphics::maintain_vkvg_direct_target_cache()",
      "void graphics::destroy_vkvg_direct_target(");

   assert(header.find("class saved_vkvg_context") != std::string::npos);
   assert(header.find("::pointer<direct_target>              m_pdirecttarget;") !=
      std::string::npos);
   assert(header.find("::array<saved_vkvg_context>            m_savedvkvgcontexta;") !=
      std::string::npos);
   assert(header.find("bool is_vkvg_direct_target_saved(direct_target * pdirecttarget);") !=
      std::string::npos);
   assert(save.find("m_savedvkvgcontexta.get_size()") != std::string::npos);
   assert(save.find("auto vkvgcontext = vkvg_context();") != std::string::npos);
   assert(save.find("savedcontext.m_pdirecttarget = m_pdirecttargetActive;") !=
      std::string::npos);
   assert(save.find("vkvg_save(vkvgcontext);") != std::string::npos);
   assert(save.find("m_savedvkvgcontexta.add(savedcontext);") != std::string::npos);
   assert(restore.find("m_savedvkvgcontexta.get_upper_bound()") != std::string::npos);
   assert(restore.find("auto vkvgcontext = m_savedvkvgcontexta[iContext].m_vkvgcontext;") !=
      std::string::npos);
   assert(restore.find("vkvg_restore(vkvgcontext);") != std::string::npos);
   assert(restore.find("m_savedvkvgcontexta.set_size(iSavedContext);") !=
      std::string::npos);
   assert(restore.find("vkvg_context()") == std::string::npos);
   assert(prepare.find("is_vkvg_direct_target_saved(pdirecttargetCandidate)") !=
      std::string::npos);
   assert(maintain.find("is_vkvg_direct_target_saved(pdirecttarget)") !=
      std::string::npos);
   assert(source.find("clear_saved_vkvg_contexts();") != std::string::npos);

   return 0;

}

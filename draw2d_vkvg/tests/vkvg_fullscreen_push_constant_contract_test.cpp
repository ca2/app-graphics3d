#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>


namespace
{


   std::string read_text(const std::filesystem::path &path)
   {

      std::ifstream stream(path, std::ios::binary);

      if (!stream)
      {

         return {};

      }

      return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};

   }


   bool materialize_vkvg(
      const std::filesystem::path &wrapper,
      const std::filesystem::path &output)
   {

      const auto materializer = wrapper / "patches/materialize_vkvg_sources.cmake";
      const auto command =
         "cmake -DVKVG_SOURCE_DIR=\"" + (wrapper / "vkvg").generic_string() +
         "\" -DVKVG_OUTPUT_DIR=\"" + output.generic_string() +
         "\" -DVKVG_PATCH_DIR=\"" + (wrapper / "patches").generic_string() +
         "\" -P \"" + materializer.generic_string() + "\"";

      return std::system(command.c_str()) == 0;

   }


} // namespace


int main()
{

   const auto repositoryRoot =
      std::filesystem::path(__FILE__).parent_path().parent_path().parent_path().parent_path().parent_path();
   const auto wrapper = repositoryRoot / "port/graphics3d/vkvg";
   const auto pristinePath = wrapper / "vkvg/src/vkvg_context_internal.c";
   const auto patchPath = wrapper / "patches/0001-clear-fullscreen-bit-before-indexed-draw.patch";
   const auto pristineText = read_text(pristinePath);
   const auto patchText = read_text(patchPath);
   const auto clearStatement = "ctx->pushConsts.fsq_patternType &= ~FULLSCREEN_BIT;";

   if (pristineText.empty() || patchText.empty())
   {

      std::cerr << "Unable to read the pristine VKVG source and wrapper patch.\n";
      return 1;

   }

   const auto pristineFunctionBegin =
      pristineText.find("void _emit_draw_cmd_undrawn_vertices(VkvgContext ctx)");
   const auto pristineFunctionEnd = pristineText.find("void _flush_cmd_until_vx_base", pristineFunctionBegin);

   if (pristineFunctionBegin == std::string::npos || pristineFunctionEnd == std::string::npos)
   {

      std::cerr << "Unable to locate VKVG's pristine indexed-draw emission function.\n";
      return 1;

   }

   const auto pristineFunction =
      pristineText.substr(pristineFunctionBegin, pristineFunctionEnd - pristineFunctionBegin);

   if (pristineFunction.find(clearStatement) != std::string::npos)
   {

      std::cerr << "The vendored VKVG source must remain pristine; the fullscreen fix belongs to the wrapper.\n";
      return 1;

   }

   if (patchText.find(clearStatement) == std::string::npos)
   {

      std::cerr << "The wrapper fullscreen patch does not contain the required state reset.\n";
      return 1;

   }

   const auto nonce = std::chrono::high_resolution_clock::now().time_since_epoch().count();
   const auto output = std::filesystem::temp_directory_path() /
      ("vkvg-fullscreen-contract-" + std::to_string(nonce));

   if (!materialize_vkvg(wrapper, output))
   {

      std::cerr << "Unable to materialize the wrapper-owned VKVG patches.\n";
      return 1;

   }

   const auto text = read_text(output / "src/vkvg_context_internal.c");
   std::error_code removeError;
   std::filesystem::remove_all(output, removeError);
   const auto functionBegin = text.find("void _emit_draw_cmd_undrawn_vertices(VkvgContext ctx)");
   const auto functionEnd = text.find("void _flush_cmd_until_vx_base", functionBegin);

   if (functionBegin == std::string::npos || functionEnd == std::string::npos)
   {

      std::cerr << "Unable to locate VKVG's staged indexed-draw emission function.\n";
      return 1;

   }

   const auto function = text.substr(functionBegin, functionEnd - functionBegin);
   const auto clearFullscreenBit = function.find(clearStatement);
   const auto pushOrdinaryGeometryState = function.find(
      "CmdPushConstants(ctx->cmd, ctx->dev->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 24, 4,",
      clearFullscreenBit);
   const auto indexedDraw = function.find("CmdDrawIndexed", pushOrdinaryGeometryState);

   if (clearFullscreenBit == std::string::npos ||
       pushOrdinaryGeometryState == std::string::npos ||
       indexedDraw == std::string::npos)
   {

      std::cerr << "Every ordinary indexed draw must explicitly clear and push FULLSCREEN_BIT before drawing; "
                   "otherwise pixel-coordinate geometry is interpreted as clip-space and disappears.\n";
      return 1;

   }

   return 0;

}

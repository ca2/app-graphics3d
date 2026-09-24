#include <cctype>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>


namespace
{


   int count_occurrences(const std::string &text, const std::string &needle)
   {

      int count = 0;
      std::string::size_type position = 0;

      while ((position = text.find(needle, position)) != std::string::npos)
      {

         count++;
         position += needle.size();

      }

      return count;

   }


   std::string without_whitespace(const std::string &text)
   {

      std::string compact;

      for (const auto character : text)
      {

         if (!std::isspace(static_cast<unsigned char>(character)))
         {

            compact += character;

         }

      }

      return compact;

   }


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
   const auto pristineInternalText = read_text(wrapper / "vkvg/src/vkvg_device_internal.c");
   const auto pristineDeviceText = read_text(wrapper / "vkvg/src/vkvg_device.c");
   const auto dependencyPatch = read_text(wrapper / "patches/0004-strengthen-render-pass-handoffs.patch");
   const auto samplePatch = read_text(wrapper / "patches/0003-honor-requested-sample-count.patch");
   const auto allCommands = "VK_PIPELINE_STAGE_ALL_COMMANDS_BIT";
   const auto sampleGuard = "if (!(counts & info->samples))";

   if (pristineInternalText.empty() || pristineDeviceText.empty() ||
       dependencyPatch.empty() || samplePatch.empty())
   {

      std::cerr << "Unable to read the pristine VKVG device sources and wrapper patches.\n";
      return 1;

   }

   const auto pristineInternalCompact = without_whitespace(pristineInternalText);
   const auto pristineRenderPassBegin =
      pristineInternalCompact.find("VkRenderPass_device_createRenderPassNoResolve");
   const auto pristineRenderPassEnd =
      pristineInternalCompact.find("void_device_setupPipelines", pristineRenderPassBegin);

   if (pristineRenderPassBegin == std::string::npos || pristineRenderPassEnd == std::string::npos)
   {

      std::cerr << "Unable to locate VKVG's pristine render-pass creation functions.\n";
      return 1;

   }

   const auto pristineRenderPass = pristineInternalCompact.substr(
      pristineRenderPassBegin, pristineRenderPassEnd - pristineRenderPassBegin);

   if (pristineRenderPass.find(allCommands) != std::string::npos ||
       pristineDeviceText.find(sampleGuard) != std::string::npos)
   {

      std::cerr << "The vendored VKVG device sources must remain pristine; the fixes belong to the wrapper.\n";
      return 1;

   }

   if (dependencyPatch.find(allCommands) == std::string::npos ||
       dependencyPatch.find("VK_ATTACHMENT_STORE_OP_STORE") == std::string::npos ||
       samplePatch.find(sampleGuard) == std::string::npos)
   {

      std::cerr << "The wrapper patch series is missing a required VKVG device fix.\n";
      return 1;

   }

   const auto nonce = std::chrono::high_resolution_clock::now().time_since_epoch().count();
   const auto output = std::filesystem::temp_directory_path() /
      ("vkvg-render-pass-contract-" + std::to_string(nonce));

   if (!materialize_vkvg(wrapper, output))
   {

      std::cerr << "Unable to materialize the wrapper-owned VKVG patches.\n";
      return 1;

   }

   const auto sourceText = read_text(output / "src/vkvg_device_internal.c");
   const auto deviceSourceText = read_text(output / "src/vkvg_device.c");
   std::error_code removeError;
   std::filesystem::remove_all(output, removeError);
   const auto text = without_whitespace(sourceText);
   const auto deviceText = without_whitespace(deviceSourceText);

   const auto incomingStage =
      "VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT";
   const auto incomingAccess =
      "VK_ACCESS_MEMORY_WRITE_BIT,VK_ACCESS_COLOR_ATTACHMENT_READ_BIT|"
      "VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT";
   const auto outgoingStage =
      "VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,VK_PIPELINE_STAGE_ALL_COMMANDS_BIT";
   const auto outgoingAccess =
      "VK_ACCESS_COLOR_ATTACHMENT_READ_BIT|VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,"
      "VK_ACCESS_MEMORY_READ_BIT|VK_ACCESS_MEMORY_WRITE_BIT";
   const auto multisampleBegin = text.find("VkRenderPass_device_createRenderPassMS");
   const auto multisampleEnd = text.find("void_device_setupPipelines", multisampleBegin);

   if (multisampleBegin == std::string::npos || multisampleEnd == std::string::npos)
   {

      std::cerr << "Unable to locate VKVG's staged multisample render pass.\n";
      return 1;

   }

   const auto multisampleRenderPass = text.substr(multisampleBegin, multisampleEnd - multisampleBegin);
   const auto preservedMultisampleColor =
      ".samples=dev->samples,.loadOp=loadOp,.storeOp=VK_ATTACHMENT_STORE_OP_STORE";
   const auto framebufferLocalExternalDependency = "VK_DEPENDENCY_BY_REGION_BIT";

   if (count_occurrences(text, incomingStage) != 2 ||
       count_occurrences(text, incomingAccess) != 2 ||
       count_occurrences(text, outgoingStage) != 2 ||
       count_occurrences(text, outgoingAccess) != 2 ||
       count_occurrences(text, framebufferLocalExternalDependency) != 0 ||
       multisampleRenderPass.find(preservedMultisampleColor) == std::string::npos ||
       deviceText.find("if(!(counts&info->samples))info->samples=VK_SAMPLE_COUNT_1_BIT;") == std::string::npos ||
       deviceText.find("info->samples=get_max_sample_count(counts);") != std::string::npos)
   {

      std::cerr << "VKVG render passes must make all prior writes (including interop transfers) visible before "
                   "LOAD and make their output available to subsequent consumers without framebuffer-local "
                   "dependency scope; the multisample color attachment must also be stored.\n";
      return 1;

   }

   return 0;

}

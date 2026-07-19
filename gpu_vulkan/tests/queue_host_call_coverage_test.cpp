#include <array>
#include <fstream>
#include <iterator>
#include <string>


namespace
{


   std::string read_file(const char *path)
   {

      std::ifstream stream(path, std::ios::binary);

      return {
         std::istreambuf_iterator<char>(stream),
         std::istreambuf_iterator<char>()};

   }


   std::string remove_comments(const std::string &text)
   {

      std::string result;
      bool bLineComment = false;
      bool bBlockComment = false;

      for (std::size_t i = 0; i < text.size(); ++i)
      {

         if (bLineComment)
         {

            if (text[i] == '\n')
            {

               bLineComment = false;
               result += text[i];

            }

            continue;

         }

         if (bBlockComment)
         {

            if (text[i] == '*' && i + 1 < text.size() && text[i + 1] == '/')
            {

               bBlockComment = false;
               ++i;

            }

            continue;

         }

         if (text[i] == '/' && i + 1 < text.size() && text[i + 1] == '/')
         {

            bLineComment = true;
            ++i;
            continue;

         }

         if (text[i] == '/' && i + 1 < text.size() && text[i + 1] == '*')
         {

            bBlockComment = true;
            ++i;
            continue;

         }

         result += text[i];

      }

      return result;

   }


} // namespace


int main()
{

   constexpr std::array sourcePaths{
      "gpu_vulkan/command_buffer.cpp",
      "gpu_vulkan/context.cpp",
      "gpu_vulkan/renderer.cpp",
      "gpu_vulkan/swap_chain.cpp"};

   constexpr std::array rawQueueCalls{
      "vkQueueSubmit(",
      "vkQueueWaitIdle(",
      "vkQueuePresentKHR("};

   for (const auto *path : sourcePaths)
   {

      const auto source = remove_comments(read_file(path));

      for (const auto *rawQueueCall : rawQueueCalls)
      {

         if (source.find(rawQueueCall) != std::string::npos)
         {

            return 1;

         }

      }

   }

   return 0;

}

#include <cctype>
#include <fstream>
#include <iterator>
#include <string>


namespace
{


   std::string read_file(const char * path)
   {

      std::ifstream stream(path, std::ios::binary);

      return {
         std::istreambuf_iterator<char>(stream),
         std::istreambuf_iterator<char>()};

   }


   std::string compact_whitespace(const std::string & text)
   {

      std::string compact;
      bool previousWasWhitespace = false;

      for (const auto character : text)
      {

         const bool isWhitespace = std::isspace(static_cast<unsigned char>(character)) != 0;

         if (!isWhitespace || !previousWasWhitespace)
         {

            compact += isWhitespace ? ' ' : character;

         }

         previousWasWhitespace = isWhitespace;

      }

      return compact;

   }


} // namespace


int main()
{

   const auto header = compact_whitespace(read_file("gpu_vulkan/command_buffer.h"));
   const auto implementation = read_file("gpu_vulkan/command_buffer.cpp");

   if (header.find("VkFence m_vkfence;") != std::string::npos)
   {

      return 1;

   }

   if (implementation.find("m_vkfence = VK_NULL_HANDLE") != std::string::npos
      || implementation.find("&m_vkfence") != std::string::npos)
   {

      return 1;

   }

   return 0;

}

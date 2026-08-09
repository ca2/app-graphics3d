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

   const auto swapChainSource = remove_comments(read_file("../app/bred/gpu/swap_chain.cpp"));
   const auto renderTargetSource = remove_comments(read_file("../app/bred/gpu/render_target.cpp"));
   const auto vulkanSwapChainSource = remove_comments(read_file("gpu_vulkan/swap_chain.cpp"));

   if (swapChainSource.find("m_ptextureaSwapChain->element_at(m_iCurrentSwapChainImage)") == std::string::npos)
   {

      return 1;

   }

   if (swapChainSource.find("m_semaphoreaWait.add_unique(framesync.m_pgpusemaphoreImageAvailable)") == std::string::npos)
   {

      return 2;

   }

   if (renderTargetSource.find("m_pgpusemaphoreImageAvailable") != std::string::npos)
   {

      return 3;

   }

   if (vulkanSwapChainSource.find("VK_IMAGE_LAYOUT_PRESENT_SRC_KHR") == std::string::npos)
   {

      return 4;

   }

   return 0;

}


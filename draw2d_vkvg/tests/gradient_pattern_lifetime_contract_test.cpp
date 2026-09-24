#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>


int main()
{

   const auto sourcePath = std::filesystem::path(__FILE__).parent_path().parent_path() / "graphics.cpp";
   std::ifstream source(sourcePath);

   if (!source)
   {

      std::cerr << "Unable to open " << sourcePath << '\n';
      return 1;

   }

   int activePatternDestroyCount = 0;
   bool hasDiagnosticSourceOverride = false;
   std::string line;

   while (std::getline(source, line))
   {

      const auto firstNonWhitespace = line.find_first_not_of(" \t");
      if (firstNonWhitespace == std::string::npos)
      {

         continue;

      }

      const auto code = line.substr(firstNonWhitespace);
      if (code == "vkvg_pattern_destroy(ppattern);")
      {

         activePatternDestroyCount++;

      }

      if (code.find("Diagnostic: isolate gradient state") != std::string::npos ||
          code.find("vkvg_set_source_rgba(vkvgcontext, 1.0f, 0.0f, 1.0f, 1.0f);") != std::string::npos)
      {

         hasDiagnosticSourceOverride = true;

      }

   }

   if (activePatternDestroyCount != 2)
   {

      std::cerr << "Expected the radial and linear gradient branches to release their pattern references; found "
                << activePatternDestroyCount << " active destroy calls.\n";
      return 1;

   }

   if (hasDiagnosticSourceOverride)
   {

      std::cerr << "The temporary solid-color gradient diagnostic is still active.\n";
      return 1;

   }

   return 0;

}

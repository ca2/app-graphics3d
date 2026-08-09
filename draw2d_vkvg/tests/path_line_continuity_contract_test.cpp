#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
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

   const std::string text((std::istreambuf_iterator<char>(source)), std::istreambuf_iterator<char>());
   const auto functionBegin = text.find("bool graphics::_set(const ::f64_line& line)");
   const auto functionEnd = text.find("bool graphics::_set(const ::i32_point_array& pointa)", functionBegin);

   if (functionBegin == std::string::npos || functionEnd == std::string::npos)
   {

      std::cerr << "Unable to locate the f64_line path adapter.\n";
      return 1;

   }

   const auto function = text.substr(functionBegin, functionEnd - functionBegin);
   const auto discontinuity = function.find("if (is_different(x, line.m_p1.x, 0.0001)");
   const auto fallback = function.find("else", discontinuity);
   const auto connect = function.find("vkvg_line_to(vkvgcontext, line.m_p1.x, line.m_p1.y);", discontinuity);

   if (discontinuity == std::string::npos || fallback == std::string::npos ||
       connect == std::string::npos || connect > fallback)
   {

      std::cerr << "A line item whose first point differs from VKVG's current point must connect to it "
                   "instead of starting a new two-point subpath.\n";
      return 1;

   }

   return 0;

}

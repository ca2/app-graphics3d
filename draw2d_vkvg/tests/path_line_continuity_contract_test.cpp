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
   const auto currentPoint = function.find(
      "vkvg_get_current_point(vkvgcontext, &xCurrent, &yCurrent);");
   const auto getMatrix = function.find(
      "vkvg_get_matrix(vkvgcontext, &matrix);", currentPoint);
   const auto transformStart = function.find(
      "vkvg_matrix_transform_point(&matrix, &xStart, &yStart);", getMatrix);
   const auto discontinuity = function.find(
      "if (is_different(xCurrent, xStart, 0.0001)", transformStart);
   const auto fallback = function.find("else", discontinuity);
   const auto connect = function.find(
      "vkvg_line_to(vkvgcontext, (::f32)line.m_p1.x, (::f32)line.m_p1.y);",
      discontinuity);

   if (currentPoint == std::string::npos || getMatrix == std::string::npos ||
       transformStart == std::string::npos || discontinuity == std::string::npos ||
       fallback == std::string::npos ||
       connect == std::string::npos || connect > fallback)
   {

      std::cerr << "A line item must compare its transformed start with VKVG's device-space current "
                   "point and connect discontinuities without inserting translated duplicate vertices.\n";
      return 1;

   }

   return 0;

}

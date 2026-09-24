#include <fstream>
#include <iterator>
#include <string>
#include <cstdio>
#include <cstdlib>

static void require(bool ok, const char *message) {
   if (!ok) { std::fprintf(stderr, "%s\n", message); std::exit(1); }
}

int main() {
   std::ifstream file("draw2d_vkvg/graphics.cpp", std::ios::binary);
   require(file.good(), "Run from source/app-graphics3d");
   const std::string source{std::istreambuf_iterator<char>(file), {}};
   const auto floating = source.find("void graphics::draw_text(const ::scoped_string& str, const ::f64_rectangle&");
   const auto integer = source.find("void graphics::draw_text(const ::scoped_string& str, const ::i32_rectangle&");
   require(floating != std::string::npos && integer > floating, "rectangle text overloads missing");
   const auto floatingBody = source.substr(floating, integer - floating);
   require(floatingBody.find("if (str.is_empty() || rectangle.is_empty())") != std::string::npos,
           "empty text must be a no-op");
   require(floatingBody.find("::gpu::context_lock contextlock(gpu_context());") != std::string::npos,
           "rectangle text must acquire the GPU context");
   require(floatingBody.find("internal_draw_text(str, rectangle, ealign, edrawtext);") != std::string::npos,
           "floating rectangle text is not routed to VKVG");
   require(source.substr(integer, 400).find("draw_text(str, ::f64_rectangle(rectangle), ealign, edrawtext);")
           != std::string::npos, "integer rectangle text must preserve alignment and flags");
   const auto glyphStart = source.find("void graphics::internal_draw_text_vkvg(");
   const auto glyphEnd = source.find("bool graphics::_set(::write_text::font*", glyphStart);
   require(glyphStart != std::string::npos && glyphEnd > glyphStart, "glyph renderer missing");
   const auto glyphBody = source.substr(glyphStart, glyphEnd - glyphStart);
   const auto brush = glyphBody.find("_set(m_pdraw2dbrush);");
   const auto draw = glyphBody.find("(*ftext)(vkvgcontext, strLine);");
   require(brush != std::string::npos && draw != std::string::npos && brush < draw,
           "text brush must be bound before glyph drawing");
   std::puts("File-list text overload routing and glyph brush binding passed.");
}

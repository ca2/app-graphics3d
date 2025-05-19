#include "framework.h"
#include "texture.h"
#include "acme/filesystem/filesystem/file_context.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>


namespace graphics3d_opengl
{


   texture::texture(::particle * pparticle, const std::string& filePath)
      : m_FilePath(filePath), m_TextureID(0)
   {
      initialize(pparticle);

      int width, height, nrChannels;
      stbi_set_flip_vertically_on_load(true); // Flip the image vertically if needed
      auto mem = file()->as_memory(filePath.c_str());
      unsigned char* data = stbi_load_from_memory(
         (const stbi_uc*)mem.data(),
         mem.size(), &width, &height, &nrChannels, 0);
      //unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

      if (data) {
         glGenTextures(1, &m_TextureID);
         glBindTexture(GL_TEXTURE_2D, m_TextureID);

         // Set the texture wrapping parameters
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

         // Set texture filtering parameters
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

         // Load the texture data into OpenGL
         GLenum format;
         if (nrChannels == 1)
            format = GL_RED;
         else if (nrChannels == 3)
            format = GL_RGB;
         else if (nrChannels == 4)
            format = GL_RGBA;
         else {
            std::cerr << "Unsupported number of channels: " << nrChannels << std::endl;
            stbi_image_free(data);
            return;
         }

         glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
         glGenerateMipmap(GL_TEXTURE_2D);

         stbi_image_free(data);
      }
      else {
         std::cerr << "Failed to load texture: " << filePath << std::endl;
      }
   }

   texture::~texture() {
      glDeleteTextures(1, &m_TextureID);
   }

   void texture::Bind(unsigned int slot) const {
      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(GL_TEXTURE_2D, m_TextureID);

   }

   void texture::Unbind() const {
      glBindTexture(GL_TEXTURE_2D, 0);
   }
   //*/


} // namespace graphics3d_opengl





#include "framework.h"
#include "cube_map.h"
#include "acme/filesystem/filesystem/file_context.h"
#include <stb_image.h>
#include <iostream>


namespace graphics3d_opengl
{

   // Constructor
   sky_box::sky_box(::particle * pparticle, const std::vector<std::string>& faces)
      : facesCubemap(faces)
   {
      initialize(pparticle);
      SetupSkybox();
   }

   // Destructor
   sky_box::~sky_box() {
      glDeleteVertexArrays(1, &skyboxVAO);
      glDeleteBuffers(1, &skyboxVBO);
      glDeleteTextures(1, &cubemapTexture);
   }

   // Setup the skybox (VAO, VBO, EBO, and cubemap textures)
   void sky_box::SetupSkybox() {
      // Generate buffers
      glGenVertexArrays(1, &skyboxVAO);
      glGenBuffers(1, &skyboxVBO);

      glBindVertexArray(skyboxVAO);

      glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
      //glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

      // Set the vertex attribute pointer for the cube vertices
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

      // Load cubemap textures
      LoadCubemapTextures();

      glBindVertexArray(0);  // Unbind VAO
   }

   // Load cubemap textures
   void sky_box::LoadCubemapTextures() {
      glGenTextures(1, &cubemapTexture);
      glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

      int width, height, nrChannels;
      for (unsigned int i = 0; i < facesCubemap.size(); i++) {
//         unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
         auto mem = file()->as_memory(facesCubemap[i].c_str());
         unsigned char* data = stbi_load_from_memory(
            (const stbi_uc*)mem.data(),
            mem.size(), &width, &height, &nrChannels, 0);

         if (data) {
            // Load the texture data into the cubemap
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
               0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
         }
         else if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
               0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
         }
         else {
            std::cout << "Failed to load cubemap texture at path: " << facesCubemap[i] << std::endl;
            stbi_image_free(data);
         }
      }

      // Set texture parameters for cubemap
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
   }



} // namespace graphics3d_opengl




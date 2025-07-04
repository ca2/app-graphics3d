#pragma once

#include <glad.h>
#include <string>
#include <vector>


namespace graphics3d_opengl
{

   class sky_box :
   virtual public ::particle 
   
   {
   public:
      sky_box(::particle * pparticle, const ::array<std::string>& faces);
      ~sky_box();

      void SetupSkybox();


   private:
      unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
      unsigned int cubemapTexture;
      ::array<std::string> facesCubemap;

      void LoadCubemapTextures();
   };



} // namespace graphics3d_opengl




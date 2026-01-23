#pragma once

//#include <glad/glad.h>
//#include <string>
//#include <vector>


namespace graphics3d_opengl
{

   class sky_box :
   virtual public ::particle 
   
   {
   public:
      sky_box(::particle * pparticle, const ::string_array & faces);
      ~sky_box();

      void SetupSkybox();


   private:
      unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
      unsigned int cubemapTexture;
      ::string_array facesCubemap;

      void LoadCubemapTextures();
   };



} // namespace graphics3d_opengl




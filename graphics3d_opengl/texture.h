
#pragma once 

#include <string>
#include <glad.h>  

namespace graphics3d_opengl
{

   class texture :
      virtual public ::particle
   
   {
   public:
      texture(::particle * pparticle,const std::string& filePath);
      ~texture();

      void Bind(unsigned int slot = 0) const;
      void Unbind() const;

      GLuint m_TextureID;
      std::string m_FilePath;
      ::std::string type;
   };

   //*/

} // namespace graphics3d_opengl



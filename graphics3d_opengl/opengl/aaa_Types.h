
#pragma once
#pragma warning(push, 0)



#include <glad.h>
//#include <GLFW/glfw3.h>

#include <string>



//#include "Math.h"
#pragma warning(pop)
#include "Defines.h"
#include "_constant.h"


#define MAX_BONE_INFLUENCE 4

namespace glc
{
   struct Vertex {
      //float Position[3];
      //float Color[4];
      //float TexCoords[2];
      //float TexID;

            // position
      floating_sequence3 Position;
      // normal
      floating_sequence3 Normal;
      // texCoords
      floating_sequence2 TexCoords;
      // tangent
      floating_sequence3 Tangent;
      // bitangent
      floating_sequence3 Bitangent;
      //bone indexes which will influence this vertex
      int m_BoneIDs[MAX_BONE_INFLUENCE];
      //weights from each bone
      float m_Weights[MAX_BONE_INFLUENCE];
   };
   //struct Vertex {
   //   // position
   //   floating_sequence3 Position;
   //   // normal
   //   floating_sequence3 Normal;
   //   // texCoords
   //   floating_sequence2 TexCoords;
   //   // tangent
   //   floating_sequence3 Tangent;
   //   // bitangent
   //   floating_sequence3 Bitangent;
   //   //bone indexes which will influence this vertex
   //   int m_BoneIDs[MAX_BONE_INFLUENCE];
   //   //weights from each bone
   //   float m_Weights[MAX_BONE_INFLUENCE];
   //};

} // namespace glc


/** @brief State of mouse/touch input */
struct mouseState
{
   struct {
      bool left = false;
      bool right = false;
      bool middle = false;
   } buttons;
   floating_sequence2 position;
};



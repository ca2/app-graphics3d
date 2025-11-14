
#pragma once
#pragma warning(push, 0)
#define GLM_FORCE_SILENT_WARNINGS
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glad.h>
//#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/hash.hpp"
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



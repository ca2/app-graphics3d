#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include "renderer.h"
#include "shader.h"
#include "mesh.h"
#include "camera.h"
#include "texture.h"


namespace graphics3d_opengl
{

   class renderer;
   class shader;
   class mesh;
   //class Camera;
   class sky_box;

   class render_data :
      virtual public ::particle
   {
   public:

      render_data();
      ~render_data();



      struct DrawIndexedIndirectCommand {
         GLuint indexCount;    // Number of indices to draw
         GLuint instanceCount; // Number of instances
         GLuint firstIndex;    // Starting index in index buffer
         GLuint baseVertex;    // Base vertex offset
         GLuint baseInstance;  // Instance offset
      };

      void Init();
      void Update(float deltaTime);
      void Render(renderer *prenderer, ::graphics3d::camera *pcamera);
      void LoadModel(std::string& path);
      // Misc


   private:
      // Shaders
      ::pointer<shader> m_Shader;
      ::pointer<shader> m_LightingShader;
      ::pointer<shader> m_LightSourceShader;
      ::pointer<shader> m_SkyboxShader;
      ::pointer<shader> m_WallShader;

      // Textures
      ::pointer<texture> m_PlaneTexture;
      ::pointer<texture> m_BoxTexture;
      ::pointer<texture> m_Specular;
      ::pointer<texture> m_SphereTexture;
      ::pointer<texture> m_Misc;

      // Meshes
      ::pointer<mesh> m_BoxMesh;
      ::pointer<mesh> m_PlaneMesh;
      ::pointer<mesh> m_SkyboxMesh;
      ::pointer<mesh> m_SphereMesh;
      ::pointer<mesh> m_WallMesh;

      // Models
      std::vector<::pointer<mesh>> m_Meshes;

      //Instances
      int m_PlaneInstanceCount = 1;
      int m_BoxInstanceCount = 200;
      int m_SphereInstanceCount = 400;
      int m_WallInstanceCount = 1000;


      // Misc
      std::vector<glm::mat4> boxModelMatrices; // Declare as a member variable
      std::vector<glm::mat4> wallModelMatrices;
      std::vector<glm::vec3> wallPositions;




      float rotationSpeed = 60.0f; // Degrees per second


      float scaleFactorX = 20.0f;
      float scaleFactorY = 20.0f;
      float scaleFactorZ = 20.0f;

      ::pointer<sky_box> m_Skybox;


   };





} //  namespace graphics3d_opengl






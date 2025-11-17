#pragma once

#include <string>
#include <memory>

#include "bred/graphics3d/scene.h"
#include "bred/graphics3d/mesh.h"
//#include "shader.h"
//#include "mesh.h"
//#include "camera.h"
//#include "texture.h"


namespace app_graphics3d_MyPlace2025
{

   //class renderer;
   //class shader;
   //class mesh;
   //class Camera;
   //class sky_box;

   class main_scene :
      virtual public ::graphics3d::scene
   {
   public:

      main_scene();
      ~main_scene();



      //struct DrawIndexedIndirectCommand {
      //   GLuint indexCount;    // Number of indexes to draw
      //   GLuint instanceCount; // Number of instances
      //   GLuint firstIndex;    // Starting index in index buffer
      //   GLuint baseVertex;    // Base vertex offset
      //   GLuint baseInstance;  // Instance offset
      //};

      void Init();
      void Update(float deltaTime);
      void Render(::gpu::renderer* prenderer, ::graphics3d::camera* pcamera);
      void LoadModel(std::string& path);
      // Misc


   //private:
      // Shaders
      ::pointer<::gpu::shader> m_Shader;
      ::pointer<::gpu::shader> m_LightingShader;
      ::pointer<::gpu::shader> m_LightSourceShader;
      //::pointer<::gpu::shader> m_SkyboxShader;
      ::pointer<::gpu::shader> m_WallShader;
                
      // Textures
      ::pointer<::gpu::texture> m_PlaneTexture;
      ::pointer<::gpu::texture> m_BoxTexture;
      ::pointer<::gpu::texture> m_Specular;
      ::pointer<::gpu::texture> m_SphereTexture;
      ::pointer<::gpu::texture> m_Misc;

      // Meshes
      ::pointer<::graphics3d::mesh> m_BoxMesh;
      ::pointer<::graphics3d::mesh> m_PlaneMesh;
      ::pointer<::graphics3d::mesh> m_SkyboxMesh;
      ::pointer<::graphics3d::mesh> m_SphereMesh;
      ::pointer<::graphics3d::mesh> m_WallMesh;

      // Models
      std::vector<::pointer<::graphics3d::mesh>> m_Meshes;

      //Instances
      int m_PlaneInstanceCount = 1;
      int m_BoxInstanceCount = 200;
      int m_SphereInstanceCount = 400;
      int m_WallInstanceCount = 1000;


      // Misc
      std::vector<floating_matrix4> boxModelMatrices; // Declare as a member variable
      std::vector<floating_matrix4> wallModelMatrices;
      std::vector<floating_sequence3> wallPositions;




      float rotationSpeed = 60.0f; // Degrees per second


      float scaleFactorX = 20.0f;
      float scaleFactorY = 20.0f;
      float scaleFactorZ = 20.0f;

      ::pointer<::graphics3d::skybox> m_Skybox;


   };


} //  namespace app_graphics3d_MyPlace2025






#include "../framework.h"
//#include "render_data.h"
//#include "renderer.h"
//#include "shader.h"
//#include "cube_map.h"
//
//
//#include "mesh.h"
//#include <cmath>
//#include <vector>
//#include <stb_image.h>
//#include "shapes.h"
////#include "common.h"
//#include <random>
//  // For matrix transforms like rotate, translate
// 
//
//
//namespace graphics3d_opengl
//{
//
//
//   void render_data::Init()
//   {
//
//
//      // Initialize shaders
//      m_Shader = øallocate shader(this,
//         "matter://shaders/default.vert",
//         "matter://shaders/default.frag"
//      );
//
//      //m_Shader->initialize(this);
//
//      m_WallShader = øallocate shader(
//         this,
//         "matter://shaders/misc.vert",
//         "matter://shaders/misc.frag"
//      );
//
//      //m_WallShader->initialize(this);
//
//      // Load textures
//      m_BoxTexture      = øallocate texture(this, "matter://textures/stoneWall.jpg");
//      m_PlaneTexture    = øallocate texture(this, "matter://textures/spaceFloor.jpg");
//      m_SphereTexture   = øallocate texture(this, "matter://textures/greenWall.jpg");
//      m_Misc            = øallocate texture(this, "matter://textures/mystical1.jpg");
//
//      //m_BoxTexture->initialize(this);
//        // m_PlaneTexture -> initialize(this);
//         //m_SphereTexture -> initialize(this);
//         //m_Misc -> initialize(this);
//      // sky_box textures
//      ::array<std::string> facesCubemap = {
//
//         // Cloudy skybox
//        /* "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CloudySkybox/bluecloud_ft.jpg",
//         "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CloudySkybox/bluecloud_bk.jpg",
//         "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CloudySkybox/bluecloud_dn.jpg",
//         "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CloudySkybox/bluecloud_up.jpg",
//         "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CloudySkybox/bluecloud_rt.jpg",
//         "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CloudySkybox/bluecloud_lf.jpg",*/
//
//         // Hell skybox
//        /* "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CoolSkybox/NightSky_Front.png",
//         "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CoolSkybox/NightSky_Back.png",
//
//          "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CoolSkybox/NightSky_Top.png",
//          "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CoolSkybox/NightSky_Bottom.png",
//
//         "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CoolSkybox/NightSky_Left.png",
//         "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CoolSkybox/NightSky_Right.png",
//       */
//
//
//       // Space sky_box
//        "matter://textures/SpaceSkybox/right.png",
//        "matter://textures/SpaceSkybox/left.png",
//        "matter://textures/SpaceSkybox/bot.png",
//        "matter://textures/SpaceSkybox/top.png",
//        "matter://textures/SpaceSkybox/front.png",
//        "matter://textures/SpaceSkybox/back.png"
//      };
//
//      m_Skybox = øallocate sky_box(this, facesCubemap);
//
//      // Initialize skybox shader
//      m_SkyboxShader = øallocate shader(
//         this,
//         "matter://shaders/skybox.vert",
//         "matter://shaders/skybox.frag"
//      );
//
//      //m_Skybox->initialize(this);
//      //m_SkyboxShader->initialize(this);
//
//      ::array<floating_matrix4> planeModelMatrices(m_PlaneInstanceCount);
//      ::array<floating_matrix4> sphereModelMatrices(m_SphereInstanceCount);
//      ::array<floating_matrix4> wallModelMatricies(m_WallInstanceCount);
//
//      // Model instances
//      // Create a random number generator
//      std::random_device rd;  // Obtain a random number from hardware
//      std::mt19937 gen(rd()); // Seed the generator
//      std::uniform_real_distribution<float> dis(-300.0f, 200.0f); // Increase the range for more spread
//
//      // Box instances
//      for (int i = 0; i < m_BoxInstanceCount; ++i) {
//         // Apply random translations
//         float randomX = dis(gen);
//         float randomY = dis(gen);
//         float randomZ = dis(gen);
//         boxModelMatrices[i] = glm::translate(floating_matrix4(1.0f), floating_sequence3(randomX, randomY, randomZ));
//         //boxModelMatrices[i] = glm::scale(floating_matrix4(1.0f), floating_sequence3(10.0f, 10.0f, 10.0f));
//      }
//
//      // Plane instances
//      for (int i = 0; i < m_PlaneInstanceCount; ++i) {
//         // Apply random translations
//
//         float explicitY = 15.0f;
//         float randomX = dis(gen);
//         float randomY = dis(gen) - 5.0f; // Shift downwards
//         float randomZ = dis(gen);
//         planeModelMatrices[i] = glm::translate(floating_matrix4(1.0f), floating_sequence3(randomX, explicitY, randomZ));
//         planeModelMatrices[i] = glm::scale(floating_matrix4(1.0f), floating_sequence3(100.0f, 100.0f, 100.0f));
//      }
//
//      // Sphere instances
//      for (int i = 0; i < m_SphereInstanceCount; ++i) {
//         float randomX = dis(gen);
//         float randomY = dis(gen);
//         float randomZ = dis(gen);
//         sphereModelMatrices[i] = glm::translate(floating_matrix4(1.0f), floating_sequence3(randomX, randomY, randomZ));
//      }
//
//      // Wall Instances
//      for (int i = 0; i < m_WallInstanceCount; i++) {
//         float y = -30.0f;
//         float randomX = dis(gen);
//         float randomY = dis(gen) + 5.0f;
//         float randomZ = dis(gen);
//         wallModelMatricies[i] = glm::translate(floating_matrix4(1.0f), floating_sequence3(randomX, y, randomZ));
//
//      }
//
//
//
//      // Set the model matrices for each mesh instance
//      m_BoxMesh->SetInstanceModelMatrices(boxModelMatrices);
//      m_PlaneMesh->SetInstanceModelMatrices(planeModelMatrices);
//      m_SphereMesh->SetInstanceModelMatrices(sphereModelMatrices);
//      m_WallMesh->SetInstanceModelMatrices(wallModelMatricies);
//   }
//
//   void render_data::Update(float deltaTime) {
//      floating_sequence3 rotationAxis(0.0f, 1.0f, 0.0f);
//      float rotationSpeed = ::radians(50.0f) * deltaTime;
//
//      // Update box model matrices with rotation (this part is working as expected)
//      for (int i = 0; i < m_BoxInstanceCount; ++i) {
//         boxModelMatrices[i] = glm::rotate(boxModelMatrices[i], rotationSpeed, rotationAxis);
//      }
//
//
//      // Update the meshes with the new model matrices
//      m_BoxMesh->SetInstanceModelMatrices(boxModelMatrices);
//
//   }
//
//
//   render_data::render_data()
//      : m_Shader(nullptr),
//      m_WallShader(nullptr),
//      boxModelMatrices(m_BoxInstanceCount, floating_matrix4(1.0f)),
//      wallModelMatrices(m_WallInstanceCount, floating_matrix4(1.0f))
//
//   {
//
//      // Generate shape data
//      ShapeFactory::ShapeData graphics3d = ShapeFactory::CreateCube(1.7f);
//      ShapeFactory::ShapeData plane = ShapeFactory::CreatePlane(2.0f, 2.0f);
//      ShapeFactory::ShapeData skyboxCube = ShapeFactory::CreateCube(1.0f);
//      ShapeFactory::ShapeData sphere = ShapeFactory::CreateSphere(1.0f, 32, 32);
//      ShapeFactory::ShapeData wall = ShapeFactory::CreateWall(5.0, 5.0);
//
//
//      // Offsets for vertex and index data (dynamic approach)
//      size_t cubeVertexOffset = 0;
//      size_t cubeIndexOffset = 0;
//      size_t cubeIndexCount = graphics3d.indexes.size();
//
//      // Combine the graphics3d data and prepare offsets for the plane
//      ::array<float> combinedVertices = graphics3d.vertexes;  // Start with graphics3d's vertexes
//      ::array<unsigned int> combinedIndices = graphics3d.indexes;  // Start with graphics3d's indexes
//
//      size_t planeVertexOffset = combinedVertices.size() / 8;  // Calculate vertex offset for plane
//      size_t planeIndexOffset = combinedIndices.size();         // Calculate index offset for plane
//      size_t planeIndexCount = plane.indexes.size();
//
//      // Append the plane data to the combined data
//      combinedVertices.insert(combinedVertices.end(), plane.vertexes.begin(), plane.vertexes.end());
//      combinedIndices.insert(combinedIndices.end(), plane.indexes.begin(), plane.indexes.end());
//
//      // Sphere offsets and data appending
//      size_t sphereVertexOffset = combinedVertices.size() / 8;  // Sphere vertex offset
//      size_t sphereIndexOffset = combinedIndices.size();        // Sphere index offset
//      size_t sphereIndexCount = sphere.indexes.size();
//
//      combinedVertices.insert(combinedVertices.end(), sphere.vertexes.begin(), sphere.vertexes.end());
//      combinedIndices.insert(combinedIndices.end(), sphere.indexes.begin(), sphere.indexes.end());
//
//      // Wall offsets and data appending
//      size_t wallVertexOffset = combinedVertices.size() / 8;
//      size_t wallIndexOffset = combinedIndices.size();
//      size_t wallIndexCount = wall.indexes.size();
//
//      combinedVertices.insert(combinedVertices.end(), wall.vertexes.begin(), wall.vertexes.end());
//      combinedIndices.insert(combinedIndices.end(), wall.indexes.begin(), wall.indexes.end());
//
//
//      // Create Cube mesh object
//      m_BoxMesh = øallocate mesh(
//         combinedVertices, combinedIndices, cubeVertexOffset, cubeIndexOffset, cubeIndexCount
//      );
//
//      // Create Plane mesh object
//      m_PlaneMesh = øallocate mesh(
//         combinedVertices, combinedIndices, planeVertexOffset, planeIndexOffset, planeIndexCount
//      );
//
//      // Create sky_box mesh object
//      m_SkyboxMesh = øallocate mesh(
//         skyboxCube.vertexes, skyboxCube.indexes, 0, 0, skyboxCube.indexes.size()
//      );
//
//      // Create Sphere mesh object
//      m_SphereMesh = øallocate mesh(
//         combinedVertices, combinedIndices, sphereVertexOffset, sphereIndexOffset, sphereIndexCount
//      );
//
//      m_WallMesh = øallocate  mesh(
//         combinedVertices, combinedIndices, wallVertexOffset, wallIndexOffset, wallIndexCount
//      );
//
//      m_BoxMesh->initialize(this);
//      m_PlaneMesh->initialize(this);
//      m_SphereMesh->initialize(this);
//      m_SkyboxMesh->initialize(this);
//      m_WallMesh->initialize(this);
//
//
//
//   }
//
//
//   void render_data::Render(renderer *prenderer, ::graphics3d:: *pcamera) 
//   {
//
//
//      // Calculate view and projection matrices
//      floating_matrix4 view = pcamera->GetViewMatrix();
//      floating_matrix4 projection = glm::perspective(::radians(pcamera->GetZoom()), 1280.0f / 720.0f, 0.1f, 1000.0f);
//
//      // sky_box
//      floating_matrix4 skyboxView = floating_matrix4(floating_matrix3(view)); // Remove translation from the view matrix
//      m_SkyboxShader->Bind();
//      m_SkyboxShader->SetUniformMat4f("view", skyboxView);
//      m_SkyboxShader->SetUniformMat4f("projection", projection);
//
//
//      // Set uniforms in the shader
//      m_SkyboxShader->Bind(); // Make sure to bind the shader first
//
//      prenderer->DrawSkybox(m_SkyboxMesh, m_SkyboxShader);
//
//      // Bind the shader and set the view and projection matrices
//      m_Shader->Bind();
//      m_Shader->SetUniformMat4f("view", view);
//      m_Shader->SetUniformMat4f("projection", projection);
//
//      // Bind the box mesh and render multiple instances using instanced rendering
//      m_BoxMesh->Bind();
//      m_BoxTexture->Bind();
//      prenderer->DrawInstanced({ m_BoxMesh.get() }, m_Shader, m_BoxInstanceCount);  // Drawing the box instances
//
//      // Bind the plane texture
//      m_PlaneTexture->Bind();
//      m_PlaneMesh->Bind();
//      prenderer->DrawInstanced({ m_PlaneMesh.get() }, m_Shader, m_PlaneInstanceCount);  // Drawing the plane instances
//
//      // Render the sphere instances
//      m_SphereTexture->Bind();
//      m_SphereMesh->Bind();
//      prenderer->DrawInstanced({ m_SphereMesh.get() }, m_Shader, m_SphereInstanceCount);  // Draw sphere instances
//
//
//      // Render the wall
//      m_WallShader->Bind();
//      m_WallShader->SetUniformMat4f("view", view);
//      m_WallShader->SetUniformMat4f("projection", projection);
//
//
//      // Get camera position
//      floating_sequence3 cameraPosition = pcamera->GetPosition();
//
//      m_WallShader->SetUniform3f("cameraPos", cameraPosition.x, cameraPosition.y, cameraPosition.z);
//
//      m_Misc->Bind();
//      m_WallMesh->Bind();
//      prenderer->DrawInstanced({ m_WallMesh.get() }, m_WallShader, m_WallInstanceCount); // Draw robin
//
//      // Unbind the shader
//      m_Shader->Unbind();
//   }
//
//   render_data::~render_data() {}
//
//
//
//} // namespace graphics3d_opengl

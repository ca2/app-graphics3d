#include "platform.h"
#include "main.h"
#include "aura/platform/application.h"
#include "bred/gpu/renderer.h"
#include "bred/gpu/shader.h"
#include "bred/gpu/texture.h"
#include "bred/graphics3d/skybox.h"
#include "bred/graphics3d/engine.h"
#include "bred/graphics3d/immersion_layer.h"


//#include "bred/graphics3d/cube_map.h"
#include "bred/graphics3d/mesh.h"
#include <cmath>
#include <vector>
//#include <stb_image.h>
//#include "bred/graphics3d/cube_map.h"
#include "bred/graphics3d/shape_factory.h"
//#include "common.h"
#include <random>
  // For matrix transforms like rotate, translate
 


namespace app_graphics3d_MyPlace2025
{


   void main_scene::Init()
   {

      auto pgpucontext = m_pimmersionlayer->m_pengine->gpu_context();

      // Initialize shaders
      defer_constructø(m_Shader);
      
      m_Shader->initialize_shader(pgpucontext->m_pgpurenderer,
         "matter://shaders/default.vert",
         "matter://shaders/default.frag"
      );

      //m_Shader->initialize(this);

      defer_constructø(m_WallShader);

      m_WallShader->initialize_shader(pgpucontext->m_pgpurenderer,
         "matter://shaders/misc.vert",
         "matter://shaders/misc.frag"
      );

      //m_WallShader->initialize(this);

      // Load textures
      m_BoxTexture = pgpucontext->texture("matter://textures/stoneWall.jpg");
      m_PlaneTexture = pgpucontext->texture("matter://textures/spaceFloor.jpg");
      m_SphereTexture = pgpucontext->texture("matter://textures/greenWall.jpg");
      m_Misc = pgpucontext->texture("matter://textures/mystical1.jpg");

      //m_BoxTexture->initialize(this);
        // m_PlaneTexture -> initialize(this);
         //m_SphereTexture -> initialize(this);
         //m_Misc -> initialize(this);
      // sky_box textures
      ::graphics3d::skybox::cube cube = {

         // Cloudy skybox
        /* "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CloudySkybox/bluecloud_ft.jpg",
         "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CloudySkybox/bluecloud_bk.jpg",
         "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CloudySkybox/bluecloud_dn.jpg",
         "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CloudySkybox/bluecloud_up.jpg",
         "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CloudySkybox/bluecloud_rt.jpg",
         "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CloudySkybox/bluecloud_lf.jpg",*/

         // Hell skybox
        /* "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CoolSkybox/NightSky_Front.png",
         "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CoolSkybox/NightSky_Back.png",

          "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CoolSkybox/NightSky_Top.png",
          "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CoolSkybox/NightSky_Bottom.png",

         "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CoolSkybox/NightSky_Left.png",
         "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CoolSkybox/NightSky_Right.png",
       */


       // Space sky_box
         {"matter://textures/SpaceSkybox/right.png",
        "matter://textures/SpaceSkybox/left.png",
        "matter://textures/SpaceSkybox/bot.png",
        "matter://textures/SpaceSkybox/top.png",
        "matter://textures/SpaceSkybox/front.png",
        "matter://textures/SpaceSkybox/back.png"
        }
      };

      m_Skybox = allocateø ::graphics3d::skybox();

      m_Skybox->initialize_sky_box(this, cube);

      //// Initialize skybox shader
      //defer_constructø(m_SkyboxShader);

      //m_SkyboxShader->initialize_shader(pgpucontext->m_pgpurenderer,
      //   "matter://shaders/skybox.vert",
      //   "matter://shaders/skybox.frag"
      //);

      //m_Skybox->initialize(this);
      //m_SkyboxShader->initialize(this);

      std::vector<floating_matrix4> planeModelMatrices(m_PlaneInstanceCount);
      std::vector<floating_matrix4> sphereModelMatrices(m_SphereInstanceCount);
      std::vector<floating_matrix4> wallModelMatricies(m_WallInstanceCount);

      // Model instances
      // Create a random number generator
      std::random_device rd;  // Obtain a random number from hardware
      std::mt19937 gen(rd()); // Seed the generator
      std::uniform_real_distribution<float> dis(-300.0f, 200.0f); // Increase the range for more spread

      // Box instances
      for (int i = 0; i < m_BoxInstanceCount; ++i) {
         // Apply random translations
         float randomX = dis(gen);
         float randomY = dis(gen);
         float randomZ = dis(gen);
         boxModelMatrices[i] = glm::translate(floating_matrix4(1.0f), floating_sequence3(randomX, randomY, randomZ));
         //boxModelMatrices[i] = glm::scale(floating_matrix4(1.0f), floating_sequence3(10.0f, 10.0f, 10.0f));
      }

      // Plane instances
      for (int i = 0; i < m_PlaneInstanceCount; ++i) {
         // Apply random translations

         float explicitY = 15.0f;
         float randomX = dis(gen);
         float randomY = dis(gen) - 5.0f; // Shift downwards
         float randomZ = dis(gen);
         planeModelMatrices[i] = glm::translate(floating_matrix4(1.0f), floating_sequence3(randomX, explicitY, randomZ));
         planeModelMatrices[i] = glm::scale(floating_matrix4(1.0f), floating_sequence3(100.0f, 100.0f, 100.0f));
      }

      // Sphere instances
      for (int i = 0; i < m_SphereInstanceCount; ++i) {
         float randomX = dis(gen);
         float randomY = dis(gen);
         float randomZ = dis(gen);
         sphereModelMatrices[i] = glm::translate(floating_matrix4(1.0f), floating_sequence3(randomX, randomY, randomZ));
      }

      // Wall Instances
      for (int i = 0; i < m_WallInstanceCount; i++) {
         float y = -30.0f;
         float randomX = dis(gen);
         float randomY = dis(gen) + 5.0f;
         float randomZ = dis(gen);
         wallModelMatricies[i] = glm::translate(floating_matrix4(1.0f), floating_sequence3(randomX, y, randomZ));

      }



      //// Set the model matrices for each mesh instance
      //m_BoxMesh->SetInstanceModelMatrices(boxModelMatrices);
      //m_PlaneMesh->SetInstanceModelMatrices(planeModelMatrices);
      //m_SphereMesh->SetInstanceModelMatrices(sphereModelMatrices);
      //m_WallMesh->SetInstanceModelMatrices(wallModelMatricies);
   }

   void main_scene::Update(float deltaTime) {
      floating_sequence3 rotationAxis(0.0f, 1.0f, 0.0f);
      float rotationSpeed = glm::radians(50.0f) * deltaTime;

      // Update box model matrices with rotation (this part is working as expected)
      for (int i = 0; i < m_BoxInstanceCount; ++i) {
         boxModelMatrices[i] = glm::rotate(boxModelMatrices[i], rotationSpeed, rotationAxis);
      }


      // Update the meshes with the new model matrices
      //m_BoxMesh->SetInstanceModelMatrices(boxModelMatrices);

   }


   main_scene::main_scene()
      : m_Shader(nullptr),
      m_WallShader(nullptr),
      boxModelMatrices(m_BoxInstanceCount, floating_matrix4(1.0f)),
      wallModelMatrices(m_WallInstanceCount, floating_matrix4(1.0f))

   {

      // Generate shape data
      ::graphics3d::shape_factory::shape cube = ::graphics3d::shape_factory::create_cube(1.7f);
      ::graphics3d::shape_factory::shape plane = ::graphics3d::shape_factory::create_plane(2.0f, 2.0f);
      ::graphics3d::shape_factory::shape skyboxCube = ::graphics3d::shape_factory::create_cube(1.0f);
      ::graphics3d::shape_factory::shape sphere = ::graphics3d::shape_factory::create_sphere(1.0f, 32, 32);
      ::graphics3d::shape_factory::shape wall = ::graphics3d::shape_factory::create_wall(5.0, 5.0);


      //// Offsets for vertex and index data (dynamic approach)
      //size_t cubeVertexOffset = 0;
      //size_t cubeIndexOffset = 0;
      //size_t cubeIndexCount = cube.m_indexes.size();

      //::graphics3d::shape_factory::shape shapeCombined;

      //// Combine the graphics3d data and prepare offsets for the plane
      //auto combinedVertices = cube.m_vertexes;  // Start with graphics3d's vertexes
      //auto combinedIndices = cube.m_indexes;  // Start with graphics3d's indexes

      //shapeCombined.append(cube);

      //size_t planeVertexOffset = combinedVertices.size() / 8;  // Calculate vertex offset for plane
      //size_t planeIndexOffset = combinedIndices.size();         // Calculate index offset for plane
      //size_t planeIndexCount = plane.m_indexes.size();

      //// Append the plane data to the combined data
      //combinedVertices.append(plane.m_vertexes);
      //combinedIndices.append(plane.m_indexes);

      //shapeCombined 

      //// Sphere offsets and data appending
      //size_t sphereVertexOffset = combinedVertices.size() / 8;  // Sphere vertex offset
      //size_t sphereIndexOffset = combinedIndices.size();        // Sphere index offset
      //size_t sphereIndexCount = sphere.m_indexes.size();

      //combinedVertices.append(sphere.m_vertexes);
      //combinedIndices.append(sphere.m_indexes);

      //// Wall offsets and data appending
      //size_t wallVertexOffset = combinedVertices.size() / 8;
      //size_t wallIndexOffset = combinedIndices.size();
      //size_t wallIndexCount = wall.m_indexes.size();

      //combinedVertices.append(wall.m_vertexes);
      //combinedIndices.append(wall.m_indexes);


      // Create Cube mesh object
      defer_construct_newø(m_BoxMesh);

      // Create Plane mesh object
      defer_construct_newø(m_PlaneMesh);

      // Create sky_box mesh object
      defer_construct_newø(m_SkyboxMesh);

      // Create Sphere mesh object
      defer_construct_newø(m_SphereMesh);

      defer_construct_newø(m_WallMesh);

      m_BoxMesh->initialize_mesh(cube);
      m_PlaneMesh->initialize_mesh(plane);
      m_SphereMesh->initialize_mesh(sphere);
      m_SkyboxMesh->initialize_mesh(skyboxCube);
      m_WallMesh->initialize_mesh(wall);



   }


   void main_scene::Render(::gpu::renderer* prenderer, ::graphics3d::camera* pcamera)
   {


      //// Calculate view and projection matrices
      //floating_matrix4 view = pcamera->GetViewMatrix();
      //floating_matrix4 projection = glm::perspective(pcamera->GetZoom(), 1280.0f / 720.0f, 0.1f, 1000.0f);

      ////// sky_box
      ////floating_matrix4 skyboxView = floating_matrix4(floating_matrix3(view)); // Remove translation from the view matrix
      ////m_SkyboxShader->bind();
      ////m_SkyboxShader->set_matrix4("view", skyboxView);
      ////m_SkyboxShader->set_matrix4("projection", projection);


      ////// Set uniforms in the shader
      ////m_SkyboxShader->bind(); // Make sure to bind the shader first
      ////m_SkyboxMesh->bind();
      ////m_SkyboxMesh->draw(m_SkyboxShader);

      ////// prenderer->DrawSkybox(m_SkyboxMesh, m_SkyboxShader);
      ////m_SkyboxMesh->unbind();
      ////m_SkyboxShader->unbind(); // Make sure to bind the shader first

      //// Bind the shader and set the view and projection matrices
      //m_Shader->bind();
      //m_Shader->set_matrix4("view", view);
      //m_Shader->set_matrix4("projection", projection);

      //// Bind the box mesh and render multiple instances using instanced rendering
      //m_BoxMesh->bind();
      ////m_BoxTexture->bind();
      ////prenderer->DrawInstanced({ m_BoxMesh.get() }, m_Shader, m_BoxInstanceCount);  // Drawing the box instances
      //m_BoxMesh->draw_instanced();

      //// Bind the plane texture
      ////m_PlaneTexture->Bind();
      ////m_PlaneMesh->Bind();
      ////prenderer->DrawInstanced({ m_PlaneMesh.get() }, m_Shader, m_PlaneInstanceCount);  // Drawing the plane instances

      //// Render the sphere instances
      ////m_SphereTexture->Bind();
      ////m_SphereMesh->Bind();
      ////prenderer->DrawInstanced({ m_SphereMesh.get() }, m_Shader, m_SphereInstanceCount);  // Draw sphere instances


      //// Render the wall
      ////m_WallShader->Bind();
      ////m_WallShader->SetUniformMat4f("view", view);
      ////m_WallShader->SetUniformMat4f("projection", projection);


      ////// Get camera position
      ////floating_sequence3 cameraPosition = pcamera->GetPosition();

      ////m_WallShader->SetUniform3f("cameraPos", cameraPosition.x, cameraPosition.y, cameraPosition.z);

      ////m_Misc->Bind();
      ////m_WallMesh->Bind();
      ////prenderer->DrawInstanced({ m_WallMesh.get() }, m_WallShader, m_WallInstanceCount); // Draw robin

      ////// Unbind the shader
      ////m_Shader->Unbind();
   }





   main_scene::~main_scene() {}



} // namespace graphics3d_opengl





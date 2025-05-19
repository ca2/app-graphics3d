#include "framework.h"
#include "glad.h"
//#include "GLFW/glfw3.h"
#include <vector>
#include <map>
//#include "GLError.h"
#include "renderer.h"
#include "mesh.h"


namespace graphics3d_opengl
{


   renderer::renderer()
   {

      //glEnable(GL_DEPTH_TEST);


      //glDepthFunc(GL_LESS);
   }

   renderer::~renderer()
   {
   }

   void renderer::Clear() const
   {

      // Clear the screen 
      GLCheckError();
      glClearColor(0.678f, 0.847f, 0.902f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      GLCheckError();
   }

   // sky_box
   void renderer::DrawSkybox(const mesh *pskyboxMesh, const shader *pshader) const {
      GLCheckError();
      // Disable depth writing for the skybox
      glDepthFunc(GL_LEQUAL);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      pshader->Bind();

      pskyboxMesh->Bind();
      glDrawElements(GL_TRIANGLES, pskyboxMesh->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
      pskyboxMesh->Unbind();

      glDepthFunc(GL_LESS);
      pshader->Unbind();
      GLCheckError();
   }


   void renderer::DrawInstanced(const std::vector<mesh*>& meshes, const shader *pshader, unsigned int instanceCount) const {
      GLCheckError();

      for (const mesh* mesh : meshes) {
         mesh->Bind();
         glDrawElementsInstanced(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, (void*)(mesh->GetIndexOffset() * sizeof(unsigned int)), instanceCount);
         mesh->Unbind();
      }

      GLCheckError();
   }

   void renderer::DrawModel(const std::vector<mesh*>& mehses, const shader *pshader)
   {

   }


} // namespace graphics3d_opengl






#pragma once


#include "app-cube/cube/graphics3d/renderer.h"
#include "mesh.h"
#include "shader.h"
//#include "GLFW/glfw3.h"
#include <memory>
#include <glm/glm.hpp>


namespace graphics3d_opengl
{


   class renderer :
      virtual public ::graphics3d::renderer
   {
   public:

      //memory m_memory;
      //pixmap m_pixmap;
      renderer();
      ~renderer();


      int width() override;

      int height() override;

      ::pointer < ::graphics3d::frame > beginFrame() override;

      void Clear() const;

      // Draws the skybox using the given mesh and shader
      void DrawSkybox(const mesh *pskyboxMesh, const shader * pshader) const;

      void DrawInstanced(const std::vector<mesh*>& meshes, const shader *pshader, unsigned int instanceCount) const;
      void MultiDrawIndirectCommand() {}

      void DrawModel(const std::vector<mesh*>& mehses, const shader *pshader);

      void endFrame() override;

      //virtual void _sample();

      //virtual void _swap();

      //virtual void on_layout(int cx, int cy);

   private:
      int instanceCount = 0;
   };



} // namespace graphics3d_opengl




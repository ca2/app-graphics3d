#pragma once

//
//#include "cube/graphics3d/renderer.h"
//#include "mesh.h"
//#include "shader.h"
////#include "GLFW/glfw3.h"
//#include <memory>
//#include <glm/glm.hpp>
//
//
//namespace graphics3d_opengl
//{
//
//
//   class renderer :
//      virtual public ::graphics3d::renderer
//   {
//   public:
//
//      //memory m_memory;
//      //pixmap m_pixmap;
//      renderer();
//      ~renderer();
//
//
//      ::pointer < ::graphics3d::frame > beginFrame() override;
//
//      virtual void on_begin_render(::graphics3d::frame* pframe);
//
//      void Clear() const;
//
//      // Draws the skybox using the given mesh and shader
//      void DrawSkybox(const mesh *pskyboxMesh, ::gpu::shader * pshader) const;
//
//      void DrawInstanced(const ::array<mesh*>& meshes, ::gpu::shader *pshader, unsigned int instanceCount) const;
//      void MultiDrawIndirectCommand() {}
//
//      void DrawModel(const ::array<mesh*>& mehses, ::gpu::shader *pshader);
//
//      void endFrame() override;
//
//      virtual void _sample();
//
//      virtual void _swap();
//
//      //virtual void on_layout(int cx, int cy);
//
//   private:
//      int instanceCount = 0;
//   };
//
//
//
//} // namespace graphics3d_opengl
//
//
//

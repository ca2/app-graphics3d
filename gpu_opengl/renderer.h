#pragma once


#include "app-cube/cube/gpu/renderer.h"
//#include "mesh.h"
#include "shader.h"
//#include "GLFW/glfw3.h"
#include <memory>
#include <glm/glm.hpp>


namespace gpu_opengl
{


   class CLASS_DECL_GPU_OPENGL renderer :
      virtual public ::gpu::renderer
   {
   public:

      //memory m_memory;
      //pixmap m_pixmap;
      renderer();
      ~renderer();


      ::pointer < ::gpu::frame > beginFrame() override;

      virtual void on_begin_render(::gpu::frame* pframe);

      void Clear() const;


      int get_frame_count() const override;
      // Draws the skybox using the given mesh and shader
      //void DrawSkybox(const mesh *pskyboxMesh, ::gpu::shader * pshader) const;

      //void DrawInstanced(const ::array<mesh*>& meshes, ::gpu::shader *pshader, unsigned int instanceCount) const;
      void MultiDrawIndirectCommand() {}

      //void DrawModel(const ::array<mesh*>& mehses, ::gpu::shader *pshader);

      void endFrame() override;

      virtual void _sample();

      virtual void _swap();

      //virtual void on_layout(int cx, int cy);

   private:
      int instanceCount = 0;
   };



} // namespace gpu_opengl




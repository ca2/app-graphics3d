// From github:/tristancalderbank/OpenGL-PBR-Renderer/gltf_mesh.h by
// camilo on 2025-09-26 18:28 <3ThomasBorregaardSorensen!!
#pragma once


#include "gpu/model/mesh.h"



namespace gpu_vulkan
{


   namespace gltf
   {
      /**
       * A gltf_mesh is a collection of geometry paired with a material.
       */
      class mesh :
         virtual public ::gpu::model::mesh
      {
      public:

         // OpenGL data structures
         unsigned int m_uVAO, m_uVBO, m_uEBO;

         //array_base<gltf::vertex> m_vertexa;
         // unsigned_int_array m_indexa;
         // ::pointer<gltf_material> m_pmaterial;
         //

         mesh();
         ~mesh() override;


         //void initialize_gpu_gltf_mesh(const ::array_base<::gpu::gltf::vertex> &vertexa,
         //                          const ::unsigned_int_array &indexa, ::gpu::model::material *pmaterial) override;


         //void on_initialize_gpu_mesh() override;

         void draw2(::gpu::command_buffer *pcommandbuffer) override;
         // private:
         //    // OpenGL data structures
         //    unsigned int mVAO, mVBO, mEBO;
      };


   } // namespace gltf


} // namespace gpu_vulkan

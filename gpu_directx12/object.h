// Created by camilo on 2022-04-25 20:42 <3ThomasBorregaardSorensen!!
#pragma once

//#include "acme/prototype/geometry3d/location.h"
#include "bred/gpu/object.h"


namespace gpu_directx12
{


   class CLASS_DECL_GPU_DIRECTX12 object :
      virtual public ::gpu::object
   {
   public:


      //::gpu::element             m_vao_vertexes;
      //::gpu::element             m_vao_normals;

      //::gpu::element             m_vbo_vertexes;
      //::gpu::element             m_vbo_normals;
      //::gpu::element             m_vbo_uvs;

      //::gpu::element             m_ibo_elements;

      //::array<unsigned short>    m_indexes;
      //::array<::floating_sequence3>       m_vertexes;
      //::array<::floating_sequence2>       m_uvs;
      //::array<::floating_sequence3>       m_normals;

      //::gpu::payload             m_payloadTexture;
      //::gpu::element             m_uniformTexture;


      //::file::path               m_path;

      ::pointer < ::gpu::device > m_pgpudevice;



      object();
      ~object() override;


      void initialize_directx12_object(::gpu::device * pgpudevice);


      //void load(const ::file::path & path) override;


      ////void prepare(::gpu::approach * pgpuapproach) override;


      //void _prepare(::gpu::approach * pgpuapproach) override;


      //void draw() override;

      //
      //void set_texture_uniform(const ::gpu::payload & uniformTexture) override;


   };



} // namespace gpu_directx12



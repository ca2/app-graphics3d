// Created by camilo on 2025-06-08 19:59 < 3ThomasBorregaardSørensen!!
#pragma once



namespace gpu_directx12
{


   class CLASS_DECL_GPU_DIRECTX12 depth_stencil :
      virtual public ::particle
   {
   public:


      ::pointer < ::gpu::renderer >    m_prenderer;
      ::comptr<ID3D12DescriptorHeap>   m_pheap;
      ::comptr<ID3D12Resource>         m_presource;
      D3D12_CPU_DESCRIPTOR_HANDLE      m_handle;



      depth_stencil();
      ~depth_stencil() override;


      void initialize_depth_stencil(::gpu::renderer* prenderer, const ::int_size& size);


   };


} // namespace gpu_directx12




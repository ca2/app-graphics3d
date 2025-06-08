// Created by camilo on 2025-06-08 18:11 < 3ThomasBorregaardSørensen!!
#pragma once



namespace gpu_directx12
{


   class CLASS_DECL_GPU_DIRECTX12 texture :
      virtual public ::particle
   {
   public:


      struct
      {

         int m_bForOnAfterDoneFrameStep : 1;

         void set_new_texture()
         {
            memset(this, 0xff, sizeof(*this));

         }


      }new_texture;

      ::pointer < ::gpu::renderer >    m_prenderer;
      ::comptr<ID3D12Resource>         m_presource;
      ::comptr<ID3D12DescriptorHeap>   m_pheapRenderTargetView;
      D3D12_RESOURCE_STATES            m_estate;
      D3D12_CPU_DESCRIPTOR_HANDLE      m_handleRenderTargetView;
      ::comptr<ID3D12DescriptorHeap>   m_pheapShaderResourceView;
      D3D12_CPU_DESCRIPTOR_HANDLE      m_handleShaderResourceView;



      texture();
      ~texture() override;


      void initialize_texture(::gpu::renderer* prenderer, const ::int_size & size, bool bCreateRenderTargetView,  bool bCreateShaderResourceView);


   };


} // namespace gpu_directx12




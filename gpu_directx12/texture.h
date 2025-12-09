// Created by camilo on 2025-06-08 18:11 < 3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/frame_storage.h"
#include "bred/gpu/texture.h"
#include <d3d11.h>
#include <d2d1_1.h>


namespace gpu_directx12
{


   class CLASS_DECL_GPU_DIRECTX12 texture :
      virtual public ::gpu::texture
   {
   public:


      class upload_buffer :
         virtual public ::gpu::frame_storage::object
      {
      public:

         class damage
         {
         public:
            D3D12_TEXTURE_COPY_LOCATION m_copylocationTarget;
            D3D12_TEXTURE_COPY_LOCATION m_copylocationSource;
            CD3DX12_BOX m_boxSource;

            int m_iLeft;
            int m_iTop;

            damage() :
               m_copylocationTarget{},
               m_copylocationSource{},
               m_boxSource{}
            {


            }

            void update_copyable_region(ID3D12GraphicsCommandList* pcommandlist);

         };


         void initialize_upload_buffer(texture* ptexture);

         D3D12_RESOURCE_DESC m_descTexture;

         UINT64 m_uUploadBufferSize = 0;
         UINT64 m_uRowSizeInBytes = 0;
         UINT   m_uNumRows = 0;

         D3D12_PLACED_SUBRESOURCE_FOOTPRINT m_footprint;


         ::array_base < damage > m_damagea;

         ::gpu_directx12::texture* m_ptexture;
         ::comptr<ID3D12Resource>         m_presourceUpload;

         void* m_pMap = nullptr;

         upload_buffer();
         ~upload_buffer();


         void map();
         void unmap();


         void on_end_frame() override;

         virtual void upload_damaged_regions();

         void update_pixels(const ::int_rectangle& rectangle, const void* data);

      };


      struct
      {

         int m_bForOnAfterDoneFrameStep : 1;

         void set_new_texture()
         {
            memset(this, 0xff, sizeof(*this));

         }


      }new_texture;

      
      ::comptr<ID3D12Resource>         m_presource;
      ::comptr<ID3D12Resource>         m_presourceDepthStencilView;
      ::comptr<ID3D12DescriptorHeap>   m_pheapRenderTargetView;
      D3D12_RESOURCE_STATES            m_estate;
      D3D12_CPU_DESCRIPTOR_HANDLE      m_handleRenderTargetView;
      ::comptr<ID3D12DescriptorHeap>   m_pheapShaderResourceView;
      D3D12_CPU_DESCRIPTOR_HANDLE      m_handleShaderResourceView;
      ::comptr<ID3D12DescriptorHeap>   m_pheapDepthStencilView;
      D3D12_CPU_DESCRIPTOR_HANDLE      m_handleDepthStencilView;
      ::comptr<ID3D12DescriptorHeap>   m_pheapSampler;
      D3D12_CPU_DESCRIPTOR_HANDLE      m_handleSampler;
      //UINT m_rtvDescriptorSize;
      //bool m_bRenderTarget;
      bool m_bShaderResource;

      class d3d11 :
         virtual public ::particle
      {
      public:

         ID3D11Resource* m_d3d11wrappedresources[1];

         //::comptr<ID3D12Resource> d3d12Resource;
         ::comptr<ID3D11Resource> wrappedResource;
         ::comptr<IDXGISurface> dxgiSurface;
         ::comptr<ID2D1Bitmap1> d2dBitmap;
         HANDLE sharedHandle = NULL;

         d3d11() {}
         ~d3d11() override {}



      };


      ::pointer < d3d11 > m_pd3d11;

      texture();
      ~texture() override;

      void create_image(const ::pointer_array < ::image::image > * pimagea);
      //void initialize_image_texture(::gpu::renderer* prenderer, const ::int_rectangle & rectangle, bool bWithDepth, const ::pointer_array < ::image::image >& imagea, enum_type etype) override;

      void _new_state(ID3D12GraphicsCommandList* pcommandlist, D3D12_RESOURCE_STATES estate);

      class d3d11* d3d11();

      void blend(::gpu::texture* ptexture) override;


      void create_render_target();

      void create_shader_resource();

      void create_depth_resources();

      virtual void _initialize_gpu_texture(::gpu::renderer* prenderer, UINT uCurrentBufferIndex, IDXGISwapChain3* pdxgiswapchain);


      void set_pixels(const ::int_rectangle& rectangle, const void* data) override;

      virtual upload_buffer * _get_upload_buffer();

   };


   class texture_guard
   {
   public:

      texture* m_ptexture;
      ID3D12GraphicsCommandList* m_pcommandlist;
      D3D12_RESOURCE_STATES m_estateOld;

      texture_guard(ID3D12GraphicsCommandList* pcommandlist, texture* ptexture, D3D12_RESOURCE_STATES estate)
      {

         m_ptexture = ptexture;
         m_pcommandlist = pcommandlist;
         m_estateOld = m_ptexture->m_estate;

         m_ptexture->_new_state(m_pcommandlist, estate);

      }

      ~texture_guard()
      {

         m_ptexture->_new_state(m_pcommandlist, m_estateOld);

      }

   };



} // namespace gpu_directx12




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

         void update_pixels(const ::i32_rectangle& rectangle, const void* data);

      };

      class static_upload_buffer : virtual public ::particle
      {
      public:


         void initialize_static_upload_buffer(texture *ptexture);

         D3D12_RESOURCE_DESC m_descTexture;

         UINT64 m_uUploadBufferSize = 0;
         UINT64 m_uRowSizeInBytes = 0;
         UINT m_uNumRows = 0;
         int m_iResourceCount = -1;

         D3D12_PLACED_SUBRESOURCE_FOOTPRINT m_footprint;


//         ::array_base<damage> m_damagea;

         ::gpu_directx12::texture *m_ptexture;
         ::pointer<d3d12_resource> m_pd3d12resourceUpload;

         //void *m_pMap = nullptr;

         static_upload_buffer();
         ~static_upload_buffer();


         //void map();
         //void unmap();


         //void on_end_frame() override;

         //virtual void upload_damaged_regions();

         void update_with_texture_data(::gpu::command_buffer *pgpucommandbuffer, const ::gpu::texture_data &data);
      };

      struct
      {

         int m_bForOnAfterDoneFrameStep : 1;

         void set_new_texture()
         {
            memset(this, 0xff, sizeof(*this));

         }


      }new_texture;

            struct layer
      {
         /// @brief [0] -> color, [1] -> depth
         //VkImageView m_vkimageviewaAttachment[2] = {VK_NULL_HANDLE, VK_NULL_HANDLE};
         //VkFramebuffer m_vkframebuffer = VK_NULL_HANDLE;

               //UINT width;
         //UINT height;
         //UINT layerCount;

         D3D12_CPU_DESCRIPTOR_HANDLE m_handleRenderTargetView{};
         D3D12_CPU_DESCRIPTOR_HANDLE m_handleDepthStencilView{};
         ::i32_size m_size{-1, -1};
         int m_iLayerCount = -1;
         bool is_empty() const
         {

            return !m_handleRenderTargetView.ptr;
         }

         void create_render_target_view(::gpu_directx12::texture *ptexture);
         void _create_render_target_view(::gpu_directx12::texture *ptexture, int iAttachmentCount);
         //void create_color_attachment(::gpu_vulkan::texture *ptexture);
         //void create_depth_attachment(::gpu_vulkan::texture *ptexture);
      };

            class mip_map_generator :
               virtual public particle
            {

               public:

                  ::comptr<ID3D12RootSignature> m_prootsignatureMipMap;
               ::pointer<texture> m_ptexture;

               ::comptr<ID3D12PipelineState> m_ppipelinestateMipMap;
               ::comptr<ID3D12DescriptorHeap> m_pheapMipMap;
               D3D12_CPU_DESCRIPTOR_HANDLE m_cpuBase;
               D3D12_GPU_DESCRIPTOR_HANDLE m_gpuBase;

               ::array_base< D3D12_GPU_DESCRIPTOR_HANDLE> m_handleaShaderResourceView;
               ::array_base<D3D12_GPU_DESCRIPTOR_HANDLE> m_handleaUnorderedAccessView;
               int m_iMipCount;
               int m_iLayerCount;
               mip_map_generator();
               ~mip_map_generator();

               int idx(UINT mip, UINT face)
               { 
                  return mip * m_iLayerCount + face; 
               
               }


               virtual void initialize_mip_map_generator(texture *ptexture);

               void generate(::gpu::command_buffer * pgpucommandbuffer);

            };

                  class layer_array : virtual public ::array_base<layer>
      {
      public:
      };


      class mip_layer_array : virtual public ::array_base<layer_array>
      {
      public:
      };


      mip_layer_array m_miplayera;

      ::pointer<d3d12_resource> m_pd3d12resourceTexture;
      ::pointer<mip_map_generator> m_pmipmapgenerator;
      ::pointer<static_upload_buffer> m_pstaticuploadbuffer;
      D3D12_RESOURCE_DESC              m_resourcedesc;
      ::comptr<ID3D12Resource>         m_presourceDepthStencilView;
      ::comptr<ID3D12DescriptorHeap>   m_pheapRenderTargetView;
      //D3D12_RESOURCE_STATES            m_estate;
      int m_iRenderTargetViewHandleCount = -1;
      int m_iRenderTargetViewHandle= -1;
      UINT m_uRenderTargetViewIncrement = 0;
      
      ::comptr<ID3D12DescriptorHeap>   m_pheapShaderResourceView;
      D3D12_CPU_DESCRIPTOR_HANDLE      m_handleShaderResourceView;
      ::comptr<ID3D12DescriptorHeap>   m_pheapDepthStencilView;
      D3D12_CPU_DESCRIPTOR_HANDLE      m_handleDepthStencilView;
      ::comptr<ID3D12DescriptorHeap>   m_pheapSampler;
      D3D12_CPU_DESCRIPTOR_HANDLE      m_handleSampler;
      //UINT m_rtvDescriptorSize;
      //bool m_bRenderTarget;
      //bool m_bShaderResource;

      //state_t m_state;

      //DXGI_FORMAT m_dxgiformat;

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

      void _create_texture(const ::gpu::texture_data & data);
      //void initialize_image_texture(::gpu::renderer* prenderer, const ::i32_rectangle & rectangle, bool bWithDepth, const ::pointer_array < ::image::image >& imagea, enum_type etype) override;
      D3D12_CPU_DESCRIPTOR_HANDLE _allocate_render_target_view_handle();

      class d3d11* d3d11();

      //void blend(::gpu::texture* ptexture) override;

      struct texture::layer &current_layer();
      void create_render_target();

      void create_shader_resource();

      void create_depth_resources();

      virtual void _initialize_gpu_texture(::gpu::context * pgpucontext, UINT uCurrentBufferIndex, IDXGISwapChain3* pdxgiswapchain);


      void set_pixels(const ::i32_rectangle& rectangle, const void* data) override;

      virtual upload_buffer * _get_upload_buffer();

      virtual static_upload_buffer *_get_static_upload_buffer(int iResourceCount = -1);

      void initialize_hdr_texture_on_memory(::gpu::context *pgpucontext, const ::block &block) override;


      void set_state(::gpu::command_buffer *pgpucommandbuffer, ::gpu::enum_texture_state etexturestate) override;
      void generate_mipmap(::gpu::command_buffer *pgpucommandbuffer) override;

   };




} // namespace gpu_directx12




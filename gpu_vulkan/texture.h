// From directx2 texture by camilo on 2025-06-16 02:11 <3ThomasBorregaardSørensen!!
// Created by camilo on 2025-06-08 18:11 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/texture.h"
#include "app-graphics3d/gpu_vulkan/descriptors.h"

namespace tinygltf
{

   struct Image;

}
namespace gpu_vulkan
{

   class binding_slot_set;

   class texture_synchronization :
      virtual public ::particle
   {
   public:

      //::gpu_vulkan::render_pass *   m_prenderpass = nullptr;
      //::gpu_vulkan::render_target * m_prendertarget = nullptr;
      ::gpu_vulkan::texture* m_ptexture = nullptr;


      //bool                          m_bAdvancedPipelineSynchronization = false;
      int                           m_iImageAvailable = -1;
      int                           m_iRendering = -1;
      //VkSemaphore                   m_vksemaphoreAvailable = VK_NULL_HANDLE;
      //VkSemaphore                   m_vksemaphoreRenderFinished = VK_NULL_HANDLE;
      //VkFence                       m_vkfenceInFlight2 = VK_NULL_HANDLE;
      //VkFence                       m_vkfenceImageInFlight = VK_NULL_HANDLE;
      ::pointer<::gpu::semaphore> m_pgpusemaphoreAvailable;
      ::pointer<::gpu::semaphore> m_pgpusemaphoreRenderFinished;
      ::pointer<::gpu::fence> m_pgpufenceInFlight;
      ::pointer<::gpu::fence> m_pgpufenceImageInFlight;

      texture_synchronization();
      ~texture_synchronization();

      VkFramebuffer get_frame_buffer(::gpu_vulkan::render_pass * prenderpass);
      VkFramebuffer _get_frame_buffer(::gpu_vulkan::render_pass* prenderpass);

      //VkFence in_flight_fence();

      ::gpu::fence *in_flight_fence();

      //virtual texture_synchronization& synchronization(::gpu::render_target * prendertarget);


   };



   class CLASS_DECL_GPU_VULKAN texture :
      virtual public ::gpu::texture
   {
   //protected:


      //texture* _get_depth_texture();


   public:

      class _001OnAfterEndFrameItem :
         virtual public ::particle
      {
      public:
         ::pointer<texture> m_ptexture;
         ::pointer<::gpu_vulkan::context> m_pcontext;
         ::pointer<buffer> m_pbufferStaging;
         ::i32_rectangle m_rectangle;

      };
      class _001OnAfterEndFrame : virtual public ::particle
      {
      public:
         ::pointer_array<_001OnAfterEndFrameItem> m_itema;
      };
      class _001OnNextFrameStart : virtual public ::particle
      {
      public:
         ::pointer_array<texture> m_texturea;
      };
      ::pointer<_001OnAfterEndFrame> m_p_001OnAfterEndFrame;
      ::pointer<_001OnNextFrameStart> m_p_001OnNextFrameStart;

      struct render_pass_t
      {

         VkFramebuffer m_vkframebuffer = VK_NULL_HANDLE;

      };


      struct
      {

         int m_bForOnAfterDoneFrameStep : 1;

         void set_new_texture()
         {
            memset(this, 0xff, sizeof(*this));

         }


      }new_texture;

      struct state_t
      {
         VkAccessFlags              m_vkaccessflags;
         VkImageLayout              m_vkimagelayout;
         VkPipelineStageFlags       m_vkpipelinestageflags;

         state_t() {}

         state_t(
            VkAccessFlags              vkaccessflags,
            VkImageLayout              vkimagelayout,
            VkPipelineStageFlags       vkpipelinestageflags
         ) :
            m_vkimagelayout(vkimagelayout),
            m_vkaccessflags(vkaccessflags),
            m_vkpipelinestageflags(vkpipelinestageflags)
         {


         }

      };


      class state_array_2d : 
         public ::array_base<::array_base<state_t>>
      {
      public:


         state_array_2d();
         ~state_array_2d();


         void set_state(const state_t & stateNew, int iMip, int iLayer);
         void synchronize_from(const state_array_2d &state2aSynchronize);


         state_t &mip_layer_state(int iMip, int iLayer) { return this->atø(iMip).atø(iLayer); }
         const state_t &mip_layer_state(int iMip, int iLayer) const { return (*this)[iMip][iLayer]; }


      };


      struct layer
      {
         /// @brief [0] -> color, [1] -> depth
         VkImageView m_vkimageviewaAttachment[2] = {VK_NULL_HANDLE, VK_NULL_HANDLE};
         VkFramebuffer m_vkframebuffer = VK_NULL_HANDLE;
         ::i32_size m_size{-1, -1};
         int m_iLayerCount = -1;
         bool is_empty() const
         {

            return m_vkimageviewaAttachment[0] == VK_NULL_HANDLE || m_vkframebuffer == VK_NULL_HANDLE;

         }

         void create_framebuffer(::gpu_vulkan::texture *ptexture, ::gpu_vulkan::render_pass *prenderpass);
         void _create_framebuffer(::gpu_vulkan::texture *ptexture, ::gpu_vulkan::render_pass *prenderpass, int iAttachmentCount);
         void create_color_attachment(::gpu_vulkan::texture *ptexture);
         void create_depth_attachment(::gpu_vulkan::texture *ptexture);

      };

      class layer_array : virtual public ::array_base<layer>
      {
      public:
      };


      class mip_layer_array : virtual public ::array_base<layer_array>
      {
      public:
      };

      class render_pass_mip_layer_array : virtual public map<VkRenderPass, mip_layer_array>
      {
      public:

         texture::layer &layer(::gpu_vulkan::texture *ptexture, ::gpu_vulkan::render_pass *prenderpass);
         //VkFramebuffer _framebuffer(::gpu_vulkan::texture *ptexture, ::gpu_vulkan::render_pass *prenderpass,
         //                                 int iLayer, int iMip);

      };

         

         ///VkImageView m_imageviewa[6] = {};
         ///int m_iImageViewCount = 0;
         //::pointer_array<::gpu_vulkan::render_pass> m_renderpassa;

         //map<::gpu_vulkan::render_pass *, ::array_base < render_pass_t >> m_mapRenderPass;

//         void create_image_views(::gpu_vulkan::texture *ptexture, int iLayer, int iMip);
//         VkImageView get_image_view(::gpu_vulkan::texture * ptexture, int iLayer, int iMip);
//         //VkFramebuffer framebuffer2(::gpu_vulkan::texture *ptexture, ::gpu_vulkan::render_pass *prenderpass, int iMip);
//         //VkFramebuffer _framebuffer2(::gpu_vulkan::texture *ptexture, ::gpu_vulkan::render_pass *prenderpass, int iMip);
//         VkFramebuffer layer_framebuffer(::gpu_vulkan::texture *ptexture, ::gpu_vulkan::render_pass *prenderpass,
//                                         int iLayer, int iMip);
//         VkFramebuffer _layer_framebuffer(::gpu_vulkan::texture *ptexture, ::gpu_vulkan::render_pass *prenderpass, int iLayer, int iMip);
////         ::gpu_vulkan::render_pass * get_render_pass(::gpu_vulkan::texture *ptexture, int iIndex);
//      };

      class scoped_state
      {
      public:
         
         
         ::gpu_vulkan::command_buffer * m_pcommandbuffer;
         texture * m_ptexture;
         state_array_2d m_state2a;


         scoped_state(::gpu_vulkan::command_buffer* pcommandbuffer, texture* ptexture, const state_array_2d & state2a) :
            m_pcommandbuffer(pcommandbuffer),
            m_ptexture(ptexture),
            m_state2a(state2a)

         {
         }


         ~scoped_state()
         {

            m_ptexture->_set_state(m_pcommandbuffer, m_state2a);

         }


      };



      bool                       m_bOwnImage;
      VkImage                    m_vkimage;
      VkFormat                   m_vkformat;
      VkImageUsageFlags          m_vkimageusageflags = 0;
      VkDeviceMemory             m_vkdevicememory;
      state_array_2d             m_state2a;
      state_array_2d             m_state2aExternal;
      //int                        m_iMipCount;
      /// Does every texture needs its own sampler?
      VkSampler                  m_vksampler3;
      //VkDescriptorImageInfo      m_descriptor3;
      VkSampler                  m_vksamplerDedicated;
      //VkImage                    m_vkimageDepth;
      //VkDeviceMemory             m_vkdevicememoryDepth;
      VkImageView                m_vkimageview;
      //VkImageView                m_vkimageviewDepth;
      ::pointer < texture_synchronization >           m_ptexturesynchronization;
      map<VkRenderPass, VkFramebuffer >             m_mapFramebuffer;
      map<::gpu_vulkan::shader *, ::pointer<::gpu_vulkan::descriptor_set_array>> m_mapShaderDescriptorSetArray;
      map<::gpu_vulkan::shader *, ::pointer<::gpu_vulkan::binding_slot_set>> m_mapBindingSlotSet1;
      // map<::gpu_vulkan::render_target*, texture_synchronization > m_mapSynchronization;
      /// indexed by mip
      render_pass_mip_layer_array m_mapRenderPassMipLayer;

      map < ::gpu_vulkan::render_pass *, render_pass_t > m_mapRenderPass;
      ::pointer<::gpu_vulkan::render_pass> m_prenderpass;
      ::pointer<::gpu_vulkan::render_pass> m_prenderpassFace;


      texture();
      ~texture() override;

      void _set_data(const ::gpu::texture_data &data);
      void _create_texture(const ::gpu::texture_data & texturedata) override;

      struct texture::layer &current_layer(::gpu_vulkan::render_pass * prenderpass);
      // void initialize_image_texture(::gpu::renderer* prenderer,
      //    const ::i32_rectangle& rectangleTarget,
      //    bool bWithDepth,
      //    const ::pointer_array < ::image::image > *pimagea = nullptr,
      //    ::gpu::enum_texture etype = e_type_image) override;
      //void initialize_cubemap_image_texture_with_mipmap(::gpu::renderer *pgpurenderer,
      //                                                           const ::i32_rectangle &rectangleTarget, int iMipCount,
      //                                                           bool bRenderTarget, bool bShaderResourceView) override;

      void _set_image_data(const void *p, int w, int h, int channel_count, int bit_count_per_channel, bool bFloat);
      
      void initialize_depth_texture(::gpu::context *pgpucontext, const ::i32_rectangle &rectangleTarget) override;
      void initialize_texture_from_file_path(::gpu::context *pgpucontext, const ::file::path & path, bool bIsSrgb);
      void initialize_hdr_texture_on_memory(::gpu::context *pgpucontext, const ::block &block) override;
      //virtual void load_Cubemap(const ::file::path & path);
      //void blend(::gpu::texture* ptexture, const ::i32_rectangle& rectangleTarget) override;
      //void TransitionImageLayout(
      //   VkImageLayout newLayout,
      //   uint32_t    layerCount);
      void set_state(::gpu::command_buffer *pgpucommandbuffer, ::gpu::enum_texture_state etexturestate) override;


      state_t &mip_layer_state(int iMip, int iLayer) { return m_state2a.mip_layer_state(iMip, iLayer); }
      void _set_state(::gpu_vulkan::command_buffer * pcommandbuffer, 
         state_t state);
      void _set_state(::gpu_vulkan::command_buffer *pcommandbuffer, const state_array_2d & state2a);
      void _set_all_states(::gpu_vulkan::command_buffer *pcommandbuffer, state_t state);
      void _set_state(::gpu_vulkan::command_buffer *pcommandbuffer, state_t state, int iMip, int iLayer);
       scoped_state _scoped_state(::gpu_vulkan::command_buffer* pcommandbuffer,
         state_t state)
      {

         auto state2aRestore = m_state2a;

         _set_state(pcommandbuffer, state);

         return { pcommandbuffer, this, state2aRestore };

      }


      void _attach(VkImage vkimage, ::gpu::enum_texture etexture);
      virtual unsigned int _get_layer_count();
      virtual VkImageViewType _get_image_view_type();

      void destroy();

      virtual void create_image_view();
      virtual void create_sampler();


      void set_all_states(const state_t &state);

      VkImageView get_image();

      VkImageView get_image_view();

      VkSampler get_vk_sampler();

      VkImage get_depth_image();

      VkImageView get_depth_image_view();

      VkDescriptorImageInfo descriptor_info();

      //void __set_state(const state_t &state);

      /// @brief Singular descriptor set (_001 case here), when the texture is the only binding
      /// in the descriptorset
      /// @param pshader 
      /// @param pgpucommandbuffer 
      /// @return 
      VkDescriptorSet _001DescriptorSet(::gpu_vulkan::shader* pshader, ::gpu::command_buffer * pgpucommandbuffer);

      /// @brief Singular binding slot set (_001 case here), when the texture is
      /// the only binding in the descriptor set
      ::gpu::binding_slot_set *_001BindingSlotSet(::gpu_vulkan::shader *psahder,
                                                  ::gpu::command_buffer *pgpucommandbuffer);


      bool _is_ok() const override;

      virtual ::gpu_vulkan::render_pass * get_render_pass();
      //virtual ::gpu_vulkan::render_pass *get_face_render_pass();
      virtual void update_render_pass();
      ///virtual void update_face_render_pass();

      //VkFramebuffer framebuffer2(::gpu_vulkan::render_pass * prenderpass, int iMip = -1);
      //VkFramebuffer _framebuffer2(::gpu_vulkan::render_pass* prenderpass, int iMip = -1);

      //VkFramebuffer layer_framebuffer(::gpu_vulkan::render_pass *prenderpass, int iLayer, int iMip = -1);
      //VkFramebuffer _layer_framebuffer(::gpu_vulkan::render_pass *prenderpass, int iLayer, int iMip = -1);


      // VkFramebuffer create_framebuffer(VkRenderPass renderpass);
      void _LoadCubeMap(const ::pointer_array < ::pixmap >& pixmapa);


      virtual texture_synchronization * synchronization();
      //virtual texture_synchronization* synchronization(::gpu::render_target* prendertarget);
      void set_pixels(const ::i32_rectangle& rectangle, const void* data) override;


      virtual VkDeviceMemory AllocateMemory(VkMemoryRequirements memRequirements, VkMemoryPropertyFlags properties);

      virtual bool CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
                               VkImageUsageFlags usage,
                       VkMemoryPropertyFlags properties, uint32_t arrayLayers, VkImageCreateFlags flags);

      virtual bool imaging_load_from_file(const ::file::path &path);

      virtual bool KTXLoadFromFile(const ::file::path &path, VkQueue copyQueue,
                                    VkImageUsageFlags imageUsageFlags, VkImageLayout imageLayout, bool forceLinear);
      virtual bool KTXLoadFrom_ktxTexture(void * p_ktxTexture, VkQueue copyQueue,
                                   VkImageUsageFlags imageUsageFlags, VkImageLayout imageLayout, bool forceLinear);


      virtual void KtxLoadCubemapFromFile(
         const ::scoped_string &name,
         ::string filename, 
         VkFormat format,
         VkQueue copyQueue, 
         VkImageUsageFlags imageUsageFlags,
         VkImageLayout imageLayout);


      void from_external_state(::gpu::enum_texture_state etexturestate,
                               ::gpu::enum_texture_state etexturestateNow) override;
      void to_external_state(::gpu::command_buffer *pgpucommandbuffer) override;


      //void UpdateDescriptor();

      bool is_in_shader_sampling_state() override;

      //void _fromglTfImage(tinygltf::Image * pgltfimage, const ::file::path & path, ::gpu::renderer * pgpurender,  bool isSrgb);
      void on_finish_load_texture();

      void generate_mipmap(::gpu::command_buffer *pgpucommandbuffer) override;


      static state_t _s_state_from_texture_state(::gpu::enum_texture_state etexturestate);


   };


} // namespace gpu_vulkan




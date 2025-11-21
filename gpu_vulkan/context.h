#pragma once


#include "gpu/context.h"
#include "acme/prototype/prototype/memory.h"
#include "app-graphics3d/gpu_vulkan/device.h"


namespace gpu_vulkan
{


   class CLASS_DECL_GPU_VULKAN context :
      virtual public ::gpu_gpu::context
   {
   public:


      VkSampler                     m_vksampler001;

      VkFormat                      m_formatImageDefault;

    
      bool                          m_bMesa;
   
      ::pointer < device >          m_pgpudevice;

  
      //VkQueue m_vkqueueGraphics;
      //VkQueue m_vkqueuePresent;
      //VkQueue m_vkqueueTransfer3;

      
      VkCommandPool m_vkcommandpoolGraphics;
      VkCommandPool m_vkcommandpoolTransfer;
      VkCommandPool m_vkcommandpoolPresent;


      ::pointer<::gpu_vulkan::descriptor_set_layout>           m_psetdescriptorlayoutGlobal;
      ::pointer<::gpu_vulkan::descriptor_set_layout>           m_psetdescriptorlayoutGltfImage;
      ::pointer<::gpu_vulkan::descriptor_set_layout>           m_psetdescriptorlayoutIbl;
      ::array<VkDescriptorSet>                                 m_descriptorsetsGlobal;
      ::pointer_array<::gpu_vulkan::memory_buffer>					m_uboBuffers;
      ::pointer <::gpu_vulkan::descriptor_pool>                m_pdescriptorpoolGlobal;


      ::pointer <::gpu::shader>                m_pshaderBlend3;

      context();
      ~context() override;


      string _001GetIntroProjection() override;
      string _001GetIntroFragment() override;

      void draw() override;
      void start_drawing() override;
      void global_transform() override;
      void render() override;

      void set_bitmap_1(::image::image *pimage) override;

      VkFormat findDepthFormat();

      VkSampler _001VkSampler();


      //void gpu_debug_message(const ::scoped_string& scopedstr) override;
      void start_debug_happening(::gpu::command_buffer * pgpucommandbuffer, const ::scoped_string& scopedstr) override;

      void on_start_layer(::gpu::layer* player) override;
      void on_end_layer(::gpu::layer *player) override;

      void merge_layers(::gpu::texture* ptextureTarget, ::pointer_array < ::gpu::layer >* playera) override;

      void copy(::gpu::texture* ptextureTarget, ::gpu::texture* ptextureSource) override;

      
      void set_matrix_uniform(const ::gpu::payload & uniformMatrix) override;

      void _001BeginRenderPass(::gpu::command_buffer *pcommandbuffer, ::gpu::texture * pgputexture = nullptr);
      void _001BeginRenderPass(::gpu::command_buffer *pcommandbuffer, ::gpu::ibl::cubemap_framebuffer * pgpucubeframebuffer, int iFace, ::gpu::enum_scene escene);
      void _001EndRenderPass(::gpu::command_buffer *pcommandbuffer);


      void layout_push_constants(::gpu::properties &properties, bool bGlobalUbo) override;


      virtual VkCommandPool
      createCommandPool(uint32_t queueFamilyIndex,
                        VkCommandPoolCreateFlags createFlags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);


      VkCommandPool getGraphicsCommandPool() { return m_vkcommandpoolGraphics; }
      VkCommandPool getTransferCommandPool() { return m_vkcommandpoolTransfer; }
      VkCommandPool getPresentCommandPool() { return m_vkcommandpoolPresent; }



      virtual void _create_context_win32(::gpu::device* pgpudevice, const ::gpu::enum_output& eoutput, ::windowing::window* pwindow, const ::int_size& size);

      void on_create_context(::gpu::device* pgpudevice, const ::gpu::enum_output& eoutput, ::windowing::window* pwindow, const ::int_size& size) override;


      VkDevice logicalDevice();

      virtual void _create_offscreen_window(const ::int_size & size);

      void defer_create_window_context(::windowing::window * pwindow) override;
      void _defer_create_window_context(::windowing::window * pwindow) override;
      virtual void _create_window_context(::windowing::window * pwindow);

      //virtual void _create_window_buffer();
      void _create_cpu_buffer(const ::int_size & size) override;
      void resize_cpu_buffer(const ::int_size & size) override;
      void destroy_cpu_buffer() override;

      //void make_current() override;

      bool is_mesa() override;

      //void render();


      ::memory white_to_color_sampler_vert() override;
      ::memory white_to_color_sampler_frag() override;

      string get_shader_version_text() override;

      void _translate_shader(string_array_base & straFragment) override;

      //void swap_buffers() override;


      //graphics3d_vulkan::context


      //VkCommandPool getCommandPool() { return m_vkcommandpool; }
      //VkDevice logicalDevice() { return m_vkdevice; }


      // Buffer Helper Functions
      ::pointer < buffer > create_buffer(
         VkDeviceSize size,
         VkBufferUsageFlags usage,
         VkMemoryPropertyFlags properties);

      ::pointer < ::gpu::command_buffer > beginSingleTimeCommands(::gpu::queue * pgpuqueue, ::gpu::enum_command_buffer ecommandbuffer = ::gpu::e_command_buffer_graphics) override;
      void endSingleTimeCommands(::gpu::command_buffer * pcommandbuffer);
      void endSingleTimeCommands(command_buffer * pcommandbuffer, int iSubmitCount, VkSubmitInfo * psubmitinfo);
      void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
      void copyBufferToImage(::gpu::command_buffer* pcommandbuffer, ::gpu_vulkan::texture* ptexture, ::gpu_vulkan::buffer* pbuffer, const ::int_rectangle& rectangleSubImage = {});
///      void copyBufferToImage(::gpu::command_buffer* pcommandbuffer, ::gpu::pixmap* pixmap, ::gpu_vulkan::buffer* pbuffer);
      //(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);
      void createImageWithInfo(
         const VkImageCreateInfo & imageInfo,
         VkMemoryPropertyFlags properties,
         VkImage & image,
         VkDeviceMemory & imageMemory);

      VkPhysicalDeviceProperties properties;


      void submitWork(command_buffer * pcommandbuffer, VkQueue queue);




      //public:
      //void _createInstance();
      //void _setupDebugMessenger();
      //void _createSurface();
      //void _pickPhysicalDevice();
      //void _createLogicalDevice();
      //void _createCommandPool();

      //virtual VkCommandBuffer createCommandBuffer(VkCommandBufferLevel level, bool begin = false);
      //virtual VkCommandBuffer createCommandBuffer(VkCommandBufferLevel level, VkCommandPool pool, bool begin = false);

      // /// Ends, submits and frees a one‑time command buffer
      //virtual void flushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue, bool free = true);
      //virtual void flushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue, VkCommandPool pool, bool free = true);



      // helper functions
      //bool isDeviceSuitable(VkPhysicalDevice pvkcdevice);
      //::array<const char *> getRequiredExtensions();
      //bool checkValidationLayerSupport();
      //void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT & createInfo);
      //void hasGflwRequiredInstanceExtensions();
      //bool checkDeviceExtensionSupport(VkPhysicalDevice pvkcdevice);

            //descriptor_set_layout* get_set_descriptor_layout(::gpu::context * pgpucontext);
      virtual VkDescriptorSet getGlobalDescriptorSet(::gpu_vulkan::renderer* prenderer, ::collection::index iFrameIndex = -1);

      ::gpu_vulkan::descriptor_pool* get_global_pool(int iFrameCount);

      //void transitionImageLayout(
      //   VkImage image,
      //   VkFormat format,
      //   VkImageLayout oldLayout,
      //   VkImageLayout newLayout,
      //   uint32_t mizLevels,
      //   uint32_t layerCount);
       
      ////descriptor_set_layout* get_set_descriptor_layout(::gpu::context * pgpucontext);
      //virtual VkDescriptorSet getGlobalDescriptorSet(::gpu::context* pgpucontext, ::gpu_vulkan::renderer* prenderer);

      //::gpu_vulkan::descriptor_pool* get_global_pool(::gpu::context* pgpucontext, int iFrameCount);


      void create_global_ubo(int iSize, int iFrameCount) override;
      void update_global_ubo(const ::block& block) override;


      void engine_on_frame_context_initialization() override;

      void onBeforePreloadGlobalAssets() override;


      //void copy(::gpu::texture* ptexture) override;
      ::memory rectangle_shader_vert() override;
      ::memory rectangle_shader_frag() override;
      bool hasStencilComponent(VkFormat format);
//      void initialize_rectangle_shader(::gpu::shader* pshader) override;


//      ::pointer<::gpu::texture> generate_ibl_prefiltered_env_map(
//         ::gpu::texture *environmentCubeExisting,
//         ::graphics3d::renderable *prenderableSkybox) override;
//
//      /// generate irradianceCube
//      /// @return irradianceCube
//      ::pointer < ::gpu::texture > generate_ibl_irradiance_map(
////         ::gpu::texture * irradianceCube,
//         ::gpu::texture * environmentCube, ::graphics3d::renderable *prenderableSkyboxx) override;

      ::pointer<::gpu::texture> load_cube_map(const ::scoped_string &scopedstrName, const ::file::path &path, bool b32) override;
      ::pointer<::gpu::texture> load_sandbox_texture(const ::scoped_string &scopedstrName,
                                                              const ::file::path &path,
                                                              const ::scoped_string &scopedstrImageFormat) override;

      ::pointer<::gpu::texture> _loadTexture(
         const ::scoped_string &name,
         const ::file::path & path, 
         VkFormat format,
         VkImageUsageFlags usageFlags, 
         VkImageLayout imageLayout);


       ::pointer<::gpu::texture> loadCubemap(
          const ::scoped_string& name,
          const ::scoped_string& ktxFilename,
          VkFormat format,
          VkQueue vkqueueCopy,
          VkImageUsageFlags usageFlags,
          VkImageLayout initialLayout);
  ////    virtual void generateBRDFlut(
  //  //     ::gpu::texture * lutBrdf);
  //    /// generate lutBrdf
  //    /// @return lutBrdf
  //    virtual ::pointer < ::gpu::texture > generateBRDFlut();

      ::pointer<::graphics3d::renderable> _load_gltf_model(const ::gpu::renderable_t & model) override;


      
            
      virtual void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout,
                                         VkImageLayout newLayout, uint32_t mizLevels, uint32_t layerCount);

   void load_generic_texture(::pointer<::gpu::texture> &ptexture, const ::file::path &path,
                                int iAssimpTextureType) override;
      



   };


} // namespace gpu




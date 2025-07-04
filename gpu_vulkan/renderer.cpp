#include "framework.h"
#include "accumulation_render_pass.h"
#include "approach.h"
#include "command_buffer.h"
#include "descriptors.h"
#include "frame.h"
#include "initializers.h"
#include "input_layout.h"
#include "layer.h"
#include "model_buffer.h"
#include "renderer.h"
#include "offscreen_render_pass.h"
#include "physical_device.h"
#include "swap_chain.h"
#include "texture.h"
#include "bred/gpu/cpu_buffer.h"
#include "bred/gpu/render_state.h"
#include "app-graphics3d/gpu_vulkan/shader.h"
#include "acme/parallelization/synchronous_lock.h"
#include "acme/platform/application.h"
#include "aura/graphics/image/target.h"
#include "aura/user/user/interaction.h"
#include "aura/windowing/window.h"
#include "bred/gpu/types.h"
#include "bred/graphics3d/types.h"
//#include "tools.h"
//#include "bred/user/user/graphics3d.h"

//#include <array>
//#include <cassert>
//#include <stdexcept>
//

using namespace vulkan;


namespace gpu_vulkan
{

   static unsigned int g_blend2_vertex[] = {
#include "shader/blend2.vert.spv.inl"
   };


   static unsigned int g_blend2_fragment[] = {
 #include "shader/blend2.frag.spv.inl"
   };


   // Create vertex and index buffers
   void create_quad_buffers(VkDevice device, VkPhysicalDevice physicalDevice,
      VkBuffer* vertexBuffer, VkDeviceMemory* vertexMemory,
      VkBuffer* indexBuffer, VkDeviceMemory* indexMemory, bool bYSwap);


   // Fragment shader (GLSL -> SPIR-V):
   // layout(set = 0, binding = 0) uniform sampler2D srcImage;
   // layout(location = 0) in vec2 fragUV;
   // layout(location = 0) out vec4 outColor;
   // void main() {
   //     outColor = texture(srcImage, fragUV);
   // }
   static unsigned int g_uaImageBlendFragmentShader[] = {
 #include "shader/image_blend.frag.spv.inl"
   };
   // 
   // Vertex shader (GLSL -> SPIR-V):
   // layout(location = 0) in vec2 inPos;
   // layout(location = 1) in vec2 inUV;
   // layout(location = 0) out vec2 fragUV;
   // void main() {
   //     fragUV = inUV;
   //     gl_Position = vec4(inPos, 0.0, 1.0);
   // }
   static unsigned int g_uaImageBlendVertexShader[] = {
 #include "shader/image_blend.vert.spv.inl"
   };


   static unsigned int g_uaResolveFragmentShader[] = {
 #include "shader/resolve.frag.spv.inl"
   };


   // renderer::renderer(VkWindow& window, context* pvkcdevice) : vkcWindow{ window }, m_pgpucontext{ pvkcdevice } 
   renderer::renderer()
   {

   }


   //int renderer::width()
   //{

   //   return pgpurenderpass->width();

   //}

   //int renderer::height()
   //{

   //   return pgpurenderpass->height();

   //}





   void renderer::initialize_gpu_renderer(::gpu::context* pgpucontext)
   {

      ::gpu::renderer::initialize_gpu_renderer(pgpucontext);

      m_pgpucontext = pgpucontext;

      if (m_pgpucontext->m_eoutput == ::gpu::e_output_cpu_buffer)
      {

         //m_pimpact = pgpucontext->m_pimpact;

         pgpucontext->create_cpu_buffer(pgpucontext->rectangle().size());

         __construct_new(m_pcpubuffersampler);

         m_pcpubuffersampler->initialize_cpu_buffer_sampler(pgpucontext);

         m_pcpubuffersampler->m_prenderer = this;

      }

      //m_poffscreensampler->set_storing_flag

      //defer_layout();

   }


   renderer::~renderer()
   {

      free_command_buffers();

   }


   ::gpu::command_buffer* renderer::getCurrentCommandBuffer2()
   {

      return ::gpu::renderer::getCurrentCommandBuffer2();

      //assert(isFrameStarted && "Cannot get command buffer when frame not in progress");

      //if (m_commandbuffera.is_empty())
      //{

      //   create_command_buffers();

      //}

      //auto pcommandbuffer = m_commandbuffera[m_pgpurendertarget->get_frame_index()];

      //return pcommandbuffer;

   }


   //int renderer::get_frame_index()
   //{

   //   assert(m_iFrameSerial2 >= 0
   //      && m_iCurrentFrame2 >= 0
   //      && m_estate != e_state_initial
   //      && "Cannot get frame index when frame not in progress");

   //   return (int)m_iCurrentFrame2;

   //}


   void renderer::on_new_frame()
   {

      ::gpu::renderer::on_new_frame();

      //m_iFrameSerial2++;

      //m_iCurrentFrame2 = (m_iCurrentFrame2 + 1) % render_pass::MAX_FRAMES_IN_FLIGHT;

      //on_happening(e_happening_new_frame);

   }


   //void renderer::restart_frame_counter()
   //{

   //   m_iCurrentFrame2 = -1;
   //   m_iFrameSerial2 = -1;

   //   on_happening(e_happening_reset_frame_counter);

   //}



   //int renderer::get_frame_count()
   //{

   //   return m_pgpurendertarget->m_texturea.size();

   //}


   void renderer::on_context_resize()
   {

      //m_pgpucontext->m_rectangle = rectanglePlacement.size();

      //::gpu::renderer::set_placement(rectanglePlacement);

      //auto size = m_pimpact->size();

      defer_update_renderer();

      //auto extent = vkcWindow.getExtent();
      //while (extent.width == 0 || extent.height == 0) {
      //	glfwWaitEvents();
      //}
      //vkDeviceWaitIdle(m_pgpucontext->logicalDevice());

      //if (vkcSwapChain == nullptr) {
      //	vkcSwapChain = std::make_unique<swap_chain_render_pass>(m_pgpucontext, extent);
      //}
      //else {
      //	::pointer<swap_chain_render_pass> oldSwapChain = std::move(vkcSwapChain);
      //	vkcSwapChain = std::make_unique<swap_chain_render_pass>(m_pgpucontext, extent, oldSwapChain);
      //	if (!oldSwapChain->compareSwapFormats(*vkcSwapChain.get())) {
      //		throw ::exception(error_failed, "Swap chain image(or depth) format has changed!");
      //	}

      //}
   }


   //void renderer::on_defer_update_renderer_allocate_render_target(::gpu::enum_output eoutput, const ::int_size& size, ::gpu::render_target* previous)
   //{

   //   ::cast < context > pcontext = m_pgpucontext;

   //   if (eoutput == ::gpu::e_output_cpu_buffer
   //      || eoutput == ::gpu::e_output_gpu_buffer)
   //   {

   //      auto poffscreenrendertargetview = __allocate offscreen_render_pass();
   //      poffscreenrendertargetview->m_formatImage = pcontext->m_formatImageDefault;
   //      m_pgpurendertarget = poffscreenrendertargetview;

   //   }
   //   else if (eoutput == ::gpu::e_output_swap_chain)
   //   {

   //      ::cast < ::gpu_vulkan::swap_chain > pswapchain = m_pgpucontext->m_pgpudevice->get_swap_chain();
   //      pswapchain->m_formatImage = pcontext->m_formatImageDefault;
   //      m_pgpurendertarget = pswapchain.m_p;

   //   }
   //   else if (eoutput == ::gpu::e_output_swap_chain)
   //   {
   //    
   //      auto poffscreenrendertargetview = __allocate offscreen_render_pass();
   //      poffscreenrendertargetview->m_formatImage = pcontext->m_formatImageDefault;
   //      m_pgpurendertarget = poffscreenrendertargetview;

   //   }
   //   else if (eoutput == ::gpu::e_output_color_and_alpha_accumulation_buffers)
   //   {

   //      auto paccumulationrenderpass = __allocate accumulation_render_pass();
   //      paccumulationrenderpass->m_formatImage = VK_FORMAT_R32G32B32A32_SFLOAT;
   //      paccumulationrenderpass->m_formatAlphaAccumulation = VK_FORMAT_R32_SFLOAT;
   //      m_pgpurendertarget = paccumulationrenderpass;

   //   }
   //   else if (eoutput == ::gpu::e_output_resolve_color_and_alpha_accumulation_buffers)
   //   {

   //      auto poffscreenrenderpass = __allocate offscreen_render_pass();
   //      poffscreenrenderpass->m_formatImage = pcontext->m_formatImageDefault;
   //      m_pgpurendertarget = poffscreenrenderpass;

   //   }
   //   else
   //   {

   //      throw ::exception(error_wrong_state, "Unexpected gpu e_output");

   //   }

   //}



   //   void renderer::defer_update_renderer()
   //   {
   //
   //      auto rectangle = m_pgpucontext->rectangle();
   //
   //      if (m_extentRenderer.width == rectangle.width()
   //         && m_extentRenderer.height == rectangle.height())
   //      {
   //
   //         return;
   //
   //      }
   //
   //      m_bNeedToRecreateSwapChain = true;
   //
   //      m_extentRenderer.width = m_pgpucontext->rectangle().width();
   //
   //      m_extentRenderer.height = m_pgpucontext->rectangle().height();
   //
   //      auto eoutput = m_pgpucontext->m_eoutput;
   //
   //      if (eoutput == ::gpu::e_output_cpu_buffer)
   //      {
   //
   //         auto poffscreenrenderpass = __allocate offscreen_render_pass();
   //#ifdef WINDOWS_DESKTOP
   //         poffscreenrenderpass->m_formatImage = VK_FORMAT_B8G8R8A8_UNORM;
   //#else
   //         poffscreenrenderpass->m_formatImage = VK_FORMAT_R8G8B8A8_UNORM;
   //#endif
   //         m_pgpurendertarget = poffscreenrenderpass;
   //         //m_prendererResolve.release();
   //
   //      }
   //      else if (eoutput == ::gpu::e_output_swap_chain)
   //      {
   //
   //         ::cast < ::gpu_vulkan::swap_chain > pswapchain = m_pgpucontext->m_pgpudevice->get_swap_chain();
   //#ifdef WINDOWS_DESKTOP
   //         pswapchain->m_formatImage = VK_FORMAT_B8G8R8A8_UNORM;
   //#else
   //         pswapchain->m_formatImage = VK_FORMAT_R8G8B8A8_UNORM;
   //#endif
   //
   //         m_pgpurendertarget = pswapchain.m_p;
   //         //pgpurenderpass = __allocate swap_chain_render_pass(this, m_extentRenderer, pgpurenderpass);
   //         //m_prendererResolve.release();
   //
   //      }
   //      else if (eoutput == ::gpu::e_output_gpu_buffer)
   //      {
   //
   //         auto poffscreenrenderpass = __allocate offscreen_render_pass();
   //#ifdef WINDOWS_DESKTOP
   //         poffscreenrenderpass->m_formatImage = VK_FORMAT_B8G8R8A8_UNORM;
   //#else
   //         poffscreenrenderpass->m_formatImage = VK_FORMAT_R8G8B8A8_UNORM;
   //#endif
   //         m_pgpurendertarget = poffscreenrenderpass;
   //         //m_prendererResolve;
   //
   //      }
   //      else if (eoutput == ::gpu::e_output_color_and_alpha_accumulation_buffers)
   //      {
   //
   //         auto paccumulationrenderpass = __allocate accumulation_render_pass();
   //         paccumulationrenderpass->m_formatImage = VK_FORMAT_R32G32B32A32_SFLOAT;
   //         paccumulationrenderpass->m_formatAlphaAccumulation = VK_FORMAT_R32_SFLOAT;
   //         m_pgpurendertarget = paccumulationrenderpass;
   //
   //         //__construct_new(m_prendererResolve);
   //
   //         //m_prendererResolve->initialize_renderer(m_pgpucontext, ::gpu::e_output_resolve_color_and_alpha_accumulation_buffers);
   //
   //         //m_prendererResolve->set_placement(m_pgpucontext->rectangle);
   //         //
   //         //            auto poffscreenrenderpass = __allocate offscreen_render_pass(m_pgpucontext, m_extentRenderer, pgpurenderpassResolve);
   //         //#ifdef WINDOWS_DESKTOP
   //         //            poffscreenrenderpass->m_formatImage = VK_FORMAT_B8G8R8A8_UNORM;
   //         //#else
   //         //            poffscreenrenderpass->m_formatImage = VK_FORMAT_R8G8B8A8_UNORM;
   //         //#endif
   //         //            pgpurenderpassResolve = poffscreenrenderpass;
   //      }
   //      else if (eoutput == ::gpu::e_output_resolve_color_and_alpha_accumulation_buffers)
   //      {
   //
   //         auto poffscreenrenderpass = __allocate offscreen_render_pass();
   //#ifdef WINDOWS_DESKTOP
   //         poffscreenrenderpass->m_formatImage = VK_FORMAT_B8G8R8A8_UNORM;
   //#else
   //         poffscreenrenderpass->m_formatImage = VK_FORMAT_R8G8B8A8_UNORM;
   //#endif
   //         m_pgpurendertarget = poffscreenrenderpass;
   //
   //      }
   //      else
   //      {
   //
   //         throw ::exception(error_wrong_state, "Unexpected gpu e_output");
   //
   //      }
   //
   //      ::cast < render_pass > pgpurenderpass = m_pgpurendertarget;
   //
   //      if (pgpurenderpass->m_texturea.is_empty())
   //      {
   //
   //         ::int_size size;
   //
   //         size.cx() = m_extentRenderer.width;
   //         size.cy() = m_extentRenderer.height;
   //
   //         pgpurenderpass->initialize_render_target(this, size, pgpurenderpass.m_p);
   //
   //         pgpurenderpass->init();
   //
   //         create_command_buffers();
   //
   //      }
   //
   //      //if (m_prendererResolve)
   //      //{
   //
   //      //	if (m_prendererResolve->pgpurenderpass->m_images.is_empty())
   //      //	{
   //
   //      //		m_prendererResolve->defer_update_render_pass();
   //
   //      //	}
   //
   //      //}
   //
   //      m_bNeedToRecreateSwapChain = false;
   //
   //   }


   //   ::gpu::render_target* renderer::back_buffer_render_target()
   //   {
   //
   //      auto rectangle = m_pgpucontext->rectangle();
   //
   //      if (m_pgpurendertargetBackBuffer)
   //      {
   //
   //         auto size = m_pgpurendertargetBackBuffer->m_size;
   //
   //         auto sizeNew = rectangle.size();
   //
   //         if (size == sizeNew)
   //         {
   //
   //            return m_pgpurendertargetBackBuffer;
   //
   //         }
   //
   //      }
   //
   //      auto poffscreenrenderpass = __allocate offscreen_render_pass();
   //#ifdef WINDOWS_DESKTOP
   //      poffscreenrenderpass->m_formatImage = VK_FORMAT_B8G8R8A8_UNORM;
   //#else
   //      poffscreenrenderpass->m_formatImage = VK_FORMAT_R8G8B8A8_UNORM;
   //#endif
   //      poffscreenrenderpass->m_bBackBuffer = true;
   //      m_pgpurendertargetBackBuffer = poffscreenrenderpass;
   //
   //      ::cast < render_pass > pgpurenderpass = m_pgpurendertargetBackBuffer;
   //
   //      if (pgpurenderpass->m_texturea.is_empty())
   //      {
   //
   //         ::int_size size = rectangle.size();
   //
   //         pgpurenderpass->initialize_render_target(this, size, pgpurenderpass.m_p);
   //
   //         pgpurenderpass->init();
   //
   //      }
   //
   //      //if (m_prendererResolve)
   //      //{
   //
   //      //	if (m_prendererResolve->pgpurenderpass->m_images.is_empty())
   //      //	{
   //
   //      //		m_prendererResolve->defer_update_render_pass();
   //
   //      //	}
   //
   //      //}
   //
   //      m_bNeedToRecreateSwapChain = false;
   //
   //      return m_pgpurendertargetBackBuffer;
   //
   //   }


//   ::pointer < ::gpu::render_target > renderer::allocate_offscreen_render_target()
//   {
//
//      auto poffscreenrenderpass = __allocate offscreen_render_pass();
//
//#ifdef WINDOWS_DESKTOP
//      poffscreenrenderpass->m_formatImage = VK_FORMAT_B8G8R8A8_UNORM;
//#else
//      poffscreenrenderpass->m_formatImage = VK_FORMAT_R8G8B8A8_UNORM;
//#endif
//
//      return poffscreenrenderpass;
//
//   }


   void renderer::create_command_buffers()
   {

      m_commandbuffera.set_size(m_pgpurendertarget->get_frame_count());

      //::array<VkCommandBuffer > a;

      //a.set_size(m_commandbuffera.size());

      //VkCommandBufferAllocateInfo allocInfo{};
      //allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      //allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      //allocInfo.commandPool = m_pgpucontext->m_pgpudevice->getCommandPool();
      //allocInfo.commandBufferCount = (uint32_t) a.size();

      //if (vkAllocateCommandBuffers(
      //   m_pgpucontext->logicalDevice(),
      //   &allocInfo, 
      //   a.data()
      //) !=  VK_SUCCESS) 
      //{

      //   throw ::exception(error_failed, "failed to allocate command buffers!");

      //}

      for (int i = 0; i < m_commandbuffera.size(); i++)
      {

         auto& pcommandbuffer = m_commandbuffera[i];

         __defer_construct(pcommandbuffer);

         pcommandbuffer->initialize_command_buffer(m_pgpurendertarget);

      }

   }


   void renderer::free_command_buffers()
   {

      ::gpu::renderer::free_command_buffers();

      //m_commandbuffera.clear();

      //::array<VkCommandBuffer > a;

      //a.set_size(m_commandbuffera.size());

      //for (int i = 0; i < a.size(); i++)
      //{

      //   a[i] = m_commandbuffera[i]->m_vkcommandbuffer;

      //}

      //vkFreeCommandBuffers(
      //   m_pgpucontext->logicalDevice(),
      //   m_pgpucontext->m_pgpudevice->getCommandPool(),
      //   (uint32_t)a.size(),
      //   a.data());
      //a.clear();

   }


   void renderer::on_begin_draw()
   {

      if (m_pgpucontext->m_rectangle.is_empty())
      {

         throw ::exception(error_wrong_state, "please call set size before at least once with no empty preferrably good initial size");

         //set_size(size);

      }

      assert(!isFrameStarted && "Can't call beginFrame while already in progress");

      //if (m_bOffScreen)
      {

         ::cast < render_pass > pgpurenderpass = m_pgpurendertarget;

         auto result = pgpurenderpass->acquireNextImage();

         if (result == VK_ERROR_OUT_OF_DATE_KHR
            || pgpurenderpass->m_bNeedRebuild)
         {
            vkDeviceWaitIdle(m_pgpucontext->logicalDevice());
            pgpurenderpass->init();
            //set_placement(size);
            //throw ::exception(todo, "resize?!?!");
            //return nullptr;
            return;
         }
         if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw ::exception(error_failed, "Failed to aquire swap chain image");
         }

         isFrameStarted = true;

         ::cast < command_buffer > pcommandbuffer = getCurrentCommandBuffer2();

         pcommandbuffer->begin_command_buffer(false);

         //VkCommandBufferBeginInfo beginInfo{};
         //beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

         //if (vkBeginCommandBuffer(
         //   pcommandbuffer->m_vkcommandbuffer, 
         //   &beginInfo) != VK_SUCCESS) 
         //{

         //   throw ::exception(error_failed, "failed to begin recording command buffer!");

         //}

         //m_
         //auto pframe = __create_new < frame >();
         //pframe->m_pcommandbuffer->m_vkcommandbuffer = pcommandbuffer->m_vkcommandbuffer;
         //m_pframe = pframe;
         //return m_pframe;

      }
      //else
      //{


      //	auto result = m_pvkcswapchain->acquireNextImage(&m_uCurrentSwapChainImage);

      //	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      //		recreateRenderPass();
      //		return nullptr;
      //	}
      //	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      //		throw ::exception(error_failed, "Failed to aquire swap chain image");
      //	}

      //	isFrameStarted = true;

      //	auto pcommandbuffer = getCurrentCommandBuffer();

      //	VkCommandBufferBeginInfo beginInfo{};
      //	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      //	if (vkBeginCommandBuffer(pcommandbuffer->m_vkcommandbuffer, &beginInfo) != VK_SUCCESS) {
      //		throw ::exception(error_failed, "failed to begin recording command buffer!");
      //	}
      //	return pcommandbuffer->m_vkcommandbuffer;

      //}

      _on_begin_render();

   }


   renderer::cpu_buffer_sampler::cpu_buffer_sampler()
   {

      clear();

   }


   renderer::cpu_buffer_sampler::~cpu_buffer_sampler()
   {

      destroy();

   }


   void renderer::cpu_buffer_sampler::initialize_cpu_buffer_sampler(::gpu::context* pgpucontext)
   {

      m_pcontext = pgpucontext;

   }




   void renderer::cpu_buffer_sampler::clear()
   {

      //m_vkextent2d.width = 0;
      //m_vkextent2d.height = 0;
      //m_vkdevicememory = nullptr;
      //m_vkimage = nullptr;

      m_texturea.clear();

   }


   void renderer::cpu_buffer_sampler::update(const ::int_size& size)
   {

      auto& ptexture = m_texturea.element_at_grow(m_prenderer->m_pgpurendertarget->get_frame_index());

      if (ptexture &&
         ptexture->m_rectangleTarget.size() == size)
      {

         return;

      }

      ptexture.release();

      if (size.is_empty())
      {

         return;

      }

      __defer_construct_new(ptexture);

      ptexture->m_bTransferDst = true;
      ptexture->m_bCpuRead = true;

      ptexture->initialize_image_texture(m_pcontext->get_gpu_renderer(),
         ::int_rectangle(size), 
         m_prenderer->m_pgpurendertarget->m_bWithDepth);

      //m_vkextent2d.width = vkextent2d.width;
      //m_vkextent2d.height = vkextent2d.height;

      //VkImageCreateInfo imgCreateInfo(initializers::imageCreateInfo());
      //imgCreateInfo.imageType = VK_IMAGE_TYPE_2D;
      //imgCreateInfo.format = VK_FORMAT_B8G8R8A8_UNORM;
      //imgCreateInfo.extent.width = m_vkextent2d.width;
      //imgCreateInfo.extent.height = m_vkextent2d.height;
      //imgCreateInfo.extent.depth = 1;
      //imgCreateInfo.arrayLayers = 1;
      //imgCreateInfo.mipLevels = 1;
      //imgCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      //imgCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
      //imgCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
      //imgCreateInfo.usage =
      //   VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
      //   VK_IMAGE_USAGE_SAMPLED_BIT;
      //// Create the image
      ////VkImage dstImage;
      //VK_CHECK_RESULT(vkCreateImage(m_pgpucontext->logicalDevice(), &imgCreateInfo, nullptr, &m_vkimage));
      //// Create memory to back up the image
      //VkMemoryRequirements memRequirements;
      //VkMemoryAllocateInfo memAllocInfo(initializers::memoryAllocateInfo());
      //vkGetImageMemoryRequirements(m_pgpucontext->logicalDevice(), m_vkimage, &memRequirements);
      //memAllocInfo.allocationSize = memRequirements.size;
      //// Memory must be host visible to copy from
      //memAllocInfo.memoryTypeIndex = m_pgpucontext->m_pgpudevice->m_pphysicaldevice->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
      //VK_CHECK_RESULT(vkAllocateMemory(m_pgpucontext->logicalDevice(), &memAllocInfo, nullptr, &m_vkdevicememory));
      //VK_CHECK_RESULT(vkBindImageMemory(m_pgpucontext->logicalDevice(), m_vkimage, m_vkdevicememory, 0));

      //if (1)
      //{

      //   auto pcommandbuffer = m_pgpucontext->beginSingleTimeCommands();

      //   VkImageMemoryBarrier barrier = {
      //      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      //      .srcAccessMask = 0,
      //      .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      //      .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      //      .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //      .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //      .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //      .image = m_vkimage,
      //      .subresourceRange = {
      //         .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //         .baseMipLevel = 0,
      //         .levelCount = 1,
      //         .baseArrayLayer = 0,
      //         .layerCount = 1
      //      },
      //   };

      //   vkCmdPipelineBarrier(
      //      pcommandbuffer->m_vkcommandbuffer,
      //      VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
      //      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      //      0,
      //      0, NULL,
      //      0, NULL,
      //      1, &barrier
      //   );

      //   m_pgpucontext->endSingleTimeCommands(pcommandbuffer);

      //}


   }


   void renderer::cpu_buffer_sampler::destroy()
   {

      //if (m_vkdevicememory)
      //{

      //   vkFreeMemory(m_pgpucontext->logicalDevice(), m_vkdevicememory, nullptr);
      //   vkDestroyImage(m_pgpucontext->logicalDevice(), m_vkimage, nullptr);

      clear();

      //}

   }


   void renderer::cpu_buffer_sampler::sample(::gpu::texture* pgputexture)
   {

      auto iFrameIndex = m_prenderer->m_pgpurendertarget->get_frame_index();

      auto& ptextureRef = m_texturea.element_at_grow(iFrameIndex);

      if (!ptextureRef)
      {

         return;

      }

      ::cast < texture > ptexture = pgputexture;

      auto pcommandbuffer = m_pcontext->beginSingleTimeCommands();
      // Create the linear tiled destination image to copy to and to read the memory from

  //// Do the actual blit from the offscreen image to our host visible destination image
  //    VkCommandBufferAllocateInfo cmdBufAllocateInfo = initializers::commandBufferAllocateInfo(m_pgpucontext->m_pgpudevice->getCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);
  //    VkCommandBuffer copyCmd;
  //    VK_CHECK_RESULT(vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &cmdBufAllocateInfo, &copyCmd));
  //    VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
  //    VK_CHECK_RESULT(vkBeginCommandBuffer(copyCmd, &cmdBufInfo));
      //ptexture->_new_state(pcommandbuffer, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
      VkPipelineStageFlags vkpipelinestageflagsWait = ptexture->m_vkpipelinestageflags;
      {
         //VkImageMemoryBarrier barrier = {
         //   .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
         //   .srcAccessMask = ptexture->m_vkaccessflags,
         //   .dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT,
         //   .oldLayout = ptexture->m_vkimagelayout,
         //   .newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
         //  .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
         //  .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
         //  .image = ptexture->m_vkimage,
         //  .subresourceRange = {
         //      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
         //      .baseMipLevel = 0,
         //      .levelCount = 1,
         //      .baseArrayLayer = 0,
         //      .layerCount = 1
         //  }
         //};



         //vkCmdPipelineBarrier(
         //   pcommandbuffer->m_vkcommandbuffer,
         //   ptexture->m_vkpipelinestageflags,  // ✅ srcStageMask
         //   VK_PIPELINE_STAGE_TRANSFER_BIT,                 // or SHADER_BIT, depending on next usage         0,
         //   0,
         //   0, NULL,
         //   0, NULL,
         //   1, &barrier
         //);

         //ptexture->m_vkaccessflags = VK_ACCESS_TRANSFER_READ_BIT;
         //ptexture->m_vkimagelayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
         //ptexture->m_vkpipelinestageflags = VK_PIPELINE_STAGE_TRANSFER_BIT;

         ptexture->_new_state(pcommandbuffer,
            VK_ACCESS_TRANSFER_READ_BIT,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            VK_PIPELINE_STAGE_TRANSFER_BIT);
      }

      {
         ////m_ptexture->_new_state(pcommandbuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
         //   VkImageMemoryBarrier barrier = {
         //     .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
         //     .srcAccessMask = m_ptexture->m_vkaccessflags,
         //     .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
         //     .oldLayout = m_ptexture->m_vkimagelayout,
         //     .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
         //    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
         //    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
         //    .image = m_ptexture->m_vkimage,
         //    .subresourceRange = {
         //        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
         //        .baseMipLevel = 0,
         //        .levelCount = 1,
         //        .baseArrayLayer = 0,
         //        .layerCount = 1
         //    }
         //   };

         //   
         //   vkCmdPipelineBarrier(
         //      pcommandbuffer->m_vkcommandbuffer,
         //      m_ptexture->m_vkpipelinestageflags,  // ✅ srcStageMask
         //      VK_PIPELINE_STAGE_TRANSFER_BIT,                 // or SHADER_BIT, depending on next usage         0,
         //      0,
         //      0, NULL,
         //      0, NULL,
         //      1, &barrier
         //   );

         //   m_ptexture->m_vkaccessflags = VK_ACCESS_TRANSFER_WRITE_BIT;
         //   m_ptexture->m_vkimagelayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
         //   m_ptexture->m_vkpipelinestageflags = VK_PIPELINE_STAGE_TRANSFER_BIT;
         ptextureRef->_new_state(pcommandbuffer,
            VK_ACCESS_TRANSFER_WRITE_BIT,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_PIPELINE_STAGE_TRANSFER_BIT);
      }


      //// Transition source image to transfer destination layout
      //insertImageMemoryBarrier(
      //   copyCmd,
      //   vkimage,
      //   0,
      //   VK_ACCESS_TRANSFER_WRITE_BIT,
      //   VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //   VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
      //   VK_PIPELINE_STAGE_TRANSFER_BIT,
      //   VK_PIPELINE_STAGE_TRANSFER_BIT,
      //   VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

      //// Transition destination image to transfer destination layout
      //insertImageMemoryBarrier(
      //   copyCmd,
      //   m_vkimage,
      //   0,
      //   VK_ACCESS_TRANSFER_WRITE_BIT,
      //   VK_IMAGE_LAYOUT_UNDEFINED,
      //   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      //   VK_PIPELINE_STAGE_TRANSFER_BIT,
      //   VK_PIPELINE_STAGE_TRANSFER_BIT,
      //   VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

      // colorAttachment.image is already in VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, and does not need to be transitioned
      {

         VkClearColorValue clearColor = {
             .float32 = { 0.5f, 0.0f, 0.5f, 0.5f }  // R, G, B, A
         };

         VkImageSubresourceRange subresourceRange = {
             .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
             .baseMipLevel = 0,
             .levelCount = 1,
             .baseArrayLayer = 0,
             .layerCount = 1,
         };

         // Image must already be in VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
         vkCmdClearColorImage(
            pcommandbuffer->m_vkcommandbuffer,
            ptextureRef->m_vkimage,
            ptextureRef->m_vkimagelayout,
            &clearColor,
            1,
            &subresourceRange
         );

      }

      VkImageCopy imageCopyRegion{};
      imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      imageCopyRegion.srcSubresource.layerCount = 1;
      imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      imageCopyRegion.dstSubresource.layerCount = 1;
      imageCopyRegion.extent.width = ptexture->m_rectangleTarget.width();
      imageCopyRegion.extent.height = ptexture->m_rectangleTarget.height();
      imageCopyRegion.extent.depth = 1;

      vkCmdCopyImage(
         pcommandbuffer->m_vkcommandbuffer,
         //colorAttachment.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
         ptexture->m_vkimage, ptexture->m_vkimagelayout,
         ptextureRef->m_vkimage, ptextureRef->m_vkimagelayout,
         1,
         &imageCopyRegion);

      //{

      //   VkClearColorValue clearColor = {
      //       .float32 = { 0.5f, 0.5f, 0.0f, 0.5f }  // R, G, B, A
      //   };

      //   VkImageSubresourceRange subresourceRange = {
      //       .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //       .baseMipLevel = 0,
      //       .levelCount = 1,
      //       .baseArrayLayer = 0,
      //       .layerCount = 1,
      //   };

      //   // Image must already be in VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
      //   vkCmdClearColorImage(
      //      pcommandbuffer->m_vkcommandbuffer,
      //      ptextureRef->m_vkimage,
      //      ptextureRef->m_vkimagelayout,
      //      &clearColor,
      //      1,
      //      &subresourceRange
      //   );

      //}

      //m_ptexture->_new_state(pcommandbuffer, VK_IMAGE_LAYOUT_GENERAL);
      //ptexture->_new_state(pcommandbuffer, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

      {
         //VkImageMemoryBarrier barrier = {
         //   .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
         //   .srcAccessMask = m_ptexture->m_vkaccessflags,
         //   .dstAccessMask = VK_ACCESS_HOST_READ_BIT,
         //   .oldLayout = m_ptexture->m_vkimagelayout,
         //   .newLayout = VK_IMAGE_LAYOUT_GENERAL,
         //  .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
         //  .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
         //  .image = m_ptexture->m_vkimage,
         //  .subresourceRange = {
         //      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
         //      .baseMipLevel = 0,
         //      .levelCount = 1,
         //      .baseArrayLayer = 0,
         //      .layerCount = 1
         //  }
         //};


         //vkCmdPipelineBarrier(
         //   pcommandbuffer->m_vkcommandbuffer,
         //   m_ptexture->m_vkpipelinestageflags,  // ✅ srcStageMask
         //   VK_PIPELINE_STAGE_HOST_BIT,                 // or SHADER_BIT, depending on next usage         0,
         //   0,
         //   0, NULL,
         //   0, NULL,
         //   1, &barrier
         //);
         //m_ptexture->m_vkaccessflags = VK_ACCESS_HOST_READ_BIT;
         //m_ptexture->m_vkimagelayout = VK_IMAGE_LAYOUT_GENERAL;
         //m_ptexture->m_vkpipelinestageflags = VK_PIPELINE_STAGE_HOST_BIT;
         ptextureRef->_new_state(
            pcommandbuffer,
            VK_ACCESS_HOST_READ_BIT,
            VK_IMAGE_LAYOUT_GENERAL,
            VK_PIPELINE_STAGE_HOST_BIT);
      }

      //{
      //   //VkImageMemoryBarrier barrier = {
      //   //   .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      //   //   .srcAccessMask = ptexture->m_vkaccessflags,
      //   //   .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      //   //   .oldLayout = ptexture->m_vkimagelayout,
      //   //   .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //   //  .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //   //  .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //   //  .image = ptexture->m_vkimage,
      //   //  .subresourceRange = {
      //   //      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //   //      .baseMipLevel = 0,
      //   //      .levelCount = 1,
      //   //      .baseArrayLayer = 0,
      //   //      .layerCount = 1
      //   //  }
      //   //};


      //   //vkCmdPipelineBarrier(
      //   //   pcommandbuffer->m_vkcommandbuffer,
      //   //   ptexture->m_vkpipelinestageflags,  // ✅ srcStageMask
      //   //   VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,                 // or SHADER_BIT, depending on next usage         0,
      //   //   0,
      //   //   0, NULL,
      //   //   0, NULL,
      //   //   1, &barrier
      //   //);
      //   //ptexture->m_vkaccessflags = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
      //   //ptexture->m_vkimagelayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
      //   //ptexture->m_vkpipelinestageflags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
      ptexture->_new_state(
         pcommandbuffer,
         VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
         VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
      );
      //}


      ::cast < renderer > prenderer = m_pcontext->get_gpu_renderer();


      VkSubmitInfo submitInfo{};
      submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = &pcommandbuffer->m_vkcommandbuffer;
      ::array<VkSemaphore> waitSemaphores;
      ::array<VkPipelineStageFlags> waitStages;
      waitStages.add(vkpipelinestageflagsWait);
      ::cast < render_pass > pgpurenderpass = prenderer->m_pgpurendertarget;
      VkSemaphore vksemaphore = pgpurenderpass->renderFinishedSemaphores[prenderer->m_pgpurendertarget->get_frame_index()];
      waitSemaphores.add(vksemaphore);
      submitInfo.waitSemaphoreCount = (uint32_t)waitSemaphores.size();
      submitInfo.pWaitSemaphores = waitSemaphores.data();
      submitInfo.pWaitDstStageMask = waitStages.data();

      m_pcontext->endSingleTimeCommands(pcommandbuffer, 1, &submitInfo);
      //// Transition destination image to general layout, which is the required layout for mapping the image memory later on
      //insertImageMemoryBarrier(
      //   copyCmd,
      //   m_vkimage,
      //   VK_ACCESS_TRANSFER_WRITE_BIT,
      //   VK_ACCESS_MEMORY_READ_BIT,
      //   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      //   VK_IMAGE_LAYOUT_GENERAL,
      //   VK_PIPELINE_STAGE_TRANSFER_BIT,
      //   VK_PIPELINE_STAGE_TRANSFER_BIT,
      //   VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

      //VK_CHECK_RESULT(vkEndCommandBuffer(copyCmd));

      //::cast < offscreen_render_pass > ppass = m_prenderer->m_pgpurendertarget;

      //ppass->submitSamplingWork(copyCmd);

      //vkQueueWaitIdle(m_pgpucontext->graphicsQueue());

      //vkFreeCommandBuffers(m_pgpucontext->logicalDevice(), m_pgpucontext->m_pgpudevice->getCommandPool(), 1, &copyCmd);


   }


   void renderer::cpu_buffer_sampler::send_sample()
   {

      auto& ptextureRef = m_texturea.element_at_grow(m_prenderer->m_pgpurendertarget->get_frame_index());

      if (!ptextureRef)
      {

         return;

      }

      // Make sure you wait for GPU rendering to finish
      vkDeviceWaitIdle(m_pcontext->logicalDevice()); // Or wait on a specific fence


      //auto pimpact = m_pgpucontext->m_pimpact;

      //auto callback = pimpact->m_callbackImage32CpuBuffer;

      //auto callback = m_prenderer->m_callbackImage32CpuBuffer;

      // Get layout of the image (including row pitch)
      VkImageSubresource subResource{};
      subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      VkSubresourceLayout subResourceLayout;

      vkGetImageSubresourceLayout(m_pcontext->logicalDevice(),
         ptextureRef->m_vkimage, &subResource, &subResourceLayout);

      const char* imagedata = nullptr;
      // Map image memory so we can start copying from it
      vkMapMemory(m_pcontext->logicalDevice(),
         ptextureRef->m_vkdevicememory, 0, VK_WHOLE_SIZE, 0, (void**)&imagedata);
      imagedata += subResourceLayout.offset;

      /*
         Save host visible framebuffer image to disk (ppm format)
      */

      //::memory mem;

      //mem.set_size(m_width * m_height * 4);

      //::array<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
      //const bool colorSwizzle = (std::find(formatsBGR.begin(), formatsBGR.end(), VK_FORMAT_R8G8B8A8_UNORM) != formatsBGR.end());
      //{

      auto pgpucontext = m_pcontext;

      auto pcpubuffer = pgpucontext->m_pcpubuffer;

      pcpubuffer->set_pixels(
         imagedata,
         ptextureRef->m_rectangleTarget.width(),
         ptextureRef->m_rectangleTarget.height(),
         (int)subResourceLayout.rowPitch,
         true);

      //_synchronous_lock synchronouslock(m_pgpucontext->m_pmutexOffscreen);
      //   m_pgpucontext->m_sizeOffscreen.cx() = m_vkextent2d.width;
      //m_pgpucontext->m_sizeOffscreen.cy() = m_vkextent2d.height;
      //m_pgpucontext->m_iScanOffscreen = subResourceLayout.rowPitch;
      //auto area = m_pgpucontext->m_iScanOffscreen * m_pgpucontext->m_sizeOffscreen.cy();
      //m_pgpucontext->m_memoryOffscreen.set_size(area);
      //m_pgpucontext->m_memoryOffscreen.assign(imagedata, area);
      //callback((void *)imagedata,
        // m_vkextent2d.width,
         //m_vkextent2d.height,
         //subResourceLayout.rowPitch);

   //}
   //else
   //{


   //}


      vkUnmapMemory(m_pcontext->logicalDevice(), ptextureRef->m_vkdevicememory);

   }


   void renderer::sample()
   {

      vkQueueWaitIdle(m_pgpucontext->graphicsQueue());

      //auto callback = m_callbackImage32CpuBuffer;

  //      if (callback)
      {

         ::cast < render_pass > pgpurenderpass = m_pgpurendertarget;

         m_pcpubuffersampler->update(m_pgpucontext->m_rectangle.size());
         /*
            Copy framebuffer image to host visible image
         */
         /*const char* imagedata;*/
         {



            ::cast < texture > ptexture = pgpurenderpass->m_texturea[m_pgpurendertarget->get_frame_index()];


            m_pcpubuffersampler->sample(ptexture);

            //// Create the linear tiled destination image to copy to and to read the memory from

            //// Do the actual blit from the offscreen image to our host visible destination image
            //VkCommandBufferAllocateInfo cmdBufAllocateInfo = initializers::commandBufferAllocateInfo(m_pgpucontext->getCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);
            //VkCommandBuffer copyCmd;
            //VK_CHECK_RESULT(vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &cmdBufAllocateInfo, &copyCmd));
            //VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
            //VK_CHECK_RESULT(vkBeginCommandBuffer(copyCmd, &cmdBufInfo));

            //// Transition destination image to transfer destination layout
            //::graphics3d_vulkan::tools::insertImageMemoryBarrier(
            //	copyCmd,
            //	m_poffscreensampler->m_vkimage,
            //	0,
            //	VK_ACCESS_TRANSFER_WRITE_BIT,
            //	VK_IMAGE_LAYOUT_UNDEFINED,
            //	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            //	VK_PIPELINE_STAGE_TRANSFER_BIT,
            //	VK_PIPELINE_STAGE_TRANSFER_BIT,
            //	VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

            //// colorAttachment.image is already in VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, and does not need to be transitioned

            //VkImageCopy imageCopyRegion{};
            //imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            //imageCopyRegion.srcSubresource.layerCount = 1;
            //imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            //imageCopyRegion.dstSubresource.layerCount = 1;
            //imageCopyRegion.extent.width = pgpurenderpass->width();
            //imageCopyRegion.extent.height = pgpurenderpass->height();
            //imageCopyRegion.extent.depth = 1;

            //vkCmdCopyImage(
            //	copyCmd,
            //	//colorAttachment.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            //	pgpurenderpass->m_images[iIndex], VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            //	dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            //	1,
            //	&imageCopyRegion);

            //// Transition destination image to general layout, which is the required layout for mapping the image memory later on
            //::graphics3d_vulkan::tools::insertImageMemoryBarrier(
            //	copyCmd,
            //	dstImage,
            //	VK_ACCESS_TRANSFER_WRITE_BIT,
            //	VK_ACCESS_MEMORY_READ_BIT,
            //	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            //	VK_IMAGE_LAYOUT_GENERAL,
            //	VK_PIPELINE_STAGE_TRANSFER_BIT,
            //	VK_PIPELINE_STAGE_TRANSFER_BIT,
            //	VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

            //VK_CHECK_RESULT(vkEndCommandBuffer(copyCmd));

            //submitWork(copyCmd, queue);

            //submitInfo.commandBufferCount = 1;
            //submitInfo.pCommandBuffers = &copyCmd;
            //VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));


            //// Get layout of the image (including row pitch)
            //VkImageSubresource subResource{};
            //subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            //VkSubresourceLayout subResourceLayout;

            //vkGetImageSubresourceLayout(m_pgpucontext->logicalDevice(), dstImage, &subResource, &subResourceLayout);

            //// Map image memory so we can start copying from it
            //vkMapMemory(m_pgpucontext->logicalDevice(), dstImageMemory, 0, VK_WHOLE_SIZE, 0, (void**)&imagedata);
            //imagedata += subResourceLayout.offset;

            m_pcpubuffersampler->send_sample();

            ///*
            //	Save host visible framebuffer image to disk (ppm format)
            //*/

            ////::memory mem;

            ////mem.set_size(m_width * m_height * 4);

            ////::array<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
            ////const bool colorSwizzle = (std::find(formatsBGR.begin(), formatsBGR.end(), VK_FORMAT_R8G8B8A8_UNORM) != formatsBGR.end());
            //if (callback)
            //{
            //	callback((void*)imagedata, 
            //		pgpurenderpass->width(),
            //		pgpurenderpass->height(),
            //		subResourceLayout.rowPitch);

            //}

            //// ppm binary pixel data
            //for (int32_t y = 0; y < m_height; y++) {
            //   unsigned int * row = (unsigned int *)imagedata;
            //   for (int32_t x = 0; x < m_width; x++) {
            //      //               if (colorSwizzle) {
            //         file.write((char *)row + 2, 1);
            //         file.write((char *)row + 1, 1);
            //         file.write((char *)row, 1);
            //      }
            //      //else {
            //      //   file.write((char *)row, 3);
            //      //}
            //      row++;
            //   }
            //   imagedata += subResourceLayout.rowPitch;
            //}


        //         callback

        //#if defined (VK_USE_PLATFORM_ANDROID_KHR)
        //         const char * filename = strcat(getenv("EXTERNAL_STORAGE"_ansi), "/headless.ppm"_ansi);
        //#else
        //         const char * filename = "headless.ppm"_ansi;
        //#endif
        //         std::ofstream file(filename, std::ios::out | std::ios::binary);
        //
        //         // ppm header
        //         file << "P6\n"_ansi << m_width << "\n"_ansi << m_height << "\n"_ansi << 255 << "\n"_ansi;

               //// If source is BGR (destination is always RGB) and we can't use blit (which does automatic conversion), we'hi have to manually swizzle color components
               //// Check if source is BGR and needs swizzle
               //::array<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
               //const bool colorSwizzle = (std::find(formatsBGR.begin(), formatsBGR.end(), VK_FORMAT_R8G8B8A8_UNORM) != formatsBGR.end());

               //// ppm binary pixel data
               //for (int32_t y = 0; y < m_height; y++) {
               //   unsigned int * row = (unsigned int *)imagedata;
               //   for (int32_t x = 0; x < m_width; x++) {
               //      if (colorSwizzle) {
               //         file.write((char *)row + 2, 1);
               //         file.write((char *)row + 1, 1);
               //         file.write((char *)row, 1);
               //      }
               //      else {
               //         file.write((char *)row, 3);
               //      }
               //      row++;
               //   }
               //   imagedata += subResourceLayout.rowPitch;
               //}
               //file.close();

               //LOG("Framebuffer image saved to %s\n"_ansi, filename);

               // Clean up resources
         }


      }


   }


   //	void renderer::resolve_color_and_alpha_accumulation_buffers()
   //	{
   //
   //		auto pcommandbuffer = m_pgpucontext->beginSingleTimeCommands();
   //
   //		::cast < accumulation_render_pass > ppass = pgpurenderpass;
   //
   //		auto iPassCurrentFrame = get_frame_index();
   //
   //		auto image = ppass->m_images[iPassCurrentFrame];
   //
   //		insertImageMemoryBarrier(pcommandbuffer->m_vkcommandbuffer,
   //			image,
   //			0,
   //			VK_ACCESS_TRANSFER_WRITE_BIT,
   //			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //			VK_PIPELINE_STAGE_TRANSFER_BIT,
   //			VK_PIPELINE_STAGE_TRANSFER_BIT,
   //			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });
   //
   //		auto imageAlphaAccumulation = ppass->m_imagesAlphaAccumulation[iPassCurrentFrame];
   //
   //		insertImageMemoryBarrier(pcommandbuffer->m_vkcommandbuffer,
   //			imageAlphaAccumulation,
   //			0,
   //			VK_ACCESS_TRANSFER_WRITE_BIT,
   //			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //			VK_PIPELINE_STAGE_TRANSFER_BIT,
   //			VK_PIPELINE_STAGE_TRANSFER_BIT,
   //			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });
   //
   //
   //		VkSubmitInfo submitInfo{};
   //		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
   //		submitInfo.commandBufferCount = 1;
   //		submitInfo.pCommandBuffers = &pcommandbuffer->m_vkcommandbuffer;
   //		::array<VkSemaphore> waitSemaphores;
   //		::array<VkPipelineStageFlags> waitStages;
   //		waitStages.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
   //		waitSemaphores.add(pgpurenderpass->renderFinishedSemaphores[iPassCurrentFrame]);
   //		submitInfo.waitSemaphoreCount = waitSemaphores.size();
   //		submitInfo.pWaitSemaphores = waitSemaphores.data();
   //		submitInfo.pWaitDstStageMask = waitStages.data();
   //		m_pgpucontext->endSingleTimeCommands(pcommandbuffer->m_vkcommandbuffer, 1, &submitInfo);
   //
   //		//m_prendererResolve->pgpurenderpass->m_semaphoreaWaitToSubmit.add(
   //		//   pgpurenderpass->renderFinishedSemaphores[iPassCurrentFrame]
   //		//);
   //          //m_prendererResolve->pgpurenderpass->m_stageaWaitToSubmit.add(
         //   VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
         //);

   ////
   //		//m_prendererResolve->_resolve_color_and_alpha_accumulation_buffers();
   //
   //	}


      //void renderer::_resolve_color_and_alpha_accumulation_buffers()
      //{

      //	on_new_frame();

      //	auto pcommandbuffer = m_pgpucontext->beginSingleTimeCommands();

      //	auto iFrameIndex1 = get_frame_index();

      //	VkImage image1 = pgpurenderpass->m_images[iFrameIndex1];

      //	if (is_starting_frame())
      //	{

      //		insertImageMemoryBarrier(pcommandbuffer->m_vkcommandbuffer,
      //			image1,
      //			0,
      //			VK_ACCESS_TRANSFER_WRITE_BIT,
      //			VK_IMAGE_LAYOUT_UNDEFINED,
      //			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //			VK_PIPELINE_STAGE_TRANSFER_BIT,
      //			VK_PIPELINE_STAGE_TRANSFER_BIT,
      //			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });
      //	}
      //	else
      //	{

      //		insertImageMemoryBarrier(pcommandbuffer->m_vkcommandbuffer,
      //			image1,
      //			0,
      //			VK_ACCESS_TRANSFER_WRITE_BIT,
      //			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //			VK_PIPELINE_STAGE_TRANSFER_BIT,
      //			VK_PIPELINE_STAGE_TRANSFER_BIT,
      //			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });
      //	}

      //	m_pgpucontext->endSingleTimeCommands(pcommandbuffer->m_vkcommandbuffer);

      //	auto pframe = beginFrame();

      //	on_begin_render(pframe);

      //	// Resolve (Color and Alpha Accumulation Buffers) descriptors
      //	if (!m_psetdescriptorlayoutResolve)
      //	{

      //		m_psetdescriptorlayoutResolve = ::gpu_vulkan::set_descriptor_layout::Builder(m_pgpucontext)
      //			.addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      //			.addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      //			.build();

      //		int iFrameCount = get_frame_count();

      //		auto pdescriptorpoolbuilder = __allocate::gpu_vulkan::descriptor_pool::Builder();

      //		pdescriptorpoolbuilder->initialize_builder(m_pgpucontext);
      //		pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
      //		pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

      //		m_pdescriptorpoolResolve = pdescriptorpoolbuilder->build();

      //	}

      //	if (!m_pshaderResolve)
      //	{

      //		auto pshadervertexinput = __allocate  shader_vertex_input();

      //		pshadervertexinput->m_bindings.add(
      //			{
      //			   .binding = 0,
      //			   .stride = sizeof(float) * 4,
      //			   .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
      //			});

      //		pshadervertexinput->m_attribs.add({ .location = 0, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = 0 });
      //		pshadervertexinput->m_attribs.add({ .location = 1, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = sizeof(float) * 2 });

      //		auto pshaderResolve = __create_new<::gpu_vulkan::shader>();

      //		m_pshaderResolve = pshaderResolve;

      //		pshaderResolve->m_bDisableDepthTest = true;

      //		//VkDescriptorSetLayoutBinding samplerLayoutBinding = {
      //		//   .binding = 0,
      //		//   .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
      //		//   .descriptorCount = 1,
      //		//   .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
      //		//   .pImmutableSamplers = NULL,
      //		//};

      //		//VkDescriptorSetLayoutCreateInfo layoutInfo =
      //		//{
      //		//   .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
      //		//   .bindingCount = 1,
      //		//   .pBindings = &samplerLayoutBinding,
      //		//};

      //		//VkDescriptorSetLayout descriptorSetLayout;
      //		//if (vkCreateDescriptorSetLayout(device, &layoutInfo, NULL, &descriptorSetLayout) != VK_SUCCESS) 
      //		//{
      //		//   // Handle error
      //		//}

      //		::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

      //		pshaderResolve->initialize_shader_with_block(
      //			this,
      //			as_memory_block(g_uaImageBlendVertexShader),
      //			as_memory_block(g_uaResolveFragmentShader),
      //			{ ::gpu::shader::e_descriptor_set_slot_local },
      //			m_psetdescriptorlayoutResolve,
      //			pshadervertexinput);

      //	}

      //	auto pshader = m_pshaderResolve;

      //	pshader->bind();

      //	auto& pdescriptor = m_pdescriptorResolve;

      //	if (__defer_construct_new(pdescriptor))
      //	{

      //		pdescriptor->m_descriptorsets.set_size(get_frame_count());

      //		::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

      //		::cast < renderer > pgpurendererParent = m_pgpucontext->m_pgpurenderer;

      //		::cast < accumulation_render_pass > paccumulationrenderpass = pgpurendererParent->pgpurenderpass;

      //		for (int i = 0; i < get_frame_count(); i++)
      //		{
      //			VkDescriptorImageInfo imageinfo;

      //			imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      //			imageinfo.imageView = paccumulationrenderpass->m_imageviews[i];
      //			imageinfo.sampler = m_pgpucontext->_001VkSampler();

      //			VkDescriptorImageInfo imageinfo2;

      //			imageinfo2.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      //			imageinfo2.imageView = paccumulationrenderpass->m_imageviewsAlphaAccumulation[i];
      //			imageinfo2.sampler = m_pgpucontext->_001VkSampler();

      //			auto& playout = m_psetdescriptorlayoutResolve;

      //			auto& ppool = m_pdescriptorpoolResolve;

      //			descriptor_writer(*playout, *ppool)
      //				.writeImage(0, &imageinfo)
      //				.writeImage(1, &imageinfo2)
      //				.build(pdescriptor->m_descriptorsets[i]);

      //		}

      //		auto descriptorSetLayout = m_psetdescriptorlayoutResolve->getDescriptorSetLayout();

      //		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
      //   .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
      //   .setLayoutCount = 1,
      //   .pSetLayouts = &descriptorSetLayout,
      //		};

      //		//VkPipelineLayout pipelineLayout;
      //		if (vkCreatePipelineLayout(m_pgpucontext->logicalDevice(), &pipelineLayoutInfo, NULL, &pdescriptor->m_vkpipelinelayout) != VK_SUCCESS) {
      //			// Handle error
      //		}

      //	}

      //	auto pcommandbuffer = getCurrentCommandBuffer();

      //	//auto pcommandbuffer = this->getCurrentCommandBuffer();

      //	// Bind pipeline and descriptor sets
      // //      vkCmdBindPipeline(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
      //   //    vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, ...);
      //	vkCmdBindDescriptorSets(
      //		pcommandbuffer->m_vkcommandbuffer,
      //		VK_PIPELINE_BIND_POINT_GRAPHICS,   // Bind point
      //		pdescriptor->m_vkpipelinelayout,                     // Layout used when pipeline was created
      //		0,                                  // First set (set = 0)
      //		1,                                  // Descriptor set count
      //		&pdescriptor->m_descriptorsets[get_frame_index()],                     // Pointer to descriptor set
      //		0,                                  // Dynamic offset count
      //		NULL                                // Dynamic offsets
      //	);


      //	auto& pmodel = m_pmodelResolve;

      //	if (__defer_construct_new(pmodel))
      //	{

      //		create_quad_buffers(m_pgpucontext->logicalDevice(),
      //			m_pgpucontext->m_pgpudevice->m_pphysicaldevice->m_physicaldevice,
      //			&pmodel->m_vertexBuffer,
      //			&pmodel->m_vertexMemory,
      //			&pmodel->m_indexBuffer,
      //			&pmodel->m_indexMemory);

      //	}



      //	VkDeviceSize offsets[] = { 0 };
      //	vkCmdBindVertexBuffers(pcommandbuffer->m_vkcommandbuffer, 0, 1, &pmodel->m_vertexBuffer, offsets);
      //	vkCmdBindIndexBuffer(pcommandbuffer->m_vkcommandbuffer, pmodel->m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);
      //	auto rectangle = m_rectangle;
      //	VkViewport vp = {
      //	   (float)rectangle.left(),
      //	   (float)rectangle.top(),
      //	   (float)rectangle.width(),
      //	   (float)rectangle.height(),
      //	   0.0f, 1.0f };
      //	VkRect2D sc = {
      //	   {
      //	   (float)rectangle.left(),
      //	   (float)rectangle.top(),
      //	   },
      //	   {
      //				   (float)rectangle.width(),
      //	   (float)rectangle.height(),


      //	}
      //	};
      //	vkCmdSetViewport(pcommandbuffer->m_vkcommandbuffer, 0, 1, &vp);
      //	vkCmdSetScissor(pcommandbuffer->m_vkcommandbuffer, 0, 1, &sc);

      //	vkCmdDrawIndexed(pcommandbuffer->m_vkcommandbuffer, 6, 1, 0, 0, 0);
      //	// Draw full-screen quad
      //	//vkCmdDraw(pcommandbuffer->m_vkcommandbuffer, 6, 1, 0, 0); // assuming full-screen triangle/quad

      //	pshader->unbind();

      //	on_end_render(pframe);

      //	endFrame();

      //}


   void renderer::swap_chain()
   {

      assert(isFrameStarted && "Can't call endFrame while frame is not in progress");

      ::cast < command_buffer > pcommandbuffer = getCurrentCommandBuffer2();

      if (vkEndCommandBuffer(pcommandbuffer->m_vkcommandbuffer) != VK_SUCCESS)
      {

         throw ::exception(error_failed, "failed to record command buffer!");

      }

      ::cast < render_pass > pgpurenderpass = m_pgpurendertarget;

      auto result = pgpurenderpass->submitCommandBuffers(
         pcommandbuffer,
         {},
         {},
         {});

      if (result == VK_ERROR_OUT_OF_DATE_KHR
         || result == VK_SUBOPTIMAL_KHR
         || m_bNeedToRecreateSwapChain)
      {

         m_bNeedToRecreateSwapChain = false;

         defer_update_renderer();

      }
      else if (result != VK_SUCCESS)
      {

         throw ::exception(error_failed, "failed to present swap chain image!");

      }

      vkQueueWaitIdle(m_pgpucontext->graphicsQueue());

   }


   void renderer::on_end_draw()
   {

      _on_end_render();

      for (auto& procedure : m_procedureaAfterEndRender)
      {

         procedure();

      }

      m_procedureaAfterEndRender.clear();
      //if (m_bOffScreen)
      //{

      //   assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
      //   auto pcommandbuffer = getCurrentCommandBuffer();
      //   if (vkEndCommandBuffer(pcommandbuffer->m_vkcommandbuffer) != VK_SUCCESS) {
      //      throw ::exception(error_failed, "failed to record command buffer!");
      //   }
      //   auto result = pgpurenderpass->submitCommandBuffers(&pcommandbuffer->m_vkcommandbuffer, &m_uCurrentSwapChainImage);
      //   //if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      //   //	vkcWindow.wasWindowResized()) 
      //   //{
      //   //	vkcWindow.resetWindowResizedFlag();
      //   //	recreateSwapchain();
      //   //}
      //   //else 
      //   //	if (result != VK_SUCCESS) {
      //   //	throw ::exception(error_failed, "failed to present swap chain image!");
      //   //}

      if (m_pgpucontext->m_eoutput == ::gpu::e_output_cpu_buffer)
      {
         sample();
      }
      else if (m_pgpucontext->m_eoutput == ::gpu::e_output_swap_chain)
      {
         swap_chain();


      }
      isFrameStarted = false;
      //currentFrameIndex = (currentFrameIndex + 1) % render_pass::MAX_FRAMES_IN_FLIGHT;

      //}
      //else
      //{


      //	assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
      //	auto pcommandbuffer = getCurrentCommandBuffer();
      //	if (vkEndCommandBuffer(pcommandbuffer->m_vkcommandbuffer) != VK_SUCCESS) {
      //		throw ::exception(error_failed, "failed to record command buffer!");
      //	}
      //	auto result = m_pvkcswapchain->submitCommandBuffers(&pcommandbuffer->m_vkcommandbuffer, &m_uCurrentSwapChainImage);
      //	//if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      //	//	vkcWindow.wasWindowResized()) 
      //	//{
      //	//	vkcWindow.resetWindowResizedFlag();
      //	//	recreateSwapchain();
      //	//}
      //	//else 
      //	//	if (result != VK_SUCCESS) {
      //	//	throw ::exception(error_failed, "failed to present swap chain image!");
      //	//}
      //	isFrameStarted = false;
      //	currentFrameIndex = (currentFrameIndex + 1) % swap_chain_render_pass::MAX_FRAMES_IN_FLIGHT;

      //}

   }


   shader* renderer::_get_image_blend_shader()
   {

      if (!m_pshaderImageBlend)
      {

         __construct_new(m_pshaderImageBlend);

         // Image Blend descriptors
         //if (!m_psetdescriptorlayoutImageBlend)
         {

            m_pshaderImageBlend->shader_sampler()->m_psetdescriptorlayout
               = ::gpu_vulkan::set_descriptor_layout::Builder(m_pgpucontext)
               .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
               .build();

            int iFrameCount = m_pgpurendertarget->get_frame_count();

            auto pdescriptorpoolbuilder = __allocate::gpu_vulkan::descriptor_pool::Builder();

            pdescriptorpoolbuilder->initialize_builder(m_pgpucontext);
            pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
            pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

            m_pshaderImageBlend->shader_sampler()->m_pdescriptorpool
               = pdescriptorpoolbuilder->build();

         }

         //auto pinputlayout = __øcreate<::gpu::input_layout> ();


         //pshadervertexinput->m_bindings.add(
         //   {
         //      .binding = 0,
         //      .stride = sizeof(float) * 4,
         //      .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
         //   });

         //pshadervertexinput->m_attribs.add({ .location = 0, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = 0 });
         //pshadervertexinput->m_attribs.add({ .location = 1, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = sizeof(float) * 2 });

         m_pshaderImageBlend->m_bEnableBlend = true;
         m_pshaderImageBlend->m_bDisableDepthTest = true;

         ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

         m_pshaderImageBlend->initialize_shader_with_block(
            this,
            as_memory_block(g_uaImageBlendVertexShader),
            as_memory_block(g_uaImageBlendFragmentShader),
            { ::gpu::shader::e_descriptor_set_slot_s1 },
            {}, {},
            m_pgpucontext->input_layout(::graphics3d::sequence2_uv_properties()));

      }

      return m_pshaderImageBlend;

   }


   shader* renderer::_get_image_set_shader()
   {

      if (!m_pshaderImageSet)
      {

         __construct_new(m_pshaderImageSet);

         //auto pshadervertexinput = __allocate  shader_vertex_input();

         //pshadervertexinput->m_bindings.add(
         //   {
         //      .binding = 0,
         //      .stride = sizeof(float) * 4,
         //      .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
         //   });

         //pshadervertexinput->m_attribs.add({ .location = 0, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = 0 });
         //pshadervertexinput->m_attribs.add({ .location = 1, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = sizeof(float) * 2 });

         //::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

         m_pshaderImageBlend->initialize_shader_with_block(
            this,
            as_memory_block(g_uaImageBlendVertexShader),
            as_memory_block(g_uaImageBlendFragmentShader),
            { ::gpu::shader::e_descriptor_set_slot_local },
            m_pshaderImageSet->shader_sampler()->m_psetdescriptorlayout,
            {},
            m_pgpucontext->input_layout(::graphics3d::sequence2_uv_properties()));

      }

      return m_pshaderImageSet;

   }



   //// Fullscreen quad vertex data
   //float quadVertices[] = {
   //   // pos     // uv
   //   -1.0f, -1.0f, 0.0f, 0.0f,
   //    1.0f, -1.0f, 1.0f, 0.0f,
   //    1.0f,  1.0f, 1.0f, 1.0f,
   //   -1.0f,  1.0f, 0.0f, 1.0f,
   //};
   //uint16_t quadIndices[] = {
   //   0, 1, 2,
   //   2, 3, 0
   //};

   //// Create vertex and index buffers
   //void create_quad_buffers(VkDevice device, VkPhysicalDevice physicalDevice,
   //   VkBuffer* vertexBuffer, VkDeviceMemory* vertexMemory,
   //   VkBuffer* indexBuffer, VkDeviceMemory* indexMemory, bool bYSwap)
   //{

   //   float y0;
   //   float y1;

   //   if (bYSwap)
   //   {

   //      y0 = 1.0f;
   //      y1 = -1.0f;


   //   }
   //   else
   //   {

   //      y0 = -1.0f;
   //      y1 = 1.0f;

   //   }


   //   // Fullscreen quad vertex data
   //   float quadVertices[] = {
   //      // pos     // uv
   //      -1.0f, y0, 0.0f, 0.0f,
   //       1.0f, y0, 1.0f, 0.0f,
   //       1.0f, y1, 1.0f, 1.0f,
   //      -1.0f, y1, 0.0f, 1.0f,
   //   };
   //   uint16_t quadIndices[] = {
   //      0, 1, 2,
   //      2, 3, 0
   //   };


   //   VkDeviceSize vertexSize = sizeof(quadVertices);
   //   VkDeviceSize indexSize = sizeof(quadIndices);

   //   VkBufferCreateInfo bufferInfo = {
   //      .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
   //      .size = vertexSize,
   //      .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
   //      .sharingMode = VK_SHARING_MODE_EXCLUSIVE
   //   };
   //   vkCreateBuffer(device, &bufferInfo, NULL, vertexBuffer);

   //   VkMemoryRequirements memReq;
   //   vkGetBufferMemoryRequirements(device, *vertexBuffer, &memReq);

   //   VkMemoryAllocateInfo allocInfo = {
   //      .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
   //      .allocationSize = memReq.size,
   //      .memoryTypeIndex = 0
   //   };

   //   VkPhysicalDeviceMemoryProperties memProps;
   //   vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProps);
   //   for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
   //      if ((memReq.memoryTypeBits & (1 << i)) &&
   //         (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) &&
   //         (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
   //         allocInfo.memoryTypeIndex = i;
   //         break;
   //      }
   //   }

   //   vkAllocateMemory(device, &allocInfo, NULL, vertexMemory);
   //   vkBindBufferMemory(device, *vertexBuffer, *vertexMemory, 0);

   //   void* data;
   //   vkMapMemory(device, *vertexMemory, 0, vertexSize, 0, &data);
   //   memcpy(data, quadVertices, (size_t)vertexSize);
   //   vkUnmapMemory(device, *vertexMemory);

   //   // Index buffer
   //   bufferInfo.size = indexSize;
   //   bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
   //   vkCreateBuffer(device, &bufferInfo, NULL, indexBuffer);
   //   vkGetBufferMemoryRequirements(device, *indexBuffer, &memReq);
   //   allocInfo.allocationSize = memReq.size;

   //   for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
   //      if ((memReq.memoryTypeBits & (1 << i)) &&
   //         (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) &&
   //         (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
   //         allocInfo.memoryTypeIndex = i;
   //         break;
   //      }
   //   }

   //   vkAllocateMemory(device, &allocInfo, NULL, indexMemory);
   //   vkBindBufferMemory(device, *indexBuffer, *indexMemory, 0);
   //   vkMapMemory(device, *indexMemory, 0, indexSize, 0, &data);
   //   memcpy(data, quadIndices, (size_t)indexSize);
   //   vkUnmapMemory(device, *indexMemory);
   //}


   void renderer::_blend_image(::gpu::texture* pgputexture, const ::int_rectangle& rectangle, bool bYSwap)
   {


      //vkCmdBeginRenderPass(...);

      ::cast < command_buffer > pcommandbuffer = this->getCurrentCommandBuffer2();

      //VkCommandBufferAllocateInfo commandBufferAllocateInfo = initializers::commandBufferAllocateInfo(m_pgpucontext->getCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);

      //auto pcommandbuffer;
      //VK_CHECK_RESULT(vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &commandBufferAllocateInfo, &pcommandbuffer->m_vkcommandbuffer));
      //VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
      //VK_CHECK_RESULT(vkBeginCommandBuffer(pcommandbuffer->m_vkcommandbuffer, &cmdBufInfo));


      //m_procedureaAfterEndRender.add(
      //   [this, image, pcommandbuffer->m_vkcommandbuffer]()
      //   {

      //      //            {
      //      //            VkImageMemoryBarrier barrier = {
      //      //.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      //      //.pNext = nullptr,
      //      //   .srcAccessMask = VK_ACCESS_SHADER_READ_BIT,
      //      //.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      //      //
      //      //.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //      //.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //      //.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //      //.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //      //.image = image,
      //      //.subresourceRange = {
      //      //    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //      //    .baseMipLevel = 0,
      //      //    .levelCount = 1,
      //      //    .baseArrayLayer = 0,
      //      //    .layerCount = 1,
      //      //},
      //      //            };
      //      //
      //      //            vkCmdPipelineBarrier(
      //      //               pcommandbuffer->m_vkcommandbuffer,
      //      //               VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      //      //               VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      //      //               0,
      //      //               0, NULL,
      //      //               0, NULL,
      //      //               1, &barrier
      //      //            );
      //      //
      //      //         }
      //      //            {
      //      //               VkImageMemoryBarrier barrier = {
      //      //         .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      //      //         .pNext = nullptr,
      //      //         .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      //      //         .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
      //      //
      //      //         .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //      //         .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //      //         .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //      //         .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //      //         .image = image,  // <-- your VkImage here
      //      //         .subresourceRange = {
      //      //             .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //      //             .baseMipLevel = 0,
      //      //             .levelCount = 1,
      //      //             .baseArrayLayer = 0,
      //      //             .layerCount = 1,
      //      //         },
      //      //               };
      //      //
      //      //               vkCmdPipelineBarrier(
      //      //                  pcommandbuffer->m_vkcommandbuffer,
      //      //                  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // srcStageMask
      //      //                  VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,         // dstStageMask
      //      //                  0,
      //      //                  0, NULL,
      //      //                  0, NULL,
      //      //                  1, &barrier
      //      //               );
      //      //
      //      //            }
      //      //
      //      //         }

      //if(1)
      //      {


      //   VkImageMemoryBarrier barrier = {
      //       .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      //       .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      //       .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
      //       .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //       .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //       .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //       .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //       .image = image,
      //       .subresourceRange = {
      //           .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //           .baseMipLevel = 0,
      //           .levelCount = 1,
      //           .baseArrayLayer = 0,
      //           .layerCount = 1
      //       },
      //   };

      //   vkCmdPipelineBarrier(
      //      pcommandbuffer->m_vkcommandbuffer,
      //      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      //      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      //      0,
      //      0, NULL,
      //      0, NULL,
      //      1, &barrier
      //   );

      //      }

      /*   }
         );*/

      ::cast < texture > ptexture = pgputexture;

      ::cast < context > pcontext = m_pgpucontext;

      auto& pmodelbuffer = m_imagemodelbuffer[ptexture->m_vkimage];

      if (__defer_construct_new(pmodelbuffer))
      {

         pmodelbuffer->sequence2_uv_create_rectangle(pcontext, true, true);

         //pmodelcreate_quad_buffers(m_pgpucontext->logicalDevice(),
         //   m_pgpucontext->m_pgpudevice->m_pphysicaldevice->m_physicaldevice,
         //   &pmodel->m_vertexBuffer,
         //   &pmodel->m_vertexMemory,
         //   &pmodel->m_indexBuffer,
         //   &pmodel->m_indexMemory, true);

      }

      //auto pshader = get_image_blend_shader();

      m_pshaderImageBlend->bind(m_pgpurendertarget->current_texture(), pgputexture);

      //m_pshaderImageBlend->_bind_sampler(image, 0);

      pmodelbuffer->bind(pcommandbuffer);

      //VkViewport vp = {
      //   (float)rectangle.left(),
      //   (float)rectangle.top(),
      //   (float)rectangle.width(),
      //   (float)rectangle.height(),
      //   0.0f, 1.0f };

      //VkRect2D sc = {
      //   {
      //      rectangle.left(),
      //      rectangle.top(),
      //   },
      //   {
      //      (uint32_t)rectangle.width(),
      //      (uint32_t)rectangle.height(),
      //   }
      //};

      pcommandbuffer->set_viewport(rectangle);

      pcommandbuffer->set_scissor(rectangle);

      //vkCmdSetViewport(pcommandbuffer->m_vkcommandbuffer, 0, 1, &vp);

      //vkCmdSetScissor(pcommandbuffer->m_vkcommandbuffer, 0, 1, &sc);

      pcommandbuffer->draw(pmodelbuffer);

      //vkCmdDrawIndexed(pcommandbuffer->m_vkcommandbuffer, 6, 1, 0, 0, 0);
      // Draw full-screen quad
      //vkCmdDraw(pcommandbuffer->m_vkcommandbuffer, 6, 1, 0, 0); // assuming full-screen triangle/quad

      m_pshaderImageBlend->unbind();

      //vkCmdEndRenderPass(...);

      vkQueueWaitIdle(m_pgpucontext->graphicsQueue());

      //vkFreeCommandBuffers(m_pgpucontext->logicalDevice(), m_pgpucontext->m_vkcommandpool, 1, &pcommandbuffer->m_vkcommandbuffer);


   }


   void renderer::copy(::gpu::texture* pgputextureTarget, ::gpu::texture* pgputextureSource)
   {

      ::cast < command_buffer > pcommandbuffer = this->getCurrentCommandBuffer2();

      ::cast < texture > ptextureSrc = pgputextureSource;
      ::cast < texture > ptextureDst = pgputextureTarget;

      ptextureSrc->_new_state(
         pcommandbuffer,
         0,
         VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

      ptextureDst->_new_state(
         pcommandbuffer,
         VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
         VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

      //  if (!m_psha)
      //  {

      //     __construct_new(m_pblend2);

      //     m_pblend2->m_pgpurenderer = this;

      //     m_pblend2->create_render_pass();

      //  }

      //  auto& imagestruct = m_pblend2->m_mapImage[ptextureDst->m_vkimage];

      //  if (!imagestruct.m_vkframebuffer)
      //  {

      //     ptextureDst->create_image_view();

      //     imagestruct.m_vkframebuffer = ptextureDst->create_framebuffer(m_pblend2->m_vkrenderpass);

      //  }


      //  // Image Blend descriptors
      //  if (!m_psetdescriptorlayoutBlend2)
      //  {

      //     m_psetdescriptorlayoutBlend2 = ::gpu_vulkan::set_descriptor_layout::Builder(m_pgpucontext)
      //        .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      //        .build();

      //     int iFrameCount = get_frame_count();

      //     auto pdescriptorpoolbuilder = __allocate::gpu_vulkan::descriptor_pool::Builder();

      //     pdescriptorpoolbuilder->initialize_builder(m_pgpucontext);
      //     pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
      //     pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

      //     m_pdescriptorpoolBlend2 = pdescriptorpoolbuilder->build();

      //     ::cast < context > pcontext = m_pgpucontext;

      //     VkDescriptorSetAllocateInfo allocInfo = {
      //.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
      //.descriptorPool = m_pdescriptorpoolBlend2->descriptorPool,
      //.descriptorSetCount = 1,
      //.pSetLayouts = &m_psetdescriptorlayoutBlend2->descriptorSetLayout
      //     };
      //     VK_CHECK_RESULT(vkAllocateDescriptorSets(
      //        pcontext->logicalDevice(),
      //        &allocInfo, &m_pblend2->descSet));

      //  }

      if (!m_pshaderCopyImage)
      {

         __construct_new(m_pshaderCopyImage);

         m_pshaderCopyImage->m_pgpurenderer = this;
         m_pshaderCopyImage->m_iSamplerSlot = 0;
         // Image Blend descriptors
//if (!m_psetdescriptorlayoutImageBlend)
         {

            m_pshaderCopyImage->shader_sampler()->m_psetdescriptorlayout
               = ::gpu_vulkan::set_descriptor_layout::Builder(m_pgpucontext)
               .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
               .build();

            int iFrameCount = m_pgpurendertarget->get_frame_count();

            auto pdescriptorpoolbuilder = __allocate::gpu_vulkan::descriptor_pool::Builder();

            pdescriptorpoolbuilder->initialize_builder(m_pgpucontext);
            pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
            pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

            m_pshaderCopyImage->shader_sampler()->m_pdescriptorpool
               = pdescriptorpoolbuilder->build();

         }

         auto pinputlayoutEmpty = __øcreate<::gpu::input_layout>();

         //pshadervertexinput->m_bindings.add(
         //   {
         //      .binding = 0,
         //      .stride = sizeof(float) * 4,
         //      .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
         //   });

         //pshadervertexinput->m_attribs.add({ .location = 0, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = 0 });
         //pshadervertexinput->m_attribs.add({ .location = 1, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = sizeof(float) * 2 });

         //auto pshaderBlend2 = __create_new<::gpu_vulkan::shader>();

         //m_pshaderCopyImage = pshaderBlend2;

         m_pshaderCopyImage->m_bEnableBlend = true;
         m_pshaderCopyImage->m_bDisableDepthTest = true;
         m_pshaderCopyImage->m_iSamplerSlot = 0;

         ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;


         m_pshaderCopyImage->initialize_shader_with_block(
            this,
            as_memory_block(g_blend2_vertex),
            as_memory_block(g_blend2_fragment),
            { ::gpu::shader::e_descriptor_set_slot_s1 },
            {},
            {},
            pinputlayoutEmpty,
            ::gpu::shader::e_flag_clear_default_bindings_and_attributes_descriptions);


      }


      if (m_pshaderCopyImage->has_shader_sampler())
      {

         auto pshadertextureDst = m_pshaderCopyImage->shader_texture(ptextureDst, false);
         //auto& imagestruct = *pimagestruct;
         //VkClearValue clearColor = { .color = {{0.0f, 0.0f, 0.0f, 0.0f}} };

         //vkCmdBeginRenderPass(pcommandbuffer->m_vkcommandbuffer,

         auto rectangleTarget = ptextureSrc->m_rectangleTarget;

         VkRenderPassBeginInfo renderPassInfo = {
               .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
               .renderPass = m_pshaderCopyImage->shader_sampler()->get_render_pass(),
               .framebuffer = pshadertextureDst->get_framebuffer(),
               .renderArea = {{rectangleTarget.left(), rectangleTarget.top()},
            {(uint32_t)rectangleTarget.width(),(uint32_t)rectangleTarget.height()}},
            //.clearValueCount = 1,
            //.pClearValues = &clearColor
         .clearValueCount = 0,
         .pClearValues = nullptr,
         };

         //auto& imagestructa = s1()->m_imagestructamap[image];

///         vkCmdBeginRenderPass(
   //         pcommandbuffer->m_vkcommandbuffer, 
     //       &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);


      }


      VkClearValue clearColor = { .color = {{0.0f, 0.0f, 0.0f, 0.0f}} };

      //  VkRenderPassBeginInfo renderPassInfo = {
      //.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
      //.renderPass = m_pblend2->m_vkrenderpass,
      //.framebuffer = imagestruct.m_vkframebuffer,
      //.renderArea = {{0, 0}, {(uint32_t)rectangleTarget.width(), (uint32_t)rectangleTarget.height()}},
      //.clearValueCount = 1,
      //.pClearValues = &clearColor
      //  };

      //  vkCmdBeginRenderPass(pcommandbuffer->m_vkcommandbuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        //vkCmdBindPipeline(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
      m_pshaderCopyImage->bind(ptextureDst, ptextureSrc);

      //m_pshaderCopyImage->_bind_sampler(ptextureDst->m_vkimage, 0);

      //vkCmdBindDescriptorSets(pcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pshaderCopyImage->m_pipelinelayout,
         //0, 1, &descriptorSet, 0, NULL);
      // draw full screen triangle or quad
      vkCmdDraw(pcommandbuffer->m_vkcommandbuffer, 3, 1, 0, 0);


      m_pshaderCopyImage->unbind();


      //if (m_pshaderCopyImage->has_shader_sampler())
      //{

      //   vkCmdEndRenderPass(pcommandbuffer->m_vkcommandbuffer);

      //}
      //vkCmdEndRenderPass(pcommandbuffer->m_vkcommandbuffer);

   }


   void renderer::_set_image(::gpu::texture* pgputexture, const ::int_rectangle& rectangle, bool bYSwap)
   {

      //// Image Blend descriptors
      //if (!m_psetdescriptorlayoutImageBlend)
      //{

      //   m_psetdescriptorlayoutImageBlend = ::gpu_vulkan::set_descriptor_layout::Builder(m_pgpucontext)
      //      .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      //      .build();

      //   int iFrameCount = get_frame_count();

      //   auto pdescriptorpoolbuilder = __allocate::gpu_vulkan::descriptor_pool::Builder();

      //   pdescriptorpoolbuilder->initialize_builder(m_pgpucontext);
      //   pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
      //   pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

      //   m_pdescriptorpoolImageBlend = pdescriptorpoolbuilder->build();

      //}

      //vkCmdBeginRenderPass(...);

      ::cast < command_buffer > pcommandbuffer = this->getCurrentCommandBuffer2();

      //VkCommandBufferAllocateInfo commandBufferAllocateInfo = initializers::commandBufferAllocateInfo(m_pgpucontext->getCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);

      //auto pcommandbuffer;
      //VK_CHECK_RESULT(vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &commandBufferAllocateInfo, &pcommandbuffer->m_vkcommandbuffer));
      //VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
      //VK_CHECK_RESULT(vkBeginCommandBuffer(pcommandbuffer->m_vkcommandbuffer, &cmdBufInfo));


      //m_procedureaAfterEndRender.add(
      //   [this, image, pcommandbuffer->m_vkcommandbuffer]()
      //   {

      //      //            {
      //      //            VkImageMemoryBarrier barrier = {
      //      //.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      //      //.pNext = nullptr,
      //      //   .srcAccessMask = VK_ACCESS_SHADER_READ_BIT,
      //      //.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      //      //
      //      //.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //      //.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //      //.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //      //.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //      //.image = image,
      //      //.subresourceRange = {
      //      //    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //      //    .baseMipLevel = 0,
      //      //    .levelCount = 1,
      //      //    .baseArrayLayer = 0,
      //      //    .layerCount = 1,
      //      //},
      //      //            };
      //      //
      //      //            vkCmdPipelineBarrier(
      //      //               pcommandbuffer->m_vkcommandbuffer,
      //      //               VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      //      //               VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      //      //               0,
      //      //               0, NULL,
      //      //               0, NULL,
      //      //               1, &barrier
      //      //            );
      //      //
      //      //         }
      //      //            {
      //      //               VkImageMemoryBarrier barrier = {
      //      //         .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      //      //         .pNext = nullptr,
      //      //         .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      //      //         .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
      //      //
      //      //         .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //      //         .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //      //         .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //      //         .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //      //         .image = image,  // <-- your VkImage here
      //      //         .subresourceRange = {
      //      //             .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //      //             .baseMipLevel = 0,
      //      //             .levelCount = 1,
      //      //             .baseArrayLayer = 0,
      //      //             .layerCount = 1,
      //      //         },
      //      //               };
      //      //
      //      //               vkCmdPipelineBarrier(
      //      //                  pcommandbuffer->m_vkcommandbuffer,
      //      //                  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // srcStageMask
      //      //                  VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,         // dstStageMask
      //      //                  0,
      //      //                  0, NULL,
      //      //                  0, NULL,
      //      //                  1, &barrier
      //      //               );
      //      //
      //      //            }
      //      //
      //      //         }

      //if(1)
      //      {


      //   VkImageMemoryBarrier barrier = {
      //       .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      //       .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      //       .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
      //       .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //       .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //       .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //       .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //       .image = image,
      //       .subresourceRange = {
      //           .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //           .baseMipLevel = 0,
      //           .levelCount = 1,
      //           .baseArrayLayer = 0,
      //           .layerCount = 1
      //       },
      //   };

      //   vkCmdPipelineBarrier(
      //      pcommandbuffer->m_vkcommandbuffer,
      //      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      //      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      //      0,
      //      0, NULL,
      //      0, NULL,
      //      1, &barrier
      //   );

      //      }

      /*   }
         );*/

      ::cast <texture > ptexture = pgputexture;

      ::cast <context > pcontext = m_pgpucontext;

      auto& pmodelbuffer = m_imagemodelbuffer[ptexture->m_vkimage];

      if (__defer_construct_new(pmodelbuffer))
      {

         pmodelbuffer->sequence2_uv_create_rectangle(pcontext, true, bYSwap);

         //create_quad_buffers(m_pgpucontext->logicalDevice(),
         //   m_pgpucontext->m_pgpudevice->m_pphysicaldevice->m_physicaldevice,
         //   &pmodel->m_vertexBuffer,
         //   &pmodel->m_vertexMemory,
         //   &pmodel->m_indexBuffer,
         //   &pmodel->m_indexMemory,
         //   bYSwap);

      }

      auto pshader = _get_image_blend_shader();

      pshader->bind(m_pgpurendertarget->current_texture(), pgputexture);

      // pshader->_bind_sampler(image, 0);

      //  auto& pdescriptor = m_imagedescriptor[image];

      //  //if (__defer_construct_new(pdescriptor))
      //  {

      //     pdescriptor->m_descriptorsets.set_size(get_frame_count());

      //     VkImageViewCreateInfo viewInfo = {
      //  .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      //  .image = image,  // <-- Your existing VkImage
      //  .viewType = VK_IMAGE_VIEW_TYPE_2D,
      //  .format = VK_FORMAT_B8G8R8A8_UNORM,  // <-- Match your image's format
      //  .components = {
      //     .r = VK_COMPONENT_SWIZZLE_IDENTITY,
      //     .g = VK_COMPONENT_SWIZZLE_IDENTITY,
      //     .b = VK_COMPONENT_SWIZZLE_IDENTITY,
      //     .a = VK_COMPONENT_SWIZZLE_IDENTITY,
      //  },
      //  .subresourceRange = {
      //     .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //     .baseMipLevel = 0,
      //     .levelCount = 1,
      //     .baseArrayLayer = 0,
      //     .layerCount = 1,
      //  },
      //     };

      //     VkImageView imageView;
      //     if (vkCreateImageView(m_pgpucontext->logicalDevice(), &viewInfo, NULL, &imageView) != VK_SUCCESS) {
      //        // Handle error
      //     }

      //     ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

      //     for (int i = 0; i < get_frame_count(); i++)
      //     {
      //        VkDescriptorImageInfo imageinfo;

      //        imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      //        imageinfo.imageView = imageView;
      //        imageinfo.sampler = m_pgpucontext->_001VkSampler();

      //        auto& playout = m_psetdescriptorlayoutImageBlend;

      //        auto& ppool = m_pdescriptorpoolImageBlend;

      //        descriptor_writer(*playout, *ppool)
      //           .writeImage(0, &imageinfo)
      //           .build(pdescriptor->m_descriptorsets[i]);

      //     }

      //     auto descriptorSetLayout = m_psetdescriptorlayoutImageBlend->getDescriptorSetLayout();

      //     VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
      //  .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
      //  .setLayoutCount = 1,
      //  .pSetLayouts = &descriptorSetLayout,
      //     };

      //     //VkPipelineLayout pipelineLayout;
      //     if (vkCreatePipelineLayout(m_pgpucontext->logicalDevice(), &pipelineLayoutInfo, NULL, &pdescriptor->m_vkpipelinelayout) != VK_SUCCESS) {
      //        // Handle error
      //     }

      //  }

      //  //auto pcommandbuffer = this->getCurrentCommandBuffer();

      //  // Bind pipeline and descriptor sets
      ////      vkCmdBindPipeline(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
      //  //    vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, ...);
      //  vkCmdBindDescriptorSets(
      //     pcommandbuffer->m_vkcommandbuffer,
      //     VK_PIPELINE_BIND_POINT_GRAPHICS,   // Bind point
      //     pdescriptor->m_vkpipelinelayout,                     // Layout used when pipeline was created
      //     0,                                  // First set (set = 0)
      //     1,                                  // Descriptor set count
      //     &pdescriptor->m_descriptorsets[get_frame_index()],                     // Pointer to descriptor set
      //     0,                                  // Dynamic offset count
      //     NULL                                // Dynamic offsets
      //  );

      pmodelbuffer->bind(pcommandbuffer);

      //VkDeviceSize offsets[] = { 0 };
      //vkCmdBindVertexBuffers(pcommandbuffer->m_vkcommandbuffer, 0, 1, &pmodel->m_vertexBuffer, offsets);
      //vkCmdBindIndexBuffer(pcommandbuffer->m_vkcommandbuffer, pmodel->m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);


      pcommandbuffer->set_viewport(rectangle);

      pcommandbuffer->set_scissor(rectangle);


      pcommandbuffer->draw(pmodelbuffer);
      // Draw full-screen quad
      //vkCmdDraw(pcommandbuffer->m_vkcommandbuffer, 6, 1, 0, 0); // assuming full-screen triangle/quad

      pshader->unbind();

      //vkCmdEndRenderPass(...);

      vkQueueWaitIdle(m_pgpucontext->graphicsQueue());

      //vkFreeCommandBuffers(m_pgpucontext->logicalDevice(), m_pgpucontext->m_vkcommandpool, 1, &pcommandbuffer->m_vkcommandbuffer);


   }




   void renderer::_blend_renderer(::gpu_vulkan::renderer* prendererSrc, bool bYSwap)
   {

      ::cast < render_pass > pgpurenderpassSrc = prendererSrc->m_pgpurendertarget;

      ::cast < texture > ptexture = pgpurenderpassSrc->m_texturea[prendererSrc->m_pgpurendertarget->get_frame_index()];

      auto rectanglePlacement = prendererSrc->m_pgpucontext->m_rectangle;

      //// Image Blend descriptors
      //if (!m_psetdescriptorlayoutImageBlend)
      //{

      //   m_psetdescriptorlayoutImageBlend = ::gpu_vulkan::set_descriptor_layout::Builder(m_pgpucontext)
      //      .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      //      .build();

      //   int iFrameCount = get_frame_count();

      //   auto pdescriptorpoolbuilder = __allocate::gpu_vulkan::descriptor_pool::Builder();

      //   pdescriptorpoolbuilder->initialize_builder(m_pgpucontext);
      //   pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
      //   pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

      //   m_pdescriptorpoolImageBlend = pdescriptorpoolbuilder->build();

      //}

      //vkCmdBeginRenderPass(...);

      ::cast < command_buffer > pcommandbuffer = this->getCurrentCommandBuffer2();

      //VkCommandBufferAllocateInfo commandBufferAllocateInfo = initializers::commandBufferAllocateInfo(m_pgpucontext->getCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);

      //auto pcommandbuffer;
      //VK_CHECK_RESULT(vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &commandBufferAllocateInfo, &pcommandbuffer->m_vkcommandbuffer));
      //VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
      //VK_CHECK_RESULT(vkBeginCommandBuffer(pcommandbuffer->m_vkcommandbuffer, &cmdBufInfo));


      //m_procedureaAfterEndRender.add(
      //   [this, image, pcommandbuffer->m_vkcommandbuffer]()
      //   {

      //      //            {
      //      //            VkImageMemoryBarrier barrier = {
      //      //.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      //      //.pNext = nullptr,
      //      //   .srcAccessMask = VK_ACCESS_SHADER_READ_BIT,
      //      //.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      //      //
      //      //.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //      //.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //      //.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //      //.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //      //.image = image,
      //      //.subresourceRange = {
      //      //    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //      //    .baseMipLevel = 0,
      //      //    .levelCount = 1,
      //      //    .baseArrayLayer = 0,
      //      //    .layerCount = 1,
      //      //},
      //      //            };
      //      //
      //      //            vkCmdPipelineBarrier(
      //      //               pcommandbuffer->m_vkcommandbuffer,
      //      //               VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      //      //               VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      //      //               0,
      //      //               0, NULL,
      //      //               0, NULL,
      //      //               1, &barrier
      //      //            );
      //      //
      //      //         }
      //      //            {
      //      //               VkImageMemoryBarrier barrier = {
      //      //         .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      //      //         .pNext = nullptr,
      //      //         .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      //      //         .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
      //      //
      //      //         .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //      //         .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //      //         .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //      //         .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //      //         .image = image,  // <-- your VkImage here
      //      //         .subresourceRange = {
      //      //             .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //      //             .baseMipLevel = 0,
      //      //             .levelCount = 1,
      //      //             .baseArrayLayer = 0,
      //      //             .layerCount = 1,
      //      //         },
      //      //               };
      //      //
      //      //               vkCmdPipelineBarrier(
      //      //                  pcommandbuffer->m_vkcommandbuffer,
      //      //                  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // srcStageMask
      //      //                  VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,         // dstStageMask
      //      //                  0,
      //      //                  0, NULL,
      //      //                  0, NULL,
      //      //                  1, &barrier
      //      //               );
      //      //
      //      //            }
      //      //
      //      //         }

      //if(1)
      //      {


      //   VkImageMemoryBarrier barrier = {
      //       .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      //       .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      //       .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
      //       .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //       .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //       .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //       .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //       .image = image,
      //       .subresourceRange = {
      //           .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //           .baseMipLevel = 0,
      //           .levelCount = 1,
      //           .baseArrayLayer = 0,
      //           .layerCount = 1
      //       },
      //   };

      //   vkCmdPipelineBarrier(
      //      pcommandbuffer->m_vkcommandbuffer,
      //      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      //      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      //      0,
      //      0, NULL,
      //      0, NULL,
      //      1, &barrier
      //   );

      //      }

      /*   }
         );*/

      ::cast < context > pcontext = m_pgpucontext;

      auto& pmodelbuffer = m_imagemodelbuffer[ptexture->m_vkimage];

      if (__defer_construct_new(pmodelbuffer))
      {

         pmodelbuffer->sequence2_uv_create_rectangle(pcontext, true, bYSwap);
         //create_quad_buffers(m_pgpucontext->logicalDevice(),
         //   m_pgpucontext->m_pgpudevice->m_pphysicaldevice->m_physicaldevice,
         //   &pmodel->m_vertexBuffer,
         //   &pmodel->m_vertexMemory,
         //   &pmodel->m_indexBuffer,
         //   &pmodel->m_indexMemory,
         //   bYSwap);

      }

      auto pshader = _get_image_blend_shader();

      pshader->bind(m_pgpurendertarget->current_texture(), ptexture);

      // pshader->_bind_sampler(ptexture->m_vkimage, 0);

      //  auto& pdescriptor = m_imagedescriptor[ptexture->m_vkimage];

      //  if (__defer_construct_new(pdescriptor))
      //  {

      //     pdescriptor->m_descriptorsets.set_size(get_frame_count());

      //     //     VkImageViewCreateInfo viewInfo = {
      //     //.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      //     //.image = image,  // <-- Your existing VkImage
      //     //.viewType = VK_IMAGE_VIEW_TYPE_2D,
      //     //.format = VK_FORMAT_B8G8R8A8_UNORM,  // <-- Match your image's format
      //     //.components = {
      //     //    .r = VK_COMPONENT_SWIZZLE_IDENTITY,
      //     //    .g = VK_COMPONENT_SWIZZLE_IDENTITY,
      //     //    .b = VK_COMPONENT_SWIZZLE_IDENTITY,
      //     //    .a = VK_COMPONENT_SWIZZLE_IDENTITY,
      //     //},
      //     //.subresourceRange = {
      //     //    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //     //    .baseMipLevel = 0,
      //     //    .levelCount = 1,
      //     //    .baseArrayLayer = 0,
      //     //    .layerCount = 1,
      //     //},
      //     //     };

      //     //     VkImageView imageView;
      //     //     if (vkCreateImageView(m_pgpucontext->logicalDevice(), &viewInfo, NULL, &imageView) != VK_SUCCESS) {
      //     //        // Handle error
      //     //     }

      //     ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;
      //     ::cast < accumulation_render_pass > ppass = pgpurenderpassSrc;

      //     for (int i = 0; i < get_frame_count(); i++)
      //     {

      //        VkDescriptorImageInfo imageinfo;

      //        auto imageview = ppass->m_imageviews[i];

      //        imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      //        imageinfo.imageView = imageview;
      //        imageinfo.sampler = m_pgpucontext->_001VkSampler();

      //        VkDescriptorImageInfo imageinfoAlpha;

      //        auto imageviewAlpha = ppass->m_imageviewsAlphaAccumulation[i];

      //        imageinfoAlpha.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      //        imageinfoAlpha.imageView = imageviewAlpha;
      //        imageinfoAlpha.sampler = m_pgpucontext->_001VkSampler();


      //        auto& playout = m_psetdescriptorlayoutImageBlend;

      //        auto& ppool = m_pdescriptorpoolImageBlend;

      //        descriptor_writer(*playout, *ppool)
      //           .writeImage(0, &imageinfo)
      //           .writeImage(0, &imageinfoAlpha)
      //           .build(pdescriptor->m_descriptorsets[i]);

      //     }

      //     auto descriptorSetLayout = m_psetdescriptorlayoutImageBlend->getDescriptorSetLayout();

      //     VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
      //  .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
      //  .setLayoutCount = 1,
      //  .pSetLayouts = &descriptorSetLayout,
      //     };

      //     //VkPipelineLayout pipelineLayout;
      //     if (vkCreatePipelineLayout(m_pgpucontext->logicalDevice(), &pipelineLayoutInfo, NULL, &pdescriptor->m_vkpipelinelayout) != VK_SUCCESS) {
      //        // Handle error
      //     }

      //  }

      //  //auto pcommandbuffer = this->getCurrentCommandBuffer();

      //  // Bind pipeline and descriptor sets
      ////      vkCmdBindPipeline(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
      //  //    vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, ...);
      //  vkCmdBindDescriptorSets(
      //     pcommandbuffer->m_vkcommandbuffer,
      //     VK_PIPELINE_BIND_POINT_GRAPHICS,   // Bind point
      //     pdescriptor->m_vkpipelinelayout,                     // Layout used when pipeline was created
      //     0,                                  // First set (set = 0)
      //     1,                                  // Descriptor set count
      //     &pdescriptor->m_descriptorsets[get_frame_index()],                     // Pointer to descriptor set
      //     0,                                  // Dynamic offset count
      //     NULL                                // Dynamic offsets
      //  );

      pmodelbuffer->bind(pcommandbuffer);

      //VkDeviceSize offsets[] = { 0 };
      //vkCmdBindVertexBuffers(pcommandbuffer->m_vkcommandbuffer, 0, 1, &pmodel->m_vertexBuffer, offsets);
      //vkCmdBindIndexBuffer(pcommandbuffer->m_vkcommandbuffer, pmodel->m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);


      //VkViewport vp = {
      //   (float)rectanglePlacement.left(),
      //   (float)rectanglePlacement.top(),
      //   (float)rectanglePlacement.width(),
      //   (float)rectanglePlacement.height(),
      //   0.0f, 1.0f };

      //VkRect2D sc = {
      //   {
      //      rectanglePlacement.left(),
      //      rectanglePlacement.top(),
      //   },
      //   {
      //      (uint32_t)rectanglePlacement.width(),
      //      (uint32_t)rectanglePlacement.height(),
      //   }
      //};

      //vkCmdSetViewport(pcommandbuffer->m_vkcommandbuffer, 0, 1, &vp);
      //vkCmdSetScissor(pcommandbuffer->m_vkcommandbuffer, 0, 1, &sc);

      pcommandbuffer->set_viewport(rectanglePlacement);
      pcommandbuffer->set_scissor(rectanglePlacement);

      pmodelbuffer->draw(pcommandbuffer);
      // Draw full-screen quad
      //vkCmdDraw(pcommandbuffer->m_vkcommandbuffer, 6, 1, 0, 0); // assuming full-screen triangle/quad

      pshader->unbind();

      //vkCmdEndRenderPass(...);

      vkQueueWaitIdle(m_pgpucontext->graphicsQueue());

      //vkFreeCommandBuffers(m_pgpucontext->logicalDevice(), m_pgpucontext->m_vkcommandpool, 1, &pcommandbuffer->m_vkcommandbuffer);


   }


   void renderer::_on_begin_render()
   {

      //::cast < frame > pframe = pframeParam;

      //auto pcommandbuffer = pframe->m_pcommandbuffer->m_vkcommandbuffer;

      ::cast < command_buffer > pcommandbuffer = this->getCurrentCommandBuffer2();

      ::cast < render_pass > pgpurenderpass = m_pgpurendertarget;

      //if (m_bOffScreen)
      {

         assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
         assert(
            pcommandbuffer == getCurrentCommandBuffer2() &&
            "Can't begin render pass on command buffer from a different frame");




         //VkRenderPassBeginInfo renderPassInfo{};
         //renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
         //renderPassInfo.renderPass = pgpurenderpass->getRenderPass();
         //renderPassInfo.framebuffer = pgpurenderpass->getCurrentFrameBuffer();

         //renderPassInfo.renderArea.offset = { 0, 0 };
         //renderPassInfo.renderArea.extent = pgpurenderpass->getExtent();

         //VkClearValue clearValues[2];
         ////clearValues[0].color = { 2.01f, 0.01f, 0.01f, 1.0f };
         //clearValues[0].color = { 0.f, 0.0f, 0.0f, 0.0f };
         //clearValues[1].depthStencil = { 1.0f, 0 };
         //renderPassInfo.clearValueCount = 2;
         //renderPassInfo.pClearValues = clearValues;

         ////vkCmdBeginRenderPass(pcommandbuffer->m_vkcommandbuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

         VkViewport viewport{};
         viewport.x = 0.0f;
         viewport.y = 0.0f;
         viewport.width = static_cast<float>(pgpurenderpass->getExtent().width);
         viewport.height = static_cast<float>(pgpurenderpass->getExtent().height);
         viewport.minDepth = 0.0f;
         viewport.maxDepth = 1.0f;
         VkRect2D scissor{ {0, 0}, pgpurenderpass->getExtent() };
         vkCmdSetViewport(pcommandbuffer->m_vkcommandbuffer, 0, 1, &viewport);
         vkCmdSetScissor(pcommandbuffer->m_vkcommandbuffer, 0, 1, &scissor);

      }
      //else
      //{

      //	assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
      //	assert(
      //		pcommandbuffer == getCurrentCommandBuffer() &&
      //		"Can't begin render pass on command buffer from a different frame");

      //	VkRenderPassBeginInfo renderPassInfo{};
      //	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      //	renderPassInfo.renderPass = m_pvkcswapchain->getRenderPass();
      //	renderPassInfo.framebuffer = m_pvkcswapchain->getFrameBuffer(m_uCurrentSwapChainImage);

      //	renderPassInfo.renderArea.offset = { 0, 0 };
      //	renderPassInfo.renderArea.extent = m_pvkcswapchain->getExtent();

      //	std::array<VkClearValue, 2> clearValues{};
      //	clearValues[0].color = { 2.01f, 0.01f, 0.01f, 1.0f };
      //	clearValues[1].depthStencil = { 1.0f, 0 };
      //	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      //	renderPassInfo.pClearValues = clearValues.data();

      //	vkCmdBeginRenderPass(pcommandbuffer->m_vkcommandbuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      //	VkViewport viewport{};
      //	viewport.x = 0.0f;
      //	viewport.y = 0.0f;
      //	viewport.width = static_cast<float>(vkcSwapChain->getSwapChainExtent().width);
      //	viewport.height = static_cast<float>(vkcSwapChain->getSwapChainExtent().height);
      //	viewport.minDepth = 0.0f;
      //	viewport.maxDepth = 1.0f;
      //	VkRect2D scissor{ {0, 0}, vkcSwapChain->getSwapChainExtent() };
      //	vkCmdSetViewport(pcommandbuffer->m_vkcommandbuffer, 0, 1, &viewport);
      //	vkCmdSetScissor(pcommandbuffer->m_vkcommandbuffer, 0, 1, &scissor);


      //}
   }


   void renderer::_on_begin_render(::gpu::frame* pframeParam)
   {

      ::cast < frame > pframe = pframeParam;

      auto pcommandbuffer = pframe->m_pcommandbuffer;

      if (m_papplication->m_gpu.m_bUseSwapChainWindow)
      {

         ::cast < context > pcontextMain = m_pgpucontext->m_pgpudevice->main_context();

         ::cast < ::gpu_vulkan::swap_chain > pswapchain = pcontextMain->get_swap_chain();

         if (pswapchain)
         {
            pswapchain->m_stageaWaitToSubmit.clear();
            pswapchain->m_semaphoreaWaitToSubmit.clear();

         }

      }

      //pgpurenderpass->m_iFrameSerial++;

      //pgpurenderpass->m_iCurrentFrame = (pgpurenderpass->m_iCurrentFrame + 1) % 
      //   get_frame_count();

      ::cast < render_pass > pgpurenderpass = m_pgpurendertarget;

      ::cast < texture > ptexture = pgpurenderpass->current_texture();

      {

         ptexture->_new_state(
            pcommandbuffer,
            VK_ACCESS_TRANSFER_WRITE_BIT,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_PIPELINE_STAGE_TRANSFER_BIT
         );

         VkClearColorValue clearColor = { .float32 = { 0.0f, 0.0f, 0.0f, 0.0f } };

         VkImageSubresourceRange range = {
             .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
             .baseMipLevel = 0,
             .levelCount = 1,
             .baseArrayLayer = 0,
             .layerCount = 1,
         };
         vkCmdClearColorImage(pcommandbuffer->m_vkcommandbuffer,
            ptexture->m_vkimage,
            ptexture->m_vkimagelayout,
            &clearColor,
            1, // rangeCount
            &range
         );

      }

      ::cast < texture > ptextureDepth;

      if (m_pgpulayer)
      {

         ptextureDepth = ptexture->get_depth_texture();

      }
      else
      {

         ptextureDepth = pgpurenderpass->current_depth_texture();

      }

      if(ptextureDepth)
      {

         ptextureDepth->_new_state(
            pcommandbuffer,
            VK_ACCESS_TRANSFER_WRITE_BIT,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_PIPELINE_STAGE_TRANSFER_BIT
         );
         
         VkClearDepthStencilValue clearValue = {
            .depth = 1.0f,
            .stencil = 0
         };

         VkImageSubresourceRange range = {
             .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
             .baseMipLevel = 0,
             .levelCount = 1,
             .baseArrayLayer = 0,
             .layerCount = 1
         };

         vkCmdClearDepthStencilImage(
            pcommandbuffer->m_vkcommandbuffer,
            ptextureDepth->get_depth_image(),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,  // Must be in a writable layout
            &clearValue,
            1,
            &range
         );

      }

      pgpurenderpass->on_before_begin_render(pframe);

      //if (m_bOffScreen)
      {

         assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
         assert(
            pcommandbuffer == getCurrentCommandBuffer2() &&
            "Can't begin render pass on command buffer from a different frame");

         //if (pgpurenderpass->getRenderPass())
         //{
         //   VkRenderPassBeginInfo renderPassInfo{};
         //   renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
         //   renderPassInfo.renderPass = pgpurenderpass->getRenderPass();
         //   renderPassInfo.framebuffer = pgpurenderpass->getCurrentFrameBuffer();

         //   renderPassInfo.renderArea.offset = { 0, 0 };
         //   renderPassInfo.renderArea.extent = pgpurenderpass->getExtent();

         //   VkClearValue clearValues[2];
         //   //clearValues[0].color = { 2.01f, 0.01f, 0.01f, 1.0f };
         //   clearValues[0].color = { 0.0f, 0.0f, 0.0f, 0.0f };
         //   clearValues[1].depthStencil = { 1.0f, 0 };
         //   renderPassInfo.clearValueCount = 2;
         //   renderPassInfo.pClearValues = clearValues;

         //   //vkCmdBeginRenderPass(pcommandbuffer->m_vkcommandbuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

         //}

         VkViewport viewport{};
         viewport.x = 0.0f;
         viewport.y = 0.0f;
         viewport.width = static_cast<float>(pgpurenderpass->getExtent().width);
         viewport.height = static_cast<float>(pgpurenderpass->getExtent().height);
         viewport.minDepth = 0.0f;
         viewport.maxDepth = 1.0f;
         VkRect2D scissor{ {0, 0}, pgpurenderpass->getExtent() };
         vkCmdSetViewport(pcommandbuffer->m_vkcommandbuffer, 0, 1, &viewport);
         vkCmdSetScissor(pcommandbuffer->m_vkcommandbuffer, 0, 1, &scissor);

      }
      //else
      //{

      //	assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
      //	assert(
      //		pcommandbuffer == getCurrentCommandBuffer() &&
      //		"Can't begin render pass on command buffer from a different frame");

      //	VkRenderPassBeginInfo renderPassInfo{};
      //	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      //	renderPassInfo.renderPass = m_pvkcswapchain->getRenderPass();
      //	renderPassInfo.framebuffer = m_pvkcswapchain->getFrameBuffer(m_uCurrentSwapChainImage);

      //	renderPassInfo.renderArea.offset = { 0, 0 };
      //	renderPassInfo.renderArea.extent = m_pvkcswapchain->getExtent();

      //	std::array<VkClearValue, 2> clearValues{};
      //	clearValues[0].color = { 2.01f, 0.01f, 0.01f, 1.0f };
      //	clearValues[1].depthStencil = { 1.0f, 0 };
      //	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      //	renderPassInfo.pClearValues = clearValues.data();

      //	vkCmdBeginRenderPass(pcommandbuffer->m_vkcommandbuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      //	VkViewport viewport{};
      //	viewport.x = 0.0f;
      //	viewport.y = 0.0f;
      //	viewport.width = static_cast<float>(vkcSwapChain->getSwapChainExtent().width);
      //	viewport.height = static_cast<float>(vkcSwapChain->getSwapChainExtent().height);
      //	viewport.minDepth = 0.0f;
      //	viewport.maxDepth = 1.0f;
      //	VkRect2D scissor{ {0, 0}, vkcSwapChain->getSwapChainExtent() };
      //	vkCmdSetViewport(pcommandbuffer->m_vkcommandbuffer, 0, 1, &viewport);
      //	vkCmdSetScissor(pcommandbuffer->m_vkcommandbuffer, 0, 1, &scissor);


      //}
      //on_happening(e_happening_begin_render);
   }


   void renderer::on_end_render(::gpu::frame* pframeParam)
   {

      //m_prenderstate->on_happening(::gpu::e_happening_end_render);

      ::cast < frame > pframe = pframeParam;

      auto pcommandbuffer = pframe->m_pcommandbuffer;

      assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
      //assert(
        // pcommandbuffer == getCurrentCommandBuffer2() &&
         //"Can't end render pass on command buffer from a different frame");
      //vkCmdEndRenderPass(pcommandbuffer->m_vkcommandbuffer);

      //{

      //   ::cast < texture > ptexture = m_pgpurendertarget->current_texture();

      //   VkClearColorValue clearColor = {
      //         .float32 = { 0.5f, 0.0f, 0.5f, 0.5f }  // R, G, B, A
      //   };

      //   VkImageSubresourceRange subresourceRange = {
      //       .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //       .baseMipLevel = 0,
      //       .levelCount = 1,
      //       .baseArrayLayer = 0,
      //       .layerCount = 1,
      //   };

      //   vkCmdClearColorImage(
      //      pcommandbuffer->m_vkcommandbuffer,
      //      ptexture->m_vkimage,
      //      ptexture->m_vkimagelayout,
      //      &clearColor,
      //      1,
      //      &subresourceRange
      //   );

      //}

      //{

      //   {

      //      if (m_pgpucontext->m_pgpudevice->m_iLayer == 0)
      //      {

      //         VkClearAttachment clearAttachment = {
      //            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //            .colorAttachment = 0,
      //            .clearValue={.color = {0.5f, 0.0f, 0.0f, 0.5f}} // Red
      //         };

      //         VkClearRect clearRect = {
      //             .rect = {
      //                 .offset = {100, 100},
      //                 .extent = {100, 100}
      //             },
      //             .baseArrayLayer = 0,
      //             .layerCount = 1
      //         };

      //         vkCmdClearAttachments(
      //            pcommandbuffer->m_vkcommandbuffer,
      //            1,
      //            &clearAttachment,
      //            1,
      //            &clearRect);

      //      }
      //      else if (m_pgpucontext->m_pgpudevice->m_iLayer == 1)
      //      {

      //         VkClearAttachment clearAttachment = {
      //            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //            .colorAttachment = 0,
      //            .clearValue = {.color = {0.0f, 0.5f, 0.0f, 0.5f} } // Green
      //         };

      //         VkClearRect clearRect = {
      //             .rect = {
      //                 .offset = {200, 100},
      //                 .extent = {100, 100}
      //             },
      //             .baseArrayLayer = 0,
      //             .layerCount = 1
      //         };

      //         vkCmdClearAttachments(
      //            pcommandbuffer->m_vkcommandbuffer,
      //            1,
      //            &clearAttachment,
      //            1,
      //            &clearRect);


      //      }
      //      else if (m_pgpucontext->m_pgpudevice->m_iLayer == 2)
      //      {

      //         VkClearAttachment clearAttachment = {
      //            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //            .colorAttachment = 0,
      //            .clearValue = {.color = {0.0f, 0.0f, 0.5f, 0.5f} } // Blue
      //         };

      //         VkClearRect clearRect = {
      //             .rect = {
      //                 .offset = {200, 100},
      //                 .extent = {100, 100}
      //             },
      //             .baseArrayLayer = 0,
      //             .layerCount = 1
      //         };

      //         vkCmdClearAttachments(
      //            pcommandbuffer->m_vkcommandbuffer,
      //            1,
      //            &clearAttachment,
      //            1,
      //            &clearRect);

      //      }
  
      //   }

      //}


      gpu::renderer::on_end_render(pframe);

      if (m_iSentLayerCount > 0)
      {

         ::cast < context > pcontextMain = m_pgpucontext->m_pgpudevice->m_pgpucontextMain;

         ::cast < ::gpu_vulkan::swap_chain > pswapchain = pcontextMain->get_swap_chain();

         ::cast < layer > playerLast = pframe->m_pgpulayer;

         auto vksemaphoreRenderFinished = playerLast->m_vksemaphoreRenderFinished;

         pswapchain->m_stageaWaitToSubmit.add(VK_PIPELINE_STAGE_TRANSFER_BIT);
         pswapchain->m_semaphoreaWaitToSubmit.add(vksemaphoreRenderFinished);

      }

   }


   //void renderer::on_end_render(::graphics3d::frame * pframeParam)
   void renderer::_on_end_render()
   {

      //::cast < frame > pframe = pframeParam;

      //auto pcommandbuffer = pframe->m_pcommandbuffer->m_vkcommandbuffer;

      ::cast < command_buffer > pcommandbuffer = getCurrentCommandBuffer2();

      assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
      assert(
         pcommandbuffer == getCurrentCommandBuffer2() &&
         "Can't end render pass on command buffer from a different frame");
      vkCmdEndRenderPass(pcommandbuffer->m_vkcommandbuffer);
   }


   ::pointer < ::gpu::frame > renderer::beginFrame()
   {

      //defer_layout();

      if (m_commandbuffera.is_empty())
      {

         create_command_buffers();

      }

      ::cast < render_pass > pgpurenderpass = m_pgpurendertarget;

      assert(!isFrameStarted && "Can't call beginFrame while already in progress");

      //if (m_bOffScreen)
      {

         auto result = pgpurenderpass->acquireNextImage();

         if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            //defer_layout();
            pgpurenderpass->init();
            //throw ::exception(todo, "resize happened?!?!");
            return nullptr;
         }
         if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw ::exception(error_failed, "Failed to aquire swap chain image");
         }

         isFrameStarted = true;

         ::cast < command_buffer > pcommandbuffer = getCurrentCommandBuffer2();


         pcommandbuffer->begin_command_buffer(false);
         //VkCommandBufferBeginInfo beginInfo{};
         //beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

         //if (vkBeginCommandBuffer(pcommandbuffer->m_vkcommandbuffer, &beginInfo) != VK_SUCCESS)
         //{

         //   throw ::exception(error_failed, "failed to begin recording command buffer!");

         //}

         //pcommandbuffer->m_estate = ::gpu::command_buffer::e_state_recording;

         auto pframe = __create_new < ::gpu_vulkan::frame >();

         pframe->m_pcommandbuffer = pcommandbuffer.m_p;

         m_pgpurendertarget->m_pframe = pframe;

         return ::gpu::renderer::beginFrame();


      }
      //else
      //{


      //	auto result = m_pvkcswapchain->acquireNextImage(&m_uCurrentSwapChainImage);

      //	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      //		recreateRenderPass();
      //		return nullptr;
      //	}
      //	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      //		throw ::exception(error_failed, "Failed to aquire swap chain image");
      //	}

      //	isFrameStarted = true;

      //	auto pcommandbuffer = getCurrentCommandBuffer();

      //	VkCommandBufferBeginInfo beginInfo{};
      //	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      //	if (vkBeginCommandBuffer(pcommandbuffer->m_vkcommandbuffer, &beginInfo) != VK_SUCCESS) {
      //		throw ::exception(error_failed, "failed to begin recording command buffer!");
      //	}
      //	return pcommandbuffer->m_vkcommandbuffer;

      //}
      m_prenderstate->on_happening(::gpu::e_happening_begin_frame);
   }


   void renderer::endFrame()
   {

      m_prenderstate->on_happening(::gpu::e_happening_end_frame);

      //if (m_pgpucontext->m_eoutput == ::gpu::e_output_cpu_buffer)
      //{

      assert(isFrameStarted && "Can't call endFrame while frame is not in progress");

      ::cast < command_buffer > pcommandbuffer = getCurrentCommandBuffer2();

      auto eoutput = m_pgpucontext->m_eoutput;

      if (eoutput == ::gpu::e_output_swap_chain)
      {

         ::cast < ::gpu_vulkan::swap_chain > pswapchain = m_pgpucontext->get_swap_chain();

         ::cast < texture  > ptexture = pswapchain->current_texture();

         ptexture->_new_state(
            pcommandbuffer,
            0,
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT
         );

      }

      if (pcommandbuffer->m_estate == ::gpu::command_buffer::e_state_recording)
      {

         auto vkcommandbuffer = pcommandbuffer->m_vkcommandbuffer;

         if (vkEndCommandBuffer(vkcommandbuffer) != VK_SUCCESS)
         {

            throw ::exception(error_failed, "failed to record command buffer!");

         }

      }

      ::cast < render_pass > pgpurenderpass;

      if (eoutput == ::gpu::e_output_swap_chain)
      {

         ::cast < ::gpu_vulkan::swap_chain > pswapchain = m_pgpucontext->get_swap_chain();

         if (!pswapchain->::gpu_vulkan::render_pass::m_pgpurenderer)
         {

            pswapchain->initialize_render_target(
               this,
               m_pgpucontext->m_rectangle.size());

         }

         pgpurenderpass = pswapchain;

      }
      else
      {

         pgpurenderpass = m_pgpurendertarget;

      }

      if (pcommandbuffer->m_estate == ::gpu::command_buffer::e_state_recording)
      {

         auto result = pgpurenderpass->submitCommandBuffers(
            pcommandbuffer,
            {}, {}, {});

         if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
            m_bNeedToRecreateSwapChain)
         {

            m_bNeedToRecreateSwapChain = false;

            defer_update_renderer();

         }
         else if (result != VK_SUCCESS)
         {

            throw ::exception(error_failed, "failed to present swap chain image!");

         }

      }

      isFrameStarted = false;

      if (m_pgpucontext->m_eoutput == ::gpu::e_output_cpu_buffer)
      {

         sample();

      }
      //else if (m_eoutput == ::gpu::e_output_color_and_alpha_accumulation_buffers)
   //{

   //	resolve_color_and_alpha_accumulation_buffers();

   //}

   //rrentImageIndex = pgpurenderpass->currentFrame;
   //currentFrameIndex = (currentFrameIndex + 1) % ::gpu_vulkan::render_pass::MAX_FRAMES_IN_FLIGHT;

   //}
   //else
   //{


   //	assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
   //	auto pcommandbuffer = getCurrentCommandBuffer();
   //	if (vkEndCommandBuffer(pcommandbuffer->m_vkcommandbuffer) != VK_SUCCESS) {
   //		throw ::exception(error_failed, "failed to record command buffer!");
   //	}
   //	auto result = m_pvkcswapchain->submitCommandBuffers(&pcommandbuffer->m_vkcommandbuffer, &m_uCurrentSwapChainImage);
   //	//if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
   //	//	vkcWindow.wasWindowResized()) 
   //	//{
   //	//	vkcWindow.resetWindowResizedFlag();
   //	//	recreateSwapchain();
   //	//}
   //	//else 
   //	//	if (result != VK_SUCCESS) {
   //	//	throw ::exception(error_failed, "failed to present swap chain image!");
   //	//}
   //	isFrameStarted = false;
   //	currentFrameIndex = (currentFrameIndex + 1) % swap_chain_render_pass::MAX_FRAMES_IN_FLIGHT;

   //}


   }


   //void renderer::on_end_layer(::gpu::layer* player)
   //{

   //   m_pgpucontext->on_end_layer(player);

   //   auto ptextureTarget = player->texture();

   //   auto ptextureSource = m_pgpurendertarget->current_texture();

   //   m_pgpucontext->copy(ptextureTarget, ptextureSource);

   //   auto pcommandbuffer = getCurrentCommandBuffer2();

   //   if (pcommandbuffer)
   //   {

   //      pcommandbuffer->submit_command_buffer();

   //      pcommandbuffer->wait_commands_to_execute();

   //   }

   //}


   void renderer::_on_graphics_end_draw(::gpu::texture* pgputexture, const ::int_rectangle& rectangle)
   {

      m_pgpucontext->set_placement(rectangle);

      on_new_frame();

      ::cast <texture > ptexture = pgputexture;

      if (1)
      {

         auto pcommandbuffer = m_pgpucontext->beginSingleTimeCommands();

         VkImageMemoryBarrier barrier = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .srcAccessMask = ptexture->m_vkaccessflags,
            .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
            .oldLayout = ptexture->m_vkimagelayout,
            .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = ptexture->m_vkimage,
            .subresourceRange = {
               .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
               .baseMipLevel = 0,
               .levelCount = 1,
               .baseArrayLayer = 0,
               .layerCount = 1
            },
         };

         ptexture->m_vkaccessflags = VK_ACCESS_SHADER_READ_BIT;

         ptexture->m_vkimagelayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

         vkCmdPipelineBarrier(
            pcommandbuffer->m_vkcommandbuffer,
            ptexture->m_vkpipelinestageflags,
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            0,
            0, NULL,
            0, NULL,
            1, &barrier
         );

         ptexture->m_vkpipelinestageflags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

         m_pgpucontext->endSingleTimeCommands(pcommandbuffer);

      }

      if (auto pframe = beginFrame())
      {

         //on_begin_frame();
         // render
         on_begin_render(pframe);



         //if (m_pimpact->global_ubo_block().size() > 0)
         //{

           // update_global_ubo(m_pgpucontext);

         //}

         //m_pscene->on_render(m_pgpucontext);

         _blend_image(pgputexture, rectangle, false);

         on_end_render(pframe);

         endFrame();

      }

      if (1)
      {
         auto pcommandbuffer = m_pgpucontext->beginSingleTimeCommands();

         VkImageMemoryBarrier barrier = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .srcAccessMask = ptexture->m_vkaccessflags,
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            .oldLayout = ptexture->m_vkimagelayout,
            .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = ptexture->m_vkimage,
            .subresourceRange = {
               .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
               .baseMipLevel = 0,
               .levelCount = 1,
               .baseArrayLayer = 0,
               .layerCount = 1
            },
         };

         ptexture->m_vkaccessflags = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
         ptexture->m_vkimagelayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

         vkCmdPipelineBarrier(
            pcommandbuffer->m_vkcommandbuffer,
            ptexture->m_vkpipelinestageflags,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            0,
            0, NULL,
            0, NULL,
            1, &barrier
         );

         ptexture->m_vkpipelinestageflags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

         m_pgpucontext->endSingleTimeCommands(pcommandbuffer);

      }

   }


   void renderer::_on_graphics_end_draw(::gpu_vulkan::renderer* prendererSrc)
   {

      ::gpu_vulkan::renderer* prenderer;

      //if (prendererSrc->m_prendererResolve)
      //{

      //	prenderer = prendererSrc->m_prendererResolve;

      //}
      //else
      {

         prenderer = prendererSrc;

      }

      m_pgpucontext->set_placement(prenderer->m_pgpucontext->rectangle());

      ::cast < render_pass > pgpurenderpass = prenderer->m_pgpurendertarget;

      ::cast < texture > ptexture = pgpurenderpass->m_texturea[prenderer->m_pgpurendertarget->get_frame_index()];

      defer_update_renderer();

      on_new_frame();

      if (1)
      {
         //auto pcommandbuffer = m_pgpucontext->beginSingleTimeCommands();

         //VkImageMemoryBarrier barrier = {
         //    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
         //    .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
         //    .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
         //    .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
         //    .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
         //    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
         //    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
         //    .image = image,
         //    .subresourceRange = {
         //        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
         //        .baseMipLevel = 0,
         //        .levelCount = 1,
         //        .baseArrayLayer = 0,
         //        .layerCount = 1
         //    },
         //};

         //vkCmdPipelineBarrier(
         //   pcommandbuffer->m_vkcommandbuffer,
         //   VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
         //   VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
         //   0,
         //   0, NULL,
         //   0, NULL,
         //   1, &barrier
         //);

         //VkSubmitInfo submitInfo = {};
         //submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

         //VkSemaphore waitSemaphores[] = { prendererSrc->pgpurenderpass->renderFinishedSemaphores[prendererSrc->pgpurenderpass->currentFrame] };
         //VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
         //submitInfo.waitSemaphoreCount = 1;
         //submitInfo.pWaitSemaphores = waitSemaphores;
         //submitInfo.pWaitDstStageMask = waitStages;

         //submitInfo.commandBufferCount = 1;
         //submitInfo.pCommandBuffers = &pcommandbuffer->m_vkcommandbuffer;

         //m_pgpucontext->endSingleTimeCommands(pcommandbuffer->m_vkcommandbuffer, 1, &submitInfo);
         auto pcommandbuffer = m_pgpucontext->beginSingleTimeCommands();


         insertImageMemoryBarrier(pcommandbuffer->m_vkcommandbuffer,
            ptexture->m_vkimage,
            0,
            VK_ACCESS_TRANSFER_WRITE_BIT,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });



         VkSubmitInfo submitInfo{};
         submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
         submitInfo.commandBufferCount = 1;
         submitInfo.pCommandBuffers = &pcommandbuffer->m_vkcommandbuffer;
         ::array<VkSemaphore> waitSemaphores;
         ::array<VkPipelineStageFlags> waitStages;
         waitStages.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
         ::cast < render_pass > pgpurenderpass = prenderer->m_pgpurendertarget;
         waitSemaphores.add(pgpurenderpass->renderFinishedSemaphores[prenderer->m_pgpurendertarget->get_frame_index()]);
         submitInfo.waitSemaphoreCount = (uint32_t)waitSemaphores.size();
         submitInfo.pWaitSemaphores = waitSemaphores.data();
         submitInfo.pWaitDstStageMask = waitStages.data();

         m_pgpucontext->endSingleTimeCommands(pcommandbuffer, 1, &submitInfo);

         //m_prendererResolve->pgpurenderpass->m_semaphoreaWaitToSubmit.add(
         //   pgpurenderpass->renderFinishedSemaphores[iPassCurrentFrame]
         //);
         //m_prendererResolve->pgpurenderpass->m_stageaWaitToSubmit.add(
         //   VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
         //);


      }

      if (auto pframe = beginFrame())
      {

         ::cast < render_pass > pgpurenderpassSrc = prendererSrc->m_pgpurendertarget;

         pgpurenderpass->m_semaphoreaSignalOnSubmit.add(pgpurenderpassSrc->imageAvailableSemaphores[prendererSrc->m_pgpurendertarget->get_frame_index()]);


         //on_begin_frame();
         // render
         on_begin_render(pframe);



         //if (m_pimpact->global_ubo_block().size() > 0)
         //{

           // update_global_ubo(m_pgpucontext);

         //}

         //m_pscene->on_render(m_pgpucontext);

         _blend_image(ptexture, m_pgpucontext->rectangle(), false);

         on_end_render(pframe);

         endFrame();

      }

      //if (1)
      //{
      //   auto pcommandbuffer = m_pgpucontext->beginSingleTimeCommands();

      //   VkImageMemoryBarrier barrier = {
      //       .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      //       .srcAccessMask = VK_ACCESS_SHADER_READ_BIT,
      //      .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      //      .oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //       .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //       .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //       .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //       .image = image,
      //       .subresourceRange = {
      //           .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //           .baseMipLevel = 0,
      //           .levelCount = 1,
      //           .baseArrayLayer = 0,
      //           .layerCount = 1
      //       },
      //   };

      //   vkCmdPipelineBarrier(
      //      pcommandbuffer->m_vkcommandbuffer,
      //      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      //      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      //      0,
      //      0, NULL,
      //      0, NULL,
      //      1, &barrier
      //   );
      //   m_pgpucontext->endSingleTimeCommands(pcommandbuffer->m_vkcommandbuffer);
      //}


   }


   //void renderer::_on_frame_draw(::gpu_vulkan::renderer* prendererUpper)
   //{




   //}


   //void renderer::endDraw(::draw2d_gpu::graphics* pgraphics, ::user::interaction* puserinteraction)
   //{

   //   ::cast < renderer > prenderer = this;

   //   ::cast < render_pass > pgpurenderpass = prenderer->m_pgpurendertarget;

   //   ::cast <texture > ptexture = pgpurenderpass->m_texturea[prenderer->m_pgpurendertarget->get_frame_index()];

   //   ::int_rectangle rectangle = prenderer->m_pgpucontext->rectangle();

   //   auto pcommandbuffer = m_pgpucontext->beginSingleTimeCommands();

   //   insertImageMemoryBarrier(
   //      pcommandbuffer->m_vkcommandbuffer,
   //      ptexture->m_vkimage,
   //      0,
   //      VK_ACCESS_TRANSFER_WRITE_BIT,
   //      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //      VK_PIPELINE_STAGE_TRANSFER_BIT,
   //      VK_PIPELINE_STAGE_TRANSFER_BIT,
   //      VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

   //   VkSubmitInfo submitInfo{};
   //   submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
   //   submitInfo.commandBufferCount = 1;
   //   submitInfo.pCommandBuffers = &pcommandbuffer->m_vkcommandbuffer;
   //   ::array<VkSemaphore> waitSemaphores;
   //   ::array<VkPipelineStageFlags> waitStages;
   //   waitStages.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
   //   ::cast < render_pass > pgpurenderpass2 = prenderer->m_pgpurendertarget;
   //   waitSemaphores.add(pgpurenderpass2->renderFinishedSemaphores[prenderer->m_pgpurendertarget->get_frame_index()]);
   //   submitInfo.waitSemaphoreCount = (uint32_t)waitSemaphores.size();
   //   submitInfo.pWaitSemaphores = waitSemaphores.data();
   //   submitInfo.pWaitDstStageMask = waitStages.data();

   //   m_pgpucontext->endSingleTimeCommands(pcommandbuffer);

   //   ::cast < ::gpu_vulkan::swap_chain > pswapchain = m_pgpucontext->m_pgpudevice->get_swap_chain();

   //   pswapchain->endDraw(pgraphics, puserinteraction, this);

   //   //defer_update_renderer();

   //   //on_new_frame();

   //   //auto pframe = beginFrame();

   //   //on_begin_render(pframe);

   //   //_copy_image(vkimage, rectangle, false);

   //   //on_end_render(pframe);

   //   //endFrame();

   //   ////vkQueueWaitIdle(m_pgpucontext->graphicsQueue());

   //   ////vkQueueWaitIdle(m_pgpucontext->presentQueue());

   //}


   void renderer::blend(::gpu::renderer* prendererSourceParam)
   {

      ::cast < ::gpu_vulkan::renderer > prendererSource = prendererSourceParam;

      ::cast < render_pass > pgpurenderpassSource = prendererSource->m_pgpurendertarget;

      ::cast < texture > ptexture = pgpurenderpassSource->m_texturea[prendererSource->m_pgpurendertarget->get_frame_index()];

      auto pcommandbuffer = m_pgpucontext->beginSingleTimeCommands();

      ::vulkan::insertImageMemoryBarrier(
         pcommandbuffer->m_vkcommandbuffer,
         ptexture->m_vkimage,
         0,
         VK_ACCESS_TRANSFER_WRITE_BIT,
         VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
         VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
         VK_PIPELINE_STAGE_TRANSFER_BIT,
         VK_PIPELINE_STAGE_TRANSFER_BIT,
         VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

      VkSubmitInfo submitInfo{};
      submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = &pcommandbuffer->m_vkcommandbuffer;
      ::array<VkSemaphore> waitSemaphores;
      ::array<VkPipelineStageFlags> waitStages;
      waitStages.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
      ::cast < render_pass > pgpurenderpass = prendererSource->m_pgpurendertarget;
      waitSemaphores.add(pgpurenderpass->renderFinishedSemaphores[prendererSource->m_pgpurendertarget->get_frame_index()]);
      submitInfo.waitSemaphoreCount = (uint32_t)waitSemaphores.size();
      submitInfo.pWaitSemaphores = waitSemaphores.data();
      submitInfo.pWaitDstStageMask = waitStages.data();

      //vkQueueWaitIdle(pgpucontext->graphicsQueue());

      m_pgpucontext->endSingleTimeCommands(pcommandbuffer, 1, &submitInfo);

      auto rectangle = prendererSource->m_pgpucontext->rectangle();

      _blend_image(ptexture, rectangle, true);

   }


   void renderer::soft_restore_context()
   {

      auto rectangle = m_pgpucontext->rectangle();

      VkViewport vp = {
         (float)rectangle.left(),
         (float)rectangle.top(),
         (float)rectangle.width(),
         (float)rectangle.height(),
         0.0f, 1.0f };

      VkRect2D sc = {
         {
            rectangle.left(),
            rectangle.top(),
         },
         {
            (uint32_t)rectangle.width(),
            (uint32_t)rectangle.height(),
         }
      };

      ::cast < command_buffer > pcommandbuffer = this->getCurrentCommandBuffer2();

      vkCmdSetViewport(pcommandbuffer->m_vkcommandbuffer, 0, 1, &vp);
      vkCmdSetScissor(pcommandbuffer->m_vkcommandbuffer, 0, 1, &sc);

   }


   //void renderer::blend2::create_render_pass()
   //{

   //   ::cast < context > pcontext = m_pgpurenderer->m_pgpucontext;

   //   VkAttachmentDescription colorAttachment = {
   //       .format = VK_FORMAT_R8G8B8A8_UNORM,
   //       .samples = VK_SAMPLE_COUNT_1_BIT,
   //       .loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,  // Blend onto existing dstImage
   //       .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
   //       .initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //       .finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
   //   };

   //   VkAttachmentReference colorRef = {
   //       .attachment = 0,
   //       .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
   //   };

   //   VkSubpassDescription subpass = {
   //       .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
   //       .colorAttachmentCount = 1,
   //       .pColorAttachments = &colorRef
   //   };

   //   VkRenderPassCreateInfo renderPassInfo = {
   //       .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
   //       .attachmentCount = 1,
   //       .pAttachments = &colorAttachment,
   //       .subpassCount = 1,
   //       .pSubpasses = &subpass
   //   };

   //   auto device = pcontext->logicalDevice();

   //   VK_CHECK_RESULT(
   //      vkCreateRenderPass(
   //         device,
   //         &renderPassInfo,
   //         NULL,
   //         &m_vkrenderpass));

   //}




   void renderer::blend(::gpu::texture* ptextureTarget, ::gpu::texture* ptextureSource)
   {

      ::cast < command_buffer > pcommandbuffer = this->getCurrentCommandBuffer2();

      ::cast < texture > ptextureSrc = ptextureSource;
      ::cast < texture > ptextureDst = ptextureTarget;

      ptextureSrc->_new_state(
         pcommandbuffer,
         0,
         VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

      ptextureDst->_new_state(
         pcommandbuffer,
         VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
         VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

      //  if (!m_psha)
      //  {

      //     __construct_new(m_pblend2);

      //     m_pblend2->m_pgpurenderer = this;

      //     m_pblend2->create_render_pass();

      //  }

      //  auto& imagestruct = m_pblend2->m_mapImage[ptextureDst->m_vkimage];

      //  if (!imagestruct.m_vkframebuffer)
      //  {

      //     ptextureDst->create_image_view();

      //     imagestruct.m_vkframebuffer = ptextureDst->create_framebuffer(m_pblend2->m_vkrenderpass);

      //  }


      //  // Image Blend descriptors
      //  if (!m_psetdescriptorlayoutBlend2)
      //  {

      //     m_psetdescriptorlayoutBlend2 = ::gpu_vulkan::set_descriptor_layout::Builder(m_pgpucontext)
      //        .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      //        .build();

      //     int iFrameCount = get_frame_count();

      //     auto pdescriptorpoolbuilder = __allocate::gpu_vulkan::descriptor_pool::Builder();

      //     pdescriptorpoolbuilder->initialize_builder(m_pgpucontext);
      //     pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
      //     pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

      //     m_pdescriptorpoolBlend2 = pdescriptorpoolbuilder->build();

      //     ::cast < context > pcontext = m_pgpucontext;

      //     VkDescriptorSetAllocateInfo allocInfo = {
      //.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
      //.descriptorPool = m_pdescriptorpoolBlend2->descriptorPool,
      //.descriptorSetCount = 1,
      //.pSetLayouts = &m_psetdescriptorlayoutBlend2->descriptorSetLayout
      //     };
      //     VK_CHECK_RESULT(vkAllocateDescriptorSets(
      //        pcontext->logicalDevice(),
      //        &allocInfo, &m_pblend2->descSet));

      //  }

      if (!m_pshaderBlend2)
      {

         __construct_new(m_pshaderBlend2);

         m_pshaderBlend2->m_pgpurenderer = this;
         m_pshaderBlend2->m_iSamplerSlot = 0;
         // Image Blend descriptors
//if (!m_psetdescriptorlayoutImageBlend)
         {

            m_pshaderBlend2->shader_sampler()->m_psetdescriptorlayout
               = ::gpu_vulkan::set_descriptor_layout::Builder(m_pgpucontext)
               .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
               .build();

            int iFrameCount = m_pgpurendertarget->get_frame_count();

            auto pdescriptorpoolbuilder = __allocate::gpu_vulkan::descriptor_pool::Builder();

            pdescriptorpoolbuilder->initialize_builder(m_pgpucontext);
            pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
            pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

            m_pshaderBlend2->shader_sampler()->m_pdescriptorpool
               = pdescriptorpoolbuilder->build();

         }

         auto pinputlayoutEmpty = __øcreate<::gpu::input_layout >();

         //pshadervertexinput->m_bindings.add(
         //   {
         //      .binding = 0,
         //      .stride = sizeof(float) * 4,
         //      .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
         //   });

         //pshadervertexinput->m_attribs.add({ .location = 0, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = 0 });
         //pshadervertexinput->m_attribs.add({ .location = 1, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = sizeof(float) * 2 });

         //auto pshaderBlend2 = __create_new<::gpu_vulkan::shader>();

         //m_pshaderBlend2 = pshaderBlend2;

         m_pshaderBlend2->m_bEnableBlend = true;
         m_pshaderBlend2->m_bDisableDepthTest = true;
         m_pshaderBlend2->m_iSamplerSlot = 0;

         ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;


         m_pshaderBlend2->initialize_shader_with_block(
            this,
            as_memory_block(g_blend2_vertex),
            as_memory_block(g_blend2_fragment),
            { ::gpu::shader::e_descriptor_set_slot_s1 },
            {},
            {},
            pinputlayoutEmpty,
            ::gpu::shader::e_flag_clear_default_bindings_and_attributes_descriptions);


      }


      if (m_pshaderBlend2->has_shader_sampler())
      {

         auto pshadertextureDst = m_pshaderBlend2->shader_texture(ptextureDst, false);
         //auto& imagestruct = *pimagestruct;
         //VkClearValue clearColor = { .color = {{0.0f, 0.0f, 0.0f, 0.0f}} };

         //vkCmdBeginRenderPass(pcommandbuffer->m_vkcommandbuffer,

         auto rectangleTarget = ptextureSrc->m_rectangleTarget;

         VkRenderPassBeginInfo renderPassInfo = {
               .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
               .renderPass = m_pshaderBlend2->shader_sampler()->get_render_pass(),
               .framebuffer = pshadertextureDst->get_framebuffer(),
               .renderArea = {{rectangleTarget.left(), rectangleTarget.top()},
            {(uint32_t)rectangleTarget.width(),(uint32_t)rectangleTarget.height()}},
            //.clearValueCount = 1,
            //.pClearValues = &clearColor
         .clearValueCount = 0,
         .pClearValues = nullptr,
         };

         //auto& imagestructa = s1()->m_imagestructamap[image];

///         vkCmdBeginRenderPass(
   //         pcommandbuffer->m_vkcommandbuffer, 
     //       &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);


      }


      VkClearValue clearColor = { .color = {{0.0f, 0.0f, 0.0f, 0.0f}} };

      //  VkRenderPassBeginInfo renderPassInfo = {
      //.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
      //.renderPass = m_pblend2->m_vkrenderpass,
      //.framebuffer = imagestruct.m_vkframebuffer,
      //.renderArea = {{0, 0}, {(uint32_t)rectangleTarget.width(), (uint32_t)rectangleTarget.height()}},
      //.clearValueCount = 1,
      //.pClearValues = &clearColor
      //  };

      //  vkCmdBeginRenderPass(pcommandbuffer->m_vkcommandbuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        //vkCmdBindPipeline(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
      m_pshaderBlend2->bind(ptextureTarget, ptextureSrc);

      //m_pshaderBlend2->_bind_sampler(ptextureDst->m_vkimage, 0);

      //vkCmdBindDescriptorSets(pcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pshaderBlend2->m_pipelinelayout,
         //0, 1, &descriptorSet, 0, NULL);
      // draw full screen triangle or quad
      vkCmdDraw(pcommandbuffer->m_vkcommandbuffer, 3, 1, 0, 0);


      m_pshaderBlend2->unbind();


      //if (m_pshaderBlend2->has_shader_sampler())
      //{

      //   vkCmdEndRenderPass(pcommandbuffer->m_vkcommandbuffer);

      //}
      //vkCmdEndRenderPass(pcommandbuffer->m_vkcommandbuffer);

   }



} // namespace gpu_vulkan




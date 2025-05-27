#include "framework.h"
#include "approach.h"
#include "descriptors.h"
#include "frame.h"
#include "renderer.h"
#include "offscreen_render_pass.h"
#include "physical_device.h"
#include "swap_chain_render_pass.h"
#include "initializers.h"
#include "app-cube/gpu_vulkan/shader.h"
#include "acme/parallelization/synchronous_lock.h"
//#include "tools.h"
//#include "app-cube/cube/impact.h"

#include <array>
#include <cassert>
#include <stdexcept>


using namespace vulkan;


namespace gpu_vulkan
{


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
   // renderer::renderer(VkWindow& window, context* pvkcdevice) : vkcWindow{ window }, m_pgpucontext{ pvkcdevice } 
   renderer::renderer()
   {

   }


   //int renderer::width()
   //{

   //   return m_pvkcrenderpass->width();

   //}

   //int renderer::height()
   //{

   //   return m_pvkcrenderpass->height();

   //}


   void renderer::initialize_renderer(::gpu::context* pgpucontext)
   {

      ::gpu::renderer::initialize_renderer(pgpucontext);

      m_pgpucontext = pgpucontext;

      m_pgpucontext->m_prenderer = this;

      //m_pimpact = pgpucontext->m_pimpact;

      __construct_new(m_poffscreensampler);

      m_poffscreensampler->initialize_offscreen_sampler(pgpucontext);

      m_poffscreensampler->m_prenderer = this;

      //defer_layout();

      createCommandBuffers();

   }


   renderer::~renderer()
   {

      freeCommandBuffers();

   }


   int renderer::get_frame_index()
   {

      assert(isFrameStarted && "Cannot get frame index when frame not in progress");

      return currentFrameIndex;

   }


   int renderer::get_frame_count()
   {

      return ::gpu_vulkan::render_pass::MAX_FRAMES_IN_FLIGHT;

   }


   void renderer::set_placement(const ::int_rectangle& rectanglePlacement)
   {

      ::gpu::renderer::set_placement(rectanglePlacement);

      //auto size = m_pimpact->size();

      if (m_extentRenderer.width == m_rectangle.width()
         && m_extentRenderer.height == m_rectangle.height())
      {

         return;

      }

      m_bNeedToRecreateSwapChain = true;

      m_extentRenderer.width = m_rectangle.width();
      m_extentRenderer.height = m_rectangle.height();

      defer_update_render_pass();

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


   void renderer::defer_update_render_pass()
   {

      if (m_pgpucontext->m_eoutput == ::gpu::e_output_cpu_buffer)
      {

         m_pvkcrenderpass = __allocate offscreen_render_pass(m_pgpucontext, m_extentRenderer, m_pvkcrenderpass);

      }
      else if (m_pgpucontext->m_eoutput == ::gpu::e_output_swap_chain)
      {

         m_pvkcrenderpass = __allocate swap_chain_render_pass(m_pgpucontext, m_extentRenderer, m_pvkcrenderpass);

      }
      else
      {

         throw ::exception(error_wrong_state, "Unexpected gpu e_output");

      }

      m_pvkcrenderpass->init();

      m_bNeedToRecreateSwapChain = false;

   }


   void renderer::createCommandBuffers()
   {

      commandBuffers.resize(render_pass::MAX_FRAMES_IN_FLIGHT);

      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandPool = m_pgpucontext->m_pgpudevice->getCommandPool();
      allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

      if (vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &allocInfo, commandBuffers.data()) !=
         VK_SUCCESS) {
         throw ::exception(error_failed, "failed to allocate command buffers!");
      }

   }


   void renderer::freeCommandBuffers()
   {

      vkFreeCommandBuffers(
         m_pgpucontext->logicalDevice(),
         m_pgpucontext->m_pgpudevice->getCommandPool(),
         static_cast<float>(commandBuffers.size()),
         commandBuffers.data());
      commandBuffers.clear();

   }


   void renderer::on_begin_draw()
   {

      if (m_rectangle.is_empty())
      {

         throw ::exception(error_wrong_state, "please call set size before at least once with no empty preferrably good initial size");

         //set_size(size);

      }

      assert(!isFrameStarted && "Can't call beginFrame while already in progress");

      //if (m_bOffScreen)
      {

         auto result = m_pvkcrenderpass->acquireNextImage(&currentImageIndex);

         if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            //set_placement(size);
            throw ::exception(todo, "resize?!?!");
            //return nullptr;
            return;
         }
         if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw ::exception(error_failed, "Failed to aquire swap chain image");
         }

         isFrameStarted = true;

         auto commandBuffer = getCurrentCommandBuffer();

         VkCommandBufferBeginInfo beginInfo{};
         beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

         if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw ::exception(error_failed, "failed to begin recording command buffer!");
         }

         //m_
         //auto pframe = __create_new < frame >();
         //pframe->commandBuffer = commandBuffer;
         //m_pframe = pframe;
         //return m_pframe;

      }
      //else
      //{


      //	auto result = m_pvkcswapchain->acquireNextImage(&currentImageIndex);

      //	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      //		recreateRenderPass();
      //		return nullptr;
      //	}
      //	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      //		throw ::exception(error_failed, "Failed to aquire swap chain image");
      //	}

      //	isFrameStarted = true;

      //	auto commandBuffer = getCurrentCommandBuffer();

      //	VkCommandBufferBeginInfo beginInfo{};
      //	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      //	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
      //		throw ::exception(error_failed, "failed to begin recording command buffer!");
      //	}
      //	return commandBuffer;

      //}

      _on_begin_render();

   }


   renderer::OffScreenSampler::OffScreenSampler()
   {

      clear();

   }


   renderer::OffScreenSampler::~OffScreenSampler()
   {

      destroy();

   }


   void renderer::OffScreenSampler::initialize_offscreen_sampler(::gpu::context* pgpucontext)
   {

      m_pgpucontext = pgpucontext;

   }


   void renderer::OffScreenSampler::clear()
   {

      m_vkextent2d.width = 0;
      m_vkextent2d.height = 0;
      m_vkdevicememory = nullptr;
      m_vkimage = nullptr;

   }


   void renderer::OffScreenSampler::update(VkExtent2D vkextent2d)
   {

      if (vkextent2d.width == m_vkextent2d.width
         && vkextent2d.height == m_vkextent2d.height)
      {

         return;

      }

      destroy();

      if (vkextent2d.width <= 0 ||
         vkextent2d.height <= 0)
      {

         return;

      }

      m_vkextent2d.width = vkextent2d.width;
      m_vkextent2d.height = vkextent2d.height;

      VkImageCreateInfo imgCreateInfo(initializers::imageCreateInfo());
      imgCreateInfo.imageType = VK_IMAGE_TYPE_2D;
      imgCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
      imgCreateInfo.extent.width = m_vkextent2d.width;
      imgCreateInfo.extent.height = m_vkextent2d.height;
      imgCreateInfo.extent.depth = 1;
      imgCreateInfo.arrayLayers = 1;
      imgCreateInfo.mipLevels = 1;
      imgCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      imgCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
      imgCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
      imgCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;
      // Create the image
      //VkImage dstImage;
      VK_CHECK_RESULT(vkCreateImage(m_pgpucontext->logicalDevice(), &imgCreateInfo, nullptr, &m_vkimage));
      // Create memory to back up the image
      VkMemoryRequirements memRequirements;
      VkMemoryAllocateInfo memAllocInfo(initializers::memoryAllocateInfo());
      vkGetImageMemoryRequirements(m_pgpucontext->logicalDevice(), m_vkimage, &memRequirements);
      memAllocInfo.allocationSize = memRequirements.size;
      // Memory must be host visible to copy from
      memAllocInfo.memoryTypeIndex = m_pgpucontext->m_pgpudevice->m_pphysicaldevice->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
      VK_CHECK_RESULT(vkAllocateMemory(m_pgpucontext->logicalDevice(), &memAllocInfo, nullptr, &m_vkdevicememory));
      VK_CHECK_RESULT(vkBindImageMemory(m_pgpucontext->logicalDevice(), m_vkimage, m_vkdevicememory, 0));


   }


   void renderer::OffScreenSampler::destroy()
   {

      if (m_vkdevicememory)
      {

         vkFreeMemory(m_pgpucontext->logicalDevice(), m_vkdevicememory, nullptr);
         vkDestroyImage(m_pgpucontext->logicalDevice(), m_vkimage, nullptr);

         clear();

      }

   }


   void renderer::OffScreenSampler::sample(VkImage vkimage)
   {

      if (!m_vkimage || !m_vkdevicememory)
      {

         return;

      }


      // Create the linear tiled destination image to copy to and to read the memory from

// Do the actual blit from the offscreen image to our host visible destination image
      VkCommandBufferAllocateInfo cmdBufAllocateInfo = initializers::commandBufferAllocateInfo(m_pgpucontext->m_pgpudevice->getCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);
      VkCommandBuffer copyCmd;
      VK_CHECK_RESULT(vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &cmdBufAllocateInfo, &copyCmd));
      VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
      VK_CHECK_RESULT(vkBeginCommandBuffer(copyCmd, &cmdBufInfo));

      // Transition destination image to transfer destination layout
      insertImageMemoryBarrier(
         copyCmd,
         m_vkimage,
         0,
         VK_ACCESS_TRANSFER_WRITE_BIT,
         VK_IMAGE_LAYOUT_UNDEFINED,
         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
         VK_PIPELINE_STAGE_TRANSFER_BIT,
         VK_PIPELINE_STAGE_TRANSFER_BIT,
         VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

      // colorAttachment.image is already in VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, and does not need to be transitioned

      VkImageCopy imageCopyRegion{};
      imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      imageCopyRegion.srcSubresource.layerCount = 1;
      imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      imageCopyRegion.dstSubresource.layerCount = 1;
      imageCopyRegion.extent.width = m_vkextent2d.width;
      imageCopyRegion.extent.height = m_vkextent2d.height;
      imageCopyRegion.extent.depth = 1;

      vkCmdCopyImage(
         copyCmd,
         //colorAttachment.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
         vkimage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
         m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
         1,
         &imageCopyRegion);

      // Transition destination image to general layout, which is the required layout for mapping the image memory later on
      insertImageMemoryBarrier(
         copyCmd,
         m_vkimage,
         VK_ACCESS_TRANSFER_WRITE_BIT,
         VK_ACCESS_MEMORY_READ_BIT,
         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
         VK_IMAGE_LAYOUT_GENERAL,
         VK_PIPELINE_STAGE_TRANSFER_BIT,
         VK_PIPELINE_STAGE_TRANSFER_BIT,
         VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

      VK_CHECK_RESULT(vkEndCommandBuffer(copyCmd));

      ::cast < offscreen_render_pass > ppass = m_prenderer->m_pvkcrenderpass;

      ppass->submitSamplingWork(copyCmd, &m_prenderer->currentImageIndex);

      vkQueueWaitIdle(m_pgpucontext->graphicsQueue());

      vkFreeCommandBuffers(m_pgpucontext->logicalDevice(), m_pgpucontext->m_pgpudevice->getCommandPool(), 1, &copyCmd);


   }


   void renderer::OffScreenSampler::send_sample()
   {

      if (!m_vkimage || !m_vkdevicememory)
      {

         return;

      }

      //auto pimpact = m_pgpucontext->m_pimpact;

      //auto callback = pimpact->m_callbackImage32CpuBuffer;

      //auto callback = m_prenderer->m_callbackImage32CpuBuffer;

      // Get layout of the image (including row pitch)
      VkImageSubresource subResource{};
      subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      VkSubresourceLayout subResourceLayout;

      vkGetImageSubresourceLayout(m_pgpucontext->logicalDevice(), m_vkimage, &subResource, &subResourceLayout);

      const char* imagedata = nullptr;
      // Map image memory so we can start copying from it
      vkMapMemory(m_pgpucontext->logicalDevice(), m_vkdevicememory, 0, VK_WHOLE_SIZE, 0, (void**)&imagedata);
      imagedata += subResourceLayout.offset;

      /*
         Save host visible framebuffer image to disk (ppm format)
      */

      //::memory mem;

      //mem.set_size(m_width * m_height * 4);

      //::array<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
      //const bool colorSwizzle = (std::find(formatsBGR.begin(), formatsBGR.end(), VK_FORMAT_R8G8B8A8_UNORM) != formatsBGR.end());
      {
         m_pgpucontext->m_callbackImage32CpuBuffer(
            (const ::image32_t*)imagedata,
            m_vkextent2d.width,
            m_vkextent2d.height,
            subResourceLayout.rowPitch);
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

      }
      //else
      //{


      //}


      vkUnmapMemory(m_pgpucontext->logicalDevice(), m_vkdevicememory);

   }


   void renderer::sample()
   {

      //auto callback = m_callbackImage32CpuBuffer;

//      if (callback)
      {

         m_poffscreensampler->update(m_pvkcrenderpass->getExtent());
         /*
            Copy framebuffer image to host visible image
         */
         /*const char* imagedata;*/
         {

            m_poffscreensampler->sample(m_pvkcrenderpass->m_images[currentImageIndex]);

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
            //imageCopyRegion.extent.width = m_pvkcrenderpass->width();
            //imageCopyRegion.extent.height = m_pvkcrenderpass->height();
            //imageCopyRegion.extent.depth = 1;

            //vkCmdCopyImage(
            //	copyCmd,
            //	//colorAttachment.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            //	m_pvkcrenderpass->m_images[iIndex], VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
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

            m_poffscreensampler->send_sample();

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
            //		m_pvkcrenderpass->width(),
            //		m_pvkcrenderpass->height(),
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
      vkQueueWaitIdle(m_pgpucontext->graphicsQueue());


   }

   void renderer::swap_chain()
   {



      assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
      auto commandBuffer = getCurrentCommandBuffer();
      if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
         throw ::exception(error_failed, "failed to record command buffer!");
      }
      auto result = m_pvkcrenderpass->submitCommandBuffers(&commandBuffer, &currentImageIndex);
      if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
         m_bNeedToRecreateSwapChain)
      {
         m_bNeedToRecreateSwapChain = false;
         defer_update_render_pass();
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
      //   auto commandBuffer = getCurrentCommandBuffer();
      //   if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
      //      throw ::exception(error_failed, "failed to record command buffer!");
      //   }
      //   auto result = m_pvkcrenderpass->submitCommandBuffers(&commandBuffer, &currentImageIndex);
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
      //	auto commandBuffer = getCurrentCommandBuffer();
      //	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
      //		throw ::exception(error_failed, "failed to record command buffer!");
      //	}
      //	auto result = m_pvkcswapchain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
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

   ::gpu::shader* renderer::get_image_blend_shader()
   {

      if (!m_pshaderImageBlend)
      {

         auto pshadervertexinput = __allocate  shader_vertex_input();

         pshadervertexinput->m_bindings.add(
            {
               .binding = 0,
               .stride = sizeof(float) * 4,
               .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
            });

         pshadervertexinput->m_attribs.add({ .location = 0, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = 0 });
         pshadervertexinput->m_attribs.add({ .location = 1, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = sizeof(float) * 2 });

         auto pshaderImageBlend = __create_new<::gpu_vulkan::shader>();

         m_pshaderImageBlend = pshaderImageBlend;

         //VkDescriptorSetLayoutBinding samplerLayoutBinding = {
         //   .binding = 0,
         //   .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
         //   .descriptorCount = 1,
         //   .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
         //   .pImmutableSamplers = NULL,
         //};

         //VkDescriptorSetLayoutCreateInfo layoutInfo =
         //{
         //   .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
         //   .bindingCount = 1,
         //   .pBindings = &samplerLayoutBinding,
         //};

         //VkDescriptorSetLayout descriptorSetLayout;
         //if (vkCreateDescriptorSetLayout(device, &layoutInfo, NULL, &descriptorSetLayout) != VK_SUCCESS) 
         //{
         //   // Handle error
         //}

         ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

         pshaderImageBlend->initialize_shader_with_block(
            m_pgpucontext,
            as_memory_block(g_uaImageBlendVertexShader),
            as_memory_block(g_uaImageBlendFragmentShader),
            { ::gpu::shader::e_descriptor_set_slot_local },
            m_psetdescriptorlayoutImageBlend,
            pshadervertexinput);

      }

      return m_pshaderImageBlend;

   }
   // Fullscreen quad vertex data
   float quadVertices[] = {
      // pos     // uv
      -1.0f, -1.0f, 0.0f, 0.0f,
       1.0f, -1.0f, 1.0f, 0.0f,
       1.0f,  1.0f, 1.0f, 1.0f,
      -1.0f,  1.0f, 0.0f, 1.0f,
   };
   uint16_t quadIndices[] = {
       0, 1, 2,
       2, 3, 0
   };

   // Create vertex and index buffers
   void create_quad_buffers(VkDevice device, VkPhysicalDevice physicalDevice,
      VkBuffer* vertexBuffer, VkDeviceMemory* vertexMemory,
      VkBuffer* indexBuffer, VkDeviceMemory* indexMemory)
   {

      VkDeviceSize vertexSize = sizeof(quadVertices);
      VkDeviceSize indexSize = sizeof(quadIndices);

      VkBufferCreateInfo bufferInfo = {
          .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
          .size = vertexSize,
          .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
          .sharingMode = VK_SHARING_MODE_EXCLUSIVE
      };
      vkCreateBuffer(device, &bufferInfo, NULL, vertexBuffer);

      VkMemoryRequirements memReq;
      vkGetBufferMemoryRequirements(device, *vertexBuffer, &memReq);

      VkMemoryAllocateInfo allocInfo = {
          .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
          .allocationSize = memReq.size,
          .memoryTypeIndex = 0
      };

      VkPhysicalDeviceMemoryProperties memProps;
      vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProps);
      for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
         if ((memReq.memoryTypeBits & (1 << i)) &&
            (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) &&
            (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
            allocInfo.memoryTypeIndex = i;
            break;
         }
      }

      vkAllocateMemory(device, &allocInfo, NULL, vertexMemory);
      vkBindBufferMemory(device, *vertexBuffer, *vertexMemory, 0);

      void* data;
      vkMapMemory(device, *vertexMemory, 0, vertexSize, 0, &data);
      memcpy(data, quadVertices, (size_t)vertexSize);
      vkUnmapMemory(device, *vertexMemory);

      // Index buffer
      bufferInfo.size = indexSize;
      bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
      vkCreateBuffer(device, &bufferInfo, NULL, indexBuffer);
      vkGetBufferMemoryRequirements(device, *indexBuffer, &memReq);
      allocInfo.allocationSize = memReq.size;

      for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
         if ((memReq.memoryTypeBits & (1 << i)) &&
            (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) &&
            (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
            allocInfo.memoryTypeIndex = i;
            break;
         }
      }

      vkAllocateMemory(device, &allocInfo, NULL, indexMemory);
      vkBindBufferMemory(device, *indexBuffer, *indexMemory, 0);
      vkMapMemory(device, *indexMemory, 0, indexSize, 0, &data);
      memcpy(data, quadIndices, (size_t)indexSize);
      vkUnmapMemory(device, *indexMemory);
   }

   void renderer::_blend_image(VkImage image, const ::int_rectangle& rectangle)
   {

      // Image Blend descriptors
      if (!m_psetdescriptorlayoutImageBlend)
      {

         m_psetdescriptorlayoutImageBlend = ::gpu_vulkan::set_descriptor_layout::Builder(m_pgpucontext)
            .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
            .build();

         auto pdescriptorpoolbuilder = __allocate::gpu_vulkan::descriptor_pool::Builder();

         pdescriptorpoolbuilder->initialize_builder(m_pgpucontext);
         pdescriptorpoolbuilder->setMaxSets(get_frame_count());
         pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, get_frame_count());

         m_pdescriptorpoolImageBlend = pdescriptorpoolbuilder->build();

      }

      //vkCmdBeginRenderPass(...);

      auto commandBuffer = this->getCurrentCommandBuffer();

      //VkCommandBufferAllocateInfo commandBufferAllocateInfo = initializers::commandBufferAllocateInfo(m_pgpucontext->getCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);

      //VkCommandBuffer commandBuffer;
      //VK_CHECK_RESULT(vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &commandBufferAllocateInfo, &commandBuffer));
      //VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
      //VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &cmdBufInfo));


      //m_procedureaAfterEndRender.add(
      //   [this, image, commandBuffer]()
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
      //      //               commandBuffer,
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
      //      //                  commandBuffer,
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

            {


               VkImageMemoryBarrier barrier =
               {
                  .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                  .pNext = nullptr,
                  .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                  .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
                  .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                  .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                  .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                  .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                  .image = image,
                  .subresourceRange =
                     {
                        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                        .baseMipLevel = 0,
                        .levelCount = 1,
                        .baseArrayLayer = 0,
                        .layerCount = 1,
                     },
               };

               vkCmdPipelineBarrier(
                  commandBuffer,
                  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                  VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                  0,
                  0, NULL,
                  0, NULL,
                  1, &barrier
               );

            }

      /*   }
         );*/

      auto& pmodel = m_imagemodel[image];

      if (__defer_construct_new(pmodel))
      {

         create_quad_buffers(m_pgpucontext->logicalDevice(),
            m_pgpucontext->m_pgpudevice->m_pphysicaldevice->m_physicaldevice,
            &pmodel->m_vertexBuffer,
            &pmodel->m_vertexMemory,
            &pmodel->m_indexBuffer,
            &pmodel->m_indexMemory);

      }

      auto pshader = get_image_blend_shader();

      pshader->bind();

      auto& pdescriptor = m_imagedescriptor[image];

      if (__defer_construct_new(pdescriptor))
      {

         pdescriptor->m_descriptorsets.set_size(get_frame_count());

         VkImageViewCreateInfo viewInfo = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .image = image,  // <-- Your existing VkImage
    .viewType = VK_IMAGE_VIEW_TYPE_2D,
    .format = VK_FORMAT_B8G8R8A8_UNORM,  // <-- Match your image's format
    .components = {
        .r = VK_COMPONENT_SWIZZLE_IDENTITY,
        .g = VK_COMPONENT_SWIZZLE_IDENTITY,
        .b = VK_COMPONENT_SWIZZLE_IDENTITY,
        .a = VK_COMPONENT_SWIZZLE_IDENTITY,
    },
    .subresourceRange = {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1,
    },
         };

         VkImageView imageView;
         if (vkCreateImageView(m_pgpucontext->logicalDevice(), &viewInfo, NULL, &imageView) != VK_SUCCESS) {
            // Handle error
         }

         ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

         for (int i = 0; i < get_frame_count(); i++)
         {
            VkDescriptorImageInfo imageinfo;

            imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageinfo.imageView = imageView;
            imageinfo.sampler = m_pgpucontext->_001VkSampler();

            auto& playout = m_psetdescriptorlayoutImageBlend;

            auto& ppool = m_pdescriptorpoolImageBlend;

            descriptor_writer(*playout, *ppool)
               .writeImage(0, &imageinfo)
               .build(pdescriptor->m_descriptorsets[i]);

         }

         auto descriptorSetLayout = m_psetdescriptorlayoutImageBlend->getDescriptorSetLayout();

         VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .setLayoutCount = 1,
    .pSetLayouts = &descriptorSetLayout,
         };

         //VkPipelineLayout pipelineLayout;
         if (vkCreatePipelineLayout(m_pgpucontext->logicalDevice(), &pipelineLayoutInfo, NULL, &pdescriptor->m_vkpipelinelayout) != VK_SUCCESS) {
            // Handle error
         }

      }

      //auto commandBuffer = this->getCurrentCommandBuffer();

      // Bind pipeline and descriptor sets
//      vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
  //    vkCmdBindDescriptorSets(commandBuffer, ...);
      vkCmdBindDescriptorSets(
         commandBuffer,
         VK_PIPELINE_BIND_POINT_GRAPHICS,   // Bind point
         pdescriptor->m_vkpipelinelayout,                     // Layout used when pipeline was created
         0,                                  // First set (set = 0)
         1,                                  // Descriptor set count
         &pdescriptor->m_descriptorsets[get_frame_index()],                     // Pointer to descriptor set
         0,                                  // Dynamic offset count
         NULL                                // Dynamic offsets
      );




      VkDeviceSize offsets[] = { 0 };
      vkCmdBindVertexBuffers(commandBuffer, 0, 1, &pmodel->m_vertexBuffer, offsets);
      vkCmdBindIndexBuffer(commandBuffer, pmodel->m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);

      VkViewport vp = {
         (float)rectangle.left(),
         (float)rectangle.top(),
         (float)rectangle.width(),
         (float)rectangle.height(),
         0.0f, 1.0f };
      VkRect2D sc = {
         {
         (float)rectangle.left(),
         (float)rectangle.top(),
         },
         {
                     (float)rectangle.width(),
         (float)rectangle.height(),


      }
      };
      vkCmdSetViewport(commandBuffer, 0, 1, &vp);
      vkCmdSetScissor(commandBuffer, 0, 1, &sc);

      vkCmdDrawIndexed(commandBuffer, 6, 1, 0, 0, 0);
      // Draw full-screen quad
      //vkCmdDraw(commandBuffer, 6, 1, 0, 0); // assuming full-screen triangle/quad

      pshader->unbind();

      //vkCmdEndRenderPass(...);

      vkQueueWaitIdle(m_pgpucontext->graphicsQueue());

      //vkFreeCommandBuffers(m_pgpucontext->logicalDevice(), m_pgpucontext->m_vkcommandpool, 1, &commandBuffer);


         }


         void renderer::_on_begin_render()
      {

         //::cast < frame > pframe = pframeParam;

         //auto commandBuffer = pframe->commandBuffer;

         auto commandBuffer = this->getCurrentCommandBuffer();

         //if (m_bOffScreen)
         {

            assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
            assert(
               commandBuffer == getCurrentCommandBuffer() &&
               "Can't begin render pass on command buffer from a different frame");

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = m_pvkcrenderpass->getRenderPass();
            renderPassInfo.framebuffer = m_pvkcrenderpass->getFrameBuffer(currentImageIndex);

            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = m_pvkcrenderpass->getExtent();

            std::array<VkClearValue, 2> clearValues{};
            //clearValues[0].color = { 2.01f, 0.01f, 0.01f, 1.0f };
            clearValues[0].color = { 0.5f, 0.75f, 1.0f, 1.0f };
            clearValues[1].depthStencil = { 1.0f, 0 };
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = static_cast<float>(m_pvkcrenderpass->getExtent().width);
            viewport.height = static_cast<float>(m_pvkcrenderpass->getExtent().height);
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            VkRect2D scissor{ {0, 0}, m_pvkcrenderpass->getExtent() };
            vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
            vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

         }
         //else
         //{

         //	assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
         //	assert(
         //		commandBuffer == getCurrentCommandBuffer() &&
         //		"Can't begin render pass on command buffer from a different frame");

         //	VkRenderPassBeginInfo renderPassInfo{};
         //	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
         //	renderPassInfo.renderPass = m_pvkcswapchain->getRenderPass();
         //	renderPassInfo.framebuffer = m_pvkcswapchain->getFrameBuffer(currentImageIndex);

         //	renderPassInfo.renderArea.offset = { 0, 0 };
         //	renderPassInfo.renderArea.extent = m_pvkcswapchain->getExtent();

         //	std::array<VkClearValue, 2> clearValues{};
         //	clearValues[0].color = { 2.01f, 0.01f, 0.01f, 1.0f };
         //	clearValues[1].depthStencil = { 1.0f, 0 };
         //	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
         //	renderPassInfo.pClearValues = clearValues.data();

         //	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

         //	VkViewport viewport{};
         //	viewport.x = 0.0f;
         //	viewport.y = 0.0f;
         //	viewport.width = static_cast<float>(vkcSwapChain->getSwapChainExtent().width);
         //	viewport.height = static_cast<float>(vkcSwapChain->getSwapChainExtent().height);
         //	viewport.minDepth = 0.0f;
         //	viewport.maxDepth = 1.0f;
         //	VkRect2D scissor{ {0, 0}, vkcSwapChain->getSwapChainExtent() };
         //	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
         //	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);


         //}
      }


      void renderer::on_begin_render(::gpu::frame * pframeParam)
      {

         ::cast < frame > pframe = pframeParam;

         auto commandBuffer = pframe->commandBuffer;

         //if (m_bOffScreen)
         {

            assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
            assert(
               commandBuffer == getCurrentCommandBuffer() &&
               "Can't begin render pass on command buffer from a different frame");

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = m_pvkcrenderpass->getRenderPass();
            renderPassInfo.framebuffer = m_pvkcrenderpass->getFrameBuffer(currentImageIndex);

            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = m_pvkcrenderpass->getExtent();

            std::array<VkClearValue, 2> clearValues{};
            //clearValues[0].color = { 2.01f, 0.01f, 0.01f, 1.0f };
            clearValues[0].color = { 0.5f, 0.75f, 1.0f, 1.0f };
            clearValues[1].depthStencil = { 1.0f, 0 };
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = static_cast<float>(m_pvkcrenderpass->getExtent().width);
            viewport.height = static_cast<float>(m_pvkcrenderpass->getExtent().height);
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            VkRect2D scissor{ {0, 0}, m_pvkcrenderpass->getExtent() };
            vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
            vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

         }
         //else
         //{

         //	assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
         //	assert(
         //		commandBuffer == getCurrentCommandBuffer() &&
         //		"Can't begin render pass on command buffer from a different frame");

         //	VkRenderPassBeginInfo renderPassInfo{};
         //	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
         //	renderPassInfo.renderPass = m_pvkcswapchain->getRenderPass();
         //	renderPassInfo.framebuffer = m_pvkcswapchain->getFrameBuffer(currentImageIndex);

         //	renderPassInfo.renderArea.offset = { 0, 0 };
         //	renderPassInfo.renderArea.extent = m_pvkcswapchain->getExtent();

         //	std::array<VkClearValue, 2> clearValues{};
         //	clearValues[0].color = { 2.01f, 0.01f, 0.01f, 1.0f };
         //	clearValues[1].depthStencil = { 1.0f, 0 };
         //	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
         //	renderPassInfo.pClearValues = clearValues.data();

         //	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

         //	VkViewport viewport{};
         //	viewport.x = 0.0f;
         //	viewport.y = 0.0f;
         //	viewport.width = static_cast<float>(vkcSwapChain->getSwapChainExtent().width);
         //	viewport.height = static_cast<float>(vkcSwapChain->getSwapChainExtent().height);
         //	viewport.minDepth = 0.0f;
         //	viewport.maxDepth = 1.0f;
         //	VkRect2D scissor{ {0, 0}, vkcSwapChain->getSwapChainExtent() };
         //	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
         //	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);


         //}
      }


      void renderer::on_end_render(::gpu::frame * pframeParam)
      {

         ::cast < frame > pframe = pframeParam;

         auto commandBuffer = pframe->commandBuffer;

         assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
         assert(
            commandBuffer == getCurrentCommandBuffer() &&
            "Can't end render pass on command buffer from a different frame");
         vkCmdEndRenderPass(commandBuffer);
      }


      //void renderer::on_end_render(::graphics3d::frame * pframeParam)
      void renderer::_on_end_render()
      {

         //::cast < frame > pframe = pframeParam;

         //auto commandBuffer = pframe->commandBuffer;

         auto commandBuffer = getCurrentCommandBuffer();

         assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
         assert(
            commandBuffer == getCurrentCommandBuffer() &&
            "Can't end render pass on command buffer from a different frame");
         vkCmdEndRenderPass(commandBuffer);
      }


      ::pointer < ::gpu::frame > renderer::beginFrame()
      {

         //defer_layout();

         assert(!isFrameStarted && "Can't call beginFrame while already in progress");

         //if (m_bOffScreen)
         {

            auto result = m_pvkcrenderpass->acquireNextImage(&currentImageIndex);

            if (result == VK_ERROR_OUT_OF_DATE_KHR) {
               //defer_layout();
               throw ::exception(todo, "resize happened?!?!");
               return nullptr;
            }
            if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
               throw std::runtime_error("Failed to aquire swap chain image");
            }

            isFrameStarted = true;

            auto commandBuffer = getCurrentCommandBuffer();

            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
               throw std::runtime_error("failed to begin recording command buffer!");
            }
            auto pframe = __create_new < ::gpu_vulkan::frame >();
            pframe->commandBuffer = commandBuffer;
            m_pframe = pframe;
            return m_pframe;

         }
         //else
         //{


         //	auto result = m_pvkcswapchain->acquireNextImage(&currentImageIndex);

         //	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
         //		recreateRenderPass();
         //		return nullptr;
         //	}
         //	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
         //		throw std::runtime_error("Failed to aquire swap chain image");
         //	}

         //	isFrameStarted = true;

         //	auto commandBuffer = getCurrentCommandBuffer();

         //	VkCommandBufferBeginInfo beginInfo{};
         //	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

         //	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
         //		throw std::runtime_error("failed to begin recording command buffer!");
         //	}
         //	return commandBuffer;

         //}
      }


      void renderer::endFrame()
      {

         //if (m_pgpucontext->m_eoutput == ::gpu::e_output_cpu_buffer)
         //{

         assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
         auto commandBuffer = getCurrentCommandBuffer();
         if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
         {
            throw ::exception(error_failed, "failed to record command buffer!");
         }

         auto result = m_pvkcrenderpass->submitCommandBuffers(&commandBuffer, &currentImageIndex);

         if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
            m_bNeedToRecreateSwapChain)
         {
            m_bNeedToRecreateSwapChain = false;
            defer_update_render_pass();
         }
         else if (result != VK_SUCCESS)
         {
            throw ::exception(error_failed, "failed to present swap chain image!");
         }
         if (m_pgpucontext->m_eoutput == ::gpu::e_output_cpu_buffer)
         {
            sample();

         }
         isFrameStarted = false;
         //rrentImageIndex = m_pvkcrenderpass->currentFrame;
         currentFrameIndex = (currentFrameIndex + 1) % ::gpu_vulkan::render_pass::MAX_FRAMES_IN_FLIGHT;

         //}
         //else
         //{


         //	assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
         //	auto commandBuffer = getCurrentCommandBuffer();
         //	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
         //		throw std::runtime_error("failed to record command buffer!");
         //	}
         //	auto result = m_pvkcswapchain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
         //	//if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
         //	//	vkcWindow.wasWindowResized()) 
         //	//{
         //	//	vkcWindow.resetWindowResizedFlag();
         //	//	recreateSwapchain();
         //	//}
         //	//else 
         //	//	if (result != VK_SUCCESS) {
         //	//	throw std::runtime_error("failed to present swap chain image!");
         //	//}
         //	isFrameStarted = false;
         //	currentFrameIndex = (currentFrameIndex + 1) % swap_chain_render_pass::MAX_FRAMES_IN_FLIGHT;

         //}

      }


      void renderer::_on_graphics_end_draw(VkImage image, const ::int_rectangle& rectangle)
      {

         set_placement(rectangle);

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

            _blend_image(image, rectangle);

            on_end_render(pframe);

            

            endFrame();

         }

      }


   } // namespace gpu_vulkan




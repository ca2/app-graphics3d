// From vk_swapchain by camilo on 2025-05-09 02:01 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "command_buffer.h"
#include "physical_device.h"
#include "render_pass.h"
#include "renderer.h"
#include "texture.h"


namespace gpu_vulkan
{


   render_pass::render_pass()
   {


   }


   render_pass::~render_pass()
   {

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

      //for (auto imageView : m_imageviews) 
      //{
        // vkDestroyImageView(pcontext->logicalDevice(), imageView, nullptr);
      //}
      //m_imageviews.clear();

      //if (swapChain != nullptr) {
      //   vkDestroySwapchainKHR(m_pgpucontext->logicalDevice(), swapChain, nullptr);
      //   swapChain = nullptr;
      //}

      //for (int i = 0; i < depthImages.size(); i++) 
      //{
      //   vkDestroyImageView(pcontext->logicalDevice(), depthImageViews[i], nullptr);
      //   vkDestroyImage(pcontext->logicalDevice(), depthImages[i], nullptr);
      //   vkFreeMemory(pcontext->logicalDevice(), depthImageMemorys[i], nullptr);
      //}

      for (auto framebuffer : m_framebuffers)
      {

         vkDestroyFramebuffer(pcontext->logicalDevice(), framebuffer, nullptr);

      }

      vkDestroyRenderPass(pcontext->logicalDevice(), m_vkrenderpass, nullptr);

      // cleanup synchronization objects
      for (size_t i = 0; i < m_texturea.size(); i++)
      {
         
         vkDestroySemaphore(pcontext->logicalDevice(), renderFinishedSemaphores[i], nullptr);
         vkDestroySemaphore(pcontext->logicalDevice(), imageAvailableSemaphores[i], nullptr);

      }

   }


   void render_pass::initialize_render_target(::gpu::renderer* pgpurenderer, const ::int_size& size, ::pointer<::gpu::render_target> previous)
   {

      ::gpu::render_target::initialize_render_target(pgpurenderer, size, previous);

      m_bNeedRebuild = false;

   }


   void render_pass::on_init()
   {

      createRenderPassImpl();
      createImageViews();
      createRenderPass();
      createDepthResources();
      createFramebuffers();
      createSyncObjects();
      // Cleans up old swap chain since it's no longer needed after resizing
      //m_pvkcrenderpassOld = nullptr;

   }


   VkImageView render_pass::getImageView(int index)
   { 

      ::cast < texture > ptexture = m_texturea[index];

      if(!ptexture)
      {
         
         throw ::exception(error_wrong_state, "No texture at index");

      }

      auto imageview = ptexture->get_image_view();

      if(!imageview)
      {
         
         throw ::exception(error_wrong_state, "No image view at index");

      }
      
      return imageview; 
   
   }


   VkImageView render_pass::getDepthImageView(int index)
   {

      ::cast < texture > ptexture = m_texturea[index];

      if (!ptexture)
      {

         throw ::exception(error_wrong_state, "No texture at index");

      }

      auto imageview = ptexture->get_depth_image_view();

      if (!imageview)
      {

         throw ::exception(error_wrong_state, "No depth image view at index");

      }

      return imageview;

   }


   void render_pass::on_before_begin_render(frame* pframe)
   {

      ::cast < texture > ptextureCurrent = current_texture();

      ::cast < command_buffer > pcommandbuffer = m_pgpurenderer->getCurrentCommandBuffer2();

      ptextureCurrent->_new_state(
         pcommandbuffer,
         VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
         VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
      );

      ::cast < texture > ptextureDepth = ptextureCurrent->m_ptextureDepth;

      if(ptextureDepth)
      {

         ptextureDepth->_new_state(
            pcommandbuffer,
            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT
         );

      }

   }


   //int render_pass::get_image_index() const
   //{
   //   
   //   return get_frame_index(); 
   //
   //}


   
   VkResult render_pass::acquireNextImage() 
   {

      //vkWaitForFences(
      //   m_pgpucontext->logicalDevice(),
      //   1,
      //   &inFlightFences[currentFrame],
      //   VK_TRUE,
      //   std::numeric_limits<uint64_t>::max());

      //VkResult result = vkAcquireNextImageKHR(
      //   m_pgpucontext->logicalDevice(),
      //   swapChain,
      //   std::numeric_limits<uint64_t>::max(),
      //   imageAvailableSemaphores[currentFrame],  // must be a not signaled semaphore
      //   VK_NULL_HANDLE,
      //   imageIndex);

      return VK_ERROR_FEATURE_NOT_PRESENT;

   }


   VkResult render_pass::submitCommandBuffers(
      command_buffer * pcommandbuffer,
      const ::array < VkSemaphore >& semaphoreaWait,
      const ::array < VkPipelineStageFlags >& stageaWait,
      const ::array < VkSemaphore >& semaphoreaSignal)
   {

      //if (imagesInFlight[*imageIndex] != VK_NULL_HANDLE) {
      //   vkWaitForFences(m_pgpucontext->logicalDevice(), 1, &imagesInFlight[*imageIndex], VK_TRUE, UINT64_MAX);
      //}
      //imagesInFlight[*imageIndex] = inFlightFences[currentFrame];

      //VkSubmitInfo submitInfo = {};
      //submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

      //VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
      //VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
      //submitInfo.waitSemaphoreCount = 1;
      //submitInfo.pWaitSemaphores = waitSemaphores;
      //submitInfo.pWaitDstStageMask = waitStages;

      //submitInfo.commandBufferCount = 1;
      //submitInfo.pCommandBuffers = buffers;

      //VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
      //submitInfo.signalSemaphoreCount = 1;
      //submitInfo.pSignalSemaphores = signalSemaphores;

      //vkResetFences(m_pgpucontext->logicalDevice(), 1, &inFlightFences[currentFrame]);
      //if (vkQueueSubmit(m_pgpucontext->graphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) !=
      //   VK_SUCCESS) {
      //   throw ::exception(error_failed,"failed to submit draw command buffer!");
      //}

      //VkPresentInfoKHR presentInfo = {};
      //presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

      //presentInfo.waitSemaphoreCount = 1;
      //presentInfo.pWaitSemaphores = signalSemaphores;

      //VkSwapchainKHR swapChains[] = { swapChain };
      //presentInfo.swapchainCount = 1;
      //presentInfo.pSwapchains = swapChains;

      //presentInfo.pImageIndices = imageIndex;

      //auto result = vkQueuePresentKHR(m_pgpucontext->presentQueue(), &presentInfo);

      //currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

      //return result;

      return VK_SUCCESS;

   }


   void render_pass::createRenderPassImpl() 
   {

      //SwapChainSupportDetails swapChainSupport = m_pgpucontext->getSwapChainSupport();

      //VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
      //VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
      //VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

      //uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
      //if (swapChainSupport.capabilities.maxImageCount > 0 &&
      //   imageCount > swapChainSupport.capabilities.maxImageCount) {
      //   imageCount = swapChainSupport.capabilities.maxImageCount;
      //}

      //VkSwapchainCreateInfoKHR createInfo = {};
      //createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
      //createInfo.surface = m_pgpucontext->surface();

      //createInfo.minImageCount = imageCount;
      //createInfo.imageFormat = surfaceFormat.format;
      //createInfo.imageColorSpace = surfaceFormat.colorSpace;
      //createInfo.imageExtent = extent;
      //createInfo.imageArrayLayers = 1;
      //createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

      //QueueFamilyIndices indices = m_pgpucontext->findPhysicalQueueFamilies();
      //uint32_t queueFamilyIndices[] = { indices.graphicsFamily, indices.presentFamily };

      //if (indices.graphicsFamily != indices.presentFamily) {
      //   createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      //   createInfo.queueFamilyIndexCount = 2;
      //   createInfo.pQueueFamilyIndices = queueFamilyIndices;
      //}
      //else {
      //   createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      //   createInfo.queueFamilyIndexCount = 0;      // Optional
      //   createInfo.pQueueFamilyIndices = nullptr;  // Optional
      //}

      //createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
      //createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

      //createInfo.presentMode = presentMode;
      //createInfo.clipped = VK_TRUE;

      //createInfo.oldSwapchain = oldSwapChain == nullptr ? VK_NULL_HANDLE : oldSwapChain->swapChain;

      //if (vkCreateSwapchainKHR(m_pgpucontext->logicalDevice(), &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
      //   throw ::exception(error_failed,"failed to create swap chain!");
      //}

      //// we only specified a minimum number of images in the swap chain, so the implementation is
      //// allowed to create a swap chain with more. That's why we'll first query the final number of
      //// images with vkGetSwapchainImagesKHR, then resize the container and finally call it again to
      //// retrieve the handles.
      //vkGetSwapchainImagesKHR(m_pgpucontext->logicalDevice(), swapChain, &imageCount, nullptr);
      //swapChainImages.resize(imageCount);
      //vkGetSwapchainImagesKHR(m_pgpucontext->logicalDevice(), swapChain, &imageCount, swapChainImages.data());

      //swapChainImageFormat = surfaceFormat.format;
      //extent = extent;

   }

   
   void render_pass::createImageViews() 
   {

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

      //m_imageviews.resize(m_texturea.size());

      for (::collection::index i = 0; i < m_texturea.size(); i++) 
      {

         ::cast < texture > ptexture = m_texturea[i];

         ptexture->get_image_view();

         //VkImageViewCreateInfo viewInfo{};
         //viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
         //viewInfo.image = ptexture->m_vkimage;
         //viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
         //viewInfo.format = pcontext->m_formatImageDefault;
         //viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         //viewInfo.subresourceRange.baseMipLevel = 0;
         //viewInfo.subresourceRange.levelCount = 1;
         //viewInfo.subresourceRange.baseArrayLayer = 0;
         //viewInfo.subresourceRange.layerCount = 1;

         //if (vkCreateImageView(
         //   pcontext->logicalDevice(), 
         //   &viewInfo, 
         //   nullptr, 
         //   &m_imageviews[i]) !=
         //   VK_SUCCESS) 
         //{

         //   throw ::exception(error_failed,"failed to create texture image view!");

         //}

      }

   }


   void render_pass::createRenderPass() 
   {

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

      VkAttachmentDescription depthAttachment{};
      depthAttachment.format = findDepthFormat();
      depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
      depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
      depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
      depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

      VkAttachmentReference depthAttachmentRef{};
      depthAttachmentRef.attachment = 1;
      depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

      VkAttachmentDescription colorAttachment = {};
      colorAttachment.format = pcontext->m_formatImageDefault;
      colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
      colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
      colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
      colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
      colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

      VkAttachmentReference colorAttachmentRef = {};
      colorAttachmentRef.attachment = 0;
      colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

      VkSubpassDescription subpass = {};
      subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
      subpass.colorAttachmentCount = 1;
      subpass.pColorAttachments = &colorAttachmentRef;
      subpass.pDepthStencilAttachment = &depthAttachmentRef;

      VkSubpassDependency dependency = {};
      dependency.dstSubpass = 0;
      dependency.dstAccessMask =
         VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
      dependency.dstStageMask =
         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
      dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
      dependency.srcAccessMask = 0;
      dependency.srcStageMask =
         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

      VkAttachmentDescription attachments[2] = {colorAttachment, depthAttachment};
      VkRenderPassCreateInfo renderPassInfo = {};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
      renderPassInfo.attachmentCount = 2;
      renderPassInfo.pAttachments = attachments;
      renderPassInfo.subpassCount = 1;
      renderPassInfo.pSubpasses = &subpass;
      renderPassInfo.dependencyCount = 1;
      renderPassInfo.pDependencies = &dependency;

      if (vkCreateRenderPass(pcontext->logicalDevice(), &renderPassInfo, nullptr, &m_vkrenderpass) != VK_SUCCESS) 
      {

         throw ::exception(error_failed,"failed to create render pass!");

      }

   }


   void render_pass::createFramebuffers() 
   {

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

      m_framebuffers.resize(imageCount());

      for (size_t i = 0; i < imageCount(); i++) 
      {

         VkImageView imageView = getImageView(i);

         VkImageView attachments[2];

         attachments[0] = imageView;

         int iAttachmentCount;

         if (m_bWithDepth)
         {

            VkImageView depthImageView = getDepthImageView(i);

            attachments[1] = depthImageView;

            iAttachmentCount = 2;

         }
         else
         {

            iAttachmentCount = 1;

         }

         VkExtent2D extent = getExtent();
         VkFramebufferCreateInfo framebufferInfo = {};
         framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
         framebufferInfo.renderPass = m_vkrenderpass;
         framebufferInfo.attachmentCount = iAttachmentCount;
         framebufferInfo.pAttachments = attachments;
         framebufferInfo.width = extent.width;
         framebufferInfo.height = extent.height;
         framebufferInfo.layers = 1;

         auto& vkframebuffer = m_framebuffers[i];

         if (vkCreateFramebuffer(
            pcontext->logicalDevice(),
            &framebufferInfo,
            nullptr,
            &vkframebuffer) != VK_SUCCESS) 
         {

            throw ::exception(error_failed,"failed to create framebuffer!");

         }

         debug() << "created framebuffer " << i << " with image view " << imageView;

      }

   }


   void render_pass::createDepthResources()
   {

      if (!m_bWithDepth)
      {

         return;

      }

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

      VkFormat depthFormat = findDepthFormat();

      m_formatDepth = depthFormat;

      VkExtent2D extent = getExtent();

      //depthImages.resize(imageCount());
      //depthImageMemorys.resize(imageCount());
      //depthImageViews.resize(imageCount());

      auto iImageCount = imageCount();

      //for (int i = 0; i < depthImages.size(); i++) 
      for (int i = 0; i < iImageCount; i++)
      {
         getDepthImageView(i); // Ensure depth image view is created
         //VkImageCreateInfo imageInfo{};
         //imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
         //imageInfo.imageType = VK_IMAGE_TYPE_2D;
         //imageInfo.extent.width = extent.width;
         //imageInfo.extent.height = extent.height;
         //imageInfo.extent.depth = 1;
         //imageInfo.mipLevels = 1;
         //imageInfo.arrayLayers = 1;
         //imageInfo.format = depthFormat;
         //imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
         //imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
         //imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
         //   VK_IMAGE_USAGE_TRANSFER_DST_BIT;
         //imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
         //imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
         //imageInfo.flags = 0;

         //auto& depthImage = depthImages[i];
         //auto& depthImageMemory = depthImageMemorys[i];
         //pcontext->createImageWithInfo(
         //   imageInfo,
         //   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
         //   depthImage,
         //   depthImageMemory);

         //VkImageViewCreateInfo viewInfo{};
         //viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
         //viewInfo.image = depthImages[i];
         //viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
         //viewInfo.format = depthFormat;
         //viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
         //viewInfo.subresourceRange.baseMipLevel = 0;
         //viewInfo.subresourceRange.levelCount = 1;
         //viewInfo.subresourceRange.baseArrayLayer = 0;
         //viewInfo.subresourceRange.layerCount = 1;

         //auto& depthImageView = depthImageViews[i];

         //if (vkCreateImageView(pcontext->logicalDevice(), &viewInfo, nullptr, &depthImageView) != VK_SUCCESS) 
         //{

         //   throw ::exception(error_failed,"failed to create texture image view!");

         //}

         debug() << "created a depth resource";

      }

   }


   void render_pass::createSyncObjects() 
   {

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

      int iImageCount = m_texturea.size();
      int iMaxFramesInFlight = get_frame_count();

      imageAvailable.resize(iImageCount);
      imageAvailableSemaphores.resize(iImageCount);
      renderFinishedSemaphores.resize(iImageCount);
      inFlightFences.resize(iImageCount);
      imagesInFlight.resize(iImageCount, VK_NULL_HANDLE);


      VkSemaphoreCreateInfo semaphoreInfo = {};
      semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

      VkFenceCreateInfo fenceInfo = {};
      fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
      fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

      for (size_t i = 0; i < iImageCount; i++)
      {
         imageAvailable[i] = 0;
         if (vkCreateSemaphore(pcontext->logicalDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) !=
            VK_SUCCESS ||
            vkCreateSemaphore(pcontext->logicalDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) !=
            VK_SUCCESS 
            ||        vkCreateFence(pcontext->logicalDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS
             ) 
         {
            throw ::exception(error_failed,"failed to create synchronization objects for a frame!");
         }
      }
   }

   
   //VkSurfaceFormatKHR render_pass::chooseSwapSurfaceFormat(
   //   const ::array<VkSurfaceFormatKHR>& availableFormats) {
   //   for (const auto& availableFormat : availableFormats) {
   //      // SRGB can be changed to "UNORM" instead
   //      if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
   //         availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
   //         return availableFormat;
   //      }
   //   }

   //   return availableFormats[0];
   //}

   //VkPresentModeKHR render_pass::chooseSwapPresentMode(
   //   const ::array<VkPresentModeKHR>& availablePresentModes) {
   //   for (const auto& availablePresentMode : availablePresentModes) {
   //      if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
   //         debug() << "Present mode: Mailbox";
   //         return availablePresentMode;
   //      }
   //   }

   //   // for (const auto &availablePresentMode : availablePresentModes) {
   //   //   if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
   //   //     debug() << "Present mode: Immediate";
   //   //     return availablePresentMode;
   //   //   }
   //   // }

   //   debug() << "Present mode: V-Sync";
   //   return VK_PRESENT_MODE_FIFO_KHR;
   //}

   //VkExtent2D render_pass::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
   //   if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
   //      return capabilities.currentExtent;
   //   }
   //   else {
   //      VkExtent2D actualExtent = windowExtent;
   //      actualExtent.width = std::max(
   //         capabilities.minImageExtent.width,
   //         std::min(capabilities.maxImageExtent.width, actualExtent.width));
   //      actualExtent.height = std::max(
   //         capabilities.minImageExtent.height,
   //         std::min(capabilities.maxImageExtent.height, actualExtent.height));

   //      return actualExtent;
   //   }
   //}

   VkFormat render_pass::findDepthFormat() 
   {

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

      return pcontext->m_pgpudevice->m_pphysicaldevice->findSupportedFormat(
         { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
         VK_IMAGE_TILING_OPTIMAL,
         VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

   }

} // namespace gpu_vulkan




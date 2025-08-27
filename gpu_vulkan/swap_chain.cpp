#include "framework.h"
#include "command_buffer.h"
#include "context.h"
#include "physical_device.h"
#include "queue.h"
#include "renderer.h"
#include "render_target.h"
#include "shader.h"
#include "swap_chain.h"
#include "texture.h"
#include "acme/platform/application.h"
#include "aura/user/user/interaction.h"
#include "aura/windowing/window.h"
#include "bred/gpu/frame.h"


namespace gpu_vulkan
{


   swap_chain::swap_chain()
   {

      m_iSwapSeed = 0;
      m_iCurrentSwapSerial = 0;
      m_uCurrentSwapChainImage = 0;

   }


   swap_chain::~swap_chain()
   {

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpucontext;

      //for (auto imageView : m_imageviews) {
      //   vkDestroyImageView(pcontext->logicalDevice(), imageView, nullptr);
      //}
      //m_imageviews.clear();

      //if (m_vkswapchain != nullptr) {
      //   vkDestroySwapchainKHR(pcontext->logicalDevice(), m_vkswapchain, nullptr);
      //   m_vkswapchain = nullptr;
      //}

      //for (int i = 0; i < depthImages.size(); i++) {
      //   vkDestroyImageView(m_pgpucontext->logicalDevice(), depthImageViews[i], nullptr);
      //   vkDestroyImage(m_pgpucontext->logicalDevice(), depthImages[i], nullptr);
      //   vkFreeMemory(m_pgpucontext->logicalDevice(), depthImageMemorys[i], nullptr);
      //}

      //for (auto framebuffer : m_vkswapchainFramebuffers) {
      //   vkDestroyFramebuffer(m_pgpucontext->logicalDevice(), framebuffer, nullptr);
      //}

      //vkDestroyRenderPass(m_pgpucontext->logicalDevice(), m_vkrenderpass, nullptr);

      //// cleanup synchronization objects
      //for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      //   vkDestroySemaphore(m_pgpucontext->logicalDevice(), renderFinishedSemaphores[i], nullptr);
      //   vkDestroySemaphore(m_pgpucontext->logicalDevice(), imageAvailableSemaphores[i], nullptr);
      //   vkDestroyFence(m_pgpucontext->logicalDevice(), inFlightFences[i], nullptr);
      //}
      //for (::collection::index i = 0; i < m_ptexturea->size(); i++)
      //{

      //   vkDestroyFence(pcontext->logicalDevice(), inFlightFences[i], nullptr);

      //}

   }


   void swap_chain::initialize_gpu_swap_chain(::gpu::renderer* pgpurenderer)
   {

      ::gpu::swap_chain::initialize_gpu_swap_chain(pgpurenderer);

   }


   VkResult swap_chain::acquireNextImage()
   {

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpucontext;

      auto prenderer = pcontext->m_pgpurenderer;

      ::cast < render_target > prendertarget = prenderer->m_pgpurendertarget;

      ::cast < texture > ptexture = current_swap_chain_texture();

      auto psynchronization = ptexture->synchronization();

      auto imageIndex = &m_uCurrentSwapChainImage;

      prenderer->wait_swap_chain_command_buffer_ready();

      //// Wait for the fence of the current frame first (prevents CPU running too fast)
      ////vkWaitForFences(pcontext->logicalDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

      //auto fence = texture.in_flight_fence();

      //vkWaitForFences(pcontext->logicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
      ////vkResetFences(pcontext->logicalDevice(), 1, &inFlightFences[currentFrame]);


      VkResult result = vkAcquireNextImageKHR(
         pcontext->logicalDevice(),
         m_vkswapchain,
         UINT64_MAX,
         psynchronization->m_vksemaphoreAvailable,  // signal semaphore
         VK_NULL_HANDLE,
         imageIndex);

      if (result == VK_ERROR_OUT_OF_DATE_KHR) {
         // Swapchain needs to be recreated - handle outside
         return result;
      }
      else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
         throw ::exception(error_failed, "failed to acquire swap chain image!");
      }

      ::cast < ::gpu_vulkan::texture > ptextureAcquire = m_ptextureaSwapChain->element_at(*imageIndex);

      auto psynchronizationAcquire = ptextureAcquire->synchronization();

      //// If the image we acquired is already being used (fence not signaled), wait for it
      if (psynchronizationAcquire->m_vkfenceImageInFlight != VK_NULL_HANDLE) {
         vkWaitForFences(pcontext->logicalDevice(), 1, &psynchronizationAcquire->m_vkfenceImageInFlight, VK_TRUE, UINT64_MAX);
      }

      // Mark this image as now being in use by current frame
      psynchronizationAcquire->m_vkfenceImageInFlight = psynchronizationAcquire->in_flight_fence();

      return result;
      //vkWaitForFences(
      //   m_pgpucontext->logicalDevice(),
      //   1,
      //   &inFlightFences[m_pgpurenderer->get_frame_index()],
      //   VK_TRUE,
      //   std::numeric_limits<uint64_t>::max());

      //VkResult result = vkAcquireNextImageKHR(
      //   m_pgpucontext->logicalDevice(),
      //   m_vkswapchain,
      //   std::numeric_limits<uint64_t>::max(),
      //   imageAvailableSemaphores[m_pgpurenderer->get_frame_index()],  // must be a not signaled semaphore
      //   VK_NULL_HANDLE,
      //   imageIndex);
      //if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      //   // Swapchain needs to be recreated - handle outside
      //   return result;
      //}
      //else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      //   throw ::exception(error_failed, "failed to acquire swap chain image!");
      //}

      //// If the image we acquired is already being used (fence not signaled), wait for it
      //if (imagesInFlight[*imageIndex] != VK_NULL_HANDLE) {
      //   vkWaitForFences(m_pgpucontext->logicalDevice(), 1, &imagesInFlight[*imageIndex], VK_TRUE, UINT64_MAX);
      //}

      //// Mark this image as now being in use by current frame
      //imagesInFlight[*imageIndex] = inFlightFences[m_pgpurenderer->get_frame_index()];
      //return result;
   }


   //int swap_chain::get_frame_index()
   //{

   //   return (int) ::gpu_vulkan::render_pass::get_frame_index();

   //}


   void swap_chain::on_new_swap_chain()
   {

      ::gpu::swap_chain::on_new_swap_chain();

   }


   bool swap_chain::should_use_advanced_pipeline_synchronization()
   {

      return true;

   }


   //void swap_chain::create_render_pass()
   //{
   //   //if (m_pgpurenderer->m_pgpucontext->m_eoutput == ::gpu::e_output_swap_chain)
   //   {

   //      ::cast < swap_chain > pswapchain = m_pgpurenderer->m_pgpucontext->get_swap_chain();

   //      pswapchain->update_render_pass(m_pgpurenderer->m_pgpucontext, m_prenderpass->m_prenderpassOld);

   //      pswapchain->on_init_render_pass();

   //   }

   //}



   VkResult swap_chain::submitCommandBuffers2(
      command_buffer* pcommandbuffer,
      ::gpu::texture* pgputexture,
      const ::array < VkSemaphore >& semaphoreaWait,
      const ::array < VkPipelineStageFlags >& stageaWait,
      const ::array < VkSemaphore >& semaphoreaSignal)
   {

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpucontext;

      ::cast < ::gpu_vulkan::texture > ptexture = pgputexture;

      uint32_t* imageIndex = &m_uCurrentSwapChainImage;

      auto prendertarget = pcontext->m_pgpurenderer->m_pgpurendertarget;

      auto currentFrame = prendertarget->get_frame_index();

      ::cast < ::gpu_vulkan::texture > ptextureIndex = m_ptextureaSwapChain->element_at(*imageIndex);

      auto psynchronization = ptexture->synchronization();

      auto psynchronizationIndex = ptextureIndex->synchronization();

      // Use currentFrame to access per-frame sync objects
      //vkWaitForFences(pcontext->logicalDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
      //vkResetFences(pcontext->logicalDevice(), 1, &inFlightFences[currentFrame]);
      //if (VK_TIMEOUT == vkWaitForFences(pcontext->logicalDevice(), 1, &inFlightFences[m_uCurrentSwapChainImage], VK_TRUE, 0))
      auto fence = psynchronization->in_flight_fence();
      {


         vkWaitForFences(pcontext->logicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
      }
      vkResetFences(pcontext->logicalDevice(), 1, &fence);

      VkSubmitInfo submitInfo{};
      submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

      //VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
      ::comparable_array<VkSemaphore> waitSemaphores;
      ::array<VkPipelineStageFlags> waitStages;
      for (int i = 0; i < semaphoreaWait.size(); i++)
      {
         if (waitSemaphores.add_unique(semaphoreaWait[i]))
         {
            waitStages.add(stageaWait[i]);

         }

      }
      if (psynchronization && psynchronization->m_iImageAvailable > 0)
      {
         if (waitSemaphores.add_unique(psynchronization->m_vksemaphoreAvailable))
         {
            waitStages.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
         }
      }
      if (psynchronizationIndex && psynchronizationIndex->m_vksemaphoreAvailable)
      {
         if (waitSemaphores.add_unique(psynchronizationIndex->m_vksemaphoreAvailable))
         {
            waitStages.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
         }
      }
      if (psynchronization && psynchronization->m_iRendering > 0)
      {
         if (waitSemaphores.add_unique(psynchronization->m_vksemaphoreRenderFinished))
         {
            waitStages.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
         }
         psynchronization->m_iRendering = 0;
      }

      for (int i = 0; i < pcommandbuffer->m_semaphoreaWaitToSubmit.size(); i++)
      {
         if (waitSemaphores.add_unique(pcommandbuffer->m_semaphoreaWaitToSubmit[i]))
         {
            waitStages.add(pcommandbuffer->m_stageaWaitToSubmit[i]);

         }

      }
      pcommandbuffer->m_semaphoreaWaitToSubmit.clear();
      pcommandbuffer->m_stageaWaitToSubmit.clear();
      submitInfo.waitSemaphoreCount = (uint32_t)waitSemaphores.size();
      submitInfo.pWaitSemaphores = waitSemaphores.data();
      submitInfo.pWaitDstStageMask = waitStages.data();


      submitInfo.commandBufferCount = 1;
      VkCommandBuffer commandbuffera[] = { pcommandbuffer->m_vkcommandbuffer };
      submitInfo.pCommandBuffers = commandbuffera;

      ::array<VkSemaphore> signalSemaphores(semaphoreaSignal);

      if (psynchronizationIndex && psynchronizationIndex->m_vksemaphoreRenderFinished)
      {
         signalSemaphores.add(psynchronizationIndex->m_vksemaphoreRenderFinished);
      }
      signalSemaphores.append(::transfer(pcommandbuffer->m_semaphoreaSignalOnSubmit));
      submitInfo.signalSemaphoreCount = (uint32_t)signalSemaphores.count();
      submitInfo.pSignalSemaphores = signalSemaphores.data();

      ::cast < ::gpu_vulkan::queue > pqueueGraphics = pcontext->graphics_queue();

      auto vkqueueGraphics = pqueueGraphics->m_vkqueue;

      ::cast < ::gpu_vulkan::texture > ptextureIdx = ptextureIndex;

      //auto fence = texture.in_flight_fence();

      //VkResult vkresult = vkQueueSubmit(pcontext->graphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]);
      VkResult vkresult = vkQueueSubmit(vkqueueGraphics, 1,
         &submitInfo, fence);
      if (vkresult != VK_SUCCESS) {
         throw ::exception(error_failed, "failed to submit draw command buffer!");
      }

      auto pgpuqueuePresent = pcontext->present_queue();

      ::cast < ::gpu_vulkan::queue > pqueuePresent = pgpuqueuePresent;

      {

         auto pgpucommandbufferPresent = pcontext->beginSingleTimeCommands(pqueuePresent, ::gpu::e_command_buffer_present);

         ::cast < command_buffer > pcommandbufferPresent = pgpucommandbufferPresent;

         ::comparable_array<VkSemaphore> waitSemaphores2;
         ::array<VkPipelineStageFlags> waitStages2;
         VkSubmitInfo submitInfo{};

      submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

         for(auto semaphore : signalSemaphores)
         {
            if (waitSemaphores2.add_unique(semaphore))
            {
               waitStages2.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

            }
         }
         submitInfo.waitSemaphoreCount = (uint32_t)waitSemaphores2.size();
         submitInfo.pWaitSemaphores = waitSemaphores2.data();
         submitInfo.pWaitDstStageMask = waitStages2.data();
         submitInfo.commandBufferCount = 1;
         VkCommandBuffer commandbuffera[] = { pcommandbufferPresent->m_vkcommandbuffer };
         submitInfo.pCommandBuffers = commandbuffera;
         pcontext->endSingleTimeCommands(pcommandbufferPresent, 1, &submitInfo);
      }
      VkPresentInfoKHR presentInfo{};
      presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
      //presentInfo.waitSemaphoreCount = (uint32_t) signalSemaphores.size();
      //presentInfo.pWaitSemaphores = signalSemaphores.data();
      presentInfo.waitSemaphoreCount = (uint32_t)0;
      presentInfo.pWaitSemaphores = nullptr;
      presentInfo.swapchainCount = 1;
      presentInfo.pSwapchains = &m_vkswapchain;
      presentInfo.pImageIndices = imageIndex;

      VkResult result = vkQueuePresentKHR(pqueuePresent->m_vkqueue, &presentInfo);

      //currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
      m_iCurrentSwapChainFrame = (m_iCurrentSwapChainFrame + 1) % m_ptextureaSwapChain->size();

      return result;

      ////if (imagesInFlight[*imageIndex] != VK_NULL_HANDLE) {
      //  // vkWaitForFences(m_pgpucontext->logicalDevice(), 1, &imagesInFlight[*imageIndex], VK_TRUE, UINT64_MAX);
      ////}
      ////imagesInFlight[*imageIndex] = inFlightFences[currentFrame];

      //vkWaitForFences(m_pgpucontext->logicalDevice(), 1, &inFlightFences[m_pgpurenderer->get_frame_index()], VK_TRUE, UINT64_MAX);
      //vkResetFences(m_pgpucontext->logicalDevice(), 1, &inFlightFences[m_pgpurenderer->get_frame_index()]);

      //VkSubmitInfo submitInfo = {};
      //submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

      //VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[m_pgpurenderer->get_frame_index()] };
      //VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
      //submitInfo.waitSemaphoreCount = 1;
      //submitInfo.pWaitSemaphores = waitSemaphores;
      //submitInfo.pWaitDstStageMask = waitStages;

      //submitInfo.commandBufferCount = 1;
      //submitInfo.pCommandBuffers = buffers;

      //VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[m_pgpurenderer->get_frame_index()] };
      //submitInfo.signalSemaphoreCount = 1;
      //submitInfo.pSignalSemaphores = signalSemaphores;

      ////vkResetFences(m_pgpucontext->logicalDevice(), 1, &inFlightFences[currentFrame]);
      //if (vkQueueSubmit(m_pgpucontext->graphicsQueue(), 1, &submitInfo, inFlightFences[m_pgpurenderer->get_frame_index()]) !=
      //   VK_SUCCESS) {
      //   throw ::exception(error_failed,"failed to submit draw command buffer!");
      //}

      //VkPresentInfoKHR presentInfo = {};
      //presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

      //presentInfo.waitSemaphoreCount = 1;
      //presentInfo.pWaitSemaphores = signalSemaphores;

      //VkSwapchainKHR m_vkswapchains[] = { m_vkswapchain };
      //presentInfo.swapchainCount = 1;
      //presentInfo.pSwapchains = m_vkswapchains;

      //presentInfo.pImageIndices = imageIndex;

      //auto result = vkQueuePresentKHR(m_pgpucontext->presentQueue(), &presentInfo);

      //return result;

   }


   int swap_chain::get_image_index() const
   {

      return m_uCurrentSwapChainImage;

   }


   void swap_chain::create_images()
   {

      auto ptextureaOld = m_ptextureaSwapChain;

      if (ptextureaOld)
      {

         m_papplication->post([this, ptextureaOld]()
            {

               preempt(1_s);

               ptextureaOld->destroy();

            });

      }

      øconstruct_new(m_ptextureaSwapChain);

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpucontext;

      auto prenderer = pcontext->m_pgpurenderer;

      auto prendertarget = prenderer->m_pgpurendertarget;

      auto pgpucontext = pcontext;

      auto pgpudevice = pgpucontext->m_pgpudevice;

      auto pphysicaldevice = pgpudevice->m_pphysicaldevice;

      auto swapchainSupport = pphysicaldevice->querySwapChainSupport();

      VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapchainSupport.formats);
      VkPresentModeKHR presentMode = chooseSwapPresentMode(swapchainSupport.presentModes);
      VkExtent2D extent = chooseSwapExtent(swapchainSupport.capabilities);
      uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;
      if (swapchainSupport.capabilities.maxImageCount > 0 &&
         imageCount > swapchainSupport.capabilities.maxImageCount) {
         imageCount = swapchainSupport.capabilities.maxImageCount;
      }

      pcontext->on_resize({ extent.width, extent.height });

      VkSwapchainCreateInfoKHR createInfo = {};
      createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
      createInfo.surface = pcontext->m_pgpudevice->m_pphysicaldevice->surface();

      //createInfo.minImageCount = imageCount;
      createInfo.minImageCount = 3;
      createInfo.imageFormat = surfaceFormat.format;
      createInfo.imageColorSpace = surfaceFormat.colorSpace;
      createInfo.imageExtent = extent;
      createInfo.imageArrayLayers = 1;
      createInfo.imageUsage = 
         VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
         | VK_IMAGE_USAGE_TRANSFER_DST_BIT
         | VK_IMAGE_USAGE_SAMPLED_BIT;

      vulkan::QueueFamilyIndices indexes = pcontext->m_pgpudevice->m_pphysicaldevice->findQueueFamilies();
      uint32_t queueFamilyIndices[] =
      {
         (uint32_t)indexes.graphicsFamily,
         (uint32_t)indexes.presentFamily
      };

      if (indexes.graphicsFamily != indexes.presentFamily) {
         createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
         createInfo.queueFamilyIndexCount = 2;
         createInfo.pQueueFamilyIndices = queueFamilyIndices;
      }
      else {
         createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
         createInfo.queueFamilyIndexCount = 0;      // Optional
         createInfo.pQueueFamilyIndices = nullptr;  // Optional
      }

      createInfo.preTransform = swapchainSupport.capabilities.currentTransform;
      createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
      //createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;

      createInfo.presentMode = presentMode;
      createInfo.clipped = VK_TRUE;

      ::pointer < swap_chain> pswapchainOld = prendertarget->m_prendertargetOld;

      createInfo.oldSwapchain = pswapchainOld == nullptr ? VK_NULL_HANDLE : pswapchainOld->m_vkswapchain;

      auto logicalDevice = pcontext->logicalDevice();

      auto vkResultCreateSwapChain = vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &m_vkswapchain);

      if(vkResultCreateSwapChain != VK_SUCCESS)
      {
         
         m_iSwapSeed++;

         return;

      }

      // we only specified a minimum number of images in the swap chain, so the implementation is
      // allowed to create a swap chain with more. That's why we'll first query the final number of
      // images with vkGetSwapchainImagesKHR, then resize the container and finally call it again to
      // retrieve the handles.
      vkGetSwapchainImagesKHR(pcontext->logicalDevice(), m_vkswapchain, &imageCount, nullptr);

      //m_pgpurenderer->m_pgpurendertarget->m_iFrameCountRequest = imageCount;

      m_ptextureaSwapChain->set_size(imageCount);

      ::array < VkImage> imagea;

      imagea.set_size(m_ptextureaSwapChain->size());

      vkGetSwapchainImagesKHR(pcontext->logicalDevice(), m_vkswapchain, &imageCount, imagea.data());
      
      for (int i = 0; i < imagea.size(); i++)
      {

         auto & pgputexture = m_ptextureaSwapChain->element_at(i);

         ødefer_construct(pgputexture);

         ::int_rectangle rectangleTarget;

         rectangleTarget.left() = 0;
         rectangleTarget.top() = 0;
         rectangleTarget.set_width(extent.width);
         rectangleTarget.set_height(extent.height);

         pgputexture->m_bTransferDst = true;

         bool bWithDepth = false;

         pgputexture->initialize_image_texture(m_pgpucontext->m_pgpurenderer, rectangleTarget, bWithDepth);

         ::cast < ::gpu_vulkan::texture > ptexture = pgputexture;

         ptexture->m_vkimage = imagea[i];

      }

      //m_formatImage = surfaceFormat.format;
      //m_extent = extent;
      pcontext->m_rectangle.set_size({ (int)extent.width, (int)extent.height });

   }


   //void swap_chain::createImageViews()
   //{

   //   ::cast < ::gpu_vulkan::context > pcontext = m_pgpucontext;

   //   //m_imageviews.resize(m_texturea.size());

   //   //for (::collection::index i = 0; i < m_imageviews.size(); i++) 
   //   for (::collection::index i = 0; i < m_ptexturea->size(); i++)
   //   {
   //      getImageView(i);

   //      //::cast < texture > ptexture = m_texturea[i];

   //      //VkImageViewCreateInfo viewInfo{};
   //      //viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
   //      //viewInfo.image = ptexture->m_vkimage;
   //      //viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
   //      //viewInfo.format = m_formatImage;
   //      //viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   //      //viewInfo.subresourceRange.baseMipLevel = 0;
   //      //viewInfo.subresourceRange.levelCount = 1;
   //      //viewInfo.subresourceRange.baseArrayLayer = 0;
   //      //viewInfo.subresourceRange.layerCount = 1;

   //      //if (vkCreateImageView(pcontext->logicalDevice(), &viewInfo, nullptr, &m_imageviews[i]) !=
   //      //   VK_SUCCESS) {
   //      //   throw ::exception(error_failed, "failed to create texture image view!");
   //      //}
   //   }
   //}


   //void swap_chain::createRenderPass()
   //{

   //   ::cast < ::gpu_vulkan::context > pcontext = m_pgpucontext;

   //   VkAttachmentDescription depthAttachment{};
   //   depthAttachment.format = findDepthFormat();
   //   depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
   //   if (m_bLoadClearOp)
   //   {
   //      depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
   //   }
   //   else
   //   {
   //      depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
   //   }
   //   depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
   //   depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
   //   depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
   //   depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
   //   depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

   //   VkAttachmentReference depthAttachmentRef{};
   //   depthAttachmentRef.attachment = 1;
   //   depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

   //   VkAttachmentDescription colorAttachment = {};
   //   colorAttachment.format = m_formatImage;
   //   colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
   //   colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
   //   colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
   //   colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
   //   colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
   //   colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
   //   colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

   //   VkAttachmentReference colorAttachmentRef = {};
   //   colorAttachmentRef.attachment = 0;
   //   colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

   //   VkSubpassDescription subpass = {};
   //   subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
   //   subpass.colorAttachmentCount = 1;
   //   subpass.pColorAttachments = &colorAttachmentRef;

   //   if (m_bWithDepth)
   //   {
   //      subpass.pDepthStencilAttachment = &depthAttachmentRef;

   //   }

   //   //VkSubpassDependency dependencies[1]{};
   //   //dependencies[0].dstSubpass = 0;
   //   //dependencies[0].dstAccessMask =
   //   //   VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
   //   //dependencies[0].dstStageMask =
   //   //   VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
   //   //dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
   //   //dependencies[0].srcAccessMask = 0;
   //   //dependencies[0].srcStageMask =
   //   //   VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;



   //   //dependency[1].srcSubpass = 0;
   //   //dependency[1].dstSubpass = 0;
   //   ////dependency[1].srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
   //   ////dependency[1].dstStageMask = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;
   //   ////dependency[1].srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
   //   ////dependency[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
   //   //dependency[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
   //   //dependency[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
   //   //dependency[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
   //   //dependency[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
   //   //dependency[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;// if needed
   //   VkClearValue clearColor = {
   // .color = { { 0.0f, 0.0f, 0.0f, 0.0f } } // fully transparent
   //   };

   //   VkAttachmentDescription attachments[2] = { colorAttachment, depthAttachment };
   //   VkRenderPassCreateInfo renderPassInfo = {};
   //   renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

   //   if (m_bWithDepth)
   //   {
   //      renderPassInfo.attachmentCount = 2;
   //   }
   //   else
   //   {
   //      renderPassInfo.attachmentCount = 1;
   //   }
   //   renderPassInfo.pAttachments = attachments;
   //   renderPassInfo.subpassCount = 1;
   //   renderPassInfo.pSubpasses = &subpass;
   //   //renderPassInfo.dependencyCount = 1;
   //   //renderPassInfo.pDependencies = dependencies;

   //   if (vkCreateRenderPass(pcontext->logicalDevice(), &renderPassInfo, nullptr, &m_vkrenderpass) != VK_SUCCESS)
   //   {

   //      throw ::exception(error_failed, "failed to create render pass!");

   //   }

   //}


   //void swap_chain::createFramebuffers()
   //{
   //   render_pass::createFramebuffers();
   //   //swapChainFramebuffers.resize(imageCount());
   //   //for (size_t i = 0; i < imageCount(); i++) {
   //   //   std::array<VkImageView, 2> attachments = { m_imageviews[i], depthImageViews[i] };

   //   //   VkExtent2D m_vkswapchainExtent = getExtent();
   //   //   VkFramebufferCreateInfo framebufferInfo = {};
   //   //   framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
   //   //   framebufferInfo.renderPass = m_vkrenderpass;
   //   //   framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
   //   //   framebufferInfo.pAttachments = attachments.data();
   //   //   framebufferInfo.width = m_vkswapchainExtent.width;
   //   //   framebufferInfo.height = m_vkswapchainExtent.height;
   //   //   framebufferInfo.layers = 1;

   //   //   if (vkCreateFramebuffer(
   //   //      m_pgpucontext->logicalDevice(),
   //   //      &framebufferInfo,
   //   //      nullptr,
   //   //      &swapChainFramebuffers[i]) != VK_SUCCESS) {
   //   //      throw ::exception(error_failed,"failed to create framebuffer!");
   //   //   }
   //   //}
   //}


   //void swap_chain::createDepthResources()
   //{
   //   render_pass::createDepthResources();

   //   //VkFormat depthFormat = findDepthFormat();
   //   //m_formatDepth = depthFormat;
   //   //VkExtent2D m_vkswapchainExtent = getExtent();

   //   //depthImages.resize(imageCount());
   //   //depthImageMemorys.resize(imageCount());
   //   //depthImageViews.resize(imageCount());

   //   //for (int i = 0; i < depthImages.size(); i++) {
   //   //   VkImageCreateInfo imageInfo{};
   //   //   imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
   //   //   imageInfo.imageType = VK_IMAGE_TYPE_2D;
   //   //   imageInfo.extent.width = m_vkswapchainExtent.width;
   //   //   imageInfo.extent.height = m_vkswapchainExtent.height;
   //   //   imageInfo.extent.depth = 1;
   //   //   imageInfo.mipLevels = 1;
   //   //   imageInfo.arrayLayers = 1;
   //   //   imageInfo.format = depthFormat;
   //   //   imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
   //   //   imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
   //   //   imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
   //   //   imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
   //   //   imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
   //   //   imageInfo.flags = 0;

   //   //   m_pgpucontext->createImageWithInfo(
   //   //      imageInfo,
   //   //      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
   //   //      depthImages[i],
   //   //      depthImageMemorys[i]);

   //   //   VkImageViewCreateInfo viewInfo{};
   //   //   viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
   //   //   viewInfo.image = depthImages[i];
   //   //   viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
   //   //   viewInfo.format = depthFormat;
   //   //   viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
   //   //   viewInfo.subresourceRange.baseMipLevel = 0;
   //   //   viewInfo.subresourceRange.levelCount = 1;
   //   //   viewInfo.subresourceRange.baseArrayLayer = 0;
   //   //   viewInfo.subresourceRange.layerCount = 1;

   //   //   if (vkCreateImageView(m_pgpucontext->logicalDevice(), &viewInfo, nullptr, &depthImageViews[i]) != VK_SUCCESS) {
   //   //      throw ::exception(error_failed,"failed to create texture image view!");
   //   //   }
   //   //}
   //}


   //void swap_chain::createSyncObjects()
   //{
   //   //imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
   //   //renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);

   //   render_pass::createSyncObjects();


   //   //inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
   //   //imagesInFlight.resize(imageCount(), VK_NULL_HANDLE);

   //   ////VkSemaphoreCreateInfo semaphoreInfo = {};
   //   ////semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

   //   //VkFenceCreateInfo fenceInfo = {};
   //   //fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
   //   //fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

   //   //for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
   //   //   if (vkCreateFence(m_pgpucontext->logicalDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
   //   //   {
   //   //      throw ::exception(error_failed,"failed to create synchronization objects for a frame!");
   //   //   }
   //   //}
   //}


   VkSurfaceFormatKHR swap_chain::chooseSwapSurfaceFormat(const ::array<VkSurfaceFormatKHR>& availableFormats)
   {

      for (const auto& availableFormat : availableFormats) {
         // SRGB can be changed to "UNORM" instead
         //if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
         if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
         }
      }

      return availableFormats[0];
   }

   VkPresentModeKHR swap_chain::chooseSwapPresentMode(const ::array<VkPresentModeKHR>& availablePresentModes)
   {
      for (const auto& availablePresentMode : availablePresentModes)
      {

         if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
         {
            debug() << "Present mode: Mailbox";
            return availablePresentMode;
         }
      }

      for (const auto& availablePresentMode : availablePresentModes) {
         if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
            debug() << "Present mode: Immediate";
            return availablePresentMode;
         }
      }

      debug() << "Present mode: V-Sync";
      return VK_PRESENT_MODE_FIFO_KHR;
   }


   VkExtent2D swap_chain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
   {

      if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
      {

         return capabilities.currentExtent;

      }
      else
      {

         ::cast < ::gpu_vulkan::context > pcontext = m_pgpucontext;

         auto prenderer = pcontext->m_pgpurenderer;

         auto prendertarget = prenderer->m_pgpurendertarget;



         VkExtent2D actualExtent;

         actualExtent.width = m_ptextureaSwapChain->element_at(0)->width();
         actualExtent.height = m_ptextureaSwapChain->element_at(0)->height();

         actualExtent.width = std::max(
            capabilities.minImageExtent.width,
            std::min(capabilities.maxImageExtent.width, actualExtent.width));
         actualExtent.height = std::max(
            capabilities.minImageExtent.height,
            std::min(capabilities.maxImageExtent.height, actualExtent.height));

         return actualExtent;
      }
   }


   VkFormat swap_chain::findDepthFormat()
   {

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpucontext;

      return pcontext->m_pgpudevice->m_pphysicaldevice->findSupportedFormat(
         { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
         VK_IMAGE_TILING_OPTIMAL,
         VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

   }


   void swap_chain::present(::gpu::texture* pgputexture)
   {

      //if (!m_papplication->m_gpu.m_bUseSwapChainWindow
        // || m_pgpucontext->m_etype != ::gpu::context::e_type_window)
      //{

        // pcommandbuffer->begin_command_buffer(false);

      //}

      ::cast < ::gpu_vulkan::context > pgpucontext = m_pgpucontext;

      ::cast < renderer > pgpurenderer = pgpucontext->m_pgpurenderer;

      ::cast < render_target > prendertarget = pgpurenderer->m_pgpurendertarget;

      ::cast < ::gpu_vulkan::texture > ptexture = prendertarget->current_texture(::gpu::current_frame());

      ::cast < swap_chain > pswapchain = pgpucontext->m_pgpuswapchain;

      VkResult vkresultAcquireNextImage = pswapchain->acquireNextImage();

      if (vkresultAcquireNextImage == VK_ERROR_OUT_OF_DATE_KHR 
         || vkresultAcquireNextImage == VK_SUBOPTIMAL_KHR) 
      {
         
         m_iSwapSeed++;

      }

      defer_check_swap_chain();

      ::cast <::gpu_vulkan::texture> ptextureSwapChain = pswapchain->current_swap_chain_texture();

      if (::nok(m_pshaderPresent))
      {

         pgpucontext->øconstruct(m_pshaderPresent);

         m_pshaderPresent->m_bindingSampler.set();

         m_pshaderPresent->m_bDisableDepthTest = true;

         unsigned int fullscreen_vertex_shader[] = {
#include "shader/fullscreen.vert.spv.inl"
         };

         unsigned int fullscreen_fragment_shader[] = {
#include "shader/fullscreen.frag.spv.inl"
         };

         m_pshaderPresent->m_bEnableBlend = false;

         //m_pshaderPresent->m_bTextureAndSampler = true;

         m_pshaderPresent->m_bDisableDepthTest = true;

         m_pshaderPresent->m_bindingSampler.set();

         m_pshaderPresent->m_bClearColor = true;

         m_pshaderPresent->m_colorClear = ::color::yellow;

         m_pshaderPresent->initialize_shader_with_block(
            pgpurenderer,
            ::as_memory_block(fullscreen_vertex_shader),
            ::as_memory_block(fullscreen_fragment_shader),
            {},
            {},
            {},
            {},
            // this means the vertex input layout will be null/empty
            // the full screen shader is embed in the shader code
            ::gpu::shader::e_flag_clear_default_bindings_and_attributes_descriptions
         );

      }

      ::cast < command_buffer > pcommandbuffer = pgpurenderer->getCurrentCommandBuffer2(::gpu::current_frame());

      pcommandbuffer->begin_command_buffer(false);

      auto vkcommandbuffer = pcommandbuffer->m_vkcommandbuffer;

      ::cast < ::gpu_vulkan::texture > ptextureSrc = pgputexture;

      //{

      //   // 2. Clear
      //   VkClearColorValue clearColor = { .float32 = { 0.95f * 0.5f, 0.75f * 0.5f, 0.95f * 0.5f, 0.5f } };
      //   VkImageSubresourceRange range = {
      //       .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //       .baseMipLevel = 0,
      //       .levelCount = 1,
      //       .baseArrayLayer = 0,
      //       .layerCount = 1,
      //   };

      //   vkCmdClearColorImage(
      //      vkcommandbuffer,
      //      ptextureSrc->m_vkimage,
      //      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      //      &clearColor,
      //      1,
      //      &range);

      //}

      //pgpurenderer->m_pgpucontext->m_iOverrideFrame = get_image_index();


      auto scopedstateSwapChain = ptextureSwapChain->_scoped_state(pcommandbuffer,
         {
         VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
         VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
         }
      );

      auto scopedstateSrc = ptextureSrc->_scoped_state(
         pcommandbuffer,
         {
         0,
         VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT

         }
      );

      m_pshaderPresent->bind(ptextureSwapChain, ptextureSrc);

      pcommandbuffer->m_semaphoreaWaitToSubmit.add(ptextureSrc->synchronization()->m_vksemaphoreRenderFinished);
      pcommandbuffer->m_stageaWaitToSubmit.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

      pcommandbuffer->set_viewport(pgpucontext->m_rectangle.size());

      pcommandbuffer->set_scissor(pgpucontext->m_rectangle.size());


      pgpucontext->_001BeginRenderPass(pcommandbuffer, ptextureSwapChain);

      //pgpucontext->m_pcontext->VSSetShader(m_pvertexshaderFullscreen, nullptr, 0);
      //pgpucontext->m_pcontext->PSSetShader(m_ppixelshaderFullscreen, nullptr, 0);

      //pgpucontext->m_pcontext->PSSetShaderResources(
      //   0, 1, m_ptextureSwapChain->m_pshaderresourceview.pp());
      //pgpucontext->m_pcontext->PSSetSamplers(
      //   0, 1, m_ptextureSwapChain->m_psamplerstate.pp());

 /*     D3D11_VIEWPORT vp = {};
      vp.TopLeftX = 0;
      vp.TopLeftY = 0;
      vp.Width = static_cast<float>(m_size.cx());
      vp.Height = static_cast<float>(m_size.cy());
      vp.MinDepth = 0.0f;
      vp.MaxDepth = 1.0f;
      pgpucontext->m_pcontext->RSSetViewports(1, &vp);*/

      //D3D12_VIEWPORT viewport = {};
      //viewport.TopLeftX = 0;
      //viewport.TopLeftY = 0;
      //viewport.Width = static_cast<float>(m_size.cx());
      //viewport.Height = static_cast<float>(m_size.cy());
      //viewport.MinDepth = 0.0f;
      //viewport.MaxDepth = 1.0f;

      //D3D12_RECT scissorRect = {};
      //scissorRect.left = 0;
      //scissorRect.top = 0;
      //scissorRect.right = static_cast<float>(m_size.cx());
      //scissorRect.bottom = static_cast<float>(m_size.cy());

      //::cast < renderer > prenderer = m_pgpurenderer;


      //// 4. Set the viewport and scissor
      //pcommandlist->RSSetViewports(1, &viewport);
      //pcommandlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      //pcommandlist->DrawInstanced(3, 1, 0, 0);
      vkCmdDraw(vkcommandbuffer, 3, 1, 0, 0);


      pgpucontext->_001EndRenderPass(pcommandbuffer);

      //{

      //   // 2. Clear
      //   VkClearColorValue clearColor = {.float32 = { 0.5f *0.5f, 0.75f * 0.5f, 0.95f * 0.5f, 0.5f } };
      //   VkImageSubresourceRange range = {
      //       .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //       .baseMipLevel = 0,
      //       .levelCount = 1,
      //       .baseArrayLayer = 0,
      //       .layerCount = 1,
      //   };

      //   vkCmdClearColorImage(
      //      vkcommandbuffer,
      //      ptextureSwapChain->m_vkimage,
      //      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      //      &clearColor,
      //      1,
      //      &range);

      //}


      m_pshaderPresent->unbind();

      //pgpurenderer->m_pgpucontext->m_iOverrideFrame = -1;
      //FLOAT colorRGBA2[] = { 0.5f * 0.5f,0.75f * 0.5f, 0.95f * 0.5f, 0.5f };

      //pcommandlist->ClearRenderTargetView(ptextureSwapChain->m_handleRenderTargetView, 
        // colorRGBA2, 0, nullptr);

      //{

      //   FLOAT colorRGBA2[] = { 0.5f * 0.5f,0.75f * 0.5f, 0.95f * 0.5f, 0.5f };

      //   D3D12_RECT r[1];

      //   r[0].left = 200;
      //   r[0].top = 100;
      //   r[0].right = 300;
      //   r[0].bottom = 200;

      //   pcommandlist->ClearRenderTargetView(ptextureSwapChain->m_handleRenderTargetView,
      //      colorRGBA2, 1, r);

      //}

   }


} // namespace gpu_vulkans



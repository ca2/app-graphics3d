
// Created by camilo on 2025-06-12 21:08 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "layer.h"
#include "renderer.h"


namespace gpu_vulkan
{


   layer::layer()
   {


   }


   layer::~layer()
   {


   }


   void layer::initialize_gpu_layer(::gpu::renderer* pgpurenderer, int iFrameIndex, int iLayerIndex)
   {

      ::gpu::layer::initialize_gpu_layer(pgpurenderer, iFrameIndex, iLayerIndex);

      ::cast <context> pcontext = pgpurenderer->m_pgpucontext;

      VkSemaphoreCreateInfo semaphoreInfo = {};

      semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

      if (vkCreateSemaphore(pcontext->logicalDevice(),
         &semaphoreInfo, nullptr,
         &m_vksemaphoreRenderFinished) != VK_SUCCESS)
      {

         throw ::exception(error_failed, "failed to create synchronization objects for a layer!");

      }

   }

   

} // namespace gpu_vulkan



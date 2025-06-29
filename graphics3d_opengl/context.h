// From V0idsEmbrace@Twich continuum project
// renamed from context to context by 
// camilo on 2025-05-17 03:00 <3ThomasBorregaardSorensen!!
#pragma once

//
//#include "bred/gpu/context.h"
//#include "bred/graphics3d/context.h"
////#include "window.h"
////#include "opengl-graphics3d/graphics3d_opengl/context.h"
//#include "app-graphics3d/graphics3d_opengl/_opengl.h"
//// std lib headers
////#include <string>
////#include <vector>
//
//
//namespace graphics3d_opengl
//{
//
//   
//   class container;
//
//   //struct SwapChainSupportDetails
//   //{
//   //   
//   //   VkSurfaceCapabilitiesKHR capabilities;
//   //   ::array<VkSurfaceFormatKHR> formats;
//   //   ::array<VkPresentModeKHR> presentModes;
//
//   //};
//
//   //struct QueueFamilyIndices 
//   //{
//   //   uint32_t graphicsFamily;
//   //   uint32_t presentFamily;
//   //   bool graphicsFamilyHasValue = false;
//   //   bool presentFamilyHasValue = false;
//   //   bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
//   //};
//
//
//   class CLASS_DECL_GRAPHICS3D_OPENGL context :
//      virtual public ::gpu::context
//   {
//   public:
//
//#if defined(NDEBUG)
//      const bool enableValidationLayers = false;
//#else
//      const bool enableValidationLayers = true;
//#endif
//
//      ::pointer<::gpu::context>             m_pgpucontext;
//
//      //context(VkWindow& window);
//      //context(VkPhysicalDevice physicalDevice);
//      //context(::graphics3d_opengl::OpenGLDevice * pdevice);
//      context();
//      ~context();
//
//      virtual void initialize_context(::user::graphics3d* pimpact);
//
//
//      //VkCommandPool getCommandPool() { return m_vkcommandpool; }
//      //VkDevice logicalDevice() { return m_vkdevice; }
//
//      //VkSurfaceKHR surface() { return m_vksurfacekhr; }
//      //VkQueue graphicsQueue() { return m_vkqueueGraphics; }
//      //VkQueue presentQueue() { return m_vkqueuePresent; }
//
//      //SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); }
//      //uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
//      //QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); }
//      //VkFormat findSupportedFormat(
//      //   const ::array<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
//
//      //// Buffer Helper Functions
//      //void createBuffer(
//      //   VkDeviceSize size,
//      //   VkBufferUsageFlags usage,
//      //   VkMemoryPropertyFlags properties,
//      //   VkBuffer& buffer,
//      //   VkDeviceMemory& bufferMemory);
//      //VkCommandBuffer beginSingleTimeCommands();
//      //void endSingleTimeCommands(VkCommandBuffer commandBuffer);
//      //void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
//      //void copyBufferToImage(
//      //   VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);
//
//      //void createImageWithInfo(
//      //   const VkImageCreateInfo& imageInfo,
//      //   VkMemoryPropertyFlags properties,
//      //   VkImage& image,
//      //   VkDeviceMemory& imageMemory);
//
//      //VkPhysicalDeviceProperties properties;
//
//
//      //void submitWork(VkCommandBuffer cmdBuffer, VkQueue queue);
//
//   public:
//      void createInstance();
//      //void setupDebugMessenger();
//      //void createSurface();
//      //void pickPhysicalDevice();
//      //void createLogicalDevice();
//      //void createCommandPool();
//
//      //// helper functions
//      //bool isDeviceSuitable(VkPhysicalDevice pvkcdevice);
//      //::array<const char*> getRequiredExtensions();
//      //bool checkValidationLayerSupport();
//      //QueueFamilyIndices findQueueFamilies(VkPhysicalDevice pvkcdevice);
//      //void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
//      //void hasGflwRequiredInstanceExtensions();
//      //bool checkDeviceExtensionSupport(VkPhysicalDevice pvkcdevice);
//      //SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice pvkcdevice);
//
//      //VkInstance m_vkinstance;
//      //VkDebugUtilsMessengerEXT debugMessenger;
//      //VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
//      //::pointer < ::user::graphics3d > m_pimpact;
//
//      //VkCommandPool m_vkcommandpool;
//
//      //VkDevice m_vkdevice;
//      //VkSurfaceKHR m_vksurfacekhr;
//      //VkQueue m_vkqueueGraphics;
//      //VkQueue m_vkqueuePresent;
//
//      //const ::array<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
//      //const ::array<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
//
//      void clear(const ::color::color & color) override;
//
//
//   };
//
//
//}  // namespace graphics3d_opengl
//
//

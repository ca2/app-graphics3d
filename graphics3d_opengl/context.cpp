// From V0idsEmbrace@Twich continuum project
// renamed from context to context by 
// camilo on 2025-05-17 03:00 <3ThomasBorregaardSorensen!!
#include "framework.h"
//#include "context.h"
////#include "initializers.h"
//#include "tools.h"
//#include "bred/gpu/approach.h"
//#include "bred/user/user/graphics3d.h"
//// std headers
//#include <cstring>
//#include <iostream>
//#include <set>
//#include <unordered_set>
//
//
//namespace graphics3d_opengl
//{
//
//   //// local callback functions
//   //static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
//   //   VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
//   //   VkDebugUtilsMessageTypeFlagsEXT messageType,
//   //   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
//   //   void* pUserData) {
//   //   std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
//
//   //   return VK_FALSE;
//   //}
//
//   //VkResult CreateDebugUtilsMessengerEXT(
//   //   VkInstance m_vkinstance,
//   //   const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
//   //   const VkAllocationCallbacks* pAllocator,
//   //   VkDebugUtilsMessengerEXT* pDebugMessenger)
//   //{
//   //   auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
//   //      m_vkinstance,
//   //      "vkCreateDebugUtilsMessengerEXT");
//   //   if (func != nullptr) {
//   //      return func(m_vkinstance, pCreateInfo, pAllocator, pDebugMessenger);
//   //   }
//   //   else {
//   //      return VK_ERROR_EXTENSION_NOT_PRESENT;
//   //   }
//   //}
//
//   //void DestroyDebugUtilsMessengerEXT(
//   //   VkInstance m_vkinstance,
//   //   VkDebugUtilsMessengerEXT debugMessenger,
//   //   const VkAllocationCallbacks* pAllocator) {
//   //   auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
//   //      m_vkinstance,
//   //      "vkDestroyDebugUtilsMessengerEXT");
//   //   if (func != nullptr) {
//   //      func(m_vkinstance, debugMessenger, pAllocator);
//   //   }
//   //}
//
//   // class member functions
//   //context::context(::graphics3d_opengl::OpenGLDevice* pdevice) : m_vkdevice{pdevice->logicalDevice} {
//   context::context()
//   {
//
//      //m_vksurfacekhr = nullptr;
//      //m_vkqueuePresent = nullptr;
//      //m_vkqueueGraphics = nullptr;
//
//      //setupDebugMessenger();
//      //createSurface();
//      //pickPhysicalDevice();
//      //createLogicalDevice();
//      //createCommandPool();
//
//   }
//
//
//   context::~context()
//   {
//      //vkDestroyCommandPool(m_vkdevice, m_vkcommandpool, nullptr);
//      //vkDestroyDevice(m_vkdevice, nullptr);
//
//      //if (enableValidationLayers) {
//      //   DestroyDebugUtilsMessengerEXT(m_vkinstance, debugMessenger, nullptr);
//      //}
//
//      //if (m_vksurfacekhr)
//      //{
//      //   vkDestroySurfaceKHR(m_vkinstance, m_vksurfacekhr, nullptr);
//      //}
//      //vkDestroyInstance(m_vkinstance, nullptr);
//
//   }
//
//
//   void context::initialize_context(::user::graphics3d* pimpact)
//   {
//
//      //m_pimpact = pimpact;
//
//
//      createInstance();
//
//   }
//
//
//   void context::createInstance()
//   {
//
//      auto pfactoryGpu = factory("gpu", "opengl");
//
//      pfactoryGpu->merge_to_global_factory();
//
//      auto pgpu = system()->get_gpu();
//
//      m_pgpucontext = pgpu->create_context(this);
//
//      //m_pgpucontext->create_context();
//      //if (enableValidationLayers && !checkValidationLayerSupport()) {
//      //   throw ::exception(error_failed, "validation layers requested, but not available!");
//      //}
//
//      //VkApplicationInfo appInfo = {};
//      //appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//      //appInfo.pApplicationName = "LittleOpenGLEngine App";
//      //appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
//      //appInfo.pEngineName = "No Engine";
//      //appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
//      //appInfo.apiVersion = VK_API_VERSION_1_0;
//
//      //VkInstanceCreateInfo createInfo = {};
//      //createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//      //createInfo.pApplicationInfo = &appInfo;
//
//      //auto extensions = getRequiredExtensions();
//      //createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
//      //createInfo.ppEnabledExtensionNames = extensions.data();
//
//      //VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
//      //if (enableValidationLayers) {
//      //   createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
//      //   createInfo.ppEnabledLayerNames = validationLayers.data();
//
//      //   populateDebugMessengerCreateInfo(debugCreateInfo);
//      //   createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
//      //}
//      //else {
//      //   createInfo.enabledLayerCount = 0;
//      //   createInfo.pNext = nullptr;
//      //}
//
//      //if (vkCreateInstance(&createInfo, nullptr, &m_vkinstance) != VK_SUCCESS) {
//      //   throw ::exception(error_failed, "failed to create m_vkinstance!");
//      //}
//
//      //hasGflwRequiredInstanceExtensions();
//   }
//
//
//   //void context::pickPhysicalDevice()
//   //{
//   //   //uint32_t deviceCount = 0;
//   //   //vkEnumeratePhysicalDevices(m_vkinstance, &deviceCount, nullptr);
//   //   //if (deviceCount == 0) {
//   //   //   throw ::exception(error_failed, "failed to find GPUs with OpenGL support!");
//   //   //}
//   //   //std::cout << "Device count: " << deviceCount << std::endl;
//   //   //::array<VkPhysicalDevice> devices(deviceCount);
//   //   //vkEnumeratePhysicalDevices(m_vkinstance, &deviceCount, devices.data());
//
//   //   //for (const auto& pvkcdevice : devices) {
//   //   //   if (isDeviceSuitable(pvkcdevice)) {
//   //   //      physicalDevice = pvkcdevice;
//   //   //      break;
//   //   //   }
//   //   //}
//
//   //   //if (physicalDevice == VK_NULL_HANDLE) {
//   //   //   throw ::exception(error_failed, "failed to find a suitable GPU!");
//   //   //}
//
//   //   //vkGetPhysicalDeviceProperties(physicalDevice, &properties);
//   //   //std::cout << "physical pvkcdevice: " << properties.deviceName << std::endl;
//   //}
//
//
//   //void context::createLogicalDevice()
//   //{
//
//   //   //QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
//
//   //   //::array<VkDeviceQueueCreateInfo> queueCreateInfos;
//   //   //std::set<uint32_t> uniqueQueueFamilies;
//   //   //if (indices.graphicsFamilyHasValue)
//   //   //{
//   //   //   uniqueQueueFamilies.insert(indices.graphicsFamily);
//   //   //}
//   //   //if (indices.presentFamilyHasValue)
//   //   //{
//   //   //   uniqueQueueFamilies.insert(indices.presentFamily);
//   //   //}
//
//   //   //float queuePriority = 1.0f;
//   //   //for (uint32_t queueFamily : uniqueQueueFamilies)
//   //   //{
//   //   //   VkDeviceQueueCreateInfo queueCreateInfo = {};
//   //   //   queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//   //   //   queueCreateInfo.queueFamilyIndex = queueFamily;
//   //   //   queueCreateInfo.queueCount = 1;
//   //   //   queueCreateInfo.pQueuePriorities = &queuePriority;
//   //   //   queueCreateInfos.push_back(queueCreateInfo);
//   //   //}
//
//   //   //VkPhysicalDeviceFeatures deviceFeatures = {};
//   //   //deviceFeatures.samplerAnisotropy = VK_TRUE;
//
//   //   //VkDeviceCreateInfo createInfo = {};
//   //   //createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//
//   //   //createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
//   //   //createInfo.pQueueCreateInfos = queueCreateInfos.data();
//
//   //   //createInfo.pEnabledFeatures = &deviceFeatures;
//   //   //createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
//   //   //createInfo.ppEnabledExtensionNames = deviceExtensions.data();
//
//   //   //// might not really be necessary anymore because pvkcdevice specific validation layers
//   //   //// have been deprecated
//   //   //if (enableValidationLayers)
//   //   //{
//   //   //   createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
//   //   //   createInfo.ppEnabledLayerNames = validationLayers.data();
//   //   //}
//   //   //else
//   //   //{
//   //   //   createInfo.enabledLayerCount = 0;
//   //   //}
//
//   //   //if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_vkdevice) != VK_SUCCESS)
//   //   //{
//   //   //   throw ::exception(error_failed, "failed to create logical pvkcdevice!");
//   //   //}
//   //   //if (indices.graphicsFamilyHasValue)
//   //   //{
//   //   //   vkGetDeviceQueue(m_vkdevice, indices.graphicsFamily, 0, &m_vkqueueGraphics);
//   //   //}
//   //   //if (indices.presentFamilyHasValue)
//   //   //{
//   //   //   vkGetDeviceQueue(m_vkdevice, indices.presentFamily, 0, &m_vkqueuePresent);
//   //   //}
//   //}
//
//
//   //void context::createCommandPool()
//   //{
//   //   //QueueFamilyIndices queueFamilyIndices = findPhysicalQueueFamilies();
//
//   //   //VkCommandPoolCreateInfo poolInfo = {};
//   //   //poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
//   //   //poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
//   //   //poolInfo.flags =
//   //   //   VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
//
//   //   //if (vkCreateCommandPool(m_vkdevice, &poolInfo, nullptr, &m_vkcommandpool) != VK_SUCCESS) {
//   //   //   throw ::exception(error_failed, "failed to create command pool!");
//   //   //}
//   //}
//
//   //void context::createSurface() { window.createWindowSurface(m_vkinstance, &m_vksurfacekhr); }
//
//   //void context::createSurface() {}
//
//
//   //bool context::isDeviceSuitable(VkPhysicalDevice pvkcdevice)
//   //{
//
//   //   QueueFamilyIndices indices = findQueueFamilies(pvkcdevice);
//
//   //   bool extensionsSupported = checkDeviceExtensionSupport(pvkcdevice);
//
//   //   bool swapChainAdequate = false;
//   //   if (m_vksurfacekhr)
//   //   {
//   //      if (extensionsSupported)
//   //      {
//   //         SwapChainSupportDetails swapChainSupport = querySwapChainSupport(pvkcdevice);
//   //         swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
//   //      }
//   //   }
//   //   else
//   //   {
//
//   //      swapChainAdequate = true;
//
//   //   }
//
//   //   VkPhysicalDeviceFeatures supportedFeatures;
//   //   vkGetPhysicalDeviceFeatures(pvkcdevice, &supportedFeatures);
//
//   //   return (!m_vksurfacekhr || indices.isComplete()) && extensionsSupported && swapChainAdequate &&
//   //      supportedFeatures.samplerAnisotropy;
//
//   //}
//
//
//   //void context::populateDebugMessengerCreateInfo(
//   //   VkDebugUtilsMessengerCreateInfoEXT& createInfo)
//   //{
//
//   //   createInfo = {};
//   //   createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
//   //   createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
//   //      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
//   //   createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
//   //      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
//   //      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
//   //   createInfo.pfnUserCallback = debugCallback;
//   //   createInfo.pUserData = nullptr;  // Optional
//
//   //}
//
//
//   //void context::setupDebugMessenger()
//   //{
//
//   //   if (!enableValidationLayers)
//   //   {
//
//   //      return;
//
//   //   }
//
//   //   VkDebugUtilsMessengerCreateInfoEXT createInfo;
//
//   //   populateDebugMessengerCreateInfo(createInfo);
//
//   //   if (CreateDebugUtilsMessengerEXT(m_vkinstance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
//   //   {
//
//   //      throw ::exception(error_failed, "failed to set up debug messenger!");
//
//   //   }
//
//   //}
//
//
//   //bool context::checkValidationLayerSupport()
//   //{
//   //   uint32_t layerCount;
//   //   vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
//
//   //   ::array<VkLayerProperties> availableLayers(layerCount);
//   //   vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
//
//   //   for (const char* layerName : validationLayers)
//   //   {
//   //      bool layerFound = false;
//
//
//   //      printf_line("checking for validation layer : %s", layerName);
//   //      for (const auto& layerProperties : availableLayers)
//   //      {
//   //         printf_line("an available layer : %s", layerProperties.layerName);
//   //         if (strcmp(layerName, layerProperties.layerName) == 0)
//   //         {
//   //            layerFound = true;
//   //            break;
//   //         }
//   //      }
//
//   //      if (!layerFound) {
//   //         return false;
//   //      }
//   //   }
//
//   //   return true;
//
//   //}
//
//
//   //::array<const char*> context::getRequiredExtensions()
//   //{
//   //   uint32_t glfwExtensionCount = 0;
//   //   const char** glfwExtensions;
//   //   //glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
//
//   //   //::array<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
//   //   ::array<const char*> extensions;
//
//   //   if (enableValidationLayers)
//   //   {
//   //      extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
//   //   }
//
//   //   return extensions;
//   //   return {};
//   //}
//
//
//   //void context::hasGflwRequiredInstanceExtensions()
//   //{
//   //   uint32_t extensionCount = 0;
//   //   vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
//   //   ::array<VkExtensionProperties> extensions(extensionCount);
//   //   vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
//
//   //   std::cout << "available extensions:" << std::endl;
//   //   std::unordered_set<std::string> available;
//   //   for (const auto& extension : extensions) {
//   //      std::cout << "\t" << extension.extensionName << std::endl;
//   //      available.insert(extension.extensionName);
//   //   }
//
//   //   std::cout << "required extensions:" << std::endl;
//   //   auto requiredExtensions = getRequiredExtensions();
//   //   for (const auto& required : requiredExtensions)
//   //   {
//   //      std::cout << "\t" << required << std::endl;
//   //      if (available.find(required) == available.end()) {
//   //         throw ::exception(error_failed, "Missing required glfw extension");
//   //      }
//   //   }
//
//   //}
//
//
//   //bool context::checkDeviceExtensionSupport(VkPhysicalDevice pvkcdevice)
//   //{
//
//   //   uint32_t extensionCount;
//   //   vkEnumerateDeviceExtensionProperties(pvkcdevice, nullptr, &extensionCount, nullptr);
//
//   //   ::array<VkExtensionProperties> availableExtensions(extensionCount);
//   //   vkEnumerateDeviceExtensionProperties(
//   //      pvkcdevice,
//   //      nullptr,
//   //      &extensionCount,
//   //      availableExtensions.data());
//
//   //   std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
//
//   //   for (const auto& extension : availableExtensions) {
//   //      requiredExtensions.erase(extension.extensionName);
//   //   }
//
//   //   return requiredExtensions.empty();
//   //}
//
//
//   //QueueFamilyIndices context::findQueueFamilies(VkPhysicalDevice pvkcdevice)
//   //{
//
//   //   QueueFamilyIndices indices;
//
//   //   uint32_t queueFamilyCount = 0;
//   //   vkGetPhysicalDeviceQueueFamilyProperties(pvkcdevice, &queueFamilyCount, nullptr);
//
//   //   ::array<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
//   //   vkGetPhysicalDeviceQueueFamilyProperties(pvkcdevice, &queueFamilyCount, queueFamilies.data());
//
//   //   int i = 0;
//   //   for (const auto& queueFamily : queueFamilies)
//   //   {
//   //      if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
//   //      {
//   //         indices.graphicsFamily = i;
//   //         indices.graphicsFamilyHasValue = true;
//   //      }
//   //      if (m_vksurfacekhr)
//   //      {
//   //         VkBool32 presentSupport = false;
//   //         vkGetPhysicalDeviceSurfaceSupportKHR(pvkcdevice, i, m_vksurfacekhr, &presentSupport);
//   //         if (queueFamily.queueCount > 0 && presentSupport)
//   //         {
//   //            indices.presentFamily = i;
//   //            indices.presentFamilyHasValue = true;
//   //         }
//   //      }
//   //      if (indices.isComplete()) {
//   //         break;
//   //      }
//
//   //      i++;
//   //   }
//
//   //   return indices;
//
//   //}
//
//
//   //SwapChainSupportDetails context::querySwapChainSupport(VkPhysicalDevice pvkcdevice)
//   //{
//
//   //   SwapChainSupportDetails details{};
//
//   //   if (m_vksurfacekhr)
//   //   {
//
//   //      vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pvkcdevice, m_vksurfacekhr, &details.capabilities);
//
//   //      uint32_t formatCount;
//   //      vkGetPhysicalDeviceSurfaceFormatsKHR(pvkcdevice, m_vksurfacekhr, &formatCount, nullptr);
//
//   //      if (formatCount != 0) {
//   //         details.formats.resize(formatCount);
//   //         vkGetPhysicalDeviceSurfaceFormatsKHR(pvkcdevice, m_vksurfacekhr, &formatCount, details.formats.data());
//   //      }
//
//   //      uint32_t presentModeCount;
//   //      vkGetPhysicalDeviceSurfacePresentModesKHR(pvkcdevice, m_vksurfacekhr, &presentModeCount, nullptr);
//
//   //      if (presentModeCount != 0) {
//   //         details.presentModes.resize(presentModeCount);
//   //         vkGetPhysicalDeviceSurfacePresentModesKHR(
//   //            pvkcdevice,
//   //            m_vksurfacekhr,
//   //            &presentModeCount,
//   //            details.presentModes.data());
//   //      }
//
//   //   }
//   //   return details;
//   //}
//
//
//   //VkFormat context::findSupportedFormat(
//   //   const ::array<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
//   //   for (VkFormat format : candidates) {
//   //      VkFormatProperties props;
//   //      vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);
//
//   //      if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
//   //         return format;
//   //      }
//   //      else if (
//   //         tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
//   //         return format;
//   //      }
//   //   }
//   //   throw ::exception(error_failed, "failed to find supported format!");
//   //}
//
//
//   //uint32_t context::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
//   //   VkPhysicalDeviceMemoryProperties memProperties;
//   //   vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
//   //   for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
//   //      if ((typeFilter & (1 << i)) &&
//   //         (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
//   //         return i;
//   //      }
//   //   }
//
//   //   throw ::exception(error_failed, "failed to find suitable memory type!");
//   //}
//
//
//   //void context::createBuffer(
//   //   VkDeviceSize size,
//   //   VkBufferUsageFlags usage,
//   //   VkMemoryPropertyFlags properties,
//   //   VkBuffer& buffer,
//   //   VkDeviceMemory& bufferMemory)
//   //{
//
//   //   VkBufferCreateInfo bufferInfo{};
//   //   bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//   //   bufferInfo.size = size;
//   //   bufferInfo.usage = usage;
//   //   bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
//
//   //   if (vkCreateBuffer(m_vkdevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
//   //   {
//
//   //      throw ::exception(error_failed, "failed to create vertex buffer!");
//
//   //   }
//
//   //   VkMemoryRequirements memRequirements;
//   //   vkGetBufferMemoryRequirements(m_vkdevice, buffer, &memRequirements);
//
//   //   VkMemoryAllocateInfo allocInfo{};
//   //   allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//   //   allocInfo.allocationSize = memRequirements.size;
//   //   allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
//
//   //   if (vkAllocateMemory(m_vkdevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
//   //   {
//
//   //      throw ::exception(error_failed, "failed to allocate vertex buffer memory!");
//
//   //   }
//
//   //   vkBindBufferMemory(m_vkdevice, buffer, bufferMemory, 0);
//
//   //}
//
//
//   //VkCommandBuffer context::beginSingleTimeCommands()
//   //{
//
//   //   VkCommandBufferAllocateInfo allocInfo{};
//   //   allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//   //   allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//   //   allocInfo.commandPool = m_vkcommandpool;
//   //   allocInfo.commandBufferCount = 1;
//
//   //   VkCommandBuffer commandBuffer;
//   //   vkAllocateCommandBuffers(m_vkdevice, &allocInfo, &commandBuffer);
//
//   //   VkCommandBufferBeginInfo beginInfo{};
//   //   beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//   //   beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
//
//   //   vkBeginCommandBuffer(commandBuffer, &beginInfo);
//   //   return commandBuffer;
//
//   //}
//
//
//   //void context::endSingleTimeCommands(VkCommandBuffer commandBuffer)
//   //{
//
//   //   vkEndCommandBuffer(commandBuffer);
//
//   //   VkSubmitInfo submitInfo{};
//   //   submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
//   //   submitInfo.commandBufferCount = 1;
//   //   submitInfo.pCommandBuffers = &commandBuffer;
//
//   //   vkQueueSubmit(m_vkqueueGraphics, 1, &submitInfo, VK_NULL_HANDLE);
//   //   vkQueueWaitIdle(m_vkqueueGraphics);
//
//   //   vkFreeCommandBuffers(m_vkdevice, m_vkcommandpool, 1, &commandBuffer);
//
//   //}
//
//
//   //void context::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
//   //{
//   //   VkCommandBuffer commandBuffer = beginSingleTimeCommands();
//
//   //   VkBufferCopy copyRegion{};
//   //   copyRegion.srcOffset = 0;  // Optional
//   //   copyRegion.dstOffset = 0;  // Optional
//   //   copyRegion.size = size;
//   //   vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
//
//   //   endSingleTimeCommands(commandBuffer);
//
//   //}
//
//
//   //void context::copyBufferToImage(
//   //   VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount)
//   //{
//
//   //   VkCommandBuffer commandBuffer = beginSingleTimeCommands();
//
//   //   VkBufferImageCopy region{};
//   //   region.bufferOffset = 0;
//   //   region.bufferRowLength = 0;
//   //   region.bufferImageHeight = 0;
//
//   //   region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//   //   region.imageSubresource.mipLevel = 0;
//   //   region.imageSubresource.baseArrayLayer = 0;
//   //   region.imageSubresource.layerCount = layerCount;
//
//   //   region.imageOffset = { 0, 0, 0 };
//   //   region.imageExtent = { width, height, 1 };
//
//   //   vkCmdCopyBufferToImage(
//   //      commandBuffer,
//   //      buffer,
//   //      image,
//   //      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
//   //      1,
//   //      &region);
//
//   //   endSingleTimeCommands(commandBuffer);
//
//   //}
//
//
//   //void context::createImageWithInfo(
//   //   const VkImageCreateInfo& imageInfo,
//   //   VkMemoryPropertyFlags properties,
//   //   VkImage& image,
//   //   VkDeviceMemory& imageMemory)
//   //{
//
//   //   if (vkCreateImage(m_vkdevice, &imageInfo, nullptr, &image) != VK_SUCCESS)
//   //   {
//
//   //      throw ::exception(error_failed, "failed to create image!");
//
//   //   }
//
//   //   VkMemoryRequirements memRequirements;
//   //   vkGetImageMemoryRequirements(m_vkdevice, image, &memRequirements);
//
//   //   VkMemoryAllocateInfo allocInfo{};
//   //   allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//   //   allocInfo.allocationSize = memRequirements.size;
//   //   allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
//
//   //   if (vkAllocateMemory(m_vkdevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
//   //      throw ::exception(error_failed, "failed to allocate image memory!");
//   //   }
//
//   //   if (vkBindImageMemory(m_vkdevice, image, imageMemory, 0) != VK_SUCCESS) {
//   //      throw ::exception(error_failed, "failed to bind image memory!");
//   //   }
//   //}
//
//
//
//   //void context::submitWork(VkCommandBuffer cmdBuffer, VkQueue queue)
//   //{
//   //   VkSubmitInfo submitInfo = initializers::submit_info();
//   //   submitInfo.commandBufferCount = 1;
//   //   submitInfo.pCommandBuffers = &cmdBuffer;
//   //   //m_submitInfo.commandBufferCount = 1;
//   //   //m_submitInfo.pCommandBuffers = &cmdBuffer;
//   //   VkFenceCreateInfo fenceInfo = initializers::fence_create_info();
//   //   VkFence fence;
//   //   VK_CHECK_RESULT(vkCreateFence(m_vkdevice, &fenceInfo, nullptr, &fence));
//   //   VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, fence));
//   //   VK_CHECK_RESULT(vkWaitForFences(m_vkdevice, 1, &fence, VK_TRUE, UINT64_MAX));
//   //   vkDestroyFence(m_vkdevice, fence, nullptr);
//   //}
//
//void context::clear(const ::color::color & color) 
//{
//
//   // Clear the screen 
//   GLCheckError("");
//   //   glClearColor(0.678f, 0.847f, 0.902f, 1.0f);//
//   glClearColor(color.f32_red(), color.f32_green(), color.f32_blue(), color.f32_opacity());//
//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//   GLCheckError("");
//}
//
//
//
//} // namespace graphics3d_opengl
//
//
//

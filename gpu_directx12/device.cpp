// From gpu_directx12::device by
// camilo on 2025-05-27 04:54 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "approach.h"
#include "buffer.h"
#include "device.h"
#include "physical_device.h"
#include "program.h"
#include "renderer.h"
#include "shader.h"
#include "swap_chain.h"
#include "acme/filesystem/filesystem/file_context.h"
#include "acme/platform/application.h"
#include "aura/graphics/image/image.h"
#include "bred/gpu/types.h"
#include "gpu_directx12/descriptors.h"
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include "glm/mat4x4.hpp"
#include "initializers.h"
#include "windowing_win32/window.h"
#pragma comment(lib, "dcomp")
#include <glm/glm.hpp>
#include <DirectXMath.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

DirectX::XMMATRIX GLMToDX_Transposed(const glm::mat4& m) {
   return DirectX::XMMatrixTranspose(
      DirectX::XMMATRIX(reinterpret_cast<const float*>(&m))
   );
}
DirectX::XMMATRIX GLMToDX(const glm::mat4& m) {
   return DirectX::XMMATRIX(reinterpret_cast<const float*>(&m));
}
using namespace directx12;


namespace gpu_directx12
{


   device::device()
   {

      m_bMesa = false;

      m_estatus = error_not_initialized;

   }


   device::~device()
   {


   }


   void device::defer_shader_memory(::memory& memory, const ::file::path& pathShader)
   {

      if (memory.is_empty())
      {

         auto path = m_pgpuapproach->shader_path(pathShader);

         memory = file()->as_memory(path);

      }

   }


   ::pointer < ::gpu::device > allocate_system_context(::particle* pparticle)
   {

      return pparticle->__create_new <device>();

   }


   bool device::is_mesa()
   {

      return m_bMesa;

   }


   void device::_create_offscreen_window(const ::int_size& size)
   {
      //if (::IsWindow(m_hwnd))
      //{

      //   if (!::SetWindowPos(m_hwnd,
      //      nullptr, 0, 0,
      //      size.cx()
      //      , size.cy(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE
      //      | SWP_NOCOPYBITS | SWP_NOSENDCHANGING
      //      | SWP_NOREPOSITION | SWP_NOREDRAW))
      //   {

      //      information() << "SetWindowPos Failed";

      //   }


      //   //return m_hwnd;

      //}
      //else
      //{

      //   LPCTSTR lpClassName = L"draw2d_directx12_offscreen_buffer_window";
      //   LPCTSTR lpWindowName = L"draw2d_directx12_offscreen_buffer_window";
      //   //unsigned int dwStyle = WS_CAPTION | WS_POPUPWINDOW; // | WS_VISIBLE
      //   unsigned int dwExStyle = 0;
      //   unsigned int dwStyle = WS_OVERLAPPEDWINDOW;
      //   dwStyle |= WS_POPUP;
      //   //dwStyle |= WS_VISIBLE;
      //   //dwStyle |= WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
      //   dwStyle &= ~WS_CAPTION;
      //   //dwStyle = 0;
      //   dwStyle &= ~WS_THICKFRAME;
      //   dwStyle &= ~WS_BORDER;
      //   int x = 0;
      //   int y = 0;
      //   int nWidth = size.cx();
      //   int nHeight = size.cy();
      //   HWND hWndParent = nullptr;
      //   HMENU hMenu = nullptr;
      //   HINSTANCE hInstance = ::GetModuleHandleW(L"gpu_directx12.dll");
      //   LPVOID lpParam = nullptr;

      //   m_hwnd = CreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

      //   if (!m_hwnd)
      //   {

      //      informationf("MS GDI - CreateWindow failed");

      //      informationf("last-error code: %d\n", GetLastError());

      //      throw ::exception(error_failed);

      //   }

      //   //return m_hwnd;

      //}


   }


   void device::initialize_gpu_device_for_swap_chain(::gpu::approach* pgpuapproachParam, ::windowing::window* pwindow)
   {

      ::gpu::device::initialize_gpu_device_for_swap_chain(pgpuapproachParam, pwindow);

      initialize_swap_chain(pwindow);

      ::cast < approach > pgpuapproach = pgpuapproachParam;

      if (!pgpuapproach)
      {

         throw ::exception(error_failed);

      }

      m_pgpuapproach = pgpuapproach.m_p;

      auto pphysicaldevice = pgpuapproach->m_pphysicaldevice;

      m_pphysicaldevice = pphysicaldevice;

      if (m_papplication->m_gpu.m_bUseSwapChainWindow)
      {

         m_pphysicaldevice->createWindowSurface(pwindow);

      }

      //if (!m_pswapchain->m_bSwapChainInitialized)
      //{

      //   m_pswapchain->initialize_gpu_swap_chain(this, pwindow);

      //}

      //      get_swap_chain();

   }


   void device::initialize_gpu_device_for_off_screen(::gpu::approach* pgpuapproachParam, const ::int_rectangle& rectanglePlacement)
   {

      bool bAddSwapChainSupport = false;

      initialize_cpu_buffer(rectanglePlacement.size());

      ::cast < approach > pgpuapproach = pgpuapproachParam;

      if (!pgpuapproach)
      {

         throw ::exception(error_failed);

      }

      m_pgpuapproach = pgpuapproach.m_p;

      auto pphysicaldevice = pgpuapproach->m_pphysicaldevice;

      m_pphysicaldevice = pphysicaldevice;

   }


   bool device::isExtensionSupported(const ::scoped_string& scopedstrExtension)
   {

      return m_straSupportedExtensions.contains(scopedstrExtension);

   }


   string device::get_shader_version_text()
   {

      return "#version 330 core";

   }


   void device::_translate_shader(string_array& stra)
   {

      ::gpu::device::_translate_shader(stra);

      character_count iFindPrecision = stra.case_insensitive_find_first_begins("precision ");

      if (iFindPrecision >= 0)
      {

         stra[iFindPrecision] = "precision highp float;";

      }
      else
      {

         stra.insert_at(1, "precision highp float;");

         iFindPrecision = 1;

      }

   }


   //// local callback functions
   //static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
   //   VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
   //   VkDebugUtilsMessageTypeFlagsEXT messageType,
   //   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
   //   void* pUserData) {
   //   //std::cerr << "validation layer: " << pCallbackData->pMessage;
   //   warning() << "validation layer: " << pCallbackData->pMessage;
   //   return VK_FALSE;
   //}

   //HRESULT CreateDebugUtilsMessengerEXT(
   //   VkInstance m_vkinstance,
   //   const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
   //   const VkAllocationCallbacks* pAllocator,
   //   VkDebugUtilsMessengerEXT* pDebugMessenger)
   //{
   //   auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
   //      m_vkinstance,
   //      "vkCreateDebugUtilsMessengerEXT");
   //   if (func != nullptr) {
   //      return func(m_vkinstance, pCreateInfo, pAllocator, pDebugMessenger);
   //   }
   //   else {
   //      return VK_ERROR_EXTENSION_NOT_PRESENT;
   //   }
   //}

   //void DestroyDebugUtilsMessengerEXT(
   //   VkInstance m_vkinstance,
   //   VkDebugUtilsMessengerEXT debugMessenger,
   //   const VkAllocationCallbacks* pAllocator) {
   //   auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
   //      m_vkinstance,
   //      "vkDestroyDebugUtilsMessengerEXT");
   //   if (func != nullptr) {
   //      func(m_vkinstance, debugMessenger, pAllocator);
   //   }
   //}

   //// class member functions
   ////device::device(::graphics3d_directx12::DirectX12Device* pgpudevice) : m_vkdevice{pgpudevice->logicalDevice} {
   //device::device()
   //{

   //   m_vksurfacekhr = nullptr;
   //   m_vkqueuePresent = nullptr;
   //   m_vkqueueGraphics = nullptr;

   //   createInstance();
   //   setupDebugMessenger();
   //   createSurface();
   //   pickPhysicalDevice();
   //   createLogicalDevice();
   //   createCommandPool();
   //}

   //device::~device()
   //{
   //   vkDestroyCommandPool(m_vkdevice, m_vkcommandpool, nullptr);
   //   vkDestroyDevice(m_vkdevice, nullptr);

   //   if (enableValidationLayers) {
   //      DestroyDebugUtilsMessengerEXT(m_vkinstance, debugMessenger, nullptr);
   //   }

   //   if (m_vksurfacekhr)
   //   {
   //      vkDestroySurfaceKHR(m_vkinstance, m_vksurfacekhr, nullptr);
   //   }
   //   vkDestroyInstance(m_vkinstance, nullptr);

   //}


   //void device::initialize_context(::user::graphics3d * pimpact)
   //{

   //   m_pimpact = pimpact;


   //   validationLayers.add("VK_LAYER_KHRONOS_validation");

   //   if (m_papplication->m_bUseSwapChainWindow)
   //   {

   //      deviceExtensions.add(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

   //   }


   //}


   //void device::createInstance()
   //{

   //   if (enableValidationLayers && !checkValidationLayerSupport()) {
   //      throw ::exception(error_failed,"validation layers requested, but not available!");
   //   }

   //   VkApplicationInfo appInfo = {};
   //   appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
   //   appInfo.pApplicationName = "LittleDirectX12Engine App";
   //   appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
   //   appInfo.pEngineName = "No Engine";
   //   appInfo.approachVersion = VK_MAKE_VERSION(1, 0, 0);
   //   appInfo.apiVersion = VK_API_VERSION_1_0;

   //   VkInstanceCreateInfo createInfo = {};
   //   createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
   //   createInfo.pApplicationInfo = &appInfo;

   //   auto extensions = getRequiredExtensions();
   //   createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
   //   createInfo.ppEnabledExtensionNames = extensions.data();

   //   VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
   //   if (enableValidationLayers) {
   //      createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
   //      createInfo.ppEnabledLayerNames = validationLayers.data();

   //      populateDebugMessengerCreateInfo(debugCreateInfo);
   //      createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
   //   }
   //   else {
   //      createInfo.enabledLayerCount = 0;
   //      createInfo.pNext = nullptr;
   //   }

   //   if (vkCreateInstance(&createInfo, nullptr, &m_vkinstance) != VK_SUCCESS) {
   //      throw ::exception(error_failed,"failed to create m_vkinstance!");
   //   }

   //   hasGflwRequiredInstanceExtensions();
   //}


   //void device::pickPhysicalDevice()
   //{
   //   uint32_t deviceCount = 0;
   //   vkEnumeratePhysicalDevices(m_vkinstance, &deviceCount, nullptr);
   //   if (deviceCount == 0) {
   //      throw ::exception(error_failed,"failed to find GPUs with DirectX12 support!");
   //   }
   //   information() << "Device count: " << deviceCount;
   //   ::array<VkPhysicalDevice> devices(deviceCount);
   //   vkEnumeratePhysicalDevices(m_vkinstance, &deviceCount, devices.data());

   //   for (const auto & pvkcdevice : devices) {
   //      if (isDeviceSuitable(pvkcdevice)) {
   //         physicalDevice = pvkcdevice;
   //         break;
   //      }
   //   }

   //   if (physicalDevice == VK_NULL_HANDLE) {
   //      throw ::exception(error_failed,"failed to find a suitable GPU!");
   //   }

   //   vkGetPhysicalDeviceProperties(physicalDevice, &properties);
   //   information() << "physical pvkcdevice: " << properties.deviceName;
   //}


   //void device::createLogicalDevice()
   //{

   //   QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

   //   ::array<VkDeviceQueueCreateInfo> queueCreateInfos;
   //   std::set<uint32_t> uniqueQueueFamilies;
   //   if (indices.graphicsFamilyHasValue)
   //   {
   //      uniqueQueueFamilies.insert(indices.graphicsFamily);
   //   }
   //   if (indices.presentFamilyHasValue)
   //   {
   //      uniqueQueueFamilies.insert(indices.presentFamily);
   //   }

   //   float queuePriority = 1.0f;
   //   for (uint32_t queueFamily : uniqueQueueFamilies)
   //   {
   //      VkDeviceQueueCreateInfo queueCreateInfo = {};
   //      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
   //      queueCreateInfo.queueFamilyIndex = queueFamily;
   //      queueCreateInfo.queueCount = 1;
   //      queueCreateInfo.pQueuePriorities = &queuePriority;
   //      queueCreateInfos.add(queueCreateInfo);
   //   }

   //   VkPhysicalDeviceFeatures deviceFeatures = {};
   //   deviceFeatures.samplerAnisotropy = VK_TRUE;

   //   VkDeviceCreateInfo createInfo = {};
   //   createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

   //   createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
   //   createInfo.pQueueCreateInfos = queueCreateInfos.data();

   //   createInfo.pEnabledFeatures = &deviceFeatures;
   //   createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
   //   createInfo.ppEnabledExtensionNames = deviceExtensions.data();

   //   // might not really be necessary anymore because pvkcdevice specific validation layers
   //   // have been deprecated
   //   if (enableValidationLayers)
   //   {
   //      createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
   //      createInfo.ppEnabledLayerNames = validationLayers.data();
   //   }
   //   else
   //   {
   //      createInfo.enabledLayerCount = 0;
   //   }

   //   if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_vkdevice) != VK_SUCCESS)
   //   {
   //      throw ::exception(error_failed,"failed to create logical pvkcdevice!");
   //   }
   //   if (indices.graphicsFamilyHasValue)
   //   {
   //      vkGetDeviceQueue(m_vkdevice, indices.graphicsFamily, 0, &m_vkqueueGraphics);
   //   }
   //   if (indices.presentFamilyHasValue)
   //   {
   //      vkGetDeviceQueue(m_vkdevice, indices.presentFamily, 0, &m_vkqueuePresent);
   //   }
   //}


   //void device::createCommandPool()
   //{
   //   QueueFamilyIndices queueFamilyIndices = findPhysicalQueueFamilies();

   //   VkCommandPoolCreateInfo poolInfo = {};
   //   poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
   //   poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
   //   poolInfo.flags =
   //      VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

   //   if (vkCreateCommandPool(m_vkdevice, &poolInfo, nullptr, &m_vkcommandpool) != VK_SUCCESS) {
   //      throw ::exception(error_failed,"failed to create command pool!");
   //   }
   //}

   //void device::createSurface() { window.createWindowSurface(m_vkinstance, &m_vksurfacekhr); }

   //void device::_createSurface() {}


   //bool device::isDeviceSuitable(VkPhysicalDevice pvkcdevice)
   //{

   //   QueueFamilyIndices indices = findQueueFamilies(pvkcdevice);

   //   bool extensionsSupported = checkDeviceExtensionSupport(pvkcdevice);

   //   bool swapChainAdequate = false;
   //   if (m_vksurfacekhr)
   //   {
   //      if (extensionsSupported)
   //      {
   //         SwapChainSupportDetails swapChainSupport = querySwapChainSupport(pvkcdevice);
   //         swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
   //      }
   //   }
   //   else
   //   {

   //      swapChainAdequate = true;

   //   }

   //   VkPhysicalDeviceFeatures supportedFeatures;
   //   vkGetPhysicalDeviceFeatures(pvkcdevice, &supportedFeatures);

   //   return (!m_vksurfacekhr || indices.isComplete()) && extensionsSupported && swapChainAdequate &&
   //      supportedFeatures.samplerAnisotropy;

   //}


   //void device::populateDebugMessengerCreateInfo(
   //   VkDebugUtilsMessengerCreateInfoEXT& createInfo)
   //{

   //   createInfo = {};
   //   createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
   //   createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
   //      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
   //   createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
   //      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
   //      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
   //   createInfo.pfnUserCallback = debugCallback;
   //   createInfo.pUserData = nullptr;  // Optional

   //}


   //void device::setupDebugMessenger()
   //{

   //   if (!enableValidationLayers)
   //   {

   //      return;

   //   }

   //   VkDebugUtilsMessengerCreateInfoEXT createInfo;

   //   populateDebugMessengerCreateInfo(createInfo);

   //   if (CreateDebugUtilsMessengerEXT(m_vkinstance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
   //   {

   //      throw ::exception(error_failed,"failed to set up debug messenger!");

   //   }

   //}


   bool device::checkValidationLayerSupport()
   {
      //uint32_t layerCount;
      //vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

      //::array<VkLayerProperties> availableLayers(layerCount);
      //vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

      //for (const char* layerName : validationLayers)
      //{
      //   bool layerFound = false;


      //   printf_line("checking for validation layer : %s", layerName);
      //   for (const auto& layerProperties : availableLayers)
      //   {
      //      printf_line("an available layer : %s", layerProperties.layerName);
      //      if (strcmp(layerName, layerProperties.layerName) == 0)
      //      {
      //         layerFound = true;
      //         break;
      //      }
      //   }

      //   if (!layerFound) {
      //      return false;
      //   }
      //}

      return true;

   }


   //::array<const char *> device::getRequiredExtensions()
   //{
   //   uint32_t glfwExtensionCount = 0;
   //   const char ** glfwExtensions;
   //   //glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

   //   //::array<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
   //   ::array<const char *> extensions;

   //   if (enableValidationLayers)
   //   {
   //      extensions.add(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
   //   }

   //   return extensions;
   //   return {};
   //}


   //void device::hasGflwRequiredInstanceExtensions()
   //{
   //   uint32_t extensionCount = 0;
   //   vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
   //   ::array<VkExtensionProperties> extensions(extensionCount);
   //   vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

   //   information() << "available extensions:";
   //   set<::string> available;
   //   for (const auto & extension : extensions) {
   //      information() << "\t" << extension.extensionName;
   //      available.set_at(extension.extensionName);
   //   }

   //   information() << "required extensions:";
   //   auto requiredExtensions = getRequiredExtensions();
   //   for (const auto & required : requiredExtensions)
   //   {
   //      information() << "\t" << required;
   //      if (!available.plookup(required)) {
   //         throw ::exception(error_failed,"Missing required glfw extension");
   //      }
   //   }

   //}


   //bool device::checkDeviceExtensionSupport(VkPhysicalDevice pvkcdevice)
   //{

   //   uint32_t extensionCount;
   //   vkEnumerateDeviceExtensionProperties(pvkcdevice, nullptr, &extensionCount, nullptr);

   //   ::array<VkExtensionProperties> availableExtensions(extensionCount);
   //   vkEnumerateDeviceExtensionProperties(
   //      pvkcdevice,
   //      nullptr,
   //      &extensionCount,
   //      availableExtensions.data());

   //   set<::string> requiredExtensions(deviceExtensions);

   //   for (const auto & extension : availableExtensions) {
   //      requiredExtensions.erase(extension.extensionName);
   //   }

   //   return requiredExtensions.empty();
   //}


   //void device::createBuffer(
   //   VkDeviceSize size,
   //   VkBufferUsageFlags usage,
   //   VkMemoryPropertyFlags properties,
   //   VkBuffer& buffer,
   //   VkDeviceMemory& bufferMemory)
   //{

   //   VkBufferCreateInfo bufferInfo{};
   //   bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
   //   bufferInfo.size = size;
   //   bufferInfo.usage = usage;
   //   bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

   //   if (vkCreateBuffer(m_vkdevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
   //   {

   //      throw ::exception(error_failed, "failed to create vertex buffer!");

   //   }

   //   VkMemoryRequirements memRequirements;
   //   vkGetBufferMemoryRequirements(m_vkdevice, buffer, &memRequirements);

   //   VkMemoryAllocateInfo allocInfo{};
   //   allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
   //   allocInfo.allocationSize = memRequirements.size;
   //   allocInfo.memoryTypeIndex = m_pphysicaldevice->findMemoryType(memRequirements.memoryTypeBits, properties);

   //   if (vkAllocateMemory(m_vkdevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
   //   {

   //      throw ::exception(error_failed, "failed to allocate vertex buffer memory!");

   //   }

   //   vkBindBufferMemory(m_vkdevice, buffer, bufferMemory, 0);

   //}


   ////VkCommandBuffer device::beginSingleTimeCommands()
   ////{

   ////   VkCommandBufferAllocateInfo allocInfo{};
   ////   allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
   ////   allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
   ////   allocInfo.commandPool = m_vkcommandpool;
   ////   allocInfo.commandBufferCount = 1;

   ////   VkCommandBuffer commandBuffer;
   ////   vkAllocateCommandBuffers(m_vkdevice, &allocInfo, &commandBuffer);

   ////   VkCommandBufferBeginInfo beginInfo{};
   ////   beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
   ////   beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

   ////   vkBeginCommandBuffer(commandBuffer, &beginInfo);
   ////   return commandBuffer;

   ////}



   //void device::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
   //{
   //   VkCommandBuffer commandBuffer = beginSingleTimeCommands();

   //   VkBufferCopy copyRegion{};
   //   copyRegion.srcOffset = 0;  // Optional
   //   copyRegion.dstOffset = 0;  // Optional
   //   copyRegion.size = size;
   //   vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

   //   endSingleTimeCommands(commandBuffer);

   //}


   //void device::copyBufferToImage(
   //   VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount)
   //{

   //   VkCommandBuffer commandBuffer = beginSingleTimeCommands();

   //   VkBufferImageCopy region{};
   //   region.bufferOffset = 0;
   //   region.bufferRowLength = 0;
   //   region.bufferImageHeight = 0;

   //   region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   //   region.imageSubresource.mipLevel = 0;
   //   region.imageSubresource.baseArrayLayer = 0;
   //   region.imageSubresource.layerCount = layerCount;

   //   region.imageOffset = { 0, 0, 0 };
   //   region.imageExtent = { width, height, 1 };

   //   vkCmdCopyBufferToImage(
   //      commandBuffer,
   //      buffer,
   //      image,
   //      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
   //      1,
   //      &region);

   //   endSingleTimeCommands(commandBuffer);

   //}


   //void device::createImageWithInfo(
   //   const VkImageCreateInfo& imageInfo,
   //   VkMemoryPropertyFlags properties,
   //   VkImage& image,
   //   VkDeviceMemory& imageMemory)
   //{

   //   if (vkCreateImage(m_vkdevice, &imageInfo, nullptr, &image) != VK_SUCCESS)
   //   {

   //      throw ::exception(error_failed, "failed to create image!");

   //   }

   //   VkMemoryRequirements memRequirements;
   //   vkGetImageMemoryRequirements(m_vkdevice, image, &memRequirements);

   //   VkMemoryAllocateInfo allocInfo{};
   //   allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
   //   allocInfo.allocationSize = memRequirements.size;
   //   allocInfo.memoryTypeIndex = m_pphysicaldevice->findMemoryType(memRequirements.memoryTypeBits, properties);

   //   if (vkAllocateMemory(m_vkdevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
   //      throw ::exception(error_failed, "failed to allocate image memory!");
   //   }

   //   if (vkBindImageMemory(m_vkdevice, image, imageMemory, 0) != VK_SUCCESS) {
   //      throw ::exception(error_failed, "failed to bind image memory!");
   //   }
   //}





   //void device::submitWork(VkCommandBuffer cmdBuffer, VkQueue queue)
   //{
   //   VkSubmitInfo submitInfo = initializers::submitInfo();
   //   submitInfo.commandBufferCount = 1;
   //   submitInfo.pCommandBuffers = &cmdBuffer;
   //   //m_submitInfo.commandBufferCount = 1;
   //   //m_submitInfo.pCommandBuffers = &cmdBuffer;
   //   VkFenceCreateInfo fenceInfo = initializers::fenceCreateInfo();
   //   VkFence fence;
   //   VK_CHECK_RESULT(vkCreateFence(m_vkdevice, &fenceInfo, nullptr, &fence));
   //   VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, fence));
   //   VK_CHECK_RESULT(vkWaitForFences(m_vkdevice, 1, &fence, VK_TRUE, UINT64_MAX));
   //   vkDestroyFence(m_vkdevice, fence, nullptr);
   //}


   //void device::submitSamplingWork(VkCommandBuffer cmdBuffer, VkQueue queue)
   //{
   //   VkSubmitInfo submitInfo = initializers::submit_info();
   //   submitInfo.commandBufferCount = 1;
   //   submitInfo.pCommandBuffers = &cmdBuffer;
   //   //m_submitInfo.commandBufferCount = 1;
   //   //m_submitInfo.pCommandBuffers = &cmdBuffer;
   //   VkFenceCreateInfo fenceInfo = initializers::fence_create_info();
   //   VkFence fence;
   //   VK_CHECK_RESULT(vkCreateFence(m_vkdevice, &fenceInfo, nullptr, &fence));
   //   VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, fence));
   //   VK_CHECK_RESULT(vkWaitForFences(m_vkdevice, 1, &fence, VK_TRUE, UINT64_MAX));
   //   vkDestroyFence(m_vkdevice, fence, nullptr);
   //}


   //directx12::QueueFamilyIndices physical_device::findQueueFamilies()
   //{

   //   directx12::QueueFamilyIndices indices;

   //   uint32_t queueFamilyCount = 0;
   //   vkGetPhysicalDeviceQueueFamilyProperties(m_physicaldevice, &queueFamilyCount, nullptr);

   //   ::array<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
   //   vkGetPhysicalDeviceQueueFamilyProperties(m_physicaldevice, &queueFamilyCount, queueFamilies.data());

   //   int i = 0;
   //   for (const auto& queueFamily : queueFamilies)
   //   {
   //      if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
   //      {
   //         indices.graphicsFamily = i;
   //         indices.graphicsFamilyHasValue = true;
   //      }
   //      if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
   //      {
   //         indices.computeFamily = i;
   //         indices.computeFamilyHasValue = true;
   //      }
   //      if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
   //      {
   //         indices.transferFamily = i;
   //         indices.transferFamilyHasValue = true;
   //      }
   //      if (m_vksurfacekhr)
   //      {
   //         VkBool32 presentSupport = false;
   //         vkGetPhysicalDeviceSurfaceSupportKHR(m_physicaldevice, i, m_vksurfacekhr, &presentSupport);
   //         if (queueFamily.queueCount > 0 && presentSupport)
   //         {
   //            indices.presentFamily = i;
   //            indices.presentFamilyHasValue = true;
   //         }
   //      }
   //      //if (indices.isComplete()) {
   //        // break;
   //      //}

   //      i++;
   //   }

   //   return indices;

   //}


   SwapChainSupportDetails physical_device::querySwapChainSupport()
   {

      SwapChainSupportDetails details{};

      //if (!m_vksurfacekhr)
      //{

      //   throw ::exception(error_wrong_state, "querying swap chain support but no vksurfacekhr");

      //}

      //SwapChainSupportDetails details{};

      //vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicaldevice, m_vksurfacekhr, &details.capabilities);

      //uint32_t formatCount;
      //vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicaldevice, m_vksurfacekhr, &formatCount, nullptr);

      //if (formatCount != 0) {
      //   details.formats.resize(formatCount);
      //   vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicaldevice, m_vksurfacekhr, &formatCount, details.formats.data());
      //}

      //uint32_t presentModeCount;
      //vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicaldevice, m_vksurfacekhr, &presentModeCount, nullptr);

      //if (presentModeCount != 0) {
      //   details.presentModes.resize(presentModeCount);
      //   vkGetPhysicalDeviceSurfacePresentModesKHR(
      //      m_physicaldevice,
      //      m_vksurfacekhr,
      //      &presentModeCount,
      //      details.presentModes.data());
      //}

      return details;

   }


   void device::on_top_end_frame()
   {

      auto procedureaOnTopFrameEnd = ::transfer(m_procedureaOnTopFrameEnd);

      for (auto& procedure : procedureaOnTopFrameEnd)
      {

         procedure();

      }

   }


   void device::GetHardwareAdapter(
      IDXGIFactory1* pFactory,
      IDXGIAdapter1** ppAdapter,
      bool requestHighPerformanceAdapter)
   {
      *ppAdapter = nullptr;

      ::comptr<IDXGIAdapter1> adapter;

      ::comptr<IDXGIFactory6> factory6;
      if (SUCCEEDED(pFactory->QueryInterface(__interface_of(factory6))))
      {
         for (
            UINT adapterIndex = 0;
            SUCCEEDED(factory6->EnumAdapterByGpuPreference(
               adapterIndex,
               requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
               IID_PPV_ARGS(&adapter)));
               ++adapterIndex)
         {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
               // Don't select the Basic Render Driver adapter.
               // If you want a software adapter, pass in "/warp" on the command line.
               continue;
            }

            // Check to see whether the adapter supports Direct3D 12, but don't create the
            // actual device yet.
            if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
            {
               break;
            }
         }
      }

      if (adapter == nullptr)
      {
         for (UINT adapterIndex = 0; SUCCEEDED(pFactory->EnumAdapters1(adapterIndex, &adapter)); ++adapterIndex)
         {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
               // Don't select the Basic Render Driver adapter.
               // If you want a software adapter, pass in "/warp" on the command line.
               continue;
            }

            // Check to see whether the adapter supports Direct3D 12, but don't create the
            // actual device yet.
            if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
            {
               break;
            }
         }
      }

      *ppAdapter = adapter.detach();
   }


   void device::initialize_swap_chain(::windowing::window* pwindow)
   {

      ::cast < ::windowing_win32::window > pwin32window = pwindow;

      auto r = pwindow->get_window_rectangle();

      UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)

      {

         ::comptr<ID3D12Debug> debugController;

         if (SUCCEEDED(D3D12GetDebugInterface(__interface_of(debugController))))
         {

            debugController->EnableDebugLayer();

            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

         }

      }

#endif

      ::defer_throw_hresult(CreateDXGIFactory2(dxgiFactoryFlags, __interface_of(m_pdxgifactory4)));

      if (m_bUseWarpDevice)
      {

         ::comptr<IDXGIAdapter> warpAdapter;

         ::defer_throw_hresult(
            m_pdxgifactory4->EnumWarpAdapter(__interface_of(warpAdapter)));

         ::defer_throw_hresult(D3D12CreateDevice(
            warpAdapter,
            D3D_FEATURE_LEVEL_11_0,
            __interface_of(m_pdevice)
         ));

      }
      else
      {

         ::comptr < IDXGIAdapter1> hardwareAdapter;

         GetHardwareAdapter(m_pdxgifactory4, &hardwareAdapter);

         ::defer_throw_hresult(D3D12CreateDevice(
            hardwareAdapter,
            D3D_FEATURE_LEVEL_11_0,
            __interface_of(m_pdevice)
         ));

      }

      if (0)
      {

         ::comptr<ID3D12InfoQueue> infoQueue;

         m_pdevice.as(infoQueue);

         if (infoQueue)
         {
            infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
            infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
            infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE); // Optional
         }

      }

      DXGI_SWAP_CHAIN_DESC1 dxgiswapchaindesc1 = {};
      dxgiswapchaindesc1.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      dxgiswapchaindesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
      dxgiswapchaindesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
      dxgiswapchaindesc1.BufferCount = 2;
      dxgiswapchaindesc1.SampleDesc.Count = 1;
      dxgiswapchaindesc1.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;

      RECT rect = {};

      GetWindowRect(pwin32window->m_hwnd, &rect);

      dxgiswapchaindesc1.Width = rect.right - rect.left;
      dxgiswapchaindesc1.Height = rect.bottom - rect.top;

      D3D12_COMMAND_QUEUE_DESC queueDesc = {};
      queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
      m_pdevice->CreateCommandQueue(&queueDesc, __interface_of(m_pcommandqueue));

      ::comptr < IDXGISwapChain1 > swapchain1;

      HRESULT hrCreateSwapChainForComposition =
         m_pdxgifactory4->CreateSwapChainForComposition(
            m_pcommandqueue,
            &dxgiswapchaindesc1,
            nullptr, // Don’t restrict
            &swapchain1);

      ::defer_throw_hresult(hrCreateSwapChainForComposition);

      ::cast < ::gpu_directx12::swap_chain > pswapchain = get_swap_chain();

      HRESULT hrQueryDxgiSwapChain3 = swapchain1.as(pswapchain->m_pdxgiswapchain3);

      ::defer_throw_hresult(hrQueryDxgiSwapChain3);

      ::comptr<ID3D12DescriptorHeap> rtvHeap;

      UINT rtvDescriptorSize = 0;

      int iFrameCount = 2;

      // Describe and create an RTV descriptor heap.
      D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
      rtvHeapDesc.NumDescriptors = iFrameCount; // One per back buffer (typically 2)
      rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
      rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // Must be NONE for RTV/DSV heaps

      HRESULT hr = m_pdevice->CreateDescriptorHeap(&rtvHeapDesc, __interface_of(rtvHeap));
      ::defer_throw_hresult(hr);

      // Store the descriptor size (used for handle incrementing)
      rtvDescriptorSize = m_pdevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
      m_resourceaBackBufferTexture.set_size(iFrameCount);

      for (int i = 0; i < iFrameCount; i++)
      {

         auto& presource = m_resourceaBackBufferTexture[i];

         HRESULT hrGetBuffer = pswapchain->m_pdxgiswapchain3->GetBuffer(i, __interface_of(presource));

         ::defer_throw_hresult(hrGetBuffer);
         
         m_handleaBackBufferRenderTargetView.element_at_grow(i)
            = rtvHeap->GetCPUDescriptorHandleForHeapStart(); // RTV descriptor heap assumed created

         m_pdevice->CreateRenderTargetView(presource, nullptr, m_handleaBackBufferRenderTargetView[i]);

      }

   }


   void device::list_dred_breadcrumbs()
   {
      ::comptr<ID3D12DeviceRemovedExtendedData> pDred;
      if (FAILED(D3D12GetDebugInterface(__interface_of(pDred))))
      {
         warningf("Failed to get DRED interface.\n");
         return;
      }

      D3D12_DRED_AUTO_BREADCRUMBS_OUTPUT breadcrumbs = {};
      if (FAILED(pDred->GetAutoBreadcrumbsOutput(&breadcrumbs)))
      {
         warningf("Failed to get breadcrumbs output.\n");
         return;
      }

      const D3D12_AUTO_BREADCRUMB_NODE* pNode = breadcrumbs.pHeadAutoBreadcrumbNode;
      while (pNode)
      {
         const wchar_t* cmdListName = pNode->pCommandListDebugNameW ? pNode->pCommandListDebugNameW : L"<Unnamed>";
         warningf("Command List: %ws\n", cmdListName);

         UINT executedOpIndex = *pNode->pLastBreadcrumbValue;
         UINT count = pNode->BreadcrumbCount;

         for (UINT i = 0; i < count; ++i)
         {
            const char* opName = "Unknown";
            switch (pNode->pCommandHistory[i])
            {
            case D3D12_AUTO_BREADCRUMB_OP_DRAWINDEXEDINSTANCED: opName = "DrawIndexedInstanced"; break;
            case D3D12_AUTO_BREADCRUMB_OP_DRAWINSTANCED: opName = "DrawInstanced"; break;
            case D3D12_AUTO_BREADCRUMB_OP_EXECUTEBUNDLE: opName = "ExecuteBundle"; break;
            case D3D12_AUTO_BREADCRUMB_OP_DISPATCH: opName = "Dispatch"; break;
            case D3D12_AUTO_BREADCRUMB_OP_COPYBUFFERREGION: opName = "CopyBufferRegion"; break;
            case D3D12_AUTO_BREADCRUMB_OP_COPYTEXTUREREGION: opName = "CopyTextureRegion"; break;
            case D3D12_AUTO_BREADCRUMB_OP_RESOURCEBARRIER: opName = "ResourceBarrier"; break;
            case D3D12_AUTO_BREADCRUMB_OP_CLEARRENDERTARGETVIEW: opName = "ClearRTV"; break;
            case D3D12_AUTO_BREADCRUMB_OP_CLEARDEPTHSTENCILVIEW: opName = "ClearDSV"; break;
            case D3D12_AUTO_BREADCRUMB_OP_SETPIPELINESTATE1: opName = "SetPipelineState1"; break;
            default:
               break;
               // Add more cases if needed
            }

            if (i == executedOpIndex)
               warningf("  >> %s (Last executed)\n", opName);
            else
               warningf("     %s\n", opName);
         }

         pNode = pNode->pNext;
      }
   }


   void device::defer_throw_hresult(HRESULT hresult)
   {

      if (hresult == DXGI_ERROR_DEVICE_REMOVED
         || hresult == DXGI_ERROR_DEVICE_HUNG)
      {

         list_dred_breadcrumbs();

      }
      else
      {

         ::defer_throw_hresult(hresult);

      }

   }


   void device::get_debug_interface(UINT& dxgiFactoryFlags)
   {


      //UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
///   get_debug_interface(dxgiFactoryFlags);
   // Enable the debug layer (requires the Graphics Tools "optional feature").
   // NOTE: Enabling the debug layer after device creation will invalidate the active device.


   //::comptr<ID3D12Debug> debugController;
      if (SUCCEEDED(D3D12GetDebugInterface(__interface_of(m_pdebug))))
      {

         m_pdebug->EnableDebugLayer();

         // Enable additional debug layers.
         dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
      }



      ::comptr<ID3D12Debug1> pdebug1;
      m_pdebug.as(pdebug1);
      if (pdebug1)
      {
         ///pdebug1->SetEnableGPUBasedValidation(TRUE);
      }


      ::comptr<ID3D12DeviceRemovedExtendedDataSettings> pdredSettings;
      if (SUCCEEDED(D3D12GetDebugInterface(__interface_of(pdredSettings))))
      {
         //pdredSettings->SetAutoBreadcrumbsEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
         //pdredSettings->SetPageFaultEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
      }

#endif


   }



   void device::initialize_cpu_buffer(const ::int_size& size)
   {

      //      // This flag adds support for surfaces with a different color channel ordering
      //      // than the API default. It is required for compatibility with Direct2D.
      //      unsigned int creationFlags = D3D12_CREATE_DEVICE_BGRA_SUPPORT;
      //
      //#if defined(__DEBUG)
      //
      //      // If the project is in a debug build, enable debugging via SDK Layers with this flag.
      //      creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
      //
      //#endif
      //
      //      // This array defines the set of DirectX12 hardware feature levels this app will support.
      //      // Note the ordering should be preserved.
      //      // Don't forget to declare your application's minimum required feature level in its
      //      // description.  All applications are assumed to support 9.1 unless otherwise stated.
      //      D3D_FEATURE_LEVEL featureLevels[] =
      //      {
      //
      //         D3D_FEATURE_LEVEL_11_1,
      //         D3D_FEATURE_LEVEL_11_0,
      //         D3D_FEATURE_LEVEL_10_1,
      //         D3D_FEATURE_LEVEL_10_0,
      //         D3D_FEATURE_LEVEL_9_3,
      //         D3D_FEATURE_LEVEL_9_2,
      //         D3D_FEATURE_LEVEL_9_1
      //
      //      };
      //
      //      // Create the Direct3D 11 API device object and a corresponding context.
      //      comptr<ID3D11Device> pdevice;
      //
      //      comptr<ID3D11DeviceContext> pdevicecontext;
      //
      //      HRESULT hr = D3D11CreateDevice(
      //         nullptr,                    // Specify nullptr to use the default adapter.
      //         D3D_DRIVER_TYPE_HARDWARE,
      //         0,
      //         creationFlags,              // Set debug and Direct2D compatibility flags.
      //         featureLevels,              // List of feature levels this app can support.
      //         ARRAYSIZE(featureLevels),
      //         D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Metro style apps.
      //         &pdevice,                    // Returns the Direct3D device created.
      //         &m_featurelevel,            // Returns feature level of device created.
      //         &pdevicecontext                    // Returns the device immediate context.
      //      );



      UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
      get_debug_interface(dxgiFactoryFlags);
      //// Enable the debug layer (requires the Graphics Tools "optional feature").
      //// NOTE: Enabling the debug layer after device creation will invalidate the active device.
      //
      //
      ////::comptr<ID3D12Debug> debugController;
      //   if (SUCCEEDED(D3D12GetDebugInterface(__interface_of(debugController))))
      //   {
      //      debugController->EnableDebugLayer();

      //      // Enable additional debug layers.
      //      dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
      //   }
      //}
#endif

      ///ComPtr<IDXGIFactory4> factory;
      ::defer_throw_hresult(CreateDXGIFactory2(dxgiFactoryFlags, __interface_of(m_pdxgifactory4)));

      if (m_bUseWarpDevice)
      {
         ::comptr<IDXGIAdapter> warpAdapter;
         ::defer_throw_hresult(m_pdxgifactory4->EnumWarpAdapter(__interface_of(warpAdapter)));

         ::defer_throw_hresult(D3D12CreateDevice(
            warpAdapter,
            D3D_FEATURE_LEVEL_11_0,
            __interface_of(m_pdevice)
         ));
      }
      else
      {
         ::comptr < IDXGIAdapter1> hardwareAdapter;
         GetHardwareAdapter(m_pdxgifactory4, &hardwareAdapter);

         ::defer_throw_hresult(D3D12CreateDevice(
            hardwareAdapter,
            D3D_FEATURE_LEVEL_11_0,
            __interface_of(m_pdevice)
         ));
      }

      ::comptr<ID3D12InfoQueue> infoQueue;

      m_pdevice.as(infoQueue);

      if (infoQueue)
      {
         infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
         infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
         infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE); // Optional
      }

      D3D12_COMMAND_QUEUE_DESC queueDesc = {};
      queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
      m_pdevice->CreateCommandQueue(&queueDesc, __interface_of(m_pcommandqueue));


   }


   int device::get_type_size(::gpu::enum_type etype)
   {

      switch (etype)
      {
      case ::gpu::e_type_int: return sizeof(int);
      case ::gpu::e_type_float: return sizeof(float);
      case ::gpu::e_type_seq4: return sizeof(::glm::vec4);
      case ::gpu::e_type_mat4: return sizeof(::glm::mat4);
      case ::gpu::e_type_seq3: return sizeof(::glm::vec3);
      case ::gpu::e_type_mat3: return sizeof(::glm::mat3);
      case ::gpu::e_type_seq2: return sizeof(::glm::vec2);
      case ::gpu::e_type_mat2: return sizeof(::glm::mat2);
      default:
         throw ::exception(error_wrong_state);

      }

   }


   void device::set_mat4(void* p, const ::glm::mat4& mat4)
   {

      auto m = GLMToDX_Transposed(mat4);
      auto& mTarget = *((decltype(&m))p);
      mTarget = m;

   }

   //void device::composition_store()
   //{

   //   gpu::device::composition_store();

   //}


   //void device::composition_end()
   //{

   //   gpu::device::composition_store();

   //}


   IDXGIDevice* device::_get_dxgi_device()
   {

      if (!m_pdxgidevice)
      {

  //          {

//      ::gpu_directx12::swap_chain::initialize_gpu_swap_chain(pgpudevice, pwindow);

      //m_pgpudevice = ::gpu::swap_chain::m_pgpudevice;

      ///::cast < ::gpu_directx12::device > pdevice = m_pgpudevice;

      auto pd3d12device = m_pdevice;

      HRESULT hrD3D11On12 = D3D11On12CreateDevice(
         pd3d12device,
         D3D11_CREATE_DEVICE_BGRA_SUPPORT,
         nullptr,
         0,
         (IUnknown* const*)m_pcommandqueue.pp(),
         1,
         0,
         &m_pd3d11device,
         &m_pd3d11context,
         nullptr
      );

      ::defer_throw_hresult(hrD3D11On12);

      ::defer_throw_hresult(m_pd3d11device.as(m_pd3d11on12)); // Query interface

      ::defer_throw_hresult(m_pd3d11device.as(m_pdxgidevice));

      //::defer_throw_hresult(m_pdxgiswapchain3.as(m_pdxgiswapchain1));

      //DXGI_SWAP_CHAIN_DESC swapchaindesc1{};

      //int FrameCount = 2;

      //if (SUCCEEDED(m_pdxgiswapchain3->GetDesc(&swapchaindesc1)))
      //{

      //   FrameCount = swapchaindesc1.BufferCount;

      //}

      //m_frameIndex = m_pdxgiswapchain3->GetCurrentBackBufferIndex();

      //// Create synchronization objects and wait until assets have been uploaded to the GPU.
      //{
      //   ::defer_throw_hresult(pd3d12device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __interface_of(m_fence)));
      //   m_fenceValue = 1;

      //   // Create an event handle to use for frame synchronization.
      //   m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
      //   if (m_fenceEvent == nullptr)
      //   {
      //      ::defer_throw_hresult(HRESULT_FROM_WIN32(GetLastError()));
      //   }

      //}

      ////_defer_d3d11on12_wrapped_resources();
      //// Create descriptor heaps.

      //{
      //   // Describe and create a render target view (RTV) descriptor heap.
      //   D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
      //   rtvHeapDesc.NumDescriptors = FrameCount;
      //   rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
      //   rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
      //   ::defer_throw_hresult(pd3d12device->CreateDescriptorHeap(&rtvHeapDesc, __interface_of(m_rtvHeap)));

      //   m_rtvDescriptorSize = pd3d12device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

      //}

      //// Create frame resources.
      //{

      //   CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

      //   // Create a RTV for each frame.
      //   for (UINT n = 0; n < FrameCount; n++)
      //   {

      //      auto & prendertarget = m_renderTargets[n];

      //      ::defer_throw_hresult(
      //         m_pdxgiswapchain1->GetBuffer(
      //            n, __interface_of(prendertarget)));

      //      pd3d12device->CreateRenderTargetView(prendertarget, nullptr, rtvHandle);

      //      rtvHandle.Offset(1, m_rtvDescriptorSize);

      //   }

      //}

      //::draw2d_direct2d::swap_chain::initialize_gpu_swap_chain(pgpudevice, pwindow);



      }

      return m_pdxgidevice;

   }

} // namespace gpu_directx12




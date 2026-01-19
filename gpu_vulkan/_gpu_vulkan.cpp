// Created by camilo on 2025-05-21 04:47 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "initializers.h"
#include "bred/gpu/properties.h"
#include "bred/graphics3d/types.h"
/*
 * Assorted commonly used Vulkan helper functions
 *
 * Copyright (C) 2016-2024 by Sascha Willems - www.saschawillems.de
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

 //
 //#if !(defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK) || defined(VK_USE_PLATFORM_METAL_EXT))
 // // iOS & macOS: getAssetPath() and getShaderBasePath() implemented externally for access to Obj-C++ path utilities
 //const ::string getAssetPath()
 //{
 //   if (vks::tools::resourcePath != "") {
 //      return vks::tools::resourcePath + "/assets/";
 //   }
 //#if defined(VK_USE_PLATFORM_ANDROID_KHR)
 //   return "";
 //#elif defined(VK_EXAMPLE_ASSETS_DIR)
 //   return VK_EXAMPLE_ASSETS_DIR;
 //#else
 //   return "./../assets/";
 //#endif
 //}
 //
 //const ::string getShaderBasePath()
 //{
 //   if (vks::tools::resourcePath != "") {
 //      return vks::tools::resourcePath + "/shaders/";
 //   }
 //#if defined(VK_USE_PLATFORM_ANDROID_KHR)
 //   return "shaders/";
 //#elif defined(VK_EXAMPLE_SHADERS_DIR)
 //   return VK_EXAMPLE_SHADERS_DIR;
 //#else
 //   return "./../shaders/";
 //#endif
 //}
 //#endif
namespace vulkan
{

   bool errorModeSilent = false;
   ::string resourcePath = "";

   ::string errorString(VkResult errorCode)
   {
      switch (errorCode)
      {
#define STR(r) case VK_ ##r: return #r
         STR(NOT_READY);
         STR(TIMEOUT);
         STR(EVENT_SET);
         STR(EVENT_RESET);
         STR(INCOMPLETE);
         STR(ERROR_OUT_OF_HOST_MEMORY);
         STR(ERROR_OUT_OF_DEVICE_MEMORY);
         STR(ERROR_INITIALIZATION_FAILED);
         STR(ERROR_DEVICE_LOST);
         STR(ERROR_MEMORY_MAP_FAILED);
         STR(ERROR_LAYER_NOT_PRESENT);
         STR(ERROR_EXTENSION_NOT_PRESENT);
         STR(ERROR_FEATURE_NOT_PRESENT);
         STR(ERROR_INCOMPATIBLE_DRIVER);
         STR(ERROR_TOO_MANY_OBJECTS);
         STR(ERROR_FORMAT_NOT_SUPPORTED);
         STR(ERROR_SURFACE_LOST_KHR);
         STR(ERROR_NATIVE_WINDOW_IN_USE_KHR);
         STR(SUBOPTIMAL_KHR);
         STR(ERROR_OUT_OF_DATE_KHR);
         STR(ERROR_INCOMPATIBLE_DISPLAY_KHR);
         STR(ERROR_VALIDATION_FAILED_EXT);
         STR(ERROR_INVALID_SHADER_NV);
         STR(ERROR_INCOMPATIBLE_SHADER_BINARY_EXT);
#undef STR
      default:
         return "UNKNOWN_ERROR";
      }
   }

   ::string physicalDeviceTypeString(VkPhysicalDeviceType type)
   {
      switch (type)
      {
#define STR(r) case VK_PHYSICAL_DEVICE_TYPE_ ##r: return #r
         STR(OTHER);
         STR(INTEGRATED_GPU);
         STR(DISCRETE_GPU);
         STR(VIRTUAL_GPU);
         STR(CPU);
#undef STR
      default: return "UNKNOWN_DEVICE_TYPE";
      }
   }

   VkBool32 getSupportedDepthFormat(VkPhysicalDevice physicalDevice, VkFormat * depthFormat)
   {
      // Since all depth formats may be optional, we need to find a suitable depth format to use
      // Start with the highest precision packed format
      ::array<VkFormat> formatList = {
         VK_FORMAT_D32_SFLOAT_S8_UINT,
         VK_FORMAT_D32_SFLOAT,
         VK_FORMAT_D24_UNORM_S8_UINT,
         VK_FORMAT_D16_UNORM_S8_UINT,
         VK_FORMAT_D16_UNORM
      };

      for (auto & format : formatList)
      {
         VkFormatProperties formatProps;
         vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProps);
         if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
         {
            *depthFormat = format;
            return true;
         }
      }

      return false;
   }

   VkBool32 getSupportedDepthStencilFormat(VkPhysicalDevice physicalDevice, VkFormat * depthStencilFormat)
   {
      ::array<VkFormat> formatList = {
         VK_FORMAT_D32_SFLOAT_S8_UINT,
         VK_FORMAT_D24_UNORM_S8_UINT,
         VK_FORMAT_D16_UNORM_S8_UINT,
      };

      for (auto & format : formatList)
      {
         VkFormatProperties formatProps;
         vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProps);
         if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
         {
            *depthStencilFormat = format;
            return true;
         }
      }

      return false;
   }


   VkBool32 formatHasStencil(VkFormat format)
   {
      ::array<VkFormat> stencilFormats = {
         VK_FORMAT_S8_UINT,
         VK_FORMAT_D16_UNORM_S8_UINT,
         VK_FORMAT_D24_UNORM_S8_UINT,
         VK_FORMAT_D32_SFLOAT_S8_UINT,
      };
      return std::find(stencilFormats.begin(), stencilFormats.end(), format) != std::end(stencilFormats);
   }

   // Returns if a given format support LINEAR filtering
   VkBool32 formatIsFilterable(VkPhysicalDevice physicalDevice, VkFormat format, VkImageTiling tiling)
   {
      VkFormatProperties formatProps;
      vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProps);

      if (tiling == VK_IMAGE_TILING_OPTIMAL)
         return formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT;

      if (tiling == VK_IMAGE_TILING_LINEAR)
         return formatProps.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT;

      return false;
   }

   // Create an image memory barrier for changing the layout of
   // an image and put it into an active command buffer
   // See chapter 11.4 "Image Layout" for details

   void setImageLayout(
      VkCommandBuffer cmdbuffer,
      VkImage image,
      VkImageLayout oldImageLayout,
      VkImageLayout newImageLayout,
      VkImageSubresourceRange subresourceRange,
      VkPipelineStageFlags srcStageMask,
      VkPipelineStageFlags dstStageMask)
   {
      // Create an image barrier object
      VkImageMemoryBarrier imageMemoryBarrier = initializers::imageMemoryBarrier();
      imageMemoryBarrier.oldLayout = oldImageLayout;
      imageMemoryBarrier.newLayout = newImageLayout;
      imageMemoryBarrier.image = image;
      imageMemoryBarrier.subresourceRange = subresourceRange;

      // Source layouts (old)
      // Source access mask controls actions that have to be finished on the old layout
      // before it will be transitioned to the new layout
      switch (oldImageLayout)
      {
      case VK_IMAGE_LAYOUT_UNDEFINED:
         // Image layout is undefined (or does not matter)
         // Only valid as initial layout
         // No flags required, listed only for completeness
         imageMemoryBarrier.srcAccessMask = 0;
         break;

      case VK_IMAGE_LAYOUT_PREINITIALIZED:
         // Image is preinitialized
         // Only valid as initial layout for linear images, preserves memory contents
         // Make sure host writes have been finished
         imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
         break;

      case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
         // Image is a color attachment
         // Make sure any writes to the color buffer have been finished
         imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
         break;

      case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
         // Image is a depth/stencil attachment
         // Make sure any writes to the depth/stencil buffer have been finished
         imageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
         break;

      case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
         // Image is a transfer source
         // Make sure any reads from the image have been finished
         imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
         break;

      case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
         // Image is a transfer destination
         // Make sure any writes to the image have been finished
         imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
         break;

      case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
         // Image is read by a shader
         // Make sure any shader reads from the image have been finished
         imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
         break;
      default:
         // Other source layouts aren't handled (yet)
         break;
      }

      // Target layouts (new)
      // Destination access mask controls the dependency for the new image layout
      switch (newImageLayout)
      {
      case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
         // Image will be used as a transfer destination
         // Make sure any writes to the image have been finished
         imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
         break;

      case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
         // Image will be used as a transfer source
         // Make sure any reads from the image have been finished
         imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
         break;

      case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
         // Image will be used as a color attachment
         // Make sure any writes to the color buffer have been finished
         imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
         break;

      case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
         // Image layout will be used as a depth/stencil attachment
         // Make sure any writes to depth/stencil buffer have been finished
         imageMemoryBarrier.dstAccessMask = imageMemoryBarrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
         break;

      case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
         // Image will be read in a shader (sampler, input attachment)
         // Make sure any writes to the image have been finished
         if (imageMemoryBarrier.srcAccessMask == 0)
         {
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
         }
         imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
         break;
      default:
         // Other source layouts aren't handled (yet)
         break;
      }

      // Put barrier inside setup command buffer
      vkCmdPipelineBarrier(
         cmdbuffer,
         srcStageMask,
         dstStageMask,
         0,
         0, nullptr,
         0, nullptr,
         1, &imageMemoryBarrier);
   }

   // Fixed sub resource on first mip level and layer
   void setImageLayout(
      VkCommandBuffer cmdbuffer,
      VkImage image,
      VkImageAspectFlags aspectMask,
      VkImageLayout oldImageLayout,
      VkImageLayout newImageLayout,
      VkPipelineStageFlags srcStageMask,
      VkPipelineStageFlags dstStageMask)
   {
      VkImageSubresourceRange subresourceRange = {};
      subresourceRange.aspectMask = aspectMask;
      subresourceRange.baseMipLevel = 0;
      subresourceRange.levelCount = 1;
      subresourceRange.layerCount = 1;
      setImageLayout(cmdbuffer, image, oldImageLayout, newImageLayout, subresourceRange, srcStageMask, dstStageMask);
   }

   void insertImageMemoryBarrier(
      VkCommandBuffer cmdbuffer,
      VkImage image,
      VkAccessFlags srcAccessMask,
      VkAccessFlags dstAccessMask,
      VkImageLayout oldImageLayout,
      VkImageLayout newImageLayout,
      VkPipelineStageFlags srcStageMask,
      VkPipelineStageFlags dstStageMask,
      VkImageSubresourceRange subresourceRange)
   {
      VkImageMemoryBarrier imageMemoryBarrier = initializers::imageMemoryBarrier();
      imageMemoryBarrier.srcAccessMask = srcAccessMask;
      imageMemoryBarrier.dstAccessMask = dstAccessMask;
      imageMemoryBarrier.oldLayout = oldImageLayout;
      imageMemoryBarrier.newLayout = newImageLayout;
      imageMemoryBarrier.image = image;
      imageMemoryBarrier.subresourceRange = subresourceRange;

      vkCmdPipelineBarrier(
         cmdbuffer,
         srcStageMask,
         dstStageMask,
         0,
         0, nullptr,
         0, nullptr,
         1, &imageMemoryBarrier);
   }

   void exitFatal(const ::string & message, int32_t exitCode)
   {
      throw ::exception(error_failed, message + " Exit Code: " + as_string(exitCode));
      //#if defined(_WIN32)
      //         if (!errorModeSilent) {
      //            MessageBox(NULL, message.c_str(), NULL, MB_OK | MB_ICONERROR);
      //         }
      //#elif defined(__ANDROID__)
      //         LOGE("Fatal error: %s", message.c_str());
      //         vks::android::showAlert(message.c_str());
      //#endif
      //         std::cerr << message << "\n";
      //#if !defined(__ANDROID__)
      //         exit(exitCode);
      //#endif
   }

   void exitFatal(const ::string & message, VkResult resultCode)
   {
      exitFatal(message, (int32_t)resultCode);
   }

   //#if defined(__ANDROID__)
   //      // Android shaders are stored as assets in the apk
   //      // So they need to be loaded via the asset manager
   //      VkShaderModule loadShader(AAssetManager * assetManager, const char * fileName, VkDevice device)
   //      {
   //         // Load shader from compressed asset
   //         AAsset * asset = AAssetManager_open(assetManager, fileName, AASSET_MODE_STREAMING);
   //         assert(asset);
   //         size_t size = AAsset_getLength(asset);
   //         assert(size > 0);
   //
   //         char * shaderCode = new char[size];
   //         AAsset_read(asset, shaderCode, size);
   //         AAsset_close(asset);
   //
   //         VkShaderModule shaderModule;
   //         VkShaderModuleCreateInfo moduleCreateInfo;
   //         moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
   //         moduleCreateInfo.pNext = NULL;
   //         moduleCreateInfo.codeSize = size;
   //         moduleCreateInfo.pCode = (uint32_t *)shaderCode;
   //         moduleCreateInfo.flags = 0;
   //
   //         VkCheckResult(vkCreateShaderModule(device, &moduleCreateInfo, NULL, &shaderModule));
   //
   //         delete[] shaderCode;
   //
   //         return shaderModule;
   //      }
   //#else
   //      VkShaderModule loadShader(const char * fileName, VkDevice device)
   //      {
   //         std::ifstream is(fileName, std::ios::binary | std::ios::in | std::ios::ate);
   //
   //         if (is.is_open())
   //         {
   //            size_t size = is.tellg();
   //            is.seekg(0, std::ios::beg);
   //            char * shaderCode = new char[size];
   //            is.read(shaderCode, size);
   //            is.close();
   //
   //            assert(size > 0);
   //
   //            VkShaderModule shaderModule;
   //            VkShaderModuleCreateInfo moduleCreateInfo{};
   //            moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
   //            moduleCreateInfo.codeSize = size;
   //            moduleCreateInfo.pCode = (uint32_t *)shaderCode;
   //
   //            VkCheckResult(vkCreateShaderModule(device, &moduleCreateInfo, NULL, &shaderModule));
   //
   //            delete[] shaderCode;
   //
   //            return shaderModule;
   //         }
   //         else
   //         {
   //            std::cerr << "Error: Could not open shader file \"" << fileName << "\"" << "\n";
   //            return VK_NULL_HANDLE;
   //         }
   //      }
   //#endif

   bool fileExists(const ::string & filename)
   {
      return ::file_exists(filename);
   }

   uint32_t alignedSize(uint32_t value, uint32_t alignment)
   {
      return (value + alignment - 1) & ~(alignment - 1);
   }

   size_t alignedSize(size_t value, size_t alignment)
   {
      return (value + alignment - 1) & ~(alignment - 1);
   }


   VkDeviceSize alignedVkSize(VkDeviceSize value, VkDeviceSize alignment)
   {
      return (value + alignment - 1) & ~(alignment - 1);
   }


   //::array<VkVertexInputBindingDescription> _001GetVertexBindingDescriptions()
   //{

   //      ::array<VkVertexInputBindingDescription> bindingDescriptions(1, VkVertexInputBindingDescription{});

   //      bindingDescriptions[0].binding = 0;
   //      bindingDescriptions[0].stride = sizeof(::graphics3d::Vertex);
   //      bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
   //      return bindingDescriptions;
   //  
   //}


   //::array<VkVertexInputBindingDescription> _001GetVertexBindingDescriptions(const ::gpu::property* pproperties)
   //{

   //   ::array<VkVertexInputBindingDescription> bindingDescriptions(1, VkVertexInputBindingDescription{});

   //   bindingDescriptions[0].binding = 0;
   //   bindingDescriptions[0].stride = pproperties->get_size();
   //   bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
   //   return bindingDescriptions;

   //}


   VkFormat get_type_vk_format(::gpu::enum_type etype)
   {

      switch (etype)
      {
      case ::gpu::e_type_seq4: return VK_FORMAT_R32G32B32A32_SFLOAT;
      case ::gpu::e_type_seq3: return VK_FORMAT_R32G32B32_SFLOAT;
      case ::gpu::e_type_seq2: return VK_FORMAT_R32G32_SFLOAT;
      default:
         throw ::exception(error_bad_argument, "get_type_vk_format: unknown ::gpu::enum_type " + ::as_string((int)etype));

      }

   }


   CLASS_DECL_GPU_VULKAN VkPrimitiveTopology as_vk_topology(::gpu::enum_topology etopology)
   {
      switch (etopology)
      {
      case ::gpu::e_topology_triangle_list:
         return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
      case ::gpu::e_topology_triangle_strip:
         return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
      case ::gpu::e_topology_line_list:
         return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
      default:
         throw ::exception(error_unexpected);
      }
   }
   
   
   //::array<VkVertexInputAttributeDescription> _001GetVertexAttributeDescriptions() 
   //{
   //
   //   ::array<VkVertexInputAttributeDescription> attributeDescriptions{};
   //   
   //   attributeDescriptions.add({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(::gpu::Vertex, position) });
   //   attributeDescriptions.add({ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(::gpu::Vertex, color) });
   //   attributeDescriptions.add({ 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(::gpu::Vertex, normal) });
   //   attributeDescriptions.add({ 3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(::gpu::Vertex, uv) });

   //   return attributeDescriptions;

   //}


   //::array<VkVertexInputAttributeDescription> _001GetVertexAttributeDescriptions(const ::gpu::property* pproperties) 
   //{

   //   ::array<VkVertexInputAttributeDescription> attributeDescriptions{};

   //   //::array<VkVertexInputAttributeDescription> attributeDescriptions{};

   //   uint32_t i = 0;
   //   uint32_t pos = 0;

   //   for (auto p = pproperties; ::is_set(p->m_pszName); p++, i++, pos += ::gpu::get_type_size(p->m_etype))
   //   {

   //      attributeDescriptions.add({ i, 0, get_type_vk_format(p->m_etype), pos});

   //   }

   //   return attributeDescriptions;

   //}

       struct Vertex2
   {
      floating_sequence3 position{};
      floating_sequence3 color{};
      floating_sequence3 normal{};
      floating_sequence2 uv{};

   };
   ::array_base<VkVertexInputBindingDescription> getBindingDescriptions2()
   {
      return {VkVertexInputBindingDescription{0, sizeof(Vertex2), VK_VERTEX_INPUT_RATE_VERTEX}};
   }

   ::array_base<VkVertexInputAttributeDescription> getAttributeDescriptions2()
   {
      return {
         {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex2, position)},
         {1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex2, color)},
         {2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex2, normal)}
         //{ 3, 0, VK_FORMAT_R32G32_SFLOAT,    offsetof(Vertex, uv) }
      };
   }

   CLASS_DECL_GPU_VULKAN void defaultPipelineConfigInfo2(pipeline_configuration &configInfo)
   {

      configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
      configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
      configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;


      configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
      configInfo.viewportInfo.viewportCount = 1;
      configInfo.viewportInfo.pViewports = nullptr;
      configInfo.viewportInfo.scissorCount = 1;
      configInfo.viewportInfo.pScissors = nullptr;

      configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
      configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
      configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
      configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
      configInfo.rasterizationInfo.lineWidth = 1.0f;
      configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
      configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
      configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
      configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f; // Optional
      configInfo.rasterizationInfo.depthBiasClamp = 0.0f; // Optional
      configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f; // Optional

      configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
      configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
      configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
      configInfo.multisampleInfo.minSampleShading = 1.0f; // Optional
      configInfo.multisampleInfo.pSampleMask = nullptr; // Optional
      configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE; // Optional
      configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE; // Optional

      configInfo.colorBlendAttachments.ø(0).colorWriteMask =
         VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
      configInfo.colorBlendAttachments[0].blendEnable = VK_TRUE;
      configInfo.colorBlendAttachments[0].srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
      configInfo.colorBlendAttachments[0].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
      configInfo.colorBlendAttachments[0].colorBlendOp = VK_BLEND_OP_ADD; // Optional
      configInfo.colorBlendAttachments[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
      configInfo.colorBlendAttachments[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
      configInfo.colorBlendAttachments[0].alphaBlendOp = VK_BLEND_OP_ADD; // Optional

      configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
      configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
      configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
      configInfo.colorBlendInfo.attachmentCount = 1;
      configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachments[0];
      configInfo.colorBlendInfo.blendConstants[0] = 0.0f; // Optional
      configInfo.colorBlendInfo.blendConstants[1] = 0.0f; // Optional
      configInfo.colorBlendInfo.blendConstants[2] = 0.0f; // Optional
      configInfo.colorBlendInfo.blendConstants[3] = 0.0f; // Optional

      configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
      configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
      configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
      configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
      configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
      configInfo.depthStencilInfo.minDepthBounds = 0.0f; // Optional
      configInfo.depthStencilInfo.maxDepthBounds = 1.0f; // Optional
      configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
      configInfo.depthStencilInfo.front = {}; // Optional
      configInfo.depthStencilInfo.back = {}; // Optional

      configInfo.dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
      configInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
      configInfo.dynamicStateInfo.pDynamicStates = configInfo.dynamicStateEnables.data();
      configInfo.dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(configInfo.dynamicStateEnables.size());
      configInfo.dynamicStateInfo.flags = 0;


      configInfo.bindingDescriptions.clear();
      configInfo.attributeDescriptions.clear();

      configInfo.bindingDescriptions = getBindingDescriptions2();
      configInfo.attributeDescriptions = getAttributeDescriptions2();
   }
   


} // namespace vulkan



// Macro to check and display Vulkan return results
#if defined(__ANDROID__)
void _vk_check_result(VkResult f, const char *pszFile, int iLine)                                                                              \
   {                                                                                                                
      VkResult res = (f);                                                                                           
      if (res != VK_SUCCESS)                                                                                        
      {                                                                                                             
         LOGE("Fatal : VkResult is \" %s \" in %s at line %d", vks::tools::errorString(res).c_str(), pszFile,       
              iLine);                                                                                            
         assert(res == VK_SUCCESS);                                                                                 
      }                                                                                                             
   }
#else

void _vk_check_result(VkResult  f, const char *pszFile, int iLine)
{
   VkResult res = (f);
   if (res != VK_SUCCESS)
   {
      ::information() << "Fatal : VkResult is \"" << ::vulkan::errorString(res) << "\" in " << pszFile << " at line "
                      << iLine;
      ASSERT(res == VK_SUCCESS);
   }							

}



#endif
// Created by camilo on 2023-04-27 17:06 <3ThomasBorregaardSorensen!!
#pragma once


#include "windowing_win32/_.h"


#undef USUAL_OPERATING_SYSTEM_SUPPRESSIONS
#include "acme/_operating_system.h"
#include "acme_windows_common/comptr.h"
#include "acme_windows_common/hresult_exception.h"

//#include <winapifamily.h>
//#include <windows.h>
//#include <shlwapi.h>
//#include <wrl/client.h>
#include <DXGI1_6.h>
#include <D3D12.h>
//#include <D3DX12.h>
//#include <D3D12_1.h>
//#include <Dxgi1_3.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <Initguid.h>
//#include <DXGIDebug.h>

#include "_d3dx12.h"
// Windows Desktop Thread Local Storage for Draw2d Direct2d plugin



#if defined(_directx12_project)
#define CLASS_DECL_DIRECTX12  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_DIRECTX12  CLASS_DECL_IMPORT
#endif


namespace directx12
{


   class directx12;


#if defined(_DEBUG)


   CLASS_DECL_GPU_DIRECTX12 void defer_dxgi_debug_initialize();
   CLASS_DECL_GPU_DIRECTX12 void dxgi_debug_terminate();


#endif


   //CLASS_DECL_DIRECTX12 void defer_initialize(::particle * pparticle);
   //CLASS_DECL_DIRECTX12 void finalize();


} // namespace directx12


#if defined(_DEBUG)


CLASS_DECL_DIRECTX12 void directx12_debug();


#endif







namespace directx12
{



   inline UINT Align256(UINT size)
   {
      return (size + 255) & ~255;
   }

//
//
   /** @brief Returns an error code as a string */
   CLASS_DECL_GPU_DIRECTX12 ::string errorString(HRESULT errorCode);
//
//   /** @brief Returns the device type as a string */
//   CLASS_DECL_GPU_DIRECTX12 ::string physicalDeviceTypeString(VkPhysicalDeviceType type);
//
//   // Selected a suitable supported depth format starting with 32 bit down to 16 bit
//   // Returns false if none of the depth formats in the list is supported by the device
//   CLASS_DECL_GPU_DIRECTX12 VkBool32 getSupportedDepthFormat(VkPhysicalDevice physicalDevice, VkFormat * depthFormat);
//   // Same as getSupportedDepthFormat but will only select formats that also have stencil
//   CLASS_DECL_GPU_DIRECTX12 VkBool32 getSupportedDepthStencilFormat(VkPhysicalDevice physicalDevice, VkFormat * depthStencilFormat);
//
//   // Returns true a given format support LINEAR filtering
//   CLASS_DECL_GPU_DIRECTX12 VkBool32 formatIsFilterable(VkPhysicalDevice physicalDevice, VkFormat format, VkImageTiling tiling);
//   // Returns true if a given format has a stencil part
//   CLASS_DECL_GPU_DIRECTX12 VkBool32 formatHasStencil(VkFormat format);
//
//   // Put an image memory barrier for setting an image layout on the sub resource into the given command buffer
//   CLASS_DECL_GPU_DIRECTX12 void setImageLayout(
//      VkCommandBuffer cmdbuffer,
//      VkImage image,
//      VkImageLayout oldImageLayout,
//      VkImageLayout newImageLayout,
//      VkImageSubresourceRange subresourceRange,
//      VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
//      VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);
//   // Uses a fixed sub resource layout with first mip level and layer
//   CLASS_DECL_GPU_DIRECTX12 void setImageLayout(
//      VkCommandBuffer cmdbuffer,
//      VkImage image,
//      VkImageAspectFlags aspectMask,
//      VkImageLayout oldImageLayout,
//      VkImageLayout newImageLayout,
//      VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
//      VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);
//
//   /** @brief Insert an image memory barrier into the command buffer */
//   CLASS_DECL_GPU_DIRECTX12 void insertImageMemoryBarrier(
//      VkCommandBuffer cmdbuffer,
//      VkImage image,
//      VkAccessFlags srcAccessMask,
//      VkAccessFlags dstAccessMask,
//      VkImageLayout oldImageLayout,
//      VkImageLayout newImageLayout,
//      VkPipelineStageFlags srcStageMask,
//      VkPipelineStageFlags dstStageMask,
//      VkImageSubresourceRange subresourceRange);
//
//   // Display error message and exit on fatal error
//   CLASS_DECL_GPU_DIRECTX12 void exitFatal(const ::string & message, int32_t exitCode);
//   CLASS_DECL_GPU_DIRECTX12 void exitFatal(const ::string & message, HRESULT resultCode);
//
////   // Load a SPIR-V shader (binary)
////#if defined(__ANDROID__)
////   VkShaderModule loadShader(AAssetManager * assetManager, const char * fileName, VkDevice device);
////#else
////   VkShaderModule loadShader(const char * fileName, VkDevice device);
////#endif
//
//   /** @brief Checks if a file exists */
//   CLASS_DECL_GPU_DIRECTX12 bool fileExists(const ::string & filename);
//
//   CLASS_DECL_GPU_DIRECTX12 uint32_t alignedSize(uint32_t value, uint32_t alignment);
//   CLASS_DECL_GPU_DIRECTX12 VkDeviceSize alignedVkSize(VkDeviceSize value, VkDeviceSize alignment);
//
//
//   //CLASS_DECL_GPU_DIRECTX12 ::array<VkVertexInputBindingDescription> _001GetVertexBindingDescriptions(const ::gpu::property* pproperties);
//   //CLASS_DECL_GPU_DIRECTX12 ::array<VkVertexInputAttributeDescription> _001GetVertexAttributeDescriptions(const ::gpu::property* pproperties);
//   CLASS_DECL_GPU_DIRECTX12::array<VkVertexInputBindingDescription> _001GetVertexBindingDescriptions();
//   CLASS_DECL_GPU_DIRECTX12::array<VkVertexInputAttributeDescription> _001GetVertexAttributeDescriptions();


} // namespace directx12




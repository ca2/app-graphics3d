// Merge gpu_opengl/diffuse_irradiance_map.h with gpu_vulkan::context::generateIrradianceMap by
// camilo on 2025-09-29 18:21 <3ThomasBorregaardSorensen!!
#include "platform.h"
#include "diffuse_irradiance_map.h"
#include "acme/prototype/mathematics/mathematics.h"
#include "bred/graphics3d/_functions.h"
#include "bred/graphics3d/engine.h"
#include "bred/graphics3d/immersion_layer.h"
#include "bred/graphics3d/scene_base.h"
#include "bred/graphics3d/skybox.h"
#include "bred/gltf/vertex.h"
//#include "gpu/ibl/cubemap_framebuffer.h"
#include "app-graphics3d/gpu_vulkan/_gpu_vulkan.h"
#include "app-graphics3d/gpu_vulkan/command_buffer.h"
#include "app-graphics3d/gpu_vulkan/context.h"
#include "app-graphics3d/gpu_vulkan/device.h"
#include "app-graphics3d/gpu_vulkan/gltf/model.h"
#include "app-graphics3d/gpu_vulkan/pipeline.h"
#include "app-graphics3d/gpu_vulkan/queue.h"
#include "app-graphics3d/gpu_vulkan/physical_device.h"
#include "app-graphics3d/gpu_vulkan/texture.h"
#include "app-graphics3d/gpu_vulkan/vk_init.h"
#include <chrono>


namespace gpu_vulkan
{


   namespace ibl
   {


      diffuse_irradiance_map::diffuse_irradiance_map()
      {


      }


      diffuse_irradiance_map::~diffuse_irradiance_map()
      {



      }


      ::block diffuse_irradiance_map::embedded_diffuse_irradiance_vert()
      {
         static unsigned int g_uaDiffuseIrradianceVertexShader[] = {
#include "ibl/shader/diffuseirradiance.vert.spv.inl"
         };
         return as_memory_block(g_uaDiffuseIrradianceVertexShader);

      }


      ::block diffuse_irradiance_map::embedded_diffuse_irradiance_frag()
      {

         static unsigned int g_uaDiffuseIrradianceFragmentShader[] = {
#include "ibl/shader/diffuseirradiance.frag.spv.inl"
         };
         return as_memory_block(g_uaDiffuseIrradianceFragmentShader);
      }

      
      void diffuse_irradiance_map::initialize_diffuse_irradiance_map(::graphics3d::scene_base * pscenebase)
      {

         ::gpu::ibl::diffuse_irradiance_map::initialize_diffuse_irradiance_map(pscenebase);

      }


      void diffuse_irradiance_map::computeIrradianceMap(::gpu::command_buffer *pgpucommandbuffer)
      {

         ::gpu::ibl::diffuse_irradiance_map::computeIrradianceMap(pgpucommandbuffer);

         //::cast<::gpu_vulkan::context> pgpucontext = m_pgpucontext;

         //::cast<::gpu_vulkan::device> pgpudevice = pgpucontext->m_pgpudevice;

         //::pointer<::gpu::texture> pgputextureIrradianceCubeNew;

         //constructø(pgputextureIrradianceCubeNew);

         //::cast<::gpu_vulkan::texture> pgputextureIrradianceCube = pgputextureIrradianceCubeNew;

         //::cast<::graphics3d::skybox> pskybox = m_pscene->current_skybox();

         //::cast<::gpu_vulkan::texture> pgputextureSkybox = pskybox->m_ptexture;

         //if (!pgputextureIrradianceCube || !pgputextureSkybox)
         //{

         //   error("bad argument");

         //   throw ::exception(error_failed);

         //}

         //auto pscene = pgpucontext->m_pengine->m_pimmersionlayer->m_pscene;

         //auto prenderableSkybox = pskybox->m_prenderable;

         //pgputextureIrradianceCube->m_vksampler3 = pgpucontext->_001VkSampler();

         //// m_vkqueueTransfer3 = m_vkqueueGraphics;

         //auto tStart = std::chrono::high_resolution_clock::now();

         ////::cast<device> pdevice = m_pgpudevice;

         //auto pphysicaldevice = pgpudevice->m_pphysicaldevice;

         //const VkFormat format = VK_FORMAT_R32G32B32A32_SFLOAT;
         //const int32_t dim = m_udiffuse_irradiance_mapWidth;
         ////const uint32_t numMips = static_cast<uint32_t>(floor(log2(dim))) + 1;
         //const uint32_t numMips = 1;

         //// create irradiance cubemap (same as before)
         //VkImageCreateInfo imageCI = vkinit::imageCreateInfo();
         //imageCI.imageType = VK_IMAGE_TYPE_2D;
         //imageCI.format = format;
         //imageCI.extent.width = dim;
         //imageCI.extent.height = dim;
         //imageCI.extent.depth = 1;
         //imageCI.mipLevels = numMips;
         //imageCI.arrayLayers = 6;
         //imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
         //imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
         //imageCI.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
         //imageCI.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
         //VkCheckResult(vkCreateImage(pgpucontext->logicalDevice(), &imageCI, nullptr, &pgputextureIrradianceCube->m_vkimage));

         //VkMemoryRequirements memReqs;
         //vkGetImageMemoryRequirements(pgpucontext->logicalDevice(), pgputextureIrradianceCube->m_vkimage, &memReqs);
         //VkMemoryAllocateInfo memAlloc = vkinit::memoryAllocateInfo();
         //memAlloc.allocationSize = memReqs.size;
         //memAlloc.memoryTypeIndex =
         //   pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
         //VkCheckResult(
         //   vkAllocateMemory(pgpucontext->logicalDevice(), &memAlloc, nullptr, &pgputextureIrradianceCube->m_vkdevicememory));
         //VkCheckResult(
         //   vkBindImageMemory(pgpucontext->logicalDevice(), pgputextureIrradianceCube->m_vkimage, pgputextureIrradianceCube->m_vkdevicememory, 0));

         //// view & sampler
         //VkImageViewCreateInfo viewCI = vkinit::imageViewCreateInfo();
         //viewCI.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
         //viewCI.format = format;
         //viewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         //viewCI.subresourceRange.baseMipLevel = 0;
         //viewCI.subresourceRange.levelCount = numMips;
         //viewCI.subresourceRange.baseArrayLayer = 0;
         //viewCI.subresourceRange.layerCount = 6;
         //viewCI.image = pgputextureIrradianceCube->m_vkimage;
         //VkCheckResult(vkCreateImageView(pgpucontext->logicalDevice(), &viewCI, nullptr, &pgputextureIrradianceCube->m_vkimageview));

         //VkSamplerCreateInfo samplerCI = vkinit::samplerCreateInfo();
         //samplerCI.magFilter = VK_FILTER_LINEAR;
         //samplerCI.minFilter = VK_FILTER_LINEAR;
         //samplerCI.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
         //samplerCI.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
         //samplerCI.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
         //samplerCI.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
         //samplerCI.minLod = 0.0f;
         //samplerCI.maxLod = static_cast<float>(numMips);
         //samplerCI.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
         //VkCheckResult(
         //   vkCreateSampler(pgpucontext->logicalDevice(), &samplerCI, nullptr, &pgputextureIrradianceCube->m_vksamplerDedicated));
         //auto &desc3 = pgputextureIrradianceCube->m_descriptor3;
         //desc3.imageView = pgputextureIrradianceCube->m_vkimageview;
         //desc3.sampler = pgputextureIrradianceCube->m_vksamplerDedicated;
         //desc3.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
         //// pgputextureIrradianceCube->m_pDevice = m_pgpudevice;

         //// --- create offscreen renderpass/framebuffer (unchanged) ---
         //VkAttachmentDescription attDesc = {};
         //attDesc.format = format;
         //attDesc.samples = VK_SAMPLE_COUNT_1_BIT;
         //attDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
         //attDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
         //attDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
         //attDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
         //attDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
         //attDesc.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
         //VkAttachmentReference colorReference = {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

         //VkSubpassDescription subpassDescription = {};
         //subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
         //subpassDescription.colorAttachmentCount = 1;
         //subpassDescription.pColorAttachments = &colorReference;

         //::block_array<VkSubpassDependency, 2> dependencies;
         //dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
         //dependencies[0].dstSubpass = 0;
         //dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
         //dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
         //dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
         //dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
         //dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
         //dependencies[1].srcSubpass = 0;
         //dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
         //dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
         //dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
         //dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
         //dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
         //dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

         //VkRenderPassCreateInfo renderPassCI = vkinit::renderPassCreateInfo();
         //renderPassCI.attachmentCount = 1;
         //renderPassCI.pAttachments = &attDesc;
         //renderPassCI.subpassCount = 1;
         //renderPassCI.pSubpasses = &subpassDescription;
         //renderPassCI.dependencyCount = static_cast<uint32_t>(dependencies.size());
         //renderPassCI.pDependencies = dependencies.data();
         //VkRenderPass renderpass;
         //VkCheckResult(vkCreateRenderPass(pgpucontext->logicalDevice(), &renderPassCI, nullptr, &renderpass));

         //// offscreen color image (1 mip, reused for all mips/faces)
         //struct
         //{
         //   VkImage image;
         //   VkImageView view;
         //   VkDeviceMemory memory;
         //   VkFramebuffer framebuffer;
         //} offscreen;

         //{
         //   VkImageCreateInfo imageCreateInfo = vkinit::imageCreateInfo();
         //   imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
         //   imageCreateInfo.format = format;
         //   imageCreateInfo.extent.width = dim;
         //   imageCreateInfo.extent.height = dim;
         //   imageCreateInfo.extent.depth = 1;
         //   imageCreateInfo.mipLevels = 1;
         //   imageCreateInfo.arrayLayers = 1;
         //   imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
         //   imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
         //   imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
         //   imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
         //   imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
         //   VkCheckResult(vkCreateImage(pgpucontext->logicalDevice(), &imageCreateInfo, nullptr, &offscreen.image));

         //   vkGetImageMemoryRequirements(pgpucontext->logicalDevice(), offscreen.image, &memReqs);
         //   memAlloc.allocationSize = memReqs.size;
         //   memAlloc.memoryTypeIndex =
         //      pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
         //   VkCheckResult(vkAllocateMemory(pgpucontext->logicalDevice(), &memAlloc, nullptr, &offscreen.memory));
         //   VkCheckResult(vkBindImageMemory(pgpucontext->logicalDevice(), offscreen.image, offscreen.memory, 0));

         //   VkImageViewCreateInfo colorImageView = vkinit::imageViewCreateInfo();
         //   colorImageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
         //   colorImageView.format = format;
         //   colorImageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         //   colorImageView.subresourceRange.baseMipLevel = 0;
         //   colorImageView.subresourceRange.levelCount = 1;
         //   colorImageView.subresourceRange.baseArrayLayer = 0;
         //   colorImageView.subresourceRange.layerCount = 1;
         //   colorImageView.image = offscreen.image;
         //   VkCheckResult(vkCreateImageView(pgpucontext->logicalDevice(), &colorImageView, nullptr, &offscreen.view));

         //   VkFramebufferCreateInfo fbufCreateInfo = vkinit::framebufferCreateInfo();
         //   fbufCreateInfo.renderPass = renderpass;
         //   fbufCreateInfo.attachmentCount = 1;
         //   fbufCreateInfo.pAttachments = &offscreen.view;
         //   fbufCreateInfo.width = dim;
         //   fbufCreateInfo.height = dim;
         //   fbufCreateInfo.layers = 1;
         //   VkCheckResult(
         //      vkCreateFramebuffer(pgpucontext->logicalDevice(), &fbufCreateInfo, nullptr, &offscreen.framebuffer));

         //   // VkCommandBuffer layoutCmd = pgpucontext->beginSingleTimeCommands((VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
         //   ::pointer<::gpu_vulkan::command_buffer> pcommandbuffer = pgpucontext->beginSingleTimeCommands(pgpucontext->m_pgpudevice->transfer_queue());
         //   vulkan::setImageLayout(pcommandbuffer->m_vkcommandbuffer, offscreen.image, VK_IMAGE_ASPECT_COLOR_BIT,
         //                          VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
         //   // this->flushCommandBuffer(layoutCmd, m_vkqueueTransfer3, true);
         //   pgpucontext->endSingleTimeCommands(pcommandbuffer);
         //}

         //// Descriptor layout/pool/set (same as before)
         //aaaVkDescriptorSetLayout descriptorsetlayout;
         //::array_base<VkDescriptorSetLayoutBinding> setLayoutBindings = {
         //   vkinit::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT,
         //                                      0),
         //};
         //VkDescriptorSetLayoutCreateInfo descriptorsetlayoutCI =
         //   vkinit::descriptorSetLayoutCreateInfo(setLayoutBindings);
         //VkCheckResult(
         //   vkCreateDescriptorSetLayout(pgpucontext->logicalDevice(), &descriptorsetlayoutCI, nullptr, &descriptorsetlayout));

         //::array_base<VkDescriptorPoolSize> poolSizes = {
         //   vkinit::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1)};
         //VkDescriptorPoolCreateInfo descriptorPoolCI = vkinit::descriptorPoolCreateInfo(poolSizes, 2);
         //VkDescriptorPool descriptorpool;
         //VkCheckResult(vkCreateDescriptorPool(pgpucontext->logicalDevice(), &descriptorPoolCI, nullptr, &descriptorpool));

         //VkDescriptorSet descriptorset;
         //VkDescriptorSetAllocateInfo allocInfo =
         //   vkinit::descriptorSetAllocateInfo(descriptorpool, &descriptorsetlayout, 1);
         //VkCheckResult(vkAllocateDescriptorSets(pgpucontext->logicalDevice(), &allocInfo, &descriptorset));
         //VkWriteDescriptorSet writeDescriptorSet = vkinit::writeDescriptorSet(
         //   descriptorset, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &pgputextureSkybox->m_descriptor3);
         //vkUpdateDescriptorSets(pgpucontext->logicalDevice(), 1, &writeDescriptorSet, 0, nullptr);

         //// Push block
         //struct PushBlock
         //{
         //   floating_matrix4 mvp;
         //   float deltaPhi;
         //   float deltaTheta;
         //} pushBlock;


         //pushBlock.deltaPhi = (2.0f * float(this->mathematics()->π())) / 180.0f;
         //pushBlock.deltaTheta = (0.5f * float(this->mathematics()->π())) / 64.0f;


         //// Pipeline config — IMPORTANT: provide vertex input descriptions to match shader (location 0)
         //::vulkan::pipeline_configuration pipelineconfiguration{};
         //::vulkan::defaultPipelineConfigInfo2(pipelineconfiguration);

         //// Vertex input: location 0 is a vec3 position (adjust if your skybox vertex layout differs)
         //VkVertexInputBindingDescription bindingDesc{};
         //bindingDesc.binding = 0;
         //bindingDesc.stride = sizeof(::gpu::gltf::vertex);
         //bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
         //VkVertexInputAttributeDescription attrDesc{};
         //attrDesc.binding = 0;
         //attrDesc.location = 0;
         //attrDesc.format = VK_FORMAT_R32G32B32_SFLOAT; // vec3
         //attrDesc.offset = 0;

         //pipelineconfiguration.bindingDescriptions = {bindingDesc};
         //pipelineconfiguration.attributeDescriptions = {attrDesc};

         //pipelineconfiguration.renderPass = renderpass;
         //pipelineconfiguration.pipelineLayout = VK_NULL_HANDLE;
         //pipelineconfiguration.dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
         //pipelineconfiguration.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
         //pipelineconfiguration.dynamicStateInfo.pDynamicStates = pipelineconfiguration.dynamicStateEnables.data();
         //pipelineconfiguration.dynamicStateInfo.dynamicStateCount =
         //   uint32_t(pipelineconfiguration.dynamicStateEnables.size());
         //pipelineconfiguration.aaadescriptorSetLayouts = {descriptorsetlayout};

         //VkPushConstantRange pushRange{};
         //pushRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
         //pushRange.offset = 0;
         //pushRange.size = sizeof(PushBlock);
         //pipelineconfiguration.pushConstantRanges = {pushRange};

         //pipelineconfiguration.pipelineLayout = VK_NULL_HANDLE;

         //// std::string vert = std::string(PROJECT_ROOT_DIR) + "/res/shaders/spirV/prefiltered_environment_map.vert.spv";
         //// std::string frag = std::string(PROJECT_ROOT_DIR) + "/res/shaders/spirV/irradiance_cube.frag.spv";
         //auto ppipelineIrradiance = createø<pipeline>();

         ////::memory vert;
         ////::memory frag;
         ////pgpudevice->defer_shader_memory(vert, "matter://shaders/prefiltered_environment_map.vert");
         ////pgpudevice->defer_shader_memory(frag, "matter://shaders/irradiance_cube.frag");
         //auto vert = embedded_diffuse_irradiance_vert();
         //auto frag = embedded_diffuse_irradiance_frag();

         //ppipelineIrradiance->initialize_graphics_pipeline(
         //   pgpucontext->m_pgpurenderer,
         //   vert, 
         //   frag, 
         //   pipelineconfiguration);

         //// COMMAND RECORDING
         //// VkCommandBuffer vkcommandbuffer = this->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
         //::pointer<::gpu_vulkan::command_buffer> pcommandbuffer = pgpucontext->beginSingleTimeCommands(pgpucontext->m_pgpudevice->transfer_queue());

         //// Transition irradiance cubemap to TRANSFER_DST (outside any renderpass)
         //VkImageSubresourceRange cubemapRange{VK_IMAGE_ASPECT_COLOR_BIT, 0, numMips, 0, 6};
         //::vulkan::setImageLayout(pcommandbuffer->m_vkcommandbuffer, pgputextureIrradianceCube->m_vkimage,
         //                         VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, cubemapRange);
         //using namespace graphics3d;
         //// Setup matrices (same as Sascha)
         //::array_base<floating_matrix4> matrices = {
         //   lookAt(origin, unitX, -unitY),
         //   lookAt(origin, -unitX, -unitY),
         //   lookAt(origin, unitY, unitZ),  
         //   lookAt(origin, -unitY, -unitZ),
         //   lookAt(origin, unitZ, -unitY), 
         //   lookAt(origin, -unitZ, -unitY)
         //};

         ////numMips = 1; // for diffuse irradiance, we only need the first mip

         //// Main loop: mips and faces (matches Sascha's approach)
         //for (uint32_t m = 0; m < numMips; ++m)
         //{
         //   uint32_t mipDim = static_cast<uint32_t>(dim * std::pow(0.5f, (float)m));
         //   VkViewport vp = vkinit::viewport((float)mipDim, (float)mipDim, 0.0f, 1.0f);
         //   VkRect2D sc = vkinit::rect2D(mipDim, mipDim, 0, 0);

         //   for (uint32_t face = 0; face < 6; ++face)
         //   {
         //      // Begin render pass into offscreen framebuffer
         //      VkClearValue clear{{{0.0f, 0.0f, 0.0f, 0.0f}}};
         //      VkRenderPassBeginInfo rpBI = vkinit::renderPassBeginInfo();
         //      rpBI.renderPass = renderpass;
         //      rpBI.framebuffer = offscreen.framebuffer;
         //      rpBI.renderArea.extent = {mipDim, mipDim};
         //      rpBI.clearValueCount = 1;
         //      rpBI.pClearValues = &clear;
         //      vkCmdBeginRenderPass(pcommandbuffer->m_vkcommandbuffer, &rpBI, VK_SUBPASS_CONTENTS_INLINE);

         //      vkCmdSetViewport(pcommandbuffer->m_vkcommandbuffer, 0, 1, &vp);
         //      vkCmdSetScissor(pcommandbuffer->m_vkcommandbuffer, 0, 1, &sc);

         //      // push constants
         //      pushBlock.mvp = m_pgpucontext->m_pengine->perspective(90f_degrees, 1.0f, 0.1f, 512.0f) * matrices[face];
         //      vkCmdPushConstants(pcommandbuffer->m_vkcommandbuffer, ppipelineIrradiance->_pipeline_layout(),
         //                         VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushBlock),
         //                         &pushBlock);

         //      // bind pipeline and descriptor set (USE the allocated VkDescriptorSet)
         //      ppipelineIrradiance->bind(pcommandbuffer);
         //      vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
         //                              ppipelineIrradiance->_pipeline_layout(), 0, 1, &descriptorset, 0, nullptr);

         //      // draw skybox — ensure your skybox.draw binds the vertex buffer that matches location 0 vec3 pos
         //      // if (!m_skyboxModel) {
         //      //     spdlog::error("[AssetManager] No skybox model loaded - skipping draw in generateIrradianceMap()");
         //      // }
         //      // else {


         //      prenderableSkybox->bind(pcommandbuffer);
         //      prenderableSkybox->draw(pcommandbuffer);
         //      // pmodelbufferSkybox->gltfDraw(pcommandbuffer->m_vkcommandbuffer);
         //      // }


         //      // END render pass BEFORE any barriers/copies
         //      vkCmdEndRenderPass(pcommandbuffer->m_vkcommandbuffer);

         //      // Transition offscreen image -> TRANSFER_SRC and copy to target cubemap mip/face
         //      ::vulkan::setImageLayout(pcommandbuffer->m_vkcommandbuffer, offscreen.image, VK_IMAGE_ASPECT_COLOR_BIT,
         //                               VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

         //      VkImageCopy copyRegion{};
         //      copyRegion.srcSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
         //      copyRegion.dstSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, m, face, 1};
         //      copyRegion.extent = {mipDim, mipDim, 1};

         //      vkCmdCopyImage(pcommandbuffer->m_vkcommandbuffer, offscreen.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
         //                     pgputextureIrradianceCube->m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

         //      // restore offscreen layout for next render
         //      ::vulkan::setImageLayout(pcommandbuffer->m_vkcommandbuffer, offscreen.image, VK_IMAGE_ASPECT_COLOR_BIT,
         //                               VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
         //   }
         //}

         //// final transition for cubemap to shader read layout
         //::vulkan::setImageLayout(pcommandbuffer->m_vkcommandbuffer, pgputextureIrradianceCube->m_vkimage,
         //                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
         //                         cubemapRange);

         //// this->flushCommandBuffer(vkcommandbuffer, m_vkqueueTransfer3);
         //pgpucontext->endSingleTimeCommands(pcommandbuffer);
         //::cast<::gpu_vulkan::queue> pqueue = pcommandbuffer->m_pgpuqueue;
         //vkQueueWaitIdle(pqueue->m_vkqueue);

         //// cleanup (destroy created renderpass/framebuffer)
         //vkDestroyFramebuffer(pgpucontext->logicalDevice(), offscreen.framebuffer, nullptr);
         //vkDestroyRenderPass(pgpucontext->logicalDevice(), renderpass, nullptr);

         //auto tEnd = std::chrono::high_resolution_clock::now();
         //auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
         //information("Generating irradiance cube took {} ms", tDiff);

         //defer_construct_newø(m_pframebufferDiffuseIrradiance);

         //m_pframebufferDiffuseIrradiance->m_ptexture = pgputextureIrradianceCubeNew;

         ////return pgputextureIrradianceCubeNew;
      }

   } // namespace ibl

} // namespace gpu_vulkan




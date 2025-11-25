// Merge gpu_opengl/diffuse_irradiance_map.h with gpu_vulkan::context::generatePreFilteredEnvMap by
// camilo on 2025-09-29 18:34 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "specular_map.h"
#include "app-graphics3d/gpu_vulkan/command_buffer.h"
#include "app-graphics3d/gpu_vulkan/context.h"
#include "app-graphics3d/gpu_vulkan/device.h"
#include "app-graphics3d/gpu_vulkan/gltf_model.h"
#include "app-graphics3d/gpu_vulkan/physical_device.h"
#include "app-graphics3d/gpu_vulkan/pipeline.h"
#include "app-graphics3d/gpu_vulkan/queue.h"
#include "app-graphics3d/gpu_vulkan/texture.h"
#include "app-graphics3d/gpu_vulkan/vk_init.h"
#include "bred/graphics3d/_functions.h"
#include "bred/graphics3d/engine.h"
#include "bred/graphics3d/immersion_layer.h"
#include "bred/graphics3d/scene_base.h"
#include "bred/graphics3d/skybox.h"
#include <array>
#include <chrono>


namespace gpu_vulkan
{

   namespace ibl
   {

      specular_map::specular_map()
      {



      }



      specular_map::~specular_map() {}


      ::block specular_map::embedded_prefiltered_env_map_vert()
      {
         static unsigned int g_uaPrefilteredEnvMapVertexShader[] = {
#include "ibl/shader/filtered_cube.vert.spv.inl"
         };
         return as_memory_block(g_uaPrefilteredEnvMapVertexShader);
      }


      ::block specular_map::embedded_prefiltered_env_map_frag()
      {

         static unsigned int g_uaPrefilteredEnvMapFragmentShader[] = {
#include "ibl/shader/prefiltered_env_map.frag.spv.inl"
         };
         return as_memory_block(g_uaPrefilteredEnvMapFragmentShader);
      }

      
      ::block specular_map::embedded_brdf_convolution_vert()
      {
         static unsigned int g_uaBrdfConvolutionVertexShader[] = {
#include "ibl/shader/filtered_cube.vert.spv.inl"
         };
         return as_memory_block(g_uaBrdfConvolutionVertexShader);
      }


      ::block specular_map::embedded_brdf_convolution_frag()
      {

         static unsigned int g_uaBrdfConvolutionFragmentShader[] = {
#include "ibl/shader/prefiltered_env_map.frag.spv.inl"
         };
         return as_memory_block(g_uaBrdfConvolutionFragmentShader);
      }


            void specular_map::initialize_specular_map(::graphics3d::scene_base *pscenebase)
      //,
      //::graphics3d::renderable * prenderableSkybox)
      // const ::scoped_string & scopedstrengineRoot, const unsigned int uEnvironmentCubemapId)
      {
               initialize_scene_object(pscenebase);

      }
            
      //rh
      floating_matrix4 lookAtRH_withFlip(
         ::floating_sequence3 eye, 
         ::floating_sequence3 center,
         ::floating_sequence3 worldUp,
         bool bFlipX, 
         bool bFlipZ)
      {

         auto M = ::graphics3d::lookAt(eye, center, worldUp);

         // Flip X axis (right column)
         if (bFlipX)
         {
            M[0][0] = -M[0][0];
            M[1][0] = -M[1][0];
            M[2][0] = -M[2][0];
         }

         // Flip Z axis (forward column)
         if (bFlipZ)
         {
            M[0][2] = -M[0][2];
            M[1][2] = -M[1][2];
            M[2][2] = -M[2][2];
         }

         // Recompute translation so it matches the flipped axes
         floating_sequence3 rx = {M[0][0], M[1][0], M[2][0]};
         floating_sequence3 uy = {M[0][1], M[1][1], M[2][1]};
         floating_sequence3 fz = {M[0][2], M[1][2], M[2][2]};

         M[3][0] = -rx.dotted(eye);
         M[3][1] = -uy.dotted(eye);
         M[3][2] = fz.dotted(eye);

         return M;

      }


      void specular_map::computePrefilteredEnvMap(::gpu::command_buffer *pgpucommandbuffer)
      {

         ::pointer<::gpu::texture> pgputexturePrefilteredCubeNew;

         øconstruct(pgputexturePrefilteredCubeNew);

         ::cast<::gpu_vulkan::texture> pgputexturePrefilteredCube = pgputexturePrefilteredCubeNew;

         ::cast<::graphics3d::skybox> pskybox = m_pscene->current_skybox();

         ::cast<::gpu_vulkan::texture> pgputextureSkybox = pskybox->m_ptexture;

         if (!pgputexturePrefilteredCube || !pgputextureSkybox)
         {

            error("bad argument");

            throw ::exception(error_failed);
         }

         ::cast<::gpu_vulkan::context> pgpucontext = m_pgpucontext;

         ::cast<device> pdevice = pgpucontext->m_pgpudevice;

         auto pphysicaldevice = pdevice->m_pphysicaldevice;

         auto pscene = pgpucontext->m_pengine->m_pimmersionlayer->m_pscene;

         auto prenderableSkybox = pskybox->m_prenderable;

         auto tStart = std::chrono::high_resolution_clock::now();

         const VkFormat format = VK_FORMAT_R16G16B16A16_SFLOAT;
         const int32_t dim = m_uPrefilteredEnvMapWidth;
         const uint32_t numMips = static_cast<uint32_t>(floor(log2(dim))) + 1;

         // Pre-filtered cube map
         // Image
         VkImageCreateInfo imageCI = vkinit::imageCreateInfo();
         imageCI.imageType = VK_IMAGE_TYPE_2D;
         imageCI.format = format;
         imageCI.extent.width = dim;
         imageCI.extent.height = dim;
         imageCI.extent.depth = 1;
         imageCI.mipLevels = numMips;
         imageCI.arrayLayers = 6;
         imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
         imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
         imageCI.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
         imageCI.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
         VK_CHECK_RESULT(vkCreateImage(pgpucontext->logicalDevice(), &imageCI, nullptr, &pgputexturePrefilteredCube->m_vkimage));
         VkMemoryAllocateInfo memAlloc = vkinit::memoryAllocateInfo();
         VkMemoryRequirements memReqs;
         vkGetImageMemoryRequirements(pgpucontext->logicalDevice(), pgputexturePrefilteredCube->m_vkimage, &memReqs);
         memAlloc.allocationSize = memReqs.size;
         memAlloc.memoryTypeIndex =
            pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
         VK_CHECK_RESULT(
            vkAllocateMemory(pgpucontext->logicalDevice(), &memAlloc, nullptr, &pgputexturePrefilteredCube->m_vkdevicememory));
         VK_CHECK_RESULT(vkBindImageMemory(pgpucontext->logicalDevice(), pgputexturePrefilteredCube->m_vkimage,
                                           pgputexturePrefilteredCube->m_vkdevicememory, 0));
         // Image view
         VkImageViewCreateInfo viewCI = vkinit::imageViewCreateInfo();
         viewCI.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
         viewCI.format = format;
         viewCI.subresourceRange = {};
         viewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         viewCI.subresourceRange.levelCount = numMips;
         viewCI.subresourceRange.layerCount = 6;
         viewCI.image = pgputexturePrefilteredCube->m_vkimage;
         VK_CHECK_RESULT(
            vkCreateImageView(pgpucontext->logicalDevice(), &viewCI, nullptr, &pgputexturePrefilteredCube->m_vkimageview));
         // Sampler
         VkSamplerCreateInfo samplerCI = vkinit::samplerCreateInfo();
         samplerCI.magFilter = VK_FILTER_LINEAR;
         samplerCI.minFilter = VK_FILTER_LINEAR;
         samplerCI.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
         samplerCI.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
         samplerCI.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
         samplerCI.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
         samplerCI.minLod = 0.0f;
         samplerCI.maxLod = static_cast<float>(numMips);
         samplerCI.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
         VK_CHECK_RESULT(
            vkCreateSampler(pgpucontext->logicalDevice(), &samplerCI, nullptr, &pgputexturePrefilteredCube->m_vksamplerDedicated));

         pgputexturePrefilteredCube->m_descriptor3.imageView = pgputexturePrefilteredCube->m_vkimageview;
         pgputexturePrefilteredCube->m_descriptor3.sampler = pgputexturePrefilteredCube->m_vksamplerDedicated;
         pgputexturePrefilteredCube->m_descriptor3.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
         // pgputexturePrefilteredCube->m_pDevice = &m_device;

         // FB, Att, RP, Pipe, etc.
         VkAttachmentDescription attDesc = {};
         // Color attachment
         attDesc.format = format;
         attDesc.samples = VK_SAMPLE_COUNT_1_BIT;
         attDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
         attDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
         attDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
         attDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
         attDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
         attDesc.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
         VkAttachmentReference colorReference = {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

         VkSubpassDescription subpassDescription = {};
         subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
         subpassDescription.colorAttachmentCount = 1;
         subpassDescription.pColorAttachments = &colorReference;

         // Use subpass dependencies for layout transitions
         std::array<VkSubpassDependency, 2> dependencies;
         dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
         dependencies[0].dstSubpass = 0;
         dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
         dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
         dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
         dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
         dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
         dependencies[1].srcSubpass = 0;
         dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
         dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
         dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
         dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
         dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
         dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

         // Renderpass
         VkRenderPassCreateInfo renderPassCI = vkinit::renderPassCreateInfo();
         renderPassCI.attachmentCount = 1;
         renderPassCI.pAttachments = &attDesc;
         renderPassCI.subpassCount = 1;
         renderPassCI.pSubpasses = &subpassDescription;
         renderPassCI.dependencyCount = 2;
         renderPassCI.pDependencies = dependencies.data();
         VkRenderPass renderpass;
         VK_CHECK_RESULT(vkCreateRenderPass(pgpucontext->logicalDevice(), &renderPassCI, nullptr, &renderpass));

         struct
         {
            VkImage image;
            VkImageView view;
            VkDeviceMemory memory;
            VkFramebuffer framebuffer;
         } offscreen;

         // Offfscreen framebuffer
         {
            // Color attachment
            VkImageCreateInfo imageCreateInfo = vkinit::imageCreateInfo();
            imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
            imageCreateInfo.format = format;
            imageCreateInfo.extent.width = dim;
            imageCreateInfo.extent.height = dim;
            imageCreateInfo.extent.depth = 1;
            imageCreateInfo.mipLevels = 1;
            imageCreateInfo.arrayLayers = 1;
            imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
            imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            VK_CHECK_RESULT(vkCreateImage(pgpucontext->logicalDevice(), &imageCreateInfo, nullptr, &offscreen.image));

            VkMemoryAllocateInfo memAlloc = vkinit::memoryAllocateInfo();
            VkMemoryRequirements memReqs;
            vkGetImageMemoryRequirements(pgpucontext->logicalDevice(), offscreen.image, &memReqs);
            memAlloc.allocationSize = memReqs.size;
            memAlloc.memoryTypeIndex =
               pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
            VK_CHECK_RESULT(vkAllocateMemory(pgpucontext->logicalDevice(), &memAlloc, nullptr, &offscreen.memory));
            VK_CHECK_RESULT(vkBindImageMemory(pgpucontext->logicalDevice(), offscreen.image, offscreen.memory, 0));

            VkImageViewCreateInfo colorImageView = vkinit::imageViewCreateInfo();
            colorImageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
            colorImageView.format = format;
            colorImageView.flags = 0;
            colorImageView.subresourceRange = {};
            colorImageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            colorImageView.subresourceRange.baseMipLevel = 0;
            colorImageView.subresourceRange.levelCount = 1;
            colorImageView.subresourceRange.baseArrayLayer = 0;
            colorImageView.subresourceRange.layerCount = 1;
            colorImageView.image = offscreen.image;
            VK_CHECK_RESULT(vkCreateImageView(pgpucontext->logicalDevice(), &colorImageView, nullptr, &offscreen.view));

            VkFramebufferCreateInfo fbufCreateInfo = vkinit::framebufferCreateInfo();
            fbufCreateInfo.renderPass = renderpass;
            fbufCreateInfo.attachmentCount = 1;
            fbufCreateInfo.pAttachments = &offscreen.view;
            fbufCreateInfo.width = dim;
            fbufCreateInfo.height = dim;
            fbufCreateInfo.layers = 1;
            VK_CHECK_RESULT(
               vkCreateFramebuffer(pgpucontext->logicalDevice(), &fbufCreateInfo, nullptr, &offscreen.framebuffer));

            // VkCommandBuffer layoutCmd = m_device.createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
            auto pgpucommandbufferLayoutCmd = pgpucontext->beginSingleTimeCommands(pgpucontext->m_pgpudevice->transfer_queue());
            ::cast<command_buffer> pcommandbufferLayoutCmd = pgpucommandbufferLayoutCmd;
            ::vulkan::setImageLayout(pcommandbufferLayoutCmd->m_vkcommandbuffer, offscreen.image,
                                     VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_UNDEFINED,
                                     VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
            // m_device.flushCommandBuffer(pcommandbufferLayoutCmd->m_vkcommandbuffer, m_transferQueue, true); ;
            pgpucontext->endSingleTimeCommands(pcommandbufferLayoutCmd);
         }

         // --- Descriptor layout / pool / set ---
         VkDescriptorSetLayout descriptorsetlayout = VK_NULL_HANDLE;
         ::array_base<VkDescriptorSetLayoutBinding> setLayoutBindings = {
            vkinit::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT,
                                               0),
         };
         VkDescriptorSetLayoutCreateInfo descriptorsetlayoutCI =
            vkinit::descriptorSetLayoutCreateInfo(setLayoutBindings);
         VK_CHECK_RESULT(vkCreateDescriptorSetLayout(pgpucontext->logicalDevice(), &descriptorsetlayoutCI, nullptr,
                                                     &descriptorsetlayout));

         // Descriptor Pool
         VkDescriptorPool descriptorpool = VK_NULL_HANDLE;
         ::array_base<VkDescriptorPoolSize> poolSizes = {
            vkinit::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1)};
         VkDescriptorPoolCreateInfo descriptorPoolCI = vkinit::descriptorPoolCreateInfo(poolSizes, 2);
         VK_CHECK_RESULT(
            vkCreateDescriptorPool(pgpucontext->logicalDevice(), &descriptorPoolCI, nullptr, &descriptorpool));

         // Allocate descriptor set
         VkDescriptorSet descriptorset = VK_NULL_HANDLE;
         VkDescriptorSetAllocateInfo allocInfo =
            vkinit::descriptorSetAllocateInfo(descriptorpool, &descriptorsetlayout, 1);
         VK_CHECK_RESULT(vkAllocateDescriptorSets(pgpucontext->logicalDevice(), &allocInfo, &descriptorset));

         // Write the environment cubemap descriptor (make sure pgputextureSkybox is valid)
         if (!pgputextureSkybox)
         {
            throw ::exception(error_failed, "generatePrefilteredEnvMap: pgputextureSkybox is null");
         }
         VkWriteDescriptorSet writeDescriptorSet = vkinit::writeDescriptorSet(
            descriptorset, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &pgputextureSkybox->m_descriptor3);
         vkUpdateDescriptorSets(pgpucontext->logicalDevice(), 1, &writeDescriptorSet, 0, nullptr);

         // --- Pipeline layout & push constants ---
         struct PushBlock
         {
            floating_matrix4 mvp;
            float roughness;
            uint32_t numSamples = 32u;
         } pushBlock;

         VkPushConstantRange pushRange{};
         pushRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
         pushRange.offset = 0;
         pushRange.size = sizeof(PushBlock);

         VkPipelineLayout pipelineLayoutLocal = VK_NULL_HANDLE;
         VkPipelineLayoutCreateInfo pipelineLayoutCI = vkinit::pipelineLayoutCreateInfo(&descriptorsetlayout, 1);
         pipelineLayoutCI.pushConstantRangeCount = 1;
         pipelineLayoutCI.pPushConstantRanges = &pushRange;
         VK_CHECK_RESULT(
            vkCreatePipelineLayout(pgpucontext->logicalDevice(), &pipelineLayoutCI, nullptr, &pipelineLayoutLocal));

         // --- Pipeline creation using your VkSandboxPipeline wrapper (vertex pos only) ---
         ::vulkan::pipeline_configuration cfg{};
         ::vulkan::defaultPipelineConfigInfo2(cfg);

         // Vertex input: vec3 position only (location 0)
         VkVertexInputBindingDescription bindingDesc{};
         bindingDesc.binding = 0;
         bindingDesc.stride = sizeof(::gpu_vulkan::gltf::Vertex);
         bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

         VkVertexInputAttributeDescription attrDesc{};
         attrDesc.binding = 0;
         attrDesc.location = 0;
         attrDesc.format = VK_FORMAT_R32G32B32_SFLOAT;
         attrDesc.offset = 0;

         cfg.bindingDescriptions = {bindingDesc};
         cfg.attributeDescriptions = {attrDesc};
         cfg.renderPass = renderpass;
         cfg.pipelineLayout = pipelineLayoutLocal;
         cfg.dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
         cfg.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
         cfg.dynamicStateInfo.pDynamicStates = cfg.dynamicStateEnables.data();
         cfg.dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(cfg.dynamicStateEnables.size());
         cfg.descriptorSetLayouts = {descriptorsetlayout};
         cfg.pushConstantRanges = {pushRange};

         // shader paths (match your project layout)
         //::memory vert;
         //::memory frag;
         //pdevice->defer_shader_memory(vert, "matter://shaders/filtered_cube.vert");
         //pdevice->defer_shader_memory(frag, "matter://shaders/prefiltered_env_map.frag");
         // auto vert = file()->as_memory("matter://shaders/filtered_cube.vert");
         // auto frag = file()->as_memory("matter://shaders/prefiltered_env_map.frag");

         auto vert = embedded_prefiltered_env_map_vert();
         auto frag = embedded_prefiltered_env_map_frag();

         if (frag.is_empty())
         {
            // cleanup minimal resources
            if (pipelineLayoutLocal != VK_NULL_HANDLE)
               vkDestroyPipelineLayout(pgpucontext->logicalDevice(), pipelineLayoutLocal, nullptr);
            if (descriptorsetlayout != VK_NULL_HANDLE)
               vkDestroyDescriptorSetLayout(pgpucontext->logicalDevice(), descriptorsetlayout, nullptr);
            if (descriptorpool != VK_NULL_HANDLE)
               vkDestroyDescriptorPool(pgpucontext->logicalDevice(), descriptorpool, nullptr);
            throw ::exception(error_failed, "Prefilter fragment shader SPIR-V not found");
         }

         auto ppipelinePrefilter = øcreate<::gpu_vulkan::pipeline>();


         ppipelinePrefilter->initialize_graphics_pipeline(
            pgpucontext->m_pgpurenderer,
            vert,
            frag,
            cfg);

         // --- Command buffer & initial transitions (use m_device helpers) ---
         // VkCommandBuffer pcommandbufferCmd->m_vkcommandbuffer =
         // m_device.createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

         auto pgpucommandbufferCmd = pgpucontext->beginSingleTimeCommands(pgpucontext->m_pgpudevice->transfer_queue());

         ::cast<::gpu_vulkan::command_buffer> pcommandbufferCmd = pgpucommandbufferCmd;

         VkImageSubresourceRange subresourceRange = {};
         subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         subresourceRange.baseMipLevel = 0;
         subresourceRange.levelCount = numMips;
         subresourceRange.layerCount = 6;

         // Transition target cubemap to transfer dst
         ::vulkan::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, pgputexturePrefilteredCube->m_vkimage,
                                  VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourceRange);
         using namespace graphics3d;
         // Setup matrices and viewports
         ::array_base<floating_matrix4> matrices = {
            lookAt(origin, unitX, -unitY),
            lookAt(origin, -unitX, -unitY), 
            lookAtRH_withFlip(origin, -unitY, -unitZ, 0, 1), 
            lookAtRH_withFlip(origin, unitY, unitZ, 0, 1),
            lookAt(origin, unitZ, -unitY),
            lookAt(origin, -unitZ, -unitY)
         };
//      };

         VkViewport viewport = vkinit::viewport((float)dim, (float)dim, 0.0f, 1.0f);
         VkRect2D scissor = vkinit::rect2D(dim, dim, 0, 0);

         vkCmdSetViewport(pcommandbufferCmd->m_vkcommandbuffer, 0, 1, &viewport);
         vkCmdSetScissor(pcommandbufferCmd->m_vkcommandbuffer, 0, 1, &scissor);

         // --- Main render loop (mips + faces) ---
         for (uint32_t m = 0; m < numMips; m++)
         {
            pushBlock.roughness = static_cast<float>(m) / static_cast<float>(numMips - 1);
            uint32_t mipDim = static_cast<uint32_t>(dim * std::pow(0.5f, (float)m));
            viewport.width = static_cast<float>(mipDim);
            viewport.height = static_cast<float>(mipDim);

            for (uint32_t f = 0; f < 6; f++)
            {
               // Update render area for this mip
               VkRenderPassBeginInfo rpBI = vkinit::renderPassBeginInfo();
               rpBI.renderPass = renderpass;
               rpBI.framebuffer = offscreen.framebuffer;
               rpBI.renderArea.extent.width = mipDim;
               rpBI.renderArea.extent.height = mipDim;
               VkClearValue clear{{{0.0f, 0.0f, 0.0f, 0.0f}}};
               rpBI.clearValueCount = 1;
               rpBI.pClearValues = &clear;

               vkCmdBeginRenderPass(pcommandbufferCmd->m_vkcommandbuffer, &rpBI, VK_SUBPASS_CONTENTS_INLINE);

               vkCmdSetViewport(pcommandbufferCmd->m_vkcommandbuffer, 0, 1, &viewport);
               vkCmdSetScissor(pcommandbufferCmd->m_vkcommandbuffer, 0, 1, &scissor);

               // push constants (projection * view)
               pushBlock.mvp = m_pgpucontext->m_pengine->perspective(
                  90f_degrees, 1.0f, 0.1f, 512.0f) * matrices[f];
               pushBlock.mvp[1][1] *= -1.0f; // flip y

               vkCmdPushConstants(pcommandbufferCmd->m_vkcommandbuffer, ppipelinePrefilter->m_vkpipelinelayout,
                                  VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushBlock),
                                  &pushBlock);

               // bind pipeline and descriptor set (environment cubemap sampler)
               ppipelinePrefilter->bind(pcommandbufferCmd);
               vkCmdBindDescriptorSets(pcommandbufferCmd->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                       ppipelinePrefilter->m_vkpipelinelayout, 0, 1, &descriptorset, 0, nullptr);

               // draw the skybox mesh (ensure it binds position vertex at location 0)
               // if (!m_skyboxModel)
               //{
               //   spdlog::error("[AssetManager] No skybox model loaded - skipping draw in
               //   generatePrefilteredEnvMap()");
               //}
               // else
               //{
               prenderableSkybox->bind(pcommandbufferCmd);
               // prenderableSkybox->gltfDraw(pcommandbufferCmd);
               prenderableSkybox->draw(pcommandbufferCmd);
               //}

               vkCmdEndRenderPass(pcommandbufferCmd->m_vkcommandbuffer);

               // copy from offscreen -> pgputexturePrefilteredCube mip/face
               ::vulkan::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, offscreen.image,
                                        VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

               VkImageCopy copyRegion{};
               copyRegion.srcSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
               copyRegion.dstSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, m, f, 1};
               copyRegion.extent = {mipDim, mipDim, 1};

               vkCmdCopyImage(pcommandbufferCmd->m_vkcommandbuffer, offscreen.image,
                              VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, pgputexturePrefilteredCube->m_vkimage,
                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

               // restore offscreen layout
               ::vulkan::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, offscreen.image,
                                        VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
            }
         }

         // final transition: prefiltered cubemap -> shader read
         ::vulkan::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, pgputexturePrefilteredCube->m_vkimage,
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                  subresourceRange);

         pgpucontext->endSingleTimeCommands(pcommandbufferCmd);
         ::cast<::gpu_vulkan::queue> pqueue = pcommandbufferCmd->m_pgpuqueue;
         vkQueueWaitIdle(pqueue->m_vkqueue);


         // --- Cleanup: destroy only resources we created here (do NOT destroy pipeline layout; wrapper owns pipeline)
         if (offscreen.framebuffer != VK_NULL_HANDLE)
            vkDestroyFramebuffer(pgpucontext->logicalDevice(), offscreen.framebuffer, nullptr);
         if (renderpass != VK_NULL_HANDLE)
            vkDestroyRenderPass(pgpucontext->logicalDevice(), renderpass, nullptr);
         if (offscreen.memory != VK_NULL_HANDLE)
            vkFreeMemory(pgpucontext->logicalDevice(), offscreen.memory, nullptr);
         if (offscreen.view != VK_NULL_HANDLE)
            vkDestroyImageView(pgpucontext->logicalDevice(), offscreen.view, nullptr);
         if (offscreen.image != VK_NULL_HANDLE)
            vkDestroyImage(pgpucontext->logicalDevice(), offscreen.image, nullptr);
         if (descriptorpool != VK_NULL_HANDLE)
            vkDestroyDescriptorPool(pgpucontext->logicalDevice(), descriptorpool, nullptr);
         if (descriptorsetlayout != VK_NULL_HANDLE)
            vkDestroyDescriptorSetLayout(pgpucontext->logicalDevice(), descriptorsetlayout, nullptr);

         auto tEnd = std::chrono::high_resolution_clock::now();
         auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
         information("Generating pre-filtered environment cube with {} mip levels took {} ms", numMips, tDiff);

         ødefer_construct_new(m_pframebufferPrefilteredEnvMap);

         m_pframebufferPrefilteredEnvMap->m_ptexture = pgputexturePrefilteredCubeNew;

      }


















      //::pointer<::gpu::texture> context::generateBRDFlut()
      void specular_map::computeBrdfConvolutionMap()
      {

         ::pointer<::gpu::texture> pgputextureLutBrdfNew;

         øconstruct(pgputextureLutBrdfNew);

         ::cast<::gpu_vulkan::texture> pgputextureLutBrdf = pgputextureLutBrdfNew;

         ::cast<::gpu_vulkan::context> pgpucontext = m_pgpucontext;

         ::cast<::gpu_vulkan::device> pgpudevice = pgpucontext->m_pgpudevice;

         auto pphysicaldevice = pgpudevice->m_pphysicaldevice;

         auto tStart = std::chrono::high_resolution_clock::now();

         const VkFormat format = VK_FORMAT_R16G16_SFLOAT; // R16G16 is supported pretty much everywhere
         const int32_t dim = 512;

         // Image
         VkImageCreateInfo imageCI = vkinit::imageCreateInfo();
         imageCI.imageType = VK_IMAGE_TYPE_2D;
         imageCI.format = format;
         imageCI.extent.width = dim;
         imageCI.extent.height = dim;
         imageCI.extent.depth = 1;
         imageCI.mipLevels = 1;
         imageCI.arrayLayers = 1;
         imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
         imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
         imageCI.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
         VK_CHECK_RESULT(vkCreateImage(pgpucontext->logicalDevice(), &imageCI, nullptr, &pgputextureLutBrdf->m_vkimage));
         VkMemoryAllocateInfo memAlloc = vkinit::memoryAllocateInfo();
         VkMemoryRequirements memReqs;
         vkGetImageMemoryRequirements(pgpucontext->logicalDevice(), pgputextureLutBrdf->m_vkimage, &memReqs);
         memAlloc.allocationSize = memReqs.size;
         memAlloc.memoryTypeIndex =
            pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
         VK_CHECK_RESULT(vkAllocateMemory(pgpucontext->logicalDevice(), &memAlloc, nullptr, &pgputextureLutBrdf->m_vkdevicememory));
         VK_CHECK_RESULT(vkBindImageMemory(pgpucontext->logicalDevice(), pgputextureLutBrdf->m_vkimage, pgputextureLutBrdf->m_vkdevicememory, 0));


         // Image view
         VkImageViewCreateInfo viewCI = vkinit::imageViewCreateInfo();
         viewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
         viewCI.format = format;
         viewCI.subresourceRange = {};
         viewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         viewCI.subresourceRange.levelCount = 1;
         viewCI.subresourceRange.layerCount = 1;
         viewCI.image = pgputextureLutBrdf->m_vkimage;
         VK_CHECK_RESULT(vkCreateImageView(pgpucontext->logicalDevice(), &viewCI, nullptr, &pgputextureLutBrdf->m_vkimageview));

         // pgputextureLutBrdf->m_vksampler3 = _001VkSampler();

         // Sampler
         VkSamplerCreateInfo samplerCI = vkinit::samplerCreateInfo();
         samplerCI.magFilter = VK_FILTER_LINEAR;
         samplerCI.minFilter = VK_FILTER_LINEAR;
         samplerCI.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
         samplerCI.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
         samplerCI.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
         samplerCI.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
         samplerCI.minLod = 0.0f;
         samplerCI.maxLod = 1.0f;
         samplerCI.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
         VK_CHECK_RESULT(vkCreateSampler(pgpucontext->logicalDevice(), &samplerCI, nullptr, &pgputextureLutBrdf->m_vksamplerDedicated));

         pgputextureLutBrdf->m_descriptor3.imageView = pgputextureLutBrdf->m_vkimageview;
         pgputextureLutBrdf->m_descriptor3.sampler = pgputextureLutBrdf->m_vksamplerDedicated;
         pgputextureLutBrdf->m_descriptor3.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
         // pgputextureLutBrdf->m_pDevice = m_pgpudevice;

         // FB, Att, RP, Pipe, etc.
         VkAttachmentDescription attDesc = {};
         // Color attachment
         attDesc.format = format;
         attDesc.samples = VK_SAMPLE_COUNT_1_BIT;
         attDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
         attDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
         attDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
         attDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
         attDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
         attDesc.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
         VkAttachmentReference colorReference = {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

         VkSubpassDescription subpassDescription = {};
         subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
         subpassDescription.colorAttachmentCount = 1;
         subpassDescription.pColorAttachments = &colorReference;

         // Use subpass dependencies for layout transitions
         ::block_array<VkSubpassDependency, 2> dependencies;
         dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
         dependencies[0].dstSubpass = 0;
         dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
         dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
         dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
         dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
         dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
         dependencies[1].srcSubpass = 0;
         dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
         dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
         dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
         dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
         dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
         dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

         // Create the actual renderpass
         VkRenderPassCreateInfo renderPassCI{};
         renderPassCI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
         renderPassCI.attachmentCount = 1;
         renderPassCI.pAttachments = &attDesc;
         renderPassCI.subpassCount = 1;
         renderPassCI.pSubpasses = &subpassDescription;
         renderPassCI.dependencyCount = static_cast<uint32_t>(dependencies.size());
         renderPassCI.pDependencies = dependencies.data();

         VkRenderPass renderpass = VK_NULL_HANDLE;
         VK_CHECK_RESULT(vkCreateRenderPass(pgpucontext->logicalDevice(), &renderPassCI, nullptr, &renderpass));

         VkFramebufferCreateInfo framebufferCI = vkinit::framebufferCreateInfo();
         framebufferCI.renderPass = renderpass;
         framebufferCI.attachmentCount = 1;
         framebufferCI.pAttachments = &pgputextureLutBrdf->m_vkimageview;
         framebufferCI.width = dim;
         framebufferCI.height = dim;
         framebufferCI.layers = 1;

         VkFramebuffer framebuffer;
         VK_CHECK_RESULT(vkCreateFramebuffer(pgpucontext->logicalDevice(), &framebufferCI, nullptr, &framebuffer));

         // Descriptors
         VkDescriptorSetLayout descriptorsetlayout;
         ::array_base<VkDescriptorSetLayoutBinding> setLayoutBindings = {};
         VkDescriptorSetLayoutCreateInfo descriptorsetlayoutCI =
            vkinit::descriptorSetLayoutCreateInfo(setLayoutBindings);
         VK_CHECK_RESULT(
            vkCreateDescriptorSetLayout(pgpucontext->logicalDevice(), &descriptorsetlayoutCI, nullptr, &descriptorsetlayout));

         // Descriptor Pool
         ::array_base<VkDescriptorPoolSize> poolSizes = {
            vkinit::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1)};
         VkDescriptorPoolCreateInfo descriptorPoolCI = vkinit::descriptorPoolCreateInfo(poolSizes, 2);
         VkDescriptorPool descriptorpool;
         VK_CHECK_RESULT(vkCreateDescriptorPool(pgpucontext->logicalDevice(), &descriptorPoolCI, nullptr, &descriptorpool));

         // Descriptor sets
         VkDescriptorSet descriptorset;
         VkDescriptorSetAllocateInfo allocInfo =
            vkinit::descriptorSetAllocateInfo(descriptorpool, &descriptorsetlayout, 1);
         VK_CHECK_RESULT(vkAllocateDescriptorSets(pgpucontext->logicalDevice(), &allocInfo, &descriptorset));

         // Pipeline layout
         VkPipelineLayout pipelinelayout;
         VkPipelineLayoutCreateInfo pipelineLayoutCI = vkinit::pipelineLayoutCreateInfo(&descriptorsetlayout, 1);
         VK_CHECK_RESULT(vkCreatePipelineLayout(pgpucontext->logicalDevice(), &pipelineLayoutCI, nullptr, &pipelinelayout));

         // Pipeline
         VkPipelineInputAssemblyStateCreateInfo inputAssemblyState =
            vkinit::pipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, VK_FALSE);
         VkPipelineRasterizationStateCreateInfo rasterizationState = vkinit::pipelineRasterizationStateCreateInfo(
            VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE);
         VkPipelineColorBlendAttachmentState blendAttachmentState =
            vkinit::pipelineColorBlendAttachmentState(0xf, VK_FALSE);
         VkPipelineColorBlendStateCreateInfo colorBlendState =
            vkinit::pipelineColorBlendStateCreateInfo(1, &blendAttachmentState);
         VkPipelineDepthStencilStateCreateInfo depthStencilState =
            vkinit::pipelineDepthStencilStateCreateInfo(VK_FALSE, VK_FALSE, VK_COMPARE_OP_LESS_OR_EQUAL);
         VkPipelineViewportStateCreateInfo viewportState = vkinit::pipelineViewportStateCreateInfo(1, 1);
         VkPipelineMultisampleStateCreateInfo multisampleState =
            vkinit::pipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT);
         ::array_base<VkDynamicState> dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
         VkPipelineDynamicStateCreateInfo dynamicState = vkinit::pipelineDynamicStateCreateInfo(dynamicStateEnables);
         VkPipelineVertexInputStateCreateInfo emptyInputState = vkinit::pipelineVertexInputStateCreateInfo();
         ::block_array<VkPipelineShaderStageCreateInfo, 2> shaderStages;

         VkGraphicsPipelineCreateInfo pipelineCI = vkinit::pipelineCreateInfo(pipelinelayout, renderpass);
         pipelineCI.pInputAssemblyState = &inputAssemblyState;
         pipelineCI.pRasterizationState = &rasterizationState;
         pipelineCI.pColorBlendState = &colorBlendState;
         pipelineCI.pMultisampleState = &multisampleState;
         pipelineCI.pViewportState = &viewportState;
         pipelineCI.pDepthStencilState = &depthStencilState;
         pipelineCI.pDynamicState = &dynamicState;
         pipelineCI.stageCount = 2;
         pipelineCI.pStages = shaderStages.data();
         pipelineCI.pVertexInputState = &emptyInputState;


         // 4) Fill your pipeline_configuration_information
         ::vulkan::pipeline_configuration pipelineconfiguration{};
         ::vulkan::defaultPipelineConfigInfo2(pipelineconfiguration);

         pipelineconfiguration.bindingDescriptions.clear();
         pipelineconfiguration.attributeDescriptions.clear();
         pipelineconfiguration.renderPass = renderpass;
         pipelineconfiguration.pipelineLayout = pipelinelayout;
         //  viewport & scissor will be dynamic
         pipelineconfiguration.dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
         pipelineconfiguration.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
         pipelineconfiguration.dynamicStateInfo.pDynamicStates = pipelineconfiguration.dynamicStateEnables.data();
         pipelineconfiguration.dynamicStateInfo.dynamicStateCount =
            (uint32_t)pipelineconfiguration.dynamicStateEnables.size();

         // Look-up-table (from BRDF) pipeline

         auto ppipelineBrdf = øcreate_new<pipeline>();

         ::memory vert;
         ::memory frag;
         pgpudevice->defer_shader_memory(vert, "matter://shaders/gen_brdflut.vert");
         pgpudevice->defer_shader_memory(frag, "matter://shaders/gen_brdflut.frag");

         ppipelineBrdf->initialize_graphics_pipeline(pgpucontext->m_pgpurenderer, vert, frag, pipelineconfiguration);

         // COMMAND RECORDING
         // VkCommandBuffer vkcommandbuffer = this->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
         ::pointer<::gpu_vulkan::command_buffer> pcommandbuffer =
            pgpucontext->beginSingleTimeCommands(pgpucontext->m_pgpudevice->transfer_queue());

         // Render
         VkClearValue clearValues[1];
         clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};

         VkRenderPassBeginInfo renderPassBeginInfo = vkinit::renderPassBeginInfo();
         renderPassBeginInfo.renderPass = renderpass;
         renderPassBeginInfo.renderArea.extent.width = dim;
         renderPassBeginInfo.renderArea.extent.height = dim;
         renderPassBeginInfo.clearValueCount = 1;
         renderPassBeginInfo.pClearValues = clearValues;
         renderPassBeginInfo.framebuffer = framebuffer;

         // VkCommandBuffer vkcommandbuffer = m_pgpudevice->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
         vkCmdBeginRenderPass(pcommandbuffer->m_vkcommandbuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
         VkViewport viewport = vkinit::viewport((float)dim, (float)dim, 0.0f, 1.0f);
         VkRect2D scissor = vkinit::rect2D(dim, dim, 0, 0);
         vkCmdSetViewport(pcommandbuffer->m_vkcommandbuffer, 0, 1, &viewport);
         vkCmdSetScissor(pcommandbuffer->m_vkcommandbuffer, 0, 1, &scissor);

         ppipelineBrdf->bind(pcommandbuffer);
         vkCmdDraw(pcommandbuffer->m_vkcommandbuffer, 3, 1, 0, 0);
         vkCmdEndRenderPass(pcommandbuffer->m_vkcommandbuffer);
         // m_pgpudevice->flushCommandBuffer(pcommandbuffer->m_vkcommandbuffer, m_vkqueueTransfer3);
         pgpucontext->endSingleTimeCommands(pcommandbuffer);

         // vkQueueWaitIdle(m_vkqueueTransfer3);
         ::cast<::gpu_vulkan::queue> pqueue = pcommandbuffer->m_pgpuqueue;
         vkQueueWaitIdle(pqueue->m_vkqueue);

         vkDestroyFramebuffer(pgpucontext->logicalDevice(), framebuffer, nullptr);
         vkDestroyRenderPass(pgpucontext->logicalDevice(), renderpass, nullptr);

         auto tEnd = std::chrono::high_resolution_clock::now();
         auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
         information() << "Generating BRDF LUT took " << tDiff << " ms";

         ødefer_construct_new(m_pbrdfconvolutionframebuffer);

         m_pbrdfconvolutionframebuffer->m_ptexture = pgputextureLutBrdfNew;

      }



   } // namespace ibl

} // namespace gpu_vulkan

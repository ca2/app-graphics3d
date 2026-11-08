/*
* UI overlay class using ImGui
*
* Copyright (C) 2017-2024 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <sstream>
#include <iomanip>

#include <vulkan/vulkan.h>
#include "VulkanTools.h"
#include "VulkanDebug.h"
#include "VulkanBuffer.h"
#include "VulkanDevice.h"

#include "imgui/imgui.h"

#if defined(__ANDROID__)
#include "VulkanAndroid.h"
#endif

namespace vks
{
	class UIOverlay
	{
	public:
		vks::VulkanDevice* device{ nullptr };
		VkQueue queue{ VK_NULL_HANDLE };

		VkSampleCountFlagBits rasterizationSamples{ VK_SAMPLE_COUNT_1_BIT };
		uint32_t subpass{ 0 };

		vks::Buffer vertexBuffer;
		vks::Buffer indexBuffer;
		int32_t vertexCount{ 0 };
		int32_t indexCount{ 0 };

		std::vector<VkPipelineShaderStageCreateInfo> shaders;

		VkDescriptorPool descriptorPool{ VK_NULL_HANDLE };
		VkDescriptorSetLayout descriptorSetLayout{ VK_NULL_HANDLE };
		VkDescriptorSet descriptorSet{ VK_NULL_HANDLE };
		VkPipelineLayout pipelineLayout{ VK_NULL_HANDLE };
		VkPipeline pipeline{ VK_NULL_HANDLE };

		VkDeviceMemory fontMemory{ VK_NULL_HANDLE };
		VkImage fontImage{ VK_NULL_HANDLE };
		VkImageView fontView{ VK_NULL_HANDLE };
		VkSampler sampler{ VK_NULL_HANDLE };

		struct PushConstBlock {
			glm::vec2 scale;
			glm::vec2 translate;
		} pushConstBlock;

		bool visible{ true };
		bool updated{ false };
		::f32 scale{ 1.0f };
		::f32 updateTimer{ 0.0f };

		UIOverlay();
		~UIOverlay();

		void preparePipeline(const VkPipelineCache pipelineCache, const VkRenderPass renderPass, const VkFormat colorFormat, const VkFormat depthFormat);
		void prepareResources();

		bool update();
		void draw(const VkCommandBuffer commandBuffer);
		void resize(uint32_t width, uint32_t height);

		void freeResources();

		bool header(const_char_pointer caption);
		bool checkBox(const_char_pointer caption, bool* value);
		bool checkBox(const_char_pointer caption, int32_t* value);
		bool radioButton(const_char_pointer caption, bool value);
		bool inputFloat(const_char_pointer caption, ::f32* value, ::f32 step, const_char_pointer format = "%.3f"_ansi);
		bool sliderFloat(const_char_pointer caption, ::f32* value, ::f32 min, ::f32 max);
		bool sliderInt(const_char_pointer caption, int32_t* value, int32_t min, int32_t max);
		bool comboBox(const_char_pointer caption, int32_t* itemindex, std::vector<std::string> items);
		bool button(const_char_pointer caption);
		bool colorPicker(const_char_pointer caption, ::f32* color);
		void text(const_char_pointer formatstr, ...);
	};
}

#pragma once

#include "vk_camera.h"
#include "vk_gameObject.h"

// lib
#include <vulkan/vulkan.h>

namespace vkc {


#define MAX_LIGHTS 10

	struct PointLight {
		floating_sequence4 position{};  // ignore w
		floating_sequence4 color{};     // w is intensity
	};


	struct GlobalUbo {
		floating_matrix4 projection{ 1.f };
		floating_matrix4 view{ 1.f };
		floating_matrix4 inverseView{ 1.f };
		floating_sequence4 ambientLightColor{ 1.f, 1.f, 1.f, .02f };
		PointLight pointLights[MAX_LIGHTS];
		int numLights;
	};

	struct FrameInfo {
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		VkcCamera &camera;
		VkDescriptorSet globalDescriptorSet;
		VkcGameObject::Map &gameObjects;
	};
}
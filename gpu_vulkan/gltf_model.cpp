///*
//* Vulkan gltf model and texture loading class based on tinyglTF (https://github.com/syoyo/tinygltf)
//*
//* Copyright (C) 2018-2024 by Sascha Willems - www.saschawillems.de
//*
//* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
//*/
//
///*
// * Note that this isn't a complete gltf loader and not all features of the gltf 2.0 spec are supported
// * For details on how gltf 2.0 works, see the official spec at https://github.com/KhronosGroup/gltf/tree/master/specification/2.0
// *
// * If you are looking for a complete gltf implementation, check out https://github.com/SaschaWillems/Vulkan-gltf-PBR/
// */
#include "framework.h"
#include "acme/filesystem/filesystem/file_context.h"
#include "gpu_vulkan/physical_device.h"
#include "gltf_model.h"
#include "command_buffer.h"
#include "descriptors.h"
#include "render_target.h"
#include "vk_init.h"
#include "context.h"
#include "aura/graphics/image/context.h"
#include "gpu_vulkan/texture.h"
#include <filesystem> // C++17
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE

//#include "vulkan_wrapper/vulkan_gltf.h"
//#include "vk_tools/vk_tools.h"


//#include <fstream>
//#include <iostream>
//#include <string>
//#include <vector>
//
//#define TINYGLTF_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "tiny_gltf.h"

// ---------------------------------------------------------
// Global filesystem callbacks
// ---------------------------------------------------------

//bool tinygltf_example_FileExists(const std::string &abs_filename, void *user_data)
//{
//   std::ifstream f(abs_filename.c_str());
//   return f.good();
//}

//bool tinygltf_example_ReadWholeFile(std::vector<unsigned char> *out, std::string *err, const std::string &filepath,
//                                    void *user_data)
//{
//   std::ifstream f(filepath, std::ios::binary);
//   if (!f)
//   {
//      if (err)
//         *err = "Could not open file: " + filepath;
//      return false;
//   }
//   f.seekg(0, std::ios::end);
//   size_t sz = static_cast<size_t>(f.tellg());
//   f.seekg(0, std::ios::beg);
//
//   out->resize(sz);
//   f.read(reinterpret_cast<char *>(out->data()), sz);
//   return true;
//}
//
//bool tinygltf_example_WriteWholeFile(std::string *err, const std::string &filepath, const std::vector<unsigned char> &contents,
//                    void *user_data)
//{
//   std::ofstream f(filepath, std::ios::binary);
//   if (!f)
//   {
//      if (err)
//         *err = "Could not write file: " + filepath;
//      return false;
//   }
//   f.write(reinterpret_cast<const char *>(contents.data()), contents.size());
//   return true;
//}

//// ---------------------------------------------------------
//// Global image loader callback
//// ---------------------------------------------------------
//
//bool tinygltf_example_LoadImageData(tinygltf::Image *image, const int image_idx, std::string *err, std::string *warn,
//                                    int req_width,
//                   int req_height, const unsigned char *bytes, int size, void *user_data)
//{
//   // Decode using stb_image (already included by tinygltf)
//   int w, h, comp;
//   unsigned char *data = stbi_load_from_memory(bytes, size, &w, &h, &comp, 0);
//   if (!data)
//   {
//      if (err)
//         *err = "Failed to decode image " + image->uri;
//      return false;
//   }
//
//   image->width = w;
//   image->height = h;
//   image->component = comp;
//   image->image.assign(data, data + (w * h * comp));
//   image->bits = 8;
//   image->pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
//
//   stbi_image_free(data);
//   return true;
//}

//// ---------------------------------------------------------
//// Main
//// ---------------------------------------------------------
//
//int tinygltf_example_main(int argc, char **argv)
//{
//   if (argc < 2)
//   {
//      std::cerr << "Usage: " << argv[0] << " model.gltf" << std::endl;
//      return -1;
//   }
//
//   std::string filename(argv[1]);
//
//   tinygltf::Model model;
//   tinygltf::TinyGLTF loader;
//   std::string err, warn;
//
//   // Set custom filesystem callbacks
//   tinygltf::FsCallbacks fsCallbacks;
//   fsCallbacks.FileExists = FileExists;
//   fsCallbacks.ReadWholeFile = ReadWholeFile;
//   fsCallbacks.WriteWholeFile = WriteWholeFile;
//   fsCallbacks.ExpandFilePath = nullptr; // use default
//   fsCallbacks.user_data = nullptr;
//   loader.SetFsCallbacks(fsCallbacks);
//
//   // Set custom image loader
//   loader.SetImageLoader(LoadImageData, nullptr);
//
//   bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
//
//   if (!warn.empty())
//      std::cout << "Warn: " << warn << std::endl;
//   if (!err.empty())
//      std::cerr << "Err: " << err << std::endl;
//   if (!ret)
//   {
//      std::cerr << "Failed to load " << filename << std::endl;
//      return -1;
//   }
//
//   std::cout << "Loaded glTF file: " << filename << std::endl;
//
//   // Print buffers
//   for (size_t i = 0; i < model.buffers.size(); i++)
//   {
//      const auto &buffer = model.buffers[i];
//      std::cout << "Buffer[" << i << "] size: " << buffer.data.size() << " bytes" << std::endl;
//   }
//
//   // Print images
//   for (size_t i = 0; i < model.images.size(); i++)
//   {
//      const auto &image = model.images[i];
//      std::cout << "Image[" << i << "] : " << image.uri << " (" << image.width << "x" << image.height
//                << ", comp=" << image.component << ")" << std::endl;
//   }
//
//   return 0;
//}


namespace gpu_vulkan
{

//#include <fstream>
//#include <iostream>
//#include <string>
//#include <vector>
//
//#define TINYGLTF_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "tiny_gltf.h"
//
   // ---------------------------------------------------------
   // Global filesystem callbacks
   // ---------------------------------------------------------

   bool tinygltf_FileExists(const std::string &abs_filename, void *user_data)
   {
      
      auto pfile = (file_context *) user_data;
      
      ::file::path path(abs_filename.c_str());

      return pfile->exists(path);

   }


   bool tinygltf_ReadWholeFile(std::vector<unsigned char> *out, std::string *err, const std::string &filepath,
                               void *user_data)
   {

      
      auto pfile = (file_context *)user_data;

      try
      {

         ::file::path path(filepath.c_str());

         auto memory = pfile->as_memory(path);

         out->assign(memory.begin(), memory.end());
      }
      catch (...)
      {

         return false;

      }

      //std::ifstream f(filepath, std::ios::binary);
      //if (!f)
      //{
      //   if (err)
      //      *err = "Could not open file: " + filepath;
      //   return false;
      //}
      //f.seekg(0, std::ios::end);
      //size_t sz = static_cast<size_t>(f.tellg());
      //f.seekg(0, std::ios::beg);
      //f.read(reinterpret_cast<char *>(out->data()), sz);
      return true;

   }


   // ExpandFilePath callback: normalize/adjust file paths
   std::string tinygltf_ExpandFilePath(const std::string &filename, void *user_data)
   {
      //// Example: prepend a base asset directory (stored in user_data)
      //const char *baseDir = static_cast<const char *>(user_data);

      //std::filesystem::path base(baseDir ? baseDir : "");
      //d::filesystem::path file(filename);
    return filename;
      // Combine and normalize
      //std::filesystem::path full = std::filesystem::weakly_canonical(base / file);
      //return full.string();
   }


   bool tinygltf_WriteWholeFile(std::string *err, const std::string &filepath,
                                const std::vector<unsigned char> &contents,
                       void *user_data)
   {
      auto pfile = (file_context *)user_data;

      try
      {

         ::file::path path(filepath.c_str());

         pfile->put_memory(path, {contents.data(), contents.size()});

      }
      catch (...)
      {

         return false;

      }

      return true;
      //std::ofstream f(filepath, std::ios::binary);
      //if (!f)
      //{
      //   if (err)
      //      *err = "Could not write file: " + filepath;
      //   return false;
      //}
      //f.write(reinterpret_cast<const char *>(contents.data()), contents.size());
      //return true;
   }

   // ---------------------------------------------------------
   // Global image loader callback
   // ---------------------------------------------------------

   bool tinygltf_LoadImageData(tinygltf::Image *image, const int image_idx, std::string *err, std::string *warn, int req_width,
                      int req_height, const unsigned char *bytes, int size, void *user_data)
   {

      auto pimagecontext = (::image::image_context *)user_data;


      // Decode using stb_image (already included by tinygltf)
      int w, h, comp;
      unsigned char *data = pimagecontext->like_stbi_load_from_memory(bytes, size, &w, &h, &comp, 0);
      if (!data)
      {
         if (err)
            *err = "Failed to decode image " + image->uri;
         return false;
      }

      image->width = w;
      image->height = h;
      image->component = comp;
      image->image.assign(data, data + (w * h * comp));
      image->bits = 8;
      image->pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;

      pimagecontext->like_stbi_image_free(data);
      return true;
   }

   //// ---------------------------------------------------------
   //// Main
   //// ---------------------------------------------------------

   //int main(int argc, char **argv)
   //{
   //   if (argc < 2)
   //   {
   //      std::cerr << "Usage: " << argv[0] << " model.gltf" << std::endl;
   //      return -1;
   //   }

   //   std::string filename(argv[1]);

   //   tinygltf::Model model;
   //   tinygltf::TinyGLTF loader;
   //   std::string err, warn;

   //   // Set custom filesystem callbacks
   //   tinygltf::FsCallbacks fsCallbacks;
   //   fsCallbacks.FileExists = FileExists;
   //   fsCallbacks.ReadWholeFile = ReadWholeFile;
   //   fsCallbacks.WriteWholeFile = WriteWholeFile;
   //   fsCallbacks.ExpandFilePath = nullptr; // use default
   //   fsCallbacks.user_data = nullptr;
   //   loader.SetFsCallbacks(fsCallbacks);

   //   // Set custom image loader
   //   loader.SetImageLoader(LoadImageData, nullptr);

   //   bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename);

   //   if (!warn.empty())
   //      std::cout << "Warn: " << warn << std::endl;
   //   if (!err.empty())
   //      std::cerr << "Err: " << err << std::endl;
   //   if (!ret)
   //   {
   //      std::cerr << "Failed to load " << filename << std::endl;
   //      return -1;
   //   }

   //   std::cout << "Loaded glTF file: " << filename << std::endl;

   //   // Print buffers
   //   for (size_t i = 0; i < model.buffers.size(); i++)
   //   {
   //      const auto &buffer = model.buffers[i];
   //      std::cout << "Buffer[" << i << "] size: " << buffer.data.size() << " bytes" << std::endl;
   //   }

   //   // Print images
   //   for (size_t i = 0; i < model.images.size(); i++)
   //   {
   //      const auto &image = model.images[i];
   //      std::cout << "Image[" << i << "] : " << image.uri << " (" << image.width << "x" << image.height
   //                << ", comp=" << image.component << ")" << std::endl;
   //   }

   //   return 0;
   //}


	//VkDescriptorSetLayout  gltf::descriptorSetLayoutImage = VK_NULL_HANDLE;
	//VkDescriptorSetLayout  gltf::descriptorSetLayoutIbl = VK_NULL_HANDLE;
	//VkDescriptorSetLayout  gltf::descriptorSetLayoutUbo = VK_NULL_HANDLE;
	//VkMemoryPropertyFlags  gltf::memoryPropertyFlags = 0;
	//uint32_t gltf::descriptorBindingFlags =  gltf::DescriptorBindingFlags::ImageBaseColor |  gltf::DescriptorBindingFlags::ImageNormalMap;

   namespace gltf
   {
    
      //CLASS_DECL_GPU_VULKAN VkDescriptorSetLayout ubo_descriptor_set_layout()
      //{
      //   
      //   return descriptorSetLayoutUbo;
      //
      //}

      //      CLASS_DECL_GPU_VULKAN VkDescriptorSetLayout image_descriptor_set_layout()
      //      {
      //         return descriptorSetLayoutImage; 
      //      }

   } // 
	////class VkSandboxDevice;
	//

	///*
	//	We use a custom image loading function with tinyglTF, so we can do custom stuff loading ktx textures
	//*/
	//bool loadImageDataFunc(tinygltf::Image* image, const int imageIndex, std::string* error, std::string* warning, int req_width, int req_height, const unsigned char* bytes, int size, void* userData)
	//{
	//	// KTX files will be handled by our own code
	//	if (image->uri.find_last_of(".") != std::string::npos) {
	//		if (image->uri.substr(image->uri.find_last_of(".") + 1) == "ktx") {
	//			return true;
	//		}
	//	}

	//	return tinygltf::LoadImageData(image, imageIndex, error, warning, req_width, req_height, bytes, size, userData);
	//}

	bool loadImageDataFuncEmpty(tinygltf::Image* image, const int imageIndex, std::string* error, std::string* warning, int req_width, int req_height, const unsigned char* bytes, int size, void* userData)
	{
		// This function will be used for samples that don't require images to be loaded
		return true;
	}


	///*
	//	gltf texture loading class
	//*/

	//void gltf::Texture::updateDescriptor()
	//{
	//	descriptor.sampler = sampler;
	//	descriptor.imageView = view;
	//	descriptor.imageLayout = imageLayout;
	//}

	//void gltf::Texture::destroy()
	//{
	//	if (m_pgpucontext)
	//	{
	//		::cast < ::gpu_vulkan::context > pcontext = m_pgpucontext;
	//		vkDestroyImageView(pcontext->logicalDevice(), view, nullptr);
	//		vkDestroyImage(pcontext->logicalDevice(), image, nullptr);
	//		vkFreeMemory(pcontext->logicalDevice(), deviceMemory, nullptr);
	//		vkDestroySampler(pcontext->logicalDevice(), sampler, nullptr);
	//	}
	//}


	gltf::Model::Model()
	{

	}



	/*
		gltf material
	*/
	void gltf::Material::addDescriptorSet(
		VkDescriptorPool descriptorPool,
		VkDescriptorSetLayout descriptorSetLayout,
		uint32_t descriptorBindingFlags,
		gpu_vulkan::texture* fallbackTexture
	) {

		::cast < ::gpu_vulkan::context > pcontext = m_pgpucontext;
		::cast < ::gpu_vulkan::device > pgpudevice = pcontext->m_pgpudevice;
		auto pphysicaldevice = pgpudevice->m_pphysicaldevice;

      VkDescriptorSet descriptorSet = VK_NULL_HANDLE;

		// Allocate descriptor set
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &descriptorSetLayout;
		VK_CHECK_RESULT(vkAllocateDescriptorSets(pcontext->logicalDevice(), &allocInfo, &descriptorSet));

		// Prepare image infos with fallback
		VkDescriptorImageInfo baseColorImageInfo = (baseColorTexture && (descriptorBindingFlags & DescriptorBindingFlags::ImageBaseColor))
			? baseColorTexture->m_descriptor3 : fallbackTexture->m_descriptor3;

		VkDescriptorImageInfo normalImageInfo = (normalTexture && (descriptorBindingFlags & DescriptorBindingFlags::ImageNormalMap))
            ? normalTexture->m_descriptor3
            : fallbackTexture->m_descriptor3;

		VkDescriptorImageInfo metallicRoughnessImageInfo = (metallicRoughnessTexture && (descriptorBindingFlags & DescriptorBindingFlags::ImageMetallicMap))
            ? metallicRoughnessTexture->m_descriptor3
            : fallbackTexture->m_descriptor3;

		// If you have a separate roughness texture, bind it here, otherwise fallback
      VkDescriptorImageInfo roughnessImageInfo = fallbackTexture->m_descriptor3;

		VkDescriptorImageInfo occlusionImageInfo = (occlusionTexture && (descriptorBindingFlags & DescriptorBindingFlags::ImageAOMap))
            ? occlusionTexture->m_descriptor3
            : fallbackTexture->m_descriptor3;

		std::array<VkWriteDescriptorSet, 5> writeDescriptorSets{};

		writeDescriptorSets[0] = {
			VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, descriptorSet, 0, 0, 1,
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &baseColorImageInfo, nullptr, nullptr
		};
		writeDescriptorSets[1] = {
			VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, descriptorSet, 1, 0, 1,
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &normalImageInfo, nullptr, nullptr
		};
		writeDescriptorSets[2] = {
			VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, descriptorSet, 2, 0, 1,
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &metallicRoughnessImageInfo, nullptr, nullptr
		};
		writeDescriptorSets[3] = {
			VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, descriptorSet, 3, 0, 1,
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &roughnessImageInfo, nullptr, nullptr
		};
		writeDescriptorSets[4] = {
			VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, descriptorSet, 4, 0, 1,
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &occlusionImageInfo, nullptr, nullptr
		};

		vkUpdateDescriptorSets(pcontext->logicalDevice(), static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);

      m_descriptorseta.add(descriptorSet);

	}


	/*
		gltf primitive
	*/
	void  gltf::Primitive::setDimensions(glm::vec3 min, glm::vec3 max) {
		dimensions.min = min;
		dimensions.max = max;
		dimensions.size = max - min;
		dimensions.center = (min + max) / 2.0f;
		dimensions.radius = glm::distance(min, max) / 2.0f;
	}

	/*
		gltf mesh
	*/
	gltf::Mesh::Mesh(::gpu::context * pgpucontext, glm::mat4 matrix) {
		this->m_pgpucontext = pgpucontext;
		::cast < ::gpu_vulkan::context > pcontext = m_pgpucontext;
		::cast < ::gpu_vulkan::device > pgpudevice = pcontext->m_pgpudevice;
		auto pphysicaldevice = pgpudevice->m_pphysicaldevice;

		this->uniformBlock.matrix = matrix;
		VK_CHECK_RESULT(pgpudevice->createBuffer(
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			sizeof(uniformBlock),
			&uniformBuffer.buffer,
			&uniformBuffer.memory,
			&uniformBlock));
		VK_CHECK_RESULT(vkMapMemory(pcontext->logicalDevice(), uniformBuffer.memory, 0, sizeof(uniformBlock), 0, &uniformBuffer.mapped));
		uniformBuffer.descriptor = { uniformBuffer.buffer, 0, sizeof(uniformBlock) };
	};

	gltf::Mesh::~Mesh() {
		::cast < ::gpu_vulkan::context > pcontext = m_pgpucontext;
		::cast < ::gpu_vulkan::device > pgpudevice = pcontext->m_pgpudevice;
		auto pphysicaldevice = pgpudevice->m_pphysicaldevice;

		vkDestroyBuffer(pcontext->logicalDevice(), uniformBuffer.buffer, nullptr);
		vkFreeMemory(pcontext->logicalDevice(), uniformBuffer.memory, nullptr);
		for (auto primitive : primitives)
		{
			delete primitive;
		}
	}

	/*
		gltf node
	*/
	glm::mat4 gltf::Node::localMatrix() {
		return glm::translate(glm::mat4(1.0f), translation) * glm::mat4(rotation) * glm::scale(glm::mat4(1.0f), scale) * matrix;
	}

	glm::mat4 gltf::Node::getMatrix() {
		glm::mat4 m = localMatrix();
		Node* p = parent;
		while (p) {
			m = p->localMatrix() * m;
			p = p->parent;
		}
		return m;
	}

	void gltf::Node::update() {
		if (mesh) {
			glm::mat4 m = getMatrix();
			if (skin) {
				mesh->uniformBlock.matrix = m;
				// Update join matrices
				glm::mat4 inverseTransform = glm::inverse(m);
				for (size_t i = 0; i < skin->joints.size(); i++) {
					Node* jointNode = skin->joints[i];
					glm::mat4 jointMat = jointNode->getMatrix() * skin->inverseBindMatrices[i];
					jointMat = inverseTransform * jointMat;
					mesh->uniformBlock.jointMatrix[i] = jointMat;
				}
				mesh->uniformBlock.jointcount = (float)skin->joints.size();
				memcpy(mesh->uniformBuffer.mapped, &mesh->uniformBlock, sizeof(mesh->uniformBlock));
			}
			else {
				memcpy(mesh->uniformBuffer.mapped, &m, sizeof(glm::mat4));
			}
		}

		for (auto& child : children) {
			child->update();
		}
	}

	gltf::Node::~Node() {
		if (mesh) {
			delete mesh;
		}
		for (auto& child : children) {
			delete child;
		}
	}

	/*
		gltf default vertex layout with easy Vulkan mapping functions
	*/

	VkVertexInputBindingDescription   gltf::Vertex::vertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription>   gltf::Vertex::vertexInputAttributeDescriptions;
	VkPipelineVertexInputStateCreateInfo   gltf::Vertex::pipelineVertexInputStateCreateInfo;

	VkVertexInputBindingDescription   gltf::Vertex::inputBindingDescription(uint32_t binding) {
		return VkVertexInputBindingDescription({ binding, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX });
	}

	VkVertexInputAttributeDescription   gltf::Vertex::inputAttributeDescription(uint32_t binding, uint32_t location, VertexComponent component) {
		switch (component) {
		case VertexComponent::Position:
			return VkVertexInputAttributeDescription({ location, binding, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos) });
		case VertexComponent::Normal:
			return VkVertexInputAttributeDescription({ location, binding, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal) });
		case VertexComponent::UV:
			return VkVertexInputAttributeDescription({ location, binding, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv) });
		case VertexComponent::Color:
			return VkVertexInputAttributeDescription({ location, binding, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, color) });
		case VertexComponent::Tangent:
			return VkVertexInputAttributeDescription({ location, binding, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, tangent) });
		case VertexComponent::Joint0:
			return VkVertexInputAttributeDescription({ location, binding, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, joint0) });
		case VertexComponent::Weight0:
			return VkVertexInputAttributeDescription({ location, binding, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, weight0) });
		default:
			return VkVertexInputAttributeDescription({});
		}
	}

	std::vector<VkVertexInputAttributeDescription>   gltf::Vertex::inputAttributeDescriptions(uint32_t binding, const std::vector<VertexComponent> components) {
		std::vector<VkVertexInputAttributeDescription> result;
		uint32_t location = 0;
		for (VertexComponent component : components) {
			result.push_back(Vertex::inputAttributeDescription(binding, location, component));
			location++;
		}
		return result;
	}

	/** @brief Returns the default pipeline vertex input state create info structure for the requested vertex components */
	VkPipelineVertexInputStateCreateInfo* gltf::Vertex::getPipelineVertexInputState(const std::vector<VertexComponent> components) {
		vertexInputBindingDescription = Vertex::inputBindingDescription(0);
		Vertex::vertexInputAttributeDescriptions = Vertex::inputAttributeDescriptions(0, components);
		pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
		pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = &Vertex::vertexInputBindingDescription;
		pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(Vertex::vertexInputAttributeDescriptions.size());
		pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = Vertex::vertexInputAttributeDescriptions.data();
		return &pipelineVertexInputStateCreateInfo;
	}

	gpu_vulkan::texture* gltf::Model::getTexture(uint32_t index)
	{

		if (index < m_textures.size()) {
			return m_textures[index];
		}
		return nullptr;
	}

	void gltf::Model::createEmptyTexture(VkQueue transferQueue)
	{

      øconstruct_new(emptyTexture);

		// Define the static member here (outside any class/function)

		emptyTexture->m_pgpurenderer = m_pgpucontext->m_pgpurenderer;
      emptyTexture->m_rectangleTarget.set_width(1);
      emptyTexture->m_rectangleTarget.set_height(1);
      emptyTexture->m_iLayerCount = 1;
		emptyTexture->m_mipsLevel = 1;

		::cast < ::gpu_vulkan::context > pcontext = m_pgpucontext;
		::cast < ::gpu_vulkan::device > pgpudevice = pcontext->m_pgpudevice;
		auto pphysicaldevice = pgpudevice->m_pphysicaldevice;


		size_t bufferSize = emptyTexture->m_rectangleTarget.area() * 4;
		unsigned char* buffer = new unsigned char[bufferSize];
		memset(buffer, 0, bufferSize);
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingMemory;
		VkBufferCreateInfo bufferCreateInfo = vkinit::bufferCreateInfo();
		bufferCreateInfo.size = bufferSize;
		// This buffer is used as a transfer source for the buffer copy
		bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		VK_CHECK_RESULT(vkCreateBuffer(pcontext->logicalDevice(), &bufferCreateInfo, nullptr, &stagingBuffer));

		VkMemoryAllocateInfo memAllocInfo = vkinit::memoryAllocateInfo();
		VkMemoryRequirements memReqs;
		vkGetBufferMemoryRequirements(pcontext->logicalDevice(), stagingBuffer, &memReqs);
		memAllocInfo.allocationSize = memReqs.size;
		memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		VK_CHECK_RESULT(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &stagingMemory));
		VK_CHECK_RESULT(vkBindBufferMemory(pcontext->logicalDevice(), stagingBuffer, stagingMemory, 0));

		// Copy texture data into staging buffer
		uint8_t* data;
		VK_CHECK_RESULT(vkMapMemory(pcontext->logicalDevice(), stagingMemory, 0, memReqs.size, 0, (void**)&data));
		memcpy(data, buffer, bufferSize);
		vkUnmapMemory(pcontext->logicalDevice(), stagingMemory);

		VkBufferImageCopy bufferCopyRegion = {};
		bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		bufferCopyRegion.imageSubresource.layerCount = 1;
		bufferCopyRegion.imageExtent.width = emptyTexture->m_rectangleTarget.width();
		bufferCopyRegion.imageExtent.height = emptyTexture->m_rectangleTarget.height();
		bufferCopyRegion.imageExtent.depth = 1;

		// Create optimal tiled target image
		VkImageCreateInfo imageCreateInfo = vkinit::imageCreateInfo();
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
		imageCreateInfo.mipLevels = 1;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      imageCreateInfo.extent.width = emptyTexture->m_rectangleTarget.width();
      imageCreateInfo.extent.height = emptyTexture->m_rectangleTarget.height();
      imageCreateInfo.extent.depth = 1;
      imageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		VK_CHECK_RESULT(vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &emptyTexture->m_vkimage));

		vkGetImageMemoryRequirements(pcontext->logicalDevice(), emptyTexture->m_vkimage, &memReqs);
		memAllocInfo.allocationSize = memReqs.size;
		memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		VK_CHECK_RESULT(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &emptyTexture->m_vkdevicememory));
      VK_CHECK_RESULT(
         vkBindImageMemory(pcontext->logicalDevice(), emptyTexture->m_vkimage, emptyTexture->m_vkdevicememory, 0));

		VkImageSubresourceRange subresourceRange{};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.layerCount = 1;

		//VkCommandBuffer pcommandbufferCopy->m_vkcommandbuffer = pcontext->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

	   auto pgpucommandbufferCopy = pcontext->beginSingleTimeCommands(pcontext->transfer_queue());

	   ::cast < command_buffer > pcommandbufferCopy = pgpucommandbufferCopy;

		vulkan::setImageLayout(pcommandbufferCopy->m_vkcommandbuffer, emptyTexture->m_vkimage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourceRange);
		vkCmdCopyBufferToImage(pcommandbufferCopy->m_vkcommandbuffer, stagingBuffer, emptyTexture->m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferCopyRegion);
		vulkan::setImageLayout(pcommandbufferCopy->m_vkcommandbuffer, emptyTexture->m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, subresourceRange);
		//pcontext->flushCommandBuffer(pcommandbufferCopy->m_vkcommandbuffer, transferQueue);
	   pcontext->endSingleTimeCommands(pcommandbufferCopy);
		emptyTexture->m_state.m_vkimagelayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		// Clean up staging resources
		vkDestroyBuffer(pcontext->logicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(pcontext->logicalDevice(), stagingMemory, nullptr);

		VkSamplerCreateInfo samplerCreateInfo = vkinit::samplerCreateInfo();
		samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
		samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
		samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
		samplerCreateInfo.maxAnisotropy = 1.0f;
		VK_CHECK_RESULT(vkCreateSampler(pcontext->logicalDevice(), &samplerCreateInfo, nullptr, &emptyTexture->m_vksampler3));

		VkImageViewCreateInfo viewCreateInfo = vkinit::imageViewCreateInfo();
		viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
		viewCreateInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
		viewCreateInfo.subresourceRange.levelCount = 1;
		viewCreateInfo.image = emptyTexture->m_vkimage;
		VK_CHECK_RESULT(vkCreateImageView(pcontext->logicalDevice(), &viewCreateInfo, nullptr, &emptyTexture->m_vkimageview));

		emptyTexture->m_descriptor3.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      emptyTexture->m_descriptor3.imageView = emptyTexture->m_vkimageview;
      emptyTexture->m_descriptor3.sampler = emptyTexture->m_vksampler3;
	}

	/*
		gltf model loading and rendering class
	*/
	gltf::Model::~Model()
	{


		::cast < ::gpu_vulkan::context > pcontext = m_pgpucontext;
		::cast < ::gpu_vulkan::device > pgpudevice = pcontext->m_pgpudevice;
		auto pphysicaldevice = pgpudevice->m_pphysicaldevice;

		vkDestroyBuffer(pcontext->logicalDevice(), vertices.buffer, nullptr);
		vkFreeMemory(pcontext->logicalDevice(), vertices.memory, nullptr);
		vkDestroyBuffer(pcontext->logicalDevice(), indices.buffer, nullptr);
		vkFreeMemory(pcontext->logicalDevice(), indices.memory, nullptr);
		//for (auto texture : m_textures) {
		//	texture.destroy();
		//}
		for (auto node : m_nodes) {
			delete node;
		}
		for (auto skin : m_skins) {
			delete skin;
		}
		//if (descriptorSetLayoutUbo != VK_NULL_HANDLE) {
		//	vkDestroyDescriptorSetLayout(pcontext->logicalDevice(), descriptorSetLayoutUbo, nullptr);
		//	descriptorSetLayoutUbo = VK_NULL_HANDLE;
		//}
		//if (descriptorSetLayoutImage != VK_NULL_HANDLE) {
		//	vkDestroyDescriptorSetLayout(pcontext->logicalDevice(), descriptorSetLayoutImage, nullptr);
		//	descriptorSetLayoutImage = VK_NULL_HANDLE;
		//}
		vkDestroyDescriptorPool(pcontext->logicalDevice(), m_descriptorPool, nullptr);
		emptyTexture->destroy();
	}

	void   gltf::Model::loadNode( Node* parent, const tinygltf::Node& node, uint32_t nodeIndex, const tinygltf::Model& model, std::vector<uint32_t>& indexBuffer, std::vector<Vertex>& vertexBuffer, float globalscale)
	{
		Node* newNode = new Node{};
		newNode->index = nodeIndex;
		newNode->parent = parent;
		newNode->name = node.name;
		newNode->skinIndex = node.skin;
		newNode->matrix = glm::mat4(1.0f);


		::cast < ::gpu_vulkan::context > pcontext = m_pgpucontext;
		::cast < ::gpu_vulkan::device > pgpudevice = pcontext->m_pgpudevice;
		auto pphysicaldevice = pgpudevice->m_pphysicaldevice;


		// Generate local node matrix
		glm::vec3 translation = glm::vec3(0.0f);
		if (node.translation.size() == 3) {
			translation = glm::make_vec3(node.translation.data());
			newNode->translation = translation;
		}
		glm::mat4 rotation = glm::mat4(1.0f);
		if (node.rotation.size() == 4) {
			glm::quat q = glm::make_quat(node.rotation.data());
			newNode->rotation = glm::mat4(q);
		}
		glm::vec3 scale = glm::vec3(1.0f);
		if (node.scale.size() == 3) {
			scale = glm::make_vec3(node.scale.data());
			newNode->scale = scale;
		}
		if (node.matrix.size() == 16) {
			newNode->matrix = glm::make_mat4x4(node.matrix.data());
			if (globalscale != 1.0f) {
				//newNode->matrix = glm::scale(newNode->matrix, glm::vec3(globalscale));
			}
		};

		// Node with children
		if (node.children.size() > 0) {
			for (auto i = 0; i < node.children.size(); i++) {
				loadNode(newNode, model.nodes[node.children[i]], node.children[i], model, indexBuffer, vertexBuffer, globalscale);
			}
		}

		// Node contains mesh data
		if (node.mesh > -1) {
			const tinygltf::Mesh mesh = model.meshes[node.mesh];
			Mesh* newMesh = new Mesh(pcontext, newNode->matrix);
			newMesh->name = mesh.name;
			for (size_t j = 0; j < mesh.primitives.size(); j++) {
				const tinygltf::Primitive& primitive = mesh.primitives[j];
				if (primitive.indices < 0) {
					continue;
				}
				uint32_t indexStart = static_cast<uint32_t>(indexBuffer.size());
				uint32_t vertexStart = static_cast<uint32_t>(vertexBuffer.size());
				uint32_t indexCount = 0;
				uint32_t vertexCount = 0;
				glm::vec3 posMin{};
				glm::vec3 posMax{};
				bool hasSkin = false;
				// Vertices
				{
					const float* bufferPos = nullptr;
					const float* bufferNormals = nullptr;
					const float* bufferTexCoords = nullptr;
					const float* bufferColors = nullptr;
					const float* bufferTangents = nullptr;
					uint32_t numColorComponents;
					const uint16_t* bufferJoints = nullptr;
					const float* bufferWeights = nullptr;

					// Position attribute is required
					assert(primitive.attributes.find("POSITION") != primitive.attributes.end());

					const tinygltf::Accessor& posAccessor = model.accessors[primitive.attributes.find("POSITION")->second];
					const tinygltf::BufferView& posView = model.bufferViews[posAccessor.bufferView];
					bufferPos = reinterpret_cast<const float*>(&(model.buffers[posView.buffer].data[posAccessor.byteOffset + posView.byteOffset]));
					posMin = glm::vec3(posAccessor.minValues[0], posAccessor.minValues[1], posAccessor.minValues[2]);
					posMax = glm::vec3(posAccessor.maxValues[0], posAccessor.maxValues[1], posAccessor.maxValues[2]);

					if (primitive.attributes.find("NORMAL") != primitive.attributes.end()) {
						const tinygltf::Accessor& normAccessor = model.accessors[primitive.attributes.find("NORMAL")->second];
						const tinygltf::BufferView& normView = model.bufferViews[normAccessor.bufferView];
						bufferNormals = reinterpret_cast<const float*>(&(model.buffers[normView.buffer].data[normAccessor.byteOffset + normView.byteOffset]));
					}

					if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
						const tinygltf::Accessor& uvAccessor = model.accessors[primitive.attributes.find("TEXCOORD_0")->second];
						const tinygltf::BufferView& uvView = model.bufferViews[uvAccessor.bufferView];
						bufferTexCoords = reinterpret_cast<const float*>(&(model.buffers[uvView.buffer].data[uvAccessor.byteOffset + uvView.byteOffset]));
					}

					if (primitive.attributes.find("COLOR_0") != primitive.attributes.end())
					{
						const tinygltf::Accessor& colorAccessor = model.accessors[primitive.attributes.find("COLOR_0")->second];
						const tinygltf::BufferView& colorView = model.bufferViews[colorAccessor.bufferView];
						// Color buffer are either of type vec3 or vec4
						numColorComponents = colorAccessor.type == TINYGLTF_PARAMETER_TYPE_FLOAT_VEC3 ? 3 : 4;
						bufferColors = reinterpret_cast<const float*>(&(model.buffers[colorView.buffer].data[colorAccessor.byteOffset + colorView.byteOffset]));
					}

					if (primitive.attributes.find("TANGENT") != primitive.attributes.end())
					{
						const tinygltf::Accessor& tangentAccessor = model.accessors[primitive.attributes.find("TANGENT")->second];
						const tinygltf::BufferView& tangentView = model.bufferViews[tangentAccessor.bufferView];
						bufferTangents = reinterpret_cast<const float*>(&(model.buffers[tangentView.buffer].data[tangentAccessor.byteOffset + tangentView.byteOffset]));
					}

					// Skinning
					// Joints
					if (primitive.attributes.find("JOINTS_0") != primitive.attributes.end()) {
						const tinygltf::Accessor& jointAccessor = model.accessors[primitive.attributes.find("JOINTS_0")->second];
						const tinygltf::BufferView& jointView = model.bufferViews[jointAccessor.bufferView];
						bufferJoints = reinterpret_cast<const uint16_t*>(&(model.buffers[jointView.buffer].data[jointAccessor.byteOffset + jointView.byteOffset]));
					}

					if (primitive.attributes.find("WEIGHTS_0") != primitive.attributes.end()) {
						const tinygltf::Accessor& uvAccessor = model.accessors[primitive.attributes.find("WEIGHTS_0")->second];
						const tinygltf::BufferView& uvView = model.bufferViews[uvAccessor.bufferView];
						bufferWeights = reinterpret_cast<const float*>(&(model.buffers[uvView.buffer].data[uvAccessor.byteOffset + uvView.byteOffset]));
					}

					hasSkin = (bufferJoints && bufferWeights);

					vertexCount = static_cast<uint32_t>(posAccessor.count);

					for (size_t v = 0; v < posAccessor.count; v++) {
						Vertex vert{};
						vert.pos = glm::vec4(glm::make_vec3(&bufferPos[v * 3]), 1.0f);
						vert.normal = glm::normalize(glm::vec3(bufferNormals ? glm::make_vec3(&bufferNormals[v * 3]) : glm::vec3(0.0f)));
						vert.uv = bufferTexCoords ? glm::make_vec2(&bufferTexCoords[v * 2]) : glm::vec3(0.0f);
						if (bufferColors) {
							switch (numColorComponents) {
							case 3:
								vert.color = glm::vec4(glm::make_vec3(&bufferColors[v * 3]), 1.0f);
							case 4:
								vert.color = glm::make_vec4(&bufferColors[v * 4]);
							}
						}
						else {
							vert.color = glm::vec4(1.0f);
						}
						vert.tangent = bufferTangents ? glm::vec4(glm::make_vec4(&bufferTangents[v * 4])) : glm::vec4(0.0f);
						vert.joint0 = hasSkin ? glm::vec4(glm::make_vec4(&bufferJoints[v * 4])) : glm::vec4(0.0f);
						vert.weight0 = hasSkin ? glm::make_vec4(&bufferWeights[v * 4]) : glm::vec4(0.0f);
						vertexBuffer.push_back(vert);
					}
				}
				// Indices
				{
					const tinygltf::Accessor& accessor = model.accessors[primitive.indices];
					const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
					const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

					indexCount = static_cast<uint32_t>(accessor.count);

					switch (accessor.componentType) {
					case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT: {
							uint32_t* buf = new uint32_t[accessor.count];
							memcpy(buf, &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(uint32_t));
							for (size_t index = 0; index < accessor.count; index++) {
								indexBuffer.push_back(buf[index] + vertexStart);
							}
							delete[] buf;
							break;
					}
					case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT: {
							uint16_t* buf = new uint16_t[accessor.count];
							memcpy(buf, &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(uint16_t));
							for (size_t index = 0; index < accessor.count; index++) {
								indexBuffer.push_back(buf[index] + vertexStart);
							}
							delete[] buf;
							break;
					}
					case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE: {
							uint8_t* buf = new uint8_t[accessor.count];
							memcpy(buf, &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(uint8_t));
							for (size_t index = 0; index < accessor.count; index++) {
								indexBuffer.push_back(buf[index] + vertexStart);
							}
							delete[] buf;
							break;
					}
					default:
						error() << "Index component type " << accessor.componentType << " not supported!";
						return;
					}
				}
				Primitive* newPrimitive = new Primitive(indexStart, indexCount, primitive.material > -1 ? &m_materials[primitive.material] : &m_materials.back());
				newPrimitive->firstVertex = vertexStart;
				newPrimitive->vertexCount = vertexCount;
				newPrimitive->setDimensions(posMin, posMax);
				newMesh->primitives.push_back(newPrimitive);
			}
			newNode->mesh = newMesh;
		}
		if (parent) {
			parent->children.push_back(newNode);
		}
		else {
			m_nodes.push_back(newNode);
		}
		m_linearNodes.push_back(newNode);
	}

	void   gltf::Model::loadSkins(tinygltf::Model& gltfModel)
	{
		for (tinygltf::Skin& source : gltfModel.skins) {
			Skin* newSkin = new Skin{};
			newSkin->name = source.name;

			// Find skeleton root node
			if (source.skeleton > -1) {
				newSkin->skeletonRoot = nodeFromIndex(source.skeleton);
			}

			// Find joint nodes
			for (int jointIndex : source.joints) {
				Node* node = nodeFromIndex(jointIndex);
				if (node) {
					newSkin->joints.push_back(nodeFromIndex(jointIndex));
				}
			}

			// Get inverse bind matrices from buffer
			if (source.inverseBindMatrices > -1) {
				const tinygltf::Accessor& accessor = gltfModel.accessors[source.inverseBindMatrices];
				const tinygltf::BufferView& bufferView = gltfModel.bufferViews[accessor.bufferView];
				const tinygltf::Buffer& buffer = gltfModel.buffers[bufferView.buffer];
				newSkin->inverseBindMatrices.resize(accessor.count);
				memcpy(newSkin->inverseBindMatrices.data(), &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(glm::mat4));
			}

			m_skins.push_back(newSkin);
		}
	}

	
   void gltf::Model::loadImages(tinygltf::Model& gltfModel, ::gpu::context * pgpucontext, VkQueue transferQueue)
	{
		
      for (tinygltf::Image& image : gltfModel.images) 
      {
		   ::pointer < ::gpu_vulkan::texture > ptexture;
         øconstruct_new(ptexture);
         ptexture->_fromglTfImage(&image, m_pathRenderable.folder(), pgpucontext->m_pgpurenderer, false);
         ptexture->m_iIndex = m_textures.size();
         m_textures.add(ptexture);

         //auto memory = file()->as_memory(image.m_path.c_str());
         //texture.fromglTfImage(image, memory.data(), memory.size(), pgpucontext, transferQueue, false);
			//texture.index = static_cast<uint32_t>(m_textures.size());
			//m_textures.push_back(texture);
		}
		// Create an empty texture to be used for empty material images
		createEmptyTexture(transferQueue);
		emptyTexture->m_descriptor3.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	}

   ::gpu::texture * gltf::Model::loadMaterialTexture2(const ::scoped_string & scopedstr,
                                                     tinygltf::Material & material, tinygltf::Model &gltfModel,
                                                     const ::scoped_string &scopedstrType,
                               aiTextureType type)
   {

      auto ptexture = loadMaterialTexture(scopedstr, type);
      auto iIndex = m_textures.add(ptexture);
      gltfModel.textures.resize(iIndex + 1);
      auto &texture = gltfModel.textures[iIndex];
      texture.name = scopedstr;
      texture.source = iIndex;
      material.values[scopedstrType.c_str()].json_double_value["index"] = (int)iIndex;
      return ptexture;
   }

	void gltf::Model::loadMaterials(tinygltf::Model& gltfModel)
	{
		for (tinygltf::Material& mat : gltfModel.materials) {
			Material material(m_pgpucontext);

         if (m_bExternalPbr)
         {
            material.baseColorTexture = 
               loadMaterialTexture2(
                  "albedo.ktx",
                  mat, 
                  gltfModel, 
                  "baseColorTexture",
                  aiTextureType_DIFFUSE);
            //auto iIndex= m_textures.add(material.baseColorTexture);
            //gltfModel.textures.resize(iIndex + 1);
            //auto &texture = gltfModel.textures[iIndex];
            //texture.name = "albedo.ktx";
            //texture.source = iIndex;
            //mat.values[].json_double_value["index"] = (int)iIndex;
         }
         else if (mat.values.find("baseColorTexture") != mat.values.end()) {
				material.baseColorTexture = getTexture(gltfModel.textures[mat.values["baseColorTexture"].TextureIndex()].source);
			}
			// Metallic roughness workflow
         if (m_bExternalPbr)
         {
            material.metallicRoughnessTexture
                = loadMaterialTexture2("metallic.ktx", mat, gltfModel, "metallicRoughnessTexture", aiTextureType_UNKNOWN);
         }
			else if (mat.values.find("metallicRoughnessTexture") != mat.values.end()) {
				material.metallicRoughnessTexture = getTexture(gltfModel.textures[mat.values["metallicRoughnessTexture"].TextureIndex()].source);
			}
			if (mat.values.find("roughnessFactor") != mat.values.end()) {
				material.roughnessFactor = static_cast<float>(mat.values["roughnessFactor"].Factor());
			}
			if (mat.values.find("metallicFactor") != mat.values.end()) {
				material.metallicFactor = static_cast<float>(mat.values["metallicFactor"].Factor());
			}
			if (mat.values.find("baseColorFactor") != mat.values.end()) {
				material.baseColorFactor = glm::make_vec4(mat.values["baseColorFactor"].ColorFactor().data());
			}
         if (m_bExternalPbr)
         {
            material.normalTexture =
               loadMaterialTexture2("normal.ktx", mat, gltfModel,
                                                            "normalTexture", aiTextureType_NORMALS);

         }
         else
         {
            if (mat.additionalValues.find("normalTexture") != mat.additionalValues.end())
            {
               material.normalTexture =
                  getTexture(gltfModel.textures[mat.additionalValues["normalTexture"].TextureIndex()].source);
            }
         }
         if (m_bExternalPbr)
         {
            material.emissiveTexture =
               loadMaterialTexture2("emissive.ktx", mat, gltfModel, "emissiveTexture", aiTextureType_EMISSIVE);
         }
         else if (mat.additionalValues.find("emissiveTexture") != mat.additionalValues.end())
         {
				material.emissiveTexture = getTexture(gltfModel.textures[mat.additionalValues["emissiveTexture"].TextureIndex()].source);
			}
         if (m_bExternalPbr)
         {
            material.occlusionTexture =
               loadMaterialTexture2("ao.ktx", mat, gltfModel, "occlusionTexture", aiTextureType_LIGHTMAP);
         }
         else if (mat.additionalValues.find("occlusionTexture") != mat.additionalValues.end())
         {
				material.occlusionTexture = getTexture(gltfModel.textures[mat.additionalValues["occlusionTexture"].TextureIndex()].source);
			}
			material.alphaMode = Material::ALPHAMODE_OPAQUE;

			auto itAlpha = mat.additionalValues.find("alphaMode");
			if (itAlpha != mat.additionalValues.end()) {
				const std::string& mode = itAlpha->second.string_value;
				if (mode == "BLEND") {
					material.alphaMode = Material::ALPHAMODE_BLEND;
				}
				else if (mode == "MASK") {
					material.alphaMode = Material::ALPHAMODE_MASK;
				}
				// else leave as OPAQUE
			}
			if (mat.additionalValues.find("alphaCutoff") != mat.additionalValues.end()) {
				material.alphaCutoff = static_cast<float>(mat.additionalValues["alphaCutoff"].Factor());
			}

			m_materials.push_back(material);
		}
		// Push a default material at the end of the list for meshes with no material assigned
		m_materials.push_back(Material(m_pgpucontext));
	}


   //loader.SetImageLoader(
   //   [](tinygltf::Image *image, int index, std::string *err, std::string *warn, int req_width, int req_height,
   //      const unsigned char *bytes, int size, void *user_data)
   //   {
   //      // You can decode image bytes here with stb_image or your own decoder
   //      // Example: load with stb_image
   //      int w, h, comp;
   //      unsigned char *data = stbi_load_from_memory(bytes, size, &w, &h, &comp, req_width > 0 ? req_width : 0);
   //      if (!data)
   //      {
   //         if (err)
   //            *err = "Failed to load image";
   //         return false;
   //      }
   //      image->width = w;
   //      image->height = h;
   //      image->component = comp;
   //      image->image.assign(data, data + w * h * comp);
   //      stbi_image_free(data);
   //      return true;
   //   },
   //   nullptr);


   //loader.SetFsCallbacks({// fileExists
   //                       [](const std::string &path, void *) { return std::filesystem::exists(path); },
   //                       // readWholeFile
   //                       [](std::vector<unsigned char> *out, std::string *err, const std::string &path, void *)
   //                       {
   //                          std::ifstream f(path, std::ios::binary);
   //                          if (!f)
   //                          {
   //                             if (err)
   //                                *err = "Could not open: " + path;
   //                             return false;
   //                          }
   //                          f.seekg(0, std::ios::end);
   //                          size_t size = f.tellg();
   //                          f.seekg(0);
   //                          out->resize(size);
   //                          f.read(reinterpret_cast<char *>(out->data()), size);
   //                          return true;
   //                       },
   //                       // writeWholeFile (rarely needed)
   //                       nullptr, nullptr});


	void  gltf::Model::loadFromFile(std::string filename, ::gpu::context* pgpucontext, VkQueue transferQueue, uint32_t fileLoadingFlags, float scale)
	{

		tinygltf::Model gltfModel;

		tinygltf::TinyGLTF gltfContext;

		if (fileLoadingFlags & FileLoadingFlags::DontLoadImages) {
			gltfContext.SetImageLoader(loadImageDataFuncEmpty, nullptr);
		}
		else {
			gltfContext.SetImageLoader(tinygltf_LoadImageData, image());
		}
  //    if (fileLoadingFlags & FileLoadingFlags::UseFsCallbacks)
  //    {

             // Set custom filesystem callbacks
         tinygltf::FsCallbacks fsCallbacks;
         fsCallbacks.FileExists = tinygltf_FileExists;
         fsCallbacks.ReadWholeFile = tinygltf_ReadWholeFile;
         fsCallbacks.WriteWholeFile = tinygltf_WriteWholeFile;
         fsCallbacks.ExpandFilePath = tinygltf_ExpandFilePath; // use default
         fsCallbacks.user_data = file();
         gltfContext.SetFsCallbacks(fsCallbacks);
         
      //}
#if defined(__ANDROID__)
		// On Android all assets are packed with the apk in a compressed form, so we need to open them using the asset manager
		// We let tinygltf handle this, by passing the asset manager of our app
		tinygltf::asset_manager = androidApp->activity->assetManager;
#endif
		size_t pos = filename.find_last_of('/');
		m_path1 = filename.substr(0, pos);

      m_strDirectory = m_path1.c_str();

		std::string error, warning;

		this->m_pgpucontext = pgpucontext;

#if defined(__ANDROID__)
		// On Android all assets are packed with the apk in a compressed form, so we need to open them using the asset manager
		// We let tinygltf handle this, by passing the asset manager of our app
		tinygltf::asset_manager = androidApp->activity->assetManager;
#endif
		bool fileLoaded = gltfContext.LoadASCIIFromFile(&gltfModel, &error, &warning, filename);

		std::vector<uint32_t> indexBuffer;
		std::vector<Vertex> vertexBuffer;

		if (fileLoaded) {
			if (!(fileLoadingFlags & FileLoadingFlags::DontLoadImages)) {
				loadImages(gltfModel, pgpucontext, transferQueue);
			}
			loadMaterials(gltfModel);
			const tinygltf::Scene& scene = gltfModel.scenes[gltfModel.defaultScene > -1 ? gltfModel.defaultScene : 0];
			for (size_t i = 0; i < scene.nodes.size(); i++) {
				const tinygltf::Node node = gltfModel.nodes[scene.nodes[i]];
				loadNode(nullptr, node, scene.nodes[i], gltfModel, indexBuffer, vertexBuffer, scale);
			}
			if (gltfModel.animations.size() > 0) {
				loadAnimations(gltfModel);
			}
			loadSkins(gltfModel);

			for (auto node : m_linearNodes) {
				// Assign skins
				if (node->skinIndex > -1) {
					node->skin = m_skins[node->skinIndex];
				}
				// Initial pose
				if (node->mesh) {
					node->update();
				}
			}
		}
		else
		{

			::string strMessage;

			strMessage <<  "Could not load gltf file \"" << filename.c_str() << "\": " << error.c_str();

			throw ::exception(error_failed, strMessage);

			return;

		}

		// Pre-Calculations for requested features
		if ((fileLoadingFlags & FileLoadingFlags::PreTransformVertices) || (fileLoadingFlags & FileLoadingFlags::PreMultiplyVertexColors) || (fileLoadingFlags & FileLoadingFlags::FlipY)) {
			const bool preTransform = fileLoadingFlags & FileLoadingFlags::PreTransformVertices;
			const bool preMultiplyColor = fileLoadingFlags & FileLoadingFlags::PreMultiplyVertexColors;
			const bool flipY = fileLoadingFlags & FileLoadingFlags::FlipY;
			for (Node* node : m_linearNodes) {
				if (node->mesh) {
					const glm::mat4 localMatrix = node->getMatrix();
					for (Primitive* primitive : node->mesh->primitives) {
						for (uint32_t i = 0; i < primitive->vertexCount; i++) {
							Vertex& vertex = vertexBuffer[primitive->firstVertex + i];
							// Pre-transform vertex positions by node-hierarchy
							if (preTransform) {
								vertex.pos = glm::vec3(localMatrix * glm::vec4(vertex.pos, 1.0f));
								vertex.normal = glm::normalize(glm::mat3(localMatrix) * vertex.normal);
							}
							// Flip Y-Axis of vertex positions
							if (flipY) {
								vertex.pos.y *= -1.0f;
								vertex.normal.y *= -1.0f;
							}
							// Pre-Multiply vertex colors with material base color
							if (preMultiplyColor) {
								vertex.color = primitive->m_pmaterial->baseColorFactor * vertex.color;
							}
						}
					}
				}
			}
		}

		for (auto extension : gltfModel.extensionsUsed) {
			if (extension == "KHR_materials_pbrSpecularGlossiness") {
				information() << "Required extension: " << extension.c_str();
				m_bMetallicRoughnessWorkflow = false;
			}
		}

		size_t vertexBufferSize = vertexBuffer.size() * sizeof(Vertex);
		size_t indexBufferSize = indexBuffer.size() * sizeof(uint32_t);
		indices.count = static_cast<uint32_t>(indexBuffer.size());
		vertices.count = static_cast<uint32_t>(vertexBuffer.size());

		assert((vertexBufferSize > 0) && (indexBufferSize > 0));

		struct StagingBuffer {
			VkBuffer buffer;
			VkDeviceMemory memory;
		} vertexStaging, indexStaging;


		::cast < ::gpu_vulkan::context > pcontext = m_pgpucontext;
		::cast < ::gpu_vulkan::device > pgpudevice = pcontext->m_pgpudevice;
		auto pphysicaldevice = pgpudevice->m_pphysicaldevice;


		// Create staging buffers
		// Vertex data
		VK_CHECK_RESULT(pgpudevice->createBuffer(
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			vertexBufferSize,
			&vertexStaging.buffer,
			&vertexStaging.memory,
			vertexBuffer.data()));
		// Index data
		VK_CHECK_RESULT(pgpudevice->createBuffer(
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			indexBufferSize,
			&indexStaging.buffer,
			&indexStaging.memory,
			indexBuffer.data()));
      VkMemoryPropertyFlags memoryPropertyFlags = 0;
      
		// Create device local buffers
		// Vertex buffer
		VK_CHECK_RESULT(pgpudevice->createBuffer(
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | memoryPropertyFlags,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			vertexBufferSize,
			&vertices.buffer,
			&vertices.memory));
		// Index buffer
		VK_CHECK_RESULT(pgpudevice->createBuffer(
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | memoryPropertyFlags,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			indexBufferSize,
			&indices.buffer,
			&indices.memory));


		// Copy from staging buffers
		auto pgpucommandbufferCopy = pcontext->beginSingleTimeCommands(pcontext->transfer_queue());

		::cast < ::gpu_vulkan::command_buffer > pcommandbufferCopy = pgpucommandbufferCopy;

		VkBufferCopy copyRegion = {};

		copyRegion.size = vertexBufferSize;
		vkCmdCopyBuffer(pcommandbufferCopy->m_vkcommandbuffer, vertexStaging.buffer, vertices.buffer, 1, &copyRegion);

		copyRegion.size = indexBufferSize;
		vkCmdCopyBuffer(pcommandbufferCopy->m_vkcommandbuffer, indexStaging.buffer, indices.buffer, 1, &copyRegion);

		//pcontext->flushCommandBuffer(pcommandbufferCopy->m_vkcommandbuffer, transferQueue, true);

		pcontext->endSingleTimeCommands(pcommandbufferCopy);

		vkDestroyBuffer(pcontext->logicalDevice(), vertexStaging.buffer, nullptr);
		vkFreeMemory(pcontext->logicalDevice(), vertexStaging.memory, nullptr);
		vkDestroyBuffer(pcontext->logicalDevice(), indexStaging.buffer, nullptr);
		vkFreeMemory(pcontext->logicalDevice(), indexStaging.memory, nullptr);

		getSceneDimensions();

		// Setup descriptors
		uint32_t uboCount{ 0 };
		uint32_t imageCount{ 0 };
		for (auto node : m_linearNodes) {
			if (node->mesh) {
				uboCount++;
			}
		}
		for (auto material : m_materials) {
			if (material.baseColorTexture != nullptr) {
				imageCount++;
			}
		}
		uint32_t materialCount = 0;
		for (auto& m : m_materials) {
			if (m.baseColorTexture) ++materialCount;
		}

		std::vector<VkDescriptorPoolSize> poolSizes;

		if (uboCount > 0) {
			poolSizes.push_back({ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, uboCount });
		}

		uint32_t samplerCount = materialCount * 5 *
         pgpucontext->m_pgpurenderer->m_pgpurendertarget->get_frame_count(); // baseColor + normal per material
		if (samplerCount > 0) {
			poolSizes.push_back({ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, samplerCount });
		}

		VkDescriptorPoolCreateInfo descriptorPoolCI{};
		descriptorPoolCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCI.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		descriptorPoolCI.pPoolSizes = poolSizes.data();
      descriptorPoolCI.maxSets =
         uboCount + materialCount * pgpucontext->m_pgpurenderer->m_pgpurendertarget->get_frame_count();
		VK_CHECK_RESULT(vkCreateDescriptorPool(pcontext->logicalDevice(), &descriptorPoolCI, nullptr, &m_descriptorPool));

		// Descriptors for per-node uniform buffers
		{
			{
				// Layout is global, so only create if it hasn't already been created before
				//if (descriptorSetLayoutUbo == VK_NULL_HANDLE) {
				//	std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings = {
				//		vkinit::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0),
				//	};
				//	VkDescriptorSetLayoutCreateInfo descriptorLayoutCI{};
				//	descriptorLayoutCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				//	descriptorLayoutCI.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
				//	descriptorLayoutCI.pBindings = setLayoutBindings.data();
				//	VK_CHECK_RESULT(vkCreateDescriptorSetLayout(pcontext->logicalDevice(), &descriptorLayoutCI, nullptr, &descriptorSetLayoutUbo));
				//}
				for (auto node : m_nodes) {
               prepareNodeDescriptor(node, pcontext->m_psetdescriptorlayoutGlobal->m_vkdescriptorsetlayout);
				}
			}

			// Descriptors for per-material images
			{
				// Layout is global, so only create if it hasn't already been created before
            if (!pcontext->m_psetdescriptorlayoutGltfImage)
            {
               pcontext->m_psetdescriptorlayoutGltfImage =

                  // binding 0 → baseColor (albedo)
                  // binding 1 → normal map
                  // binding 2 → metallic map
                  // binding 3 → roughness map
                  // binding 4 → ambient occlusion map
                  descriptor_set_layout::Builder(pgpucontext)
                     .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
                     .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
                     .addBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
                     .addBinding(3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
                     .addBinding(4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
                     .build();

					//// Always push exactly two bindings:
					////  • binding 0 = base-color sampler
					////  • binding 1 = normal-map sampler
					//std::array<VkDescriptorSetLayoutBinding, 5> setLayoutBindings = {
					//	// binding 0 → baseColor (albedo)
					//	vkinit::descriptorSetLayoutBinding(
					//		VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
					//		VK_SHADER_STAGE_FRAGMENT_BIT,
					//		/*binding=*/ 0
					//	),
					//		// binding 1 → normal map
					//		vkinit::descriptorSetLayoutBinding(
					//			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
					//			VK_SHADER_STAGE_FRAGMENT_BIT,
					//			/*binding=*/ 1
					//		),
					//		// binding 2 → metallic map
					//		vkinit::descriptorSetLayoutBinding(
					//			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
					//			VK_SHADER_STAGE_FRAGMENT_BIT,
					//			/*binding=*/ 2
					//		),
					//		// binding 3 → roughness map
					//		vkinit::descriptorSetLayoutBinding(
					//			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
					//			VK_SHADER_STAGE_FRAGMENT_BIT,
					//			/*binding=*/ 3
					//		),
					//		// binding 4 → ambient occlusion map
					//		vkinit::descriptorSetLayoutBinding(
					//			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
					//			VK_SHADER_STAGE_FRAGMENT_BIT,
					//			/*binding=*/ 4
					//		)
					//};


					//VkDescriptorSetLayoutCreateInfo descriptorLayoutCI{};
					//descriptorLayoutCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
					//descriptorLayoutCI.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
					//descriptorLayoutCI.pBindings = setLayoutBindings.data();
					//VK_CHECK_RESULT(vkCreateDescriptorSetLayout(
					//	pcontext->logicalDevice(),
					//	&descriptorLayoutCI,
					//	nullptr,
     //             &pcontext->m_psetdescriptorlayoutGltfImage->m_vkdescriptorsetlayout
					//));
				}

				for (auto &material: m_materials)
            {
               uint32_t descriptorBindingFlags = 0;
               if (material.baseColorTexture != nullptr)
               {
                  descriptorBindingFlags |= gltf::DescriptorBindingFlags::ImageBaseColor;
               }
               if (material.metallicRoughnessTexture != nullptr)
               {
                  descriptorBindingFlags |= gltf::DescriptorBindingFlags::ImageMetallicMap;
               }
               if (material.normalTexture != nullptr)
               {
                  descriptorBindingFlags |= gltf::DescriptorBindingFlags::ImageNormalMap;
               }
               if (material.emissiveTexture != nullptr)
               {
                  descriptorBindingFlags |= gltf::DescriptorBindingFlags::ImageEmissiveMap;
               }
               if (material.occlusionTexture != nullptr)
               {
                  descriptorBindingFlags |= gltf::DescriptorBindingFlags::ImageAOMap;
               }
               if (descriptorBindingFlags)
               {
                  for (int i = 0; i < pgpucontext->m_pgpurenderer->m_pgpurendertarget->get_frame_count(); i++)
                  {
                     material.addDescriptorSet(m_descriptorPool,
                                               pcontext->m_psetdescriptorlayoutGltfImage->m_vkdescriptorsetlayout,
                                               descriptorBindingFlags, emptyTexture);
                  }
               }
            
				}
			}
		}
	}



	void gltf::Model::drawNode(Node *node, uint32_t uFrameIndex, VkCommandBuffer commandBuffer, uint32_t renderFlags,
                              VkPipelineLayout pipelineLayout, uint32_t bindImageSet)
	{
		if (node->mesh) {
			for (Primitive* primitive : node->mesh->primitives) {
				bool skip = false;
				auto pmaterial = primitive->m_pmaterial;
				if (renderFlags & RenderFlags::RenderOpaqueNodes) {
					skip = (pmaterial->alphaMode != Material::ALPHAMODE_OPAQUE);
				}
				if (renderFlags & RenderFlags::RenderAlphaMaskedNodes) {
					skip = (pmaterial->alphaMode != Material::ALPHAMODE_MASK);
				}
				if (renderFlags & RenderFlags::RenderAlphaBlendedNodes) {
					skip = (pmaterial->alphaMode != Material::ALPHAMODE_BLEND);
				}
				if (!skip) {
					if (renderFlags & RenderFlags::BindImages)
               {
                  auto &descriptorSet = pmaterial->m_descriptorseta[uFrameIndex];
                  vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, bindImageSet,
                                          1, &descriptorSet, 0, nullptr);
					}
					vkCmdDrawIndexed(commandBuffer, primitive->indexCount, 1, primitive->firstIndex, 0, 0);
				}
			}
		}
		for (auto& child : node->children) {
			drawNode(child, uFrameIndex, commandBuffer, renderFlags, pipelineLayout, bindImageSet);
		}
	}
   void gltf::Model::bind(VkCommandBuffer commandBuffer)
   {
      const VkDeviceSize offsets[1] = {0};
      vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertices.buffer, offsets);
      vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT32);
      m_bBuffersBound = true;
   }
   void gltf::Model::bind(::gpu::command_buffer *pgpucommandbuffer)
   {
      ::cast<::gpu_vulkan::command_buffer> pcommandbuffer = pgpucommandbuffer;
      bind(pcommandbuffer->m_vkcommandbuffer);
   }
   void gltf::Model::draw(::gpu::command_buffer *pgpucommandbuffer)
   {
      ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;
      gltfDraw(pcommandbuffer->m_vkcommandbuffer, pgpucommandbuffer->m_iFrameIndex);
   }


	void  gltf::Model::gltfDraw(VkCommandBuffer commandBuffer, uint32_t uFrameIndex, uint32_t renderFlags, VkPipelineLayout pipelineLayout, uint32_t bindImageSet)
	{
		if (!m_bBuffersBound) {
			const VkDeviceSize offsets[1] = { 0 };
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertices.buffer, offsets);
			vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT32);
		}
		for (auto& node : m_nodes) {
			drawNode(node, uFrameIndex, commandBuffer, renderFlags, pipelineLayout, bindImageSet);
		}
	}


	void  gltf::Model::getNodeDimensions(Node* node, glm::vec3& min, glm::vec3& max)
	{
		if (node->mesh) {
			for (Primitive* primitive : node->mesh->primitives) {
				glm::vec4 locMin = glm::vec4(primitive->dimensions.min, 1.0f) * node->getMatrix();
				glm::vec4 locMax = glm::vec4(primitive->dimensions.max, 1.0f) * node->getMatrix();
				if (locMin.x < min.x) { min.x = locMin.x; }
				if (locMin.y < min.y) { min.y = locMin.y; }
				if (locMin.z < min.z) { min.z = locMin.z; }
				if (locMax.x > max.x) { max.x = locMax.x; }
				if (locMax.y > max.y) { max.y = locMax.y; }
				if (locMax.z > max.z) { max.z = locMax.z; }
			}
		}
		for (auto child : node->children) {
			getNodeDimensions(child, min, max);
		}
	}

	void  gltf::Model::getSceneDimensions()
	{
		dimensions.min = glm::vec3(FLT_MAX);
		dimensions.max = glm::vec3(-FLT_MAX);
		for (auto node : m_nodes) {
			getNodeDimensions(node, dimensions.min, dimensions.max);
		}
		dimensions.size = dimensions.max - dimensions.min;
		dimensions.center = (dimensions.min + dimensions.max) / 2.0f;
		dimensions.radius = glm::distance(dimensions.min, dimensions.max) / 2.0f;
	}

	void gltf::Model::loadAnimations(tinygltf::Model& gltfModel)
	{
		for (tinygltf::Animation& anim : gltfModel.animations) {
			Animation animation{};
			animation.name = anim.name;
			if (anim.name.empty()) {
				animation.name = std::to_string(m_animations.size());
			}

			// Samplers
			for (auto& samp : anim.samplers) {
				AnimationSampler sampler{};

				if (samp.interpolation == "LINEAR") {
					sampler.interpolation = AnimationSampler::InterpolationType::LINEAR;
				}
				if (samp.interpolation == "STEP") {
					sampler.interpolation = AnimationSampler::InterpolationType::STEP;
				}
				if (samp.interpolation == "CUBICSPLINE") {
					sampler.interpolation = AnimationSampler::InterpolationType::CUBICSPLINE;
				}

				// Read sampler input time values
				{
					const tinygltf::Accessor& accessor = gltfModel.accessors[samp.input];
					const tinygltf::BufferView& bufferView = gltfModel.bufferViews[accessor.bufferView];
					const tinygltf::Buffer& buffer = gltfModel.buffers[bufferView.buffer];

					assert(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);

					float* buf = new float[accessor.count];
					memcpy(buf, &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(float));
					for (size_t index = 0; index < accessor.count; index++) {
						sampler.inputs.push_back(buf[index]);
					}
					delete[] buf;
					for (auto input : sampler.inputs) {
						if (input < animation.start) {
							animation.start = input;
						};
						if (input > animation.end) {
							animation.end = input;
						}
					}
				}

				// Read sampler output T/R/S values
				{
					const tinygltf::Accessor& accessor = gltfModel.accessors[samp.output];
					const tinygltf::BufferView& bufferView = gltfModel.bufferViews[accessor.bufferView];
					const tinygltf::Buffer& buffer = gltfModel.buffers[bufferView.buffer];

					assert(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);

					switch (accessor.type) {
					case TINYGLTF_TYPE_VEC3: {
							glm::vec3* buf = new glm::vec3[accessor.count];
							memcpy(buf, &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(glm::vec3));
							for (size_t index = 0; index < accessor.count; index++) {
								sampler.outputsVec4.push_back(glm::vec4(buf[index], 0.0f));
							}
							delete[] buf;
							break;
					}
					case TINYGLTF_TYPE_VEC4: {
							glm::vec4* buf = new glm::vec4[accessor.count];
							memcpy(buf, &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(glm::vec4));
							for (size_t index = 0; index < accessor.count; index++) {
								sampler.outputsVec4.push_back(buf[index]);
							}
							delete[] buf;
							break;
					}
					default: {
							information() << "unknown type";
							break;
					}
					}
				}

				animation.samplers.push_back(sampler);
			}

			// Channels
			for (auto& source : anim.channels) {
				AnimationChannel channel{};

				if (source.target_path == "rotation") {
					channel.path = AnimationChannel::PathType::ROTATION;
				}
				if (source.target_path == "translation") {
					channel.path = AnimationChannel::PathType::TRANSLATION;
				}
				if (source.target_path == "scale") {
					channel.path = AnimationChannel::PathType::SCALE;
				}
				if (source.target_path == "weights") {
					information() << "weights not yet supported, skipping channel";
					continue;
				}
				channel.samplerIndex = source.sampler;
				channel.node = nodeFromIndex(source.target_node);
				if (!channel.node) {
					continue;
				}

				animation.channels.push_back(channel);
			}

			m_animations.push_back(animation);
		}
	}

	void  gltf::Model::updateAnimation(uint32_t index, float time)
	{
		if (index > static_cast<uint32_t>(m_animations.size()) - 1) {
			information() << "No animation with index " << index;
			return;
		}
		Animation& animation = m_animations[index];

		bool updated = false;
		for (auto& channel : animation.channels) {
			AnimationSampler& sampler = animation.samplers[channel.samplerIndex];
			if (sampler.inputs.size() > sampler.outputsVec4.size()) {
				continue;
			}

			for (auto i = 0; i < sampler.inputs.size() - 1; i++) {
				if ((time >= sampler.inputs[i]) && (time <= sampler.inputs[i + 1])) {
					float u = std::max(0.0f, time - sampler.inputs[i]) / (sampler.inputs[i + 1] - sampler.inputs[i]);
					if (u <= 1.0f) {
						switch (channel.path) {
						case  AnimationChannel::PathType::TRANSLATION: {
								glm::vec4 trans = glm::mix(sampler.outputsVec4[i], sampler.outputsVec4[i + 1], u);
								channel.node->translation = glm::vec3(trans);
								break;
						}
						case  AnimationChannel::PathType::SCALE: {
								glm::vec4 trans = glm::mix(sampler.outputsVec4[i], sampler.outputsVec4[i + 1], u);
								channel.node->scale = glm::vec3(trans);
								break;
						}
						case  AnimationChannel::PathType::ROTATION: {
								glm::quat q1;
								q1.x = sampler.outputsVec4[i].x;
								q1.y = sampler.outputsVec4[i].y;
								q1.z = sampler.outputsVec4[i].z;
								q1.w = sampler.outputsVec4[i].w;
								glm::quat q2;
								q2.x = sampler.outputsVec4[i + 1].x;
								q2.y = sampler.outputsVec4[i + 1].y;
								q2.z = sampler.outputsVec4[i + 1].z;
								q2.w = sampler.outputsVec4[i + 1].w;
								channel.node->rotation = glm::normalize(glm::slerp(q1, q2, u));
								break;
						}
						}
						updated = true;
					}
				}
			}
		}
		if (updated) {
			for (auto& node : m_nodes) {
				node->update();
			}
		}
	}

	/*
		Helper functions
	*/
	gltf::Node* gltf::Model::findNode(Node* parent, uint32_t index) {
		Node* nodeFound = nullptr;
		if (parent->index == index) {
			return parent;
		}
		for (auto& child : parent->children) {
			nodeFound = findNode(child, index);
			if (nodeFound) {
				break;
			}
		}
		return nodeFound;
	}

	gltf::Node* gltf::Model::nodeFromIndex(uint32_t index) {
		Node* nodeFound = nullptr;
		for (auto& node : m_nodes) {
			nodeFound = findNode(node, index);
			if (nodeFound) {
				break;
			}
		}
		return nodeFound;
	}

	void  gltf::Model::prepareNodeDescriptor( Node* node, VkDescriptorSetLayout descriptorSetLayout) {
		if (node->mesh) {
			VkDescriptorSetAllocateInfo descriptorSetAllocInfo{};
			descriptorSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			descriptorSetAllocInfo.descriptorPool = m_descriptorPool;
			descriptorSetAllocInfo.pSetLayouts = &descriptorSetLayout;
			descriptorSetAllocInfo.descriptorSetCount = 1;
			::cast < ::gpu_vulkan::context > pcontext = m_pgpucontext;
			::cast < ::gpu_vulkan::device > pgpudevice = pcontext->m_pgpudevice;
			auto pphysicaldevice = pgpudevice->m_pphysicaldevice;

			VK_CHECK_RESULT(vkAllocateDescriptorSets(pcontext->logicalDevice(), &descriptorSetAllocInfo, &node->mesh->uniformBuffer.descriptorSet));

			VkWriteDescriptorSet writeDescriptorSet{};
			writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			writeDescriptorSet.descriptorCount = 1;
			writeDescriptorSet.dstSet = node->mesh->uniformBuffer.descriptorSet;
			writeDescriptorSet.dstBinding = 0;
			writeDescriptorSet.pBufferInfo = &node->mesh->uniformBuffer.descriptor;

			vkUpdateDescriptorSets(pcontext->logicalDevice(), 1, &writeDescriptorSet, 0, nullptr);
		}
		for (auto& child : node->children) {
			prepareNodeDescriptor(child, descriptorSetLayout);
		}
	}


} // namespace gpu_vulkan


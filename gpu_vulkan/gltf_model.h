#pragma once
#include <ktx.h>
#include <ktxvulkan.h>


#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>

#include <vulkan/vulkan.h>
#include "bred/graphics3d/model.h"
#include "gpu/gltf/model_base.h"
//#include "vk_tools/vk_init.h"
//#include "vulkan_wrapper/vulkan_descriptor.h"
#include "acme/prototype/geometry/quaternion.h"
//

#define TINYGLTF_NO_STB_IMAGE_WRITE
#ifdef VK_USE_PLATFORM_ANDROID_KHR
#define TINYGLTF_ANDROID_LOAD_FROM_ASSETS
#endif

#include "tiny_gltf.h"


namespace gpu_vulkan
{


	namespace gltf
	{


		enum DescriptorBindingFlags
		{
			ImageBaseColor = 0x00000001,
			ImageNormalMap = 0x00000002,
			ImageMetallicMap = 0x00000004,
			ImageRoughnessMap = 0x00000008, // If you separate roughness (usually combined, but define if needed)
			ImageAOMap = 0x00000010,
			ImageEmissiveMap = 0x00000020
			// Add more if needed (specGloss, diffuse, etc)
		};


		//extern VkDescriptorSetLayout descriptorSetLayoutImage;
		//extern VkDescriptorSetLayout descriptorSetLayoutUbo;
		//extern VkDescriptorSetLayout descriptorSetLayoutIbl;
		//extern VkMemoryPropertyFlags memoryPropertyFlags;
		//extern uint32_t descriptorBindingFlags;
      //CLASS_DECL_GPU_VULKAN VkDescriptorSetLayout ubo_descriptor_set_layout();
      //CLASS_DECL_GPU_VULKAN VkDescriptorSetLayout image_descriptor_set_layout();
		struct Node;

//		/*
//			gltf texture loading class
//		*/
//		class Texture
//		{
//		public:
//			::gpu::context* m_pgpucontext = nullptr;
//			VkImage image;
//			VkImageLayout imageLayout;
//			VkDeviceMemory deviceMemory;
//			VkImageView view;
//			uint32_t width, height;
//			uint32_t mipLevels;
//			uint32_t layerCount;
//			VkDescriptorImageInfo descriptor;
//			VkSampler sampler;
//			uint32_t index;
//			void updateDescriptor();
//			void destroy();
////			void fromglTfImage(tinygltf::Image& gltfimage, void * pIfKtx, long long llIfKtx, ::gpu::context* pcontext, VkQueue copyQueue, bool isSrgb);
//
//         void fromglTfImage(tinygltf::Image &gltfimage, ::std::string path, ::gpu::context *pcontext, VkQueue copyQueue,
//                            bool isSrgb);
//
//		};


		/*
			gltf material class
		*/
      class CLASS_DECL_GPU_VULKAN Material
		{
		public:

			::gpu::context* m_pgpucontext = nullptr;
			enum AlphaMode { ALPHAMODE_OPAQUE, ALPHAMODE_MASK, ALPHAMODE_BLEND };
			AlphaMode alphaMode = ALPHAMODE_OPAQUE;
			float alphaCutoff = 1.0f;
			float metallicFactor = 1.0f;
			float roughnessFactor = 1.0f;
			floating_sequence4 baseColorFactor = floating_sequence4(1.0f);
			::pointer<::gpu_vulkan::texture> baseColorTexture;
         ::pointer<::gpu_vulkan::texture> metallicRoughnessTexture;
         ::pointer<::gpu_vulkan::texture> normalTexture;
         ::pointer<::gpu_vulkan::texture> occlusionTexture;
         ::pointer<::gpu_vulkan::texture> emissiveTexture;

			::pointer<::gpu_vulkan::texture> specularGlossinessTexture;
         ::pointer<::gpu_vulkan::texture> diffuseTexture;

			::array_base < VkDescriptorSet >m_descriptorseta;

			Material(::gpu::context* pcontext) : m_pgpucontext(pcontext) {};
			void addDescriptorSet(VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, uint32_t descriptorBindingFlags, ::gpu_vulkan::texture* fallbackTexture);
		};

		/*
			gltf primitive
		*/
		struct Primitive {
			uint32_t firstIndex;
			uint32_t indexCount;
			uint32_t firstVertex;
			uint32_t vertexCount;
			Material * m_pmaterial;

			struct Dimensions {
				floating_sequence3 min = floating_sequence3(FLT_MAX);
				floating_sequence3 max = floating_sequence3(-FLT_MAX);
				floating_sequence3 size;
				floating_sequence3 center;
				float radius;
			} dimensions;

			void setDimensions(floating_sequence3 min, floating_sequence3 max);
			Primitive(uint32_t firstIndex, uint32_t indexCount, Material * pmaterial) : firstIndex(firstIndex), indexCount(indexCount), m_pmaterial(pmaterial) {};
		};

		/*
			gltf mesh
		*/
		struct Mesh {
			::gpu::context* m_pgpucontext = nullptr;

			std::vector<Primitive*> primitives;
			std::string name;

			struct UniformBuffer {
				VkBuffer buffer;
				VkDeviceMemory memory;
				VkDescriptorBufferInfo descriptor;
				VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
				void* mapped;
			} uniformBuffer;

			struct UniformBlock {
				floating_matrix4 matrix;
				floating_matrix4 jointMatrix[64]{};
				float jointcount{ 0 };
			} uniformBlock;

			Mesh(::gpu::context * pcontext, floating_matrix4 matrix);
			~Mesh();
		};

		/*
			gltf skin
		*/
		struct Skin {
			std::string name;
			Node* skeletonRoot = nullptr;
			std::vector<floating_matrix4> inverseBindMatrices;
			std::vector<Node*> joints;
		};

		/*
			gltf node
		*/
		struct CLASS_DECL_GPU_VULKAN Node {
			Node* parent;
			uint32_t index;
			std::vector<Node*> children;
			floating_matrix4 matrix;
			std::string name;
			Mesh* mesh;
			Skin* skin;
			int32_t skinIndex = -1;
			floating_sequence3 translation{};
			floating_sequence3 scale{ 1.0f };
			floating_matrix4 rotation{};
			floating_matrix4 localMatrix();
			floating_matrix4 getMatrix();
			void update();
			~Node();
		};

		/*
			gltf animation channel
		*/
		struct AnimationChannel {
			enum PathType { TRANSLATION, ROTATION, SCALE };
			PathType path;
			Node* node;
			uint32_t samplerIndex;
		};

		/*
			gltf animation sampler
		*/
		struct AnimationSampler {
			enum InterpolationType { LINEAR, STEP, CUBICSPLINE };
			InterpolationType interpolation;
			std::vector<float> inputs;
			std::vector<floating_sequence4> outputsVec4;
		};

		/*
			gltf animation
		*/
		struct Animation {
			std::string name;
			std::vector<AnimationSampler> samplers;
			std::vector<AnimationChannel> channels;
			float start = std::numeric_limits<float>::max();
			float end = std::numeric_limits<float>::min();
		};

		/*
			gltf default vertex layout with easy Vulkan mapping functions
		*/
		//enum class VertexComponent { Position, Normal, UV, Color, Tangent, Joint0, Weight0 };
      enum class VertexComponent
      {
         Position,
         Normal,
         UV,
         Color,
         Tangent
      };

		struct Vertex {
			floating_sequence3 pos;
			floating_sequence3 normal;
			floating_sequence2 gltf_uv;
			floating_sequence4 color;
			//floating_sequence4 joint0;
			//floating_sequence4 weight0;
			floating_sequence4 tangent;
			//static VkVertexInputBindingDescription vertexInputBindingDescription;
			//static std::vector<VkVertexInputAttributeDescription> vertexInputAttributeDescriptions;
			//static VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;
			//static VkVertexInputBindingDescription inputBindingDescription(uint32_t binding);
			//static VkVertexInputAttributeDescription inputAttributeDescription(uint32_t binding, uint32_t location, VertexComponent component);
			//static std::vector<VkVertexInputAttributeDescription> inputAttributeDescriptions(uint32_t binding, const std::vector<VertexComponent> components);
			///** @brief Returns the default pipeline vertex input state create info structure for the requested vertex components */
			//static VkPipelineVertexInputStateCreateInfo* getPipelineVertexInputState(const std::vector<VertexComponent> components);
		};
      enum class Vertex2Component
      {
         Position,
         Normal,
         UV,
         Color,
         Tangent,
         Joint0,
         Weight0
      };
      		struct Vertex2
      {
         floating_sequence3 pos;
         floating_sequence3 normal;
         floating_sequence2 uv;
         floating_sequence4 color;
         floating_sequence4 joint0;
         floating_sequence4 weight0;
         floating_sequence4 tangent;
         // static VkVertexInputBindingDescription vertexInputBindingDescription;
         // static std::vector<VkVertexInputAttributeDescription> vertexInputAttributeDescriptions;
         // static VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;
         // static VkVertexInputBindingDescription inputBindingDescription(uint32_t binding);
         // static VkVertexInputAttributeDescription inputAttributeDescription(uint32_t binding, uint32_t location,
         // VertexComponent component); static std::vector<VkVertexInputAttributeDescription>
         // inputAttributeDescriptions(uint32_t binding, const std::vector<VertexComponent> components);
         ///** @brief Returns the default pipeline vertex input state create info structure for the requested vertex
         ///components */
         // static VkPipelineVertexInputStateCreateInfo* getPipelineVertexInputState(const std::vector<VertexComponent>
         // components);
      };


		enum FileLoadingFlags {
			None = 0x00000000,
			PreTransformVertices = 0x00000001,
			PreMultiplyVertexColors = 0x00000002,
			FlipY = 0x00000004,
			DontLoadImages = 0x00000008,
         //UseFsCallbacks = 0x00000010
		};

		enum RenderFlags {
         RenderNone = 0x00000000,
			BindImages = 0x00000001,
			RenderOpaqueNodes = 0x00000002,
			RenderAlphaMaskedNodes = 0x00000004,
			RenderAlphaBlendedNodes = 0x00000008
		};

		/*
			gltf model loading and rendering class
		*/
		class CLASS_DECL_GPU_VULKAN Model : public ::gpu::gltf::model_base
		{
		public:


			::gpu_vulkan::texture* getTexture(uint32_t index);
			::pointer < gpu_vulkan::texture > emptyTexture;
			void createEmptyTexture(VkQueue transferQueue);
		//public:
			///::gpu::context* m_pgpucontext = nullptr;
			VkDescriptorPool m_descriptorPool;




			struct Vertices {
				int count;
				VkBuffer buffer;
				VkDeviceMemory memory;
			} vertices;
			struct Indices {
				int count;
				VkBuffer buffer;
				VkDeviceMemory memory;
			} indices;

			std::vector<Node*> m_nodes;
			std::vector<Node*> m_linearNodes;

			std::vector<Skin*> m_skins;


			::pointer_array<::gpu_vulkan::texture> m_textures;
			std::vector<Material> m_materials;
			std::vector<Animation> m_animations;


         //::array_base<VkDescriptorSet> m_vkdescriptorsetaPbr;
         

			struct Dimensions {
				floating_sequence3 min = floating_sequence3(FLT_MAX);
				floating_sequence3 max = floating_sequence3(-FLT_MAX);
				floating_sequence3 size;
				floating_sequence3 center;
				float radius;
			} dimensions;

			bool m_bMetallicRoughnessWorkflow = true;
			bool m_bBuffersBound = false;
			::std::string m_path1;

			Model();
			~Model();
			void loadNode(gltf::Node* parent, const tinygltf::Node& node, uint32_t nodeIndex, const tinygltf::Model& model, std::vector<uint32_t>& indexBuffer, std::vector<Vertex>& vertexBuffer, float globalscale);
			void loadSkins(tinygltf::Model& gltfModel);
			void loadImages(tinygltf::Model& gltfModel, ::gpu::context * pcontext, VkQueue transferQueue);
			void loadMaterials(tinygltf::Model& gltfModel);
			void loadAnimations(tinygltf::Model& gltfModel);

			void loadFromFile(std::string filename, ::gpu::context * pcontext, VkQueue transferQueue, uint32_t fileLoadingFlags = FileLoadingFlags::None, float scale = 1.0f);


         void bind(VkCommandBuffer cmd);
			void bind(::gpu::command_buffer * pgpucommandbuffer)override;
         void draw(::gpu::command_buffer *pgpucommandbuffer) override;

			void gltfDraw(
				VkCommandBuffer cmd,
            uint32_t uFrameIndex, 
				uint32_t renderFlags = 0,
				VkPipelineLayout pipelineLayout = VK_NULL_HANDLE,
				uint32_t bindImageSet = 1
			);

			void drawNode(Node* node, uint32_t uFrameIndex,  VkCommandBuffer commandBuffer, uint32_t renderFlags = 0, VkPipelineLayout pipelineLayout = VK_NULL_HANDLE, uint32_t bindImageSet = 1);


			void getNodeDimensions(Node* node, floating_sequence3& min, floating_sequence3& max);
			void getSceneDimensions();
			void updateAnimation(uint32_t index, float time);
			Node* findNode(Node* parent, uint32_t index);
			Node* nodeFromIndex(uint32_t index);
			void prepareNodeDescriptor(gltf::Node* node, VkDescriptorSetLayout descriptorSetLayout);

         ::gpu::texture *loadMaterialTexture2(const ::scoped_string &scopedstr, tinygltf::Material &material,
                                              tinygltf::Model &gltfModel, const ::scoped_string &scopedstrType,
                                              aiTextureType type);
		};


	} // namespace gltf


} // namespace gpu_vulkan



DECLARE_GPU_PROPERTIES(CLASS_DECL_BRED, ::gpu_vulkan::gltf::Vertex);

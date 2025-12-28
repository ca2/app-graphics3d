#pragma once
#include <ktx.h>
#include <ktxvulkan.h>


#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>

#include <vulkan/vulkan.h>
//#include "bred/graphics3d/model.h"
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


		//extern aaaVkDescriptorSetLayout descriptorSetLayoutImage;
		//extern aaaVkDescriptorSetLayout descriptorSetLayoutUbo;
		//extern aaaVkDescriptorSetLayout descriptorSetLayoutIbl;
		//extern VkMemoryPropertyFlags memoryPropertyFlags;
		//extern uint32_t descriptorBindingFlags;
      //CLASS_DECL_GPU_VULKAN aaaVkDescriptorSetLayout ubo_descriptor_set_layout();
      //CLASS_DECL_GPU_VULKAN aaaVkDescriptorSetLayout image_descriptor_set_layout();
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

         //::pointer<::gpu_vulkan::binding_set> m_pbindingset;
			::map_base < ::gpu::binding_set * , ::pointer < ::array < VkDescriptorSet > > >m_mapdescriptorset;

         //::array_base<VkDescriptorSet> m_descriptorsetaSceneGltf4;

                  uint32_t m_uDescriptorBindingFlags = 0;



         Material() {}
			Material(::gpu::context* pcontext) : m_pgpucontext(pcontext) {};
          
         
         //virtual void update_binding_set( ::gpu::binding_set * pgpubindingset,
           // uint32_t descriptorBindingFlags,
             //                        ::gpu_vulkan::texture *fallbackTexture);
         virtual void update_binding_set(::gpu::binding_slot_set * pgpubindinslotset, uint32_t descriptorBindingFlags,
                                         ::gpu_vulkan::texture *fallbackTexture);
			
         
         //void addDescriptor(int iCount, ::array_base<VkDescriptorSet> & a, VkDescriptorPool descriptorPool,
         //                           aaaVkDescriptorSetLayout descriptorSetLayout, uint32_t descriptorBindingFlags,
         //                           ::gpu_vulkan::texture *fallbackTexture);
         //::array_base<VkDescriptorSet> &descriptor_set_array_gltf(gltf::Model * pmodel);
         //::array_base<VkDescriptorSet> &descriptor_set_array_scene_gltf(gltf::Model *pmodel);

         //::array_base<VkDescriptorSet> &descriptor_set_array(gltf::Model *pmodel,
           //                                                             ::gpu::binding_set *pgpubindingset);
        

         //void addDescriptorSetSceneGltf4(VkDescriptorPool descriptorPool, aaaVkDescriptorSetLayout descriptorSetLayout,
         //                      uint32_t descriptorBindingFlags, ::gpu_vulkan::texture *fallbackTexture);


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
			Node* m_pnodeParent;
			uint32_t m_uIndex;
			::array_base<Node*> m_nodeaChildren;
			floating_matrix4 m_matrix;
			::string m_strName;
			Mesh* m_pmesh;
			Skin* m_pskin;
			int32_t m_iSkinIndex = -1;
			floating_sequence3 m_sequence3Translation;
			floating_sequence3 m_sequence3Scale{1.0f};
			floating_matrix4 m_matrixRotation{1.0f};

         
  			~Node();

         floating_matrix4 localMatrix();
			floating_matrix4 getMatrix();
			void update();

		};

		/*
			gltf animation channel
		*/
		struct AnimationChannel {
			enum PathType { TRANSLATION, ROTATION, SCALE };
			PathType m_epath;
			Node* m_pnode;
			uint32_t m_uSamplerIndex;
		};

		/*
			gltf animation sampler
		*/
		struct AnimationSampler {
			enum InterpolationType { LINEAR, STEP, CUBICSPLINE };
			InterpolationType m_einterpolation;
			float_array m_faInput;
			::array_base<floating_sequence4> m_sequence4aOutput;
		};

		/*
			gltf animation
		*/
		struct Animation {
			::string m_strName;
         ::array_base<AnimationSampler> m_samplera;
			::array_base<AnimationChannel> m_channela;
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

		//struct Vertex {
		//	floating_sequence3 pos;
		//	floating_sequence3 normal;
		//	floating_sequence2 gltf_uv;
		//	floating_sequence4 color;
		//	//floating_sequence4 joint0;
		//	//floating_sequence4 weight0;
		//	floating_sequence4 tangent;
		//	//static VkVertexInputBindingDescription vertexInputBindingDescription;
		//	//static std::vector<VkVertexInputAttributeDescription> vertexInputAttributeDescriptions;
		//	//static VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;
		//	//static VkVertexInputBindingDescription inputBindingDescription(uint32_t binding);
		//	//static VkVertexInputAttributeDescription inputAttributeDescription(uint32_t binding, uint32_t location, VertexComponent component);
		//	//static std::vector<VkVertexInputAttributeDescription> inputAttributeDescriptions(uint32_t binding, const std::vector<VertexComponent> components);
		//	///** @brief Returns the default pipeline vertex input state create info structure for the requested vertex components */
		//	//static VkPipelineVertexInputStateCreateInfo* getPipelineVertexInputState(const std::vector<VertexComponent> components);
		//};
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
         //OnlySceneImages = 0x00000010, // Only Albedo and Normal
         //UseFsCallbacks = 0x00000010
		};

		enum RenderFlags {
         RenderNone = 0x00000000,
			RenderOpaqueNodes = 0x00000002,
			RenderAlphaMaskedNodes = 0x00000004,
			RenderAlphaBlendedNodes = 0x00000008,
         //BindGltfImages = 0x00000010,
         //BindJustSceneImages = 0x00000020,
      };

		/*
			gltf model loading and rendering class
		*/
		class CLASS_DECL_GPU_VULKAN Model : public ::gpu::model::model_base
		{
		public:


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

			::comparable_array_base<Node *> m_nodea;
         ::comparable_array_base<Node *> m_nodeaLinear;

			::comparable_array_base<Skin*> m_skina;


			::pointer_array<::gpu_vulkan::texture> m_texturea;
			::array_base<Material> m_materiala;
			array_base<Animation> m_animationa;


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

         ::pointer<gpu_vulkan::texture> emptyTexture2;


			Model();
			~Model();


			void loadNode(gltf::Node* parent, const tinygltf::Node& node, uint32_t nodeIndex, const tinygltf::Model& model, std::vector<uint32_t>& indexBuffer, std::vector<::gpu::gltf::vertex>& vertexBuffer, float globalscale);
			void loadSkins(tinygltf::Model& gltfModel);
			void loadImages(tinygltf::Model& gltfModel, ::gpu::context * pcontext, VkQueue transferQueue);
			void loadMaterials(tinygltf::Model& gltfModel);
			void loadAnimations(tinygltf::Model& gltfModel);

			void loadFromFile(std::string filename, ::gpu::context * pcontext, VkQueue transferQueue, uint32_t fileLoadingFlags = FileLoadingFlags::None, float scale = 1.0f);

         ::gpu_vulkan::texture *getTexture(uint32_t index);
         void createEmptyTexture(VkQueue transferQueue);
         ::gpu_vulkan::texture *empty_texture();


         void bind(VkCommandBuffer cmd);
			void bind(::gpu::command_buffer * pgpucommandbuffer)override;
         void draw(::gpu::command_buffer *pgpucommandbuffer) override;

			void gltfDraw(::gpu::command_buffer *pgpucommandbuffer, 
				//VkCommandBuffer cmd,
            //uint32_t uFrameIndex, 
            //::gpu::binding_set_pointer pbindingset,
				uint32_t renderFlags = 0
            //,				VkPipelineLayout pipelineLayout = VK_NULL_HANDLE
			);

			//void drawNode(::gpu::command_buffer * pgpucommandbuffer, Node* node, uint32_t uFrameIndex, VkCommandBuffer commandBuffer, ::gpu::binding_set_pointer pbindingset, uint32_t renderFlags = 0, VkPipelineLayout pipelineLayout = VK_NULL_HANDLE);
         //void drawNode(::gpu::command_buffer *pgpucommandbuffer, 
         //   Node *node, uint32_t uFrameIndex,
         //   ::gpu::binding_set_pointer pbindingset, uint32_t renderFlags = 0);
         void drawNode(::gpu::command_buffer *pgpucommandbuffer, Node *node, uint32_t renderFlags = 0);


			void getNodeDimensions(Node* node, floating_sequence3& min, floating_sequence3& max);
			void getSceneDimensions();
			void updateAnimation(uint32_t index, float time);
			Node* findNode(Node* parent, uint32_t index);
			Node* nodeFromIndex(uint32_t index);
			//void prepareNodeDescriptor(gltf::Node* node, aaaVkDescriptorSetLayout descriptorSetLayout);

         ::gpu::texture *loadMaterialTexture2(const ::scoped_string &scopedstr, tinygltf::Material &material,
                                              tinygltf::Model &gltfModel, const ::scoped_string &scopedstrType,
                                              aiTextureType type);
		};


	} // namespace gltf


} // namespace gpu_vulkan



//DECLARE_GPU_PROPERTIES(CLASS_DECL_BRED, ::gpu::gltf::vertex);

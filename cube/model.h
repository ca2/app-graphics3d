// From V0idsEmbrace@Twich continuum project
// by camilo on 2025-05-17 02:40 <3ThomasBorregaardSorensen!!
#pragma once


//#include "device.h"
//#include "buffer.h"
#include "utilities.h"

#define GLM_FORCE_RADIANS	
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <functional>

#include <memory>
#include <vector>
#include <unordered_map>



namespace cube
{


   class CLASS_DECL_CUBE model :
      virtual public ::particle
   {
   public:


      struct Vertex 
      {

         glm::vec3 position{};
         glm::vec3 color{};
         glm::vec3 normal{};
         glm::vec2 uv{};

         //static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
         //static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

         bool operator==(const Vertex& other) const { return position == other.position && normal == other.normal && uv == other.uv; }
      };

      struct Builder 
      {
         std::vector<Vertex> vertices{};
         std::vector<uint32_t> indices{};

         void loadModel(::cube::context* pcontext, const ::file::path & path);

      };




      //static ::pointer<model> createModelFromFile(::cube::context * pcontext, const ::file::path & path);


      model();
      ~model();


      virtual void initialize_model(::cube::context * pcontext, const model::Builder& builder);

      virtual void draw(void * posdata);
      virtual void bind(void * posdata);

   //private:
   //   void createVertexBuffers(const std::vector<Vertex>& vertices);
   //   void createIndexBuffers(const std::vector<uint32_t>& indices);

   //   bool hasIndexBuffer = false;
   //   ::pointer < context > m_pcontext;


   //   ::pointer<buffer> vertexBuffer;
   //   uint32_t vertexCount;

   //   ::pointer<buffer> indexBuffer;
   //   uint32_t indexCount;

   };


}  // namespace cube





namespace std {
   template <>
   struct hash<::cube::model::Vertex> {
      size_t operator()(::cube::model::Vertex const& vertex) const {
         size_t seed = 0;
         ::cube::hash_combine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
         return seed;
      }
   };
}  // namespace std



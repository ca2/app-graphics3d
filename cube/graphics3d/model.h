// From V0idsEmbrace@Twich continuum project
// by camilo on 2025-05-17 02:40 <3ThomasBorregaardSorensen!!
#pragma once


//#include "device.h"
//#include "buffer.h"
#include "app-cube/cube/graphics3d/utilities.h"
#include "app-cube/cube/graphics3d/types.h"

#define GLM_FORCE_RADIANS	
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <functional>

#include <memory>
#include <vector>
#include <unordered_map>



namespace graphics3d
{


   class CLASS_DECL_CUBE model :
      virtual public ::particle
   {
   public:


      

      struct Builder
      {
         std::vector<Vertex> vertices{};
         std::vector<uint32_t> indices{};

      };

      struct tinyobjloader_Builder :
         public Builder
      {

         void loadModel(::graphics3d::context* pcontext, const ::file::path & path);

      };




      //static ::pointer<model> createModelFromFile(::cube::context * pcontext, const ::file::path & path);


      model();
      ~model();


      virtual void initialize_model(::graphics3d::context * pcontext, const model::Builder& builder);

      virtual void draw(::graphics3d::context* pcontext);
      virtual void bind(::graphics3d::context* pcontext);

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


}  // namespace graphics3d





namespace std {
   template <>
   struct hash<::graphics3d::Vertex> {
      size_t operator()(::graphics3d::Vertex const& vertex) const {
         size_t seed = 0;
         ::graphics3d::hash_combine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
         return seed;
      }
   };
}  // namespace std



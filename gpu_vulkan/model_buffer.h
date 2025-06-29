// Created by camilo on 2025-06-11 01:06 <3ThomasBorregaardSørensen!!
#pragma once


#include "gpu_vulkan/context.h"


namespace gpu_vulkan
{

 
   ///graphics::RectangleVertex quadVertices[]

   class CLASS_DECL_GPU_VULKAN model_buffer :
      virtual public ::particle
   {
   public:
      
      
      ::pointer < memory_buffer > m_pbufferVertex;
      ::pointer < memory_buffer > m_pbufferIndex;
      int m_iVertexCount;
      int m_iIndexCount;   

      model_buffer();
      ~model_buffer();


      virtual void create_rectangle(::gpu_vulkan::context* pcontext, bool bIndexed, bool bYSwap);

      template < typename VERTEX >
      void create_vertex_array(::gpu_vulkan::context* pcontext, int iVertexCount)
      {

         m_iVertexCount = iVertexCount;

         auto size = sizeof(VERTEX) * m_iVertexCount;

         pcontext->defer_construct_new(m_pbufferVertex, size);

      }

      void sequence3_color_set_rectangle(
         ::gpu_vulkan::context* pcontext,
         const ::double_point points1[4],
         const  ::color::color& color,
         const ::double_size& size);


      virtual void bind(::gpu_vulkan::command_buffer* pcommandbuffer);

      virtual void draw(::gpu_vulkan::command_buffer* pcommandbuffer);

   };


} // namespace gpu_vulkan




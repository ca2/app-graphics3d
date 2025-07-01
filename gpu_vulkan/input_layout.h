// Created by camilo on 2025-06-29 03:30 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/input_layout.h"


namespace gpu_vulkan
{


   class CLASS_DECL_GPU_VULKAN input_layout :
      virtual public ::gpu::input_layout
   {
   public:

      
      ::array < VkVertexInputBindingDescription >     m_bindings;
      ::array < VkVertexInputAttributeDescription >   m_attribs;


      input_layout();
      ~input_layout() override;

      
      void on_initialize_input_layout() override;

      
      virtual ::array<VkVertexInputBindingDescription> _001GetVertexBindingDescriptions();
      virtual ::array<VkVertexInputAttributeDescription> _001GetVertexAttributeDescriptions();


   };


} // namespace gpu_vulkan



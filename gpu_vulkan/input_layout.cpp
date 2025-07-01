// From bred/gpu/input_layout.cpp by camilo on 2025-06-29 03:54 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "input_layout.h"
#include "_gpu_vulkan.h"


namespace gpu_vulkan
{


   input_layout::input_layout()
   {


   }


   input_layout::~input_layout()
   {


   }


   ::array<VkVertexInputBindingDescription> input_layout::_001GetVertexBindingDescriptions()
   {

      ::array<VkVertexInputBindingDescription> bindingDescriptions(1, VkVertexInputBindingDescription{});

      bindingDescriptions[0].binding = 0;
      bindingDescriptions[0].stride = this->size();
      bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

      return bindingDescriptions;


   }


   ::array<VkVertexInputAttributeDescription> input_layout::_001GetVertexAttributeDescriptions()
   {

      ::array<VkVertexInputAttributeDescription> attributeDescriptions{};

      //::array<VkVertexInputAttributeDescription> attributeDescriptions{};

      uint32_t i = 0;
      uint32_t pos = 0;

      auto pproperties = this->m_pproperties;

      for (auto p = pproperties; 
         ::is_set(p->m_pszName) && p->m_etype != ::gpu::e_type_none;
         p++, i++)
      {

         attributeDescriptions.add({ i, 0, ::vulkan::get_type_vk_format(p->m_etype), pos});

         pos += ::gpu::get_type_size(p->m_etype);

      }

      return attributeDescriptions;

   }


   void input_layout::on_initialize_input_layout()
   {

      m_bindings = _001GetVertexBindingDescriptions();

      m_attribs = _001GetVertexAttributeDescriptions();

      //      ::array<VkVertexInputAttributeDescription> attributeDescriptions{};
//
//      //::array<VkVertexInputAttributeDescription> attributeDescriptions{};
//
//      uint32_t i = 0;
//      uint32_t pos = 0;
//
//      for (auto p = pproperties; ::is_set(p->m_pszName); p++, i++, pos += ::gpu::get_type_size(p->m_etype))
//      {
//
//         attributeDescriptions.add({ i, 0, get_type_vk_format(p->m_etype), pos});
//
//      }



   }


} // namespace gpu_vulkan




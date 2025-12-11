// From bred/gpu/binding.h by camilo on 2025-12-11 01:08 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/binding.h"


namespace gpu_vulkan
{

   class binding :
      virtual public ::gpu::binding
   {
   public:

   };

   class binding_set : virtual public ::gpu::binding_set
   {
   public:

   };


   //class CLASS_DECL_BRED binding_set_array : virtual public ::pointer_array<binding_set>
   //{
   //public:
   //   bool has_global_ubo() const;
   //};


} // namespace gpu_vulkan





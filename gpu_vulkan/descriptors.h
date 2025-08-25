#pragma once


#include "context.h"
#include "acme/prototype/collection/map.h"


namespace gpu_vulkan
{


   class CLASS_DECL_GPU_VULKAN descriptor_set_layout :
      virtual public ::particle
   {
   public:
      class CLASS_DECL_GPU_VULKAN Builder
      {
      public:
         Builder(::gpu::context * pgpucontext) : m_pgpucontext{ pgpucontext } {}

         Builder & addBinding(
             uint32_t binding,
             VkDescriptorType descriptorType,
             VkShaderStageFlags stageFlags,
             uint32_t count = 1);
         ::pointer<descriptor_set_layout> build() const;

      private:
         ::pointer < context > m_pgpucontext;
         ::map < unsigned int, VkDescriptorSetLayoutBinding> bindings{};
      };

      descriptor_set_layout(
          context * pvkcdevice, ::map<unsigned int, VkDescriptorSetLayoutBinding> bindings);
      ~descriptor_set_layout();

      VkDescriptorSetLayout getDescriptorSetLayout() const { return m_vkdescriptorsetlayout; }

      ::pointer < context > m_pgpucontext;
      VkDescriptorSetLayout m_vkdescriptorsetlayout;
      ::map<unsigned int, VkDescriptorSetLayoutBinding> m_bindings;

   };


   class CLASS_DECL_GPU_VULKAN descriptor_pool :
      virtual public ::particle
   {
   public:


      class CLASS_DECL_GPU_VULKAN Builder :
         virtual public ::particle
      {
      public:

         //Builder(context& m_pgpucontext) : m_pgpucontext{ m_pgpucontext } {}
         Builder() {}
         void initialize_builder(::gpu::context * pgpucontext)
         {
            m_pgpucontext = pgpucontext;

         }
         Builder & addPoolSize(VkDescriptorType descriptorType, uint32_t count);
         Builder & setPoolFlags(VkDescriptorPoolCreateFlags flags);
         Builder & setMaxSets(uint32_t count);
         ::pointer<descriptor_pool> build() const;

      private:
         ::pointer < context > m_pgpucontext;
         ::array<VkDescriptorPoolSize> poolSizes{};
         uint32_t maxSets = 1000;
         VkDescriptorPoolCreateFlags poolFlags = 0;
      };

      descriptor_pool(
          context * pvkcdevice,
          uint32_t maxSets,
          VkDescriptorPoolCreateFlags poolFlags,
          const ::array<VkDescriptorPoolSize> & poolSizes);
      ~descriptor_pool();
      descriptor_pool(const descriptor_pool &) = delete;
      descriptor_pool & operator=(const descriptor_pool &) = delete;

      bool allocateDescriptor(
          const VkDescriptorSetLayout descriptorSetLayout, 
         VkDescriptorSet & descriptor,
                              uint32_t variableDescriptorCount) const;

      void freeDescriptors(::array<VkDescriptorSet> & descriptors) const;

      void resetPool();

      ::pointer < context > m_pgpucontext;
      VkDescriptorPool m_vkdescriptorpool;

   };


   class CLASS_DECL_GPU_VULKAN descriptor_writer
   {
   public:
      descriptor_writer(descriptor_set_layout & setLayout, descriptor_pool & pool);

      descriptor_writer & writeBuffer(uint32_t binding, VkDescriptorBufferInfo * bufferInfo);
      descriptor_writer & writeImage(uint32_t binding, VkDescriptorImageInfo * imageInfo);
      descriptor_writer & writeImage(uint32_t binding, const VkDescriptorImageInfo *imageInfos, uint32_t count);

      bool build(VkDescriptorSet & set);
      void overwrite(VkDescriptorSet & set);

      descriptor_set_layout & m_setLayout;
      descriptor_pool & pool;
      ::array_base<VkWriteDescriptorSet> m_vkwritedescriptorseta;
      uint32_t m_uVariableDescriptorCount = 0;
   };


} // namespace graphics3d_vulkan




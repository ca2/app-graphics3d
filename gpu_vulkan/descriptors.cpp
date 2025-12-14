#include "framework.h"
#include "descriptors.h"

#include <assert.h>

namespace gpu_vulkan
{


   // *************** Descriptor Set Layout Builder *********************

   descriptor_set_layout::Builder & descriptor_set_layout::Builder::addBinding(
       uint32_t binding,
       VkDescriptorType descriptorType,
       VkShaderStageFlags stageFlags,
       uint32_t count) {
      assert(bindings.count(binding) == 0 && "Binding already in use");
      VkDescriptorSetLayoutBinding layoutBinding{};
      layoutBinding.binding = binding;
      layoutBinding.descriptorType = descriptorType;
      layoutBinding.descriptorCount = count;
      layoutBinding.stageFlags = stageFlags;
      bindings[binding] = layoutBinding;
      return *this;
   }

   ::pointer<descriptor_set_layout> descriptor_set_layout::Builder::build() const
   {
      auto pvkcdevice = this->m_pgpucontext.m_p;
      __refdbg_this(pvkcdevice);
      return øallocate descriptor_set_layout(m_pgpucontext, bindings);
   }

   // *************** Descriptor Set Layout *********************

   descriptor_set_layout::descriptor_set_layout(
       context * pvkcdevice, ::map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
      : m_pgpucontext{ pvkcdevice }, m_bindings{ bindings } {
      ::array<VkDescriptorSetLayoutBinding> setLayoutBindings{};
      for (auto kv : bindings) {
         setLayoutBindings.add(kv.m_element2);
      }

      VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
      descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
      descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
      descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();
      ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;
      if (vkCreateDescriptorSetLayout(
         m_pgpucontext->logicalDevice(),
         &descriptorSetLayoutInfo,
         nullptr,
                                      &m_vkdescriptorsetlayout) != VK_SUCCESS)
      {
         throw ::exception(error_failed, "failed to create descriptor set layout!");
      }
   }

   descriptor_set_layout::~descriptor_set_layout() {
      if (m_vkdescriptorsetlayout != VK_NULL_HANDLE)
      {
         ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;
         vkDestroyDescriptorSetLayout(m_pgpucontext->logicalDevice(), m_vkdescriptorsetlayout, nullptr);
         m_vkdescriptorsetlayout = VK_NULL_HANDLE;
      }
   }

   // *************** Descriptor Pool Builder *********************

   descriptor_pool::Builder & descriptor_pool::Builder::addPoolSize(
       VkDescriptorType descriptorType, uint32_t count) {
      poolSizes.add({ descriptorType, count });
      return *this;
   }

   descriptor_pool::Builder & descriptor_pool::Builder::setPoolFlags(
       VkDescriptorPoolCreateFlags flags) {
      poolFlags = flags;
      return *this;
   }
   descriptor_pool::Builder & descriptor_pool::Builder::setMaxSets(uint32_t count) {
      maxSets = count;
      return *this;
   }

   ::pointer <descriptor_pool> descriptor_pool::Builder::build() const {
      return øallocate descriptor_pool(m_pgpucontext, maxSets, poolFlags, poolSizes);
   }

   // *************** Descriptor Pool *********************

   descriptor_pool::descriptor_pool(
       context * pvkcdevice,
       uint32_t maxSets,
       VkDescriptorPoolCreateFlags poolFlags,
       const ::array<VkDescriptorPoolSize> & poolSizes)
      : m_pgpucontext{ pvkcdevice } {
      VkDescriptorPoolCreateInfo descriptorPoolInfo{};
      descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
      descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
      descriptorPoolInfo.pPoolSizes = poolSizes.data();
      descriptorPoolInfo.maxSets = maxSets;
      descriptorPoolInfo.flags = poolFlags;

      if (vkCreateDescriptorPool(m_pgpucontext->logicalDevice(), &descriptorPoolInfo, nullptr, &m_vkdescriptorpool) !=
          VK_SUCCESS) {
         throw ::exception(error_failed, "failed to create descriptor pool!");
      }
   }

   descriptor_pool::~descriptor_pool() {

            for (auto &pdescriptorset: m_descriptorset2a)
      {

         pdescriptorset->clear();
      }
      m_descriptorset2a.clear();


      if (m_vkdescriptorpool != VK_NULL_HANDLE) {
         vkDestroyDescriptorPool(m_pgpucontext->logicalDevice(), m_vkdescriptorpool, nullptr);
         m_vkdescriptorpool = VK_NULL_HANDLE;
      }
   }

   bool descriptor_pool::allocateDescriptor(
       const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet & descriptor, uint32_t variableDescriptorCount) const
   {
      VkDescriptorSetAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
      allocInfo.descriptorPool = m_vkdescriptorpool;
      allocInfo.pSetLayouts = &descriptorSetLayout;
      allocInfo.descriptorSetCount = 1;


          // Optional variable descriptor count extension
      VkDescriptorSetVariableDescriptorCountAllocateInfo countInfo{};
      if (variableDescriptorCount > 0)
      {
         countInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
         countInfo.descriptorSetCount = 1;
         countInfo.pDescriptorCounts = &variableDescriptorCount;

         allocInfo.pNext = &countInfo; // chain if used
      }

      // Might want to create a "DescriptorPoolManager" class that handles this case, and builds
      // a new pool whenever an old pool fills up. But this is beyond our current scope
      if (vkAllocateDescriptorSets(m_pgpucontext->logicalDevice(), &allocInfo, &descriptor) != VK_SUCCESS) {
         return false;
      }
      return true;
   }

   void descriptor_pool::freeDescriptors(::pointer<descriptor_set_array> &&pdescriptorseta) const
   {
      auto p = ::transfer(pdescriptorseta);
      vkFreeDescriptorSets(
          m_pgpucontext->logicalDevice(),
          m_vkdescriptorpool,
                                             static_cast<uint32_t>(p->size()),
          p->data());
   }

   void descriptor_pool::resetPool() {
      for (auto & pdescriptorset : m_descriptorset2a)
      {

         pdescriptorset->clear();

      }
      m_descriptorset2a.clear();
      vkResetDescriptorPool(m_pgpucontext->logicalDevice(), m_vkdescriptorpool, 0);
   }

   // *************** Descriptor Writer *********************

   descriptor_writer::descriptor_writer(descriptor_set_layout & setLayout, descriptor_pool & pool)
      : m_setLayout{ setLayout }, pool{ pool } {
   }

   descriptor_writer & descriptor_writer::writeBuffer(
       uint32_t binding, VkDescriptorBufferInfo bufferInfo) {
      assert(m_setLayout.m_bindings.count(binding) == 1 && "Layout does not contain specified binding");

      auto & bindingDescription = m_setLayout.m_bindings[binding];

      assert(
          bindingDescription.descriptorCount == 1 &&
          "Binding single descriptor info, but binding expects multiple");

      VkWriteDescriptorSet vkwritedescriptorset{};
      vkwritedescriptorset.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      vkwritedescriptorset.descriptorType = bindingDescription.descriptorType;
      vkwritedescriptorset.dstBinding = binding;
      vkwritedescriptorset.pBufferInfo = &bufferInfo;
      vkwritedescriptorset.descriptorCount = 1;

      m_vkwritedescriptorseta.add(vkwritedescriptorset);
      return *this;
   }


   descriptor_writer & descriptor_writer::writeImage(
       uint32_t binding, VkDescriptorImageInfo * imageInfo) 
   {

      assert(m_setLayout.m_bindings.count(binding) == 1 && "Layout does not contain specified binding");

      auto & bindingDescription = m_setLayout.m_bindings[binding];

      assert(
          bindingDescription.descriptorCount == 1 &&
          "Binding single descriptor info, but binding expects multiple");

      VkWriteDescriptorSet vkwritedescriptorset{};
      vkwritedescriptorset.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      vkwritedescriptorset.descriptorType = bindingDescription.descriptorType;
      vkwritedescriptorset.dstBinding = binding;
      vkwritedescriptorset.pImageInfo = imageInfo;
      vkwritedescriptorset.descriptorCount = 1;

      m_vkwritedescriptorseta.add(vkwritedescriptorset);
      return *this;
   }


   descriptor_writer &descriptor_writer::writeImage(uint32_t binding, const VkDescriptorImageInfo *imageInfos,
                                                    uint32_t count)
   {
      ASSERT(m_setLayout.m_bindings.count(binding) == 1 && "Layout does not contain specified binding");
      auto &bindingDescriptorCount = m_setLayout.m_bindings[binding].descriptorCount;
      ASSERT(bindingDescriptorCount >= count && "Too many image descriptors for binding");


      m_uVariableDescriptorCount = count;
      VkWriteDescriptorSet vkwritedescriptorset{};
      vkwritedescriptorset.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      vkwritedescriptorset.dstBinding = binding;
      vkwritedescriptorset.dstArrayElement = 0;
      vkwritedescriptorset.descriptorType = m_setLayout.m_bindings[binding].descriptorType;
      vkwritedescriptorset.descriptorCount = count;
      vkwritedescriptorset.pImageInfo = imageInfos;
      vkwritedescriptorset.pBufferInfo = nullptr;
      vkwritedescriptorset.pTexelBufferView = nullptr;

      m_vkwritedescriptorseta.add(vkwritedescriptorset);
      return *this;
   }

   bool descriptor_writer::build(VkDescriptorSet & set) {
      bool success = pool.allocateDescriptor(m_setLayout.getDescriptorSetLayout(), 
         set, m_uVariableDescriptorCount);
      if (!success) {
         return false;
      }
      overwrite(set);
      return true;
   }

   void descriptor_writer::overwrite(VkDescriptorSet & set) {
      for (auto & vkwritedescriptorset : m_vkwritedescriptorseta) {
         vkwritedescriptorset.dstSet = set;
      }
      vkUpdateDescriptorSets(pool.m_pgpucontext->logicalDevice(), 
         (uint32_t)m_vkwritedescriptorseta.size(),
                             m_vkwritedescriptorseta.data(), 0, nullptr);
   }


} // namespace gpu_vulkan




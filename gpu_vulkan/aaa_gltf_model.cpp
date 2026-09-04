///*
//* Vulkan gltf model and texture loading class based on tinyglTF (https://github.com/syoyo/tinygltf)
//*
//* Copyright (C) 2018-2024 by Sascha Willems - www.saschawillems.de
//*
//* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
//*/
//
///*
// * Note that this isn't a complete gltf loader and not all features of the gltf 2.0 spec are supported
// * For details on how gltf 2.0 works, see the official spec at
// https://github.com/KhronosGroup/gltf/tree/master/specification/2.0
// *
// * If you are looking for a complete gltf implementation, check out https://github.com/SaschaWillems/Vulkan-gltf-PBR/
// */
#include "platform.h"
#include "binding.h"
#include "command_buffer.h"
#include "context.h"
#include "descriptors.h"
#include "device.h"
#include "gltf_model.h"
#include <acme/constant/status.h>
#include <acme/exception/exception.h>
#include <acme/filesystem/filesystem/path.h>
#include <acme/include/_initial_concepts.h>
#include <acme/include/_template_forward_declaration.h>
#include <acme/platform/_referencing_debugging.h>
#include <acme/prototype/collection/array_base.h>
#include <acme/prototype/prototype/cast.h>
#include <acme/prototype/prototype/pointer.h>
#include <array>
#include <assimp/material.h>
#include <bred/gpu/_constant.h>
#include <bred/gpu/_types.h>
#include <bred/gpu/context.h>
#include "bred/gpu/properties.h"
#include "bred/gltf/vertex.h"
#include "bred/gpu/texture.h"
#include <cassert>
#include <cfloat>
#include <cmath>
#include <cstdint>
#include <string.h>
#include <string>
#include <tiny_gltf.h>
#include <vector>
#include <vulkan/vulkan_core.h>
#include "_gpu_vulkan.h"
#include "acme/filesystem/filesystem/file_context.h"
#include "aura/graphics/image/context.h"
#include "bred/gpu/render_target.h"
#include "bred/gpu/renderer.h"
#include "gpu_vulkan/physical_device.h"
#include "gpu_vulkan/queue.h"
#include "gpu_vulkan/texture.h"
#include "bred/graphics3d/engine.h"
#include "vk_init.h"
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE

// #include "vulkan_wrapper/vulkan_gltf.h"
// #include "vk_tools/vk_tools.h"


// #include <fstream>
// #include <iostream>
// #include <string>
// #include <vector>
//
// #define TINYGLTF_IMPLEMENTATION
// #define STB_IMAGE_IMPLEMENTATION
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "tiny_gltf.h"

// ---------------------------------------------------------
// Global filesystem callbacks
// ---------------------------------------------------------

// bool tinygltf_example_FileExists(const std::string &abs_filename, void *user_data)
//{
//    std::ifstream f(abs_filename.c_str());
//    return f.good();
// }

// bool tinygltf_example_ReadWholeFile(std::vector<unsigned char> *out, std::string *err, const std::string &filepath,
//                                     void *user_data)
//{
//    std::ifstream f(filepath, std::ios::binary);
//    if (!f)
//    {
//       if (err)
//          *err = "Could not open file: " + filepath;
//       return false;
//    }
//    f.seekg(0, std::ios::end);
//    size_t sz = static_cast<size_t>(f.tellg());
//    f.seekg(0, std::ios::beg);
//
//    out->resize(sz);
//    f.read(reinterpret_cast<char *>(out->data()), sz);
//    return true;
// }
//
// bool tinygltf_example_WriteWholeFile(std::string *err, const std::string &filepath, const std::vector<unsigned char>
// &contents,
//                     void *user_data)
//{
//    std::ofstream f(filepath, std::ios::binary);
//    if (!f)
//    {
//       if (err)
//          *err = "Could not write file: " + filepath;
//       return false;
//    }
//    f.write(reinterpret_cast<const char *>(contents.data()), contents.size());
//    return true;
// }

//// ---------------------------------------------------------
//// Global image loader callback
//// ---------------------------------------------------------
//
// bool tinygltf_example_LoadImageData(tinygltf::Image *image, const int image_idx, std::string *err, std::string *warn,
//                                    int req_width,
//                   int req_height, const unsigned char *bytes, int size, void *user_data)
//{
//   // Decode using stb_image (already included by tinygltf)
//   int w, h, comp;
//   unsigned char *data = stbi_load_from_memory(bytes, size, &w, &h, &comp, 0);
//   if (!data)
//   {
//      if (err)
//         *err = "Failed to decode image " + image->uri;
//      return false;
//   }
//
//   image->width = w;
//   image->height = h;
//   image->component = comp;
//   image->image.assign(data, data + (w * h * comp));
//   image->bits = 8;
//   image->pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
//
//   stbi_image_free(data);
//   return true;
//}

//// ---------------------------------------------------------
//// Main
//// ---------------------------------------------------------
//
// int tinygltf_example_main(int argc, char **argv)
//{
//   if (argc < 2)
//   {
//      std::cerr << "Usage: " << argv[0] << " model.gltf" << std::endl;
//      return -1;
//   }
//
//   std::string filename(argv[1]);
//
//   tinygltf::Model model;
//   tinygltf::TinyGLTF loader;
//   std::string err, warn;
//
//   // Set custom filesystem callbacks
//   tinygltf::FsCallbacks fsCallbacks;
//   fsCallbacks.FileExists = FileExists;
//   fsCallbacks.ReadWholeFile = ReadWholeFile;
//   fsCallbacks.WriteWholeFile = WriteWholeFile;
//   fsCallbacks.ExpandFilePath = nullptr; // use default
//   fsCallbacks.user_data = nullptr;
//   loader.SetFsCallbacks(fsCallbacks);
//
//   // Set custom image loader
//   loader.SetImageLoader(LoadImageData, nullptr);
//
//   bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
//
//   if (!warn.empty())
//      std::cout << "Warn: " << warn << std::endl;
//   if (!err.empty())
//      std::cerr << "Err: " << err << std::endl;
//   if (!ret)
//   {
//      std::cerr << "Failed to load " << filename << std::endl;
//      return -1;
//   }
//
//   std::cout << "Loaded glTF file: " << filename << std::endl;
//
//   // Print buffers
//   for (size_t i = 0; i < model.buffers.size(); i++)
//   {
//      const auto &buffer = model.buffers[i];
//      std::cout << "Buffer[" << i << "] size: " << buffer.data.size() << " bytes" << std::endl;
//   }
//
//   // Print images
//   for (size_t i = 0; i < model.images.size(); i++)
//   {
//      const auto &image = model.images[i];
//      std::cout << "Image[" << i << "] : " << image.uri << " (" << image.width << "x" << image.height
//                << ", comp=" << image.component << ")" << std::endl;
//   }
//
//   return 0;
//}


namespace gpu_vulkan
{

   // #include <fstream>
   // #include <iostream>
   // #include <string>
   // #include <vector>
   //
   // #define TINYGLTF_IMPLEMENTATION
   // #define STB_IMAGE_IMPLEMENTATION
   // #define STB_IMAGE_WRITE_IMPLEMENTATION
   // #include "tiny_gltf.h"
   //
   //  ---------------------------------------------------------
   //  Global filesystem callbacks
   //  ---------------------------------------------------------

   bool tinygltf_FileExists(const std::string &abs_filename, void *user_data)
   {

      auto pfile = (file_context *)user_data;

      ::file::path path(abs_filename.c_str());

      return pfile->exists(path);
   }


   bool tinygltf_ReadWholeFile(std::vector<unsigned char> *out, std::string *err, const std::string &filepath,
                               void *user_data)
   {


      auto pfile = (file_context *)user_data;

      try
      {

         ::file::path path(filepath.c_str());

         auto memory = pfile->as_memory(path);

         out->assign(memory.begin(), memory.end());
      }
      catch (...)
      {

         return false;
      }

      // std::ifstream f(filepath, std::ios::binary);
      // if (!f)
      //{
      //    if (err)
      //       *err = "Could not open file: " + filepath;
      //    return false;
      // }
      // f.seekg(0, std::ios::end);
      // size_t sz = static_cast<size_t>(f.tellg());
      // f.seekg(0, std::ios::beg);
      // f.read(reinterpret_cast<char *>(out->data()), sz);
      return true;
   }


   // ExpandFilePath callback: normalize/adjust file paths
   std::string tinygltf_ExpandFilePath(const std::string &filename, void *user_data)
   {
      //// Example: prepend a base asset directory (stored in user_data)
      // const char *baseDir = static_cast<const char *>(user_data);

      // std::filesystem::path base(baseDir ? baseDir : "");
      // d::filesystem::path file(filename);
      return filename;
      // Combine and normalize
      // std::filesystem::path full = std::filesystem::weakly_canonical(base / file);
      // return full.string();
   }


   bool tinygltf_WriteWholeFile(std::string *err, const std::string &filepath,
                                const std::vector<unsigned char> &contents, void *user_data)
   {
      auto pfile = (file_context *)user_data;

      try
      {

         ::file::path path(filepath.c_str());

         pfile->put_memory(path, {contents.data(), contents.size()});
      }
      catch (...)
      {

         return false;
      }

      return true;
      // std::ofstream f(filepath, std::ios::binary);
      // if (!f)
      //{
      //    if (err)
      //       *err = "Could not write file: " + filepath;
      //    return false;
      // }
      // f.write(reinterpret_cast<const char *>(contents.data()), contents.size());
      // return true;
   }

   // ---------------------------------------------------------
   // Global image loader callback
   // ---------------------------------------------------------

   bool tinygltf_LoadImageData(tinygltf::Image *image, const int image_idx, std::string *err, std::string *warn,
                               int req_width, int req_height, const unsigned char *bytes, int size, void *user_data)
   {

      auto pimagecontext = (::image::image_context *)user_data;


      // Decode using stb_image (already included by tinygltf)
      int w, h, comp;
      unsigned char *data = pimagecontext->like_stbi_load_from_memory(bytes, size, &w, &h, &comp, 0);
      if (!data)
      {
         if (err)
            *err = "Failed to decode image " + image->uri;
         return false;
      }

      image->width = w;
      image->height = h;
      image->component = comp;
      image->image.assign(data, data + (w * h * comp));
      image->bits = 8;
      image->pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;

      pimagecontext->like_stbi_image_free(data);
      return true;
   }


   bool loadImageDataFuncEmpty(tinygltf::Image *image, const int imageIndex, std::string *error, std::string *warning,
                               int req_width, int req_height, const unsigned char *bytes, int size, void *userData)
   {
      // This function will be used for samples that don't require images to be loaded
      return true;
   }


   gltf::Model::Model() {}


   ///*
   //   gltf material
   //*/
   //void gltf::Material::addDescriptor(int iCount, ::array_base<VkDescriptorSet> &a, VkDescriptorPool descriptorPool,
   //                                   aaaVkDescriptorSetLayout descriptorSetLayout, uint32_t descriptorBindingFlags,
   //                                   gpu_vulkan::texture *fallbackTexture)
   //{

   //   ::cast<::gpu_vulkan::context> pcontext = m_pgpucontext;
   //   ::cast<::gpu_vulkan::device> pgpudevice = pcontext->m_pgpudevice;
   //   auto pphysicaldevice = pgpudevice->m_pphysicaldevice;

   //   VkDescriptorSet descriptorSet = VK_NULL_HANDLE;

   //   // Allocate descriptor set
   //   VkDescriptorSetAllocateInfo allocInfo{};
   //   allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
   //   allocInfo.descriptorPool = descriptorPool;
   //   allocInfo.descriptorSetCount = 1;
   //   allocInfo.pSetLayouts = &descriptorSetLayout;
   //   VkCheckResult(vkAllocateDescriptorSets(pcontext->logicalDevice(), &allocInfo, &descriptorSet));

   //   // Prepare image infos with fallback
   //   VkDescriptorImageInfo baseColorImageInfo =
   //      (baseColorTexture && (descriptorBindingFlags & DescriptorBindingFlags::ImageBaseColor))
   //         ? baseColorTexture->m_descriptor3
   //         : fallbackTexture->m_descriptor3;

   //   VkDescriptorImageInfo normalImageInfo =
   //      (normalTexture && (descriptorBindingFlags & DescriptorBindingFlags::ImageNormalMap))
   //         ? normalTexture->m_descriptor3
   //         : fallbackTexture->m_descriptor3;

   //   VkDescriptorImageInfo metallicRoughnessImageInfo =
   //      (metallicRoughnessTexture && (descriptorBindingFlags & DescriptorBindingFlags::ImageMetallicMap))
   //         ? metallicRoughnessTexture->m_descriptor3
   //         : fallbackTexture->m_descriptor3;

   //   // If you have a separate roughness texture, bind it here, otherwise fallback
   //   // VkDescriptorImageInfo roughnessImageInfo = fallbackTexture->m_descriptor3;

   //   VkDescriptorImageInfo occlusionImageInfo =
   //      (occlusionTexture && (descriptorBindingFlags & DescriptorBindingFlags::ImageAOMap))
   //         ? occlusionTexture->m_descriptor3
   //         : fallbackTexture->m_descriptor3;

   //   VkDescriptorImageInfo emissiveImageInfo =
   //      (occlusionTexture && (descriptorBindingFlags & DescriptorBindingFlags::ImageEmissiveMap))
   //         ? emissiveTexture->m_descriptor3
   //         : fallbackTexture->m_descriptor3;

   //   ::array_base<VkWriteDescriptorSet> writeDescriptorSets;

   //   writeDescriptorSets.set_size(iCount);

   //   writeDescriptorSets[0] = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
   //                             nullptr,
   //                             descriptorSet,
   //                             0,
   //                             0,
   //                             1,
   //                             VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
   //                             &baseColorImageInfo,
   //                             nullptr,
   //                             nullptr};
   //   writeDescriptorSets[1] = {
   //      VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,    nullptr,          descriptorSet, 1,      0, 1,
   //      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &normalImageInfo, nullptr,       nullptr};

   //   if (iCount >= 3)
   //   {
   //      writeDescriptorSets[2] = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
   //                                nullptr,
   //                                descriptorSet,
   //                                2,
   //                                0,
   //                                1,
   //                                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
   //                                &metallicRoughnessImageInfo,
   //                                nullptr,
   //                                nullptr};
   //      //    writeDescriptorSets[3] = {
   //      //	VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, descriptorSet, 3, 0, 1,
   //      //	VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &roughnessImageInfo, nullptr, nullptr
   //      //};
   //      writeDescriptorSets[3] = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
   //                                nullptr,
   //                                descriptorSet,
   //                                3,
   //                                0,
   //                                1,
   //                                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
   //                                &occlusionImageInfo,
   //                                nullptr,
   //                                nullptr};
   //      writeDescriptorSets[4] = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
   //                                nullptr,
   //                                descriptorSet,
   //                                4,
   //                                0,
   //                                1,
   //                                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
   //                                &emissiveImageInfo,
   //                                nullptr,
   //                                nullptr};

   //   }

   //   vkUpdateDescriptorSets(
   //      pcontext->logicalDevice(), 
   //      static_cast<uint32_t>(writeDescriptorSets.size()),
   //      writeDescriptorSets.data(), 0, nullptr);

   //   a.add(descriptorSet);
   //}


   //::array_base<VkDescriptorSet> &gltf::Material::descriptor_set_array_gltf()
   //{

   //   if (m_descriptorsetaGltf4.is_empty())
   //   {
   //   }
   //}

   //::array_base<VkDescriptorSet> &descriptor_set_array_scene_gltf();


   // void gltf::terial::addDescriptor(
   //       ::array_base<VkDescriptorSet> &a,
   //       VkDescriptorPool descriptorPool,
   //       aaaVkDescriptorSetLayout descriptorSetLayout,
   //       uint32_t descriptorBindingFlags, gpu_vulkan::texture *fallbackTexture)
   //{

   //   ::cast<::gpu_vulkan::context> pcontext = m_pgpucontext;
   //   ::cast<::gpu_vulkan::device> pgpudevice = pcontext->m_pgpudevice;
   //   auto pphysicaldevice = pgpudevice->m_pphysicaldevice;

   //   VkDescriptorSet descriptorSet = VK_NULL_HANDLE;

   //   // Allocate descriptor set
   //   VkDescriptorSetAllocateInfo allocInfo{};
   //   allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
   //   allocInfo.descriptorPool = descriptorPool;
   //   allocInfo.descriptorSetCount = 1;
   //   allocInfo.pSetLayouts = &descriptorSetLayout;
   //   VkCheckResult(vkAllocateDescriptorSets(pcontext->logicalDevice(), &allocInfo, &descriptorSet));

   //   // Prepare image infos with fallback
   //   VkDescriptorImageInfo baseColorImageInfo =
   //      (baseColorTexture && (descriptorBindingFlags & DescriptorBindingFlags::ImageBaseColor))
   //         ? baseColorTexture->m_descriptor3
   //         : fallbackTexture->m_descriptor3;

   //   VkDescriptorImageInfo normalImageInfo =
   //      (normalTexture && (descriptorBindingFlags & DescriptorBindingFlags::ImageNormalMap))
   //         ? normalTexture->m_descriptor3
   //         : fallbackTexture->m_descriptor3;

   //   VkDescriptorImageInfo metallicRoughnessImageInfo =
   //      (metallicRoughnessTexture && (descriptorBindingFlags & DescriptorBindingFlags::ImageMetallicMap))
   //         ? metallicRoughnessTexture->m_descriptor3
   //         : fallbackTexture->m_descriptor3;

   //   // If you have a separate roughness texture, bind it here, otherwise fallback
   //   // VkDescriptorImageInfo roughnessImageInfo = fallbackTexture->m_descriptor3;

   //   VkDescriptorImageInfo occlusionImageInfo =
   //      (occlusionTexture && (descriptorBindingFlags & DescriptorBindingFlags::ImageAOMap))
   //         ? occlusionTexture->m_descriptor3
   //         : fallbackTexture->m_descriptor3;

   //   VkDescriptorImageInfo emissiveImageInfo =
   //      (occlusionTexture && (descriptorBindingFlags & DescriptorBindingFlags::ImageEmissiveMap))
   //         ? emissiveTexture->m_descriptor3
   //         : fallbackTexture->m_descriptor3;

   //   std::array<VkWriteDescriptorSet, 5> writeDescriptorSets{};

   //   writeDescriptorSets[0] = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
   //                             nullptr,
   //                             descriptorSet,
   //                             0,
   //                             0,
   //                             1,
   //                             VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
   //                             &baseColorImageInfo,
   //                             nullptr,
   //                             nullptr};
   //   writeDescriptorSets[1] = {
   //      VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,    nullptr,          descriptorSet, 1,      0, 1,
   //      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &normalImageInfo, nullptr,       nullptr};
   //   writeDescriptorSets[2] = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
   //                             nullptr,
   //                             descriptorSet,
   //                             2,
   //                             0,
   //                             1,
   //                             VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
   //                             &metallicRoughnessImageInfo,
   //                             nullptr,
   //                             nullptr};
   //   //    writeDescriptorSets[3] = {
   //   //	VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, descriptorSet, 3, 0, 1,
   //   //	VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &roughnessImageInfo, nullptr, nullptr
   //   //};
   //   writeDescriptorSets[3] = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
   //                             nullptr,
   //                             descriptorSet,
   //                             3,
   //                             0,
   //                             1,
   //                             VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
   //                             &occlusionImageInfo,
   //                             nullptr,
   //                             nullptr};
   //   writeDescriptorSets[4] = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
   //                             nullptr,
   //                             descriptorSet,
   //                             4,
   //                             0,
   //                             1,
   //                             VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
   //                             &emissiveImageInfo,
   //                             nullptr,
   //                             nullptr};

   //   vkUpdateDescriptorSets(pcontext->logicalDevice(), static_cast<uint32_t>(writeDescriptorSets.size()),
   //                          writeDescriptorSets.data(), 0, nullptr);

   //   a.add(descriptorSet);
   //}


   /*
      gltf primitive
   */
   void gltf::Primitive::setDimensions(floating_sequence3 min, floating_sequence3 max)
   {
      dimensions.min = min;
      dimensions.max = max;
      dimensions.size = max - min;
      dimensions.center = (min + max) / 2.0f;
      dimensions.radius = min.distance(max) / 2.0f;
   }

   /*
      gltf mesh
   */
   gltf::Mesh::Mesh(::gpu::context *pgpucontext, floating_matrix4 matrix)
   {
      this->m_pgpucontext = pgpucontext;
      ::cast<::gpu_vulkan::context> pcontext = m_pgpucontext;
      ::cast<::gpu_vulkan::device> pgpudevice = pcontext->m_pgpudevice;
      auto pphysicaldevice = pgpudevice->m_pphysicaldevice;

      this->uniformBlock.matrix = matrix;
      VkCheckResult(pgpudevice->createBuffer(
         VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
         sizeof(uniformBlock), &uniformBuffer.buffer, &uniformBuffer.memory, &uniformBlock));
      VkCheckResult(vkMapMemory(pcontext->logicalDevice(), uniformBuffer.memory, 0, sizeof(uniformBlock), 0,
                                  &uniformBuffer.mapped));
      uniformBuffer.descriptor = {uniformBuffer.buffer, 0, sizeof(uniformBlock)};
   };

   gltf::Mesh::~Mesh()
   {
      ::cast<::gpu_vulkan::context> pcontext = m_pgpucontext;
      ::cast<::gpu_vulkan::device> pgpudevice = pcontext->m_pgpudevice;
      auto pphysicaldevice = pgpudevice->m_pphysicaldevice;

      vkDestroyBuffer(pcontext->logicalDevice(), uniformBuffer.buffer, nullptr);
      vkFreeMemory(pcontext->logicalDevice(), uniformBuffer.memory, nullptr);
      for (auto primitive: primitives)
      {
         delete primitive;
      }
   }

   /*
      gltf node
   */
   floating_matrix4 gltf::Node::localMatrix()
   {

      auto T = floating_matrix4::translation(m_sequence3Translation);

      auto R = floating_matrix4(m_matrixRotation);

      auto S = floating_matrix4::scaling(m_sequence3Scale);

      auto M = m_matrix;

      return T * R * S * M;
   }


   floating_matrix4 gltf::Node::getMatrix()
   {

      floating_matrix4 matrix = localMatrix();

      Node *pnode = m_pnodeParent;

      while (pnode)
      {

         matrix = pnode->localMatrix() * matrix;

         pnode = pnode->m_pnodeParent;
      }

      return matrix;
   }


   void gltf::Node::update()
   {

      if (m_pmesh)
      {
         floating_matrix4 m = getMatrix();
         if (m_pskin)
         {
            m_pmesh->uniformBlock.matrix = m;
            // Update join matrices
            floating_matrix4 inverseTransform = m.inversed();
            for (size_t i = 0; i < m_pskin->joints.size(); i++)
            {
               Node *jointNode = m_pskin->joints[i];
               floating_matrix4 jointMat = jointNode->getMatrix() * m_pskin->inverseBindMatrices[i];
               jointMat = inverseTransform * jointMat;
               m_pmesh->uniformBlock.jointMatrix[i] = jointMat;
            }
            m_pmesh->uniformBlock.jointcount = (float)m_pskin->joints.size();
            memcpy(m_pmesh->uniformBuffer.mapped, &m_pmesh->uniformBlock, sizeof(m_pmesh->uniformBlock));
         }
         else
         {
            memcpy(m_pmesh->uniformBuffer.mapped, &m, sizeof(floating_matrix4));
         }
      }

      for (auto &pnodeChild: m_nodeaChildren)
      {
         pnodeChild->update();
      }
   }

   gltf::Node::~Node()
   {
      if (m_pmesh)
      {
         delete m_pmesh;
      }
      for (auto &pnodeChild: m_nodeaChildren)
      {
         delete pnodeChild;
      }
   }

   gpu_vulkan::texture *gltf::Model::getTexture(uint32_t uIndex)
   {

      if (uIndex < m_texturea.size())
      {
         return m_texturea[uIndex];
      }
      return nullptr;
   }



   /*
      gltf model loading and rendering class
   */
   gltf::Model::~Model()
   {


      ::cast<::gpu_vulkan::context> pcontext = m_pgpucontext;
      ::cast<::gpu_vulkan::device> pgpudevice = pcontext->m_pgpudevice;
      auto pphysicaldevice = pgpudevice->m_pphysicaldevice;

      vkDestroyBuffer(pcontext->logicalDevice(), vertices.buffer, nullptr);
      vkFreeMemory(pcontext->logicalDevice(), vertices.memory, nullptr);
      vkDestroyBuffer(pcontext->logicalDevice(), indices.buffer, nullptr);
      vkFreeMemory(pcontext->logicalDevice(), indices.memory, nullptr);
      // for (auto texture : m_texturea) {
      //	texture.destroy();
      // }
      for (auto pnode: m_nodea)
      {
         delete pnode;
      }
      for (auto pskin: m_skina)
      {
         delete pskin;
      }
      // if (descriptorSetLayoutUbo != VK_NULL_HANDLE) {
      //	vkDestroyDescriptorSetLayout(pcontext->logicalDevice(), descriptorSetLayoutUbo, nullptr);
      //	descriptorSetLayoutUbo = VK_NULL_HANDLE;
      // }
      // if (descriptorSetLayoutImage != VK_NULL_HANDLE) {
      //	vkDestroyDescriptorSetLayout(pcontext->logicalDevice(), descriptorSetLayoutImage, nullptr);
      //	descriptorSetLayoutImage = VK_NULL_HANDLE;
      // }
      aaavkDestroyDescriptorPool(pcontext->logicalDevice(), m_descriptorPool, nullptr);
      //emptyTexture->destroy();
   }


   // Compute tangents from indexed mesh data
   void computeTangents(std::vector<::gpu::gltf::vertex> &vertices, const std::vector<uint32_t> &indices)
   {
      std::vector<floating_sequence3> tan1(vertices.size(), floating_sequence3(0.0f));
      std::vector<floating_sequence3> tan2(vertices.size(), floating_sequence3(0.0f));

      // Step 1: accumulate per-triangle tangents and bitangents
      for (size_t i = 0; i < indices.size(); i += 3)
      {
         uint32_t i1 = indices[i + 0];
         uint32_t i2 = indices[i + 1];
         uint32_t i3 = indices[i + 2];

         const ::floating_sequence3 &v1 = vertices[i1].position;
         const ::floating_sequence3 &v2 = vertices[i2].position;
         const ::floating_sequence3 &v3 = vertices[i3].position;

         const floating_sequence2 &w1 = vertices[i1].uv;
         const floating_sequence2 &w2 = vertices[i2].uv;
         const floating_sequence2 &w3 = vertices[i3].uv;

         floating_sequence3 e1 = v2 - v1;
         floating_sequence3 e2 = v3 - v1;

         float x1 = e1.x, x2 = e2.x;
         float y1 = e1.y, y2 = e2.y;
         float z1 = e1.z, z2 = e2.z;

         float s1 = w2.x - w1.x;
         float s2 = w3.x - w1.x;
         float t1 = w2.y - w1.y;
         float t2 = w3.y - w1.y;

         float r = (s1 * t2 - s2 * t1);
         if (fabs(r) < 1e-10f)
            r = 1.0f;
         else
            r = 1.0f / r;

         floating_sequence3 sdir = (e1 * t2 - e2 * t1) * r;
         floating_sequence3 tdir = (e2 * s1 - e1 * s2) * r;

         tan1[i1] += sdir;
         tan1[i2] += sdir;
         tan1[i3] += sdir;

         tan2[i1] += tdir;
         tan2[i2] += tdir;
         tan2[i3] += tdir;
      }

      // Step 2: orthogonalize and compute handedness
      for (size_t i = 0; i < vertices.size(); ++i)
      {
         const auto &n = vertices[i].normal;
         const auto &t = tan1[i];

         // Gram-Schmidt orthogonalize
         auto tangent = (t - n * n.dotted(t)).normalized();

         // Determine handedness (sign)
         float sign = (n.crossed(t).dotted(tan2[i]) < 0.0f) ? -1.0f : 1.0f;

         vertices[i].tangent.x = tangent.x;
         vertices[i].tangent.y = tangent.y;
         vertices[i].tangent.z = tangent.z;
         vertices[i].tangent.w = sign;
      }
   }

   void gltf::Model::loadNode(Node *pnodeParent, const tinygltf::Node &node, uint32_t nodeIndex,
                              const tinygltf::Model &model, std::vector<uint32_t> &indexBuffer,
                              std::vector<::gpu::gltf::vertex> &vertexBuffer, float globalscale)
   {
      Node *newNode = new Node{};
      newNode->m_uIndex = nodeIndex;
      newNode->m_pnodeParent = pnodeParent;
      newNode->m_strName = node.name.c_str();
      newNode->m_iSkinIndex = node.skin;
      newNode->m_matrix = floating_matrix4(1.0f);


      ::cast<::gpu_vulkan::context> pcontext = m_pgpucontext;
      ::cast<::gpu_vulkan::device> pgpudevice = pcontext->m_pgpudevice;
      auto pphysicaldevice = pgpudevice->m_pphysicaldevice;


      // Generate local node matrix
      floating_sequence3 translation = floating_sequence3(0.0f);
      if (node.translation.size() == 3)
      {
         translation = floating_sequence3(node.translation.data());
         newNode->m_sequence3Translation = translation;
      }
      floating_matrix4 rotation = floating_matrix4(1.0f);
      if (node.rotation.size() == 4)
      {
         auto p = node.rotation.data();
         auto quaternion = floating_quaternion(p[3], p[0], p[1], p[2]);
         newNode->m_matrixRotation = floating_matrix4(quaternion);
      }
      floating_sequence3 scale = floating_sequence3(1.0f);
      if (node.scale.size() == 3)
      {
         scale = floating_sequence3(node.scale.data());
         newNode->m_sequence3Scale = scale;
      }
      if (node.matrix.size() == 16)
      {
         newNode->m_matrix = floating_matrix4(node.matrix.data());
         if (globalscale != 1.0f)
         {
            // newNode->m_matrix = glm::scale(newNode->m_matrix, floating_sequence3(globalscale));
         }
      };

      // Node with children
      if (node.children.size() > 0)
      {
         for (auto i = 0; i < node.children.size(); i++)
         {
            loadNode(newNode, model.nodes[node.children[i]], node.children[i], model, indexBuffer, vertexBuffer,
                     globalscale);
         }
      }

      // Node contains mesh data
      if (node.mesh > -1)
      {
         const tinygltf::Mesh mesh = model.meshes[node.mesh];
         Mesh *newMesh = new Mesh(pcontext, newNode->m_matrix);
         newMesh->name = mesh.name;
         for (size_t j = 0; j < mesh.primitives.size(); j++)
         {
            const tinygltf::Primitive &primitive = mesh.primitives[j];
            if (primitive.indices < 0)
            {
               continue;
            }
            uint32_t indexStart = static_cast<uint32_t>(indexBuffer.size());
            uint32_t vertexStart = static_cast<uint32_t>(vertexBuffer.size());
            uint32_t indexCount = 0;
            uint32_t vertexCount = 0;
            floating_sequence3 posMin{};
            floating_sequence3 posMax{};
            bool hasSkin = false;
            bool bHasStoredTangents = false;


            // Vertices
            {
               const float *bufferPos = nullptr;
               const float *bufferNormals = nullptr;
               const float *bufferTexCoords = nullptr;
               const float *bufferColors = nullptr;
               const float *bufferTangents = nullptr;
               uint32_t numColorComponents;
               const uint16_t *bufferJoints = nullptr;
               const float *bufferWeights = nullptr;

               // Position attribute is required
               assert(primitive.attributes.find("POSITION") != primitive.attributes.end());

               const tinygltf::Accessor &posAccessor = model.accessors[primitive.attributes.find("POSITION")->second];
               const tinygltf::BufferView &posView = model.bufferViews[posAccessor.bufferView];
               bufferPos = reinterpret_cast<const float *>(
                  &(model.buffers[posView.buffer].data[posAccessor.byteOffset + posView.byteOffset]));
               posMin =
                  floating_sequence3(posAccessor.minValues[0], posAccessor.minValues[1], posAccessor.minValues[2]);
               posMax =
                  floating_sequence3(posAccessor.maxValues[0], posAccessor.maxValues[1], posAccessor.maxValues[2]);

               if (primitive.attributes.find("NORMAL") != primitive.attributes.end())
               {
                  const tinygltf::Accessor &normAccessor = model.accessors[primitive.attributes.find("NORMAL")->second];
                  const tinygltf::BufferView &normView = model.bufferViews[normAccessor.bufferView];
                  bufferNormals = reinterpret_cast<const float *>(
                     &(model.buffers[normView.buffer].data[normAccessor.byteOffset + normView.byteOffset]));
               }

               if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end())
               {
                  const tinygltf::Accessor &uvAccessor =
                     model.accessors[primitive.attributes.find("TEXCOORD_0")->second];
                  const tinygltf::BufferView &uvView = model.bufferViews[uvAccessor.bufferView];
                  bufferTexCoords = reinterpret_cast<const float *>(
                     &(model.buffers[uvView.buffer].data[uvAccessor.byteOffset + uvView.byteOffset]));
               }

               if (primitive.attributes.find("COLOR_0") != primitive.attributes.end())
               {
                  const tinygltf::Accessor &colorAccessor =
                     model.accessors[primitive.attributes.find("COLOR_0")->second];
                  const tinygltf::BufferView &colorView = model.bufferViews[colorAccessor.bufferView];
                  // Color buffer are either of type vec3 or vec4
                  numColorComponents = colorAccessor.type == TINYGLTF_PARAMETER_TYPE_FLOAT_VEC3 ? 3 : 4;
                  bufferColors = reinterpret_cast<const float *>(
                     &(model.buffers[colorView.buffer].data[colorAccessor.byteOffset + colorView.byteOffset]));
               }

               if (primitive.attributes.find("TANGENT") != primitive.attributes.end())
               {
                  bHasStoredTangents = true;
                  const tinygltf::Accessor &tangentAccessor =
                     model.accessors[primitive.attributes.find("TANGENT")->second];
                  const tinygltf::BufferView &tangentView = model.bufferViews[tangentAccessor.bufferView];
                  bufferTangents = reinterpret_cast<const float *>(
                     &(model.buffers[tangentView.buffer].data[tangentAccessor.byteOffset + tangentView.byteOffset]));
               }

               // Skinning
               // Joints
               if (primitive.attributes.find("JOINTS_0") != primitive.attributes.end())
               {
                  const tinygltf::Accessor &jointAccessor =
                     model.accessors[primitive.attributes.find("JOINTS_0")->second];
                  const tinygltf::BufferView &jointView = model.bufferViews[jointAccessor.bufferView];
                  bufferJoints = reinterpret_cast<const uint16_t *>(
                     &(model.buffers[jointView.buffer].data[jointAccessor.byteOffset + jointView.byteOffset]));
               }

               if (primitive.attributes.find("WEIGHTS_0") != primitive.attributes.end())
               {
                  const tinygltf::Accessor &uvAccessor =
                     model.accessors[primitive.attributes.find("WEIGHTS_0")->second];
                  const tinygltf::BufferView &uvView = model.bufferViews[uvAccessor.bufferView];
                  bufferWeights = reinterpret_cast<const float *>(
                     &(model.buffers[uvView.buffer].data[uvAccessor.byteOffset + uvView.byteOffset]));
               }

               hasSkin = (bufferJoints && bufferWeights);

               vertexCount = static_cast<uint32_t>(posAccessor.count);

               for (size_t v = 0; v < posAccessor.count; v++)
               {
                  ::gpu::gltf::vertex vert{};
                  vert.position = floating_sequence4(floating_sequence3(&bufferPos[v * 3]), 1.0f);
                  vert.normal = floating_sequence3(bufferNormals ? floating_sequence3(&bufferNormals[v * 3])
                                                                 : floating_sequence3(0.0f))
                                   .normalized();
                  vert.uv =
                     bufferTexCoords ? floating_sequence2(&bufferTexCoords[v * 2]) : floating_sequence2(0.0f);
                  if (bufferColors)
                  {
                     switch (numColorComponents)
                     {
                        case 3:
                           vert.color = floating_sequence4(floating_sequence3(&bufferColors[v * 3]), 1.0f);
                        case 4:
                           vert.color = floating_sequence4(&bufferColors[v * 4]);
                     }
                  }
                  else
                  {
                     vert.color = floating_sequence4(1.0f);
                  }
                  vert.tangent = bufferTangents ? floating_sequence4(floating_sequence4(&bufferTangents[v * 4]))
                                                : floating_sequence4(0.0f);
                  // vert.joint0 = hasSkin ? floating_sequence4(glm::make_vec4(&bufferJoints[v * 4])) :
                  // floating_sequence4(0.0f); vert.weight0 = hasSkin ? glm::make_vec4(&bufferWeights[v * 4]) :
                  // floating_sequence4(0.0f);
                  vertexBuffer.push_back(vert);
               }
            }

            // Indices
            {
               const tinygltf::Accessor &accessor = model.accessors[primitive.indices];
               const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
               const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

               indexCount = static_cast<uint32_t>(accessor.count);

               switch (accessor.componentType)
               {
                  case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT:
                  {
                     uint32_t *buf = new uint32_t[accessor.count];
                     memcpy(buf, &buffer.data[accessor.byteOffset + bufferView.byteOffset],
                            accessor.count * sizeof(uint32_t));
                     for (size_t uIndex = 0; uIndex < accessor.count; uIndex++)
                     {
                        indexBuffer.push_back(buf[uIndex] + vertexStart);
                     }
                     delete[] buf;
                     break;
                  }
                  case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT:
                  {
                     uint16_t *buf = new uint16_t[accessor.count];
                     memcpy(buf, &buffer.data[accessor.byteOffset + bufferView.byteOffset],
                            accessor.count * sizeof(uint16_t));
                     for (size_t uIndex = 0; uIndex < accessor.count; uIndex++)
                     {
                        indexBuffer.push_back(buf[uIndex] + vertexStart);
                     }
                     delete[] buf;
                     break;
                  }
                  case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE:
                  {
                     uint8_t *buf = new uint8_t[accessor.count];
                     memcpy(buf, &buffer.data[accessor.byteOffset + bufferView.byteOffset],
                            accessor.count * sizeof(uint8_t));
                     for (size_t uIndex = 0; uIndex < accessor.count; uIndex++)
                     {
                        indexBuffer.push_back(buf[uIndex] + vertexStart);
                     }
                     delete[] buf;
                     break;
                  }
                  default:
                     error() << "Index component type " << accessor.componentType << " not supported!";
                     return;
               }
            }
            if (!bHasStoredTangents)
            {
               computeTangents(vertexBuffer, indexBuffer);
            }
            Primitive *newPrimitive =
               new Primitive(indexStart, indexCount,
                             primitive.material > -1 ? &m_materiala[primitive.material] : &m_materiala.last());
            newPrimitive->firstVertex = vertexStart;
            newPrimitive->vertexCount = vertexCount;
            newPrimitive->setDimensions(posMin, posMax);
            newMesh->primitives.push_back(newPrimitive);
         }
         newNode->m_pmesh = newMesh;
      }
      if (pnodeParent)
      {
         pnodeParent->m_nodeaChildren.add(newNode);
      }
      else
      {
         m_nodea.add(newNode);
      }
      m_nodeaLinear.add(newNode);
   }

   void gltf::Model::loadSkins(tinygltf::Model &gltfModel)
   {
      for (tinygltf::Skin &source: gltfModel.skins)
      {
         Skin *newSkin = new Skin{};
         newSkin->name = source.name;

         // Find skeleton root node
         if (source.skeleton > -1)
         {
            newSkin->skeletonRoot = nodeFromIndex(source.skeleton);
         }

         // Find joint nodes
         for (int jointIndex: source.joints)
         {
            Node *pnode = nodeFromIndex(jointIndex);
            if (pnode)
            {
               newSkin->joints.push_back(nodeFromIndex(jointIndex));
            }
         }

         // Get inverse bind matrices from buffer
         if (source.inverseBindMatrices > -1)
         {
            const tinygltf::Accessor &accessor = gltfModel.accessors[source.inverseBindMatrices];
            const tinygltf::BufferView &bufferView = gltfModel.bufferViews[accessor.bufferView];
            const tinygltf::Buffer &buffer = gltfModel.buffers[bufferView.buffer];
            newSkin->inverseBindMatrices.resize(accessor.count);
            memcpy(newSkin->inverseBindMatrices.data(), &buffer.data[accessor.byteOffset + bufferView.byteOffset],
                   accessor.count * sizeof(floating_matrix4));
         }

         m_skina.add(newSkin);
      }
   }


   void gltf::Model::loadImages(tinygltf::Model &gltfModel, ::gpu::context *pgpucontext, VkQueue transferQueue)
   {

      for (tinygltf::Image &image: gltfModel.images)
      {
         ::pointer<::gpu_vulkan::texture> ptexture;
         construct_newø(ptexture);
         ptexture->_fromglTfImage(&image, m_pathRenderable.folder(), pgpucontext->m_pgpurenderer, false);
         ptexture->m_iIndex = m_texturea.size();
         m_texturea.add(ptexture);

         // auto memory = file()->as_memory(image.m_path.c_str());
         // texture.fromglTfImage(image, memory.data(), memory.size(), pgpucontext, transferQueue, false);
         // texture.uIndex = static_cast<uint32_t>(m_texturea.size());
         // m_texturea.push_back(texture);
      }
      // Create an empty texture to be used for empty material images
      createEmptyTexture(transferQueue);
      emptyTexture2->m_descriptor3.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
   }

   ::gpu::texture *gltf::Model::loadMaterialTexture2(const ::scoped_string &scopedstr, tinygltf::Material &material,
                                                     tinygltf::Model &gltfModel, const ::scoped_string &scopedstrType,
                                                     aiTextureType type)
   {

      auto ptexture = loadMaterialTexture(scopedstr, type);
      auto iIndex = m_texturea.add(ptexture);
      gltfModel.textures.resize(iIndex + 1);
      auto &texture = gltfModel.textures[iIndex];
      texture.name = scopedstr;
      texture.source = iIndex;
      material.values[scopedstrType.c_str()].json_double_value["index"] = (int)iIndex;
      return ptexture;
   }

   void gltf::Model::loadMaterials(tinygltf::Model &gltfModel)
   {
      for (tinygltf::Material &mat: gltfModel.materials)
      {
         Material material(m_pgpucontext);

         if (m_bExternalPbr)
         {
            material.baseColorTexture =
               loadMaterialTexture2("albedo.ktx", mat, gltfModel, "baseColorTexture", aiTextureType_DIFFUSE);
            // auto iIndex= m_texturea.add(material.baseColorTexture);
            // gltfModel.textures.resize(iIndex + 1);
            // auto &texture = gltfModel.textures[iIndex];
            // texture.name = "albedo.ktx";
            // texture.source = iIndex;
            // mat.values[].json_double_value["index"] = (int)iIndex;
         }
         else if (mat.values.find("baseColorTexture") != mat.values.end())
         {
            material.baseColorTexture =
               getTexture(gltfModel.textures[mat.values["baseColorTexture"].TextureIndex()].source);
         }
         // Metallic roughness workflow
         if (m_bExternalPbr)
         {
            material.metallicRoughnessTexture =
               loadMaterialTexture2("metallic.ktx", mat, gltfModel, "metallicRoughnessTexture", aiTextureType_UNKNOWN);
         }
         else if (mat.values.find("metallicRoughnessTexture") != mat.values.end())
         {
            material.metallicRoughnessTexture =
               getTexture(gltfModel.textures[mat.values["metallicRoughnessTexture"].TextureIndex()].source);
         }
         if (mat.values.find("roughnessFactor") != mat.values.end())
         {
            material.roughnessFactor = static_cast<float>(mat.values["roughnessFactor"].Factor());
         }
         if (mat.values.find("metallicFactor") != mat.values.end())
         {
            material.metallicFactor = static_cast<float>(mat.values["metallicFactor"].Factor());
         }
         if (mat.values.find("baseColorFactor") != mat.values.end())
         {
            material.baseColorFactor = floating_sequence4(mat.values["baseColorFactor"].ColorFactor().data());
         }
         if (m_bExternalPbr)
         {
            material.normalTexture =
               loadMaterialTexture2("normal.ktx", mat, gltfModel, "normalTexture", aiTextureType_NORMALS);
         }
         else
         {
            if (mat.additionalValues.find("normalTexture") != mat.additionalValues.end())
            {
               material.normalTexture =
                  getTexture(gltfModel.textures[mat.additionalValues["normalTexture"].TextureIndex()].source);
            }
         }
         if (m_bExternalPbr)
         {
            material.emissiveTexture =
               loadMaterialTexture2("emissive.ktx", mat, gltfModel, "emissiveTexture", aiTextureType_EMISSIVE);
         }
         else if (mat.additionalValues.find("emissiveTexture") != mat.additionalValues.end())
         {
            material.emissiveTexture =
               getTexture(gltfModel.textures[mat.additionalValues["emissiveTexture"].TextureIndex()].source);
         }
         if (m_bExternalPbr)
         {
            material.occlusionTexture =
               loadMaterialTexture2("ao.ktx", mat, gltfModel, "occlusionTexture", aiTextureType_LIGHTMAP);
         }
         else if (mat.additionalValues.find("occlusionTexture") != mat.additionalValues.end())
         {
            material.occlusionTexture =
               getTexture(gltfModel.textures[mat.additionalValues["occlusionTexture"].TextureIndex()].source);
         }
         material.alphaMode = Material::ALPHAMODE_OPAQUE;

         auto itAlpha = mat.additionalValues.find("alphaMode");
         if (itAlpha != mat.additionalValues.end())
         {
            const std::string &mode = itAlpha->second.string_value;
            if (mode == "BLEND")
            {
               material.alphaMode = Material::ALPHAMODE_BLEND;
            }
            else if (mode == "MASK")
            {
               material.alphaMode = Material::ALPHAMODE_MASK;
            }
            // else leave as OPAQUE
         }
         if (mat.additionalValues.find("alphaCutoff") != mat.additionalValues.end())
         {
            material.alphaCutoff = static_cast<float>(mat.additionalValues["alphaCutoff"].Factor());
         }

         m_materiala.add(material);
      }
      // Push a default material at the end of the list for meshes with no material assigned
      m_materiala.add(Material(m_pgpucontext));
   }


   // loader.SetImageLoader(
   //    [](tinygltf::Image *image, int index, std::string *err, std::string *warn, int req_width, int req_height,
   //       const unsigned char *bytes, int size, void *user_data)
   //    {
   //       // You can decode image bytes here with stb_image or your own decoder
   //       // Example: load with stb_image
   //       int w, h, comp;
   //       unsigned char *data = stbi_load_from_memory(bytes, size, &w, &h, &comp, req_width > 0 ? req_width : 0);
   //       if (!data)
   //       {
   //          if (err)
   //             *err = "Failed to load image";
   //          return false;
   //       }
   //       image->width = w;
   //       image->height = h;
   //       image->component = comp;
   //       image->image.assign(data, data + w * h * comp);
   //       stbi_image_free(data);
   //       return true;
   //    },
   //    nullptr);


   // loader.SetFsCallbacks({// fileExists
   //                        [](const std::string &path, void *) { return std::filesystem::exists(path); },
   //                        // readWholeFile
   //                        [](std::vector<unsigned char> *out, std::string *err, const std::string &path, void *)
   //                        {
   //                           std::ifstream f(path, std::ios::binary);
   //                           if (!f)
   //                           {
   //                              if (err)
   //                                 *err = "Could not open: " + path;
   //                              return false;
   //                           }
   //                           f.seekg(0, std::ios::end);
   //                           size_t size = f.tellg();
   //                           f.seekg(0);
   //                           out->resize(size);
   //                           f.read(reinterpret_cast<char *>(out->data()), size);
   //                           return true;
   //                        },
   //                        // writeWholeFile (rarely needed)
   //                        nullptr, nullptr});


   void gltf::Model::loadFromFile(std::string filename, ::gpu::context *pgpucontext, VkQueue transferQueue,
                                  uint32_t fileLoadingFlags, float scale)
   {

      tinygltf::Model gltfModel;

      tinygltf::TinyGLTF gltfContext;

      if (fileLoadingFlags & FileLoadingFlags::DontLoadImages)
      {
         gltfContext.SetImageLoader(loadImageDataFuncEmpty, nullptr);
      }
      else
      {
         gltfContext.SetImageLoader(tinygltf_LoadImageData, image());
      }
      //    if (fileLoadingFlags & FileLoadingFlags::UseFsCallbacks)
      //    {

      // Set custom filesystem callbacks
      tinygltf::FsCallbacks fsCallbacks;
      fsCallbacks.FileExists = tinygltf_FileExists;
      fsCallbacks.ReadWholeFile = tinygltf_ReadWholeFile;
      fsCallbacks.WriteWholeFile = tinygltf_WriteWholeFile;
      fsCallbacks.ExpandFilePath = tinygltf_ExpandFilePath; // use default
      fsCallbacks.user_data = file();
      gltfContext.SetFsCallbacks(fsCallbacks);

      //}
#if defined(__ANDROID__)
      // On Android all assets are packed with the apk in a compressed form, so we need to open them using the asset
      // manager We let tinygltf handle this, by passing the asset manager of our app
      tinygltf::asset_manager = androidApp->activity->assetManager;
#endif
      size_t pos = filename.find_last_of('/');
      m_path1 = filename.substr(0, pos);

      m_strDirectory = m_path1.c_str();

      std::string error, warning;

      this->m_pgpucontext = pgpucontext;

#if defined(__ANDROID__)
      // On Android all assets are packed with the apk in a compressed form, so we need to open them using the asset
      // manager We let tinygltf handle this, by passing the asset manager of our app
      tinygltf::asset_manager = androidApp->activity->assetManager;
#endif
      bool fileLoaded = gltfContext.LoadASCIIFromFile(&gltfModel, &error, &warning, filename);

      std::vector<uint32_t> indexBuffer;
      std::vector<::gpu::gltf::vertex> vertexBuffer;

      if (fileLoaded)
      {
         if (!(fileLoadingFlags & FileLoadingFlags::DontLoadImages))
         {
            loadImages(gltfModel, pgpucontext, transferQueue);
         }
         loadMaterials(gltfModel);
         const tinygltf::Scene &scene = gltfModel.scenes[gltfModel.defaultScene > -1 ? gltfModel.defaultScene : 0];
         for (size_t i = 0; i < scene.nodes.size(); i++)
         {
            const tinygltf::Node node = gltfModel.nodes[scene.nodes[i]];
            loadNode(nullptr, node, scene.nodes[i], gltfModel, indexBuffer, vertexBuffer, scale);
         }
         if (gltfModel.animations.size() > 0)
         {
            loadAnimations(gltfModel);
         }
         loadSkins(gltfModel);

         for (auto pnode: m_nodeaLinear)
         {
            // Assign skins
            if (pnode->m_iSkinIndex > -1)
            {
               pnode->m_pskin = m_skina[pnode->m_iSkinIndex];
            }
            // Initial pose
            if (pnode->m_pmesh)
            {
               pnode->update();
            }
         }
      }
      else
      {

         ::string strMessage;

         strMessage << "Could not load gltf file \"" << filename.c_str() << "\": " << error.c_str();

         throw ::exception(error_failed, strMessage);

         return;
      }

      // Pre-Calculations for requested features
      if ((fileLoadingFlags & FileLoadingFlags::PreTransformVertices) ||
          (fileLoadingFlags & FileLoadingFlags::PreMultiplyVertexColors) ||
          (fileLoadingFlags & FileLoadingFlags::FlipY))
      {
         const bool preTransform = fileLoadingFlags & FileLoadingFlags::PreTransformVertices;
         const bool preMultiplyColor = fileLoadingFlags & FileLoadingFlags::PreMultiplyVertexColors;
         const bool flipY = fileLoadingFlags & FileLoadingFlags::FlipY;
         for (Node *pnode: m_nodeaLinear)
         {
            if (pnode->m_pmesh)
            {
               const floating_matrix4 localMatrix = pnode->getMatrix();
               for (Primitive *primitive: pnode->m_pmesh->primitives)
               {
                  for (uint32_t i = 0; i < primitive->vertexCount; i++)
                  {
                     ::gpu::gltf::vertex &vertex = vertexBuffer[primitive->firstVertex + i];
                     // Pre-transform vertex positions by node-hierarchy
                     if (preTransform)
                     {
                        vertex.position = floating_sequence3(localMatrix * floating_sequence4(vertex.position, 1.0f));
                        vertex.normal = (floating_matrix3(localMatrix) * vertex.normal).normalized();
                     }
                     // Flip Y-Axis of vertex positions
                     if (flipY)
                     {
                        vertex.position.y *= -1.0f;
                        vertex.normal.y *= -1.0f;
                     }
                     // Pre-Multiply vertex colors with material base color
                     if (preMultiplyColor)
                     {
                        vertex.color = primitive->m_pmaterial->baseColorFactor * vertex.color;
                     }
                  }
               }
            }
         }
      }

      for (auto extension: gltfModel.extensionsUsed)
      {
         if (extension == "KHR_materials_pbrSpecularGlossiness")
         {
            information() << "Required extension: " << extension.c_str();
            m_bMetallicRoughnessWorkflow = false;
         }
      }

      size_t vertexBufferSize = vertexBuffer.size() * sizeof(::gpu::gltf::vertex);
      size_t indexBufferSize = indexBuffer.size() * sizeof(uint32_t);
      indices.count = static_cast<uint32_t>(indexBuffer.size());
      vertices.count = static_cast<uint32_t>(vertexBuffer.size());

      assert((vertexBufferSize > 0) && (indexBufferSize > 0));

      struct StagingBuffer
      {
         VkBuffer buffer;
         VkDeviceMemory memory;
      } vertexStaging, indexStaging;


      ::cast<::gpu_vulkan::context> pcontext = m_pgpucontext;
      ::cast<::gpu_vulkan::device> pgpudevice = pcontext->m_pgpudevice;
      auto pphysicaldevice = pgpudevice->m_pphysicaldevice;


      // Create staging buffers
      // Vertex data
      VkCheckResult(pgpudevice->createBuffer(
         VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
         vertexBufferSize, &vertexStaging.buffer, &vertexStaging.memory, vertexBuffer.data()));
      // Index data
      VkCheckResult(pgpudevice->createBuffer(
         VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
         indexBufferSize, &indexStaging.buffer, &indexStaging.memory, indexBuffer.data()));
      VkMemoryPropertyFlags memoryPropertyFlags = 0;

      // Create device local buffers
      // Vertex buffer
      VkCheckResult(pgpudevice->createBuffer(
         VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | memoryPropertyFlags,
         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBufferSize, &vertices.buffer, &vertices.memory));
      // Index buffer
      VkCheckResult(pgpudevice->createBuffer(
         VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | memoryPropertyFlags,
         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBufferSize, &indices.buffer, &indices.memory));

      // Copy from staging buffers
      auto pgpucommandbufferCopy = pcontext->beginSingleTimeCommands(pcontext->m_pgpudevice->transfer_queue());

      ::cast<::gpu_vulkan::command_buffer> pcommandbufferCopy = pgpucommandbufferCopy;

      VkBufferCopy copyRegion = {};

      copyRegion.size = vertexBufferSize;
      vkCmdCopyBuffer(pcommandbufferCopy->m_vkcommandbuffer, vertexStaging.buffer, vertices.buffer, 1, &copyRegion);

      copyRegion.size = indexBufferSize;
      vkCmdCopyBuffer(pcommandbufferCopy->m_vkcommandbuffer, indexStaging.buffer, indices.buffer, 1, &copyRegion);

      // pcontext->flushCommandBuffer(pcommandbufferCopy->m_vkcommandbuffer, transferQueue, true);

      pgpucommandbufferCopy.commit();

      vkDestroyBuffer(pcontext->logicalDevice(), vertexStaging.buffer, nullptr);
      vkFreeMemory(pcontext->logicalDevice(), vertexStaging.memory, nullptr);
      vkDestroyBuffer(pcontext->logicalDevice(), indexStaging.buffer, nullptr);
      vkFreeMemory(pcontext->logicalDevice(), indexStaging.memory, nullptr);

      getSceneDimensions();

      // Setup descriptors
      uint32_t uboCount{0};
      uint32_t imageCount{0};
      for (auto pnode: m_nodeaLinear)
      {
         if (pnode->m_pmesh)
         {
            uboCount++;
         }
      }
      for (auto material: m_materiala)
      {
         if (material.baseColorTexture != nullptr)
         {
            imageCount++;
         }
      }
      uint32_t materialCount = 0;
      for (auto &m: m_materiala)
      {
         if (m.baseColorTexture)
            ++materialCount;
      }

      std::vector<VkDescriptorPoolSize> poolSizes;

      if (uboCount > 0)
      {
         poolSizes.push_back({VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, uboCount});
      }

      uint32_t samplerCount =
         materialCount * 5 *
         pgpucontext->m_pgpurenderer->m_pgpurendertarget->get_frame_count(); // baseColor + normal per material
      if (samplerCount > 0)
      {
         poolSizes.push_back({VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, samplerCount});
      }

      VkDescriptorPoolCreateInfo descriptorPoolCI{};
      descriptorPoolCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
      descriptorPoolCI.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
      descriptorPoolCI.pPoolSizes = poolSizes.data();
      descriptorPoolCI.maxSets =
         uboCount + materialCount * pgpucontext->m_pgpurenderer->m_pgpurendertarget->get_frame_count();
      VkCheckResult(vkCreateDescriptorPool(pcontext->logicalDevice(), &descriptorPoolCI, nullptr, &m_descriptorPool));

      // Descriptors for per-node uniform buffers
      {
         {
            // Layout is global, so only create if it hasn't already been created before
            // if (descriptorSetLayoutUbo == VK_NULL_HANDLE) {
            //	std::vector<aaaVkDescriptorSetLayoutBinding> setLayoutBindings = {
            //		vkinit::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0),
            //	};
            //	aaaVkDescriptorSetLayoutCreateInfo descriptorLayoutCI{};
            //	descriptorLayoutCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            //	descriptorLayoutCI.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
            //	descriptorLayoutCI.pBindings = setLayoutBindings.data();
            //	VkCheckResult(vkCreateDescriptorSetLayout(pcontext->logicalDevice(), &descriptorLayoutCI, nullptr,
            //&descriptorSetLayoutUbo));
            //}
            
            // for (auto pnode: m_nodea)
            // {
            //
            //    ::cast < ::gpu_vulkan::binding_set > pbindingset = pcontext->m_pengine->global_ubo1_binding_set();
            //
            //    auto pdescriptorsetlayout = pbindingset->descriptor_set_layout(pgpucommandbufferCopy);
            //
            //    prepareNodeDescriptor(pnode, pdescriptorsetlayout->m_vkdescriptorsetlayout);
            //
            // }

         }

         //// Descriptors for per-material images
         //{
         //	// Layout is global, so only create if it hasn't already been created before
         //         if (fileLoadingFlags & FileLoadingFlags::OnlySceneImages)
         //         {


         //            {
         //               // Layout is global, so only create if it hasn't already been created before
         //               if (!pcontext->m_psetdescriptorlayoutSceneGltfImage2)
         //               {
         //                  pcontext->m_psetdescriptorlayoutSceneGltfImage2 =

         //                     // binding 0 → baseColor (albedo)
         //                     // binding 1 → normal map
         //                     // binding 2 → metallic map
         //                     // binding 3 → roughness map
         //                     // binding 4 → ambient occlusion map
         //                     descriptor_set_layout::Builder(pgpucontext)
         //                        .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
         //                        VK_SHADER_STAGE_FRAGMENT_BIT) .addBinding(1,
         //                        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) .build();

         //                  //// Always push exactly two bindings:
         //                  ////  • binding 0 = base-color sampler
         //                  ////  • binding 1 = normal-map sampler
         //                  // std::array<aaaVkDescriptorSetLayoutBinding, 5> setLayoutBindings = {
         //                  //	// binding 0 → baseColor (albedo)
         //                  //	vkinit::descriptorSetLayoutBinding(
         //                  //		VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
         //                  //		VK_SHADER_STAGE_FRAGMENT_BIT,
         //                  //		/*binding=*/ 0
         //                  //	),
         //                  //		// binding 1 → normal map
         //                  //		vkinit::descriptorSetLayoutBinding(
         //                  //			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
         //                  //			VK_SHADER_STAGE_FRAGMENT_BIT,
         //                  //			/*binding=*/ 1
         //                  //		),
         //                  //		// binding 2 → metallic map
         //                  //		vkinit::descriptorSetLayoutBinding(
         //                  //			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
         //                  //			VK_SHADER_STAGE_FRAGMENT_BIT,
         //                  //			/*binding=*/ 2
         //                  //		),
         //                  //		// binding 3 → roughness map
         //                  //		vkinit::descriptorSetLayoutBinding(
         //                  //			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
         //                  //			VK_SHADER_STAGE_FRAGMENT_BIT,
         //                  //			/*binding=*/ 3
         //                  //		),
         //                  //		// binding 4 → ambient occlusion map
         //                  //		vkinit::descriptorSetLayoutBinding(
         //                  //			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
         //                  //			VK_SHADER_STAGE_FRAGMENT_BIT,
         //                  //			/*binding=*/ 4
         //                  //		)
         //                  // };


         //                  // aaaVkDescriptorSetLayoutCreateInfo descriptorLayoutCI{};
         //                  // descriptorLayoutCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
         //                  // descriptorLayoutCI.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
         //                  // descriptorLayoutCI.pBindings = setLayoutBindings.data();
         //                  // VkCheckResult(vkCreateDescriptorSetLayout(
         //                  //	pcontext->logicalDevice(),
         //                  //	&descriptorLayoutCI,
         //                  //	nullptr,
         //                  //             &pcontext->m_psetdescriptorlayoutGltfImage->m_vkdescriptorsetlayout
         //                  //));
         //               }
         //            }
         //         }
         //                        else
         //            {

         //	}

         // Descriptors for per-material images

         for (auto &material: m_materiala)
         {
            uint32_t descriptorBindingFlags = 0;
            if (material.baseColorTexture != nullptr)
            {
               descriptorBindingFlags |= gltf::DescriptorBindingFlags::ImageBaseColor;
            }
            if (material.normalTexture != nullptr)
            {
               descriptorBindingFlags |= gltf::DescriptorBindingFlags::ImageNormalMap;
            }

            // if (!(fileLoadingFlags & FileLoadingFlags::OnlySceneImages))
            {

               if (material.metallicRoughnessTexture != nullptr)
               {
                  descriptorBindingFlags |= gltf::DescriptorBindingFlags::ImageMetallicMap;
               }


               if (material.emissiveTexture != nullptr)
               {
                  descriptorBindingFlags |= gltf::DescriptorBindingFlags::ImageEmissiveMap;
               }
               if (material.occlusionTexture != nullptr)
               {
                  descriptorBindingFlags |= gltf::DescriptorBindingFlags::ImageAOMap;
               }
            }
            material.m_uDescriptorBindingFlags = descriptorBindingFlags;
         }
      }
   }



   void gltf::Material::update_binding_set(::gpu::binding_slot_set *pgpubindinslotset, uint32_t descriptorBindingFlags,
                          ::gpu_vulkan::texture * ptextureFallback)
   {

      ::pointer_array<::gpu_vulkan::texture> texturea;

      // Albedo
      texturea.add((baseColorTexture && (descriptorBindingFlags & DescriptorBindingFlags::ImageBaseColor))
            ? baseColorTexture.m_p
                      : ptextureFallback);

      // Normal
      texturea.add((normalTexture && (descriptorBindingFlags & DescriptorBindingFlags::ImageNormalMap))
                      ? normalTexture.m_p
                      : ptextureFallback);

      // Metallic/Roughness
      texturea.add((metallicRoughnessTexture && (descriptorBindingFlags & DescriptorBindingFlags::ImageMetallicMap))
                      ? metallicRoughnessTexture.m_p
                      : ptextureFallback);

      // If you have a separate roughness texture, bind it here, otherwise fallback
      // VkDescriptorImageInfo roughnessImageInfo = fallbackTexture->m_descriptor3;
      // Ambient Occlusion
      texturea.add(
         (occlusionTexture && (descriptorBindingFlags & DescriptorBindingFlags::ImageAOMap))
                      ? occlusionTexture.m_p
                      : ptextureFallback);

      // Emissive
      texturea.add(
         (occlusionTexture && (descriptorBindingFlags & DescriptorBindingFlags::ImageEmissiveMap))
                      ? emissiveTexture.m_p
                      : ptextureFallback);

      int iCommonCount = minimum(texturea.count(), pgpubindinslotset->size());

      for (int i = 0; i < iCommonCount; i++)
      {

         pgpubindinslotset->binding_slot(i)->m_ptexture = texturea[i];

      }

   }


   //::array_base<VkDescriptorSet> &gltf::Material::descriptor_set_array(gltf::Model *pmodel,
   //                                                                    ::gpu::binding_set *pgpubindingset)
   //{
   //   //if (m_descriptorsetaGltf4.is_empty() && m_uDescriptorBindingFlags)
   //   //{
   //      ::cast<::gpu_vulkan::context> pcontext = m_pgpucontext;

   //      auto pengine = m_pgpucontext->m_pengine;

   //      ::cast<::gpu_vulkan::binding_set> pbindingset = pgpubindingset;

   //      auto pdescriptorsetlayout = pbindingset->descriptor_set_layout_gltf();

   //      for (int i = 0; i < m_pgpucontext->m_pgpurenderer->m_pgpurendertarget->get_frame_count(); i++)
   //      {
   //         // if ((fileLoadingFlags & FileLoadingFlags::OnlySceneImages))
   //         //{

   //         addDescriptor(5, m_descriptorsetaGltf4, pmodel->m_descriptorPool,
   //                          pdescriptorsetlayout->m_vkdescriptorsetlayout,
   //            
   //                          m_uDescriptorBindingFlags, pmodel->empty_texture());
   //      }
   ////   }

   //  // return m_descriptorsetaGltf4;
   //}


   //::array_base<VkDescriptorSet> &gltf::Material::descriptor_set_array_scene_gltf(gltf::Model *pmodel)
   //{
   //   if (m_descriptorsetaSceneGltf4.is_empty() && m_uDescriptorBindingFlags)
   //   {

   //      ::cast<::gpu_vulkan::context> pcontext = m_pgpucontext;
   //      auto pdescriptorsetlayout = pcontext->descriptor_set_layout_scene_gltf();
   //      for (int i = 0; i < m_pgpucontext->m_pgpurenderer->m_pgpurendertarget->get_frame_count(); i++)
   //      {
   //         // if ((fileLoadingFlags & FileLoadingFlags::OnlySceneImages))
   //         //{

   //         addDescriptor(2, m_descriptorsetaSceneGltf4, pmodel->m_descriptorPool,
   //                          
   //            pdescriptorsetlayout->m_vkdescriptorsetlayout,
   //            
   //                          m_uDescriptorBindingFlags, pmodel->empty_texture());
   //      }
   //   }

   //   return m_descriptorsetaSceneGltf4;
   //}


   
   void gltf::Model::createEmptyTexture(VkQueue transferQueue)
   {

      auto &emptyTexture = this->emptyTexture2;

      construct_newø(emptyTexture);

      // Define the static member here (outside any class/function)

      emptyTexture->m_pgpurenderer = m_pgpucontext->m_pgpurenderer;
      emptyTexture->m_textureattributes.m_rectangleTarget.set_width(1);
      emptyTexture->m_textureattributes.m_rectangleTarget.set_height(1);
      emptyTexture->m_textureattributes.m_iLayerCount = 1;
      emptyTexture->m_textureattributes.m_iMipCount = 1;

      ::cast<::gpu_vulkan::context> pcontext = m_pgpucontext;
      ::cast<::gpu_vulkan::device> pgpudevice = pcontext->m_pgpudevice;
      auto pphysicaldevice = pgpudevice->m_pphysicaldevice;


      size_t bufferSize = emptyTexture->m_textureattributes.m_rectangleTarget.area() * 4;
      unsigned char *buffer = new unsigned char[bufferSize];
      memset(buffer, 0, bufferSize);
      VkBuffer stagingBuffer;
      VkDeviceMemory stagingMemory;
      VkBufferCreateInfo bufferCreateInfo = vkinit::bufferCreateInfo();
      bufferCreateInfo.size = bufferSize;
      // This buffer is used as a transfer source for the buffer copy
      bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
      bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
      VkCheckResult(vkCreateBuffer(pcontext->logicalDevice(), &bufferCreateInfo, nullptr, &stagingBuffer));

      VkMemoryAllocateInfo memAllocInfo = vkinit::memoryAllocateInfo();
      VkMemoryRequirements memReqs;
      vkGetBufferMemoryRequirements(pcontext->logicalDevice(), stagingBuffer, &memReqs);
      memAllocInfo.allocationSize = memReqs.size;
      memAllocInfo.memoryTypeIndex = pphysicaldevice->findMemoryType(
         memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
      VkCheckResult(vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &stagingMemory));
      VkCheckResult(vkBindBufferMemory(pcontext->logicalDevice(), stagingBuffer, stagingMemory, 0));

      // Copy texture data into staging buffer
      uint8_t *data;
      VkCheckResult(vkMapMemory(pcontext->logicalDevice(), stagingMemory, 0, memReqs.size, 0, (void **)&data));
      memcpy(data, buffer, bufferSize);
      vkUnmapMemory(pcontext->logicalDevice(), stagingMemory);

      VkBufferImageCopy bufferCopyRegion = {};
      bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      bufferCopyRegion.imageSubresource.layerCount = 1;
      bufferCopyRegion.imageExtent.width = emptyTexture->width();
      bufferCopyRegion.imageExtent.height = emptyTexture->height();
      bufferCopyRegion.imageExtent.depth = 1;

      // Create optimal tiled target image
      VkImageCreateInfo imageCreateInfo = vkinit::imageCreateInfo();
      imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
      imageCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
      imageCreateInfo.mipLevels = 1;
      imageCreateInfo.arrayLayers = 1;
      imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
      imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
      imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
      imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      imageCreateInfo.extent.width = emptyTexture->width();
      imageCreateInfo.extent.height = emptyTexture->height();
      imageCreateInfo.extent.depth = 1;
      imageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
      VkCheckResult(vkCreateImage(pcontext->logicalDevice(), &imageCreateInfo, nullptr, &emptyTexture->m_vkimage));

      vkGetImageMemoryRequirements(pcontext->logicalDevice(), emptyTexture->m_vkimage, &memReqs);
      memAllocInfo.allocationSize = memReqs.size;
      memAllocInfo.memoryTypeIndex =
         pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
      VkCheckResult(
         vkAllocateMemory(pcontext->logicalDevice(), &memAllocInfo, nullptr, &emptyTexture->m_vkdevicememory));
      VkCheckResult(
         vkBindImageMemory(pcontext->logicalDevice(), emptyTexture->m_vkimage, emptyTexture->m_vkdevicememory, 0));

      VkImageSubresourceRange subresourceRange{};
      subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      subresourceRange.baseMipLevel = 0;
      subresourceRange.levelCount = 1;
      subresourceRange.layerCount = 1;

      // VkCommandBuffer pcommandbufferCopy->m_vkcommandbuffer =
      // pcontext->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

      auto pgpucommandbufferCopy = pcontext->beginSingleTimeCommands(pcontext->m_pgpudevice->transfer_queue());

      ::cast<command_buffer> pcommandbufferCopy = pgpucommandbufferCopy;

      vulkan::setImageLayout(pcommandbufferCopy->m_vkcommandbuffer, emptyTexture->m_vkimage, VK_IMAGE_LAYOUT_UNDEFINED,
                             VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourceRange);
      vkCmdCopyBufferToImage(pcommandbufferCopy->m_vkcommandbuffer, stagingBuffer, emptyTexture->m_vkimage,
                             VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferCopyRegion);
      vulkan::setImageLayout(pcommandbufferCopy->m_vkcommandbuffer, emptyTexture->m_vkimage,
                             VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                             subresourceRange);
      // pcontext->flushCommandBuffer(pcommandbufferCopy->m_vkcommandbuffer, transferQueue);
      pgpucommandbufferCopy.commit();
      emptyTexture->m_state.m_vkimagelayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

      // Clean up staging resources
      vkDestroyBuffer(pcontext->logicalDevice(), stagingBuffer, nullptr);
      vkFreeMemory(pcontext->logicalDevice(), stagingMemory, nullptr);

      VkSamplerCreateInfo samplerCreateInfo = vkinit::samplerCreateInfo();
      samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
      samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
      samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
      samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
      samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
      samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
      samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
      samplerCreateInfo.maxAnisotropy = 1.0f;
      VkCheckResult(
         vkCreateSampler(pcontext->logicalDevice(), &samplerCreateInfo, nullptr, &emptyTexture->m_vksampler3));

      VkImageViewCreateInfo viewCreateInfo = vkinit::imageViewCreateInfo();
      viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      viewCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
      viewCreateInfo.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
      viewCreateInfo.subresourceRange.levelCount = 1;
      viewCreateInfo.image = emptyTexture->m_vkimage;
      VkCheckResult(
         vkCreateImageView(pcontext->logicalDevice(), &viewCreateInfo, nullptr, &emptyTexture->m_vkimageview));

      emptyTexture->m_descriptor3.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      emptyTexture->m_descriptor3.imageView = emptyTexture->m_vkimageview;
      emptyTexture->m_descriptor3.sampler = emptyTexture->m_vksampler3;

      
   }

   ::gpu_vulkan::texture * gltf::Model::empty_texture()
   {

      if (!emptyTexture2)
      {

         throw ::exception(error_wrong_state);

      }

      return emptyTexture2;

   }


   //void gltf::Model::drawNode(void drawNode(::gpu::command_buffer * pgpucommandbuffer, Node* node, uint32_t uFrameIndex, VkCommandBuffer commandBuffer, ::gpu::binding_set_pointer pbindingset, uint32_t renderFlags = 0, VkPipelineLayout pipelineLayout = VK_NULL_HANDLE);Node *pnode, uint32_t uFrameIndex, VkCommandBuffer commandBuffer,
   //                           ::gpu::binding_set_pointer pbindingset, uint32_t renderFlags,
   //                           VkPipelineLayout pipelineLayout) void gltf::Model::
   void gltf::Model::drawNode(::gpu::command_buffer *pgpucommandbuffer, Node *pnode,
                             uint32_t renderFlags)
//               Node * pnode, uint32_t uFrameIndex, VkCommandBuffer commandBuffer,
  //             ::gpu::binding_set_pointer pbindingset, uint32_t renderFlags, VkPipelineLayout pipelineLayout)
   {
      if (pnode->m_pmesh)
      {
         for (Primitive *primitive: pnode->m_pmesh->primitives)
         {
            bool skip = false;
            auto pmaterial = primitive->m_pmaterial;
            if (renderFlags & RenderFlags::RenderOpaqueNodes)
            {
               skip = (pmaterial->alphaMode != Material::ALPHAMODE_OPAQUE);
            }
            if (renderFlags & RenderFlags::RenderAlphaMaskedNodes)
            {
               skip = (pmaterial->alphaMode != Material::ALPHAMODE_MASK);
            }
            if (renderFlags & RenderFlags::RenderAlphaBlendedNodes)
            {
               skip = (pmaterial->alphaMode != Material::ALPHAMODE_BLEND);
            }
            if (!skip)
            {

               //if (::is_set(pbindingset.m_pbindingset))
               //{

               //   auto &descriptorSet = pmaterial->descriptor_set_array(this, pbindingset.m_pbindingset)[uFrameIndex];

               //   vkCmdBindDescriptorSets(
               //      commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout,
               //                           pbindingset.m_iSet,
               //                           1, &descriptorSet, 0, nullptr);
               //}
               //else if (renderFlags & RenderFlags::BindJustSceneImages)
               //{
               //   auto &descriptorSet = pmaterial->descriptor_set_array_scene_gltf(this)[uFrameIndex];
               //   vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, bindImageSet,
               //                           1, &descriptorSet, 0, nullptr);
               //}
               auto pshader = pgpucommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext->m_pshaderBound;
               pshader->binding_slot_set(2, pmaterial->m_pbindingset);
               pshader->on_before_draw(pgpucommandbuffer);
               ::cast<::gpu_vulkan::command_buffer> pcommandbuffer = pgpucommandbuffer;
               vkCmdDrawIndexed(pcommandbuffer->m_vkcommandbuffer,
                  primitive->indexCount, 1, primitive->firstIndex, 0, 0);

            }

         }

      }

      for (auto &pnodeChild: pnode->m_nodeaChildren)
      {

         drawNode(pgpucommandbuffer, pnodeChild, renderFlags);

      }

   }


   void gltf::Model::bind(VkCommandBuffer commandBuffer)
   {
      const VkDeviceSize offsets[1] = {0};
      vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertices.buffer, offsets);
      vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT32);
      m_bBuffersBound = true;
   }
   void gltf::Model::bind(::gpu::command_buffer *pgpucommandbuffer)
   {
      ::cast<::gpu_vulkan::command_buffer> pcommandbuffer = pgpucommandbuffer;
      bind(pcommandbuffer->m_vkcommandbuffer);
   }


   void gltf::Model::draw(::gpu::command_buffer *pgpucommandbuffer)
   {
      
      ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;
      
      //gltfDraw(pcommandbuffer, pgpucommandbuffer->m_iFrameIndex, pgpucommandbuffer->m_pbindingsetCurrent);
      gltfDraw(pcommandbuffer);

   }


   void gltf::Model::gltfDraw(::gpu::command_buffer * pgpucommandbuffer, uint32_t renderFlags)
   {
      ::cast<::gpu_vulkan::command_buffer> pcommandbuffer = pgpucommandbuffer;
      if (!m_bBuffersBound)
      {
         const VkDeviceSize offsets[1] = {0};
         vkCmdBindVertexBuffers(pcommandbuffer->m_vkcommandbuffer, 0, 1, &vertices.buffer, offsets);
         vkCmdBindIndexBuffer(pcommandbuffer->m_vkcommandbuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT32);
      }
      for (auto &pnode: m_nodea)
      {
         drawNode(pcommandbuffer, pnode, renderFlags);
      }
   }


   void gltf::Model::getNodeDimensions(Node *pnode, floating_sequence3 &min, floating_sequence3 &max)
   {
      if (pnode->m_pmesh)
      {
         for (Primitive *primitive: pnode->m_pmesh->primitives)
         {
            floating_sequence4 locMin = floating_sequence4(primitive->dimensions.min, 1.0f) * pnode->getMatrix();
            floating_sequence4 locMax = floating_sequence4(primitive->dimensions.max, 1.0f) * pnode->getMatrix();
            if (locMin.x < min.x)
            {
               min.x = locMin.x;
            }
            if (locMin.y < min.y)
            {
               min.y = locMin.y;
            }
            if (locMin.z < min.z)
            {
               min.z = locMin.z;
            }
            if (locMax.x > max.x)
            {
               max.x = locMax.x;
            }
            if (locMax.y > max.y)
            {
               max.y = locMax.y;
            }
            if (locMax.z > max.z)
            {
               max.z = locMax.z;
            }
         }
      }
      for (auto pnodeChild: pnode->m_nodeaChildren)
      {
         getNodeDimensions(pnodeChild, min, max);
      }
   }

   void gltf::Model::getSceneDimensions()
   {
      dimensions.min = floating_sequence3(FLT_MAX);
      dimensions.max = floating_sequence3(-FLT_MAX);
      for (auto pnode: m_nodea)
      {
         getNodeDimensions(pnode, dimensions.min, dimensions.max);
      }
      dimensions.size = dimensions.max - dimensions.min;
      dimensions.center = (dimensions.min + dimensions.max) / 2.0f;
      dimensions.radius = dimensions.min.distance(dimensions.max) / 2.0f;
   }

   void gltf::Model::loadAnimations(tinygltf::Model &gltfModel)
   {
      for (tinygltf::Animation &anim: gltfModel.animations)
      {
         Animation animation{};
         animation.m_strName = anim.name.c_str();
         if (anim.name.empty())
         {
            animation.m_strName = ::as_string(m_animationa.size());
         }

         // Samplers
         for (auto &samp: anim.samplers)
         {
            AnimationSampler sampler{};

            if (samp.interpolation == "LINEAR")
            {
               sampler.m_einterpolation = AnimationSampler::InterpolationType::LINEAR;
            }
            if (samp.interpolation == "STEP")
            {
               sampler.m_einterpolation = AnimationSampler::InterpolationType::STEP;
            }
            if (samp.interpolation == "CUBICSPLINE")
            {
               sampler.m_einterpolation = AnimationSampler::InterpolationType::CUBICSPLINE;
            }

            // Read sampler input time values
            {
               const tinygltf::Accessor &accessor = gltfModel.accessors[samp.input];
               const tinygltf::BufferView &bufferView = gltfModel.bufferViews[accessor.bufferView];
               const tinygltf::Buffer &buffer = gltfModel.buffers[bufferView.buffer];

               assert(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);

               float *buf = new float[accessor.count];
               memcpy(buf, &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(float));
               for (size_t uIndex = 0; uIndex < accessor.count; uIndex++)
               {
                  sampler.m_faInput.add(buf[uIndex]);
               }
               delete[] buf;
               for (auto fInput: sampler.m_faInput)
               {
                  if (fInput < animation.start)
                  {
                     animation.start = fInput;
                  };
                  if (fInput > animation.end)
                  {
                     animation.end = fInput;
                  }
               }
            }

            // Read sampler output T/R/S values
            {
               const tinygltf::Accessor &accessor = gltfModel.accessors[samp.output];
               const tinygltf::BufferView &bufferView = gltfModel.bufferViews[accessor.bufferView];
               const tinygltf::Buffer &buffer = gltfModel.buffers[bufferView.buffer];

               assert(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);

               switch (accessor.type)
               {
                  case TINYGLTF_TYPE_VEC3:
                  {
                     floating_sequence3 *buf = new floating_sequence3[accessor.count];
                     memcpy(buf, &buffer.data[accessor.byteOffset + bufferView.byteOffset],
                            accessor.count * sizeof(floating_sequence3));
                     for (size_t uIndex = 0; uIndex < accessor.count; uIndex++)
                     {
                        sampler.m_sequence4aOutput.add(floating_sequence4(buf[uIndex], 0.0f));
                     }
                     delete[] buf;
                     break;
                  }
                  case TINYGLTF_TYPE_VEC4:
                  {
                     floating_sequence4 *buf = new floating_sequence4[accessor.count];
                     memcpy(buf, &buffer.data[accessor.byteOffset + bufferView.byteOffset],
                            accessor.count * sizeof(floating_sequence4));
                     for (size_t uIndex = 0; uIndex < accessor.count; uIndex++)
                     {
                        sampler.m_sequence4aOutput.add(buf[uIndex]);
                     }
                     delete[] buf;
                     break;
                  }
                  default:
                  {
                     information() << "unknown type";
                     break;
                  }
               }
            }

            animation.m_samplera.add(sampler);
         }

         // Channels
         for (auto &source: anim.channels)
         {
            AnimationChannel channel{};

            if (source.target_path == "rotation")
            {
               channel.m_epath = AnimationChannel::PathType::ROTATION;
            }
            if (source.target_path == "translation")
            {
               channel.m_epath = AnimationChannel::PathType::TRANSLATION;
            }
            if (source.target_path == "scale")
            {
               channel.m_epath = AnimationChannel::PathType::SCALE;
            }
            if (source.target_path == "weights")
            {
               information() << "weights not yet supported, skipping channel";
               continue;
            }
            channel.m_uSamplerIndex = source.sampler;
            channel.m_pnode = nodeFromIndex(source.target_node);
            if (!channel.m_pnode)
            {
               continue;
            }

            animation.m_channela.add(channel);
         }

         m_animationa.add(animation);
      }
   }

   void gltf::Model::updateAnimation(uint32_t uIndex, float time)
   {
      if (uIndex > static_cast<uint32_t>(m_animationa.size()) - 1)
      {
         information() << "No animation with index " << uIndex;
         return;
      }
      Animation &animation = m_animationa[uIndex];

      bool updated = false;
      for (auto &channel: animation.m_channela)
      {
         AnimationSampler &sampler = animation.m_samplera[channel.m_uSamplerIndex];
         if (sampler.m_faInput.size() > sampler.m_sequence4aOutput.size())
         {
            continue;
         }

         for (auto i = 0; i < sampler.m_faInput.size() - 1; i++)
         {
            if ((time >= sampler.m_faInput[i]) && (time <= sampler.m_faInput[i + 1]))
            {
               float u =
                  std::max(0.0f, time - sampler.m_faInput[i]) / (sampler.m_faInput[i + 1] - sampler.m_faInput[i]);
               if (u <= 1.0f)
               {
                  switch (channel.m_epath)
                  {
                     case AnimationChannel::PathType::TRANSLATION:
                     {
                        floating_sequence4 trans =
                           sampler.m_sequence4aOutput[i].mix(sampler.m_sequence4aOutput[i + 1], u);
                        channel.m_pnode->m_sequence3Translation = floating_sequence3(trans);
                        break;
                     }
                     case AnimationChannel::PathType::SCALE:
                     {
                        floating_sequence4 trans =
                           sampler.m_sequence4aOutput[i].mix(sampler.m_sequence4aOutput[i + 1], u);
                        channel.m_pnode->m_sequence3Scale = floating_sequence3(trans);
                        break;
                     }
                     case AnimationChannel::PathType::ROTATION:
                     {
                        floating_quaternion q1;
                        q1.x = sampler.m_sequence4aOutput[i].x;
                        q1.y = sampler.m_sequence4aOutput[i].y;
                        q1.z = sampler.m_sequence4aOutput[i].z;
                        q1.w = sampler.m_sequence4aOutput[i].w;
                        floating_quaternion q2;
                        q2.x = sampler.m_sequence4aOutput[i + 1].x;
                        q2.y = sampler.m_sequence4aOutput[i + 1].y;
                        q2.z = sampler.m_sequence4aOutput[i + 1].z;
                        q2.w = sampler.m_sequence4aOutput[i + 1].w;
                        channel.m_pnode->m_matrixRotation = q1.slerp(q2, u).normalized();
                        break;
                     }
                  }
                  updated = true;
               }
            }
         }
      }
      if (updated)
      {
         for (auto &pnode: m_nodea)
         {
            pnode->update();
         }
      }
   }

   /*
      Helper functions
   */
   gltf::Node *gltf::Model::findNode(Node *pnodeParent, uint32_t uIndex)
   {
      Node *nodeFound = nullptr;
      if (pnodeParent->m_uIndex == uIndex)
      {
         return pnodeParent;
      }
      for (auto &pnodeChild: pnodeParent->m_nodeaChildren)
      {
         nodeFound = findNode(pnodeChild, uIndex);
         if (nodeFound)
         {
            break;
         }
      }
      return nodeFound;
   }

   gltf::Node *gltf::Model::nodeFromIndex(uint32_t uIndex)
   {
      Node *nodeFound = nullptr;
      for (auto &pnode: m_nodea)
      {
         nodeFound = findNode(pnode, uIndex);
         if (nodeFound)
         {
            break;
         }
      }
      return nodeFound;
   }

   // void gltf::Model::prepareNodeDescriptor(Node *pnode, aaaVkDescriptorSetLayout descriptorSetLayout)
   // {
   //    if (pnode->m_pmesh)
   //    {
   //       VkDescriptorSetAllocateInfo descriptorSetAllocInfo{};
   //       descriptorSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
   //       descriptorSetAllocInfo.descriptorPool = m_descriptorPool;
   //       descriptorSetAllocInfo.pSetLayouts = &descriptorSetLayout;
   //       descriptorSetAllocInfo.descriptorSetCount = 1;
   //       ::cast<::gpu_vulkan::context> pcontext = m_pgpucontext;
   //       ::cast<::gpu_vulkan::device> pgpudevice = pcontext->m_pgpudevice;
   //       auto pphysicaldevice = pgpudevice->m_pphysicaldevice;
   //
   //       VkCheckResult(vkAllocateDescriptorSets(pcontext->logicalDevice(), &descriptorSetAllocInfo,
   //                                                &pnode->m_pmesh->uniformBuffer.descriptorSet));
   //
   //       VkWriteDescriptorSet writeDescriptorSet{};
   //       writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
   //       writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
   //       writeDescriptorSet.descriptorCount = 1;
   //       writeDescriptorSet.dstSet = pnode->m_pmesh->uniformBuffer.descriptorSet;
   //       writeDescriptorSet.dstBinding = 0;
   //       writeDescriptorSet.pBufferInfo = &pnode->m_pmesh->uniformBuffer.descriptor;
   //
   //       vkUpdateDescriptorSets(pcontext->logicalDevice(), 1, &writeDescriptorSet, 0, nullptr);
   //    }
   //    for (auto &pnodeChild: pnode->m_nodeaChildren)
   //    {
   //       prepareNodeDescriptor(pnodeChild, descriptorSetLayout);
   //    }
   // }


} // namespace gpu_vulkan


//BEGIN_GPU_PROPERTIES(::gpu::gltf::vertex)
//GPU_PROPERTY("pos", ::gpu::e_type_seq3)
//GPU_PROPERTY("normal", ::gpu::e_type_seq3)
//GPU_PROPERTY("uv", ::gpu::e_type_seq2)
//GPU_PROPERTY("color", ::gpu::e_type_seq4)
//GPU_PROPERTY("tangent", ::gpu::e_type_seq4)
//END_GPU_PROPERTIES()

//
//		struct Vertex
//{
//   floating_sequence3 pos;
//   floating_sequence3 normal;
//   floating_sequence2 uv;
//   floating_sequence4 color;
//   floating_sequence4 joint0;
//   floating_sequence4 weight0;
//   floating_sequence4 tangent;
//   // static VkVertexInputBindingDescription vertexInputBindingDescription;
//   // static std::vector<VkVertexInputAttributeDescription> vertexInputAttributeDescriptions;
//   // static VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;
//   // static VkVertexInputBindingDescription inputBindingDescription(uint32_t binding);
//   // static VkVertexInputAttributeDescription inputAttributeDescription(uint32_t binding, uint32_t location,
//   // VertexComponent component); static std::vector<VkVertexInputAttributeDescription>
//   // inputAttributeDescriptions(uint32_t binding, const std::vector<VertexComponent> components);
//   ///** @brief Returns the default pipeline vertex input state create info structure for the requested vertex
//   ///components */
//   // static VkPipelineVertexInputStateCreateInfo* getPipelineVertexInputState(const std::vector<VertexComponent>
//   // components);
//};

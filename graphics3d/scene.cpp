// From bred/graphics3d/scene.cpp by camilo on 2025-09-29 03:46 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "scene.h"
#include "acme/filesystem/filesystem/file_context.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/device.h"
#include "bred/gpu/texture.h"
#include "bred/graphics3d/asset_manager.h"
#include "bred/graphics3d/engine.h"
#include "bred/graphics3d/immersion_layer.h"
#include "gpu/ibl/brdf_convolution_framebuffer.h"
#include "gpu/ibl/cubemap_framebuffer.h"
#include "gpu/ibl/diffuse_irradiance_map.h"
#include "gpu/ibl/equirectangular_cubemap.h"
#include "gpu/ibl/mipmap_cubemap_framebuffer.h"
#include "gpu/ibl/specular_map.h"
#include "bred/prodevian/actor.h"
#include "openssl/ct.h"
#include "point_light.h"
#include "skybox.h"

/// #include "SceneFoundry/scene_foundry/player.h"
// #include "SceneFoundry/sandbox_game/game_object.h"

// #include <json.hpp>


// 
// #include <spdlog/spdlog.h>

namespace graphics3d
{


   // using json = nlohmann::json;

   scene::scene()
   {

   }


   scene::~scene()
   {
   
   
   }


   //void scene::generateIbl()
   //{

   //         try
   //   {
   //      // Generate BRDF LUT first (your existing function)
   //      generateIblBRDFlut();
   //   }
   //   catch (...)
   //   {
   //   }

   //   // Now generate irradiance and prefiltered maps using environmentCube (must be valid)
   //   if (!m_pgpucontext->m_pengine->m_pimmersionlayer->m_passetmanager->m_ptextureEnvironmentCube)
   //   {
   //      error("[scene] environmentCube is null - aborting IBL generation to avoid descriptor errors.");
   //   }
   //   else
   //   {
   //      try
   //      {
   //         generateIblIrradianceMap();
   //         generateIblPrefilteredEnvMap();
   //         information("[scene] IBL assets generated successfully.");
   //      }
   //      catch (const ::exception &e)
   //      {
   //         errorf("[scene] IBL generation failed: %s", e.get_message().c_str());
   //      }
   //   }



   //}


   void scene::generateIblBRDFlut()
   {

      m_ptextureLuBrdf = generate_ibl_brdf_lut();

   }


   void scene::generateIblIrradianceMap()
   {

      m_ptextureIrradianceCube = generate_ibl_irradiance_map(m_prenderableSkyboxModel);

   }


   void scene::generateIblPrefilteredEnvMap()
   {

      m_ptexturePrefilteredCube = generate_ibl_prefiltered_env_map(m_prenderableSkyboxModel);

   }


   //::pointer<::graphics3d::renderable> scene::get_skybox_cube_model()
   //{
   //     return ::graphics3d::scene_base::get_skybox_cube_model();
   //   //auto pmodelbuffer = øcreate_new<::gpu::model_buffer>();
   //   //::array_base<floating_sequence3> vertexa = {// positions
   //   //                                   {-1.0f, 1.0f, -1.0f},  {-1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f},
   //   //                                   {1.0f, -1.0f, -1.0f},  {1.0f, 1.0f, -1.0f},   {-1.0f, 1.0f, -1.0f},

   //   //                                   {-1.0f, -1.0f, 1.0f},  {-1.0f, -1.0f, -1.0f}, {-1.0f, 1.0f, -1.0f},
   //   //                                   {-1.0f, 1.0f, -1.0f},  {-1.0f, 1.0f, 1.0f},   {-1.0f, -1.0f, 1.0f},

   //   //                                   {1.0f, -1.0f, -1.0f},  {1.0f, -1.0f, 1.0f},   {1.0f, 1.0f, 1.0f},
   //   //                                   {1.0f, 1.0f, 1.0f},    {1.0f, 1.0f, -1.0f},   {1.0f, -1.0f, -1.0f},

   //   //                                   {-1.0f, -1.0f, 1.0f},  {-1.0f, 1.0f, 1.0f},   {1.0f, 1.0f, 1.0f},
   //   //                                   {1.0f, 1.0f, 1.0f},    {1.0f, -1.0f, 1.0f},   {-1.0f, -1.0f, 1.0f},

   //   //                                   {-1.0f, 1.0f, -1.0f},  {1.0f, 1.0f, -1.0f},   {1.0f, 1.0f, 1.0f},
   //   //                                   {1.0f, 1.0f, 1.0f},    {-1.0f, 1.0f, 1.0f},   {-1.0f, 1.0f, -1.0f},

   //   //                                   {-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, 1.0f},  {1.0f, -1.0f, -1.0f},
   //   //                                   {1.0f, -1.0f, -1.0f},  {-1.0f, -1.0f, 1.0f},  {1.0f, -1.0f, 1.0f}};

   //   //pmodelbuffer->static_initialize_vertexes<::gpu::position3>(vertexa);

   //   //return pmodelbuffer;
   //}

   //::gpu::ibl::equirectangular_cubemap *scene::equirectangular_cubemap()
   //{
   //   if (!m_piblequirectangularcubemap)
   //   {

   //      øconstruct(m_piblequirectangularcubemap);

   //      //;
   //      //;
   //      //m_piblequirectangularcubemap->initialize_equirectangular_cubemap(
   //        // m_pgpucontext, current_sky_box());

   //      m_piblequirectangularcubemap->initialize_equirectangular_cubemap(
   //       m_);
   //   }

   //   return m_piblequirectangularcubemap;
   //}

   
   gpu::ibl::specular_map *scene::ibl_specular_map()
   {
      
      if (!m_piblspecularmap)
      {

         øconstruct(m_piblspecularmap);

         //m_piblspecularmap->initialize_specular_map(this, m_prenderableSkybox);
         m_piblspecularmap->initialize_specular_map(this);

      }

      return m_piblspecularmap;

   }


   /// @brief generatePrefilteredEnvMap
   /// @param environmentCubeExisting
   /// @param prenderableSkybox
   /// @return
   //::pointer<::gpu::texture> scene::generateIblPrefilteredEnvMap(::gpu::texture *environmentCubeExisting,
   ::pointer<::gpu::texture> scene::generate_ibl_prefiltered_env_map(
                                                                ::graphics3d::renderable *prenderableSkybox)
   {

      auto piblspecularmap = ibl_specular_map();

      //if (!piblspecularmap->m_pframebufferPrefilteredEnvMap)
      {
         auto pcommandbuffer = m_pgpucontext->beginSingleTimeCommands(m_pgpucontext->m_pgpudevice->graphics_queue());
         // this->flushCommandBuffer(layoutCmd, m_vkqueueTransfer3, true);

         m_pgpucontext->start_debug_happening(pcommandbuffer, "compute ibl_prefiltered_env_map");
         piblspecularmap->computePrefilteredEnvMap(pcommandbuffer);
         m_pgpucontext->end_debug_happening(pcommandbuffer);
         m_pgpucontext->endSingleTimeCommands(pcommandbuffer);
      }

      return piblspecularmap->m_pframebufferPrefilteredEnvMap->m_ptexture;
   }

   /// generate irradianceCube
   /// @return irradianceCube
   ::pointer<::gpu::texture> scene::generate_ibl_irradiance_map(
      //         ::gpu::texture * irradianceCube,
      //::gpu::texture *environmentCube,
      ::graphics3d::renderable *prenderableSkybox)
   {

      if (!m_pibldiffuseirradiancemap)
      {

         øconstruct(m_pibldiffuseirradiancemap);
         //m_pibldiffuseirradiancemap->initialize_diffuse_irradiance_map(
           // this, m_prenderableSkybox);
         m_pibldiffuseirradiancemap->initialize_diffuse_irradiance_map(
          this);
      }

      //if (!m_pibldiffuseirradiancemap->m_pdiffuseIrradianceFramebuffer)
      {
         auto pcommandbuffer =
            m_pgpucontext->beginSingleTimeCommands(m_pgpucontext->m_pgpudevice->graphics_queue());
         // this->flushCommandBuffer(layoutCmd, m_vkqueueTransfer3, true);

         m_pgpucontext->start_debug_happening(pcommandbuffer, "compute irradianceMap");
         m_pibldiffuseirradiancemap->computeIrradianceMap(pcommandbuffer);
         m_pgpucontext->end_debug_happening(pcommandbuffer);
         m_pgpucontext->endSingleTimeCommands(pcommandbuffer);

      }

      return m_pibldiffuseirradiancemap->m_pdiffuseIrradianceFramebuffer->m_ptexture;
   }
   // ::pointer<::gpu::texture> loadCubemap(
   //    const ::scoped_string& name,
   //    const ::scoped_string& ktxFilename,
   //    VkFormat format,
   //    VkImageUsageFlags usageFlags,
   //    VkImageLayout initialLayout);
   //    virtual void generateBRDFlut(
   //     ::gpu::texture * lutBrdf);
   /// generate lutBrdf
   /// @return lutBrdf
   ::pointer<::gpu::texture> scene::generate_ibl_brdf_lut()
   {
      auto piblspecularmap = ibl_specular_map();
      //if (!piblspecularmap->m_pbrdfconvolutionframebuffer)
      {

         piblspecularmap->computeBrdfConvolutionMap();
      }

      return piblspecularmap->m_pbrdfconvolutionframebuffer->m_ptexture;
   }


} // namespace graphics3d

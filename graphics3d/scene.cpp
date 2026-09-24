// From bred/graphics3d/scene.cpp by camilo on 2025-09-29 03:46 <3ThomasBorregaardSorensen!!
#include "platform.h"
#include "scene.h"
#include "acme/filesystem/filesystem/file_context.h"
#include "bred/gpu/binding.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/device.h"
#include "bred/gpu/texture.h"
#include "bred/gpu/texture_site.h"
#include "bred/graphics3d/asset_manager.h"
#include "bred/graphics3d/engine_instance.h"
#include "bred/graphics3d/immersion_layer.h"
//#include "gpu/ibl/brdf_convolution_framebuffer.h"
//#include "gpu/ibl/cubemap_framebuffer.h"
#include "gpu/ibl/diffuse_irradiance_map.h"
#include "gpu/ibl/equirectangular_cubemap.h"
//#include "gpu/ibl/mipmap_cubemap_framebuffer.h"
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
   //   if (!m_pgpucontext->m_pgraphics3dengineinstance->m_pimmersionlayer->m_passetmanager->m_ptextureEnvironmentCube)
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


   void scene::generateIblBRDFlut(::gpu::context * pgpucontext)
   {

      defer_construct_newø(m_ptexturesiteLuBrdf);
      m_ptexturesiteLuBrdf->m_pgputextureSite = generate_ibl_brdf_lut(pgpucontext);

   }


   void scene::generateIblIrradianceMap(::gpu::context * pgpucontext)
   {

      defer_construct_newø(m_ptexturesiteIrradianceCube);
      m_ptexturesiteIrradianceCube->m_pgputextureSite = generate_ibl_irradiance_map(m_prenderableSkyboxModel, pgpucontext);

   }


   void scene::generateIblPrefilteredEnvMap(::gpu::context * pgpucontext)
   {

      defer_construct_newø(m_ptexturesitePrefilteredCube);
      m_ptexturesitePrefilteredCube->m_pgputextureSite = generate_ibl_prefiltered_env_map(m_prenderableSkyboxModel, pgpucontext);

   }


   //::pointer<::graphics3d::renderable> scene::get_skybox_cube_model()
   //{
   //     return ::graphics3d::scene_base::get_skybox_cube_model();
   //   //auto pmodelbuffer = create_newø<::gpu::model_buffer>();
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

   //      constructø(m_piblequirectangularcubemap);

   //      //;
   //      //;
   //      //m_piblequirectangularcubemap->initialize_equirectangular_cubemap(
   //        // m_pgpucontext, current_sky_box());

   //      m_piblequirectangularcubemap->initialize_equirectangular_cubemap(
   //       m_);
   //   }

   //   return m_piblequirectangularcubemap;
   //}

   
   gpu::ibl::specular_map *scene::ibl_specular_map(::gpu::context * pgpucontext)
   {
      
      if (!m_piblspecularmap)
      {

         constructø(m_piblspecularmap);

         //m_piblspecularmap->initialize_specular_map(this, m_prenderableSkybox);
         m_piblspecularmap->initialize_specular_map(this, pgpucontext);

      }

      return m_piblspecularmap;

   }


   ::pointer<::gpu::texture_site> scene::generate_ibl_prefiltered_env_map(
                                                                ::graphics3d::renderable *prenderableSkybox, ::gpu::context * pgpucontext)
   {

      auto piblspecularmap = ibl_specular_map(pgpucontext);

      //if (!piblspecularmap->m_pframebufferPrefilteredEnvMap)
      {
         auto pcommandbuffer = m_pgpucontext->m_pcommandbufferMain;
         m_pgpucontext->start_debug_happening(pcommandbuffer, "compute ibl_prefiltered_env_map");
         piblspecularmap->computePrefilteredEnvMap(pcommandbuffer, this);
         m_pgpucontext->end_debug_happening(pcommandbuffer);
      }

      return piblspecularmap->m_ptexturesitePrefilteredEnvMapCubemap;
   }

   /// generate irradianceCube
   /// @return irradianceCube
   ::pointer<::gpu::texture_site> scene::generate_ibl_irradiance_map(
      //         ::gpu::texture * irradianceCube,
      //::gpu::texture *environmentCube,
      ::graphics3d::renderable *prenderableSkybox, ::gpu::context * pgpucontext)
   {

      if (!m_pibldiffuseirradiancemap)
      {

         constructø(m_pibldiffuseirradiancemap);
         //m_pibldiffuseirradiancemap->initialize_diffuse_irradiance_map(
           // this, m_prenderableSkybox);
         m_pibldiffuseirradiancemap->initialize_diffuse_irradiance_map(
          this, pgpucontext);
      }

      //if (!m_pibldiffuseirradiancemap->m_pdiffuseIrradianceFramebuffer)
      {
         //auto pcommandbuffer =
           // m_pgpucontext->beginSingleTimeCommands(m_pgpucontext->m_pgpudevice->graphics_queue());
         // this->flushCommandBuffer(layoutCmd, m_vkqueueTransfer3, true);
         auto pcommandbuffer = m_pgpucontext->m_pcommandbufferMain;
         m_pgpucontext->start_debug_happening(pcommandbuffer, "compute irradianceMap");
         m_pibldiffuseirradiancemap->computeIrradianceMap(pcommandbuffer, this);
         m_pgpucontext->end_debug_happening(pcommandbuffer);
         //m_pgpucontext->endSingleTimeCommands(pcommandbuffer);

      }

      return m_pibldiffuseirradiancemap->m_ptexturesiteDiffuseIrradianceCubemap;
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
   ::pointer<::gpu::texture> scene::generate_ibl_brdf_lut(::gpu::context * pgpucontext)
   {
      auto piblspecularmap = ibl_specular_map(pgpucontext);
      //if (!piblspecularmap->m_pbrdfconvolutionframebuffer)
      {

         //piblspecularmap->computeBrdfConvolutionMap();

         //auto pcommandbuffer = m_pgpucontext->beginSingleTimeCommands(m_pgpucontext->m_pgpudevice->graphics_queue());
         auto pcommandbuffer = m_pgpucontext->m_pcommandbufferMain;
         // this->flushCommandBuffer(layoutCmd, m_vkqueueTransfer3, true);

         m_pgpucontext->start_debug_happening(pcommandbuffer, "compute Brdf Convolution Map");
         piblspecularmap->computeBrdfConvolutionMap(pcommandbuffer);
         m_pgpucontext->end_debug_happening(pcommandbuffer);
         //m_pgpucontext->endSingleTimeCommands(pcommandbuffer);

      }

      return piblspecularmap->m_ptexturesiteBrdfConvolutionMap;

   }


   ::gpu::binding_slot_set *scene::ibl_binding_slot_set()
   {

      if (!m_pbindingslotsetIbl1)
      {

         ASSERT(::is_ok(m_ptexturesiteIrradianceCube));
         ASSERT(::is_ok(m_ptexturesitePrefilteredCube));
         ASSERT(::is_ok(m_ptexturesiteLuBrdf));

         constructø(m_pbindingslotsetIbl1);

         auto pbindingset = m_pgpucontext->ibl1_binding_set();

         m_pbindingslotsetIbl1->initialize_binding_slot_set(pbindingset);

         auto pbindingslot0 = m_pbindingslotsetIbl1->binding_slot(0);
         pbindingslot0->m_ptexturesite = m_ptexturesiteIrradianceCube;

         auto pbindingslot1 = m_pbindingslotsetIbl1->binding_slot(1);
         pbindingslot1->m_ptexturesite = m_ptexturesitePrefilteredCube;

         auto pbindingslot2 = m_pbindingslotsetIbl1->binding_slot(2);
         pbindingslot2->m_ptexturesite = m_ptexturesiteLuBrdf;

      }

      return m_pbindingslotsetIbl1;

   }


} // namespace graphics3d

// From bred/graphics3d/scene.h by camilo on 2025-09-29 03:46 <3ThomasBorregaardSorensen!!
#pragma once


#include "bred/graphics3d/scene_base.h"


namespace graphics3d
{


   class CLASS_DECL_GRAPHICS3D scene :
      virtual public ::graphics3d::scene_base
   {
   public:


      //pointer<::gpu::ibl::equirectangular_cubemap> m_piblequirectangularcubemap;
      pointer<::gpu::ibl::diffuse_irradiance_map> m_pibldiffuseirradiancemap;
      pointer<::gpu::ibl::specular_map> m_piblspecularmap;

            // caches
      ::pointer<::gpu::texture_site> m_ptexturesiteLuBrdf;
      ::pointer<::gpu::texture_site> m_ptexturesiteIrradianceCube;
      ::pointer<::gpu::texture_site> m_ptexturesitePrefilteredCube;
      ::pointer<::gpu::binding_slot_set> m_pbindingslotsetIbl1;


      scene();
      ~scene() override;

      //virtual void generateIbl() override;

      void generateIblBRDFlut() override;
      void generateIblIrradianceMap() override;
      void generateIblPrefilteredEnvMap() override;


      //::pointer<::graphics3d::renderable> get_skybox_cube_model();

      //virtual ::gpu::ibl::equirectangular_cubemap *equirectangular_cubemap();
      virtual ::gpu::ibl::specular_map *ibl_specular_map();
      virtual ::pointer<::gpu::texture_site> generate_ibl_prefiltered_env_map(
         ::graphics3d::renderable *prenderableSkybox);

      /// generate irradianceCube
      /// @return irradianceCube
      virtual ::pointer<::gpu::texture_site> generate_ibl_irradiance_map(
         //         ::gpu::texture * irradianceCube,
         //::gpu::texture *environmentCube, ::graphics3d::renderable *prenderableSkybox);
          ::graphics3d::renderable *prenderableSkybox);
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
      virtual ::pointer<::gpu::texture> generate_ibl_brdf_lut();

      virtual ::gpu::binding_slot_set *ibl_binding_slot_set();

   };


} // namespace graphics3d




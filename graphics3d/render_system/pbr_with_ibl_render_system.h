// Changed by camilo on 2025-12-06 21:36 <3ThomasBorregaardSørensen!!
#pragma once


//#include "SceneFoundry/graphics3d/render_system.h"
#include "bred/graphics3d/render_system/pbr_with_ibl_render_system_base.h"


//#include "SceneFoundry/graphics3d/asset_provider_i.h"
//#include "SceneFoundry/graphics3d/game_object_i.h"

//#include "device.h"
//#include "pipeline.h"
//#include "SceneFoundry/graphics3d/descriptor.h"

//#include "gltf.h"
//#include "SceneFoundry/graphics3d/renderer.h"

// STD
//#include <memory>
//#include <vector>

//#include <vulkan/vulkan.h>


namespace graphics3d
{


	class CLASS_DECL_GRAPHICS3D pbr_with_ibl_render_system : virtual public ::graphics3d::pbr_with_ibl_render_system_base
	{
	public:

  //    ::graphics3d::scene_renderable *m_pscenerenderableCurrent;
  //    bool m_bDisableAlbedo = false;
  //    bool m_bDisableMetallicRoughness = false;
  //    bool m_bDisableNormal = false;
  //    bool m_bDisableAmbientOcclusion = false;
  //    bool m_bDisableEmissive = false;
  //    bool m_bForceDefaultMetallicFactor = false;
  //    bool m_bForceDefaultRoughnessFactor = false;
  //    bool m_bForceDefaultAmbientOcclusionFactor = false;
  //    bool m_bForceDefaultAlbedo = false;
  //    bool m_bForceDefaultEmission = false;
  //    float m_fDefaultMetallicFactor = 0.0f;
  //    float m_fDefaultRoughnessFactor = 0.0f;
  //    float m_fDefaultAmbientOcclusionFactor = 0.0f;
  //    floating_sequence3 m_seq3DefaultAlbedo={};
  //    floating_sequence3 m_seq3DefaultEmission = {};
  //    ::pointer<::gpu::shader> m_pshader;
		//::pointer < ::gpu::shader > m_pshaderOpaque;
		//::pointer < ::gpu::shader > m_pshaderMask;
		//::pointer < ::gpu::shader > m_pshaderBlend;

		//::pointer <::graphics3d::asset_manager > m_passetmanager;



		pbr_with_ibl_render_system();
		~pbr_with_ibl_render_system();


      ::graphics3d::scene_renderable *current_scene_renderable() override;

      void on_prepare(::gpu::context *pgpucontext) override;

		void on_render(::gpu::context* pgpucontext, ::graphics3d::scene_base* pscene) override;

      virtual ::memory pbr_with_ibl_vert_memory();
      virtual ::memory pbr_with_ibl_frag_memory();


      virtual ::gpu::binding_set *pbr_binding_set();


      virtual ::gpu::property *push_constants_properties();


	};


} // namespace graphics3d



#pragma once


#include "bred/graphics3d/scene_object.h"


//
//#include "model.h"
//
//#include "acme/prototype/collection/map.h"
//// libs
//
//
//// std
//#include <memory>
//#include <unordered_map>
//
//
//namespace graphics3d_opengl
//{
//
//
//	struct ::graphics3d::transform {
//		floating_sequence3 translation{};
//		floating_sequence3 scale{ 1.f, 1.f, 1.f };
//		floating_sequence3 rotation{};
//
//		// Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
//		// Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
//
//		floating_matrix4 mat4();
//		floating_matrix3 normalMatrix();
//
//	};
//
//	struct PointLightComponent :
//	virtual public ::particle{
//		float lightIntensity = 1.0f;
//	};
//
//
//	class application_object :
//		virtual public ::quantum
//	{
//	public:
//		
//		::collection::index m_iId;
//
//		::graphics3d::transform m_transform;
//
//		floating_sequence3 m_color;
//
//		::pointer<model> m_pmodel;
//
//		using map = index_map < ::pointer < application_object > >;
//
//		static interlocked_long_long s_interlockedll;
//		application_object() : 
//			m_iId{ s_interlockedll++ }
//		{
//		
//		}
//
//	};
//
//
//	class point_light :
//		virtual public application_object
//	{
//	public:
//
//		
//		PointLightComponent	m_pointlightcomponent;
//
//
//		point_light(float intensity = 10.f, float radius = 0.1f, floating_sequence3 color = floating_sequence3(1.f));
//		~point_light() override;
//
//	};
//
//
//} // graphics3d_opengl
//
//
//

// Created by camilo on 2025-05-17 04:14 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "application.h"
#include "engine.h"
#include "scene.h"
#include "acme/exception/interface_only.h"


namespace graphics3d
{


	engine::engine()
	{


	} 

	engine::~engine()
	{


	}


	void engine::run()
	{


	}


	void engine::on_layout(int cx, int cy)
	{



	}


	void engine::on_mouse_move(int x, int y)
	{


	}


	::pointer<model> engine::create_model_from_file(const ::file::path& path)
	{

		model::Builder builder{};

		builder.loadModel(m_pcontext, path);

		auto pmodel = __øcreate < model>();

		pmodel->initialize_model(m_pcontext, builder);

		return pmodel;

	}




	void engine::add_scene(::graphics3d::scene* pscene)
	{

		m_mapScene[pscene->m_strName] = pscene;

	}


	void engine::set_current_scene(::graphics3d::scene* pscene)
	{

		m_pscene = pscene;

	}



} // namespace graphics3d



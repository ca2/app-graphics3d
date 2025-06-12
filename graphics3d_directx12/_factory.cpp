// Created by camilo on 2025-05-17 02:54 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "camera.h"
#include "engine.h"
#include "frame.h"
#include "model.h"
#include "aura/graphics/graphics3d/scene_object.h"
#include "aura/platform/application.h"


IMPLEMENT_FACTORY(graphics3d_directx12)
{

   pfactory->add_factory_item < ::graphics3d_directx12::camera, ::graphics3d::camera >();

   pfactory->add_factory_item < ::graphics3d_directx12::model, ::graphics3d::model >();

   pfactory->add_factory_item < ::graphics3d_directx12::engine, ::graphics3d::engine >();

   pfactory->add_factory_item < ::graphics3d::scene_object >();

}




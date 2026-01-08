// Created by camilo on 2025-05-17 02:54 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "camera.h"
#include "engine.h"
#include "frame.h"
//#include "model.h"
#include "bred/graphics3d/scene_object.h"
#include "aura/platform/application.h"
#include "render_system/gltf_render_system.h"
#include "render_system/scene_render_system.h"
#include "render_system/wavefront_obj_render_system.h"
#include "render_system/skybox_render_system.h"



IMPLEMENT_FACTORY(graphics3d_directx12)
{

   //pfactory->add_factory_item < ::graphics3d_directx12::camera, ::graphics3d::camera >();

   //pfactory->add_factory_item < ::graphics3d_directx12::model, ::graphics3d::model >();

   pfactory->add_factory_item < ::graphics3d_directx12::engine, ::graphics3d::engine >();

   pfactory->add_factory_item < ::graphics3d::scene_object >();


   pfactory->add_factory_item<::graphics3d_directx12::gltf_render_system, ::graphics3d::gltf_render_system>();
   pfactory->add_factory_item<::graphics3d_directx12::scene_render_system, ::graphics3d::scene_render_system>();
   pfactory->add_factory_item<::graphics3d_directx12::wavefront_obj_render_system,
                              ::graphics3d::wavefront_obj_render_system>();
   pfactory->add_factory_item<::graphics3d_directx12::skybox_render_system, ::graphics3d::skybox_render_system>();



}




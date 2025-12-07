// Created by camilo on 2025-05-17 02:54 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "camera.h"
//#include "context.h"
#include "engine.h"
//#include "frame.h"
//#include "model.h"
//#include "shader.h"
#include "render_systems/gltf_render_system.h"
#include "render_systems/scene_render_system.h"
#include "render_systems/wavefront_obj_render_system.h"
#include "render_systems/skybox_render_system.h"
#include "aura/platform/application.h"
#include "bred/graphics3d/scene_object.h"
//#include "networking.h"
//#include "apex/networking/http/message.h"
//#include "sockets/basic/socket_handler.h"
//#include "sockets/basic/tcp_socket.h"
//#include "sockets/basic/listen_socket_impl.h"


IMPLEMENT_FACTORY(graphics3d_opengl)
{

   //pfactory->add_factory_item < ::graphics3d_opengl::frame, ::graphics3d::frame >();

   //pfactory->add_factory_item < ::graphics3d_opengl::camera, ::graphics3d::camera >();

   // pfactory->add_factory_item < ::graphics3d_opengl::model, ::graphics3d::model >();

   //pfactory->add_factory_item < ::graphics3d_opengl::shader, ::graphics3d::shader >();

   pfactory->add_factory_item < ::graphics3d_opengl::engine, ::graphics3d::engine >();

   //pfactory->add_factory_item < ::graphics3d_opengl::context, ::gpu::context >();

   pfactory->add_factory_item < ::graphics3d::scene_object >();

   pfactory->add_factory_item< ::graphics3d_opengl::gltf_render_system, ::graphics3d::gltf_render_system>();
   pfactory->add_factory_item<::graphics3d_opengl::scene_render_system, ::graphics3d::scene_render_system>();
   pfactory->add_factory_item<::graphics3d_opengl::wavefront_obj_render_system, ::graphics3d::wavefront_obj_render_system>();
   pfactory->add_factory_item<::graphics3d_opengl::skybox_render_system, ::graphics3d::skybox_render_system>();


}




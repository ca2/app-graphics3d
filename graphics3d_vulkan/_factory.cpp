// Created by camilo on 2025-05-17 02:54 <3ThomasBorregaardSorensen!!
#include "platform.h"
//#include "camera.h"
#include "engine.h"
//#include "frame.h"
//#include "model.h"
//#include "shader.h"
#include "aura/platform/application.h"
#include "bred/graphics3d/scene_object.h"
//#include "networking.h"
//#include "apex/networking/http/message.h"
//#include "sockets/basic/socket_handler.h"
//#include "sockets/basic/tcp_socket.h"
//#include "sockets/basic/listen_socket_impl.h"
#include "render_system/gltf_render_system.h"
#include "render_system/scene_render_system.h"
#include "render_system/wavefront_obj_render_system.h"
#include "render_system/skybox_render_system.h"


IMPLEMENT_FACTORY(graphics3d_vulkan)
{

   //pfactory->add_factory_item < ::graphics3d_vulkan::frame, ::graphics3d::frame >();

   //pfactory->add_factory_item < ::graphics3d_vulkan::camera, ::graphics3d::camera >();

   //pfactory->add_factory_item<::graphics3d::camera>();

   //pfactory->add_factory_item < ::graphics3d_vulkan::model_buffer, ::graphics3d::model >();

   //pfactory->add_factory_item < ::graphics3d_vulkan::shader, ::graphics3d::shader >();

   pfactory->add_factory_item < ::graphics3d_vulkan::engine, ::graphics3d::engine >();

   //pfactory->add_factory_item < ::graphics3d_vulkan::context, ::gpu::context >();

   pfactory->add_factory_item < ::graphics3d::scene_object >();

   pfactory->add_factory_item< ::graphics3d_vulkan::gltf_render_system, ::graphics3d::gltf_render_system>();
   pfactory->add_factory_item<::graphics3d_vulkan::scene_render_system, ::graphics3d::scene_render_system>();
   pfactory->add_factory_item<::graphics3d_vulkan::wavefront_obj_render_system, ::graphics3d::wavefront_obj_render_system>();
   pfactory->add_factory_item<::graphics3d_vulkan::skybox_render_system, ::graphics3d::skybox_render_system>();

}




cmake_minimum_required(VERSION 3.16)
project(graphics3d_opengl CXX)


list(APPEND library_source
   render_systems/gltf_render_system.cpp
   render_systems/gltf_render_system.h
   render_systems/skybox_render_system.cpp
   render_systems/skybox_render_system.h
   render_systems/scene_render_system.cpp
   render_systems/scene_render_system.h
   render_systems/wavefront_obj_render_system.cpp
   render_systems/wavefront_obj_render_system.h
        _.cpp
        _.h
        _factory.cpp
        camera.cpp
        camera.h
        engine.cpp
        engine.h
        framework.cpp
        framework.h
        #model.cpp
        #model.h
   render_system.cpp
   render_system.h
)


list(APPEND library_references
        acme
        apex
        aqua
        aura
        axis
        berg
        bred
        imgui
        gpu_opengl
        graphics3d_khronos
)


add_library(${PROJECT_NAME} SHARED ${library_source})
add_library(static_${PROJECT_NAME} STATIC ${library_source})


if (NOT ${NO_PRECOMPILED_HEADER})

   target_precompile_headers(${PROJECT_NAME} PRIVATE framework.h)
   target_precompile_headers(static_${PROJECT_NAME} PRIVATE framework.h)

endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if (MSVC)
   target_compile_options(${PROJECT_NAME} PRIVATE "/MD$<$<CONFIG:Debug>:d>")
   target_link_options(${PROJECT_NAME} PRIVATE "/DEFAULTLIB:msvcrt$<$<CONFIG:Debug>:d>")
   target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:WINDOWS")
   target_link_libraries(${PROJECT_NAME} PRIVATE ${library_references} glad)
elseif (${LINUX})
   #if(USE_PKGCONFIG)
   pkg_check_modules(PKGCONFIG opengl)
   #endif()


   target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fpermissive  ${EXTRA_CXX_TARGET_COMPILER_OPTIONS}>)


   target_compile_options(${PROJECT_NAME} PRIVATE ${PKGCONFIG_CFLAGS})
   target_compile_options(${PROJECT_NAME} PRIVATE ${PKGCONFIG_CFLAGS})
   target_link_options(${PROJECT_NAME} PRIVATE ${PKGCONFIG_LDFLAGS})

   target_link_libraries(${PROJECT_NAME} PRIVATE ${library_references} ${global_library_references} ${PKGCONFIG_LIBRARIES})
   target_link_options(${PROJECT_NAME} PRIVATE -Wl,-z,defs)
   # target_link_options(static_${PROJECT_NAME} PRIVATE --whole-archive)


   message(STATUS "(C) PKGCONFIG_LIBRARIES is " ${PKGCONFIG_LIBRARIES})

   target_compile_definitions(${PROJECT_NAME} PRIVATE VK_USE_PLATFORM_WAYLAND_KHR VK_USE_PLATFORM_XCB_KHR VK_USE_PLATFORM_WAYLAND_OR_XCB_KHR)
endif ()


list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/..)
list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/../..)

target_include_directories(${PROJECT_NAME} PRIVATE ${library_include_directories})
target_include_directories(static_${PROJECT_NAME} PRIVATE ${library_include_directories})


target_compile_definitions(${PROJECT_NAME} PRIVATE _${PROJECT_NAME}_project)



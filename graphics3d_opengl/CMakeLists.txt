cmake_minimum_required(VERSION 3.16)
project(opengl_land_landen CXX)


list(APPEND library_source
        opengl/application.cpp
        opengl/application.h
        opengl/benchmark.hpp
        opengl/camera.hpp
        opengl/CMakeLists.txt
        opengl/CommandLineParser.hpp
        opengl/Entrypoints.h
        opengl/frustum.hpp
        opengl/keycodes.h
        opengl/keycodes.hpp
        opengl/offscreen.cpp
        opengl/offscreen.h
        #opengl/render_headless.cpp
        #opengl/render_headless.h
        opengl/shared.h
        #opengl/swap_chain.cpp
        #opengl/swap_chain.h
        #opengl/swap_chain2.cpp
        #opengl/swap_chain2.h
        opengl/threadpool.hpp
        opengl/vk_container.cpp
        opengl/vk_container.h
        #opengl/opengl_example_base_with_swap_chain.cpp
        #opengl/opengl_example_base_with_swap_chain.h
        opengl/opengl_example_base_no_swap_chain.cpp
        opengl/opengl_example_base_no_swap_chain.h
        opengl/OpenGLAndroid.cpp
        opengl/OpenGLAndroid.h
        opengl/OpenGLBuffer.cpp
        opengl/OpenGLBuffer.h
        opengl/OpenGLDebug.cpp
        opengl/OpenGLDebug.h
        opengl/OpenGLDevice.cpp
        opengl/OpenGLDevice.h
        opengl/OpenGLFrameBuffer.hpp
        opengl/OpenGLglTFModel.cpp
        opengl/OpenGLglTFModel.h
        opengl/OpenGLInitializers.h
        opengl/OpenGLInitializers.hpp
        #opengl/OpenGLRaytracingSample.cpp
        #opengl/OpenGLRaytracingSample.h
        #opengl/OpenGLSwapChain.cpp
        #opengl/OpenGLSwapChain.h
        #opengl/OpenGLTexture.cpp
        #opengl/OpenGLTexture.h
        opengl/OpenGLTools.cpp
        opengl/OpenGLTools.h
        opengl/OpenGLUIOverlay.cpp
        opengl/OpenGLUIOverlay.h
   _.cpp
   _.h
   application.cpp
   application.h
#   child_frame.cpp
#   child_frame.h
   document.cpp
   document.h
   #edit_impact.cpp
   #edit_impact.h
   frame.cpp
   frame.h
   framework.cpp
   framework.h
   impact.cpp
   impact.h
   #main_impact.cpp
   #main_impact.h
   #pane_impact.cpp
   #pane_impact.h
   #toggle_impact.cpp
   #toggle_impact.h
   #top_impact.cpp
   #top_impact.h
   #window.cpp
   #window.h
)


list(APPEND library_references
        acme
        apex
        aqua
        aura
        axis
        base
        bred
        core
        imgui
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
   target_link_libraries(${PROJECT_NAME} PRIVATE acme apex aqua aura axis base)
   target_compile_definitions(${PROJECT_NAME} PRIVATE VK_USE_PLATFORM_WIN32_KHR)
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


add_subdirectory(__implement)




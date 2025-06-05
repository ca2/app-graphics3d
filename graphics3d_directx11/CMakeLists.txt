cmake_minimum_required(VERSION 3.16)
project(directx11_land_landen CXX)


list(APPEND library_source
        directx11/application.cpp
        directx11/application.h
        directx11/benchmark.hpp
        directx11/camera.hpp
        directx11/CMakeLists.txt
        directx11/CommandLineParser.hpp
        directx11/Entrypoints.h
        directx11/frustum.hpp
        directx11/keycodes.h
        directx11/keycodes.hpp
        directx11/offscreen.cpp
        directx11/offscreen.h
        #directx11/render_headless.cpp
        #directx11/render_headless.h
        directx11/shared.h
        #directx11/swap_chain.cpp
        #directx11/swap_chain.h
        #directx11/swap_chain2.cpp
        #directx11/swap_chain2.h
        directx11/threadpool.hpp
        directx11/vk_container.cpp
        directx11/vk_container.h
        #directx11/directx11_example_base_with_swap_chain.cpp
        #directx11/directx11_example_base_with_swap_chain.h
        directx11/directx11_example_base_no_swap_chain.cpp
        directx11/directx11_example_base_no_swap_chain.h
        directx11/DirectX11Android.cpp
        directx11/DirectX11Android.h
        directx11/DirectX11Buffer.cpp
        directx11/DirectX11Buffer.h
        directx11/DirectX11Debug.cpp
        directx11/DirectX11Debug.h
        directx11/DirectX11Device.cpp
        directx11/DirectX11Device.h
        directx11/DirectX11FrameBuffer.hpp
        directx11/DirectX11glTFModel.cpp
        directx11/DirectX11glTFModel.h
        directx11/DirectX11Initializers.h
        directx11/DirectX11Initializers.hpp
        #directx11/DirectX11RaytracingSample.cpp
        #directx11/DirectX11RaytracingSample.h
        #directx11/DirectX11SwapChain.cpp
        #directx11/DirectX11SwapChain.h
        #directx11/DirectX11Texture.cpp
        #directx11/DirectX11Texture.h
        directx11/DirectX11Tools.cpp
        directx11/DirectX11Tools.h
        directx11/DirectX11UIOverlay.cpp
        directx11/DirectX11UIOverlay.h
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
   pkg_check_modules(PKGCONFIG directx11)
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




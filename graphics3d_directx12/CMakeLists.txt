cmake_minimum_required(VERSION 3.16)
project(directx12_land_landen CXX)


list(APPEND library_source
        directx12/application.cpp
        directx12/application.h
        directx12/benchmark.hpp
        directx12/camera.hpp
        directx12/CMakeLists.txt
        directx12/CommandLineParser.hpp
        directx12/Entrypoints.h
        directx12/frustum.hpp
        directx12/keycodes.h
        directx12/keycodes.hpp
        directx12/offscreen.cpp
        directx12/offscreen.h
        #directx12/render_headless.cpp
        #directx12/render_headless.h
        directx12/shared.h
        #directx12/swap_chain.cpp
        #directx12/swap_chain.h
        #directx12/swap_chain2.cpp
        #directx12/swap_chain2.h
        directx12/threadpool.hpp
        directx12/vk_container.cpp
        directx12/vk_container.h
        #directx12/directx12_example_base_with_swap_chain.cpp
        #directx12/directx12_example_base_with_swap_chain.h
        directx12/directx12_example_base_no_swap_chain.cpp
        directx12/directx12_example_base_no_swap_chain.h
        directx12/DirectX12Android.cpp
        directx12/DirectX12Android.h
        directx12/DirectX12Buffer.cpp
        directx12/DirectX12Buffer.h
        directx12/DirectX12Debug.cpp
        directx12/DirectX12Debug.h
        directx12/DirectX12Device.cpp
        directx12/DirectX12Device.h
        directx12/DirectX12FrameBuffer.hpp
        directx12/DirectX12glTFModel.cpp
        directx12/DirectX12glTFModel.h
        directx12/DirectX12Initializers.h
        directx12/DirectX12Initializers.hpp
        #directx12/DirectX12RaytracingSample.cpp
        #directx12/DirectX12RaytracingSample.h
        #directx12/DirectX12SwapChain.cpp
        #directx12/DirectX12SwapChain.h
        #directx12/DirectX12Texture.cpp
        #directx12/DirectX12Texture.h
        directx12/DirectX12Tools.cpp
        directx12/DirectX12Tools.h
        directx12/DirectX12UIOverlay.cpp
        directx12/DirectX12UIOverlay.h
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
   pkg_check_modules(PKGCONFIG directx12)
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




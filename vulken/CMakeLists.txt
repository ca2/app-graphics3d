cmake_minimum_required(VERSION 3.16)
project(app_core_vulken CXX)


list(APPEND library_source
   _.cpp
   _.h
   application.cpp
   application.h
   child_frame.cpp
   child_frame.h
   document.cpp
   document.h
   #edit_impact.cpp
   #edit_impact.h
   frame.cpp
   frame.h
   platform.cpp
   platform.h
   impact.cpp
   impact.h
   #main_impact.cpp
   #main_impact.h
   pane_impact.cpp
   pane_impact.h
   #toggle_impact.cpp
   #toggle_impact.h
   #top_impact.cpp
   #top_impact.h
   #window.cpp
   #window.h
)


add_library(${PROJECT_NAME} SHARED ${library_source})
add_library(static_${PROJECT_NAME} STATIC ${library_source})




#if(${LINUX} OR ${__BSD__})


   #add_custom_command(
      #OUTPUT ${PROJECT_SOURCE_DIR}/.link_object/_matter.zip.o
      #COMMAND ${APPLICATION_BUILD_HELPER_BINARY} --prepare-application ${PROJECT_SOURCE_DIR}
      #COMMENT "Generating ${PROJECT_SOURCE_DIR}/.link_object/_matter.zip.o"
   #)

   #target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/.link_object/_matter.zip.o)

#endif()






if (NOT ${NO_PRECOMPILED_HEADER})

   target_precompile_headers(${PROJECT_NAME} PRIVATE platform.h)
   target_precompile_headers(static_${PROJECT_NAME} PRIVATE platform.h)

endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if(${WIN32})


   if(MSVC)


      target_compile_options(${PROJECT_NAME} PRIVATE    "/MD$<$<CONFIG:Debug>:d>")
      target_link_options(${PROJECT_NAME} PRIVATE    "/DEFAULTLIB:msvcrt$<$<CONFIG:Debug>:d>")
      target_link_options(${PROJECT_NAME} PRIVATE    "/SUBSYSTEM:WINDOWS")
      target_link_options(${PROJECT_NAME} PRIVATE    "/bigobj")
      target_link_options(${PROJECT_NAME} PRIVATE    "/IMPLIB:${LIBRARY_OUTPUT_PATH}/${PROJECT_NAME}.lib")
      target_link_options(${PROJECT_NAME} PRIVATE    "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.def")


   else()


      target_link_options(${PROJECT_NAME} PRIVATE "-mwindows")
      target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.def")


   endif()


   target_link_libraries(${PROJECT_NAME} PRIVATE acme apex aqua aura axis berg)
elseif (${LINUX})
   target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fpermissive ${EXTRA_CXX_TARGET_COMPILER_OPTIONS}>)
   #target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-rpath=\$ORIGIN")
   target_link_libraries(${PROJECT_NAME} PRIVATE acme apex aqua aura axis berg bred core)
   target_link_options(${PROJECT_NAME} PRIVATE -Wl,-z,defs)
endif ()


list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/..)
list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/../..)

target_include_directories(${PROJECT_NAME} PRIVATE ${library_include_directories})
target_include_directories(static_${PROJECT_NAME} PRIVATE ${library_include_directories})


target_compile_definitions(${PROJECT_NAME} PRIVATE _${PROJECT_NAME}_project)


add_subdirectory(__implement)




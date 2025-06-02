cmake_minimum_required(VERSION "3.13")

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug$<$<CONFIG:Debug>:Debug>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

add_subdirectory("vendor/glfw" "vendor/glad")


project(MyPlace2024)
set(CMAKE_CXX_STANDARD 17)

# Add GLAD as a library
add_library(glad STATIC vendor/glad/src/glad.c)

# Include directories for GLAD
target_include_directories(glad PUBLIC vendor/glad/include)


add_executable(MyPlace2024 
	src/main.cpp
	src/Engine.cpp
	src/Core/Window.cpp 
	src/Core/Inputt.cpp
	src/Renderer/Renderer.cpp 
	src/Shader/Shader.cpp
	src/Renderer/Types/Mesh.cpp
	src/Core/Camera.cpp
	src/Game/Game.cpp 
	src/Renderer/RenderData.cpp
	src/Renderer/Types/Texture.cpp
	src/Renderer/Types/CubeMap.cpp
	vendor/glad/src/glad.c
)

target_include_directories(MyPlace2024 PUBLIC
"${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm"
"${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb"
"${CMAKE_CURRENT_SOURCE_DIR}/src"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Core"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Types"
)


target_link_libraries(MyPlace2024 glfw glad ${CMAKE_DL_LIBS} opengl32)
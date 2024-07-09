cmake_minimum_required(VERSION 3.5)
project(Callie VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all source and header files
# file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS *.h)
# file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS *.cpp)

add_executable(${PROJECT_NAME}
    src/Core/EntryPoint.cpp
    src/Core/Application.cpp
    src/Core/SceneView.cpp
    src/Core/PropertyPanel.cpp
    src/Core/Object.cpp
    src/Core/Log.cpp

    src/Window/Window.cpp

    src/Graphics/Renderer.cpp
    src/Graphics/UIRenderer.cpp
    src/Graphics/FrameBuffer.cpp
    src/Graphics/Camera.cpp
    src/Graphics/Shader.cpp
    src/Graphics/VertexArray.cpp
    src/Graphics/IndexBuffer.cpp
    src/Graphics/VertexBuffer.cpp
)

configure_file(
		${CMAKE_CURRENT_SOURCE_DIR}/src/Core/config.h.in
    config.h 
)

if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

target_precompile_headers(${PROJECT_NAME} PRIVATE src/clpch.h)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR} include src)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui glfw OpenGL::GL GLEW::GLEW glm::glm)


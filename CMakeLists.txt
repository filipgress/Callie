cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Callie
    VERSION 1.0.0
    LANGUAGES CXX
)

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

target_include_directories(${PROJECT_NAME} 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGui
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glew/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glm
)

add_subdirectory(src/ImGui)
add_subdirectory(glfw)
add_subdirectory(glew)
add_subdirectory(glm)


configure_file(
    src/Core/configure_file.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Config.h 
)

#set(OpenGL_GL_PREFERENCE GLVND)
#find_package(OpenGL REQUIRED)
#find_package(GLEW   REQUIRED)
#find_package(glfw3  REQUIRED)
#find_package(glm    REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE libglew_static glfw imgui ${CMAKE_DL_LIBS})
target_precompile_headers(${PROJECT_NAME} PRIVATE src/clpch.h)

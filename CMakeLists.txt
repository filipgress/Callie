cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Callie
    VERSION 1.0.1
    LANGUAGES CXX
)

add_executable(${PROJECT_NAME}
    src/app.cpp
)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(GLEW   REQUIRED)
find_package(glfw3  REQUIRED)

configure_file(
    src/Core/configure_file.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Config.h 
)

add_subdirectory(src/Log)
target_link_libraries(${PROJECT_NAME} PRIVATE GL GLEW glfw logger)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
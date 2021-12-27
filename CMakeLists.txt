cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Callie
    VERSION 1.0.0
    LANGUAGES CXX
)

add_executable(${PROJECT_NAME}
    src/app.cpp
)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(GLEW   REQUIRED)
find_package(glfw3  REQUIRED)

add_subdirectory(src/Logger)
target_link_libraries(${PROJECT_NAME} PRIVATE GL GLEW glfw logger)
# Callie 3D Viewer

## Overview
Callie is a simple 3D viewer application developed using OpenGL. This project is my final year project for secondary school, aimed at providing a basic understanding of 3D graphics rendering. The application allows users to load and visualize 3D models within a windowed interface.

## Requirements
- CMake 3.5 or higher
- C++17 compatible compiler
- vcpkg for dependency management

## Dependencies
The project depends on the following libraries, managed through `vcpkg`:

- [GLEW](https://github.com/nigels-com/glew): The OpenGL Extension Wrangler Library
- [GLFW](https://github.com/glfw/glfw): A library for creating windows with OpenGL contexts and managing input
- [GLM](https://github.com/g-truc/glm): OpenGL Mathematics
- [ImGui](https://github.com/ocornut/imgui): A bloat-free graphical user interface library for C++

## Installation

### Step 1: Clone the repository

```sh
git clone https://github.com/yourusername/callie.git
cd callie
```

### Step 2: Configure and build the project using CMakePresets
Make sure that the environment variable `VCPKG_ROOT` points to your `vcpkg` installation directory.
Then, use CMake to configure and build the project:

```sh
cmake --preset default
cmake --build out
```

### Step 3: Run
```sh
cd out
./Callie
```


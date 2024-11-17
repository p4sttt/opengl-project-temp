if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG") 
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG") 

cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)

include_directories(vendor include)

add_executable(OpenGLProject 
    src/main.cpp
    src/App.cpp
    src/ProjectLoader.cpp
    src/Math.cpp
    src/Graphics.cpp
    vendor/glad.c
)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(OpenGLProject PRIVATE glfw OpenGL::GL)

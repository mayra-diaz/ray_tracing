cmake_minimum_required(VERSION 3.26)
project(ray-tracing)

set(CMAKE_CXX_STANDARD 20)

add_executable(ray-tracing main.cpp
        Camera.cpp
        vector3.cpp
        Ray.cpp
        Object.cpp
        Light.cpp)

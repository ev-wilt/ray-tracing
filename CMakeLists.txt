cmake_minimum_required(VERSION 3.14)
project(RayTracing)

set(CMAKE_CXX_STANDARD 14)

add_executable(RayTracing src/main.cpp src/Vector3.h src/Ray.h src/Hitable.h src/Sphere.h src/HitableList.h src/Camera.h)
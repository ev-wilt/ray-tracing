cmake_minimum_required(VERSION 3.14)
project(RayTracing)

set(CMAKE_CXX_STANDARD 14)

add_executable(RayTracing main.cpp Vector3.h Ray.h Hitable.h Sphere.h HitableList.h)
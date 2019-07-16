cmake_minimum_required(VERSION 3.14)
project(RayTracing)

set(CMAKE_CXX_STANDARD 14)

add_executable(RayTracing src/main.cpp src/Vector3.h src/Ray.h src/hitables/Hitable.h src/hitables/Sphere.h src/hitables/HitableList.h src/Camera.h src/materials/Material.h src/materials/Lambertian.h src/materials/Metal.h src/Random.h src/materials/Dielectric.h src/hitables/MovingSphere.h src/AxisAlignedBoundingBox.h src/hitables/BvhNode.h src/textures/Texture.h src/textures/ConstantTexture.h src/textures/CheckerTexture.h)
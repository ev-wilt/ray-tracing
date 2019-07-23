cmake_minimum_required(VERSION 3.14)
project(RayTracing)

set(CMAKE_CXX_STANDARD 14)

add_executable(RayTracing src/main.cpp src/Vector3.h src/Ray.h src/hitables/Hitable.h src/hitables/spheres/Sphere.h src/hitables/HitableList.h src/Camera.h src/materials/Material.h src/materials/Lambertian.h src/materials/Metal.h src/Random.h src/materials/Dielectric.h src/hitables/spheres/MovingSphere.h src/AxisAlignedBoundingBox.h src/hitables/BvhNode.h src/textures/Texture.h src/textures/ConstantTexture.h src/textures/CheckerTexture.h src/Perlin.h src/textures/NoiseTexture.h src/textures/ImageTexture.h src/Scenes.h src/materials/DiffuseLight.h src/hitables/2Drectangles/XYRectangle.h src/hitables/2Drectangles/XZRectangle.h src/hitables/2Drectangles/YZRectangle.h src/hitables/FlippedNormals.h src/hitables/Box.h)
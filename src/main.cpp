#include <iostream>
#include <vector>
#include <limits>
#include <future>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../lib/stb_image_write.h"
#include "hitables/HitableList.h"
#include "hitables/Sphere.h"
#include "Camera.h"
#include "Random.h"
#include "materials/Lambertian.h"
#include "materials/Metal.h"
#include "materials/Dielectric.h"
#include "hitables/MovingSphere.h"
#include "hitables/BvhNode.h"
#include "textures/ConstantTexture.h"
#include "textures/CheckerTexture.h"

// Image size
const int WIDTH = 1280;
const int HEIGHT = 720;

// Returns the color that the given ray points to within the world
Vector3 color(const Ray& ray, Hitable *world, int depth) {
    HitRecord record;
    if (world->hit(ray, 0.001, std::numeric_limits<float>::max(), record)) {
        Ray scattered;
        Vector3 attenuation;
        if (depth < 50 && record.material->scatter(ray, record, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1);
        }
        else {
            return Vector3(0.0, 0.0, 0.0);
        }
    }
    else {
        Vector3 unitDirection = unitVector(ray.getDirection());
        float t = 0.5 * (unitDirection.y() + 1.0);
        return (1.0 - t) * Vector3(1.0, 1.0, 1.0) + t * Vector3(0.5, 0.7, 1.0);
    }
}

// Generates a random scene with 1 giant sphere, 3 large spheres, and several small spheres
std::unique_ptr<Hitable> randomScene() {
    int size = 50000;
    int index = 0;
    std::vector<std::unique_ptr<Hitable>> list(size + 1);
    auto firstTex = std::make_unique<ConstantTexture>(Vector3(0.2, 0.3, 0.1));
    auto secondTex = std::make_unique<ConstantTexture>(Vector3 (0.9, 0.9, 0.9));
    auto checkerTex = std::make_unique<CheckerTexture>(std::move(firstTex), std::move(secondTex));
    auto checkerMat = std::make_unique<Lambertian>(std::move(checkerTex));

    list[index++] = std::make_unique<Sphere>(Vector3(0, -1000, 0), 1000, std::move(checkerMat));
    for (int i = -10; i < 10; ++i) {
        for (int j = -10; j < 10; ++j) {
            float materialProb = DIST(GEN);
            Vector3 center(i + 0.9 * DIST(GEN), 0.2, j + 0.9 * DIST(GEN));
            if ((center - Vector3(4, 0.2, 0)).length() > 0.9) {
                if (materialProb < 0.8) {
                    Vector3 randomColor = Vector3(DIST(GEN) * DIST(GEN),DIST(GEN) * DIST(GEN), DIST(GEN) * DIST(GEN));
                    Vector3 centerEnd = center + Vector3(0, 0.5 * DIST(GEN), 0);
                    auto randomTex = std::make_unique<ConstantTexture>(randomColor);
                    auto lambertian = std::make_unique<Lambertian>(std::move(randomTex));
                    list[index++] = std::make_unique<MovingSphere>(center, centerEnd, 0.0, 1.0, 0.2, std::move(lambertian));
                }
                else if (materialProb < 0.95) {
                    Vector3 randomColor = Vector3(0.5 * (1 + DIST(GEN)), 0.5 * (1 + DIST(GEN)), 0.5 * (1 + DIST(GEN)));
                    auto metal = std::make_unique<Metal>(randomColor, 0.0);
                    list[index++] = std::make_unique<Sphere>(center, 0.2, std::move(metal));
                }
                else {
                    auto dielectric = std::make_unique<Dielectric>(1.5);
                    list[index++] = std::make_unique<Sphere>(center, 0.2, std::move(dielectric));
                }
            }
        }
    }

    auto dielectric = std::make_unique<Dielectric>(1.5);
    auto lambTex = std::make_unique<ConstantTexture>(Vector3(0.4, 0.2, 0.1));
    auto lambertian = std::make_unique<Lambertian>(std::move(lambTex));
    auto metal = std::make_unique<Metal>(Vector3(0.7, 0.6, 0.5), 0.0);

    list[index++] = std::make_unique<Sphere>(Vector3(0, 1, 0), 1.0, std::move(dielectric));
    list[index++] = std::make_unique<Sphere>(Vector3(-4, 1, 0), 1.0, std::move(lambertian));
    list[index++] = std::make_unique<Sphere>(Vector3(4, 1, 0), 1.0, std::move(metal));
    return std::make_unique<HitableList>(HitableList(std::move(list), index));
}

std::unique_ptr<Hitable> twoSpheres() {
    auto firstTex = std::make_unique<ConstantTexture>(Vector3(0.2, 0.3, 0.1));
    auto secondTex = std::make_unique<ConstantTexture>(Vector3 (0.9, 0.9, 0.9));
    auto checkerTex = std::make_unique<CheckerTexture>(std::move(firstTex), std::move(secondTex));
    auto checkerMat = std::make_shared<Lambertian>(std::move(checkerTex));

    int size = 50;
    std::vector<std::unique_ptr<Hitable>> list(size + 1);
    list[0] = std::make_unique<Sphere>(Vector3(0, -10, 0), 10, checkerMat);
    list[1] = std::make_unique<Sphere>(Vector3(0, 10, 0), 10, checkerMat);
    return std::make_unique<HitableList>(HitableList(std::move(list), 2));
}

int main() {
    int raysPerPixel = 100;
    std::unique_ptr<Hitable> world = twoSpheres();
    Vector3 camPos = Vector3(13,2,3);
    Vector3 camDir = Vector3(0,0,0);
    float focusDist = 10;
    Camera cam(camPos, camDir, Vector3(0,1,0), 20, float(WIDTH) / float(HEIGHT), 0.1, focusDist, 0.0, 1.0);

    const std::size_t max = WIDTH * HEIGHT * 3;
    auto buffer = std::make_unique<unsigned char[]>(WIDTH * HEIGHT * 3);
    std::size_t cores = std::thread::hardware_concurrency();
    volatile std::atomic<std::size_t> count(0);
    std::vector<std::future<void>> futures;
    while (--cores) {
        futures.emplace_back(
                std::async([=, &count, &world, &buffer]() {
                    while (true) {
                        std::size_t index = count++;
                        if (index * 3 + 2 > max) break;
                        std::size_t x = index % WIDTH;
                        std::size_t y = HEIGHT - (index / WIDTH);

                        // Send a given number of rays per pixel
                        // to find the avg color
                        Vector3 col(0.0, 0.0, 0.0);
                        for (int s = 0; s < raysPerPixel; ++s) {
                            float u = float(x + DIST(GEN)) / float(WIDTH);
                            float v = float(y + DIST(GEN)) / float(HEIGHT);
                            Ray ray = cam.getRay(u, v);
                            col += color(ray, world.get(), 0);
                        }

                        col /= float(raysPerPixel);
                        col = Vector3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

                        buffer[index * 3] = (unsigned char)(255.99 * col[0]);
                        buffer[index * 3 + 1] = (unsigned char)(255.99 * col[1]);
                        buffer[index * 3 + 2] = (unsigned char)(255.99 * col[2]);
                    }
                })
        );
    }

    // Wait for each future
    for (std::future<void> &fut: futures) {
        fut.get();
    }

    // Write buffer to a .png
    stbi_write_png("output.png", WIDTH, HEIGHT, 3, buffer.get(), 0);
    return 0;
}
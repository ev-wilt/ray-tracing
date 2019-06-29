#include <iostream>
#include <vector>
#include <limits>
#include <future>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../lib/stb_image_write.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "Random.h"
#include "materials/Lambertian.h"
#include "materials/Metal.h"
#include "materials/Dielectric.h"

// Image size
const int WIDTH = 1920;
const int HEIGHT = 1080;

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

Hitable *randomScene() {
    int size = 500;
    int index = 1;
    Hitable **list = new Hitable*[size + 1];
    list[0] = new Sphere(Vector3(0, -1000, 0), 1000, new Lambertian(Vector3(0.5, 0.5, 0.5)));
    for (int i = -11; i < 11; ++i) {
        for (int j = -11; j < 11; ++j) {
            float materialProb = DIST(GEN);
            Vector3 center(i + 0.9 * DIST(GEN), 0.2, j + 0.9 * DIST(GEN));
            if (materialProb < 0.8) {
                list[index++] = new Sphere(center, 0.2, new Lambertian(Vector3(DIST(GEN) * DIST(GEN), DIST(GEN) * DIST(GEN), DIST(GEN) * DIST(GEN))));
            }
            else if (materialProb < 0.95) {
                list[index++] = new Sphere(center, 0.2, new Metal(Vector3(0.5 * (1 + DIST(GEN)), 0.5 * (1 + DIST(GEN)), 0.5 * (1 + DIST(GEN))), 0.0));
            }
            else {
                list[index++] = new Sphere(center, 0.2, new Dielectric(1.5));
            }
        }
    }

    list[index++] = new Sphere(Vector3(0, 1, 0), 1.0, new Dielectric(1.5));
    list[index++] = new Sphere(Vector3(-4, 1, 0), 1.0, new Lambertian(Vector3(0.4, 0.2, 0.1)));
    list[index++] = new Sphere(Vector3(4, 1, 0), 1.0, new Metal(Vector3(0.7, 0.6, 0.5), 0.0));
    return new HitableList(list, index);
}

int main() {
    int raysPerPixel = 100;
    Hitable *world = randomScene();
    Vector3 camPos = Vector3(13,2,3);
    Vector3 camDir = Vector3(0,0,0);
    float focusDist = 10;
    Camera cam(camPos, camDir, Vector3(0,1,0), 20, float(WIDTH) / float(HEIGHT), 0.1, focusDist);

    const std::size_t max = WIDTH * HEIGHT * 3;
    unsigned char *buffer = new unsigned char[WIDTH * HEIGHT * 3];
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
                            col += color(ray, world, 0);
                        }

                        col /= float(raysPerPixel);
                        col = Vector3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

                        buffer[index * 3] = 255.99 * col[0];
                        buffer[index * 3 + 1] = 255.99 * col[1];
                        buffer[index * 3 + 2] = 255.99 * col[2];
                    }
                })
        );
    }

    // Wait for each future
    for (std::future<void> &fut: futures) {
        fut.get();
    }

    // Write buffer to a .png
    stbi_write_png("output.png", WIDTH, HEIGHT, 3, buffer, 0);
    delete[](buffer);
    return 0;
}
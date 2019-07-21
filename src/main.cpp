#include <iostream>
#include <vector>
#include <limits>
#include <future>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../lib/stb_image_write.h"

#include "Camera.h"
#include "materials/Material.h"
#include "Scenes.h"

// Image size
const int WIDTH = 1280;
const int HEIGHT = 720;

// Returns the color that the given ray points to within the world
Vector3 color(const Ray &ray, Hitable *world, int depth) {
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

int main() {
    int raysPerPixel = 100;
    std::unique_ptr<Hitable> world = texturedSphere();
    Vector3 camPos = Vector3(13, 2, 3);
    Vector3 camDir = Vector3(0, 0, 0);
    float focusDist = 10;
    Camera cam(camPos, camDir, Vector3(0, 1, 0), 20, float(WIDTH) / float(HEIGHT), 0.1, focusDist, 0.0, 1.0);

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
                            float u = float(x + randomReal()) / float(WIDTH);
                            float v = float(y + randomReal()) / float(HEIGHT);
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
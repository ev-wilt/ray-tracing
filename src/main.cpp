#include <iostream>
#include <vector>
#include <limits>
#include <random>
#include <future>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../lib/stb_image_write.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Camera.h"

// Initializing random variables
std::random_device randomDevice;
const std::mt19937 GEN(randomDevice());
const std::uniform_real_distribution<> DIST(0.0, 1.0);

// Image size
const int WIDTH = 600;
const int HEIGHT = 300;

// Returns a randomized point within the unit sphere
Vector3 randomPointInUnitSphere() {
    Vector3 point;
    while (point.squaredLength() >= 1.0) {
        point = 2.0 * Vector3(DIST(GEN), DIST(GEN), DIST(GEN)) - Vector3(1.0, 1.0, 1.0);
    }
    return point;
}

// Returns the color that the given ray points to within the world
Vector3 color(const Ray& ray, Hitable *world) {
    HitRecord record;
    if (world->hit(ray, 0.001, std::numeric_limits<float>::max(), record)) {
        Vector3 target = record.p + record.normal + randomPointInUnitSphere();
        return 0.5 * color(Ray(record.p, target - record.p), world);
    }
    else {
        Vector3 unitDirection = unitVector(ray.getDirection());
        float t = 0.5 * (unitDirection.y() + 1.0);
        return (1.0 - t) * Vector3(1.0, 1.0, 1.0) + t * Vector3(0.5, 0.7, 1.0);
    }
}

int main() {
    int raysPerPixel = 300;
    Hitable *list[2];
    list[0] = new Sphere(Vector3(0.0, 0.0, -1.0), 0.5);
    list[1] = new Sphere(Vector3(0.0, -100.5, -1.0), 100);
    Hitable *world = new HitableList(list, 2);
    Camera cam;
    std::vector<unsigned char> rgb;

    // Iterate through each pixel
    for (int y = HEIGHT - 1; y >= 0; --y) {
        for (int x = 0; x < WIDTH; ++x) {

            // Send a given number of rays per pixel
            // to find the avg color
            Vector3 col(0.0, 0.0, 0.0);
            for (int s = 0; s < raysPerPixel; ++s) {
                float u = float(x + DIST(GEN)) / float(WIDTH);
                float v = float(y + DIST(GEN)) / float(HEIGHT);
                Ray ray = cam.getRay(u, v);
                Vector3 point = ray.pointAtParameter(2.0);
                col += color(ray, world);
            }

            col /= float(raysPerPixel);
            col = Vector3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

            // Push RGB values into buffer
            rgb.push_back(255.99 * col[0]);
            rgb.push_back(255.99 * col[1]);
            rgb.push_back(255.99 * col[2]);
        }
    }

    unsigned char *arr = rgb.data();
    stbi_write_png("output.png", WIDTH, HEIGHT, 3, arr, 0);

    return 0;
}
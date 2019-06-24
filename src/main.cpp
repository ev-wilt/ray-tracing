#include <iostream>
#include <vector>
#include <limits>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../lib/stb_image_write.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "Random.h"
#include "materials/Lambertian.h"
#include "materials/Metal.h"

// Image size
const int WIDTH = 600;
const int HEIGHT = 300;

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

int main() {
    int raysPerPixel = 300;
    Hitable *list[4];
    list[0] = new Sphere(Vector3(0.0, 0.0, -1.0), 0.5, new Lambertian(Vector3(0.8, 0.3, 0.3)));
    list[1] = new Sphere(Vector3(0.0, -100.5, -1.0), 100, new Lambertian(Vector3(0.8, 0.8, 0.0)));
    list[2] = new Sphere(Vector3(1.0, 0.0, -1.0), 0.5, new Metal(Vector3(0.8, 0.6, 0.2), 1));
    list[3] = new Sphere(Vector3(-1.0, 0.0, -1.0), 0.5, new Metal(Vector3(0.8, 0.8, 0.8), 0));
    Hitable *world = new HitableList(list, 4);
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
                col += color(ray, world, 0);
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
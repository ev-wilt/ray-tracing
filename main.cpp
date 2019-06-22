#include <iostream>
#include <vector>
#include <limits>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "lib/stb_image_write.h"
#include "Vector3.h"
#include "Ray.h"
#include "HitableList.h"
#include "Sphere.h"

Vector3 color(const Ray& ray, Hitable *world) {
    HitRecord record;
    if (world->hit(ray, 0.0, std::numeric_limits<float>::max(), record)) {
        return 0.5 * Vector3(record.normal.x() + 1, record.normal.y() + 1, record.normal.z() + 1);
    }
    else {
        Vector3 unitDirection = unitVector(ray.getDirection());
        float t = 0.5 * (unitDirection.y() + 1.0);
        return (1.0 - t) * Vector3(1.0, 1.0, 1.0) + t * Vector3(0.5, 0.7, 1.0);
    }
}

int main() {
    int width = 600;
    int height = 300;
    Vector3 lowerLeft(-2.0, -1.0, -1.0);
    Vector3 horizontal(4.0, 0.0, 0.0);
    Vector3 vertical(0.0, 2.0, 0.0);
    Vector3 origin(0.0, 0.0, 0.0);
    Hitable *list[2];
    list[0] = new Sphere(Vector3(0.0, 0.0, -1.0), 0.5);
    list[1] = new Sphere(Vector3(0.0, -100.5, -1.0), 100);
    Hitable *world = new HitableList(list, 2);
    std::vector<unsigned char> rgb;

    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {

            float u = float(x) / float(width);
            float v = float(y) / float(height);

            Ray ray(origin, lowerLeft + u * horizontal + v * vertical);
            Vector3 point = ray.pointAtParameter(2.0);
            Vector3 col = color(ray, world);
            rgb.push_back(255.99 * col[0]);
            rgb.push_back(255.99 * col[1]);
            rgb.push_back(255.99 * col[2]);
        }
    }

    unsigned char *arr = rgb.data();
    stbi_write_png("output.png", width, height, 3, arr, 0);

    return 0;
}
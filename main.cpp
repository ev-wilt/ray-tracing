#include <iostream>
#include <vector>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "lib/stb_image_write.h"
#include "Vector3.h"

int main() {
    int width = 200;
    int height = 100;
    std::vector<unsigned char> rgb;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float r = float(x) / float(width);
            float g = float(y) / float(height);
            float b = 0.2;

            rgb.push_back(255.99 * r);
            rgb.push_back(255.99 * g);
            rgb.push_back(255.99 * b);
        }
    }

    unsigned char *arr = rgb.data();
    stbi_write_png("output.png", width, height, 3, arr, 0);

    return 0;
}
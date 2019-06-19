#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "lib/stb_image_write.h"
#include "Vector3.h"

int main() {
    int nx = 200;
    int ny = 100;
//    unsigned char imageData [nx * ny * 3];
//    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int i = ny - 1; i >= 0; i--) {
        for (int j = 0; j < nx; j++) {
            Vector3 vec(float(j) / float(nx), float(i) / float(ny), 0.2);
            int ir = int(255.99 * vec[0]);
            int ig = int(255.99 * vec[1]);
            int ib = int(255.99 * vec[2]);
//            imageData[i * j * 3] = ir;
//            imageData[i * j * 3 + 1] = ig;
//            imageData[i * j * 3 + 2] = ib;
//            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
//    stbi_write_png("output", nx, ny, 3, imageData, 0);
    return 0;
}
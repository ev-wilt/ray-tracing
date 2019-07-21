//
// Created by Evan on 7/20/2019.
//

#ifndef RAYTRACING_IMAGETEXTURE_H
#define RAYTRACING_IMAGETEXTURE_H

#include <memory>
#include "Texture.h"

class ImageTexture : public Texture {
public:
    ImageTexture() {}
    ImageTexture(std::vector<unsigned char> img, int x, int y) : imageData(std::move(img)), width(x), height(y) {}

    Vector3 value(float u, float v, const Vector3& p) const override;

    std::vector<unsigned char> imageData;
    int width, height;
};

Vector3 ImageTexture::value(float u, float v, const Vector3 &p) const {
    int i = u * width;
    int j = (1 - v) * height - 0.001;

    if (i < 0) i = 0;
    else if (i > width - 1) i = width - 1;

    if (j < 0) j = 0;
    else if (j > height - 1) j = height - 1;

    float r = int(imageData[3 * i + 3 * width * j]) / 255.0;
    float g = int(imageData[3 * i + 3 * width * j + 1]) / 255.0;
    float b = int(imageData[3 * i + 3 * width * j + 2]) / 255.0;
    return Vector3(r, g, b);
}

#endif //RAYTRACING_IMAGETEXTURE_H

//
// Created by Evan on 7/15/2019.
//

#ifndef RAYTRACING_CONSTANTTEXTURE_H
#define RAYTRACING_CONSTANTTEXTURE_H

#include "Texture.h"

class ConstantTexture : public Texture {
public:
    ConstantTexture() {}
    ConstantTexture(Vector3 col) : color(col) {}

    Vector3 value(float u, float v, const Vector3& p) const override { return color; }

    Vector3 color;
};

#endif //RAYTRACING_CONSTANTTEXTURE_H

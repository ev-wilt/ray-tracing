//
// Created by Evan on 7/17/2019.
//

#ifndef RAYTRACING_NOISETEXTURE_H
#define RAYTRACING_NOISETEXTURE_H

#include "Texture.h"
#include "../Perlin.h"

class NoiseTexture : public Texture {
public:
    NoiseTexture() {}

    Vector3 value(float u, float v, const Vector3 &p) const override {
        return Vector3(1, 1, 1) * noise.noise(p);
    }

    Perlin noise;
};

#endif //RAYTRACING_NOISETEXTURE_H

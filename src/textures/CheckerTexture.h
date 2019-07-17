//
// Created by Evan on 7/15/2019.
//

#ifndef RAYTRACING_CHECKERTEXTURE_H
#define RAYTRACING_CHECKERTEXTURE_H

#include <memory>
#include "Texture.h"

class CheckerTexture : public Texture {
public:
    CheckerTexture() {}
    CheckerTexture(std::unique_ptr<Texture> even, std::unique_ptr<Texture> odd) : evenTex(std::move(even)), oddTex(std::move(odd)) {}

    Vector3 value(float u, float v, const Vector3& p) const override {
        float sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines > 0) {
            return oddTex->value(u, v, p);
        }
        return evenTex->value(u, v, p);
    }

    std::unique_ptr<Texture> evenTex;
    std::unique_ptr<Texture> oddTex;
};

#endif //RAYTRACING_CHECKERTEXTURE_H

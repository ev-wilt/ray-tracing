//
// Created by Evan on 7/21/2019.
//

#ifndef RAYTRACING_DIFFUSELIGHT_H
#define RAYTRACING_DIFFUSELIGHT_H

#include <memory>
#include "Material.h"
#include "../textures/Texture.h"

class DiffuseLight : public Material {
public:
    DiffuseLight(std::unique_ptr<Texture> e) : emit(std::move(e)) {}

    bool scatter(const Ray &rayIn, const HitRecord &record, Vector3 &attenuation, Ray &scattered) const override {
        return false;
    }
    Vector3 emitted(float u, float v, const Vector3 &p) const override {
        return emit->value(u, v, p);
    }

    std::unique_ptr<Texture> emit;
};

#endif //RAYTRACING_DIFFUSELIGHT_H

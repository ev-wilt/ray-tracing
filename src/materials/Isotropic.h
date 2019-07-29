//
// Created by Evan on 7/28/2019.
//

#ifndef RAYTRACING_ISOTROPIC_H
#define RAYTRACING_ISOTROPIC_H

#include <memory>
#include "Material.h"
#include "../textures/Texture.h"

class Isotropic : public Material {
public:
    Isotropic(std::unique_ptr<Texture> a) : albedo(std::move(a)) {}

    bool scatter(const Ray &rayIn, const HitRecord &record, Vector3 &attenuation, Ray &scattered) const override {
        scattered = Ray(record.p, randomPointInUnitSphere(), rayIn.getTime());
        attenuation = albedo->value(record.u, record.v, record.p);
        return true;
    }

    std::unique_ptr<Texture> albedo;
};

#endif //RAYTRACING_ISOTROPIC_H

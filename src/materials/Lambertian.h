//
// Created by Evan on 6/23/2019.
//

#ifndef RAYTRACING_LAMBERTIAN_H
#define RAYTRACING_LAMBERTIAN_H

#include "Material.h"
#include "../textures/Texture.h"

class Lambertian : public Material {
public:
    Lambertian(std::unique_ptr<Texture> alb) : albedo(std::move(alb)) {}

    bool scatter(const Ray &rayIn, const HitRecord &record, Vector3 &attenuation, Ray &scattered) const override {
        Vector3 target = record.p + record.normal + randomPointInUnitSphere();
        scattered = Ray(record.p, target - record.p, rayIn.getTime());
        attenuation = albedo->value(record.u, record.v, record.p);
        return true;
    }

    std::unique_ptr<Texture> albedo;
};

#endif //RAYTRACING_LAMBERTIAN_H

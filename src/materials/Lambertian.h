//
// Created by Evan on 6/23/2019.
//

#ifndef RAYTRACING_LAMBERTIAN_H
#define RAYTRACING_LAMBERTIAN_H

#include "Material.h"

class Lambertian : public Material {
public:
    Lambertian(const Vector3& alb) : albedo(alb) {}
    virtual bool scatter(const Ray& rayIn, const HitRecord& record, Vector3& attenuation, Ray& scattered) const {
        Vector3 target = record.p + record.normal + randomPointInUnitSphere();
        scattered = Ray(record.p, target - record.p);
        attenuation = albedo;
        return true;
    }

    Vector3 albedo;
};

#endif //RAYTRACING_LAMBERTIAN_H

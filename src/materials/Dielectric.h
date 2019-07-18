//
// Created by Evan on 6/25/2019.
//

#ifndef RAYTRACING_DIELECTRIC_H
#define RAYTRACING_DIELECTRIC_H

#include "Material.h"
#include "../Random.h"

class Dielectric : public Material {
public:
    Dielectric(float ref) : refractiveIndex(ref) {}

    bool scatter(const Ray &rayIn, const HitRecord &record, Vector3 &attenuation, Ray &scattered) const override {
        Vector3 outwardNormal;
        Vector3 reflected = reflect(rayIn.getDirection(), record.normal);
        float niOverNt;
        attenuation = Vector3(1.0, 1.0, 1.0);
        Vector3 refracted;
        float reflectProbability;
        float cosine;

        if (dotProduct(rayIn.getDirection(), record.normal) > 0) {
            outwardNormal = -record.normal;
            niOverNt = refractiveIndex;
            cosine = refractiveIndex * dotProduct(rayIn.getDirection(), record.normal) / rayIn.getDirection().length();
        }
        else {
            outwardNormal = record.normal;
            niOverNt = 1.0 / refractiveIndex;
            cosine = -dotProduct(rayIn.getDirection(), record.normal) / rayIn.getDirection().length();
        }

        // Check if new ray will be a reflection or refraction
        if (refract(rayIn.getDirection(), outwardNormal, niOverNt, refracted)) {
            reflectProbability = schlick(cosine, refractiveIndex);
        }
        else {
            reflectProbability = 1.0;
        }
        if (DIST(GEN) < reflectProbability) {
            scattered = Ray(record.p, reflected, rayIn.getTime());
        }
        else {
            scattered = Ray(record.p, refracted, rayIn.getTime());
        }
        return true;
    }

    float refractiveIndex;
};


#endif //RAYTRACING_DIELECTRIC_H

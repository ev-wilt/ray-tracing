//
// Created by Evan on 6/23/2019.
//

#ifndef RAYTRACING_METAL_H
#define RAYTRACING_METAL_H

#include "Material.h"

class Metal : public Material {
public:
    Metal(const Vector3& alb, float fuzz) : albedo(alb), fuzziness(fuzz) {}
    virtual bool scatter(const Ray& rayIn, const HitRecord& record, Vector3& attenuation, Ray& scattered) const {
        Vector3 reflected = reflect(unitVector(rayIn.getDirection()), record.normal);
        scattered = Ray(record.p, reflected + fuzziness * randomPointInUnitSphere());
        attenuation = albedo;
        return (dotProduct(scattered.getDirection(), record.normal) > 0);
    }

    Vector3 albedo;
    float fuzziness;
};

#endif //RAYTRACING_METAL_H

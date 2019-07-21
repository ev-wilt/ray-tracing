//
// Created by Evan on 6/23/2019.
//

#ifndef RAYTRACING_METAL_H
#define RAYTRACING_METAL_H

#include "Material.h"

class Metal : public Material {
public:
    Metal(std::unique_ptr<Texture> alb, float fuzz) : albedo(std::move(alb)), fuzziness(fuzz) {}

    bool scatter(const Ray &rayIn, const HitRecord &record, Vector3 &attenuation, Ray &scattered) const override {
        Vector3 reflected = reflect(unitVector(rayIn.getDirection()), record.normal);
        scattered = Ray(record.p, reflected + fuzziness * randomPointInUnitSphere(), rayIn.getTime());
        attenuation = albedo->value(record.u, record.v, record.p);
        return (dotProduct(scattered.getDirection(), record.normal) > 0);
    }

    std::unique_ptr<Texture> albedo;
    float fuzziness;
};

#endif //RAYTRACING_METAL_H

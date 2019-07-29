//
// Created by Evan on 7/28/2019.
//

#ifndef RAYTRACING_CONSTANTMEDIUM_H
#define RAYTRACING_CONSTANTMEDIUM_H

#include <memory>
#include "Hitable.h"
#include "../textures/Texture.h"
#include "../materials/Isotropic.h"

class ConstantMedium : public Hitable {
public:
    ConstantMedium(std::unique_ptr<Hitable> b, float d, std::unique_ptr<Texture> t) : boundary(std::move(b)), density(d) {
        phaseFunction = std::make_shared<Isotropic>(std::move(t));
    }

    bool hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const override;
    bool boundingBox(float tStart, float tEnd, AxisAlignedBoundingBox &boundingBox) const override {
        return boundary->boundingBox(tStart, tEnd, boundingBox);
    }

    std::unique_ptr<Hitable> boundary;
    float density;
    std::shared_ptr<Material> phaseFunction;
};

bool ConstantMedium::hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const {
    HitRecord record1, record2;

    if (boundary->hit(ray, std::numeric_limits<float>::min(), std::numeric_limits<float>::max(), record1)) {
        if (boundary->hit(ray, record1.t + 0.0001, std::numeric_limits<float>::max(), record2)) {
            if (record1.t < tMin) record1.t = tMin;
            if (record2.t > tMax) record2.t = tMax;
            if (record1.t >= record2.t) return false;
            if (record1.t < 0) record1.t = 0;

            float distanceWithinBoundary = (record2.t - record1.t) * ray.getDirection().length();
            float hitDistance = -(1 / density) * log(randomReal());
            if (hitDistance < distanceWithinBoundary) {
                record.t = record1.t + hitDistance / ray.getDirection().length();
                record.p = ray.pointAtParameter(record.t);
                record.normal = Vector3(1, 0, 0);
                record.material = phaseFunction;
                return true;
            }
        }
    }
    return false;
}

#endif //RAYTRACING_CONSTANTMEDIUM_H

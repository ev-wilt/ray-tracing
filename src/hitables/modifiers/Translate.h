//
// Created by Evan on 7/25/2019.
//

#ifndef RAYTRACING_TRANSLATE_H
#define RAYTRACING_TRANSLATE_H

#include <memory>

#include "../Hitable.h"

class Translate : public Hitable {
public:
    Translate(std::unique_ptr<Hitable> hit, const Vector3 &off) : hitable(std::move(hit)), offset(off) {}

    bool hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const override;
    bool boundingBox(float tStart, float tEnd, AxisAlignedBoundingBox &boundingBox) const override;

    std::unique_ptr<Hitable> hitable;
    Vector3 offset;
};

bool Translate::hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const {
    Ray translated(ray.getOrigin() - offset, ray.getDirection(), ray.getTime());

    if (hitable->hit(translated, tMin, tMax, record)) {
        record.p += offset;
        return true;
    }
    return false;
}

bool Translate::boundingBox(float tStart, float tEnd, AxisAlignedBoundingBox &boundingBox) const {
    if (hitable->boundingBox(tStart, tEnd, boundingBox)) {
        boundingBox = AxisAlignedBoundingBox(boundingBox.getMin() + offset, boundingBox.getMax() + offset);
        return true;
    }
    return false;
}

#endif //RAYTRACING_TRANSLATE_H

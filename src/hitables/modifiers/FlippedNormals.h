//
// Created by Evan on 7/22/2019.
//

#ifndef RAYTRACING_FLIPPEDNORMALS_H
#define RAYTRACING_FLIPPEDNORMALS_H

#include <memory>

#include "../Hitable.h"

class FlippedNormals : public Hitable {
public:
    FlippedNormals(std::unique_ptr<Hitable> h) : hitable(std::move(h)) {}

    bool hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const override {
        if (hitable->hit(ray, tMin, tMax, record)) {
            record.normal = -record.normal;
            return true;
        }
        return false;
    }
    bool boundingBox(float tStart, float tEnd, AxisAlignedBoundingBox &boundingBox) const override {
        return hitable->boundingBox(tStart, tEnd, boundingBox);
    }

    std::unique_ptr<Hitable> hitable;
};

#endif //RAYTRACING_FLIPPEDNORMALS_H

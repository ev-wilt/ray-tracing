//
// Created by Evan on 7/25/2019.
//

#ifndef RAYTRACING_ROTATEY_H
#define RAYTRACING_ROTATEY_H

#include <memory>
#define _USE_MATH_DEFINES
#include <math.h>

#include "../Hitable.h"

class RotateY : public Hitable {
public:
    RotateY(std::unique_ptr<Hitable> hit, float angle);

    bool hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const override;
    bool boundingBox(float tStart, float tEnd, AxisAlignedBoundingBox &boundingBox) const override {
        boundingBox = aabb;
        return hasBox;
    }

    std::unique_ptr<Hitable> hitable;
    float sinTheta, cosTheta;
    bool hasBox;
    AxisAlignedBoundingBox aabb;
};

RotateY::RotateY(std::unique_ptr<Hitable> hit, float angle) : hitable(std::move(hit)) {
    float radians = (M_PI / 180.0) * angle;
    sinTheta = sin(radians);
    cosTheta = cos(radians);
    hasBox = hitable->boundingBox(0, 1, aabb);

    Vector3 min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Vector3 max(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());

    // Calculate the axis aligned bounding box
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                float x = i * aabb.getMax().x() + (1 - i) * aabb.getMin().x();
                float y = j * aabb.getMax().y() + (1 - j) * aabb.getMin().y();
                float z = k * aabb.getMax().z() + (1 - k) * aabb.getMin().z();

                float newX = cosTheta * x + sinTheta * z;
                float newZ = -sinTheta * x + cosTheta * z;
                Vector3 temp(newX, y, newZ);
                for (int c = 0; c < 3; ++c) {
                    if (temp[c] > max[c]) {
                        max[c] = temp[c];
                    }
                    if (temp[c] < min[c]) {
                        min[c] = temp[c];
                    }
                }
            }
        }
    }
    aabb = AxisAlignedBoundingBox(min, max);
}

bool RotateY::hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const {
    Vector3 origin = ray.getOrigin();
    Vector3 direction = ray.getDirection();

    origin[0] = cosTheta * ray.getOrigin()[0] - sinTheta * ray.getOrigin()[2];
    origin[2] = sinTheta * ray.getOrigin()[0] + cosTheta * ray.getOrigin()[2];
    direction[0] = cosTheta * ray.getDirection()[0] - sinTheta * ray.getDirection()[2];
    direction[2] = sinTheta * ray.getDirection()[0] + cosTheta * ray.getDirection()[2];
    Ray rotated(origin, direction, ray.getTime());

    if (hitable->hit(rotated, tMin, tMax, record)) {
        Vector3 p = record.p;
        Vector3 normal = record.normal;

        p[0] = cosTheta * record.p[0] + sinTheta * record.p[2];
        p[2] = -sinTheta * record.p[0] + cosTheta * record.p[2];
        normal[0] = cosTheta * record.normal[0] + sinTheta * record.normal[2];
        normal[2] = -sinTheta * record.normal[0] + cosTheta * record.normal[2];
        record.p = p;
        record.normal = normal;
        return true;
    }
    return false;
}

#endif //RAYTRACING_ROTATEY_H

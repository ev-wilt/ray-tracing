//
// Created by Evan on 6/22/2019.
//

#ifndef RAYTRACING_HITABLE_H
#define RAYTRACING_HITABLE_H

#include "../Ray.h"
#include "../AxisAlignedBoundingBox.h"

class Material;

struct HitRecord {
    float t;
    float u;
    float v;
    Vector3 p;
    Vector3 normal;
    std::shared_ptr<Material> material;
};

class Hitable {
public:
    virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const = 0;
    virtual bool boundingBox(float tStart, float tEnd, AxisAlignedBoundingBox &boundingBox) const = 0;
};

#endif //RAYTRACING_HITABLE_H

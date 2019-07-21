//
// Created by Evan on 7/21/2019.
//

#ifndef RAYTRACING_TWODRECTANGLE_H
#define RAYTRACING_TWODRECTANGLE_H

#include <memory>
#include "Hitable.h"

class TwoDRectangle : public Hitable {
public:
    TwoDRectangle() {}
    TwoDRectangle(float x0, float x1, float y0, float y1, float k, std::shared_ptr<Material> mat) :
    xMin(x0), xMax(x1), yMin(y0), yMax(y1), z(k), material(std::move(mat)) {}

    bool hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const override;
    bool boundingBox(float tStart, float tEnd, AxisAlignedBoundingBox &boundingBox) const override {
        boundingBox = AxisAlignedBoundingBox(Vector3(xMin, yMin, z - 0.0001), Vector3(xMax, yMax, z + 0.0001));
        return true;
    }

    std::shared_ptr<Material> material;
    float xMin, xMax, yMin, yMax, z;
};


bool TwoDRectangle::hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const {
    float t = (z - ray.getOrigin().z()) / ray.getDirection().z();
    if (t < tMin || t > tMax) return false;

    float x = ray.getOrigin().x() + t * ray.getDirection().x();
    float y = ray.getOrigin().y() + t * ray.getDirection().y();
    if (x < xMin || x > xMax || y < yMin || y > yMax) return false;

    record.u = (x - xMin) / (xMax - xMin);
    record.v = (y - yMin) / (yMax - yMin);
    record.t = t;
    record.material = material;
    record.p = ray.pointAtParameter(t);
    record.normal = Vector3(0, 0, 1);
    return true;
}

#endif //RAYTRACING_TWODRECTANGLE_H

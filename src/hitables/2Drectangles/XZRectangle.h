//
// Created by Evan on 7/22/2019.
//

#ifndef RAYTRACING_XZRECTANGLE_H
#define RAYTRACING_XZRECTANGLE_H

#include <memory>

#include "../Hitable.h"

class XZRectangle : public Hitable {
public:
    XZRectangle() {}
    XZRectangle(float x0, float x1, float z0, float z1, float k, std::shared_ptr<Material> mat) :
            xMin(x0), xMax(x1), zMin(z0), zMax(z1), y(k), material(std::move(mat)) {}

    bool hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const override;
    bool boundingBox(float tStart, float tEnd, AxisAlignedBoundingBox &boundingBox) const override {
        boundingBox = AxisAlignedBoundingBox(Vector3(xMin, y - 0.0001, zMin), Vector3(xMax, y + 0.0001, zMax));
        return true;
    }

    std::shared_ptr<Material> material;
    float xMin, xMax, zMin, zMax, y;
};


bool XZRectangle::hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const {
    float t = (y - ray.getOrigin().y()) / ray.getDirection().y();
    if (t < tMin || t > tMax) return false;

    float x = ray.getOrigin().x() + t * ray.getDirection().x();
    float z = ray.getOrigin().z() + t * ray.getDirection().z();
    if (x < xMin || x > xMax || z < zMin || z > zMax) return false;

    record.u = (x - xMin) / (xMax - xMin);
    record.v = (z - zMin) / (zMax - zMin);
    record.t = t;
    record.material = material;
    record.p = ray.pointAtParameter(t);
    record.normal = Vector3(0, 1, 0);
    return true;
}


#endif //RAYTRACING_XZRECTANGLE_H

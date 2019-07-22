//
// Created by Evan on 7/22/2019.
//

#ifndef RAYTRACING_YZRECTANGLE_H
#define RAYTRACING_YZRECTANGLE_H

#include <memory>

#include "../Hitable.h"

class YZRectangle : public Hitable {
public:
    YZRectangle() {}
    YZRectangle(float y0, float y1, float z0, float z1, float k, std::shared_ptr<Material> mat) :
            yMin(y0), yMax(y1), zMin(z0), zMax(z1), x(k), material(std::move(mat)) {}

    bool hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const override;
    bool boundingBox(float tStart, float tEnd, AxisAlignedBoundingBox &boundingBox) const override {
        boundingBox = AxisAlignedBoundingBox(Vector3(x - 0.0001, yMin, zMin), Vector3(x + 0.0001, yMax, zMax));
        return true;
    }

    std::shared_ptr<Material> material;
    float yMin, yMax, zMin, zMax, x;
};


bool YZRectangle::hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const {
    float t = (x - ray.getOrigin().x()) / ray.getDirection().x();
    if (t < tMin || t > tMax) return false;

    float y = ray.getOrigin().y() + t * ray.getDirection().y();
    float z = ray.getOrigin().z() + t * ray.getDirection().z();
    if (y < yMin || y > yMax || z < zMin || z > zMax) return false;

    record.u = (y - yMin) / (yMax - yMin);
    record.v = (z - zMin) / (zMax - zMin);
    record.t = t;
    record.material = material;
    record.p = ray.pointAtParameter(t);
    record.normal = Vector3(1, 0, 0);
    return true;
}


#endif //RAYTRACING_YZRECTANGLE_H

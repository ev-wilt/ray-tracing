//
// Created by Evan on 7/23/2019.
//

#ifndef RAYTRACING_BOX_H
#define RAYTRACING_BOX_H

#include <memory>
#include <vector>
#include "Hitable.h"
#include "2Drectangles/XYRectangle.h"
#include "HitableList.h"
#include "modifiers/FlippedNormals.h"
#include "2Drectangles/XZRectangle.h"
#include "2Drectangles/YZRectangle.h"

class Box : public Hitable {
public:
    Box() {}
    Box(const Vector3 &p0, const Vector3 &p1, std::shared_ptr<Material> r) : pMin(p0), pMax(p1) {
        std::vector<std::unique_ptr<Hitable>> list(6);

        list[0] = std::make_unique<XYRectangle>(pMin.x(), pMax.x(), pMin.y(), pMax.y(), pMax.z(), r);
        list[1] = std::make_unique<FlippedNormals>(std::make_unique<XYRectangle>(pMin.x(), pMax.x(), pMin.y(), pMax.y(), pMin.z(), r));
        list[2] = std::make_unique<XZRectangle>(pMin.x(), pMax.x(), pMin.z(), pMax.z(), pMax.y(), r);
        list[3] = std::make_unique<FlippedNormals>(std::make_unique<XZRectangle>(pMin.x(), pMax.x(), pMin.z(), pMax.z(), pMin.y(), r));
        list[4] = std::make_unique<YZRectangle>(pMin.y(), pMax.y(), pMin.z(), pMax.z(), pMax.x(), r);
        list[5] = std::make_unique<FlippedNormals>(std::make_unique<YZRectangle>(pMin.y(), pMax.y(), pMin.z(), pMax.z(), pMin.x(), r));
        rects = std::make_unique<HitableList>(std::move(list), 6);
   }

    bool hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const override;
    bool boundingBox(float tStart, float tEnd, AxisAlignedBoundingBox &boundingBox) const override {
        boundingBox = AxisAlignedBoundingBox(pMin, pMax);
        return true;
    }

    Vector3 pMin, pMax;
    std::unique_ptr<Hitable> rects;
};

bool Box::hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const {
    return rects->hit(ray, tMin, tMax, record);
}

#endif //RAYTRACING_BOX_H

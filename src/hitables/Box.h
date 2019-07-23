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
#include "FlippedNormals.h"
#include "2Drectangles/XZRectangle.h"
#include "2Drectangles/YZRectangle.h"

class Box : public Hitable {
public:
    Box() {}
    Box(const Vector3 &p0, const Vector3 &p1, std::shared_ptr<Material> r) : pMin(p0), pMax(p1) {
        std::vector<std::unique_ptr<Hitable>> list;

        list.push_back(std::make_unique<XYRectangle>(pMin.x(), pMax.x(), pMin.y(), pMax.y(), pMax.z(), r));
        list.push_back(std::make_unique<FlippedNormals>(std::make_unique<XYRectangle>(pMin.x(), pMax.x(), pMin.y(), pMax.y(), pMin.z(), r)));
        list.push_back(std::make_unique<XZRectangle>(pMin.x(), pMax.x(), pMin.z(), pMax.z(), pMax.y(), r));
        list.push_back(std::make_unique<FlippedNormals>(std::make_unique<XZRectangle>(pMin.x(), pMax.x(), pMin.z(), pMax.z(), pMin.y(), r)));
        list.push_back(std::make_unique<YZRectangle>(pMin.y(), pMax.y(), pMin.z(), pMax.z(), pMax.x(), r));
        list.push_back(std::make_unique<FlippedNormals>(std::make_unique<YZRectangle>(pMin.y(), pMax.y(), pMin.z(), pMax.z(), pMin.x(), r)));
        rects = HitableList(std::move(list), 6);
   }

    bool hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const override;
    bool boundingBox(float tStart, float tEnd, AxisAlignedBoundingBox &boundingBox) const override {
        boundingBox = AxisAlignedBoundingBox(pMin, pMax);
        return true;
    }

    Vector3 pMin, pMax;
    HitableList rects;
};

bool Box::hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const {
    return rects.hit(ray, tMin, tMax, record);
}

#endif //RAYTRACING_BOX_H

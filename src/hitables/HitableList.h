//
// Created by Evan on 6/22/2019.
//

#ifndef RAYTRACING_HITABLELIST_H
#define RAYTRACING_HITABLELIST_H

#include "Hitable.h"

class HitableList : public Hitable {
public:
    HitableList() {}
    HitableList(std::vector<std::unique_ptr<Hitable>> l, int n) : list(std::move(l)), listSize(n) {};

    bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;
    bool boundingBox(float tStart, float tEnd, AxisAlignedBoundingBox& boundingBox) const override;

    std::vector<std::unique_ptr<Hitable>> list;
    int listSize;
};

// Checks for a hit on each hitable in list, saving the closest hit to record
bool HitableList::hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const {
    HitRecord tempRecord;
    bool hit = false;
    double closestSoFar = tMax;

    for (int i = 0; i < listSize; ++i) {
        if (list[i]->hit(ray, tMin, closestSoFar, tempRecord)) {
            hit = true;
            closestSoFar = tempRecord.t;
            record = tempRecord;
        }
    }
    return hit;
}

// Returns whether all objects have a bounding box and sets the boundingBox to a box
// containing all objects if they all have one
bool HitableList::boundingBox(float tStart, float tEnd, AxisAlignedBoundingBox &boundingBox) const {
    if (listSize > 1) return false;
    AxisAlignedBoundingBox tempBox;
    bool firstTrue = list[0]->boundingBox(tStart, tEnd, tempBox);
    if (!firstTrue) {
        return false;
    }
    else {
        boundingBox = tempBox;
    }
    for (int i = 1; i < listSize; ++i) {
        if (list[i]->boundingBox(tStart, tEnd, tempBox)) {
            boundingBox = surroundingBox(boundingBox, tempBox);
        }
        else {
            return false;
        }
    }
    return true;
}

#endif //RAYTRACING_HITABLELIST_H

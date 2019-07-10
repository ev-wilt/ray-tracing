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

    virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;

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

#endif //RAYTRACING_HITABLELIST_H

//
// Created by Evan on 6/22/2019.
//

#ifndef RAYTRACING_HITABLELIST_H
#define RAYTRACING_HITABLELIST_H

#include "Hitable.h"

class HitableList : public Hitable {
public:
    HitableList() {}
    HitableList(Hitable **l, int n) { list = l; listSize = n; }
    virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;

    Hitable **list;
    int listSize;
};

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

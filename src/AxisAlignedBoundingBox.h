//
// Created by Evan on 7/8/2019.
//

#ifndef RAYTRACING_AXISALIGNEDBOUNDINGBOX_H
#define RAYTRACING_AXISALIGNEDBOUNDINGBOX_H

#include "Vector3.h"
#include "Ray.h"

class AxisAlignedBoundingBox {
public:
    AxisAlignedBoundingBox() {}
    AxisAlignedBoundingBox(const Vector3& mi, const Vector3& ma) : min(mi), max(ma) {}

    Vector3 getMin() const { return min; }
    Vector3 getMax() const { return max; }
    bool hit(const Ray& ray, float tMin, float tMax) const {
        for (int i = 0; i < 3; ++i) {
            float tStart = fminf((min[i] - ray.getOrigin()[i]) / ray.getDirection()[i],
                                 (max[i] - ray.getOrigin()[i]) / ray.getDirection()[i]);
            float tEnd = fmaxf((min[i] - ray.getOrigin()[i]) / ray.getDirection()[i],
                                 (max[i] - ray.getOrigin()[i]) / ray.getDirection()[i]);
            tMin = fmaxf(tStart, tMin);
            tMax = fminf(tEnd, tMax);
            if (tMax <= tMin) return false;
        }
        return true;
    }

    Vector3 min;
    Vector3 max;
};

#endif //RAYTRACING_AXISALIGNEDBOUNDINGBOX_H

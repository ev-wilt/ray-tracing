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
            float inverseDir = 1.0f / ray.getDirection()[i];
            float tStart = (min[i] - ray.getOrigin()[i]) * inverseDir;
            float tEnd = (max[i] - ray.getOrigin()[i]) * inverseDir;
            tMin = tStart > tMin ? tStart : tMin;
            tMax = tEnd < tMax ? tEnd : tMax;
            if (tMax <= tMin) return false;
        }
        return true;
    }

    Vector3 min;
    Vector3 max;
};

// Returns the box surrounding both bStart and bEnd
AxisAlignedBoundingBox surroundingBox(AxisAlignedBoundingBox bStart, AxisAlignedBoundingBox bEnd) {
    Vector3 small(fmin(bStart.getMin().x(), bEnd.getMin().x()),
                  fmin(bStart.getMin().y(), bEnd.getMin().y()),
                  fmin(bStart.getMin().z(), bEnd.getMin().z()));
    Vector3 big(fmax(bStart.getMax().x(), bEnd.getMax().x()),
                fmax(bStart.getMax().y(), bEnd.getMax().y()),
                fmax(bStart.getMax().z(), bEnd.getMax().z()));
    return {small, big};
}

#endif //RAYTRACING_AXISALIGNEDBOUNDINGBOX_H

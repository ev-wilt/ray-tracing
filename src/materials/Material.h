//
// Created by Evan on 6/23/2019.
//

#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H

#include "../Ray.h"
#include "../Hitable.h"
#include "../Random.h"

class Material {
public:
    virtual bool scatter(const Ray& rayIn, const HitRecord& record, Vector3& attenuation, Ray& scattered) const = 0;
};

// Returns a randomized point within the unit sphere
Vector3 randomPointInUnitSphere() {
    Vector3 point;

    while (point.squaredLength() >= 1.0) {
        point = 2.0 * Vector3(DIST(GEN), DIST(GEN), DIST(GEN)) - Vector3(1.0, 1.0, 1.0);
    }
    return point;
}

Vector3 reflect(const Vector3& vec, const Vector3& unitVec) {
    return vec - 2 * dotProduct(vec, unitVec) * unitVec;
}

#endif //RAYTRACING_MATERIAL_H

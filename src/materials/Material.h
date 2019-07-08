//
// Created by Evan on 6/23/2019.
//

#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H

#include "../Ray.h"
#include "../hitables/Hitable.h"
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

// Returns the vector that represents a reflection of vec
Vector3 reflect(const Vector3& vec, const Vector3& unitVec) {
    return vec - 2 * dotProduct(vec, unitVec) * unitVec;
}

// Returns whether a refraction occurs or not and, if it does, sets refracted to
// the refraction of vec
bool refract(const Vector3& vec, const Vector3& n, float niOverNt, Vector3& refracted) {
    Vector3 unitVec = unitVector(vec);
    float dot = dotProduct(unitVec, n);
    float discriminant = 1.0 - pow(niOverNt, 2) * (1.0 - pow(dot, 2));
    if (discriminant > 0) {
        refracted = niOverNt * (unitVec - n * dot) - n * sqrt(discriminant);
        return true;
    }
    else {
        return false;
    }
}

// Returns the result of Schlick's approximation, the reflection coefficient
float schlick(float cosine, float refractiveIndex) {
    float r0 = pow(((1.0 - refractiveIndex) / (1.0 + refractiveIndex)), 2);
    return r0 + (1.0 - r0) * pow((1.0 - cosine), 5);
}

#endif //RAYTRACING_MATERIAL_H

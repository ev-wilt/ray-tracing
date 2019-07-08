//
// Created by Evan on 7/7/2019.
//

#ifndef RAYTRACING_MOVINGSPHERE_H
#define RAYTRACING_MOVINGSPHERE_H

#include "Hitable.h"

class MovingSphere : public Hitable {
public:
    MovingSphere() {}
    MovingSphere(Vector3 cStart, Vector3 cEnd, float tStart, float tEnd, float r, Material *m) :
        centerStart(cStart), centerEnd(cEnd), timeStart(tStart), timeEnd(tEnd), radius(r), material(m) {};

    virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;
    Vector3 center(float time) const;
    Vector3 centerStart, centerEnd;
    float timeStart, timeEnd;
    float radius;
    Material *material;
};

Vector3 MovingSphere::center(float time) const {
    return centerStart + ((time - timeStart) / (timeEnd - timeStart)) * (centerEnd - centerStart);
}

// Returns whether the ray was within the the sphere or not, saving the record of the hit
bool MovingSphere::hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const {
    Vector3 diff = ray.getOrigin() - center(ray.getTime());
    float a = dotProduct(ray.getDirection(), ray.getDirection());
    float b = dotProduct(diff, ray.getDirection());
    float c = dotProduct(diff, diff) - pow(radius, 2);
    float discriminant = pow(b, 2) - a * c;

    if (discriminant > 0) {
        float t = (-b - sqrt(pow(b, 2) - a * c)) / a;
        if (t < tMax && t > tMin) {
            record.t = t;
            record.p = ray.pointAtParameter(record.t);
            record.normal = (record.p - center(ray.getTime())) / radius;
            record.material = material;
            return true;
        }
        t = (-b + sqrt(pow(b, 2) - a * c)) / a;
        if (t < tMax && t > tMin) {
            record.t = t;
            record.p = ray.pointAtParameter(record.t);
            record.normal = (record.p - center(ray.getTime())) / radius;
            record.material = material;
            return true;
        }
    }
    return false;
}


#endif //RAYTRACING_MOVINGSPHERE_H

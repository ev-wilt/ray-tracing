//
// Created by Evan on 6/22/2019.
//

#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include "Hitable.h"

class Sphere : public Hitable {
public:
    Sphere() {}
    Sphere(Vector3 c, float r) : center(c), radius(r) {};
    virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;

    Vector3 center;
    float radius;
};

bool Sphere::hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const {
    Vector3 diff = ray.getOrigin() - center;
    float a = dotProduct(ray.getDirection(), ray.getDirection());
    float b = dotProduct(diff, ray.getDirection());
    float c = dotProduct(diff, diff) - pow(radius, 2);
    float discriminant = pow(b, 2) - a * c;

    if (discriminant > 0) {
        float t = (-b - sqrt(pow(b, 2) - a * c)) / a;
        if (t < tMax && t > tMin) {
            record.t = t;
            record.p = ray.pointAtParameter(record.t);
            record.normal = (record.p - center) / radius;
            return true;
        }
        t = (-b + sqrt(pow(b, 2) - a * c)) / a;
        if (t < tMax && t > tMin) {
            record.t = t;
            record.p = ray.pointAtParameter(record.t);
            record.normal = (record.p - center) / radius;
            return true;
        }
    }
    return false;
}

#endif //RAYTRACING_SPHERE_H

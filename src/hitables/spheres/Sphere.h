//
// Created by Evan on 6/22/2019.
//

#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include <math.h>

#include "../Hitable.h"

// Sets u and v to the coordinates that vector p points to
void getSphereUV(const Vector3 &p, float &u, float &v) {
    float phi = atan2(p.z(), p.x());
    float theta = asin(p.y());

    u = 1 - (phi + M_PI) / (2 * M_PI);
    v = (theta + M_PI / 2) / M_PI;
}

class Sphere : public Hitable {
public:
    Sphere() {}
    Sphere(Vector3 c, float r, std::shared_ptr<Material> m) : center(c), radius(r), material(std::move(m)) {};
    bool hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const override;
    bool boundingBox(float tStart, float tEnd, AxisAlignedBoundingBox &boundingBox) const override;

    Vector3 center;
    float radius;
    std::shared_ptr<Material> material;
};

// Returns whether the ray was within the the sphere or not, saving the record of the hit
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
            getSphereUV((record.p - center) / radius, record.u, record.v);
            record.normal = (record.p - center) / radius;
            record.material = material;
            return true;
        }
        t = (-b + sqrt(pow(b, 2) - a * c)) / a;
        if (t < tMax && t > tMin) {
            record.t = t;
            record.p = ray.pointAtParameter(record.t);
            getSphereUV((record.p - center) / radius, record.u, record.v);
            record.normal = (record.p - center) / radius;
            record.material = material;
            return true;
        }
    }
    return false;
}

// Returns whether the object has a bounding box and sets the boundingBox to it
// if the object has one
bool Sphere::boundingBox(float tStart, float tEnd, AxisAlignedBoundingBox &boundingBox) const {
    boundingBox = AxisAlignedBoundingBox(center - Vector3(radius, radius, radius), center + Vector3(radius, radius, radius));
    return true;
}

#endif //RAYTRACING_SPHERE_H

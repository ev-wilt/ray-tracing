//
// Created by Evan on 6/19/2019.
//

#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H

#include "Vector3.h"

class Ray {
public:
    Ray() {}
    Ray(const Vector3& ori, const Vector3& dir, float time) : origin(ori), direction(dir), creationTime(time) {}

    Vector3 getOrigin() const { return origin; }
    Vector3 getDirection() const { return direction; }
    float getTime() const { return creationTime; }
    Vector3 pointAtParameter(float t) const { return origin + t * direction; }

    Vector3 origin;
    Vector3 direction;
    float creationTime;
};
#endif //RAYTRACING_RAY_H

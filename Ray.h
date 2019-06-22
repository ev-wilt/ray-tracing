//
// Created by Evan on 6/19/2019.
//

#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H

#include "Vector3.h"

class Ray {
public:
    Ray() {}
    Ray(const Vector3& origin, const Vector3& direction) { this->origin = origin; this->direction = direction; }
    Vector3 getOrigin() const { return origin; }
    Vector3 getDirection() const { return direction; }
    Vector3 pointAtParameter(float t) const { return origin + t * direction; }

    Vector3 origin;
    Vector3 direction;
};
#endif //RAYTRACING_RAY_H

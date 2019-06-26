//
// Created by Evan on 6/22/2019.
//

#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

#define _USE_MATH_DEFINES
#include <math.h>

#include "Ray.h"

class Camera {
public:
    Camera(Vector3 position, Vector3 lookAt, Vector3 viewUp, float vertFoV, float aspect) {
        Vector3 u, v, w;
        float radians = vertFoV * M_PI / 180;
        float halfHeight = tan(radians / 2);
        float halfWidth = aspect * halfHeight;
        w = unitVector(position - lookAt);
        u = unitVector(crossProduct(viewUp, w));
        v = crossProduct(w, u);
        origin = position;
        lowerLeft = origin - halfWidth * u - halfHeight * v - w;
        horizontal = 2 * halfWidth * u;
        vertical = 2 * halfHeight * v;
    }
    Ray getRay(float s, float t) const { return Ray(origin, lowerLeft + s * horizontal + t * vertical - origin); }

    Vector3 origin;
    Vector3 lowerLeft;
    Vector3 horizontal;
    Vector3 vertical;
};

#endif //RAYTRACING_CAMERA_H

//
// Created by Evan on 6/22/2019.
//

#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

#define _USE_MATH_DEFINES
#include <math.h>

#include "Ray.h"
#include "Random.h"

Vector3 randomPointInUnitDisk() {
    Vector3 p;
    do {
        p = 2.0 * Vector3(DIST(GEN), DIST(GEN), 0.0) - Vector3(1.0, 1.0, 0.0);
    } while (dotProduct(p,p) >= 1.0);
    return p;
}

class Camera {
public:
    Camera(Vector3 position, Vector3 lookAt, Vector3 viewUp, float vertFoV, float aspect, float aperture, float focusDist, float tStart, float tEnd) :
            timeStart(tStart), timeEnd(tEnd)
    {
        float radians = vertFoV * M_PI / 180;
        float halfHeight = tan(radians / 2);
        float halfWidth = aspect * halfHeight;
        Vector3 w = unitVector(position - lookAt);
        u = unitVector(crossProduct(viewUp, w));
        v = crossProduct(w, u);
        lensRadius = aperture / 2;
        origin = position;
        lowerLeft = origin - halfWidth * focusDist * u - halfHeight * focusDist * v - focusDist * w;
        horizontal = 2 * halfWidth * focusDist * u;
        vertical = 2 * halfHeight * focusDist * v;
    }

    // Returns the ray cast at the given coordinates
    Ray getRay(float s, float t) const {
        Vector3 point = lensRadius * randomPointInUnitDisk();
        Vector3 offset = u * point.x() + v * point.y();
        float creationtTime = timeStart + DIST(GEN) * (timeEnd - timeStart);
        return Ray(origin + offset, lowerLeft + s * horizontal + t * vertical - origin - offset, creationtTime);
    }

    Vector3 origin;
    Vector3 lowerLeft;
    Vector3 horizontal;
    Vector3 vertical;
    Vector3 u, v;
    float lensRadius;
    float timeStart, timeEnd;
};

#endif //RAYTRACING_CAMERA_H

//
// Created by Evan on 6/22/2019.
//

#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

#include "Ray.h"

class Camera {
public:
    Camera() {
        origin = Vector3(0.0, 0.0, 0.0);
        lowerLeft = Vector3(-2.0, -1.0, -1.0);
        horizontal = Vector3(4.0, 0.0, 0.0);
        vertical = Vector3(0.0, 2.0, 0.0);
    }
    Ray getRay(float u, float v) const { return Ray(origin, lowerLeft + u * horizontal + v * vertical); }

    Vector3 origin;
    Vector3 lowerLeft;
    Vector3 horizontal;
    Vector3 vertical;
};

#endif //RAYTRACING_CAMERA_H

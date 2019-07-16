//
// Created by Evan on 7/15/2019.
//

#ifndef RAYTRACING_TEXTURE_H
#define RAYTRACING_TEXTURE_H

#include "../Vector3.h"

class Texture {
public:
    virtual Vector3 value(float u, float v, const Vector3& p) const = 0;
};

#endif //RAYTRACING_TEXTURE_H

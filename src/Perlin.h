//
// Created by Evan on 7/17/2019.
//

#ifndef RAYTRACING_PERLIN_H
#define RAYTRACING_PERLIN_H

#include <vector>

#include "Vector3.h"
#include "Random.h"


static std::vector<float> perlinGenerate() {
    auto p = std::vector<float>(256);
    for (float &i : p) {
        i = DIST(GEN);
    }
    return p;
}

void permute(std::vector<int> p) {
    for (int i = p.size() - 1; i > 0; --i) {
        int target = int(DIST(GEN) * (i + 1));
        int temp = p[i];
        p[i] = p[target];
        p[target] = temp;
    }
}

static std::vector<int> perlinGeneratePerm() {
    auto p = std::vector<int>(256);
    for (int i = 0; i < p.size(); ++i) {
        p[i] = i;
    }
    permute(p);
    return p;
}

class Perlin {
public:
    Perlin() : randFloat(perlinGenerate()), permX(perlinGeneratePerm()),
    permY(perlinGeneratePerm()), permZ(perlinGeneratePerm()) {}

    float noise(const Vector3 &p) const {
        float u = p.x() - floor(p.x());
        float v = p.y() - floor(p.y());
        float w = p.z() - floor(p.z());
        int i = int(4 * p.x()) & 255;
        int j = int(4 * p.y()) & 255;
        int k = int(4 * p.z()) & 255;

        return randFloat[permX[i] ^ permY[j] ^ permZ[k]];
    }

    std::vector<float> randFloat;
    std::vector<int> permX;
    std::vector<int> permY;
    std::vector<int> permZ;
};

#endif //RAYTRACING_PERLIN_H

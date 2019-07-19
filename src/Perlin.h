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

inline float trilinearInterpolation(float c[2][2][2], float u, float v, float w) {
    float accumulator = 0;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                accumulator += (i * u + (1 - i) * (1 - u)) *
                               (j * v + (1 - j) * (1 - v)) *
                               (k * w + (1 - k) * (1 - w)) *
                               c[i][j][k];
            }
        }
    }
    return accumulator;
}

class Perlin {
public:
    Perlin() : randFloat(perlinGenerate()), permX(perlinGeneratePerm()),
    permY(perlinGeneratePerm()), permZ(perlinGeneratePerm()) {}

    float noise(const Vector3 &p) const {
        int i = floor(p.x());
        int j = floor(p.y());
        int k = floor(p.z());
        float u = p.x() - i;
        float v = p.y() - j;
        float w = p.z() - k;
        float c[2][2][2];

        for (int dI = 0; dI < 2; ++dI) {
            for (int dJ = 0; dJ < 2; ++dJ) {
                for (int dK = 0; dK < 2; ++dK) {
                    c[dI][dJ][dK] = randFloat[permX[(i + dI) & 255] ^ permY[(j + dJ) & 255] ^ permZ[(k + dK) & 255]];
                }
            }
        }
        return trilinearInterpolation(c, pow(u, 2) * (3 - 2 * u), pow(v, 2) * (3 - 2 * v), pow(w, 2) * (3 - 2 * w));
    }

    std::vector<float> randFloat;
    std::vector<int> permX;
    std::vector<int> permY;
    std::vector<int> permZ;
};

#endif //RAYTRACING_PERLIN_H

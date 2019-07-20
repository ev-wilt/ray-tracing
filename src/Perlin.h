//
// Created by Evan on 7/17/2019.
//

#ifndef RAYTRACING_PERLIN_H
#define RAYTRACING_PERLIN_H

#include <vector>

#include "Vector3.h"
#include "Random.h"


static std::vector<Vector3> perlinGenerate() {
    std::vector<Vector3> p;
    p.reserve(256);
    for (int i = 0; i < 256; ++i) {
        p.push_back(unitVector(Vector3(-1 + 2 * randomReal(), -1 + 2 * randomReal(), -1 + 2 * randomReal())));
    }
    return p;
}

void permute(std::vector<int> p) {
    for (int i = p.size() - 1; i > 0; --i) {
        int target = int(randomReal() * (i + 1));
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

inline float interpolate(Vector3 c[2][2][2], float u, float v, float w) {
    float u2 = pow(u, 2) * (3 - 2 * u);
    float v2 = pow(v, 2) * (3 - 2 * v);
    float w2 = pow(w, 2) * (3 - 2 * w);
    float accumulator = 0;

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                Vector3 weight(u - i, v - j, w - k);
                accumulator += (i * u2 + (1 - i) * (1 - u2)) *
                               (j * v2 + (1 - j) * (1 - v2)) *
                               (k * w2 + (1 - k) * (1 - w2)) *
                               dotProduct(c[i][j][k], weight);
            }
        }
    }
    return accumulator;
}

class Perlin {
public:
    Perlin() : randVectors(perlinGenerate()), permX(perlinGeneratePerm()),
               permY(perlinGeneratePerm()), permZ(perlinGeneratePerm()) {}

    float noise(const Vector3 &p) const {
        int i = floor(p.x());
        int j = floor(p.y());
        int k = floor(p.z());
        float u = p.x() - i;
        float v = p.y() - j;
        float w = p.z() - k;
        Vector3 c[2][2][2];

        for (int dI = 0; dI < 2; ++dI) {
            for (int dJ = 0; dJ < 2; ++dJ) {
                for (int dK = 0; dK < 2; ++dK) {
                    c[dI][dJ][dK] = randVectors[permX[(i + dI) & 255] ^ permY[(j + dJ) & 255] ^ permZ[(k + dK) & 255]];
                }
            }
        }
        return interpolate(c, u, v, w);
    }

    float turbulence(const Vector3 &p, int depth = 7) const {
        float accumulator = 0;
        Vector3 temp = p;
        float weight = 1.0;
        for (int i = 0; i < depth; ++i) {
            accumulator += weight * noise(temp);
            weight *= 0.5;
            temp *= 2;
        }
        return fabs(accumulator);
    }

    std::vector<Vector3> randVectors;
    std::vector<int> permX;
    std::vector<int> permY;
    std::vector<int> permZ;
};

#endif //RAYTRACING_PERLIN_H

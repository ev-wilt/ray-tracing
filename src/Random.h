//
// Created by Evan on 6/23/2019.
//

#ifndef RAYTRACING_RANDOM_H
#define RAYTRACING_RANDOM_H

#include <random>

// Initializing random variables
std::random_device randomDevice;
std::mt19937 GEN(randomDevice());
std::uniform_real_distribution<> DIST(0.0, 1.0);

float randomReal() {
    return DIST(GEN);
}

#endif //RAYTRACING_RANDOM_H

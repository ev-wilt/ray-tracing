//
// Created by Evan on 6/23/2019.
//

#ifndef RAYTRACING_RANDOM_H
#define RAYTRACING_RANDOM_H

#include <random>

// Initializing random variables
static std::random_device randomDevice;
static std::mt19937 GEN(randomDevice());
static std::uniform_real_distribution<> DIST(0.0, 1.0);


#endif //RAYTRACING_RANDOM_H

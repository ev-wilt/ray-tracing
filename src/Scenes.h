//
// Created by Evan on 7/20/2019.
//

#ifndef RAYTRACING_SCENES_H
#define RAYTRACING_SCENES_H

#include <iostream>
#include <vector>
#include <limits>
#include <future>

#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

#include "hitables/HitableList.h"
#include "hitables/Sphere.h"
#include "Random.h"
#include "materials/Lambertian.h"
#include "materials/Metal.h"
#include "materials/Dielectric.h"
#include "hitables/MovingSphere.h"
#include "textures/ConstantTexture.h"
#include "textures/CheckerTexture.h"
#include "textures/NoiseTexture.h"
#include "textures/ImageTexture.h"

// Generates a random scene with 1 giant sphere, 3 large spheres, and several small spheres
std::unique_ptr<Hitable> randomScene() {
    int size = 50000;
    int index = 0;
    std::vector<std::unique_ptr<Hitable>> list(size + 1);
    auto firstTex = std::make_unique<ConstantTexture>(Vector3(0.2, 0.3, 0.1));
    auto secondTex = std::make_unique<ConstantTexture>(Vector3 (0.9, 0.9, 0.9));
    auto checkerTex = std::make_unique<CheckerTexture>(std::move(firstTex), std::move(secondTex));
    auto checkerMat = std::make_unique<Lambertian>(std::move(checkerTex));

    list[index++] = std::make_unique<Sphere>(Vector3(0, -1000, 0), 1000, std::move(checkerMat));
    for (int i = -10; i < 10; ++i) {
        for (int j = -10; j < 10; ++j) {
            float materialProb = randomReal();
            Vector3 center(i + 0.9 * randomReal(), 0.2, j + 0.9 * randomReal());
            if ((center - Vector3(4, 0.2, 0)).length() > 0.9) {
                if (materialProb < 0.8) {
                    Vector3 randomColor = Vector3(randomReal() * randomReal(),randomReal() * randomReal(), randomReal() * randomReal());
                    Vector3 centerEnd = center + Vector3(0, 0.5 * randomReal(), 0);
                    auto randomTex = std::make_unique<ConstantTexture>(randomColor);
                    auto lambertian = std::make_unique<Lambertian>(std::move(randomTex));
                    list[index++] = std::make_unique<MovingSphere>(center, centerEnd, 0.0, 1.0, 0.2, std::move(lambertian));
                }
                else if (materialProb < 0.95) {
                    Vector3 randomColor = Vector3(0.5 * (1 + randomReal()), 0.5 * (1 + randomReal()), 0.5 * (1 + randomReal()));
                    auto randomTex = std::make_unique<ConstantTexture>(randomColor);
                    auto metal = std::make_unique<Metal>(std::move(randomTex), 0.5);
                    list[index++] = std::make_unique<Sphere>(center, 0.2, std::move(metal));
                }
                else {
                    auto dielectric = std::make_unique<Dielectric>(1.5);
                    list[index++] = std::make_unique<Sphere>(center, 0.2, std::move(dielectric));
                }
            }
        }
    }

    auto dielectric = std::make_unique<Dielectric>(1.5);

    auto lambTex = std::make_unique<ConstantTexture>(Vector3(0.4, 0.2, 0.1));
    auto lambertian = std::make_unique<Lambertian>(std::move(lambTex));

    auto metTex = std::make_unique<ConstantTexture>(Vector3(0.7, 0.6, 0.5));
    auto metal = std::make_unique<Metal>(std::move(metTex), 0.0);

    list[index++] = std::make_unique<Sphere>(Vector3(0, 1, 0), 1.0, std::move(dielectric));
    list[index++] = std::make_unique<Sphere>(Vector3(-4, 1, 0), 1.0, std::move(lambertian));
    list[index++] = std::make_unique<Sphere>(Vector3(4, 1, 0), 1.0, std::move(metal));
    return std::make_unique<HitableList>(HitableList(std::move(list), index));
}

std::unique_ptr<Hitable> twoSpheres() {
    auto firstTex = std::make_unique<ConstantTexture>(Vector3(0.2, 0.3, 0.1));
    auto secondTex = std::make_unique<ConstantTexture>(Vector3 (0.9, 0.9, 0.9));
    auto checkerTex = std::make_unique<CheckerTexture>(std::move(firstTex), std::move(secondTex));
    auto checkerMat = std::make_shared<Lambertian>(std::move(checkerTex));

    int size = 50;
    std::vector<std::unique_ptr<Hitable>> list(size + 1);
    list[0] = std::make_unique<Sphere>(Vector3(0, -10, 0), 10, checkerMat);
    list[1] = std::make_unique<Sphere>(Vector3(0, 10, 0), 10, checkerMat);
    return std::make_unique<HitableList>(HitableList(std::move(list), 2));
}

std::unique_ptr<Hitable> twoPerlinSpheres() {
    auto perlinTex = std::make_unique<NoiseTexture>();
    auto perlinMat = std::make_shared<Metal>(std::move(perlinTex), 0.5);
    std::vector<std::unique_ptr<Hitable>> list(2);
    list[0] = std::make_unique<Sphere>(Vector3(0, -1000, 0), 1000, perlinMat);
    list[1] = std::make_unique<Sphere>(Vector3(0, 2, 0), 2, perlinMat);
    return std::make_unique<HitableList>(std::move(list), 2);
}

std::unique_ptr<Hitable> texturedSphere() {
    int width, height;
    unsigned char *buffer = stbi_load("../assets/earth.jpg", &width, &height, nullptr, 0);
    std::vector<unsigned char> imageData = std::vector<unsigned char>(buffer, buffer + width * height * 3);

    auto imageTex = std::make_unique<ImageTexture>(imageData, width, height);
    auto imageMat = std::make_shared<Lambertian>(std::move(imageTex));

    std::vector<std::unique_ptr<Hitable>> list(1);
    list[0] = std::make_unique<Sphere>(Vector3(4, 1, 0), 1.0, imageMat);
    return std::make_unique<HitableList>(HitableList(std::move(list), 1));
}


#endif //RAYTRACING_SCENES_H

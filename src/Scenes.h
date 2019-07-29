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
#include "hitables/spheres/Sphere.h"
#include "Random.h"
#include "materials/Lambertian.h"
#include "materials/Metal.h"
#include "materials/Dielectric.h"
#include "hitables/spheres/MovingSphere.h"
#include "textures/ConstantTexture.h"
#include "textures/CheckerTexture.h"
#include "textures/NoiseTexture.h"
#include "textures/ImageTexture.h"
#include "materials/DiffuseLight.h"
#include "hitables/2Drectangles/XYRectangle.h"
#include "hitables/2Drectangles/YZRectangle.h"
#include "hitables/2Drectangles/XZRectangle.h"
#include "hitables/modifiers/FlippedNormals.h"
#include "hitables/Box.h"
#include "hitables/modifiers/Translate.h"
#include "hitables/modifiers/RotateY.h"
#include "hitables/ConstantMedium.h"

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

std::unique_ptr<Hitable> simpleLight() {
    auto noiseTex = std::make_unique<NoiseTexture>(4);
    auto noiseMat = std::make_shared<Lambertian>(std::move(noiseTex));
    auto lightTex = std::make_unique<ConstantTexture>(Vector3(4, 4, 4));
    auto lightMat = std::make_shared<DiffuseLight>(std::move(lightTex));
    std::vector<std::unique_ptr<Hitable>> list(4);

    list[0] = std::make_unique<Sphere>(Vector3(0, -1000, 0), 1000, noiseMat);
    list[1] = std::make_unique<Sphere>(Vector3(0, 2, 0), 2, noiseMat);
    list[2] = std::make_unique<Sphere>(Vector3(0, 7, 0), 2, lightMat);
    list[3] = std::make_unique<XYRectangle>(3, 5, 1, 3, -2, lightMat);
    return std::make_unique<HitableList>(HitableList(std::move(list), 4));
}

std::unique_ptr<Hitable> cornellBox() {
    auto redTex = std::make_unique<ConstantTexture>(Vector3(0.65, 0.05, 0.05));
    auto redMat = std::make_shared<Lambertian>(std::move(redTex));

    auto whiteTex = std::make_unique<ConstantTexture>(Vector3(0.73, 0.73, 0.73));
    auto whiteMat = std::make_shared<Lambertian>(std::move(whiteTex));

    auto greenTex = std::make_unique<ConstantTexture>(Vector3(0.12, 0.45, 0.15));
    auto greenMat = std::make_shared<Lambertian>(std::move(greenTex));

    auto lightTex = std::make_unique<ConstantTexture>(Vector3(15, 15, 15));
    auto lightMat = std::make_shared<DiffuseLight>(std::move(lightTex));
    std::vector<std::unique_ptr<Hitable>> list(8);

    list[0] = std::make_unique<FlippedNormals>(std::make_unique<YZRectangle>(0, 555, 0, 555, 555, greenMat));
    list[1] = std::make_unique<YZRectangle>(0, 555, 0, 555, 0, redMat);
    list[2] = std::make_unique<XZRectangle>(213, 343, 227, 332, 554, lightMat);
    list[3] = std::make_unique<FlippedNormals>(std::make_unique<XZRectangle>(0, 555, 0, 555, 555, whiteMat));
    list[4] = std::make_unique<XZRectangle>(0, 555, 0, 555, 0, whiteMat);
    list[5] = std::make_unique<FlippedNormals>(std::make_unique<XYRectangle>(0, 555, 0, 555, 555, whiteMat));
    list[6] = std::make_unique<Translate>(std::make_unique<RotateY>(std::make_unique<Box>(Vector3(0, 0, 0), Vector3(165, 165, 165), whiteMat), -18), Vector3(130, 0, 65));
    list[7] = std::make_unique<Translate>(std::make_unique<RotateY>(std::make_unique<Box>(Vector3(0, 0, 0), Vector3(165, 330, 165), whiteMat), 15), Vector3(265, 0, 295));

    return std::make_unique<HitableList>(HitableList(std::move(list), 8));
}

std::unique_ptr<Hitable> cornellSmoke() {
    auto redTex = std::make_unique<ConstantTexture>(Vector3(0.65, 0.05, 0.05));
    auto redMat = std::make_shared<Lambertian>(std::move(redTex));

    auto whiteTex = std::make_unique<ConstantTexture>(Vector3(0.73, 0.73, 0.73));
    auto whiteMat = std::make_shared<Lambertian>(std::move(whiteTex));

    auto greenTex = std::make_unique<ConstantTexture>(Vector3(0.12, 0.45, 0.15));
    auto greenMat = std::make_shared<Lambertian>(std::move(greenTex));

    auto lightTex = std::make_unique<ConstantTexture>(Vector3(7, 7, 7));
    auto lightMat = std::make_shared<DiffuseLight>(std::move(lightTex));

    auto box1 = std::make_unique<Translate>(std::make_unique<RotateY>(std::make_unique<Box>(Vector3(0, 0, 0), Vector3(165, 165, 165), whiteMat), -18), Vector3(130, 0, 65));
    auto box2 = std::make_unique<Translate>(std::make_unique<RotateY>(std::make_unique<Box>(Vector3(0, 0, 0), Vector3(165, 330, 165), whiteMat), 15), Vector3(265, 0, 295));
    std::vector<std::unique_ptr<Hitable>> list(8);

    list[0] = std::make_unique<FlippedNormals>(std::make_unique<YZRectangle>(0, 555, 0, 555, 555, greenMat));
    list[1] = std::make_unique<YZRectangle>(0, 555, 0, 555, 0, redMat);
    list[2] = std::make_unique<XZRectangle>(113, 443, 127, 432, 554, lightMat);
    list[3] = std::make_unique<FlippedNormals>(std::make_unique<XZRectangle>(0, 555, 0, 555, 555, whiteMat));
    list[4] = std::make_unique<XZRectangle>(0, 555, 0, 555, 0, whiteMat);
    list[5] = std::make_unique<FlippedNormals>(std::make_unique<XYRectangle>(0, 555, 0, 555, 555, whiteMat));
    list[6] = std::make_unique<ConstantMedium>(std::move(box1), 0.01, std::make_unique<ConstantTexture>(Vector3(1.0, 1.0, 1.0)));
    list[7] = std::make_unique<ConstantMedium>(std::move(box2), 0.01, std::make_unique<ConstantTexture>(Vector3(0.0, 0.0, 0.0)));

    return std::make_unique<HitableList>(HitableList(std::move(list), 8));
}

#endif //RAYTRACING_SCENES_H

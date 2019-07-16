//
// Created by Evan on 7/13/2019.
//

#ifndef RAYTRACING_BVHNODE_H
#define RAYTRACING_BVHNODE_H

#include <memory>
#include <vector>
#include <iostream>

#include "Hitable.h"
#include "../Random.h"

struct {
    bool operator() (std::shared_ptr<Hitable> a, std::shared_ptr<Hitable> b) {
        AxisAlignedBoundingBox leftBox, rightBox;
        if (!a->boundingBox(0, 0, leftBox)|| !b->boundingBox(0, 0, rightBox)) {
            std::cerr << "Bounding box missing from BVH node's constructor\n";
        }
        if (leftBox.getMin().x() - rightBox.getMin().x() < 0.0) {
            return -1;
        }
        return 1;
    }
} boxXCompare;

struct {
    bool operator() (std::shared_ptr<Hitable> a, std::shared_ptr<Hitable> b) {
        AxisAlignedBoundingBox leftBox, rightBox;
        if (!a->boundingBox(0, 0, leftBox)|| !b->boundingBox(0, 0, rightBox)) {
            std::cerr << "Bounding box missing from BVH node's constructor\n";
        }
        if (leftBox.getMin().y() - rightBox.getMin().y() < 0.0) {
            return -1;
        }
        return 1;
    }
} boxYCompare;

struct {
    bool operator() (std::shared_ptr<Hitable> a, std::shared_ptr<Hitable> b) {
        AxisAlignedBoundingBox leftBox, rightBox;
        if (!a->boundingBox(0, 0, leftBox)|| !b->boundingBox(0, 0, rightBox)) {
            std::cerr << "Bounding box missing from BVH node's constructor\n";
        }
        if (leftBox.getMin().z() - rightBox.getMin().z() < 0.0) {
            return -1;
        }
        return 1;
    }
} boxZCompare;

class BvhNode : public Hitable {
public:
    BvhNode() {}
    BvhNode(std::vector<std::shared_ptr<Hitable>> list, int size, float timeStart, float timeEnd);

    bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;
    bool boundingBox(float tStart, float tEnd, AxisAlignedBoundingBox& boundingBox) const override;

    std::shared_ptr<Hitable> left;
    std::shared_ptr<Hitable> right;
    AxisAlignedBoundingBox box;
};

BvhNode::BvhNode(std::vector<std::shared_ptr<Hitable>> list, int size, float timeStart, float timeEnd) {
    int axis = int(DIST(GEN) * 3);

    if (axis == 0) {
        std::partial_sort(list.begin(), list.begin() + size, list.end(), boxXCompare);
    }
    else if (axis == 1) {
        std::partial_sort(list.begin(), list.begin() + size, list.end(), boxYCompare);
    }
    else {
        std::partial_sort(list.begin(), list.begin() + size, list.end(), boxZCompare);
    }
    if (size == 1) {
        left = right = list[0];
    }
    else if (size == 2) {
        left = list[0];
        right = list[1];
    }
    else {
        auto rightSub = std::vector<std::shared_ptr<Hitable>>(list.begin() + size / 2, list.end());
        left = std::unique_ptr<Hitable>(new BvhNode(list, size / 2, timeStart, timeEnd));
        right = std::unique_ptr<Hitable>(new BvhNode(rightSub, size - size / 2, timeStart, timeEnd));
    }
    AxisAlignedBoundingBox leftBox, rightBox;
    if (!left->boundingBox(timeStart, timeEnd, leftBox) || !right->boundingBox(timeStart, timeEnd, rightBox)) {
        std::cerr << "Bounding box missing from BVH node's constructor\n";
    }
    box = surroundingBox(leftBox, rightBox);
}

bool BvhNode::boundingBox(float tStart, float tEnd, AxisAlignedBoundingBox &boundingBox) const {
    boundingBox = box;
    return true;
}

bool BvhNode::hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const {
    if (box.hit(ray, tMin, tMax)) {
        HitRecord recLeft, recRight;
        bool hitLeft = left->hit(ray, tMin, tMax, recLeft);
        bool hitRight = right->hit(ray, tMin, tMax, recRight);

        if (hitLeft && hitRight) {
            if (recLeft.t < recRight.t) {
                record = recLeft;
            }
            else {
                record = recRight;
            }
            return true;
        }
        else if (hitLeft) {
            record = recLeft;
            return true;
        }
        else if (hitRight) {
            record = recRight;
            return true;
        }
        return false;
    }
    return false;
}

#endif //RAYTRACING_BVHNODE_H

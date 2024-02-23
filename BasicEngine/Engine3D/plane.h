#pragma once
#include "Object.h"

class Plane : public Object {
public:
    Plane(const glm::vec3& normal, float distance, bool isReflective, bool isTransparent) : Object(isReflective, isTransparent), normal(normal), distance(distance) {}

    Intersection intersect(const Ray& ray, int index) const override;
    glm::vec3 getColor(const glm::vec3 hitPoint, const glm::vec3 material);

private:
    glm::vec3 normal;
    float distance;
};
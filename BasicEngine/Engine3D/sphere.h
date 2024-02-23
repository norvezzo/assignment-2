#pragma once
#include "Object.h"

class Sphere : public Object {
public:
    Sphere(const glm::vec3& center, float radius, bool isReflective, bool isTransparent) : Object(isReflective, isTransparent), center(center), radius(radius) {}
    
    Intersection intersect(const Ray& ray, int index) const override;

private:
    glm::vec3 center;
    float radius;
};
#pragma once
#include <glm/glm.hpp>
#include "Intersection.h"
#include "Ray.h"

class Object {
public:
    virtual ~Object() {} 
    virtual Intersection intersect(const Ray& ray, int index) const = 0;

    bool reflective;
    bool transparent;

    bool isReflective() { return reflective; };
    bool isTransparent() { return transparent; };

protected:
    Object(bool reflective, bool transparent) : reflective(reflective), transparent(transparent) {};
};
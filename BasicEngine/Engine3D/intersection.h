#pragma once
#include <glm/glm.hpp>

class Intersection {
public:
    bool hit;
    int objectIndex;
    float distance;
    glm::vec3 normal;
    glm::vec3 point;

    Intersection();
    Intersection(bool hit, int objectIndex, float distance, glm::vec3 normal, glm::vec3 point);
};
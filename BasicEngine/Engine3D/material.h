#pragma once
#include <glm/glm.hpp>

class Material {
public:
    glm::vec3 color; // ambient and diffuse
    glm::vec3 specular;
    float shininess;

    Material(glm::vec3& color, float shininess);
};
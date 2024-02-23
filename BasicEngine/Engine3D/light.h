#pragma once
#include <glm/glm.hpp>

class Light {
public:
    glm::vec4 direction;
    glm::vec4 intensity;
    glm::vec4 position;
    bool isSpotlight;

    Light(const glm::vec4& direction, const glm::vec4& intensity, const glm::vec4& position, bool isSpotlight);

    void updatePosition(const glm::vec4& newPosition);
    void updateIntensity(const glm::vec4& newIntensity);
};
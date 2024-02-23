#include "Light.h"

Light::Light(const glm::vec4& direction, const glm::vec4& intensity, const glm::vec4& position, bool isSpotlight)
    : direction(direction), intensity(intensity), position(position), isSpotlight(isSpotlight) {
}

void Light::updatePosition(const glm::vec4& newPosition) {
    this->position = newPosition;
}

void Light::updateIntensity(const glm::vec4& newIntensity) {
    this->intensity = newIntensity;
}
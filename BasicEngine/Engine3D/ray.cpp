#include "Ray.h"

Ray::Ray() : origin(glm::vec3(0.0f)), direction(glm::vec3(0.0f, 0.0f, 1.0f)) {}

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
    : origin(origin), direction(glm::normalize(direction)) {}

glm::vec3 Ray::getOrigin() const { return origin; }
glm::vec3 Ray::getDirection() const { return direction; }
glm::vec3 Ray::pointAtParameter(float t) const {
    return origin + t * direction;
}
#include "Intersection.h"
#include <limits>

Intersection::Intersection() 
    : hit(false), objectIndex(-1), distance(std::numeric_limits<float>::max()), normal(glm::vec3(0,0,0)), point(glm::vec3(0, 0, 0)) {}

Intersection::Intersection(bool hit, int objectIndex, float distance, glm::vec3 normal, glm::vec3 point)
    : hit(hit), objectIndex(objectIndex), distance(distance), normal(normal), point(point) {}


#include "Plane.h"

Intersection Plane::intersect(const Ray& ray, int index) const {
    float t = -(this->distance + glm::dot(this->normal, ray.getOrigin())) / glm::dot(this->normal, ray.getDirection());

    if (t < 0)
        return Intersection();
    else
        return Intersection(true, index, t, -this->normal, ray.getOrigin() + t * ray.getDirection());
}

glm::vec3 Plane::getColor(const glm::vec3 hitPoint, const glm::vec3 material) {
    float scale_parameter = 0.5f;
    float chessboard = 0;

    if (hitPoint.x < 0) {
        chessboard += floor((0.5 - hitPoint.x) / scale_parameter);
    }
    else {
        chessboard += floor(hitPoint.x / scale_parameter);
    }

    if (hitPoint.y < 0) {
        chessboard += floor((0.5 - hitPoint.y) / scale_parameter);
    }
    else {
        chessboard += floor(hitPoint.y / scale_parameter);
    }

    chessboard = (chessboard * 0.5) - int(chessboard * 0.5);
    chessboard *= 2;
    if (chessboard > 0.5) {
        return 0.5f * material;
    }
    return material;
}
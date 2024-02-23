#include "Sphere.h"

Intersection Sphere::intersect(const Ray& ray, int index) const {
    glm::vec3 oc = ray.getOrigin() - this->center;
    float a = glm::dot(ray.getDirection(), ray.getDirection());
    float b = 2.0f * glm::dot(oc, ray.getDirection());
    float c = glm::dot(oc, oc) - this->radius * this->radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        float dist = (-b - sqrt(discriminant)) / (2.0f * a);
        if (dist > 0) {
            glm::vec3 intersectionPoint = ray.getOrigin() + dist * ray.getDirection();
            glm::vec3 normal = glm::normalize(intersectionPoint - this->center);
            return Intersection(true, index, dist, normal, intersectionPoint);
        }
    }
    return Intersection();
}
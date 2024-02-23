#include "Material.h"

Material::Material(glm::vec3& color, float shininess) : color(color), shininess(shininess), specular(glm::vec3(0.7, 0.7, 0.7)) {}
#pragma once
#include "light.h"
#include "camera.h"
#include "material.h"
#include "object.h"
#include "sphere.h"
#include "plane.h"
#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

class Parser {
public:
    Parser(const std::string& filename);

    bool parseScene(Camera* camera,
        std::vector<Light>* lights,
        std::vector<std::shared_ptr<Object>>* objects,
        std::vector<Material>* materials,
        glm::vec4* ambientLight);

    void fixColors(std::vector<std::shared_ptr<Object>>* objects, std::vector<Material>* materials); // if an object is reflective or transparent, change his color to black

private:
    std::string filename;
};
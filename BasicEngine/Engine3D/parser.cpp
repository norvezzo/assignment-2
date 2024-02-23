#include "parser.h"
#include <fstream>
#include <sstream>
#include <iostream>

Parser::Parser(const std::string& filename) : filename(filename) {}

bool Parser::parseScene(Camera* camera,
    std::vector<Light>* lights,
    std::vector<std::shared_ptr<Object>>* objects,
    std::vector<Material>* materials,
    glm::vec4* ambientLight) {

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    int lightIdx = 0;
    int spotlightIdx = 0;
    int intensityIdx = 0;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        char type;
        iss >> type;
        switch (type) {
        case 'e': {
            glm::vec3 eye;
            iss >> eye.x >> eye.y >> eye.z;
            camera->SetView(eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); }
            break;
        case 'a':
            iss >> ambientLight->r >> ambientLight->g >> ambientLight->b >> ambientLight->a;
            break;
        case 'd': {
            lightIdx++;
            glm::vec4 direction;
            bool isSpotlight;
            iss >> direction.x >> direction.y >> direction.z >> direction.w;
            isSpotlight = (direction.w == 1.0);
            lights->push_back(Light(direction, glm::vec4(0, 0, 0, 0), glm::vec4(0, 0, 0, 0), isSpotlight)); }
            break;
        case 'p': {
            glm::vec4 position;
            iss >> position.x >> position.y >> position.z >> position.w;
            for (int i = spotlightIdx; i < lightIdx; i++) {
                if (lights->at(i).isSpotlight) {
                    lights->at(i).updatePosition(position);
                    spotlightIdx = i + 1;
                    break;
                }
            } }
            break;
        case 'i': {
            glm::vec4 intensity;
            iss >> intensity.r >> intensity.g >> intensity.b >> intensity.a;
            lights->at(intensityIdx).updateIntensity(intensity);
            intensityIdx++; }
            break;
        case 'o': {
            glm::vec4 object;
            iss >> object.x >> object.y >> object.z >> object.w;
            if (object.w > 0)
                objects->push_back(std::make_shared<Sphere>(glm::vec3(object.x, object.y, object.z), object.w, false, false));
            else
                objects->push_back(std::make_shared<Plane>(glm::vec3(object.x, object.y, object.z), object.w, false, false)); }
            break;
        case 'r': {
            glm::vec4 object;
            iss >> object.x >> object.y >> object.z >> object.w;
            if (object.w > 0)
                objects->push_back(std::make_shared<Sphere>(glm::vec3(object.x, object.y, object.z), object.w, true, false));
            else
                objects->push_back(std::make_shared<Plane>(glm::vec3(object.x, object.y, object.z), object.w, true, false)); }
            break;
        case 't': {
            glm::vec4 object;
            iss >> object.x >> object.y >> object.z >> object.w;
            if (object.w > 0)
                objects->push_back(std::make_shared<Sphere>(glm::vec3(object.x, object.y, object.z), object.w, false, true));
            else
                objects->push_back(std::make_shared<Plane>(glm::vec3(object.x, object.y, object.z), object.w, false, true)); }
            break;
        case 'c': {
            glm::vec4 color;
            iss >> color.r >> color.g >> color.b >> color.a;
            materials->push_back(Material(glm::vec3(color.r, color.g, color.b), color.a)); }
            break;
        }
    }
    return true;
}

void Parser::fixColors(std::vector<std::shared_ptr<Object>>* objects, std::vector<Material>* materials) {
    for (int i = 0; i < materials->size(); i++) {
        if (objects->at(i)->isReflective() || objects->at(i)->isTransparent()) {
            materials->at(i).color = glm::vec3(0.0f, 0.0f, 0.0f);
        }
    }
}
#pragma once
#include "Camera.h"
#include "Light.h"  
#include "Object.h" 
#include "Material.h"
#include "Ray.h"
#include <vector>
#include <memory>
#include <glm/vec4.hpp> 

class Renderer {
public:
    Renderer(Camera* camera,
        const std::vector<Light>& lights,
        const std::vector<std::shared_ptr<Object>>& objects,
        const std::vector<Material>& materials,
        const glm::vec4& ambientLight,
        unsigned int width, unsigned int height);

    ~Renderer();

    void render();
    unsigned char* getImageBuffer() const;


private:
    Camera* camera;
    std::vector<Light> lights;
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<Material> materials;
    glm::vec4 ambientLight;
    unsigned char* imageBuffer;
    unsigned int imageWidth, imageHeight;

    glm::vec3 perPixel(const Ray& ray, int depth, bool isInside);
    void setImagePixel(int x, int y, const glm::vec3& color);
    glm::vec3 calculateRefractedRayDirection(const glm::vec3& incidentRayDir, const glm::vec3& normal, float n1, float n2, bool isInside);
};

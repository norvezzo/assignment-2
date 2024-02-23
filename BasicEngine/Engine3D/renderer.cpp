#include "Renderer.h"
#include "Plane.h"

Renderer::Renderer(Camera* camera,
                   const std::vector<Light>& lights,
                   const std::vector<std::shared_ptr<Object>>& objects,
                   const std::vector<Material>& materials,
                   const glm::vec4& ambientLight,
                   unsigned int width, unsigned int height) : 
                   camera(camera), lights(lights), objects(objects), materials(materials), ambientLight(ambientLight),
                   imageWidth(width), imageHeight(height) {
    imageBuffer = new unsigned char[width * height * 4];
    std::memset(imageBuffer, 0, width * height * 4);
}

Renderer::~Renderer() {
    delete[] imageBuffer;
}

glm::vec3 Renderer::calculateRefractedRayDirection(const glm::vec3& incidentRayDir, const glm::vec3& normal, float n1, float n2, bool isInside) {
    float ratio = n1 / n2;
    if (isInside) // if inside the sphere, switch the ratio
        ratio = n2 / n1; 

    float c = glm::dot(-normal, incidentRayDir); 
    float k = 1 - ratio * ratio * (1 - c * c);

    if (k < 0) {
        return glm::vec3(0.0f);
    }
    else {
        return ratio * incidentRayDir + (ratio * c - sqrt(k)) * normal;
    }
}

void Renderer::render() {
    for (unsigned int y = 0; y < imageHeight; ++y) {
        for (unsigned int x = 0; x < imageWidth; ++x) {
            glm::vec3 color(0.0f);
            for (int sy = 0; sy < 2; ++sy) {
                for (int sx = 0; sx < 2; ++sx) {
                    float offsetX = (sx + 0.5f) * 0.5f; // offset within the pixel for this sample
                    float offsetY = (sy + 0.5f) * 0.5f;
                    glm::vec3 sampleRayOrigin = camera->getPosition();
                    glm::vec3 sampleRayDirection = camera->calculateRayDirection(x + offsetX, y + offsetY, imageWidth, imageHeight);
                    Ray sampleRay(sampleRayOrigin, sampleRayDirection);
                    color += perPixel(sampleRay, 0, false);
                }
            }
            color /= 4.0f;
            setImagePixel(x, y, color);
        }
    }
}

glm::vec3 Renderer::perPixel(const Ray& ray, int depth, bool isInside) {
    if (depth >= 5) {
        return glm::vec3(0.0f);
    }

    glm::vec3 pixelColor(0.0f); // default color is black
    Intersection closestIntersection;

    for (int i = 0; i < objects.size(); i++) {
        const auto& obj = objects[i];
        Intersection currIntersection = obj->intersect(ray, i);
        if (currIntersection.distance >= 0.0f && currIntersection.distance < closestIntersection.distance) {
            closestIntersection = currIntersection;
        }
    }

    if (closestIntersection.hit) {
        Material material = materials[closestIntersection.objectIndex];
        pixelColor = glm::vec3(ambientLight) * material.color;

        for (const Light& light : lights) {
            glm::vec3 lightDir;
            glm::vec3 intensity = glm::vec3(light.intensity);
            if (!light.isSpotlight) {
                lightDir = -glm::normalize((glm::vec3(light.direction)));
            }
            else {
                lightDir = glm::normalize(glm::vec3(light.position) - closestIntersection.point);
                float angleCos = glm::dot(glm::normalize(-glm::vec3(light.direction)), lightDir);
                if (angleCos < light.position.w) { // check if inside the cone, if not then ignore
                    continue;
                }
            }

            if (objects.at(closestIntersection.objectIndex)->isReflective()) {
                glm::vec3 newColor(0.0f, 0.0f, 0.0f);
                glm::vec3 reflectionRayDir = glm::normalize(glm::reflect(ray.getDirection(), closestIntersection.normal));
                glm::vec3 reflectionRayOrigin = closestIntersection.point + closestIntersection.normal * 0.001f; // offset to avoid self-intersection
                Ray reflectionRay(reflectionRayOrigin, reflectionRayDir);
                glm::vec3 reflectionColor = perPixel(reflectionRay, depth + 1, isInside);
                newColor += reflectionColor;
                pixelColor = glm::clamp(newColor, 0.0f, 1.0f);
                continue;
            }

            if (objects.at(closestIntersection.objectIndex)->isTransparent()) {
                glm::vec3 newColor(0.0f, 0.0f, 0.0f);
                glm::vec3 refractedRayDir = glm::normalize(calculateRefractedRayDirection(glm::normalize(ray.getDirection()), glm::normalize(closestIntersection.normal), 1.0f, 1.5f, isInside));
                glm::vec3 refractedRayOrigin = closestIntersection.point - closestIntersection.normal; 
                Ray refractedRay(refractedRayOrigin, refractedRayDir);
                glm::vec3 refractedColor = perPixel(refractedRay, depth + 1, !isInside);
                newColor += refractedColor;
                pixelColor = glm::clamp(newColor, 0.0f, 1.0f);
                continue;
            }

            bool inShadow = false;
            glm::vec3 shadowRayOrigin = closestIntersection.point + closestIntersection.normal * 0.001f; // offset to avoid self-intersection
            glm::vec3 shadowRayDirection;

            if (!light.isSpotlight) {
                shadowRayDirection = -glm::normalize(glm::vec3(light.direction));
            }
            else {
                shadowRayDirection = glm::normalize((glm::vec3(light.position) - closestIntersection.point));
            }

            Ray shadowRay(shadowRayOrigin, shadowRayDirection);

            for (int i = 0; i < objects.size(); i++) {
                const auto& obj = objects[i];
                Intersection shadowIntersect = obj->intersect(shadowRay, i);
                if (shadowIntersect.hit && shadowIntersect.distance > 0.0f && shadowIntersect.distance < glm::length(glm::vec3(light.position) - shadowRayOrigin)) {
                    inShadow = true;
                    break;
                }
            }

            if (!inShadow) {
                // calculate diffuse lighting
                float diff = std::max(glm::dot(closestIntersection.normal, lightDir), 0.0f);
                pixelColor += diff * material.color * intensity;
                pixelColor = glm::clamp(pixelColor, 0.0f, 1.0f);
                // calculate specular lighting
                glm::vec3 viewDir = glm::normalize(ray.getOrigin() - closestIntersection.point);
                glm::vec3 reflectDir = glm::normalize(glm::reflect(-lightDir, closestIntersection.normal));
                float spec = std::pow(std::max(glm::dot(viewDir, reflectDir), 0.0f), material.shininess);
                pixelColor += spec * material.specular * intensity;
                pixelColor = glm::clamp(pixelColor, 0.0f, 1.0f);
            }         
        }

        std::shared_ptr<Object> closestObj = objects[closestIntersection.objectIndex];
        std::shared_ptr<Plane> plane = std::dynamic_pointer_cast<Plane>(closestObj);
        if (plane && !(plane->isReflective() || plane->isTransparent())) {
            pixelColor = plane->getColor(closestIntersection.point, pixelColor);
        }
    }
    return pixelColor;
}

void Renderer::setImagePixel(int x, int y, const glm::vec3& color) {
    glm::vec3 clampedColor = glm::clamp(color, glm::vec3(0.0f), glm::vec3(1.0f));
    int index = (y * imageWidth + x) * 4;
    imageBuffer[index] = static_cast<unsigned char>(clampedColor.r * 255.0f);
    imageBuffer[index + 1] = static_cast<unsigned char>(clampedColor.g * 255.0f);
    imageBuffer[index + 2] = static_cast<unsigned char>(clampedColor.b * 255.0f);
    imageBuffer[index + 3] = 255;
}

unsigned char* Renderer::getImageBuffer() const {
    return imageBuffer;
}

//void Renderer::render() {
//    for (unsigned int y = 0; y < imageHeight; ++y) {
//        for (unsigned int x = 0; x < imageWidth; ++x) {
//            glm::vec3 rayOrigin = camera->getPosition();
//            glm::vec3 rayDirection = camera->calculateRayDirection(x, y, imageWidth, imageHeight);
//           Ray ray(rayOrigin, rayDirection);
//            glm::vec3 color = perPixel(ray, 0);
//            setImagePixel(x, y, color);
//        }
//    }
//}
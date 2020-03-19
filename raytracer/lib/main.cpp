#include <memory>
#include <cmath>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "image.h"
#include "renderer.h"
#include "object.h"
#include "scene.h"
#include "material.h"
#include "light.h"
#include <iostream>
int main(int argc, char *argv[])
{
    Renderer test(glm::uvec2(1024, 768), std::acos(-1.0f) / 2.0f, 5);
    Scene scene;

    auto ivory = std::make_shared<Material>(glm::vec3(0.6, 0.3, 0.1),
        glm::vec3(0.4, 0.4, 0.3), 50.0f);
    auto rubber = std::make_shared<Material>(glm::vec3(0.9, 0.1, 0.0),
        glm::vec3(0.3, 0.1, 0.1), 10.0f);
    auto mirror = std::make_shared<Material>(glm::vec3(1.0, 1.0, 1.0),
        glm::vec3(0.0, 10.0, 0.8), 1425.0f);
    
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::vec3(-3, 0, -16), 2.0f, ivory)));
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::vec3(-1, -1.5, -12), 2.0f, rubber)));
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::vec3(1.5, -0.5, -18), 3.0f, rubber)));
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::vec3(7, 5, -18), 4.0f, mirror)));

    scene.point_lights().push_back(std::unique_ptr<PointLight>(
        new PointLight(glm::vec3(-20, 20, 20), 1.5f)));
    scene.point_lights().push_back(std::unique_ptr<PointLight>(
        new PointLight(glm::vec3(30, 50, -25), 1.8f)));
    scene.point_lights().push_back(std::unique_ptr<PointLight>(
        new PointLight(glm::vec3(30, 20, 30), 1.7f)));

    Image img = test.render(scene);
    img.save_bmp("test.bmp");
    
    return 0;
}

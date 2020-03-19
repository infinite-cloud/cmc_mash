#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "image.h"
#include "renderer.h"
#include "object.h"
#include "scene.h"
#include "material.h"
#include "light.h"

int main(int argc, char *argv[])
{
    Renderer test(glm::uvec2(1024, 768));
    Scene scene;

    auto mat_1 = std::make_shared<Material>(glm::vec3(0.4, 0.4, 0.3),
        glm::vec2(0.6, 0.3), 50.0f);
    auto mat_2 = std::make_shared<Material>(glm::vec3(0.3, 0.1, 0.1),
        glm::vec2(0.9, 0.1), 10.0f);
    
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::vec3(-3, 0, -16), 2.0f, mat_1)));
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::vec3(-1, -1.5, -12), 2.0f, mat_2)));
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::vec3(1.5, -0.5, -18), 3.0f, mat_2)));
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::vec3(7, 5, -18), 4.0f, mat_1)));

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

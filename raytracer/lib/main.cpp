#include <memory>
#include <cmath>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "image.h"
#include "renderer.h"
#include "object.h"
#include "scene.h"
#include "material.h"
#include "light.h"

int main(int argc, char *argv[])
{
    Renderer test(glm::uvec2(1024, 768), std::acos(-1.0d) / 2.0d, 5,
        true);
    Scene scene;

/*    auto ivory = std::make_shared<Material>(glm::dvec3(0.4, 0.4, 0.3),
        glm::dvec3(0.0), glm::dvec4(0.6, 0.3, 0.1, 0.0), 50.0d, 1.0d,
        Material::DIFFUSE);
    auto rubber = std::make_shared<Material>(glm::dvec3(0.3, 0.1, 0.1),
        glm::dvec3(0.0), glm::dvec4(0.9, 0.1, 0.0, 0.0), 10.0d, 1.0d,
        Material::DIFFUSE);
    auto mirror = std::make_shared<Material>(glm::dvec3(1.0, 1.0, 1.0),
        glm::dvec3(0.0), glm::dvec4(0.0, 10.0, 0.8, 0.0), 1425.0d, 1.0d,
        Material::SPECULAR);
    auto glass = std::make_shared<Material>(glm::dvec3(0.6, 0.7, 0.8),
        glm::dvec3(0.0), glm::dvec4(0.0, 0.5, 0.1, 0.8), 125.0d, 1.5d,
        Material::REFRACTIVE);

    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(-3, 0, -16), 2.0d, ivory)));
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(-1, -1.5, -12), 2.0d, glass)));
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(1.5, -0.5, -18), 3.0d, rubber)));
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(7, 5, -18), 4.0d, mirror)));
    scene.objects().push_back(std::unique_ptr<Plane>(
        new Plane(glm::normalize(glm::dvec3(0, 1, 0)),
        glm::dvec3(0, -3.5, 0), ivory)));

    scene.point_lights().push_back(std::unique_ptr<PointLight>(
        new PointLight(glm::dvec3(-20, 20, 20), 1.5d)));
    scene.point_lights().push_back(std::unique_ptr<PointLight>(
        new PointLight(glm::dvec3(30, 50, -25), 1.8d)));
    scene.point_lights().push_back(std::unique_ptr<PointLight>(
        new PointLight(glm::dvec3(30, 20, 30), 1.7d))); */

    auto rubber = Material(glm::dvec3(0),
        glm::dvec3(12), glm::dvec4(0.9, 0.1, 0.0, 0.0), 10.0d, 1.0d,
        Material::DIFFUSE);
    auto glass = Material(glm::dvec3(1,1,1)*.99d,
        glm::dvec3(0.0), glm::dvec4(0.0, 0.5, 0.1, 0.8), 125.0d, 1.5d,
        Material::REFRACTIVE);
    auto ivory = Material(glm::dvec3(.75,.75,.75),
        glm::dvec3(0.0), glm::dvec4(0.6, 0.3, 0.1, 0.0), 50.0d, 1.0d,
        Material::DIFFUSE);
    auto mirror = Material(glm::dvec3(1,1,1)*.99d,
        glm::dvec3(0.0), glm::dvec4(0.0, 10.0, 0.8, 0.0), 1425.0d, 1.0d,
        Material::SPECULAR);
 /*   scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(0, 0, -160), 50, rubber)));
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(60, 60, -160), 10, glass)));
    scene.objects().push_back(std::unique_ptr<Plane>(
        new Plane(glm::normalize(glm::dvec3(0, 1, 0)),
        glm::dvec3(0, -90, -160), glass)));
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(60, -60, -160), 10, ivory))); */
        
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(1e5+1,40.8,81.6), 1e5, &ivory)));
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(-1e5+99,40.8,81.6), 1e5, &ivory)));
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(50,40.8, 1e5), 1e5, &ivory)));
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(50, 1e5, 81.6), 1e5, &ivory)));
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(50,-1e5+81.6,81.6), 1e5, &ivory)));
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(27,16.5,47), 16.5, &mirror)));
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(73,16.5,78), 16.5, &glass)));
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(50,681.6-.27,81.6), 600, &rubber)));

    Image img = test.render(scene);
    img.save_bmp("test.bmp");
    
    return 0;
}

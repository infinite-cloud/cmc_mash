#include "scene_loader.h"
#include "light.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

std::unique_ptr<Scene> SceneLoader::scene_1()
{
    auto scene = std::unique_ptr<Scene>(new Scene());

    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(-3, 0, -16), 2.0d, &_ivory)));
    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(-1, -1.5, -12), 2.0d, &_glass)));
    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(1.5, -0.5, -18), 3.0d, &_rubber)));
    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(7, 5, -18), 4.0d, &_mirror)));
    scene->objects().push_back(std::unique_ptr<Plane>(
        new Plane(glm::normalize(glm::dvec3(0, 1, 0)),
        glm::dvec3(0, -3.5, 0), &_ivory)));

    scene->point_lights().push_back(std::unique_ptr<PointLight>(
        new PointLight(glm::dvec3(-20, 20, 20), 1.5d)));
    scene->point_lights().push_back(std::unique_ptr<PointLight>(
        new PointLight(glm::dvec3(30, 50, -25), 1.8d)));
    scene->point_lights().push_back(std::unique_ptr<PointLight>(
        new PointLight(glm::dvec3(30, 20, 30), 1.7d)));

    return scene;
}

std::unique_ptr<Scene> SceneLoader::scene_2()
{
    auto scene = std::unique_ptr<Scene>(new Scene());

    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(1e5 + 20, 0, -20), 1e5, &_ivory)));
    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(-1e5 - 20, 0, -20), 1e5, &_ivory)));
    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(0, 0, 1e5 + 40), 1e5, &_ivory)));
    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(0, 0, -1e5 - 40), 1e5, &_ivory)));
    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(0, -1e5 - 20, -20), 1e5, &_ivory)));
    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(0, 1e5 + 20, -20), 1e5, &_ivory)));
    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(8, -12, -24), 8, &_mirror)));
    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(-10, -15, -25), 5, &_glass)));
    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(0, 620, -20), 600, &_rubber)));

    scene->point_lights().push_back(std::unique_ptr<PointLight>(
        new PointLight(glm::dvec3(0, 5, -20), 1.5d)));

    return scene;
}

std::unique_ptr<Scene> SceneLoader::scene_3()
{
    return std::unique_ptr<Scene>(new Scene());
}

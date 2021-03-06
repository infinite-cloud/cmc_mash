#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "scene_loader.h"
#include "light.h"
#include "object.h"
#include "model.h"

std::unique_ptr<Scene> SceneLoader::scene_1()
{
    auto scene = std::unique_ptr<Scene>(new Scene());

    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(-3, 0, -16), 2.0d, &_green)));
    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(-1, -1.5, -12), 2.0d, &_glass)));
    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(1.5, -0.5, -18), 3.0d, &_rubber)));
    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(7, 5, -18), 4.0d, &_mirror)));
    scene->objects().push_back(std::unique_ptr<Plane>(
        new Plane(glm::normalize(glm::dvec3(0, 1, 0)),
        glm::dvec3(0, -3.5, 0), &_ivory)));
    scene->objects().push_back(std::unique_ptr<Cylinder>(
        new Cylinder(glm::dvec3(-5, -2.5, -14),
        glm::normalize(glm::dvec3(1, 0, 0.4)), 1.0d, 1.0d, &_blue)));

    std::vector<Vertex> v;
    std::vector<Triangle> t;

    v.push_back(Vertex { .position = glm::vec3(0, 0, -4),
        .normal = glm::vec3(0, 0, 1) });
    v.push_back(Vertex { .position = glm::vec3(2, 1, -4),
        .normal = glm::vec3(0, 0, 1) });
    v.push_back(Vertex { .position = glm::vec3(1, 2, -4),
        .normal = glm::vec3(0, 0, 1) });

    t.push_back(Triangle { .a = 0, .b = 1, .c = 2 });

    scene->objects().push_back(std::unique_ptr<Mesh>(
        new Mesh(v, t, &_glass)));

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
        new Sphere(glm::dvec3(0, 0, 1e5 + 0.5), 1e5, &_ivory)));
    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(0, 0, -1e5 - 40), 1e5, &_ivory)));
    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(0, -1e5 - 10, -20), 1e5, &_ivory)));
    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(0, 1e5 + 14, -20), 1e5, &_ivory)));
    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(8, -2, -24), 8, &_mirror)));
    scene->objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::dvec3(-10, -5, -18), 5, &_clean)));

    std::vector<Vertex> v;
    std::vector<Triangle> t;

    v.push_back(Vertex { .position = glm::vec3(-15, 13, -25),
        .normal = glm::vec3(0, -1, 0) });
    v.push_back(Vertex { .position = glm::vec3(15, 13, -25),
        .normal = glm::vec3(0, -1, 0) });
    v.push_back(Vertex { .position = glm::vec3(0, 13, -15),
        .normal = glm::vec3(0, -1, 0) });

    t.push_back(Triangle { .a = 0, .b = 1, .c = 2 });

    scene->objects().push_back(std::unique_ptr<Mesh>(
        new Mesh(v, t, &_rubber)));

    scene->point_lights().push_back(std::unique_ptr<PointLight>(
        new PointLight(glm::dvec3(0, 5, -20), 1.5d)));

    return scene;
}

std::unique_ptr<Scene> SceneLoader::scene_3()
{
    auto scene = std::unique_ptr<Scene>(new Scene());
    Model model;

    if (!model.load("../bunny.obj", &_glass))
    {
        return nullptr;
    }

    scene->objects().push_back(std::move(model.mesh()));
    scene->objects().push_back(std::unique_ptr<Plane>(
        new Plane(glm::normalize(glm::dvec3(0, 1, 0)),
        glm::dvec3(0), &_ivory)));

    scene->point_lights().push_back(std::unique_ptr<PointLight>(
        new PointLight(glm::dvec3(-1, 1.5, 1), 1.5d)));

    return scene;
}

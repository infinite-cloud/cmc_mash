#include <limits>

#include "scene.h"

std::optional<Intersection> Scene::find_intersection(const Ray &ray) const
{
    float closest = std::numeric_limits<float>::infinity();
    std::optional<Intersection> intersection = std::nullopt;

    for (const auto &o : _objects)
    {
        std::optional<Intersection> new_intersection =
            o->find_intersection(ray);

        if (new_intersection && new_intersection->distance() < closest)
        {
            closest = new_intersection->distance();
            intersection = new_intersection;
        }
    }

    return intersection;
}

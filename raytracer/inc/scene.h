#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <optional>

#include <glm/vec3.hpp>

#include "ray.h"
#include "object.h"
#include "light.h"

class Scene
{
    std::vector<std::unique_ptr<Object>> _objects;
    std::vector<std::unique_ptr<PointLight>> _point_lights;

public:
    std::vector<std::unique_ptr<Object>> &objects()
    {
        return _objects;
    }

    const std::vector<std::unique_ptr<Object>> &objects() const
    {
        return _objects;
    }

    std::vector<std::unique_ptr<PointLight>> &point_lights()
    {
        return _point_lights;
    }

    const std::vector<std::unique_ptr<PointLight>> &point_lights() const
    {
        return _point_lights;
    }

    std::optional<Intersection> find_intersection(const Ray &ray) const;
};

#endif // SCENE_H

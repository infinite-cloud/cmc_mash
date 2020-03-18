#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <optional>

#include <glm/vec3.hpp>

#include "ray.h"
#include "object.h"

class Scene
{
    std::vector<std::unique_ptr<Object>> _objects;

public:
    std::vector<std::unique_ptr<Object>> &objects() { return _objects; }
    std::optional<Intersection> find_intersection(const Ray &ray) const;
};

#endif // SCENE_H

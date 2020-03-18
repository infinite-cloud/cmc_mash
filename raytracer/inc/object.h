#ifndef OBJECT_H
#define OBJECT_H

#include <optional>
#include <memory>

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ray.h"
#include "material.h"

class Intersection
{
    glm::vec3 _point;
    glm::vec3 _normal;
    float _distance;
    
    std::shared_ptr<Material> _material;

public:
    Intersection(const glm::vec3 &point, const glm::vec3 &normal,
            float distance, const std::shared_ptr<Material> &material) :
        _point(point), _normal(normal), _distance(distance),
        _material(material) {}

    const glm::vec3 &point() const { return _point; }
    const glm::vec3 &normal() const { return _normal; }
    float distance() const { return _distance; }
    const std::shared_ptr<Material> &material() const { return _material; }
};

class Object
{
    std::shared_ptr<Material> _material;

public:
    Object(const std::shared_ptr<Material> &material) :
        _material(material) {}

    virtual std::optional<Intersection> find_intersection(const Ray &r)
        const = 0;
    const std::shared_ptr<Material> &material() const { return _material; }
};

class Sphere : public Object
{
    glm::vec3 _center;
    float _radius;

public:
    Sphere(const glm::vec3 &center, float radius,
        const std::shared_ptr<Material> &material) :
        Object(material), _center(center), _radius(radius) {}

    std::optional<Intersection> find_intersection(const Ray &r) const;
};

#endif // OBJECT_H

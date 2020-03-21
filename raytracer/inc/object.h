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
    glm::dvec3 _point;
    glm::dvec3 _normal;
    double _distance;
    
    const Material *_material;

public:
    Intersection(const glm::dvec3 &point, const glm::dvec3 &normal,
            double distance, const Material *material) :
        _point(point), _normal(normal), _distance(distance),
        _material(material) {}

    const glm::dvec3 &point() const { return _point; }
    const glm::dvec3 &normal() const { return _normal; }
    double distance() const { return _distance; }
    const Material *material() const { return _material; }
};

class Object
{
    const Material *_material;

public:
    Object(const Material *material) :
        _material(material) {}

    virtual std::optional<Intersection> find_intersection(const Ray &r)
        const = 0;
    const Material *material() const { return _material; }
};

class Sphere : public Object
{
    glm::dvec3 _center;
    double _radius;

public:
    Sphere(const glm::dvec3 &center, double radius,
            const Material *material) :
        Object(material), _center(center), _radius(radius) {}

    std::optional<Intersection> find_intersection(const Ray &r) const;
};

class Plane : public Object
{
    glm::dvec3 _normal;
    glm::dvec3 _point;

public:
    Plane(const glm::dvec3 &normal, const glm::dvec3 &point,
            const Material *material) :
        Object(material), _normal(normal), _point(point) {}

    std::optional<Intersection> find_intersection(const Ray &r) const;
};

#endif // OBJECT_H

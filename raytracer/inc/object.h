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

class Plane : public Object
{
    glm::vec3 _normal;
    glm::vec3 _point;

public:
    Plane(const glm::vec3 &normal, const glm::vec3 &point,
            const std::shared_ptr<Material> &material) :
        Object(material), _normal(normal), _point(point) {}

    std::optional<Intersection> find_intersection(const Ray &r) const;
};

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
};

struct Triangle
{
    unsigned a;
    unsigned b;
    unsigned c;
};

class Mesh : public Object
{
    std::vector<Vertex> _vertices;
    std::vector<Triangle> _triangles;

public:
    Mesh(std::vector<Vertex> vertices, std::vector<Triangle> triangles,
            const std::shared_ptr<Material> &material) :
        Object(material), _vertices(std::move(vertices)),
        _triangles(std::move(triangles)) {}

    std::optional<Intersection> find_intersection(const Ray &r) const;
    std::optional<Intersection> find_intersection(const Ray &r,
        const Triangle &t) const;
};

#endif // OBJECT_H

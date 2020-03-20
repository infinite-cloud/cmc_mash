#include <cmath>
#include <algorithm>

#include "object.h"

std::optional<Intersection> Sphere::find_intersection(const Ray &r) const
{
    glm::vec3 l = r.origin() - _center;
    float a = glm::dot(r.direction(), r.direction());
    float b = 2.0f * glm::dot(r.direction(), l);
    float c = glm::dot(l, l) - _radius * _radius;

    float d = b * b - 4 * c * a;

    if (d < 0)
    {
        return std::nullopt;
    }

    float t_0 = -(b + std::sqrt(d)) / (2 * a);
    float t_1 = -(b - std::sqrt(d)) / (2 * a);

    if (t_0 > t_1)
    {
        std::swap(t_0, t_1);
    }

    if (t_0 < 0)
    {
        t_0 = t_1;

        if (t_0 < 0)
        {
            return std::nullopt;
        }
    }

    glm::vec3 point = r.origin() + t_0 * r.direction();

    return Intersection
    (
        point,
        glm::normalize(point - _center),
        t_0,
        material()
    );
}

std::optional<Intersection> Plane::find_intersection(const Ray &r) const
{
    float denominator = glm::dot(r.direction(), _normal);

    if (std::abs(denominator) < 1e-3f)
    {
        return std::nullopt;
    }

    float d = glm::dot(_point, -_normal);
    float t = -(d + glm::dot(r.origin(), _normal)) / denominator;

    if (t < 0)
    {
        return std::nullopt;
    }

    return Intersection(r.origin() + t * r.direction(), _normal,
        t, material());
}

std::optional<Intersection> Mesh::find_intersection(const Ray &r) const
{
    float distance = std::numeric_limits<float>::infinity();
    std::optional<Intersection> intersection = std::nullopt;

    for (const auto &t : _triangles)
    {
        auto new_intersection = find_intersection(r, t);
        
        if (new_intersection && new_intersection->distance() < distance)
        {
            distance = new_intersection->distance();
            intersection = new_intersection;
        }
    }

    return intersection;
}

std::optional<Intersection> Mesh::find_intersection(const Ray &r,
    const Triangle &t) const
{
    const auto &a = _vertices[t.a];
    const auto &b = _vertices[t.b];
    const auto &c = _vertices[t.c];

    auto ab = b.position - a.position;
    auto ac = c.position - a.position;

    auto p_vec = glm::cross(r.direction(), ac);
    float d = glm::dot(ab, p_vec);

    if (std::abs(d) <= 1e-5f)
    {
        return std::nullopt;
    }

    float inv_d = 1.0f / d;

    auto t_vec = r.origin() - a.position;
    float u = glm::dot(t_vec, p_vec) * inv_d;

    if (u < 0.0f || u > 1.0f)
    {
        return std::nullopt;
    }

    auto q_vec = glm::cross(t_vec, ab);
    float v = glm::dot(r.direction(), q_vec) * inv_d;

    if (v < 0.0f || u + v > 1.0f)
    {
        return std::nullopt;
    }

    float t_0 = glm::dot(ac, q_vec) * inv_d;

    if (t_0 < 0.0f)
    {
        return std::nullopt;
    }

    return Intersection(r.origin() + t_0 * r.direction(),
        glm::normalize((1.0f - u - v) * a.normal +
        u * b.normal + v * c.normal),
        t_0, material());
}
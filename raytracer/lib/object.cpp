#include <cmath>
#include <algorithm>

#include "object.h"

std::optional<Intersection> Sphere::find_intersection(const Ray &r) const
{
    glm::dvec3 l = r.origin() - _center;
    double a = glm::dot(r.direction(), r.direction());
    double b = 2.0d * glm::dot(r.direction(), l);
    double c = glm::dot(l, l) - _radius * _radius;

    double d = b * b - 4 * c * a;

    if (d < 0)
    {
        return std::nullopt;
    }

    double t_0 = -(b + std::sqrt(d)) / (2 * a);
    double t_1 = -(b - std::sqrt(d)) / (2 * a);

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

    glm::dvec3 point = r.origin() + t_0 * r.direction();

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
    double denominator = glm::dot(r.direction(), _normal);

    if (std::abs(denominator) < 1e-3d)
    {
        return std::nullopt;
    }

    double d = glm::dot(_point, -_normal);
    double t = -(d + glm::dot(r.origin(), _normal)) / denominator;

    if (t < 0)
    {
        return std::nullopt;
    }

    return Intersection(r.origin() + t * r.direction(), _normal,
        t, material());
}

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

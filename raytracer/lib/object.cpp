#include <cmath>
#include <algorithm>

#include <glm/gtx/norm.hpp>

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

std::optional<Intersection> Mesh::find_intersection(const Ray &r) const
{
    double distance = std::numeric_limits<double>::infinity();
    std::optional<Intersection> intersection = std::nullopt;

    _bvh.search
    (
        r,
        [&](auto &o)
        {
            auto new_intersection = find_intersection(r, o);

            if (new_intersection && new_intersection->distance() < distance)
            {
                distance = new_intersection->distance();
                intersection = new_intersection;
            }
        }
    );

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
    double d = glm::dot(ab, p_vec);

    if (std::abs(d) <= 1e-5f)
    {
        return std::nullopt;
    }

    double inv_d = 1.0f / d;

    auto t_vec = r.origin() - a.position;
    double u = glm::dot(t_vec, p_vec) * inv_d;

    if (u < 0.0f || u > 1.0f)
    {
        return std::nullopt;
    }

    auto q_vec = glm::cross(t_vec, ab);
    double v = glm::dot(r.direction(), q_vec) * inv_d;

    if (v < 0.0f || u + v > 1.0f)
    {
        return std::nullopt;
    }

    double t_0 = glm::dot(ac, q_vec) * inv_d;

    if (t_0 < 0.0f)
    {
        return std::nullopt;
    }

    return Intersection(r.origin() + t_0 * r.direction(),
        glm::normalize((1.0f - u - v) * a.normal +
        u * b.normal + v * c.normal),
        t_0, material());
}

BoundingBox Mesh::calculate_box(const Triangle &t) const
{
    const auto &a = _vertices[t.a].position;
    const auto &b = _vertices[t.b].position;
    const auto &c = _vertices[t.c].position;

    return BoundingBox
    (
        glm::vec3
        (
            std::min(a.x, std::min(b.x, c.x)),
            std::min(a.y, std::min(b.y, c.y)),
            std::min(a.z, std::min(b.z, c.z))
        ),
        glm::vec3
        (
            std::max(a.x, std::max(b.x, c.x)),
            std::max(a.y, std::max(b.y, c.y)),
            std::max(a.z, std::max(b.z, c.z))
        )
    );
}

void Mesh::regen_bvh(size_t delta)
{
    _bvh = BVH<Triangle>::construct
    (
        ([this]()
        {
            std::vector<Triangle *> ts;

            for (auto &t : _triangles)
            {
                ts.push_back(&t);
            }

            return ts;
        })(),
        delta,
        [this](const auto& t)
        {
            return calculate_box(t);
        }
    );
}

std::optional<Intersection> Cylinder::find_intersection(const Ray &r) const
{
    double i_rd = glm::l2Norm(r.direction());
    glm::dvec3 alpha = _axis * glm::dot(r.direction(), _axis);
    glm::dvec3 delta_p = r.origin() - _bottom_center;
    glm::dvec3 beta = _axis * glm::dot(delta_p, _axis);
    glm::dvec3 center = _bottom_center + _axis * _height;
    double min_t = std::numeric_limits<double>::infinity();

    double a = glm::l2Norm(r.direction() - alpha);

    a *= a;

    double b = 2.0d * glm::dot(r.direction() - alpha, delta_p - beta);
    double c = glm::l2Norm(delta_p - beta);

    c = c * c - _radius * _radius;

    double d = b * b - 4 * a * c;

    if (d < 0.0d)
    {
        return std::nullopt;
    }
    else
    {
        d = std::sqrt(d);
		double t_1 = (-b + d) / (2 * a);
		double t_2 = (-b - d) / (2 * a);

        if (t_1 >= 0.0d)
        {
            if (glm::dot(_axis, (r.origin() - _bottom_center) +
                r.direction() * t_1) > 0.0d &&
                glm::dot(_axis, (r.origin() - center) +
                r.direction() * t_1) < 0.0d &&
                min_t > t_1)
            {
                min_t = t_1;
            }
        }
        if (t_2 >= 0)
        {
            if(glm::dot(_axis, (r.origin() - _bottom_center) +
                r.direction() * t_2) > 0.0d &&
                glm::dot(_axis, (r.origin() - center) +
                r.direction() * t_2) < 0.0d &&
                min_t > t_2)
            {
                min_t = t_2;
            }
        }
    }

    double denominator = glm::dot(r.direction(), _axis);

    if (denominator > 1e-5d)
    {
        glm::dvec3 co = _bottom_center - r.origin();
        double t_3 = glm::dot(co, _axis) / denominator;

        double l = glm::l2Norm(r.direction() * t_3 - co);

        if(t_3 > 0.0d && l * l <= _radius * _radius &&
            min_t > t_3)
        {
            min_t = t_3;
        }
    }
    else if (denominator < 1e-5d)
    {
        glm::dvec3 co = center - r.origin();
        double t_4 = glm::dot(co, _axis) / denominator;

        double l = glm::l2Norm(r.direction() * t_4 - co);

        if (t_4 > 0.0d && l * l <= _radius * _radius &&
            min_t > t_4)
        {
            min_t = t_4;
        }
    }

    if (min_t < std::numeric_limits<double>::infinity())
    {
        double t = min_t / i_rd;

        glm::dvec3 point = r.origin() + (min_t / i_rd) * r.direction();
        glm::dvec3 normal = glm::normalize(glm::cross(
            _axis, glm::cross(point - _bottom_center, _axis)));

        return Intersection(point, normal, t, material());
    }
    else
    {
        return std::nullopt;
    }
}

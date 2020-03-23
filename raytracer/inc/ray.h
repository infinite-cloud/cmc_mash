#ifndef RAY_H
#define RAY_H

#include <glm/vec3.hpp>

class Ray
{
    glm::dvec3 _origin;
    glm::dvec3 _direction;

public:
    Ray(const glm::dvec3 &origin, const glm::dvec3 &direction) :
        _origin(origin), _direction(direction) {}

    const glm::dvec3 &origin() const { return _origin; }
    const glm::dvec3 &direction() const { return _direction; }
};

#endif // RAY_H

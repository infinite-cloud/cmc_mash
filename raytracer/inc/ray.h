#ifndef RAY_H
#define RAY_H

#include <glm/vec3.hpp>

class Ray
{
    glm::vec3 _origin;
    glm::vec3 _direction;

public:
    Ray(const glm::vec3 &origin, const glm::vec3 &direction) :
        _origin(origin), _direction(direction) {}

    const glm::vec3 &origin() const { return _origin; }
    const glm::vec3 &direction() const { return _direction; }
};

#endif // RAY_H

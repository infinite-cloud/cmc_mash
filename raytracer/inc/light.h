#ifndef LIGHT_H
#define LIGHT_H

#include <glm/vec3.hpp>

class PointLight
{
    glm::dvec3 _position;
    double _intensity;

public:
    PointLight(const glm::dvec3 &position, double intensity) :
        _position(position), _intensity(intensity) {}

    const glm::dvec3 &position() const { return _position; }
    double intensity() const { return _intensity; }
};

#endif // LIGHT_H

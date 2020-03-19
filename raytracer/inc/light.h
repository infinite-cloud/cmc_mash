#ifndef LIGHT_H
#define LIGHT_H

#include <glm/vec3.hpp>

class PointLight
{
    glm::vec3 _position;
    float _intensity;

public:
    PointLight(const glm::vec3 &position, float intensity) :
        _position(position), _intensity(intensity) {}

    const glm::vec3 &position() const { return _position; }
    float intensity() const { return _intensity; }
};

#endif // LIGHT_H

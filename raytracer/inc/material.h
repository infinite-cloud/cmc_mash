#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/vec3.hpp>

class Material
{
    glm::vec3 _diffuse_color;

public:
    Material() : _diffuse_color(0.0f) {}
    Material(const glm::vec3 &diffuse_color) :
        _diffuse_color(diffuse_color) {}

    const glm::vec3 &diffuse_color() const { return _diffuse_color; }
};

#endif // MATERIAL_H

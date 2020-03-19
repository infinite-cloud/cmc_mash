#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/vec3.hpp>

class Material
{
    glm::vec3 _diffuse_color;
    glm::vec2 _albedo;
    float _specular_exponent;

public:
    Material() :
        _diffuse_color(0.0f), _albedo(1.0f), _specular_exponent(0.0f) {}
    Material(const glm::vec3 &diffuse_color, const glm::vec2 &albedo,
            float specular_exponent) :
        _diffuse_color(diffuse_color), _albedo(albedo),
        _specular_exponent(specular_exponent) {}

    const glm::vec3 &diffuse_color() const { return _diffuse_color; }
    const glm::vec2 &albedo() const { return _albedo; }
    float specular_exponent() const { return _specular_exponent; }
};

#endif // MATERIAL_H

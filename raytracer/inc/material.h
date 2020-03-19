#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Material
{
    glm::vec3 _diffuse_color;
    glm::vec4 _albedo;
    float _specular_exponent;
    float _refractive_index;

public:
    Material() :
        _diffuse_color(0.0f), _albedo(1.0f, 0.0f, 0.0f, 0.0f),
        _specular_exponent(0.0f), _refractive_index(1.0f) {}
    Material(const glm::vec3 &diffuse_color, const glm::vec4 &albedo,
            float specular_exponent, float refractive_index) :
        _diffuse_color(diffuse_color), _albedo(albedo),
        _specular_exponent(specular_exponent),
        _refractive_index(refractive_index) {}

    const glm::vec3 &diffuse_color() const { return _diffuse_color; }
    const glm::vec4 &albedo() const { return _albedo; }
    float specular_exponent() const { return _specular_exponent; }
    float refractive_index() const { return _refractive_index; }
};

#endif // MATERIAL_H

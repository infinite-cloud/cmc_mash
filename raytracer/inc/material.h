#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Material
{
public:
    enum Type
    {
        DIFFUSE,
        SPECULAR,
        REFRACTIVE
    };

private:
    glm::vec3 _diffuse_color;
    glm::vec3 _emission;
    glm::vec4 _albedo;
    float _specular_exponent;
    float _refractive_index;
    Type _type;

public:
    Material() :
        _diffuse_color(0.0f), _emission(0.0f),
        _albedo(1.0f, 0.0f, 0.0f, 0.0f),
        _specular_exponent(0.0f), _refractive_index(1.0f),
        _type(DIFFUSE) {}
    Material(const glm::vec3 &diffuse_color, const glm::vec3 &emission,
            const glm::vec4 &albedo,
            float specular_exponent, float refractive_index,
            Type type) :
        _diffuse_color(diffuse_color), _emission(emission),
        _albedo(albedo),
        _specular_exponent(specular_exponent),
        _refractive_index(refractive_index),
        _type(type) {}

    const glm::vec3 &diffuse_color() const { return _diffuse_color; }
    const glm::vec3 &emission() const { return _emission; }
    const glm::vec4 &albedo() const { return _albedo; }
    float specular_exponent() const { return _specular_exponent; }
    float refractive_index() const { return _refractive_index; }
    Type type() const { return _type; }
};

#endif // MATERIAL_H

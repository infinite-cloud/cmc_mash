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
    glm::dvec3 _diffuse_color;
    glm::dvec3 _emission;
    glm::dvec4 _albedo;
    double _specular_exponent;
    double _refractive_index;
    Type _type;

public:
    Material() :
        _diffuse_color(0.0d), _emission(0.0d),
        _albedo(1.0d, 0.0d, 0.0d, 0.0d),
        _specular_exponent(0.0d), _refractive_index(1.0d),
        _type(DIFFUSE) {}
    Material(const glm::dvec3 &diffuse_color, const glm::dvec3 &emission,
            const glm::dvec4 &albedo,
            double specular_exponent, double refractive_index,
            Type type) :
        _diffuse_color(diffuse_color), _emission(emission),
        _albedo(albedo),
        _specular_exponent(specular_exponent),
        _refractive_index(refractive_index),
        _type(type) {}

    const glm::dvec3 &diffuse_color() const { return _diffuse_color; }
    const glm::dvec3 &emission() const { return _emission; }
    const glm::dvec4 &albedo() const { return _albedo; }
    double specular_exponent() const { return _specular_exponent; }
    double refractive_index() const { return _refractive_index; }
    Type type() const { return _type; }
};

#endif // MATERIAL_H

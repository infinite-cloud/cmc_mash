#ifndef RENDERER_H
#define RENDERER_H

#include <algorithm>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "image.h"
#include "scene.h"
#include "object.h"
#include "ray.h"
#include "options.h"

class Renderer
{
public:
    Image render(const Scene &scene, const Options &options) const;

private:
    glm::dvec3 render_pixel(const Scene &scene, const glm::uvec2 &position,
        const Options &options, const glm::uvec2 &supersample) const;
    glm::dvec3 render_ray(const Scene &scene, const Ray &ray,
        unsigned recursion = 0, unsigned max_recursion = 5) const;
    glm::dvec3 render_path(const Scene &scene, const Ray &ray,
        unsigned recursion = 0, unsigned max_recursion = 5) const;

    glm::dvec3 reflect(const glm::dvec3 &indice, const glm::dvec3 &normal)
        const;
    glm::dvec3 refract(const glm::dvec3 &indice, const glm::dvec3 &normal,
        double refracive_index) const;
};

#endif // RENDERER_H

#ifndef RENDERER_H
#define RENDERER_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "image.h"
#include "scene.h"
#include "object.h"
#include "ray.h"

class Renderer
{
    glm::uvec2 _size;

public:
    Renderer(const glm::uvec2 size) : _size(size) {}

    Image render(const Scene &scene) const;
};

#endif // RENDERER_H
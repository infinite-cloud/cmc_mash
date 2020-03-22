#ifndef OPTIONS_H
#define OPTIONS_H

#include <glm/vec2.hpp>

struct Options
{
    glm::uvec2 size;
    double fov;
    unsigned max_recursion;
    unsigned supersampling_rays;
    unsigned paths_per_pixel;
};

#endif // OPTIONS_H

#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Options
{
    glm::dvec3 camera_origin;
    glm::uvec2 size;
    double fov;
    unsigned max_recursion;
    unsigned supersampling_rays;
    unsigned paths_per_pixel;
    unsigned num_threads;
    unsigned scene_num;
    std::string out_path;
};

#endif // OPTIONS_H

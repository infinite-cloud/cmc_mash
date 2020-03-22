#include <cmath>

#include <glm/vec2.hpp>

#include "image.h"
#include "renderer.h"
#include "scene_loader.h"

int main(int argc, char *argv[])
{
    Options opts;

    opts.camera_origin = glm::dvec3(0, 1, 2);
    opts.size = glm::uvec2(512, 512);
    opts.fov = std::acos(-1.0d) / 2.0d;
    opts.max_recursion = 5;
    opts.supersampling_rays = 1;
    opts.paths_per_pixel = 0;

    Renderer test;
    SceneLoader loader;

    Image img = test.render(*loader.load_scene(3), opts);
    img.save_bmp("test.bmp");

    return 0;
}

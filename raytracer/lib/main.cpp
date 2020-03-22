#include <cmath>

#include <glm/vec2.hpp>

#include "image.h"
#include "renderer.h"
#include "scene_loader.h"

int main(int argc, char *argv[])
{
    Options opts;

    opts.size = glm::uvec2(1024, 768);
    opts.fov = std::acos(-1.0d) / 2.0d;
    opts.max_recursion = 5;
    opts.supersampling_rays = 2;
    opts.paths_per_pixel = 0;

    Renderer test;
    SceneLoader loader;

    Image img = test.render(*loader.load_scene(1), opts);
    img.save_bmp("test.bmp");

    return 0;
}

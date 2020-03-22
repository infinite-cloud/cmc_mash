#include <iostream>
#include <cmath>
#include <string>
#include <unordered_map>
#include <memory>

#include <omp.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "image.h"
#include "renderer.h"
#include "scene_loader.h"

int main(int argc, char *argv[])
{
    std::unordered_map<std::string, std::string> arg_list;

    for(int i = 0; i < argc; ++i)
    {
        std::string key(argv[i]);

        if(key.size() > 0 && key[0] == '-')
        {
            if(i != argc - 1)
            {
                arg_list[key] = argv[++i];
            }
            else
            {
                arg_list[key] = "";
            }
        }
    }

    Options options;

    options.scene_num = (arg_list.find("-scene") != arg_list.end()) ?
        std::atoi(arg_list["-scene"].c_str()) : 1;
    options.num_threads = (arg_list.find("-threads") != arg_list.end()) ?
        std::atoi(arg_list["-threads"].c_str()) : 1;
    options.out_path = (arg_list.find("-out") != arg_list.end()) ?
        arg_list["-out"] : std::string("out_") +
        std::to_string(options.scene_num) + std::string(".bmp");

    options.size = glm::uvec2(512, 512);
    options.fov = std::acos(-1.0d) / 2.0d;
    options.max_recursion = 5;
    options.supersampling_rays = 2;
    options.paths_per_pixel = 0;

    omp_set_num_threads(options.num_threads);

    Renderer renderer;
    SceneLoader loader;

    switch (options.scene_num)
    {
        case 2:
            options.paths_per_pixel = 100;
            options.camera_origin = glm::dvec3(0, 0, 0);
            break;

        case 3:
            options.camera_origin = glm::dvec3(0, 1, 2);
            options.supersampling_rays = 1;
            break;

        default:
            options.camera_origin = glm::dvec3(0, 0, 0);
            break;
    }

    std::unique_ptr<Scene> scene =
        std::move(loader.load_scene(options.scene_num));

    renderer.render(*scene, options).save_bmp(options.out_path);

    return 0;
}

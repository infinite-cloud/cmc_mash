#include <cmath>

#include "image.h"
#include "renderer.h"
#include "scene_loader.h"

int main(int argc, char *argv[])
{
    Renderer test(glm::uvec2(1024, 768),
        std::acos(-1.0d) / 2.0d, 5, true);
    SceneLoader loader;

    Image img = test.render(*loader.load_scene(2));
    img.save_bmp("test.bmp");

    return 0;
}

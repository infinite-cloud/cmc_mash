#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "image.h"
#include "renderer.h"
#include "object.h"
#include "scene.h"
#include "material.h"

int main(int argc, char *argv[])
{
    Renderer test(glm::uvec2(512, 512));
    Scene scene;

    auto mat = std::make_shared<Material>(glm::vec3(0.5f));
    auto mat2 = std::make_shared<Material>(glm::vec3(0.3f));
    
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::vec3(0, 0, -5), 2.0f, mat)));
    scene.objects().push_back(std::unique_ptr<Sphere>(
        new Sphere(glm::vec3(1, 1, -4), 1.0f, mat2)));

    Image img = test.render(scene);
    img.save_bmp("test.bmp");
    
    return 0;
}

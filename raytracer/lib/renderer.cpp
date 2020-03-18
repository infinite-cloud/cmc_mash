#include <cmath>
#include <optional>

#include "renderer.h"
#include "object.h"
#include "ray.h"

const float fov = 3.141592 / 2.0;

Image Renderer::render(const Scene &scene) const
{
    Image img(glm::uvec2(_size.x, _size.y));

    for (size_t y = 0; y < _size.y; ++y)
    {
        for (size_t x = 0; x < _size.x; ++x)
        {
            float x_i = (2.0f * (x + 0.5f) /
                static_cast<float>(_size.x) - 1.0f) *
                std::tan(fov / 2.0f) * _size.x / _size.y;
            float y_i = -(2.0f * (y + 0.5f) /
                static_cast<float>(_size.y) - 1.0f) *
                std::tan(fov / 2.0f);

            glm::vec3 direction = glm::normalize(glm::vec3(x_i, y_i, -1));

            Ray ray(glm::vec3(0.0), direction);
            std::optional<Intersection> intersection =
                scene.find_intersection(ray);

            if (intersection)
            {
                img.set_pixel(glm::uvec2(x, y),
                    intersection->material()->diffuse_color());
            }
            else
            {
                img.set_pixel(glm::uvec2(x, y), glm::vec3(0.2, 0.7, 0.8));
            }                
        }
    }

    return img;
}

#include <cmath>
#include <optional>
#include <algorithm>

#include <glm/gtx/norm.hpp>

#include "renderer.h"
#include "object.h"
#include "ray.h"

static glm::vec3 reflect(const glm::vec3 &indice, const glm::vec3 &normal)
{
    return indice - 2.0f * normal * glm::dot(indice, normal); 
}

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
            float diffuse_light_intensity = 0;
            float specular_light_intensity = 0;

            Ray ray(glm::vec3(0.0), direction);
            std::optional<Intersection> intersection =
                scene.find_intersection(ray);

            if (!intersection)
            {
                img.set_pixel(glm::uvec2(x, y), glm::vec3(0.2, 0.7, 0.8));

                continue;
            }

            for (const auto &o : scene.point_lights())
            {
                float light_distance = glm::l2Norm(
                    o->position() - intersection->point());
                glm::vec3 light_direction = glm::normalize(
                    o->position() - intersection->point());
                
                glm::vec3 shadow_origin = intersection->point() +
                    ((glm::dot(light_direction, intersection->normal()) < 0) ?
                        -intersection->normal() :
                        intersection->normal()) * 1e-3f;
                
                std::optional<Intersection> inter =
                    scene.find_intersection(
                    Ray(shadow_origin, light_direction));

                if (inter && glm::l2Norm(
                    inter->point() - shadow_origin) < light_distance)
                {
                    continue;
                }

                diffuse_light_intensity += o->intensity() *
                    std::max(0.0f, glm::dot(light_direction,
                    intersection->normal()));
                specular_light_intensity += std::pow(
                    std::max(0.0f, glm::dot(reflect(
                    light_direction, intersection->normal()), direction)),
                    intersection->material()->specular_exponent()) *
                    o->intensity();
            }

            glm::vec3 color = intersection->material()->diffuse_color() *
                diffuse_light_intensity *
                intersection->material()->albedo().x +
                glm::vec3(1) * specular_light_intensity *
                intersection->material()->albedo().y;

            img.set_pixel(glm::uvec2(x, y), color);
        }
    }

    return img;
}

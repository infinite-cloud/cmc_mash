#include <cmath>
#include <optional>
#include <algorithm>

#include <glm/gtx/norm.hpp>

#include "renderer.h"
#include "object.h"
#include "ray.h"

Image Renderer::render(const Scene &scene) const
{
    return render_frag(scene, glm::uvec2(0, 0), _size);
}

Image Renderer::render_frag(const Scene &scene, const glm::uvec2 &start,
        const glm::uvec2 &size) const
{
    Image img(size);

    for (size_t y = 0; y < _size.y; ++y)
    {
        for (size_t x = 0; x < _size.x; ++x)
        {
            glm::vec3 color = render_pixel(scene, glm::uvec2(x, y));
            img.set_pixel(glm::uvec2(x, y), color);
        }
    }

    return img;
}

glm::vec3 Renderer::render_pixel(const Scene &scene,
        const glm::uvec2 &position) const
{
    float x_i = (2.0f * (position.x + 0.5f) /
        static_cast<float>(_size.x) - 1.0f) *
        std::tan(_fov / 2.0f) * _size.x / _size.y;
    float y_i = -(2.0f * (position.y + 0.5f) /
        static_cast<float>(_size.y) - 1.0f) *
        std::tan(_fov / 2.0f);

    glm::vec3 direction = glm::normalize(glm::vec3(x_i, y_i, -1));

    Ray ray(glm::vec3(0.0), direction);

    return render_ray(scene, ray);
}

glm::vec3 Renderer::render_ray(const Scene &scene, const Ray &ray,
        unsigned recursion) const
{
    std::optional<Intersection> i = scene.find_intersection(ray);

    if (recursion > _max_recursion || !i)
    {
        return glm::vec3(0.2, 0.7, 0.8);
    }

    glm::vec3 reflection_direction = reflect(ray.direction(), i->normal());
    glm::vec3 reflection_origin = i->point() +
        ((glm::dot(reflection_direction, i->normal()) < 0) ?
            -i->normal() : i->normal()) * 1e-3f;
    Ray reflection = Ray(reflection_origin, reflection_direction);
    glm::vec3 reflection_color = render_ray(scene, reflection, recursion + 1);

    glm::vec3 refraction_direction = refract(ray.direction(), i->normal(),
        i->material()->refractive_index());
    glm::vec3 refraction_origin = i->point() +
        ((glm::dot(refraction_direction, i->normal()) < 0) ?
            -i->normal() : i->normal()) * 1e-3f;
    Ray refraction = Ray(refraction_origin, refraction_direction);
    glm::vec3 refraction_color = render_ray(scene, refraction, recursion + 1);

    float diffuse_light_intensity = 0;
    float specular_light_intensity = 0;

    for (const auto &o : scene.point_lights())
    {
        float light_distance = glm::l2Norm(
            o->position() - i->point());
        glm::vec3 light_direction = glm::normalize(
            o->position() - i->point());

        glm::vec3 shadow_origin = i->point() +
            ((glm::dot(light_direction, i->normal()) < 0) ?
                -i->normal() : i->normal()) * 1e-3f;

        std::optional<Intersection> intersection =
            scene.find_intersection(Ray(shadow_origin, light_direction));

        if (intersection && glm::l2Norm(
            intersection->point() - shadow_origin) < light_distance)
        {
            continue;
        }

        diffuse_light_intensity += o->intensity() *
            std::max(0.0f, glm::dot(light_direction,
            i->normal()));
        specular_light_intensity += std::pow(
            std::max(0.0f, glm::dot(reflect(
            light_direction, i->normal()), ray.direction())),
            i->material()->specular_exponent()) * o->intensity();
    }

    glm::vec3 color = i->material()->diffuse_color() *
        diffuse_light_intensity * i->material()->albedo().x +
        glm::vec3(1) * specular_light_intensity *
        i->material()->albedo().y +
        reflection_color * i->material()->albedo().z +
        refraction_color * i->material()->albedo().w;

    return color;
}

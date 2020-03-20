#include <cmath>
#include <optional>
#include <algorithm>
#include <random>

#include <glm/gtx/norm.hpp>

#include "renderer.h"
#include "object.h"
#include "ray.h"

std::default_random_engine generator;
std::uniform_real_distribution<float> distr(0.0, 1.0);

float erand48(unsigned short xi[3])
{
    return distr(generator);
}

Image Renderer::render(const Scene &scene) const
{
    return render_frag(scene, glm::uvec2(0, 0), _size);
}

Image Renderer::render_frag(const Scene &scene, const glm::uvec2 &start,
        const glm::uvec2 &size) const
{
    Image img(size);

    #pragma omp parallel for schedule(dynamic, 1)
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

glm::vec3 Renderer::render_path(const Scene &scene, const Ray &ray,
    unsigned recursion, unsigned short xi[3]) const
{
    std::optional<Intersection> i = scene.find_intersection(ray);

    if (!i)
    {
        return glm::vec3(0.2, 0.7, 0.8);
    }

    glm::vec3 color = i->material()->diffuse_color();
    float p = std::max(color.x, std::max(color.y, color.z));

    if (recursion >= _max_recursion)
    {
        if (erand48(xi) < p)
        {
            color /= p;
        }
        else
        {
            return i->material()->emission();
        }
    }
    
    glm::vec3 n = (glm::dot(ray.direction(), i->normal()) < 0) ?
        i->normal() : -i->normal();
    Ray reflected = Ray(i->point(), reflect(ray.direction(), i->normal()));

    if (i->material()->type() == Material::DIFFUSE)
    {
        float r_1 = 2 * std::acos(-1) * erand48(xi);
        float r_2 = erand48(xi);
        float r_2s = std::sqrt(r_2);
        
        glm::vec3 u = glm::normalize
            ((std::abs(n.x) > 0.1) ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0));
        glm::vec3 v = glm::cross(n, u);
        glm::vec3 d = glm::normalize(
            (u * std::cos(r_1) * r_2s + v * std::sin(r_1) * r_2s
            + n * std::sqrt(1 - r_2)));
            
        return i->material()->emission() + color *
            render_path(scene, Ray(i->point(), d), recursion + 1, xi);
    }
    else if (i->material()->type() == Material::SPECULAR)
    {
        return i->material()->emission() + color *
            render_path(scene, reflected, recursion + 1, xi);
    }

    bool outside = glm::dot(n, i->normal()) > 0;
    float nc = 1;
    float nt = 1.5;
    float nnt = (outside) ? nc / nt : nt / nc;
    float ddn = glm::dot(ray.direction(), i->normal());
    float cos2t = 1 - nnt * nnt * (1 - ddn * ddn);
    
    if (cos2t < 0)
    {
        return i->material()->emission() + color *
            render_path(scene, reflected, recursion + 1, xi);
    }

    glm::vec3 t_dir = glm::normalize(ray.direction() * nnt - i->normal() *
        ((outside) ? 1.0f : -1.0f) * (ddn * nnt * std::sqrt(cos2t)));
    
    float a = nt - nc;
    float b = nt + nc;
    float r_0 = a * a / (b * b);
    float c = 1 - ((outside) ? -ddn : glm::dot(t_dir, i->normal()));
    float r_e = r_0 + (1 - r_0) * std::pow(c, 5);
    float t_r = 1 - r_e;
    float p_i = 0.25 + 0.5 * r_e;
    float r_p = r_e / p_i;
    float t_p = t_r / (1 - p_i);
    
    return i->material()->emission() + color *
        ((recursion > 1) ? ((erand48(xi) < p_i) ?
        render_path(scene, reflected, recursion + 1, xi) * r_p :
        render_path(scene, reflected, recursion + 1, xi) * t_p) :
        render_path(scene, reflected, recursion + 1, xi) * r_e +
        render_path(scene, Ray(i->point(), t_dir), recursion + 1, xi) * t_r);
        
}

glm::vec3 Renderer::reflect(const glm::vec3 &indice, const glm::vec3 &normal)
    const
{
    return indice - 2.0f * normal * glm::dot(indice, normal);
}

glm::vec3 Renderer::refract(const glm::vec3 &indice, const glm::vec3 &normal,
    float refracive_index) const
{
    float cos_i = -std::max(-1.0f, std::min(1.0f, glm::dot(indice, normal)));
    float eta_i = 1;
    float eta_t = refracive_index;

    glm::vec3 n = normal;

    if (cos_i < 0)
    {
        cos_i = -cos_i;
        std::swap(eta_i, eta_t);
        n = -n;
    }

    float eta = eta_i / eta_t;
    float k = 1 - eta * eta * (1 - cos_i * cos_i);

    return (k < 0) ? glm::vec3(0) :
        indice * eta + n * (eta * cos_i - std::sqrt(k));
}

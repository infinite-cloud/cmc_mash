#include <cmath>
#include <optional>
#include <algorithm>
#include <random>

#include <glm/gtx/norm.hpp>
#include <glm/common.hpp>

#include "renderer.h"
#include "object.h"
#include "ray.h"

static std::default_random_engine generator;
static std::uniform_real_distribution<double> distr(0.0, 1.0);

static double erand48()
{
    return distr(generator);
}

Image Renderer::render(const Scene &scene, const Options &options) const
{
    Image img(options.size);
    glm::dvec3 color;
    double passes = options.supersampling_rays *
        options.supersampling_rays;

    auto f = [](double x) { return static_cast<int>((std::pow(
        glm::clamp(x, 0.0d, 1.0d), 1.0d / 2.2d) * 255.0d + 0.5d)); };

    #pragma omp parallel for schedule(dynamic, 1) private(color)
    for (size_t y = 0; y < options.size.y; ++y)
    {
        for (size_t x = 0; x < options.size.x; ++x)
        {
            color = glm::dvec3(0);

            for (size_t s_y = 0; s_y < options.supersampling_rays; ++s_y)
            {
                for (size_t s_x = 0; s_x < options.supersampling_rays; ++s_x)
                {
                    glm::dvec3 r = render_pixel(scene, glm::uvec2(x, y),
                        options, glm::uvec2(s_x, s_y));
                    color += glm::dvec3(glm::clamp(r.x, 0.0d, 1.0d),
                        glm::clamp(r.y, 0.0d, 1.0d),
                        glm::clamp(r.z, 0.0d, 1.0d)) / passes;
                }
            }

            if (options.paths_per_pixel > 0)
            {
                color = glm::dvec3(f(color.x), f(color.y), f(color.z)) /
                    255.0d;
            }

            img.set_pixel(glm::uvec2(x, y), color);
        }
    }

    return img;
}

glm::dvec3 Renderer::render_pixel(const Scene &scene,
        const glm::uvec2 &position, const Options &options,
        const glm::uvec2 &supersample) const
{
    double fov_scale = 2.0d * std::tan(0.5d * options.fov);

    glm::dvec3 c_x = glm::dvec3(options.size.x * fov_scale /
        options.size.y, 0, 0);
    glm::dvec3 c_y = -glm::normalize(glm::cross(c_x,
        glm::dvec3(0, 0, -1))) * fov_scale;

    double x_i = (2.0d * (position.x +
        static_cast<double>(supersample.x + 1) /
        options.supersampling_rays + 0.5d) /
        static_cast<double>(options.size.x) - 1.0d) *
        std::tan(options.fov * 0.5d) *
        options.size.x / options.size.y;
    double y_i = -(2.0d * (position.y +
        static_cast<double>(supersample.y + 1) /
        options.supersampling_rays + 0.5d) /
        static_cast<double>(options.size.y) - 1.0d) *
        std::tan(options.fov * 0.5d);

    if (options.paths_per_pixel == 0)
    {
        glm::dvec3 direction = glm::normalize(glm::dvec3(x_i, y_i, -1));
        Ray ray(options.camera_origin, direction);

        return render_ray(scene, ray);
    }
    else
    {
        size_t samples = options.paths_per_pixel /
            (options.supersampling_rays * options.supersampling_rays);
        glm::dvec3 r = glm::dvec3(0);

        for (size_t s = 0; s < samples; ++s)
        {
            double r_1 = 2 * erand48();
            double d_x = (r_1 < 1) ? std::sqrt(r_1) - 1 :
                1 - std::sqrt(2 - r_1);
            double r_2 = 2 * erand48();
            double d_y = (r_2 < 1) ? std::sqrt(r_2) - 1 :
                1 - std::sqrt(2 - r_2);

            glm::dvec3 d = c_x *
                (((supersample.x + 0.5d + d_x) / 2 + position.x) /
                options.size.x - 0.5d) +
                c_y * (((supersample.y + 0.5d + d_y) / 2 + position.y) /
                options.size.y - 0.5d) + glm::dvec3(0, 0, -1);

            r += render_path(scene,
                Ray(d, glm::normalize(d))) /
                static_cast<double>(options.paths_per_pixel);
        }

        return r;
    }
}

glm::dvec3 Renderer::render_ray(const Scene &scene, const Ray &ray,
        unsigned recursion, unsigned max_recursion) const
{
    std::optional<Intersection> i = scene.find_intersection(ray);

    if (recursion > max_recursion || !i)
    {
        return glm::dvec3(0.2, 0.7, 0.8);
    }

    glm::dvec3 reflection_direction = reflect(ray.direction(), i->normal());
    glm::dvec3 reflection_origin = i->point() +
        ((glm::dot(reflection_direction, i->normal()) < 0) ?
            -i->normal() : i->normal()) * 1e-3d;
    Ray reflection = Ray(reflection_origin, reflection_direction);
    glm::dvec3 reflection_color = render_ray(scene, reflection, recursion + 1);

    glm::dvec3 refraction_direction = refract(ray.direction(), i->normal(),
        i->material()->refractive_index());
    glm::dvec3 refraction_origin = i->point() +
        ((glm::dot(refraction_direction, i->normal()) < 0) ?
            -i->normal() : i->normal()) * 1e-3d;
    Ray refraction = Ray(refraction_origin, refraction_direction);
    glm::dvec3 refraction_color = render_ray(scene, refraction, recursion + 1);

    double diffuse_light_intensity = 0;
    double specular_light_intensity = 0;

    for (const auto &o : scene.point_lights())
    {
        double light_distance = glm::l2Norm(
            o->position() - i->point());
        glm::dvec3 light_direction = glm::normalize(
            o->position() - i->point());

        glm::dvec3 shadow_origin = i->point() +
            ((glm::dot(light_direction, i->normal()) < 0) ?
                -i->normal() : i->normal()) * 1e-3d;

        std::optional<Intersection> intersection =
            scene.find_intersection(Ray(shadow_origin, light_direction));

        if (intersection && glm::l2Norm(
            intersection->point() - shadow_origin) < light_distance)
        {
            continue;
        }

        diffuse_light_intensity += o->intensity() *
            std::max(0.0d, glm::dot(light_direction,
            i->normal()));
        specular_light_intensity += std::pow(
            std::max(0.0d, glm::dot(reflect(
            light_direction, i->normal()), ray.direction())),
            i->material()->specular_exponent()) * o->intensity();
    }

    glm::dvec3 color = i->material()->diffuse_color() *
        diffuse_light_intensity * i->material()->albedo().x +
        glm::dvec3(1) * specular_light_intensity *
        i->material()->albedo().y +
        reflection_color * i->material()->albedo().z +
        refraction_color * i->material()->albedo().w;

    return color;
}

glm::dvec3 Renderer::render_path(const Scene &scene, const Ray &ray,
    unsigned recursion, unsigned max_recursion) const
{
    std::optional<Intersection> i = scene.find_intersection(ray);

    if (!i)
    {
        return glm::dvec3(0.2, 0.7, 0.8);
    }

    glm::dvec3 color = i->material()->diffuse_color();
    double p = std::max(color.x, std::max(color.y, color.z));

    if (recursion > max_recursion)
    {
        if (erand48() < p)
        {
            color /= p;
        }
        else
        {
            return i->material()->emission();
        }
    }

    glm::dvec3 n = (glm::dot(ray.direction(), i->normal()) < 0) ?
        i->normal() : -i->normal();
    Ray reflected = Ray(i->point(), reflect(ray.direction(), i->normal()));

    if (i->material()->type() == Material::DIFFUSE)
    {
        double r_1 = 2 * std::acos(-1) * erand48();
        double r_2 = erand48();
        double r_2_s = std::sqrt(r_2);

        glm::dvec3 u = glm::normalize(glm::cross(
            ((std::abs(n.x) > 0.1d) ?
            glm::dvec3(0, 1, 0) : glm::dvec3(1, 0, 0)), n));
        glm::dvec3 v = glm::cross(n, u);
        glm::dvec3 d = glm::normalize(
            (u * std::cos(r_1) * r_2_s + v * std::sin(r_1) * r_2_s +
            n * std::sqrt(1 - r_2)));

        return i->material()->emission() + color *
            render_path(scene, Ray(i->point(), d), recursion + 1);
    }
    else if (i->material()->type() == Material::SPECULAR)
    {
        return i->material()->emission() + color *
            render_path(scene, reflected, recursion + 1);
    }

    bool outside = glm::dot(n, i->normal()) > 0;
    double nc = 1;
    double nt = 1.5;
    double nnt = (outside) ? nc / nt : nt / nc;
    double ddn = glm::dot(ray.direction(), n);
    double cos2t = 1 - nnt * nnt * (1 - ddn * ddn);
    
    if (cos2t < 0)
    {
        return i->material()->emission() + color *
            render_path(scene, reflected, recursion + 1);
    }

    glm::dvec3 t_dir = glm::normalize(ray.direction() * nnt - i->normal() *
        ((outside) ? 1.0d : -1.0d) * (ddn * nnt + std::sqrt(cos2t)));
    
    double a = nt - nc;
    double b = nt + nc;
    double r_0 = a * a / (b * b);
    double c = 1 - ((outside) ? -ddn : glm::dot(t_dir, i->normal()));
    double r_e = r_0 + (1 - r_0) * std::pow(c, 5);
    double t_r = 1 - r_e;
    double p_i = 0.25 + 0.5 * r_e;
    double r_p = r_e / p_i;
    double t_p = t_r / (1 - p_i);
    
    return i->material()->emission() + color *
        ((recursion > 1) ? ((erand48() < p_i) ?
        render_path(scene, reflected, recursion + 1) * r_p :
        render_path(scene, reflected, recursion + 1) * t_p) :
        render_path(scene, reflected, recursion + 1) * r_e +
        render_path(scene, Ray(i->point(), t_dir), recursion + 1) * t_r);
}

glm::dvec3 Renderer::reflect(const glm::dvec3 &indice, const glm::dvec3 &normal)
    const
{
    return indice - 2.0d * normal * glm::dot(indice, normal);
}

glm::dvec3 Renderer::refract(const glm::dvec3 &indice, const glm::dvec3 &normal,
    double refracive_index) const
{
    double cos_i = -std::max(-1.0d, std::min(1.0d, glm::dot(indice, normal)));
    double eta_i = 1;
    double eta_t = refracive_index;

    glm::dvec3 n = normal;

    if (cos_i < 0)
    {
        cos_i = -cos_i;
        std::swap(eta_i, eta_t);
        n = -n;
    }

    double eta = eta_i / eta_t;
    double k = 1 - eta * eta * (1 - cos_i * cos_i);

    return (k < 0) ? glm::dvec3(0) :
        indice * eta + n * (eta * cos_i - std::sqrt(k));
}

#ifndef IMAGE_H
#define IMAGE_H

#include <memory>
#include <cmath>
#include <string>

#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Image
{
    struct Pixel
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };

    glm::uvec2 _size;
    std::unique_ptr<Pixel[]> _data;

public:
    Image() : _size(glm::uvec2(0)), _data(nullptr) {}
    Image(const glm::uvec2 size) :
        _size(size), _data(std::make_unique<Pixel[]>(size.x * size.y)) {}

    const glm::uvec2 size() const { return _size; }

    const Image &fill(const glm::vec3 &value)
    {
        std::fill
        (
            &_data[0],
            &_data[_size.x * _size.y],
            Pixel
            {
                static_cast<unsigned char>(std::round(
                    glm::clamp(value.r, 0.0f, 1.0f) * 255.0f)),
                static_cast<unsigned char>(std::round(
                    glm::clamp(value.g, 0.0f, 1.0f) * 255.0f)),
                static_cast<unsigned char>(std::round(
                    glm::clamp(value.b, 0.0f, 1.0f) * 255.0f)),
            }
        );

        return *this;
    }

    const Image &set_pixel(const glm::uvec2 &pos, const glm::vec3 &value)
    {
        _data[static_cast<size_t>(pos.x + pos.y * _size.x)] = Pixel
        { 
            static_cast<unsigned char>(std::round(
                glm::clamp(value.r, 0.0f, 1.0f) * 255.0f)),
            static_cast<unsigned char>(std::round(
                glm::clamp(value.g, 0.0f, 1.0f) * 255.0f)),
            static_cast<unsigned char>(std::round(
                glm::clamp(value.b, 0.0f, 1.0f) * 255.0f)),
        };

        return *this;
    }

    glm::vec3 get_pixel(const glm::uvec2 &pos) const
    {
        Pixel p = _data[static_cast<size_t>(pos.x + pos.y * _size.x)];

        return glm::vec3(p.r / 255.0f, p.g / 255.0f, p.b / 255.0f);
    }

    const Image &copy_data(const Image &img, const glm::uvec2 &pos)
    {
        for (size_t y = 0; y < img._size.y; ++y)
        {
            std::copy
            (
                &img._data[y * img._size.x],
                &img._data[(y + 1) * img._size.x],
                &_data[pos.x + (y + pos.y) * _size.x]
            );
        }

        return *this;
    }

    int save_bmp(const std::string &fname) const;
};

#endif // IMAGE_H

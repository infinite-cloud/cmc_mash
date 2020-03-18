#include <fstream>
#include <memory>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include "image.h"

int Image::save_bmp(const std::string &fname) const
{
    auto raw = std::make_unique<uint8_t []>(_size.x * _size.y * 3);
    size_t index = 0;

    for (size_t y = 0; y < _size.y; ++y)
    {
        for (size_t x = 0; x < _size.x; ++x)
        {
            const auto &p = _data[x + y * _size.x];
            raw[index++] = p.r;
            raw[index++] = p.g;
            raw[index++] = p.b;
        }
    }

    return stbi_write_bmp(fname.c_str(), _size.x, _size.y, 3,
        reinterpret_cast<const void *>(raw.get()));
}

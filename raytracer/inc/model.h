#ifndef MODEL_H
#define MODEL_H

#include <string>

#include <glm/vec3.hpp>

#include "object.h"
#include "material.h"

class Model
{
    std::unique_ptr<Mesh> _mesh;

public:    
    bool load(const std::string &f_name, const Material *material);
    
    std::unique_ptr<Mesh> &mesh() { return _mesh; }
};

#endif // MODEL_H

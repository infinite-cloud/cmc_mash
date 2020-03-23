#include <fstream>
#include <sstream>

#include <glm/vec3.hpp>

#include "model.h"

bool Model::load(const std::string &f_name, const Material *material)
{
    std::ifstream in;

    in.open(f_name, std::ifstream::in);

    if (in.fail())
    {
        return false;
    }

    std::vector<Triangle> triangles;
    std::vector<glm::dvec3> p_vertices;
    std::vector<glm::dvec3> n_vertices;

    while (!in.eof())
    {
        std::string line;
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;

        if (!line.compare(0, 2, "v "))
        {
            iss >> trash;

            glm::dvec3 p;

            iss >> p.x;
            iss >> p.y;
            iss >> p.z;

            p_vertices.push_back(p);
        }
        else if (!line.compare(0, 2, "f "))
        {
            iss >> trash;

            Triangle t;

            iss >> t.a;
            --t.a;
            iss >> t.b;
            --t.b;
            iss >> t.c;
            --t.c;

            triangles.push_back(t);
        }
        else if (!line.compare(0, 3, "vn "))
        {
            iss >> trash;

            glm::dvec3 n;

            iss >> n.x;
            iss >> n.y;
            iss >> n.z;

            n_vertices.push_back(n);
        }
    }

    if (n_vertices.size() != p_vertices.size())
    {
        return false;
    }

    std::vector<Vertex> vertices;

    for (unsigned i = 0; i < n_vertices.size(); ++i)
    {
        vertices.push_back(
            Vertex
            {
                .position = p_vertices[i],
                .normal = n_vertices[i]
            }
        );
    }

    _mesh = std::unique_ptr<Mesh>(new Mesh(vertices, triangles, material));

    return true;
}

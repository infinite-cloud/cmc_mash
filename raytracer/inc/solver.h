#ifndef SOLVER_H
#define SOLVER_H

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class Solver
{
    static constexpr float eps = 1e-5;

public:
    static glm::vec2 solve_quadratic(float a, float b, float c);
    static float solve_cubic_normalized(float p, float q, float r);
    static glm::vec4 solve_quartic(float a, float b, float c,
        float d, float e);
};

#endif // SOLVER_H

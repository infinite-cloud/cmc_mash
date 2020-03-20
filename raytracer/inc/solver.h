#ifndef SOLVER_H
#define SOLVER_H

#include <vector>

class Solver
{
public:
    static constexpr float eps = 1e-5;
    static std::vector<float> solve_quadric(float a, float b, float c);
    static float solve_cubic_normalized(float p, float q, float r);
    static std::vector<float> solve_quartic(float a, float b, float c,
        float d, float e);
};

#endif // SOLVER_H

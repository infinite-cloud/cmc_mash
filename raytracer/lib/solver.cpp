#include <cmath>
#include <algorithm>
#include <limits>

#include "solver.h"

glm::vec2 Solver::solve_quadratic(float a, float b, float c)
{
    float d = b * b - 4.0f * a * c;

    if (d < 0.0f)
    {
        return glm::vec2(std::numeric_limits<float>::quiet_NaN());
    }

    d = std::sqrt(d);

    float q = -0.5f * ((b < 0.0f) ? (b - d) : (b + d));
    float t_0 = q / a;
    float t_1 = c / q;

    if (t_0 > t_1)
    {
        std::swap(t_0, t_1);
    }

    return glm::vec2(t_0, t_1);
}

float Solver::solve_cubic_normalized(float p, float q, float r)
{
    float a_2 = p * p;
    float q_q = (a_2 - 3.0f * q) / 9.0f;
    float r_r = (p * (a_2 - 4.5f * q) + 13.5f * r) / 27.0f;
    float q_3 = q * q * q;
    float r_r_2 = r_r * r_r;
    float d = q_3 - r_r_2;
    float a = p / 3.0f;

    if (d >= 0.0f)
    {
        float theta = std::acos(r_r / std::sqrt(q_3)) / 3.0f;
        float s_q = -2.0f * std::sqrt(q_q);

        return s_q * std::cos(theta) - a;
    }
    else
    {
        float s_q = std::pow(
            std::sqrt(r_r_2 - q_3) + std::abs(r_r), 1.0f / 3.0f);

        return ((r_r < 0.0f) ? 1.0f : -1.0f) * s_q + q / s_q - a;
    }
}

glm::vec4 Solver::solve_quartic(float a, float b, float c,
    float d, float e)
{
    float inv_a = 1.0f / a;
    float c_1 = b * inv_a;
    float c_2 = c * inv_a;
    float c_3 = d * inv_a;
    float c_4 = e * inv_a;
    float c_1_2 = c_1 * c_1;
    float p = -0.375f * c_1_2 * c_2;
    float q = 0.125 * c_1_2 * c_1 - 0.5f * c_1 * c_2 + c_3;
    float r = -0.001171875f * c_1_2 * c_1_2 + 0.0625f * c_1_2 * c_2 -
        0.25f * c_1 * c_3 + c_4;
    float z = solve_cubic_normalized(-0.5f * p, -r,
        0.5f * r * p - 0.125f * q * q);

    float d_1 = 2.0f * z - p;

    if (d_1 < 0.0f)
    {
        if (d_1 > -eps)
        {
            d_1 = 0.0f;
        }
        else
        {
            return glm::vec4(std::numeric_limits<float>::quiet_NaN());
        }
    }

    float d_2;

    if (d_1 < eps)
    {
        if ((d_2 = z * z - r) < 0.0f)
        {
            return glm::vec4(std::numeric_limits<float>::quiet_NaN());
        }

        d_2 = std::sqrt(d_2);
    }
    else
    {
        d_1 = std::sqrt(d_1);
        d_2 = 0.5f * q / d_1;
    }

    float q_1 = d_1 * d_1;
    float q_2 = -0.25f * c_1;
    float p_m = q_1 - 4.0f * (z - d_2);
    float p_p = q_1 - 4.0f * (z + d_2);
    
    glm::vec4 result;
    
    if (p_m >= 0.0f && p_p >= 0.0f)
    {
        p_m = std::sqrt(p_m);
        p_p = std::sqrt(p_p);

        result[0] = -0.5f * (d_1 + p_m) + q_2;
        result[1] = -0.5f * (d_1 - p_m) + q_2;
        result[2] = 0.5f * (d_1 + p_m) + q_2;
        result[3] = 0.5f * (d_1 - p_m) + q_2;
    }
    else if (p_m >= 0.0f)
    {
        p_m = std::sqrt(p_m);

        result[0] = -0.5f * (d_1 + p_m) + q_2;
        result[1] = result[0];
        result[2] = -0.5f * (d_1 - p_m) + q_2;
        result[3] = result[2];
    }
    else if (p_p >= 0.0f)
    {
        p_p = std::sqrt(p_p);

        result[0] = 0.5f * (d_1 - p_p) + q_2;
        result[1] = result[0];
        result[2] = 0.5f * (d_1 + p_p) + q_2;
        result[3] = result[2];
    }
    else
    {
        return glm::vec4(std::numeric_limits<float>::quiet_NaN());
    }

    for (unsigned char i = 1; i < 4; ++i)
    {
        for (unsigned char j = i; j > 0 && result[j - 1] > result[j]; ++j)
        {
            std::swap(result[j], result[i]);
        }
    }

    return result;
}

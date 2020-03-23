#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <iostream>

class Timer
{
    using clock_t = std::chrono::high_resolution_clock;
    using microseconds = std::chrono::microseconds;

    const clock_t::time_point _start;

public:
    Timer() : _start(clock_t::now()) {}

    ~Timer()
    {
        const auto finish = clock_t::now();
        auto us = std::chrono::duration_cast<microseconds> 
            (finish - _start).count();
        long multiplier = 1e6 * 60 * 60;

        if (us >= multiplier)
        {
            std::cout << us / multiplier << " hour(s) ";
            us %= multiplier;
        }

        multiplier /= 60;

        if (us >= multiplier)
        {
            std::cout << us / multiplier << " minute(s) ";
            us %= multiplier;
        }

        multiplier /= 60;

        std::cout << us / multiplier << ".";
        std::cout << us % multiplier << " second(s)";
    }
};

#endif // TIMER_H

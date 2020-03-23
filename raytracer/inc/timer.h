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

        if (us >= static_cast<int>(10e5 * 60 * 60))
        {
            std::cout << us / static_cast<int>(10e5 * 60 * 60) << " hour(s) ";
            us %= static_cast<int>(10e5 * 60 * 60);
        }

        if (us >= static_cast<int>(10e5 * 60))
        {
            std::cout << us / static_cast<int>(10e5 * 60) << " minute(s) ";
            us %= static_cast<int>(10e5 * 60);
        }

        if (us >= static_cast<int>(10e5))
        {
            std::cout << us / static_cast<int>(10e5) << " second(s) ";
            us %= static_cast<int>(10e5);
        }

		std::cout << us << " microsecond(s)";
	}
};

#endif // TIMER_H

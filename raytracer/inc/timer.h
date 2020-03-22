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
        const auto us = std::chrono::duration_cast<microseconds> 
            (finish - _start).count();

		std::cout << us << " microseconds";
	}
};

#endif // TIMER_H

#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>

class Clock {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> end;

public:
    Clock();

    void restart();
    int sleep_and_calc_next_it(const int FPS, int it);

    Clock(const Clock&) = delete;
    Clock& operator=(const Clock&) = delete;

    ~Clock();
};

#endif

#include "clock.h"
#include <cmath>
#include <thread>

Clock::Clock()
    : start(std::chrono::high_resolution_clock::now()),
      end(std::chrono::high_resolution_clock::now()) {}

void Clock::restart() {
    start = std::chrono::high_resolution_clock::now();
}

int Clock::sleep_and_calc_next_it(const int FPS, int it) {
    std::chrono::duration<double> rate(1.0 / FPS);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> x = end - start;
    auto rest = rate - x;
    if (rest.count() < 0){
        auto behind = -rest;
        rest = rate - std::chrono::duration<double>(std::fmod(behind.count(), rate.count()));
        auto lost = behind + rest;
        start = start + std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(lost);
        it += int(lost / rate); 
    }
    std::this_thread::sleep_for(rest);
    start = start + std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(rate);
    return it += 1;
}

Clock::~Clock() {}

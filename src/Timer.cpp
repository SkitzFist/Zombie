#include "Timer.hpp"

Timer::Timer():
    m_startTime(std::chrono::high_resolution_clock::now()){
}

void Timer::start(){
    m_startTime = std::chrono::high_resolution_clock::now();
}

float Timer::getElapsed() const{
    std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - m_startTime;
    return duration.count();
}

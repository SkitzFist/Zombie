#ifndef _Timer
#define _Timer

#include <chrono>

class Timer{
public:
    Timer();

    /** * \brief Sets start time to now */
    void start();
    float getElapsed() const;
private:
    std::chrono::high_resolution_clock::time_point m_startTime;
};

#endif
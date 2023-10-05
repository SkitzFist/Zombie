#ifndef _Log
#define _Log

#include <string>
#include "raylib.h"

namespace Log{
    inline void info(const std::string& log){
        TraceLog(LOG_INFO, log.c_str());
    }
}

#endif
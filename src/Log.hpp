#ifndef _Log
#define _Log

#include <string>
#include "raylib.h"


inline void logInfo(const std::string& log){
    TraceLog(LOG_INFO, log.c_str());
}

#endif
#pragma once

#include <stdio.h>

namespace NetBrain {

#define LOG_ERROR printf
#define LOG_WARN printf
#define LOG_INFO printf

#ifdef DEBUG
#define LOG_DEBUG printf
#else
#define LOG_DEBUG 
#endif // DEBUG
}
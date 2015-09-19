#pragma once
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

class screen_windows : screen {
    virtual Mat* capture() = 0;
};

#endif

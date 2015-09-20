#pragma once
#ifdef _WIN32
#include <opencv2/opencv.hpp>
#include "screen.hpp"

class screen_windows : screen {
public:
    virtual Mat* capture();
};

#endif

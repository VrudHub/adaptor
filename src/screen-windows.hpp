#pragma once
#ifdef _WIN32

class screen_windows : screen {
    virtual Mat* capture() = 0;
};

#endif

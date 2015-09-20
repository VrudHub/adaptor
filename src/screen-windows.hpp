#pragma once
#ifdef _WIN32

class screen_windows : screen {
public:
    virtual Mat* capture();
};

#endif

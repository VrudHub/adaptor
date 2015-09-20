#pragma once
#include <opencv2/opencv.hpp>
using namespace cv;

class screen {
public:
    virtual Mat* capture() = 0;
};

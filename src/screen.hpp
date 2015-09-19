#pragma once
#include <opencv2/opencv.hpp>
using namespace cv;

class screen {
    virtual Mat* capture() = 0;
};

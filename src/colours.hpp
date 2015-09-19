#pragma once
#include <opencv2/opencv.hpp>
using namespace cv;

class colours {
public:
    void calculate_colours(Mat* m);
    void apply_colours(Mat* m);

private:
    int w;
    int h;
    int ch;
    float* correction;
};

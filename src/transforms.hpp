#pragma once
#include <opencv2/opencv.hpp>
using namespace cv;

class transforms {
public:
    bool calculate_transforms(Mat* dark, Mat* light);
    void apply_transforms(Mat* m);

private:
    vector<Point2f> corners;

    void get_corners(vector<Vec4i> lines);
    void sort_corners(Point2f center);
};

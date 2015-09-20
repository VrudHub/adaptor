#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;

class transforms {
public:
    bool calculate_transforms(Mat* dark, Mat* light);
    void apply_transforms(Mat* m);

private:
    std::vector<Point2f> corners;
    std::vector<Point2f> corners_square;

    void get_corners(std::vector<Vec4i> lines);
    void sort_corners(Point2f center);
};

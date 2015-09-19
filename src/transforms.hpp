#include <opencv2/opencv.hpp>
using namespace cv;

class transforms {
public:
    void calculate_transforms(Mat* dark, Mat* light);
};

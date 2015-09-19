#include <iostream>
#include <opencv2/opencv.hpp>

#include "colours.hpp"
#include "transforms.hpp"

using namespace cv;
using namespace std;

int main(int argc, char *argv[]) {
    cout << "hello" << endl;

    Mat dark = imread("src/dark.jpg", CV_LOAD_IMAGE_COLOR);
    Mat light = imread("src/light.jpg", CV_LOAD_IMAGE_COLOR);

    transforms* t = new transforms();
    t->calculate_transforms(&dark, &light);
    delete t;
    return 0;
}

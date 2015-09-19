#include "utils.hpp"

void display_mat(Mat* m) {
    namedWindow("test", WINDOW_AUTOSIZE);
    imshow("test", *m);
    waitKey(0);
}

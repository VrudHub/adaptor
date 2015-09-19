#include "colours.hpp"

void colours::calculate_colours(Mat* dark, Mat* light) {
    int ch = dark->channels();

    int r = dark->rows;
    int c = dark->cols * ch;

    if(dark->isContinuous()) {
        c *= r;
        r = 1;
    }
}

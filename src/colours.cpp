#include <assert.h>

#include "colours.hpp"

void colours::calculate_colours(Mat* m) {
    w = m->cols;
    h = m->rows;
    ch = 3;
    assert(m->channels() == ch);

    correction = new float[w * h * ch]();
    for(int i = 0; i < h; ++i) {
        uchar *r  = m->ptr<uchar>(i);
        memcpy(correction + (i * w * ch), r, w * ch);
    }

    // TODO: find grey

    // for each pixel, find correction
}

#include "transforms.hpp"
#include "utils.hpp"

#define LIGHT_DIFF 5 // TODO

Mat* get_diff_area(Mat* dark, Mat* light) {
    int ch = dark->channels();
    assert(ch == light->channels());

    int r = dark->rows;
    int c = dark->cols;
    assert(r == light->rows);
    assert(c == light->cols);

    uchar *output = new uchar[r * c * 3]();

    assert(dark->isContinuous() == light->isContinuous());
    /*if(dark->isContinuous()) {
        c *= r;
        r = 1;
    }*/

    long c_ch = c * ch;

    for(long i = 0; i < r; ++i) {
        uchar *d  = dark->ptr<uchar>(i);
        uchar *l  = light->ptr<uchar>(i);
        for(long j = 0; j < c_ch; j += 3) {
            assert(r * c_ch > i * c_ch + j + 2);
            uchar newval;
            int diff = l[j] - d[j] + l[j+1] - d[j+1] + l[j+2] - d[j+2];
            if(diff > LIGHT_DIFF)
                newval = 255;
            else
                newval = 0;
            newval = 2;
            assert(r * c * 3 > i * c * 3 + j+2);
            output[i * c * 3 + j] = newval;
            output[i * c * 3 + j+1] = newval;
            output[i * c * 3 + j+2] = newval;
            if((j == 5 || j == 6 || j == 7) && i == 1)
                printf("ONCE\n");
        }
    }

    printf("%d, %d\n", dark->rows, dark->cols);
    return new Mat(dark->rows, dark->cols, CV_8UC3, output);
    //return new Mat(dark->rows, dark->cols, CV_8UC3, dark->ptr<uchar>(0));
}

void transforms::calculate_transforms(Mat* dark, Mat* light) {
    Mat* m = get_diff_area(dark, light);
    display_mat(m);
}

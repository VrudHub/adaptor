#include "transforms.hpp"
#include "utils.hpp"

#define LIGHT_DIFF 32 // TODO

Mat* get_diff_area(Mat* dark, Mat* light) {
    int ch = dark->channels();
    assert(ch == light->channels());

    int r = dark->rows;
    int c = dark->cols;
    assert(r == light->rows);
    assert(c == light->cols);

    uchar *output = new uchar(r * c * ch);

    assert(dark->isContinuous() == light->isContinuous());
    if(dark->isContinuous()) {
        c *= r;
        r = 1;
    }
    printf("%d, %d\n", c, r);

    int c_ch = c * ch;

    for(int i = 0; i < r; ++i) {
        uchar *d  = dark->ptr<uchar>(i);
        uchar *l  = light->ptr<uchar>(i);
        for(int j = 0; j < c_ch; j += 3) {
            assert(r * c_ch >= i * c_ch + j + 2);
            int newval;
            if(l[j] - d[j] + l[j+1] - d[j+1] + l[j+2] - d[j+2] > LIGHT_DIFF)
                newval = 255;
            else
                newval = 0;
            output[i * c_ch + j] = newval;
            output[i * c_ch + j+1] = newval;
            output[i * c_ch + j+2] = newval;
            if(j >= 10755600)
                printf("%d\n", j);
        }
        printf("%d\n", i);
    }
    printf("f");

    return new Mat(dark->rows, dark->cols, CV_8UC1, *output);
}

void transforms::calculate_transforms(Mat* dark, Mat* light) {
    Mat* m = get_diff_area(dark, light);
    printf("b\n");
    display_mat(m);
}

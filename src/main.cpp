#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>

#include "colours.hpp"
#include "project.hpp"
#include "screen.hpp"
#include "screen-windows.hpp"
#include "transforms.hpp"
#include "utils.hpp"

using namespace cv;
using namespace std;

int main(int argc, char *argv[]) {
    project* p = new project();
    if(p->err) {
        cout << "There was a problem finding the projector.\n"
            << "Please make sure you are on an extended desktop." << endl;
        delete p;
        return 1;
    }

    // dark screenshot
    uchar* arr = (uchar*)malloc(PROJ_WIDTH * PROJ_HEIGHT * 3);
    memset(arr, 0, PROJ_WIDTH * PROJ_HEIGHT * 3);
    Mat* m = new Mat(PROJ_HEIGHT, PROJ_WIDTH, CV_8UC3, arr);
    p->display(m);
    delete m;

    cout << "Press any key after you take black screenshot." << endl;
    int i = getchar();

    // light screenshot
    memset(arr, 255, PROJ_WIDTH * PROJ_HEIGHT * 3);
    m = new Mat(PROJ_HEIGHT, PROJ_WIDTH, CV_8UC3, arr);
    p->display(m);
    delete m;
    free(arr);

    cout << "Press any key after you take white screenshot." << endl;
    i = getchar();

    // TODO: get images from pi
    Mat dark = imread("C:/Users/Alyssa/Dropbox/Apps/Adaptor/a.jpg", CV_LOAD_IMAGE_COLOR);
    Mat light = imread("C:/Users/Alyssa/Dropbox/Apps/Adaptor/b.jpg", CV_LOAD_IMAGE_COLOR);

    // calculate transforms
    transforms* t = new transforms();
    if(!t->calculate_transforms(&dark, &light)) {
        cout << "Error calculating transform for projector.\n"
            << "Ensure you are pointing at a lighter wall, and the camera is"
            << " pointed at the projection." << endl;
        delete t;
        delete p;
        return 1;
    }

#ifdef _WIN32
    screen_windows* s = new screen_windows();

    while(1) {
        if(s) {
            Mat* m = s->capture();
            t->apply_transforms(m);
            p->display(m);
            delete m;
        }
        sleep(10); // TODO: tune
    }

    delete s;
#endif
    delete t;
    delete p;
    return 0;
}

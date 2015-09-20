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
    uchar arr[1] = {0};
    Mat* m = new Mat(1, 1, CV_8UC1, arr);
    p->display(m);
    delete m;

    cout << "Press any key after you take black screenshot." << endl;
    int i = getchar();

    // light screenshot
    arr[0] = 1;
    m = new Mat(1, 1, CV_8UC1, arr);
    p->display(m);
    delete m;

    cout << "Press any key after you take white screenshot." << endl;
    i = getchar();

    // TODO: get images from pi
    Mat dark = imread("dark.jpg", CV_LOAD_IMAGE_COLOR);
    Mat light = imread("light.jpg", CV_LOAD_IMAGE_COLOR);

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

    screen* s = NULL;
#ifdef _WIN32
    s = new screen_windows();
#endif

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
    delete t;
    delete p;
    return 0;
}

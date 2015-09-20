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

Mat getWebcam() {
    VideoCapture cap(0);
    if(!cap.isOpened()) {
        printf("NO\n");
    }

    Mat frame;
    cap >> frame;
    return frame;
}

void demo() {
    Mat white = imread("white.jpg", CV_LOAD_IMAGE_COLOR);
    Mat dark = imread("dark.jpg", CV_LOAD_IMAGE_COLOR);
    Mat light = imread("light.jpg", CV_LOAD_IMAGE_COLOR);
    Mat all_light = imread("all_light.jpg", CV_LOAD_IMAGE_COLOR);

    // if you're hosting a party, and you want to project something, you want
    //   a screen like this.
    display_mat(&white);
    // but you're a poor student, so your room likely looks something like this
    display_mat(&all_light);
    // so you turn out your lights
    display_mat(&dark);
    // and turn on your projector to watch the show
    display_mat(&light);
    // ... (main problem)

    // so the algorithm works is by the rasberry pi taking an image
    //   of the empty wall
    display_mat(&dark);
    // and one of the wall with the projector shining on it
    display_mat(&light);

    // from this, the computer might notice a difference, somewhere in this area.
    transforms t;
    // aka. this area.
    t.demo_show_inner = 1;
    t.calculate_transforms(&dark, &light);
    t.demo_show_inner = 0;
    // so from this, the computer can map the image onto the wall in a nice way.

    Mat poutine = imread("poutine.jpg", CV_LOAD_IMAGE_COLOR);
    // let's say you want to project some poutine using your projector. Because why not.
    display_mat(&poutine);
    // but the computer know that if it projects what it is given, it will look like this
    display_mat(&light);
    t.apply_transforms(&poutine);
    // note the top left looks small, while the bottom right looks large. So, we do the exact opposite to the image
    display_mat(&poutine);
    poutine = imread("poutine.jpg", CV_LOAD_IMAGE_COLOR);
    t.demo_show_inner = 1;
    t.apply_transforms(&poutine);
    t.demo_show_inner = 0;
    // so that the projection looks something like
    display_mat(&poutine);
}

int main(int argc, char *argv[]) {
    demo();
    return 0;

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
    waitKey(0);
    Mat dark = getWebcam();
    waitKey(0);

    // light screenshot
    memset(arr, 255, PROJ_WIDTH * PROJ_HEIGHT * 3);
    m = new Mat(PROJ_HEIGHT, PROJ_WIDTH, CV_8UC3, arr);
    p->display(m);
    delete m;
    free(arr);

    // TODO: get images from pi
    //Mat dark = imread("C:/Users/Alyssa/Dropbox/Apps/Adaptor/a.jpg", CV_LOAD_IMAGE_COLOR);
    //Mat light = imread("C:/Users/Alyssa/Dropbox/Apps/Adaptor/b.jpg", CV_LOAD_IMAGE_COLOR);
    waitKey(30);
    Mat light = getWebcam();
    waitKey(30);

    // TODO: get images from pi
    //Mat dark = imread("dark.jpg", CV_LOAD_IMAGE_COLOR);
    //Mat light = imread("light.jpg", CV_LOAD_IMAGE_COLOR);

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

#pragma once

#include <opencv2/opencv.hpp>
#include <SDL2/SDL.h>
#ifdef __APPLE__
#include <SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif
using namespace cv;

class project {
public:
    project();
    ~project();
    void display(Mat* m);
    int err;

private:
    SDL_Renderer *ren;
    SDL_Window *win;
};

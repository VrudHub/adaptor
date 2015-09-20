#include <iostream>
#include <string>

#include "project.hpp"
#include "utils.hpp"

using namespace std;

project::project() {
    err = 1;

    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return;
    }

    int display_num = SDL_GetNumVideoDisplays() - 1;
    printf("display: %d\n", display_num);

    win = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED_DISPLAY(display_num), SDL_WINDOWPOS_CENTERED_DISPLAY(display_num), PROJ_WIDTH, PROJ_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
    if(!win) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return;
    }

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren){
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return;
    }

    err = 0;
}

project::~project() {
    if(ren)
        SDL_DestroyRenderer(ren);
    if(win)
        SDL_DestroyWindow(win);
    SDL_Quit();
}

void project::display(Mat* m) {
    assert(m->isContinuous());
    SDL_Surface *s = SDL_CreateRGBSurfaceFrom(m->ptr<uchar>(0), m->cols, m->rows, 24, m->step, 0xff0000, 0x00ff00, 0x0000ff, 0);
    //SDL_Surface *s = SDL_CreateRGBSurfaceFrom(img->imageData, img->width, img->height, img->depth * img->nChannels, img->widthStep, 0xff0000, 0x00ff00, 0x0000ff, 0);

    if(!s) {
        std::cout << "SDL_CreateRGBSurfaceFrom Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, s);
    SDL_FreeSurface(s);

    if(!tex) {
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        return;
    }

    //First clear the renderer
    SDL_RenderClear(ren);
    //Draw the texture
    SDL_RenderCopy(ren, tex, NULL, NULL);
    //Update the screen
    SDL_RenderPresent(ren);

    printf("wiating\n");
    waitKey(0);
    printf("wiating 2\n");

    SDL_DestroyTexture(tex);
}

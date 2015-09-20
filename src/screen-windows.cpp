#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "screen-windows.hpp"

Mat* screen_windows::capture() {
    HDC screen = GetDC(GetDesktopWindow());
    int w = GetDeviceCaps(screen, HORZRES);
    int h = GetDeviceCaps(screen, VERTRES);

    HDC mem = CreateCompatibleDC(screen);
    HBITMAP b = CreateCompatibleBitmap(screen, w, h);
    HGDIOBJ obj = SelectObject(mem, b);
    BitBlt(mem, 0, 0, w, h, screen, 0, 0, SRCCOPY);
    SelectObject(mem, obj);

    BITMAPINFOHEADER info = {0};
    info.biSize = sizeof(BITMAPINFOHEADER);
    info.biPlanes = 1;
    info.biBitCount = 32;
    info.biWidth = w;
    info.biHeight = -h;
    info.biCompression = BI_RGB;
    info.biSizeImage = 0;

    uchar* data = new uchar[4 * w * h]();

    GetDIBits(mem, b, 0, h, data, (BITMAPINFO*)&info, DIB_RGB_COLORS);

    ReleaseDC(GetDesktopWindow(), screen);
    DeleteDC(mem);
    DeleteObject(b);

    for(long i = 0; i < h * w * 4; i += 4) {
        uchar tmp = data[i];
        data[i] = data[i+2];
        data[i+2] = tmp;
    }

    return new Mat(h, w, CV_8UC4, data);
}

#endif

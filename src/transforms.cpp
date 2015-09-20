#include <algorithm>
#include <iostream>
#include <vector>

#include "transforms.hpp"
#include "utils.hpp"

using namespace std;

#define LIGHT_DIFF 36 // TODO


transforms::transforms() {
    demo_show_inner = 0;
}

Mat* get_diff_area(Mat* dark, Mat* light) {
    int ch = dark->channels();
    assert(ch == light->channels());

    int r = dark->rows;
    int c = dark->cols;
    assert(r == light->rows);
    assert(c == light->cols);

    uchar *output = new uchar[r * c]();

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
            output[i * c + j/3] = newval;
        }
    }

    return new Mat(dark->rows, dark->cols, CV_8UC1, output);
}

// from: http://opencv-code.com/tutorials/automatic-perspective-correction-for-quadrilateral-objects/
Point2f comp_intersect(Vec4i a, Vec4i b) {
    int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];
    int x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];

    if (float d = ((float)(x1-x2) * (y3-y4)) - ((y1-y2) * (x3-x4))) {
        Point2f pt;
        pt.x = ((x1*y2 - y1*x2) * (x3-x4) - (x1-x2) * (x3*y4 - y3*x4)) / d;
        pt.y = ((x1*y2 - y1*x2) * (y3-y4) - (y1-y2) * (x3*y4 - y3*x4)) / d;
        return pt;
    } else
        return Point2f(-1, -1);
}

void transforms::get_corners(vector<Vec4i> lines) {
    corners.clear();
    for(int i = 0; i < lines.size(); ++i) {
        for(int j = i+1; j < lines.size(); ++j) {
            Point2f pt = comp_intersect(lines[i], lines[j]);
            if(pt.x >= 0 && pt.y >= 0)
                corners.push_back(pt);
        }
    }
}

void transforms::sort_corners(Point2f center) {;
    vector<Point2f> top, bot;

    for(int i = 0; i < corners.size(); ++i) {
        if(corners[i].y < center.y)
            top.push_back(corners[i]);
        else
            bot.push_back(corners[i]);
    }

    Point2f tl = top[0].x > top[1].x ? top[1] : top[0];
    Point2f tr = top[0].x > top[1].x ? top[0] : top[1];
    Point2f bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
    Point2f br = bot[0].x > bot[1].x ? bot[0] : bot[1];

    // make square
    Point2f tl_mod = Point2f(fmax(tl.x, bl.x), fmax(tl.y, tr.y));
    Point2f tr_mod = Point2f(fmin(tr.x, br.x), fmax(tl.y, tr.y));
    Point2f bl_mod = Point2f(fmax(tl.x, bl.x), fmin(bl.y, br.y));
    Point2f br_mod = Point2f(fmin(tr.x, br.x), fmin(bl.y, br.y));

    corners.clear();
    corners.push_back(tl);
    corners.push_back(tr);
    corners.push_back(br);
    corners.push_back(bl);

    corners_square.clear();
    corners_square.push_back(tl_mod);
    corners_square.push_back(tr_mod);
    corners_square.push_back(br_mod);
    corners_square.push_back(bl_mod);

    cout << "END OF SQUARES\n\n";
}

int len_line(Vec4i a) {
    return (a[0] - a[2]) + (a[1] - a[3]);
}

int length_lines(Vec4i a, Vec4i b) {
    return len_line(a) > len_line(b);
}

int point_diff(Vec4i a, Vec4i b) {
    return min(abs(a[0] - b[0]) + abs(a[1] - b[1]) + abs(a[2] - b[2]) + abs(a[3] - b[3]),
            abs(a[0] - b[2]) + abs(a[1] - b[3]) + abs(a[2] - b[0]) + abs(a[3] - b[1]));
}

float point_diff(Point2f a, Point2f b) {
    return fabs(a.x - b.x) + fabs(a.y - b.y);
}

void filter_lines(vector<Vec4i>& l) {
    sort(l.begin(), l.end(), length_lines);

    while(l.size() >= 4)
        l.pop_back();

    // TODO: better
    /*Vector<
    for(int i = 0; i < l.size(); ++i) {
        for(int j = i + 1; j < l.size(); ++j) {
            if()
        }
    }*/
}

void filter_corners(vector<Point2f>& corners) {
    while(corners.size() > 4) {
        float min_len = -1;
        int index = 0;
        for(int i = 0; i < corners.size(); ++i) {
            for(int j = i + 1; j < corners.size(); ++j) {
                float len = point_diff(corners[i], corners[j]);
                if(len < min_len || min_len == -1) {
                    min_len = len;
                    index = i;
                }
            }
        }
        corners.erase(corners.begin()+index);
    }
}

bool transforms::calculate_transforms(Mat* dark, Mat* light) {
    Mat* m = get_diff_area(dark, light);
    if(demo_show_inner)
        display_mat(m);
    Canny(*m, *m, 100, 100, 3);
    if(demo_show_inner)
        display_mat(m);

    vector<Vec4i> lines;
    HoughLinesP(*m, lines, 1, CV_PI/180, 70, 30, 10);

    //filter_lines(lines);

    cout << "Lines: " << lines.size() << endl;
    get_corners(lines);

    cout <<"Corners 1: " << corners.size() << endl;
    cout << corners << endl;
    filter_corners(corners);
    cout<< "Corners 2: " << corners.size() << endl;
    cout << corners << endl;

    vector<Point2f> approx;
    approxPolyDP(Mat(corners), approx, arcLength(Mat(corners), true) * 0.02, true);
    cout<< "approx: " << approx.size() << endl;

    if(approx.size() != 4)
        return false;

    // get center
    Point2f center(0,0);
    for(int i = 0; i < corners.size(); ++i)
        center += corners[i];

    center *= (1. / corners.size());
    sort_corners(center);
    return true;
}

void transforms::apply_transforms(Mat* m) {
    vector<Point2f> m_pts;
    m_pts.push_back(Point2f(0, 0));
    m_pts.push_back(Point2f(m->cols, 0));
    m_pts.push_back(Point2f(m->cols, m->rows));
    m_pts.push_back(Point2f(0, m->rows));

    //Mat transmtx = getPerspectiveTransform(m_pts, corners);
    Mat transmtx;
    if(!demo_show_inner)
        transmtx = getPerspectiveTransform(corners, corners_square);
    else
        transmtx = getPerspectiveTransform(m_pts, corners_square);

    // apply transform
    warpPerspective(*m, *m, transmtx, m->size());
}


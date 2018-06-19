#ifndef IMAGE_H
#define IMAGE_H
#include "all.h"
#include "vector3.h"
using namespace std;
using namespace cv;
class Image{
public:
    int w;
    int h;
    Mat image;
    Image();
    Image(int _w, int _h);
    Image(const string &filename,int flags = IMREAD_COLOR);
    void save(const string &filename) const ;
    void drawPixel(int x,int y,Color color);
    void show(const string &title) const;
    Color getPixel(int x,int y) const;
    void drawLine(double x0, double y0, double x1, double y1, Color color, int width);
};


#endif

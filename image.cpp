#include "image.h"

Image::Image(){}
Image::Image(int _w, int _h):w(_w),h(_h){
    image = Mat(h,w,CV_8UC3,Scalar(0,0,0));
}
Image::Image(const string &filename,int flags){
    image = imread(filename,flags);
    w = image.cols;
    h = image.rows;
}
void Image::save(const string &filename) const{
    imwrite(filename,image);
}

void Image::drawPixel(int x,int y,Color color){
    if(x>=0 && y>=0 && x<w && y<h) {
        color.b = max(color.b,0.01);
        color.g = max(color.g,0.01);
        color.r = max(color.r,0.01);
        image.at<Vec3b>(y, x)[0] = min((int)color.b,255);
        image.at<Vec3b>(y, x)[1] = min((int)color.g,255);
        image.at<Vec3b>(y, x)[2] = min((int)color.r,255);
    }
}
void Image::show(const string &title) const{
    imshow(title,image);
}
Color Image::getPixel(int x, int y) const{
    if(x >= 0 && y >= 0 && x < w && y < h)
        return Color(image.at<Vec3b>(y,x)[2],image.at<Vec3b>(y,x)[1],image.at<Vec3b>(y,x)[0]);
    return Color();
}

void Image::drawLine(double x0, double y0, double x1, double y1, Color color, int width){
    line(image, Point(x0,y0), Point(x1,y1), Scalar((int)color.b, (int)color.g, (int)color.r),width);
}

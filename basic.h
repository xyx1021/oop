#pragma once

#include <QString>
#define M_PI 3.1415926
struct Point
{
public:
    Point() {}
    Point(double x, double y) :x(x), y(y) { }
    double x, y;
    double getAngleBetween(const Point& p) const
    {
        double dx = p.x - this->x;
        double dy = p.y - this->y;
        return atan2(dy, dx) * 180.0 / M_PI;
    }

};


struct BBox
{
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    Point center;
};






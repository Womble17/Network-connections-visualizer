#ifndef DRAWINGPACKAGE_HPP
#define DRAWINGPACKAGE_HPP

#include <vector>

struct CurveData
{
    // coordinates of first node
    float x1;
    float y1;

    // coordinates of second node
    float x2;
    float y2;

    // RGB coded olor
    short r = 0;
    short g = 0;
    short b = 0;
};


class DrawingPackage
{
public:
    std::vector<CurveData> drawDataPackage;
};

#endif // DRAWINGPACKAGE_HPP

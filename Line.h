#ifndef Line_H
#define Line_H

#include "stdafx.h"
#include "Point2D.h"
#include "Colour.h"
#include "rapidxml.hpp"

using namespace Gdiplus;
using namespace rapidxml;

class Line {
protected:
    Point2D start;
    Point2D end;
    Stroke stroke;

public:
    Line();
    ~Line();

    Point2D getStart();
    Point2D getEnd();
    float getStrokeWidth();
    Colour getStrokeColour();

    void setStart(Point2D a);
    void setEnd(Point2D b);
    void setStrokeWidth(float w);
    void setStrokeColour(Colour StkColour);

};
vector<Line> parseLine(const string& filename);
void drawLine(Graphics* graphics, vector<Line>& line);


#endif

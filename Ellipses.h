#ifndef Ellipse_H
#define Ellipse_H

#include "stdafx.h"
#include "Point2D.h"
#include "Colour.h"
#include "rapidxml.hpp"

using namespace Gdiplus;
using namespace rapidxml;

class Ellipses {
protected:
    float rx, ry;
    Point2D center;
    Colour fill;
    Stroke stroke;
public:
    Ellipses();
    virtual ~Ellipses();

    float getRX();
    float getRY();
    Point2D getCenter();
    Colour getColour();
    float getStrokeWidth();
    Colour getStrokeColour();

    void setRX(float x);
    void setRY(float y);
    void setCenter(float x, float y);
    void setColour(Colour colour);
    void setStrokeWidth(float w);
    void setStrokeColour(Colour stkColour);
};

vector<Ellipses> parseEllipses(const string& filename);
void drawEllipses(Graphics* graphics, vector<Ellipses>& ellipses);

#endif

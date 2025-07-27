#ifndef Polylines_H
#define Polylines_H

#include "stdafx.h"
#include "Point2D.h"
#include "Colour.h"
#include "rapidxml.hpp"


using namespace Gdiplus;
using namespace rapidxml;

class Polylines {
protected:
    vector<Point2D> points;
    Stroke stroke;
    Colour fill;
public:
    Polylines();
    ~Polylines();

    vector<Point2D> getPoints();
    float getStrokeWidth();
    Colour getStrokeColour();
    Colour getColour();

    void setPoints(vector<Point2D> a);
    void setStrokeWidth(float w);
    void setStrokeColour(Colour StkColour);
    void setColour(Colour colour);
};

vector<Polylines> parsePolylines(const string& filename);
void drawPolylines(Graphics* graphics, vector<Polylines>& Polylines);

#endif

#ifndef Polyline_H
#define Polyline_H

#include "stdafx.h"
#include "Point2D.h"
#include "Colour.h"
#include "rapidxml.hpp"

using namespace Gdiplus;
using namespace rapidxml;

class Polyline {
protected:
    vector<Point2D> points;
    Stroke stroke;

public:
    Polyline();
    ~Polyline();

    vector<Point2D> getPoints();
    float getStrokeWidth();
    Colour getStrokeColour();

    void setPoints(vector<Point2D> a);
    void setStrokeWidth(float w);
    void setStrokeColour(Colour StkColour);
};

vector<Polyline> parsePolyline(const string& filename);
void drawPolyline(Graphics* graphics, vector<Polyline>& polylines);

#endif
#ifndef Polylines_H
#define Polylines_H


#include "stdafx.h"
#include "Point2D.h"
#include "Colour.h"
#include "rapidxml.hpp"

using namespace rapidxml;
using namespace Gdiplus;

class Polylines {
private:
    vector<Point2D> points;
    Stroke stroke;
    Colour fill;
public:
    Polylines();
    ~Polylines();

    vector<Point2D>& getPoints();
    Colour getColour();
    float getStrokeWidth();
    Colour getStrokeColour();

    void setPoints(vector<Point2D>& pts);
    void addPoint(Point2D& pt);
    void setColour(Colour colour);
    void setStrokeWidth(float w);
    void setStrokeColour(Colour StkColour);
};

vector<Polylines> parsePolylines(const string& filename);
void drawPolylines(Graphics* graphics, vector<Polylines>& polylines);


#endif

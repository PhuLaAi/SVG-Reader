#ifndef Shapes_H
#define Shapes_H

#include "stdafx.h"
#include "Circle.h"
#include "Line.h"
#include "Polylines.h"
#include "Ellipses.h"

struct Shapes {
    vector<Circle> circles;
    vector<Line> lines;
    vector<Polylines> polylines;
    vector<Ellipses> ellipses;
};

#endif

#ifndef LINE_H
#define LINE_H

#include <windows.h>
#include <gdiplus.h>
#include <string>
#include "SVGElement.h"
#include "Point2D.h"
#include "Color.h"
#include "rapidxml.hpp"

using namespace Gdiplus;
using namespace rapidxml;

class Line : public SVGElement {
protected:
    Point2D start;
    Point2D end;
    Color stroke;
    float strokeWidth;

public:
    Line();
    Line(Point2D s, Point2D e, Color color, float width);

    void draw(Graphics& graphics) override;
    void parse(xml_node<>* node) override;
    ~Line();
};

#endif

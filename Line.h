#ifndef LINE_H
#define LINE_H

#include <gdiplus.h>
#include "Point2D.h"
#include "Colour.h"
#include "rapidxml.hpp"

using namespace Gdiplus;
using namespace rapidxml;

class Line {
private:
    Point2D start;
    Point2D end;
    Stroke stroke;

public:
    Line();
    ~Line();

    void parse(xml_node<>* node);                // Parse from SVG <line> tag
    void draw(Graphics& graphics);               // Draw using GDI+
};

#endif

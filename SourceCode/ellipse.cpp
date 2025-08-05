#include "ellipse.h"
#include "utils.h"

ellipseShape::ellipseShape() : rx(0), ry(0), center(0, 0) {}

void ellipseShape::loadFromXML(xml_node<>* node) {
    Shape::loadFromXML(node);

    center.x = parseInt(node->first_attribute("cx") ? node->first_attribute("cx")->value() : "0");
    center.y = parseInt(node->first_attribute("cy") ? node->first_attribute("cy")->value() : "0");
    rx = parseInt(node->first_attribute("rx") ? node->first_attribute("rx")->value() : "0");
    ry = parseInt(node->first_attribute("ry") ? node->first_attribute("ry")->value() : "0");
}

void ellipseShape::draw(Graphics& g) {
    if (hasFill) {
        SolidBrush brush(fillColor);
        g.FillEllipse(&brush, center.x - rx, center.y - ry, rx * 2, ry * 2);
    }
    
    if (hasStroke) {
    Pen pen(strokeColor, strokeWidth);
    g.DrawEllipse(&pen, center.x - rx, center.y - ry, rx * 2, ry * 2);
    }
}

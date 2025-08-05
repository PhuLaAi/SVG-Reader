#include "rectangle.h"

rectangleShape::rectangleShape() {}

void rectangleShape::loadFromXML(xml_node<>* node) {
    Shape::loadFromXML(node);
}

void rectangleShape::draw(Graphics& g) {
    if (hasFill) {
        SolidBrush brush(fillColor);
        g.FillRectangle(&brush, x, y, width, height);
    }
    if (hasStroke) {
        Pen pen(strokeColor, strokeWidth);
        g.DrawRectangle(&pen, x, y, width, height);
    }
}
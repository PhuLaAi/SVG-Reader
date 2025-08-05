#include "polygon.h"
#include "utils.h"

polygonShape::polygonShape() {}

void polygonShape::loadFromXML(xml_node<>* node) {
    Shape::loadFromXML(node);

    const char* pts = node->first_attribute("points") ? node->first_attribute("points")->value() : "";
    points = parsePoints(pts);
}

void polygonShape::draw(Graphics& g) {
    if (points.size() < 2) return;
    auto gdipPoints = toGdiplusPoints(points);

    if (hasFill) {
        SolidBrush brush(fillColor);
        g.FillPolygon(&brush, gdipPoints.data(), gdipPoints.size());
    }
    if (hasStroke) {
        Pen pen(strokeColor, strokeWidth);
        g.DrawPolygon(&pen, gdipPoints.data(), gdipPoints.size());
    }
}


#include "polyline.h"
#include "utils.h"

polylineShape::polylineShape() {}

void polylineShape::loadFromXML(xml_node<>* node) {
    Shape::loadFromXML(node);
    const char* pts = node->first_attribute("points") ? node->first_attribute("points")->value() : "";
    points = parsePoints(pts);
}

void polylineShape::draw(Graphics& g) {
    if (points.size() < 2) return;
    auto gdipPoints = toGdiplusPoints(points);

    if (hasFill) {
        SolidBrush brush(fillColor);
        g.FillPolygon(&brush, gdipPoints.data(), gdipPoints.size());
    }
    if (hasStroke) {
        Pen pen(strokeColor, strokeWidth);
        g.DrawLines(&pen, gdipPoints.data(), gdipPoints.size());
    }
}

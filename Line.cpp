#include "Line.h"
#include <sstream>
#include <cstdlib>

Line::Line() : start(0, 0), end(0, 0), stroke(), strokeWidth(1.0f) {}

Line::Line(Point2D s, Point2D e, Color color, float width)
    : start(s), end(e), stroke(color), strokeWidth(width) {}

void Line::draw(Graphics& graphics) {
    Gdiplus::Color penColor(
        static_cast<BYTE>(stroke.r * 255),
        static_cast<BYTE>(stroke.g * 255),
        static_cast<BYTE>(stroke.b * 255)
    );
    Gdiplus::Pen pen(penColor, strokeWidth);
    graphics.DrawLine(&pen, start.x, start.y, end.x, end.y);
}

void Line::parse(xml_node<>* node) {
    auto getFloatAttr = [](xml_node<>* n, const char* attr, float defaultVal = 0.0f) {
        if (auto* a = n->first_attribute(attr)) return std::stof(a->value());
        return defaultVal;
    };

    float x1 = getFloatAttr(node, "x1");
    float y1 = getFloatAttr(node, "y1");
    float x2 = getFloatAttr(node, "x2");
    float y2 = getFloatAttr(node, "x2");

    start.setPoint(x1, y1);
    end.setPoint(x2, y2);

    const char* strokeStr = node->first_attribute("stroke") ? node->first_attribute("stroke")->value() : "#000000";
    stroke = Color::fromHex(strokeStr);

    strokeWidth = getFloatAttr(node, "stroke-width", 1.0f);
}

Line::~Line() {}
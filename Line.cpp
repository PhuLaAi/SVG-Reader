#include "Line.h"
#include <string>
#include <sstream>

// Constructor / Destructor
Line::Line() {}
Line::~Line() {}

// Convert hex color string (e.g. "#ff0000") to Colour
Colour parseColour(const string& hex) {
    if (hex.empty() || hex[0] != '#') return Colour(0, 0, 0, 1);

    unsigned int r, g, b;
    std::stringstream ss;
    ss << std::hex << hex.substr(1);
    unsigned int colorInt;
    ss >> colorInt;

    r = (colorInt >> 16) & 0xFF;
    g = (colorInt >> 8) & 0xFF;
    b = colorInt & 0xFF;

    return Colour(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
}

// Parse from <line> node
void Line::parse(xml_node<>* node) {
    if (!node) return;

    float x1 = node->first_attribute("x1") ? std::stof(node->first_attribute("x1")->value()) : 0.0f;
    float y1 = node->first_attribute("y1") ? std::stof(node->first_attribute("y1")->value()) : 0.0f;
    float x2 = node->first_attribute("x2") ? std::stof(node->first_attribute("x2")->value()) : 0.0f;
    float y2 = node->first_attribute("y2") ? std::stof(node->first_attribute("y2")->value()) : 0.0f;

    start.setX(x1); start.setY(y1);
    end.setX(x2); end.setY(y2);

    // Stroke color
    string strokeColor = node->first_attribute("stroke") ? node->first_attribute("stroke")->value() : "#000000";
    stroke.setStrokeColour(parseColour(strokeColor));

    // Stroke width
    float width = node->first_attribute("stroke-width") ? std::stof(node->first_attribute("stroke-width")->value()) : 1.0f;
    stroke.setStrokeWidth(width);
}

// Draw with GDI+
void Line::draw(Graphics& graphics) {
    Colour c = stroke.getStrokeColour();
    Gdiplus::Color penColor(static_cast<BYTE>(c.o * 255), static_cast<BYTE>(c.r * 255), static_cast<BYTE>(c.g * 255), static_cast<BYTE>(c.b * 255));
    Pen pen(penColor, stroke.getStrokeWidth());

    graphics.DrawLine(&pen, start.getX(), start.getY(), end.getX(), end.getY());
}

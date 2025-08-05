#include "shape.h"
#include "utils.h"

Shape::Shape() : x(0), y(0), width(0), height(0), strokeWidth(1.0f), fillOpacity(1.0f), fillColor(Color(255, 255, 255, 255)), strokeColor(Color(255, 0, 0, 0)), hasFill(true), hasStroke(true) {}

void Shape::loadFromXML(xml_node<>* node) {
    auto getAttr = [&](const char* name) -> const char* {
        xml_attribute<>* attr = node->first_attribute(name);
        return attr ? attr->value() : "";
    };

    x = parseInt(getAttr("x"));
    y = parseInt(getAttr("y"));
    width = parseInt(getAttr("width"));
    height = parseInt(getAttr("height"));
    strokeWidth = parseFloat(getAttr("stroke-width"));
    fillOpacity = parseFloat(getAttr("fill-opacity"));
    fillColor = parseRGB(getAttr("fill"), (BYTE)(255 * fillOpacity));
    strokeColor = parseRGB(getAttr("stroke"), 255);
    // Thêm vào attributes của shape
    float strokeOpacity = parseFloat(getAttr("stroke-opacity"), 1.0f);

    const char* fillStr = getAttr("fill");
    const char* strokeStr = getAttr("stroke");

    hasFill = !(fillStr && strcmp(fillStr, "none") == 0);
    hasStroke = !(strokeStr && strcmp(strokeStr, "none") == 0);

    fillColor = hasFill ? parseRGB(fillStr, (int)(255 * fillOpacity)) : Color(0, 0, 0, 0);
    strokeColor = hasStroke ? parseRGB(strokeStr, (int)(255 * strokeOpacity)) : Color(0, 0, 0, 0);
}
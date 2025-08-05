#include "line.h"
#include "utils.h"

lineShape::lineShape() : start(0, 0), end(0, 0) {}

void lineShape::loadFromXML(xml_node<>* node) {
    strokeWidth = parseFloat(node->first_attribute("stroke-width") ? node->first_attribute("stroke-width")->value() : "1");
    strokeColor = parseRGB(node->first_attribute("stroke") ? node->first_attribute("stroke")->value() : "rgb(0,0,0)", 255);

    start.x = parseInt(node->first_attribute("x1") ? node->first_attribute("x1")->value() : "0");
    start.y = parseInt(node->first_attribute("y1") ? node->first_attribute("y1")->value() : "0");
    end.x = parseInt(node->first_attribute("x2") ? node->first_attribute("x2")->value() : "0");
    end.y = parseInt(node->first_attribute("y2") ? node->first_attribute("y2")->value() : "0");
}

void lineShape::draw(Graphics& g) {
    if (hasStroke) {
        Pen pen(strokeColor, strokeWidth);
        g.DrawLine(&pen, start.x, start.y, end.x, end.y);
    }
}

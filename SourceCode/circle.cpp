#include "circle.h"
#include "utils.h"

circleShape::circleShape() {}

void circleShape::loadFromXML(xml_node<>* node) {
    ellipseShape::loadFromXML(node);

    int r = parseInt(node->first_attribute("r") ? node->first_attribute("r")->value() : "0");
    rx = ry = r;

    center.x = parseInt(node->first_attribute("cx") ? node->first_attribute("cx")->value() : "0");
    center.y = parseInt(node->first_attribute("cy") ? node->first_attribute("cy")->value() : "0");
}

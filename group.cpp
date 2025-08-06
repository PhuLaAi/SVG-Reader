#include "group.h"
#include "circle.h"
#include "ellipse.h"
#include "line.h"
#include "rectangle.h"
#include "polygon.h"
#include "polyline.h"

groupShape::groupShape() {
}

groupShape::~groupShape() {
    for (auto child : children) {
        delete child;
    }
    children.clear();
}

void groupShape::loadFromXML(xml_node<>* node) {
    for (xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {
        std::string tag = child->name();
        Shape* shape = nullptr;

        if (tag == "rect") shape = new rectangleShape();
        else if (tag == "circle") shape = new circleShape();
        else if (tag == "ellipse") shape = new ellipseShape();
        else if (tag == "line") shape = new lineShape();
        else if (tag == "polygon") shape = new polygonShape();
        else if (tag == "polyline") shape = new polylineShape();
        else if (tag == "g") shape = new groupShape();

        if (shape) {
            shape->loadFromXML(child);
            children.push_back(shape);
        }
    }
}

void groupShape::draw(Graphics& g) {
    for (Shape* child : children) {
        child->draw(g);
    }
}
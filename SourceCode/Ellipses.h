#pragma once

#include "shape.h"
#include "point2D.h"

class ellipseShape : public Shape {
protected:
    point2D center;
    int rx, ry;

public:
    ellipseShape();
    void draw(Graphics& g) override;
    void loadFromXML(xml_node<>* node) override;
};

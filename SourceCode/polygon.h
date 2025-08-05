#pragma once

#include "shape.h"
#include "point2D.h"

class polygonShape : public Shape {
protected:
    vector<point2D> points;

public:
    polygonShape();
    void draw(Graphics& g) override;
    void loadFromXML(xml_node<>* node) override;
};

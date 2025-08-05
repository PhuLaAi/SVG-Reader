#pragma once

#include "shape.h"
#include "point2D.h"

class polylineShape : public Shape {
private:
    vector<point2D> points;

public:
    polylineShape();
    void draw(Graphics& g) override;
    void loadFromXML(xml_node<>* node) override;
};

#pragma once

#include "shape.h"
#include "point2d.h"

class lineShape : public Shape {
private:
    point2D start, end;

public:
    lineShape();
    void draw(Graphics& g) override;
    void loadFromXML(xml_node<>* node) override;
};

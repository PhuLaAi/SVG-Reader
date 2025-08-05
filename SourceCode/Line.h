#pragma once

#include "Shape.h"
#include "point2D.h"

class lineShape : public Shape {
private:
    point2D start, end;

public:
    lineShape();
    void draw(Graphics& g) override;
    void loadFromXML(xml_node<>* node) override;
};



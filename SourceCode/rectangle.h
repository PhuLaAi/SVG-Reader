#pragma once

#include "shape.h"
#include "libs.h"

class rectangleShape : public Shape {
public:
    rectangleShape();
    void draw(Graphics& g) override;
    void loadFromXML(xml_node<>* node) override;
};

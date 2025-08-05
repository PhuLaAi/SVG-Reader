#pragma once
#include "ellipse.h"

class circleShape : public ellipseShape {
public:
    circleShape();
    void loadFromXML(xml_node<>* node) override;

};


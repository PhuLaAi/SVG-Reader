#pragma once

#include "libs.h"

class Shape {
protected:
    int x, y, width, height;
    float strokeWidth, fillOpacity;
    Color fillColor;
    Color strokeColor;
    bool hasFill;
    bool hasStroke;

public:
    Shape();
    virtual void draw(Graphics& g) = 0;
    virtual void loadFromXML(xml_node<>* node);
    virtual ~Shape() {}

};

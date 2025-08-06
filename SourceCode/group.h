#pragma once
#include "shape.h"
#include "libs.h"

class groupShape : public Shape {
private:
    std::vector<Shape*> children;

public:
    groupShape();
    ~groupShape();
    string getAttribute(xml_node<>* node, const char* attrName);
    void loadFromXML(xml_node<>* node) override;
    void draw(Graphics& g) override;
};

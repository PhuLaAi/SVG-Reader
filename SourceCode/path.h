#pragma once
#include "shape.h"
#include "point2D.h"
#include "utils.h"
#include "libs.h"

class pathShape : public Shape {
private:
    string d;
public:
    pathShape();
    void loadFromXML(xml_node<>* node) override;
    void draw(Graphics& g) override;
};

void parsePathData(const string& d, GraphicsPath& path);

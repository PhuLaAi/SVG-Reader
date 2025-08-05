#include "parser.h"

void LoadShapesFromSVG(const char* filename, vector<Shape*>& shapes) {
    ifstream file(filename);
    vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    xml_document<> doc;
    doc.parse<0>(&buffer[0]);

    xml_node<>* svg = doc.first_node("svg");
    for (xml_node<>* node = svg->first_node(); node; node = node->next_sibling()) {
        string tag = node->name();
        Shape* shape = nullptr;

        if (tag == "rect") shape = new rectangleShape();
        else if (tag == "ellipse") shape = new ellipseShape();
        else if (tag == "circle") shape = new circleShape();
        else if (tag == "polygon") shape = new polygonShape();
        else if (tag == "line") shape = new lineShape();
        else if (tag == "polyline") shape = new polylineShape();
        else if (tag == "text") shape = new textShape();

        if (shape) {
            shape->loadFromXML(node);
            shapes.push_back(shape);
        }
    }
}
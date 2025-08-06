#include "group.h"
#include "circle.h"
#include "ellipse.h"
#include "line.h"
#include "rectangle.h"
#include "polygon.h"
#include "polyline.h"
#include "text.h"

groupShape::groupShape() {
}

groupShape::~groupShape() {
    for (auto child : children) {
        delete child;
    }
    children.clear();
}

string groupShape::getAttribute(rapidxml::xml_node<>* node, const char* name) {
    rapidxml::xml_attribute<>* attr = node->first_attribute(name);
    if (attr)
        return attr->value();
    return "";
}

void groupShape::loadFromXML(xml_node<>* node) {
    string groupStroke = getAttribute(node, "stroke");
    string groupStrokeWidth = getAttribute(node, "stroke-width");
    string groupFillOpacity = getAttribute(node, "fill-opacity");
    string groupFill = getAttribute(node, "fill");
    xml_document<>* doc = node->document(); // Required to allocate strings

    for (xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {
        std::string tag = child->name();
        Shape* shape = nullptr;

        // Step 2: Inherit attributes if child is shape (not group) and doesn't have them
        if (tag != "g" && tag != "text") {
            if (groupStroke != "" && child->first_attribute("stroke") == nullptr) {
                char* name = doc->allocate_string("stroke");
                char* value = doc->allocate_string(groupStroke.c_str());
                child->append_attribute(doc->allocate_attribute(name, value));
            }
            if (groupStrokeWidth != "" && child->first_attribute("stroke-width") == nullptr) {
                char* name = doc->allocate_string("stroke-width");
                char* value = doc->allocate_string(groupStrokeWidth.c_str());
                child->append_attribute(doc->allocate_attribute(name, value));
            }
            if (groupFillOpacity != "" && child->first_attribute("fill-opacity") == nullptr) {
                char* name = doc->allocate_string("fill-opacity");
                char* value = doc->allocate_string(groupFillOpacity.c_str());
                child->append_attribute(doc->allocate_attribute(name, value));
            }
            if (groupFill != "" && child->first_attribute("fill") == nullptr) {
                char* name = doc->allocate_string("fill");
                char* value = doc->allocate_string(groupFill.c_str());
                child->append_attribute(doc->allocate_attribute(name, value));
            }
        }

        if (tag == "rect") shape = new rectangleShape();
        else if (tag == "circle") shape = new circleShape();
        else if (tag == "ellipse") shape = new ellipseShape();
        else if (tag == "line") shape = new lineShape();
        else if (tag == "polygon") shape = new polygonShape();
        else if (tag == "polyline") shape = new polylineShape();
        else if (tag == "text") continue;
        else if (tag == "path") continue;
        else if (tag == "g") shape = new groupShape();

        if (shape) {
            shape->loadFromXML(child);
            children.push_back(shape);
        }
    }
}

void groupShape::draw(Graphics& g) {
    for (Shape* child : children) {
        child->draw(g);
    }
}

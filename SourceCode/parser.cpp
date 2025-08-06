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
        else if (tag == "path") shape = new pathShape();
        else if (tag == "g") shape = new groupShape();

        if (shape) {
            shape->loadFromXML(node);
            shapes.push_back(shape);
        }
    }

}

const char* getAttrRecursive(xml_node<>* node, const char* name) {
    while (node) {
        xml_attribute<>* attr = node->first_attribute(name);
        if (attr) return attr->value();
        node = node->parent();
    }
    return "";
}

vector<text> parseText(const string& filename) {
    vector<text> texts;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file\n";
        return texts;
    }

    string xmlContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    vector<char> buffer(xmlContent.begin(), xmlContent.end());
    buffer.push_back('\0');

    xml_document<> doc;
    doc.parse<0>(&buffer[0]);

    xml_node<>* svg = doc.first_node("svg");
    if (!svg) {
        cerr << "Unable to find <svg> node\n";
        return texts;
    }

    function<void(xml_node<>*)> processNode = [&](xml_node<>* node) {
        if (!node) return;

        if (string(node->name()) == "text") {
            text t;

            if (xml_attribute<>* x = node->first_attribute("x"))
                t.setTextPos(stof(x->value()), t.getTextPos().getY());

            if (xml_attribute<>* y = node->first_attribute("y"))
                t.setTextPos(t.getTextPos().getX(), stof(y->value()));

            if (xml_attribute<>* dx = node->first_attribute("dx"))
                t.setDx(stof(dx->value()));

            if (xml_attribute<>* dy = node->first_attribute("dy"))
                t.setDy(stof(dy->value()));

            if (xml_attribute<>* fs = node->first_attribute("font-size"))
                t.setFontSize(stof(fs->value()));

            if (xml_attribute<>* fa = node->first_attribute("font-family"))
                t.setFontFamily(fa->value());

            if (xml_attribute<>* fst = node->first_attribute("font-style"))
                t.setFontStyle(fst->value());

            if (xml_attribute<>* anchor = node->first_attribute("text-anchor"))
                t.setTextAnchor(anchor->value());

            float fillOpacity = 1.0f;
            if (xml_attribute<>* fillOp = node->first_attribute("fill-opacity"))
                fillOpacity = stof(fillOp->value());

            if (xml_attribute<>* fill = node->first_attribute("fill")) {
                string colorStr = fill->value();

                if (colorStr[0] == '#') {
                    int r = stoi(colorStr.substr(1, 2), nullptr, 16);
                    int g = stoi(colorStr.substr(3, 2), nullptr, 16);
                    int b = stoi(colorStr.substr(5, 2), nullptr, 16);
                    t.setColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, fillOpacity));
                }
                else if (colorStr.substr(0, 4) == "rgb(") {
                    string inner = colorStr.substr(4, colorStr.size() - 5);
                    replace(inner.begin(), inner.end(), ',', ' ');
                    istringstream iss(inner);
                    int r = 0, g = 0, b = 0;
                    iss >> r >> g >> b;
                    t.setColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, fillOpacity));
                }
            }

            if (xml_attribute<>* strokeWidthAttr = node->first_attribute("stroke-width")) {
                float strokeWidth = stof(strokeWidthAttr->value());
                t.setStrokeWidth(strokeWidth);
            }

            float strokeOpacity = 1.0f;
            if (xml_attribute<>* strokeOp = node->first_attribute("stroke-opacity"))
                strokeOpacity = stof(strokeOp->value());

            if (xml_attribute<>* strokeAttr = node->first_attribute("stroke")) {
                string strokeColor = strokeAttr->value();

                if (strokeColor[0] == '#') {
                    int r = stoi(strokeColor.substr(1, 2), nullptr, 16);
                    int g = stoi(strokeColor.substr(3, 2), nullptr, 16);
                    int b = stoi(strokeColor.substr(5, 2), nullptr, 16);
                    t.setStrokeColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, strokeOpacity));
                }
                else if (strokeColor.substr(0, 4) == "rgb(") {
                    string inner = strokeColor.substr(4, strokeColor.size() - 5);
                    replace(inner.begin(), inner.end(), ',', ' ');
                    istringstream iss(inner);
                    int r = 0, g = 0, b = 0;
                    iss >> r >> g >> b;
                    t.setStrokeColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, strokeOpacity));
                }
            }

            string content = node->value();
            if (content.empty() && node->first_node())
                content = node->first_node()->value();
            t.setContent(content);

            texts.push_back(t);
        }

        for (xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {
            processNode(child);
        }
        };

    processNode(svg);

    return texts;
}




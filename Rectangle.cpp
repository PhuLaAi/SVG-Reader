#include "Libs.h"

// Constructor/destructor
Rectangle::Rectangle() : x(0), y(0), width(0), length(0), fill(), stroke() {}
Rectangle::~Rectangle() {}

float Rectangle::getX() { return x; }
float Rectangle::getY() { return y; }
float Rectangle::getWidth() { return width; }
float Rectangle::getLength() { return length; }
Colour Rectangle::getColour() { return fill; }
float Rectangle::getStrokeWidth() { return stroke.getStrokeWidth(); }
Colour Rectangle::getStrokeColour() { return stroke.getStrokeColour(); }

void Rectangle::setX(float val) { x = val; }
void Rectangle::setY(float val) { y = val; }
void Rectangle::setWidth(float w) { width = w; }
void Rectangle::setLength(float l) { length = l; }
void Rectangle::setColour(Colour c) { fill = c; }
void Rectangle::setStrokeWidth(float w) { stroke.setStrokeWidth(w); }
void Rectangle::setStrokeColour(Colour c) { stroke.setStrokeColour(c); }

// Parse rectangles from SVG
vector<Rectangle> parseRectangle(const string& filename) {
    vector<Rectangle> rectangles;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file\n";
        return rectangles;
    }

    string xmlContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    vector<char> buffer(xmlContent.begin(), xmlContent.end());
    buffer.push_back('\0');

    xml_document<> doc;
    doc.parse<0>(&buffer[0]);

    xml_node<>* svg = doc.first_node("svg");
    if (!svg) {
        cerr << "Unable to find <svg> node\n";
        return rectangles;
    }

    for (xml_node<>* node = svg->first_node("rect"); node; node = node->next_sibling("rect")) {
        Rectangle r;

        if (xml_attribute<>* xAttr = node->first_attribute("x"))
            r.setX(stof(xAttr->value()));
        if (xml_attribute<>* yAttr = node->first_attribute("y"))
            r.setY(stof(yAttr->value()));
        if (xml_attribute<>* wAttr = node->first_attribute("width"))
            r.setWidth(stof(wAttr->value()));
        if (xml_attribute<>* hAttr = node->first_attribute("height"))
            r.setLength(stof(hAttr->value()));

        float fillOpacity = 1.0f;
        if (xml_attribute<>* fillOp = node->first_attribute("fill-opacity"))
            fillOpacity = stof(fillOp->value());

        if (xml_attribute<>* fill = node->first_attribute("fill")) {
            string colorStr = fill->value();
            if (colorStr[0] == '#') {
                int rr = stoi(colorStr.substr(1, 2), nullptr, 16);
                int gg = stoi(colorStr.substr(3, 2), nullptr, 16);
                int bb = stoi(colorStr.substr(5, 2), nullptr, 16);
                r.setColour(Colour(rr / 255.0f, gg / 255.0f, bb / 255.0f, fillOpacity));
            }
            else if (colorStr.substr(0, 4) == "rgb(") {
                string inner = colorStr.substr(4, colorStr.size() - 5);
                replace(inner.begin(), inner.end(), ',', ' ');
                istringstream iss(inner);
                int rr = 0, gg = 0, bb = 0;
                iss >> rr >> gg >> bb;
                r.setColour(Colour(rr / 255.0f, gg / 255.0f, bb / 255.0f, fillOpacity));
            }
        }

        if (xml_attribute<>* strokeWidthAttr = node->first_attribute("stroke-width")) {
            float strokeWidth = stof(strokeWidthAttr->value());
            r.setStrokeWidth(strokeWidth);
        }

        float strokeOpacity = 1.0f;
        if (xml_attribute<>* strokeOp = node->first_attribute("stroke-opacity"))
            strokeOpacity = stof(strokeOp->value());

        if (xml_attribute<>* strokeAttr = node->first_attribute("stroke")) {
            string strokeColor = strokeAttr->value();
            if (strokeColor[0] == '#') {
                int rr = stoi(strokeColor.substr(1, 2), nullptr, 16);
                int gg = stoi(strokeColor.substr(3, 2), nullptr, 16);
                int bb = stoi(strokeColor.substr(5, 2), nullptr, 16);
                r.setStrokeColour(Colour(rr / 255.0f, gg / 255.0f, bb / 255.0f, strokeOpacity));
            }
            else if (strokeColor.substr(0, 4) == "rgb(") {
                string inner = strokeColor.substr(4, strokeColor.size() - 5);
                replace(inner.begin(), inner.end(), ',', ' ');
                istringstream iss(inner);
                int rr = 0, gg = 0, bb = 0;
                iss >> rr >> gg >> bb;
                r.setStrokeColour(Colour(rr / 255.0f, gg / 255.0f, bb / 255.0f, strokeOpacity));
            }
        }

        rectangles.push_back(r);
    }

    return rectangles;
}

// Vẽ rectangle lên Graphics
void drawRectangle(Graphics* graphics, vector<Rectangle>& rectangles) {
    for (Rectangle& r : rectangles) {
        Colour colour = r.getColour();
        SolidBrush brush(Color(
            BYTE(colour.o * 255),
            BYTE(colour.r * 255),
            BYTE(colour.g * 255),
            BYTE(colour.b * 255)
        ));

        float x = r.getX();
        float y = r.getY();
        float w = r.getWidth();
        float h = r.getLength();

        graphics->FillRectangle(&brush, x, y, w, h);

        Colour stroke = r.getStrokeColour();
        float strokeWidth = r.getStrokeWidth();
        if (strokeWidth > 0) {
            Pen pen(Color(
                BYTE(stroke.o * 255),
                BYTE(stroke.r * 255),
                BYTE(stroke.g * 255),
                BYTE(stroke.b * 255)
            ), strokeWidth);
            graphics->DrawRectangle(&pen, x, y, w, h);
        }
    }
}
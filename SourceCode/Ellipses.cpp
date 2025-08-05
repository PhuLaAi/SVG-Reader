#include "Ellipses.h"

Ellipses::Ellipses() {
    rx = ry = 0;
    center.setX(0);
    center.setY(0);
}
Ellipses::~Ellipses() {
}

float Ellipses::getRX() {
    return rx;
}
float Ellipses::getRY() {
    return ry;
}
Point2D Ellipses::getCenter() {
    return center;
}
Colour Ellipses::getColour() {
    return fill;
}
float Ellipses::getStrokeWidth() {
    return stroke.getStrokeWidth();
}
Colour Ellipses::getStrokeColour() {
    return stroke.getStrokeColour();
}

void Ellipses::setRX(float x) {
    rx = x;
}
void Ellipses::setRY(float y) {
    ry = y;
}
void Ellipses::setCenter(float x, float y) {
    center.setX(x);
    center.setY(y);
}
void Ellipses::setColour(Colour color) {
    fill = color;
}
void Ellipses::setStrokeWidth(float w) {
    stroke.setStrokeWidth(w);
}
void Ellipses::setStrokeColour(Colour stkColour) {
    stroke.setStrokeColour(stkColour);
}

vector<Ellipses> parseEllipses(const string& filename) {
    vector<Ellipses> ellipses;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file\n";
        return ellipses;
    }

    string xmlContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    // Prepare a char buffer (RapidXML modifies the input)
    vector<char> buffer(xmlContent.begin(), xmlContent.end());
    buffer.push_back('\0');

    // PARSE using xml_document
    xml_document<> doc;
    doc.parse<0>(&buffer[0]);

    // Find svg node
    xml_node<>* svg = doc.first_node("svg");
    if (!svg) {
        cerr << "Unable to find <svg> node\n";
        return ellipses;
    }

    // Look for all circles
    for (xml_node<>* node = svg->first_node("ellipse"); node; node = node->next_sibling("ellipse")) {
        Ellipses e;

        if (xml_attribute<>* cx = node->first_attribute("cx")) {
            e.setCenter(stof(cx->value()), e.getCenter().getY());
        }

        if (xml_attribute<>* cy = node->first_attribute("cy")) {
            e.setCenter(e.getCenter().getX(), stof(cy->value()));
        }

        if (xml_attribute<>* rx = node->first_attribute("rx"))
            e.setRX(stof(rx->value()));
        if (xml_attribute<>* ry = node->first_attribute("ry"))
            e.setRY(stof(ry->value()));

        float fillOpacity = 1.0f;
        if (xml_attribute<>* fillOp = node->first_attribute("fill-opacity")) {
            fillOpacity = stof(fillOp->value());
        }
        if (xml_attribute<>* fill = node->first_attribute("fill")) {
            string colorStr = fill->value();

            if (colorStr[0] == '#') {
                int r = stoi(colorStr.substr(1, 2), nullptr, 16);
                int g = stoi(colorStr.substr(3, 2), nullptr, 16);
                int b = stoi(colorStr.substr(5, 2), nullptr, 16);
                e.setColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, fillOpacity));
            }
            else if (colorStr.substr(0, 4) == "rgb(") {

                string inner = colorStr.substr(4, colorStr.size() - 5);
                replace(inner.begin(), inner.end(), ',', ' ');
                istringstream iss(inner);
                int r = 0, g = 0, b = 0;
                iss >> r >> g >> b;
                e.setColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, fillOpacity));
            }
        }

        if (xml_attribute<>* strokeWidthAttr = node->first_attribute("stroke-width")) {
            float strokeWidth = stof(strokeWidthAttr->value());
            e.setStrokeWidth(strokeWidth);
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
                e.setStrokeColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, strokeOpacity));
            }
            else if (strokeColor.substr(0, 4) == "rgb(") {
                string inner = strokeColor.substr(4, strokeColor.size() - 5);
                replace(inner.begin(), inner.end(), ',', ' ');
                istringstream iss(inner);
                int r = 0, g = 0, b = 0;
                iss >> r >> g >> b;
                e.setStrokeColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, strokeOpacity));
            }
        }

        ellipses.push_back(e);
    }

    return ellipses;
}

void drawEllipses(Graphics* graphics, vector<Ellipses>& ellipses) {
    for (Ellipses& e : ellipses) {
        Colour fill = e.getColour();

        SolidBrush brush(Color(
            BYTE(fill.o * 255),
            BYTE(fill.r * 255),
            BYTE(fill.g * 255),
            BYTE(fill.b * 255)
        ));

        float x = e.getCenter().getX() - e.getRX();
        float y = e.getCenter().getY() - e.getRY();
        float w = e.getRX() * 2;
        float h = e.getRY() * 2;

        graphics->FillEllipse(&brush, x, y, w, h);

        Colour stroke = e.getStrokeColour();
        float strokeWidth = e.getStrokeWidth();

        if (strokeWidth > 0) {
            Pen pen(Color(
                BYTE(stroke.o * 255),
                BYTE(stroke.r * 255),
                BYTE(stroke.g * 255),
                BYTE(stroke.b * 255)
            ), strokeWidth);
            graphics->DrawEllipse(&pen, x, y, w, h);
        }
    }
}
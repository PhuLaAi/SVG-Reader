#include "Polylines.h" 

Polylines::Polylines() {}

Polylines::~Polylines() {}

vector<Point2D>& Polylines::getPoints() {
    return points;
}

Colour Polylines::getColour() {
    return fill;
}

float Polylines::getStrokeWidth() {
    return stroke.getStrokeWidth();
}

Colour Polylines::getStrokeColour() {
    return stroke.getStrokeColour();
}

void Polylines::setPoints(vector<Point2D>& pts) {
    points = pts;
}

void Polylines::addPoint(Point2D& pt) {
    points.push_back(pt);
}

void Polylines::setColour(Colour colour) {
    fill = colour;
}

void Polylines::setStrokeWidth(float w) {
    stroke.setStrokeWidth(w);
}

void Polylines::setStrokeColour(Colour StkColour) {
    stroke.setStrokeColour(StkColour);
}

vector<Polylines> parsePolylines(const string& filename) {
    vector<Polylines> polylines;

    ifstream file(filename);
    if (!file.is_open()) {
        return polylines;
    }

    string xmlContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    vector<char> buffer(xmlContent.begin(), xmlContent.end());
    buffer.push_back('\0');

    xml_document<> doc;
    doc.parse<0>(&buffer[0]);

    xml_node<>* svg = doc.first_node("svg");
    if (!svg) {
        return polylines;
    }

    for (xml_node<>* node = svg->first_node("polyline"); node; node = node->next_sibling("polyline")) {
        Polylines pl;

        // Parse points
        if (xml_attribute<>* pointsAttr = node->first_attribute("points")) {
            string pointsStr = pointsAttr->value();
            istringstream iss(pointsStr);
            string token;
            while (getline(iss, token, ' ')) {
                size_t comma = token.find(',');
                if (comma != string::npos) {
                    float x = stof(token.substr(0, comma));
                    float y = stof(token.substr(comma + 1));
                    Point2D pt(x, y);
                    pl.addPoint(pt);
                }
            }
        }

        // === Fill ===
        float fillOpacity = 1.0f;
        if (xml_attribute<>* fillOp = node->first_attribute("fill-opacity"))
            fillOpacity = stof(fillOp->value());

        bool hasFill = false;
        if (xml_attribute<>* fill = node->first_attribute("fill")) {
            string colorStr = fill->value();
            if (colorStr[0] == '#') {
                int r = stoi(colorStr.substr(1, 2), nullptr, 16);
                int g = stoi(colorStr.substr(3, 2), nullptr, 16);
                int b = stoi(colorStr.substr(5, 2), nullptr, 16);
                pl.setColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, fillOpacity));
                hasFill = true;
            }
            else if (colorStr.substr(0, 4) == "rgb(") {
                string inner = colorStr.substr(4, colorStr.size() - 5);
                replace(inner.begin(), inner.end(), ',', ' ');
                istringstream iss(inner);
                int r, g, b;
                iss >> r >> g >> b;
                pl.setColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, fillOpacity));
                hasFill = true;
            }
        }

        if (!hasFill && fillOpacity < 1.0f) {
            pl.setColour(Colour(0.5f, 0.5f, 0.5f, fillOpacity));
        }
        float strokeOpacity = 1.0f;
        if (xml_attribute<>* strokeOp = node->first_attribute("stroke-opacity"))
            strokeOpacity = stof(strokeOp->value());

        bool hasStroke = false;
        if (xml_attribute<>* strokeAttr = node->first_attribute("stroke")) {
            string strokeColor = strokeAttr->value();
            if (strokeColor[0] == '#') {
                int r = stoi(strokeColor.substr(1, 2), nullptr, 16);
                int g = stoi(strokeColor.substr(3, 2), nullptr, 16);
                int b = stoi(strokeColor.substr(5, 2), nullptr, 16);
                pl.setStrokeColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, strokeOpacity));
                hasStroke = true;
            }
            else if (strokeColor.substr(0, 4) == "rgb(") {
                string inner = strokeColor.substr(4, strokeColor.size() - 5);
                replace(inner.begin(), inner.end(), ',', ' ');
                istringstream iss(inner);
                int r, g, b;
                iss >> r >> g >> b;
                pl.setStrokeColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, strokeOpacity));
                hasStroke = true;
            }
        }
        if (!hasStroke && fillOpacity > 0.0f) {
            pl.setStrokeColour(pl.getColour());
            pl.setStrokeWidth(1.0f);
        }

        if (!hasStroke && strokeOpacity < 1.0f) {
            pl.setStrokeColour(Colour(0.3f, 0.3f, 0.3f, strokeOpacity));
        }

        if (xml_attribute<>* strokeWidthAttr = node->first_attribute("stroke-width")) {
            float strokeWidth = stof(strokeWidthAttr->value());
            pl.setStrokeWidth(strokeWidth);
        }

        polylines.push_back(pl);
    }

    return polylines;
}


void drawPolylines(Graphics* graphics, vector<Polylines>& polylines) {
    for (Polylines& poly : polylines) {
        vector<Point2D>& points = poly.getPoints();
        if (points.size() < 2) continue;

        vector<PointF> GDIPoints;
        GDIPoints.reserve(points.size());
        for (Point2D& p : points) {
            GDIPoints.emplace_back(p.getX(), p.getY());
        }
        auto clamp = [](float v) {
            return max(0.0f, min(1.0f, v));
            };

        Colour fill = poly.getColour();
        if (clamp(fill.o) > 0.01f && GDIPoints.size() >= 3) {
            SolidBrush brush(Color(
                BYTE(clamp(fill.o) * 255),
                BYTE(clamp(fill.r) * 255),
                BYTE(clamp(fill.g) * 255),
                BYTE(clamp(fill.b) * 255)
            ));
            graphics->FillPolygon(&brush, GDIPoints.data(), GDIPoints.size());
        }
        Colour stroke = poly.getStrokeColour();
        float strokeWidth = poly.getStrokeWidth();
        if (clamp(stroke.o) > 0.01f && strokeWidth > 0.1f) {
            Pen pen(Color(
                BYTE(clamp(stroke.o) * 255),
                BYTE(clamp(stroke.r) * 255),
                BYTE(clamp(stroke.g) * 255),
                BYTE(clamp(stroke.b) * 255)
            ), strokeWidth);
            graphics->DrawLines(&pen, GDIPoints.data(), GDIPoints.size());
        }
    }
}

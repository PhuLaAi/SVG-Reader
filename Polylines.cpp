#include "Polylines.h"

Polylines::Polylines() {
}
Polylines::~Polylines() {
}

vector<Point2D> Polylines::getPoints() {
	return this->points;
}
float Polylines::getStrokeWidth() {
	return stroke.getStrokeWidth();
}
Colour Polylines::getStrokeColour() {
	return stroke.getStrokeColour();
}
Colour Polylines::getColour() {
    return this->fill;
}
void Polylines::setPoints(vector<Point2D> a) {
	points = a;
}
void Polylines::setStrokeWidth(float w) {
	stroke.setStrokeWidth(w);
}
void Polylines::setStrokeColour(Colour StkColour) {
	stroke.setStrokeColour(StkColour);
}
void Polylines::setColour(Colour colour) {
    fill = colour;
}

vector<Polylines> parsePolylines(const string& filename) {
    vector<Polylines> polylines;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file\n";
        return polylines;
    }

    string xmlContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    vector<char> buffer(xmlContent.begin(), xmlContent.end());
    buffer.push_back('\0');

    xml_document<> doc;
    doc.parse<0>(&buffer[0]);

    xml_node<>* svg = doc.first_node("svg");
    if (!svg) {
        cerr << "Unable to find <svg> node\n";
        return polylines;
    }

    for (xml_node<>* node = svg->first_node("polyline"); node; node = node->next_sibling("polyline")) {
        Polylines poly;
        float strokeOpacity = 1.0f;

        if (xml_attribute<>* strokeWidthAttr = node->first_attribute("stroke-width")) {
            poly.setStrokeWidth(stof(strokeWidthAttr->value()));
        }
        else {
            poly.setStrokeWidth(1.0f);
        }

        if (xml_attribute<>* strokeOp = node->first_attribute("stroke-opacity")) {
            strokeOpacity = stof(strokeOp->value());
        }

        if (xml_attribute<>* strokeAttr = node->first_attribute("stroke")) {
            string strokeColor = strokeAttr->value();
            if (strokeColor.substr(0, 4) == "rgb(") {
                string inner = strokeColor.substr(4, strokeColor.size() - 5);
                replace(inner.begin(), inner.end(), ',', ' ');
                istringstream iss(inner);
                int r, g, b;
                iss >> r >> g >> b;
                poly.setStrokeColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, strokeOpacity));
            }
        }
        else {
            poly.setStrokeColour(Colour(0, 0, 0, 1.0f));
        }
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
                poly.setColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, fillOpacity));
            }
            else if (colorStr.substr(0, 4) == "rgb(") {

                string inner = colorStr.substr(4, colorStr.size() - 5);
                replace(inner.begin(), inner.end(), ',', ' ');
                istringstream iss(inner);
                int r = 0, g = 0, b = 0;
                iss >> r >> g >> b;
                poly.setColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, fillOpacity));
            }
        }
        // Edit
        else {
            if (fillOpacity < 1.0f) {
                poly.setColour(Colour(0.0f, 0.0f, 0.0f, fillOpacity));
            }
            else {
                poly.setColour(Colour(0.0f, 0.0f, 0.0f, 0.0f));  // fully transparent
            }
        }
        if (xml_attribute<>* pointsAttr = node->first_attribute("points")) {
            string pointsStr = pointsAttr->value();
            replace(pointsStr.begin(), pointsStr.end(), ',', ' ');
            istringstream iss(pointsStr);
            vector<Point2D> points;
            float x, y;
            while (iss >> x >> y) {
                Point2D p;
                p.setX(x);
                p.setY(y);
                points.push_back(p);
            }
            poly.setPoints(points);
        }

        polylines.push_back(poly);
    }
    return polylines;
}
void drawPolylines(Graphics* graphics, vector<Polylines>& polylines) {
    for (Polylines& poly : polylines) {
        vector<Point2D> point = poly.getPoints();
        if (point.size() < 2) {
            continue;
        }

        vector<PointF> GDIPoints;
        for (Point2D& p : point) {
            GDIPoints.push_back(PointF(p.getX(), p.getY()));
        }
        // Edit
        PointF first = GDIPoints.front();
        PointF last = GDIPoints.back();

        if (first.X != last.X || first.Y != last.Y) {
            GDIPoints.push_back(first);
        }
        
        Colour fill = poly.getColour();
        if (fill.o > 0.01f) {
            SolidBrush brush(Color(
                BYTE(fill.o * 255),
                BYTE(fill.r * 255),
                BYTE(fill.g * 255),
                BYTE(fill.b * 255)
            ));

            // Fill for closable/closed shapes only
            if (GDIPoints.size() >= 3) {
                graphics->FillPolygon(&brush, GDIPoints.data(), GDIPoints.size());
            }
        }
        

        Colour stroke = poly.getStrokeColour();
        float strokeWidth = poly.getStrokeWidth();

        Pen pen(Color(
            BYTE(stroke.o * 255),
            BYTE(stroke.r * 255),
            BYTE(stroke.g * 255),
            BYTE(stroke.b * 255)
        ), strokeWidth);

        graphics->DrawLines(&pen, GDIPoints.data(), GDIPoints.size());
    }
}

#include "Line.h"

Line::Line() {
	start.setX(0);
	start.setY(0);
	end.setX(0);
	end.setY(0);
}
Line::~Line() {
}

Point2D Line::getStart() {
	start.getX();
	start.getY();
	return start;
}
Point2D Line::getEnd() {
	end.getX();
	end.getY();
	return end;
}
float Line::getStrokeWidth() {
	return stroke.getStrokeWidth();
}
Colour Line::getStrokeColour() {
	return stroke.getStrokeColour();
}

void Line::setStart(Point2D a) {
	start = a;
}
void Line::setEnd(Point2D b) {
	end = b;
}
void Line::setStrokeWidth(float w) {
	stroke.setStrokeWidth(w);
}
void Line::setStrokeColour(Colour StkColour) {
	stroke.setStrokeColour(StkColour);
}

vector<Line> parseLine(const string& filename) {
    vector<Line> lines;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file\n";
        return lines;
    }

    string xmlContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    vector<char> buffer(xmlContent.begin(), xmlContent.end());
    buffer.push_back('\0');

    xml_document<> doc;
    doc.parse<0>(&buffer[0]);

    xml_node<>* svg = doc.first_node("svg");
    if (!svg) {
        cerr << "Unable to find <svg> node\n";
        return lines;
    }

    for (xml_node<>* node = svg->first_node("line"); node; node = node->next_sibling("line")) {
        Line l;
        float strokeOpacity = 1.0f;

        Point2D start, end;
        if (auto x1 = node->first_attribute("x1")) start.setX(stof(x1->value()));
        if (auto y1 = node->first_attribute("y1")) start.setY(stof(y1->value()));
        if (auto x2 = node->first_attribute("x2")) end.setX(stof(x2->value()));
        if (auto y2 = node->first_attribute("y2")) end.setY(stof(y2->value()));
        l.setStart(start);
        l.setEnd(end);

        if (xml_attribute<>* strokeWidthAttr = node->first_attribute("stroke-width")) {
            l.setStrokeWidth(stof(strokeWidthAttr->value()));
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
                l.setStrokeColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, strokeOpacity));
            }
        }

        lines.push_back(l);
    }

    return lines;
}
void drawLine(Graphics* graphics, vector<Line>& line) {
    for (Line& l : line) {
        Colour stroke = l.getStrokeColour();
        float strokeWidth = l.getStrokeWidth();

        Pen pen(Color(
            BYTE(stroke.o * 255),
            BYTE(stroke.r * 255),
            BYTE(stroke.g * 255),
            BYTE(stroke.b * 255)
        ), strokeWidth);

        float x1 = l.getStart().getX();
        float y1 = l.getStart().getY();
        float x2 = l.getEnd().getX();
        float y2 = l.getEnd().getY();

        graphics->DrawLine(&pen, x1, y1, x2, y2);
    }
}

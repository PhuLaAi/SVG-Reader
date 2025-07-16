#include "Circle.h"

Circle::Circle() {
	rx = ry = 0;
	center.setX(0);
	center.setY(0);
}
Circle::~Circle() {

}

float Circle::getRX() {
	return this->rx;
}
float Circle::getRY() {
	return this->ry;
}
Point2D Circle::getCenter() {
	return this->center;
}
Colour Circle::getColour() {
	return this->fill;
}
float Circle::getStrokeWidth() {
    return stroke.getStrokeWidth();
}
Colour Circle::getStrokeColour() {
    return stroke.getStrokeColour();
}

void Circle::setRX(float x) {
	rx = x;
}
void Circle::setRY(float y) {
	ry = y;
}
void Circle::setCenter(float x, float y) {
	center.setX(x);
	center.setY(y);
}
void Circle::setColour(Colour color) {
	fill = color;
}
void Circle::setStrokeWidth(float w) {
    stroke.setStrokeWidth(w);
}
void Circle::setStrokeColour(Colour StkColour) {
    stroke.setStrokeColour(StkColour);
}

vector<Circle> parseCircle(const string& filename) {
    vector<Circle> circles;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file\n";
        return circles;
    }

    string xmlContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    // Prepare a char buffer
    vector<char> buffer(xmlContent.begin(), xmlContent.end());
    buffer.push_back('\0');

    // PARSE using xml_document
    xml_document<> doc;          
    doc.parse<0>(&buffer[0]);    

    // Find svg node
    xml_node<>* svg = doc.first_node("svg");
    if (!svg) {
        cerr << "Unable to find <svg> node\n";
        return circles;
    }

    // Look for all circles
    for (xml_node<>* node = svg->first_node("circle"); node; node = node->next_sibling("circle")) {
        Circle c;
	// getting values of each circle's attributes
	// 1. location and radius
        if (xml_attribute<>* cx = node->first_attribute("cx")) {
            c.setCenter(stof(cx->value()), c.getCenter().getY());
        }

        if (xml_attribute<>* cy = node->first_attribute("cy")) {
            c.setCenter(c.getCenter().getX(), stof(cy->value()));
        }

        if (xml_attribute<>* r = node->first_attribute("r")) {
            float radius = stof(r->value());
            c.setRX(radius);
            c.setRY(radius);
        }
	// 2. color fill
        float fillOpacity = 1.0f;
        if (xml_attribute<>* fillOp = node->first_attribute("fill-opacity"))
            fillOpacity = stof(fillOp->value());

        if (xml_attribute<>* fill = node->first_attribute("fill")) {
            string colorStr = fill->value();

            if (colorStr[0] == '#') {
                int r = stoi(colorStr.substr(1, 2), nullptr, 16);
                int g = stoi(colorStr.substr(3, 2), nullptr, 16);
                int b = stoi(colorStr.substr(5, 2), nullptr, 16);
                c.setColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, fillOpacity));
            }
            else if (colorStr.substr(0, 4) == "rgb(") {
                string inner = colorStr.substr(4, colorStr.size() - 5);
                replace(inner.begin(), inner.end(), ',', ' ');
                istringstream iss(inner);
                int r = 0, g = 0, b = 0;
                iss >> r >> g >> b;
                c.setColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, fillOpacity));
            }
        }
	// 3. stroke color and sizesize
        if (xml_attribute<>* strokeWidthAttr = node->first_attribute("stroke-width")) {
            float strokeWidth = stof(strokeWidthAttr->value());
            c.setStrokeWidth(strokeWidth);
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
                c.setStrokeColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, strokeOpacity));
            }
            else if (strokeColor.substr(0, 4) == "rgb(") {
                string inner = strokeColor.substr(4, strokeColor.size() - 5);
                replace(inner.begin(), inner.end(), ',', ' ');
                istringstream iss(inner);
                int r = 0, g = 0, b = 0;
                iss >> r >> g >> b;
                c.setStrokeColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, strokeOpacity));
            }
        }

        circles.push_back(c);
    }

    return circles;
}

void drawCircle(Graphics* graphics, vector<Circle>& circles) {
    for (Circle& c : circles) {
        Colour colour = c.getColour();
	// draws inner circle
        SolidBrush brush(Color(
            BYTE(colour.o * 255),  
            BYTE(colour.r * 255),  
            BYTE(colour.g * 255), 
            BYTE(colour.b * 255)   
        ));

        float x = c.getCenter().getX() - c.getRX();
        float y = c.getCenter().getY() - c.getRY();
        float w = c.getRX() * 2;
        float h = c.getRY() * 2;

        graphics->FillEllipse(&brush, x, y, w, h);

        Colour stroke = c.getStrokeColour();
        float strokeWidth = c.getStrokeWidth();
	// draws outer circle
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        Graphics graphics(hdc);
        graphics.Clear(Color(255, 255, 255));
        vector<Circle>* circles = (vector<Circle>*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        drawCircle(&graphics, *circles);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

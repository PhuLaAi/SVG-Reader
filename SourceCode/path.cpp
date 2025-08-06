#include "path.h"

using namespace std;

pathShape::pathShape() {}

void pathShape::loadFromXML(xml_node<>* node) {
    Shape::loadFromXML(node);
    d = node->first_attribute("d") ? node->first_attribute("d")->value() : "";
}

void pathShape::draw(Graphics& g) {
    GraphicsPath path;
    parsePathData(d, path);

    if (hasFill) {
        SolidBrush brush(fillColor);
        g.FillPath(&brush, &path);
    }
    if (hasStroke) {
        Pen pen(strokeColor, strokeWidth);
        g.DrawPath(&pen, &path);
    }
}



void parsePathData(const string& d, GraphicsPath& path) {
    vector<string> tokens = tokenizePath(d);
    PointF current(0, 0), start(0, 0);
    char cmd = 0;

    size_t i = 0;
    while (i < tokens.size()) {
        if (isalpha(tokens[i][0])) cmd = tokens[i++][0];
        bool relative = islower(cmd);

        if (cmd == 'M' || cmd == 'm') {
            float x = parseFloat(tokens[i++].c_str());
            float y = parseFloat(tokens[i++].c_str());
            current = PointF(relative ? current.X + x : x, relative ? current.Y + y : y);
            path.StartFigure();
            start = current;
        } else if (cmd == 'L' || cmd == 'l') {
            float x = parseFloat(tokens[i++].c_str());
            float y = parseFloat(tokens[i++].c_str());
            PointF p(relative ? current.X + x : x, relative ? current.Y + y : y);
            path.AddLine(current, p);
            current = p;
        } else if (cmd == 'H' || cmd == 'h') {
            float dx = parseFloat(tokens[i++].c_str());
            float x = relative ? current.X + dx : dx;
            path.AddLine(current, PointF(x, current.Y));
            current = PointF(x, current.Y);
        } else if (cmd == 'V' || cmd == 'v') {
            float dy = parseFloat(tokens[i++].c_str());
            float y = relative ? current.Y + dy : dy;
            path.AddLine(current, PointF(current.X, y));
            current = PointF(current.X, y);
        } else if (cmd == 'C' || cmd == 'c') {
            float dx1 = parseFloat(tokens[i++].c_str());
            float dy1 = parseFloat(tokens[i++].c_str());
            float dx2 = parseFloat(tokens[i++].c_str());
            float dy2 = parseFloat(tokens[i++].c_str());
            float dx = parseFloat(tokens[i++].c_str());
            float dy = parseFloat(tokens[i++].c_str());

            PointF p1 = relative ? PointF(current.X + dx1, current.Y + dy1) : PointF(dx1, dy1);
            PointF p2 = relative ? PointF(current.X + dx2, current.Y + dy2) : PointF(dx2, dy2);
            PointF p3 = relative ? PointF(current.X + dx, current.Y + dy) : PointF(dx, dy);

            path.AddBezier(current, p1, p2, p3);
            current = p3;
        } else if (cmd == 'L' || cmd == 'l') {
            float x = parseFloat(tokens[i++].c_str());
            float y = parseFloat(tokens[i++].c_str());
            PointF p(relative ? current.X + x : x, relative ? current.Y + y : y);
            path.AddLine(current, p);
            current = p;
        } else if (cmd == 'Z' || cmd == 'z') {
            path.CloseFigure();
            current = start;
        } else {
            ++i;
        }
    }
}

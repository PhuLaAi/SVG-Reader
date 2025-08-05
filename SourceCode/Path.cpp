#include "Path.h"
#include <regex>

Path::Path() {}
Path::~Path() {}

void Path::addCommand(char cmd, const vector<Point2D>& pts) {
    commands.push_back(cmd);
    points.insert(points.end(), pts.begin(), pts.end());
}

vector<Point2D>& Path::getPoints() { return points; }
vector<char>& Path::getCommands() { return commands; }

Colour Path::getColour() { return fill; }
float Path::getStrokeWidth() { return stroke.getStrokeWidth(); }
Colour Path::getStrokeColour() { return stroke.getStrokeColour(); }

void Path::setColour(Colour c) { fill = c; }
void Path::setStrokeWidth(float w) { stroke.setStrokeWidth(w); }
void Path::setStrokeColour(Colour c) { stroke.setStrokeColour(c); }

string normalizePathData(const string& d_raw) {
    string result;
    for (size_t i = 0; i < d_raw.size(); ++i) {
        char c = d_raw[i];
        if (isalpha(c)) {
            if (!result.empty() && result.back() != ' ')
                result += ' ';
            result += c;
            if (i + 1 < d_raw.size() && d_raw[i + 1] != ' ')
                result += ' ';
        }
        else if (c == '-' && i > 0 && (isdigit(d_raw[i + 1]) || d_raw[i + 1] == '.')) {
            if (result.back() != ' ')
                result += ' ';
            result += c;
        }
        else if (c == ',') {
            result += ' ';
        }
        else {
            result += c;
        }
    }
    return result;
}
vector<string> tokenizePathData(const string& d) {
    vector<string> tokens;
    size_t i = 0;
    while (i < d.size()) {
        char c = d[i];
        if (isspace(c) || c == ',') {
            i++;
            continue;
        }
        if (isalpha(c)) {
            tokens.emplace_back(1, c);
            ++i;
        }
        else if (isdigit(c) || c == '-' || c == '+' || c == '.') {
            size_t start = i;
            bool dotSeen = false;
            if (c == '-' || c == '+') ++i;
            while (i < d.size() && (isdigit(d[i]) || d[i] == '.')) {
                if (d[i] == '.') {
                    if (dotSeen) break;
                    dotSeen = true;
                }
                ++i;
            }
            tokens.push_back(d.substr(start, i - start));
        }
        else {
            ++i; 
        }
    }
    return tokens;
}
vector<Path> parsePaths(const string& filename) {
    vector<Path> paths;

    ifstream file(filename);
    if (!file.is_open()) return paths;

    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    vector<char> buffer(content.begin(), content.end());
    buffer.push_back('\0');

    xml_document<> doc;
    doc.parse<0>(&buffer[0]);

    xml_node<>* svg = doc.first_node("svg");
    if (!svg) return paths;

    for (xml_node<>* node = svg->first_node("path"); node; node = node->next_sibling("path")) {
        Path path;

        string d_raw = node->first_attribute("d")->value();
        string d = normalizePathData(d_raw);
        auto tokens = tokenizePathData(d);
        Point2D current(0, 0), start(0, 0);
        size_t i = 0;

        while (i < tokens.size()) {
            string token = tokens[i++];

            char cmd = token[0];

            if (cmd == 'M' || cmd == 'L') {
                float x = stof(tokens[i++]);
                float y = stof(tokens[i++]);
                Point2D pt(x, y);
                path.addCommand(cmd, { pt });
                current = pt;
                if (cmd == 'M') start = pt;
            }
            else if (cmd == 'H') {
                float x = stof(tokens[i++]);
                current = Point2D(x, current.getY());
                path.addCommand('L', { current });
            }
            else if (cmd == 'V') {
                float y = stof(tokens[i++]);
                current = Point2D(current.getX(), y);
                path.addCommand('L', { current });
            }
            else if (cmd == 'C') {
                float x1 = stof(tokens[i++]), y1 = stof(tokens[i++]);
                float x2 = stof(tokens[i++]), y2 = stof(tokens[i++]);
                float x = stof(tokens[i++]), y = stof(tokens[i++]);
                path.addCommand('C', { Point2D(x1, y1), Point2D(x2, y2), Point2D(x, y) });
                current = Point2D(x, y);
            }
            else if (cmd == 'Z' || cmd == 'z') {
                path.addCommand('Z', {});
                current = start;
            }
        }
        float fillOpacity = 1.0f;
        if (xml_attribute<>* fillOp = node->first_attribute("fill-opacity"))
            fillOpacity = stof(fillOp->value());

        bool hasFill = false;
        if (xml_attribute<>* fillAttr = node->first_attribute("fill")) {
            string colorStr = fillAttr->value();

            if (colorStr == "none") {
                path.setColour(Colour(0, 0, 0, 0));
                hasFill = true;
            }
            else if (colorStr[0] == '#') {
                int r = stoi(colorStr.substr(1, 2), nullptr, 16);
                int g = stoi(colorStr.substr(3, 2), nullptr, 16);
                int b = stoi(colorStr.substr(5, 2), nullptr, 16);
                path.setColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, fillOpacity));
                hasFill = true;
            }
            else if (colorStr.substr(0, 4) == "rgb(") {
                string inner = colorStr.substr(4, colorStr.size() - 5);
                replace(inner.begin(), inner.end(), ',', ' ');
                istringstream iss(inner);
                int r, g, b;
                iss >> r >> g >> b;
                path.setColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, fillOpacity));
                hasFill = true;
            }
            else {
                string key = colorStr;
                key.erase(remove_if(key.begin(), key.end(), ::isspace), key.end());
                transform(key.begin(), key.end(), key.begin(), ::tolower);

                Colour c = ColourManager::getColourByName(key, fillOpacity);
                path.setColour(c);
                hasFill = true;
            }
        }

        if (!hasFill && fillOpacity < 1.0f) {
            path.setColour(Colour(0.5f, 0.5f, 0.5f, fillOpacity));
        }
        float strokeOpacity = 1.0f;
        if (xml_attribute<>* strokeOp = node->first_attribute("stroke-opacity"))
            strokeOpacity = stof(strokeOp->value());

        bool hasStroke = false;
        if (xml_attribute<>* strokeAttr = node->first_attribute("stroke")) {
            string colorStr = strokeAttr->value();
            float opacity = strokeOpacity;

            if (colorStr == "none") {
                path.setStrokeColour(Colour(0, 0, 0, 0));
                hasStroke = true;
            }
            else if (colorStr[0] == '#') {
                int r = stoi(colorStr.substr(1, 2), nullptr, 16);
                int g = stoi(colorStr.substr(3, 2), nullptr, 16);
                int b = stoi(colorStr.substr(5, 2), nullptr, 16);
                path.setStrokeColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, opacity));
                hasStroke = true;
            }
            else if (colorStr.substr(0, 4) == "rgb(") {
                string inner = colorStr.substr(4, colorStr.size() - 5);
                replace(inner.begin(), inner.end(), ',', ' ');
                istringstream iss(inner);
                int r, g, b;
                iss >> r >> g >> b;
                path.setStrokeColour(Colour(r / 255.0f, g / 255.0f, b / 255.0f, opacity));
                hasStroke = true;
            }
            else {
               
                string key = colorStr;
                key.erase(remove_if(key.begin(), key.end(), ::isspace), key.end());
                transform(key.begin(), key.end(), key.begin(), ::tolower);

                Colour c = ColourManager::getColourByName(key, strokeOpacity);
                path.setStrokeColour(c);
                hasStroke = true;
            }
        }
        if (!hasStroke && fillOpacity > 0.0f) {
            path.setStrokeColour(path.getColour());
            path.setStrokeWidth(1.0f);
        }

        if (!hasStroke && strokeOpacity < 1.0f) {
            path.setStrokeColour(Colour(0.3f, 0.3f, 0.3f, strokeOpacity));
        }

        if (xml_attribute<>* strokeWidthAttr = node->first_attribute("stroke-width")) {
            float strokeWidth = stof(strokeWidthAttr->value());
            path.setStrokeWidth(strokeWidth);
        }

        paths.push_back(path);
    }

    return paths;
}

void drawPaths(Graphics* g, vector<Path>& paths) {
    for (auto& path : paths) {
        GraphicsPath gp;
        auto pts = path.getPoints();
        auto cmds = path.getCommands();
        Point2D current(0, 0), start(0, 0);
        size_t i = 0;

        for (char cmd : cmds) {
            if (cmd == 'M') {
                Point2D p = pts[i++];
                gp.StartFigure();
                current = p;
                start = p;
            }
            else if (cmd == 'L') {
                Point2D p = pts[i++];
                gp.AddLine(PointF(current.getX(), current.getY()), PointF(p.getX(), p.getY()));
                current = p;
            }
            else if (cmd == 'C') {
                Point2D c1 = pts[i++], c2 = pts[i++], end = pts[i++];
                gp.AddBezier(PointF(current.getX(), current.getY()), PointF(c1.getX(), c1.getY()),
                    PointF(c2.getX(), c2.getY()), PointF(end.getX(), end.getY()));
                current = end;
            }
            else if (cmd == 'Z') {
                gp.CloseFigure();
                current = start;
            }
        }

        auto clamp = [](float v) {
            return max(0.0f, min(1.0f, v));
            };

        Colour fill = path.getColour();
        if (clamp(fill.o) > 0.01f) {
            SolidBrush brush(Color(
                BYTE(clamp(fill.o) * 255),
                BYTE(clamp(fill.r) * 255),
                BYTE(clamp(fill.g) * 255),
                BYTE(clamp(fill.b) * 255)
            ));
            g->FillPath(&brush, &gp);
        }

        Colour stroke = path.getStrokeColour();
        float strokeWidth = path.getStrokeWidth();
        if (clamp(stroke.o) > 0.01f && strokeWidth > 0.1f) {
            Pen pen(Color(
                BYTE(clamp(stroke.o) * 255),
                BYTE(clamp(stroke.r) * 255),
                BYTE(clamp(stroke.g) * 255),
                BYTE(clamp(stroke.b) * 255)
            ), strokeWidth);
            g->DrawPath(&pen, &gp);
        }
    }
}

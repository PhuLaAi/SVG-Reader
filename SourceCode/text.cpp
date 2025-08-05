#include "text.h"

text::text() {
    fontSize = dx = dy = 0;
    textPos.setX(0);
    textPos.setY(0);
    content = "";
    fontFamily = "Times New Roman";
    textAnchor = "start";
    fontStyle = "normal";
}

text::~text() {}

point2D text::getTextPos() { return textPos; }
float text::getDx() { return dx; }
float text::getDy() { return dy; }
float text::getFontSize() { return fontSize; }
string text::getFontFamily() { return fontFamily; }
string text::getFontStyle() { return fontStyle; }
string text::getTextAnchor() { return textAnchor; }
string& text::getContent() { return content; }
Colour text::getColour() { return fill; }
float text::getStrokeWidth() { return stroke.getStrokeWidth(); }
Colour text::getStrokeColour() { return stroke.getStrokeColour(); }

void text::setTextPos(float x, float y) { textPos.setX(x); textPos.setY(y); }
void text::setDx(float dx) { this->dx = dx; }
void text::setDy(float dy) { this->dy = dy; }
void text::setFontSize(float size) { fontSize = size; }
void text::setFontFamily(string family) { fontFamily = family; }
void text::setFontStyle(string style) { fontStyle = style; }
void text::setTextAnchor(string anchor) { textAnchor = anchor; }
void text::setContent(string text) { content = text; }
void text::setColour(Colour colour) { fill = colour; }
void text::setStrokeWidth(float w) { stroke.setStrokeWidth(w); }
void text::setStrokeColour(Colour StkColour) { stroke.setStrokeColour(StkColour); }

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

    for (xml_node<>* node = svg->first_node("text"); node; node = node->next_sibling("text")) {
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

        if (xml_attribute<>* fs = node->first_attribute("font-style"))
            t.setFontStyle(fs->value());

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
        if (content.empty() && node->first_node()) {
            content = node->first_node()->value();
        }
        t.setContent(content);

        texts.push_back(t);
    }

    return texts;
}

void drawText(Graphics* graphics, vector<text>& texts) {
    for (text& t : texts) {
        Colour col = t.getColour();
        SolidBrush brush(Color(BYTE(col.o * 255), BYTE(col.r * 255), BYTE(col.g * 255), BYTE(col.b * 255)));

        float x = t.getTextPos().getX() + t.getDx();
        float y = t.getTextPos().getY() + t.getDy();

        FontFamily fontFamily(L"Times");
        Font font(&fontFamily, t.getFontSize(), FontStyleRegular, UnitPixel);
        PointF point(x, y);
        string& content = t.getContent();
        wstring wcontent(content.begin(), content.end());
        graphics->DrawString(wcontent.c_str(), -1, &font, point, &brush);
    }
}


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
Color text::getColour() { return fill; }
float text::getStrokeWidth() { return strokeWidth; }
Color text::getStrokeColour() { return strokeColor; }
string text::getFillStr() { return fillStr; }
string text::getStrokeStr() {return strokeStr;}
void text::setTextPos(float x, float y) { textPos.setX(x); textPos.setY(y); }
void text::setDx(float dx) { this->dx = dx; }
void text::setDy(float dy) { this->dy = dy; }
void text::setFontSize(float size) { fontSize = size; }
void text::setFontFamily(string family) { fontFamily = family; }
void text::setFontStyle(string style) { fontStyle = style; }
void text::setTextAnchor(string anchor) { textAnchor = anchor; }
void text::setContent(string text) { content = text; }
void text::setColour(Color colour) { fill = colour; }
void text::setStrokeWidth(float w) { strokeWidth = w; }
void text::setStrokeColour(Color StkColour) {strokeColor = StkColour; }
void text::setFillStr(const std::string& str) { fillStr = str; }
void text::setStrokeStr(const std::string& str) {strokeStr = str;}

void drawText(Graphics* graphics, vector<text>& texts) {
    for (text& t : texts) {
        string colourStr = t.getFillStr();
        float opacity = t.getColour().GetAlpha() / 255.0f;
        Color color = parseColourString(colourStr, opacity);
        SolidBrush brush(color);

        float x = t.getTextPos().getX() + t.getDx();
        float y = t.getTextPos().getY() + t.getDy();
        PointF point(x, y);

        string familyStr = t.getFontFamily();
        if (familyStr.empty()) familyStr = "Times New Roman";
        wstring fontName(familyStr.begin(), familyStr.end());
        FontFamily fontFamily(fontName.c_str());

        Font font(&fontFamily, t.getFontSize(), FontStyleRegular, UnitPixel);

        string& content = t.getContent();
        wstring wcontent(content.begin(), content.end());

        GraphicsPath path;
        path.AddString(wcontent.c_str(), -1, &fontFamily, FontStyleRegular, t.getFontSize(), point, nullptr);

        // Fill
        if (t.getColour().GetA() > 0) {
            SolidBrush fillBrush(t.getColour());
            graphics->FillPath(&fillBrush, &path);
        }

        // Stroke
        if (t.getStrokeColour().GetA() > 0 && t.getStrokeWidth() > 0) {
            Pen pen(t.getStrokeColour(), t.getStrokeWidth());
            pen.SetLineJoin(LineJoinRound);
            graphics->DrawPath(&pen, &path);
        }
    }
}

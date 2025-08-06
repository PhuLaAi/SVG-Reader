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



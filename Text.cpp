#include "Text.h"
#include <fstream>
#include <sstream>

using namespace std;

Text::Text() {
	fontSize = 12.0f;
	dx = dy = 0.0f;
	textPos = Point2D(0, 0);
	content = "";
	fontFamily = "Arial";
	textAnchor = "start";
	fontStyle = "normal";
	fill = Colour(0, 0, 0);
}

Text::~Text() {}


Point2D Text::getTextPos() { return textPos; }
float Text::getFontSize() { return fontSize; }
float Text::getDx() { return dx; }
float Text::getDy() { return dy; }
string Text::getContent() { return content; }
string Text::getFontFamily() { return fontFamily; }
string Text::getTextAnchor() { return textAnchor; }
string Text::getFontStyle() { return fontStyle; }
Colour Text::getFillColour() { return fill; }
float Text::getStrokeWidth() { return stroke.getWidth(); }
Colour Text::getStrokeColour() { return stroke.getColour(); }


void Text::setTextPos(float x, float y) { 
	textPos = Point2D(x, y); 
}

void Text::setFontSize(float size) { 
	fontSize = size; 
}
void Text::setDx(float val) { 
	dx = val; 
}
void Text::setDy(float val) { 
	dy = val; 
}
void Text::setContent(const string& str) { 
	content = str; 
}
void Text::setFontFamily(const string& family) { 
	fontFamily = family; 
}
void Text::setTextAnchor(const string& anchor) { 
	textAnchor = anchor; 
}
void Text::setFontStyle(const string& style) { 
	fontStyle = style;
}
void Text::setFillColour(Colour colour) { 
	fill = colour; 
}
void Text::setStrokeWidth(float width) {
	stroke.setWidth(width); 
}
void Text::setStrokeColour(Colour colour) { 
	stroke.setColour(colour); 
}


vector<Text> parseText(const string& filename) {
	vector<Text> texts;
	ifstream file(filename);
	if (!file.is_open()) return texts;

	stringstream buffer;
	buffer << file.rdbuf();
	string content = buffer.str();
	file.close();

	xml_document<> doc;
	doc.parse<0>(&content[0]);

	xml_node<>* svg = doc.first_node("svg");
	for (xml_node<>* node = svg->first_node("text"); node; node = node->next_sibling("text")) {
		Text txt;

		if (auto attr = node->first_attribute("x")) txt.setTextPos(stof(attr->value()), txt.getTextPos().getY());
		if (auto attr = node->first_attribute("y")) txt.setTextPos(txt.getTextPos().getX(), stof(attr->value()));
		if (auto attr = node->first_attribute("font-size")) txt.setFontSize(stof(attr->value()));
		if (auto attr = node->first_attribute("dx")) txt.setDx(stof(attr->value()));
		if (auto attr = node->first_attribute("dy")) txt.setDy(stof(attr->value()));
		if (auto attr = node->first_attribute("font-family")) txt.setFontFamily(attr->value());
		if (auto attr = node->first_attribute("text-anchor")) txt.setTextAnchor(attr->value());
		if (auto attr = node->first_attribute("font-style")) txt.setFontStyle(attr->value());
		if (auto attr = node->first_attribute("fill")) txt.setFillColour(Colour(attr->value()));
		if (auto attr = node->first_attribute("stroke")) txt.setStrokeColour(Colour(attr->value()));
		if (auto attr = node->first_attribute("stroke-width")) txt.setStrokeWidth(stof(attr->value()));

		if (node->value())
			txt.setContent(node->value());

		texts.push_back(txt);
	}
	return texts;
}

void drawText(Graphics* graphics, const vector<Text>& texts) {
	for (const Text& txt : texts) {
		SolidBrush brush(txt.getFillColour().toGdiplusColor());

		FontFamily fontFamily(L"Arial");
		Gdiplus::Font font(&fontFamily, txt.getFontSize(), FontStyleRegular, UnitPixel);

		PointF point(txt.getTextPos().getX() + txt.getDx(), txt.getTextPos().getY() + txt.getDy());

		wstring wstr(txt.getContent().begin(), txt.getContent().end());
		graphics->DrawString(wstr.c_str(), -1, &font, point, &brush);
	}
}

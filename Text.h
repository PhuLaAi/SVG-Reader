#ifndef TEXT_H
#define TEXT_H

#include "stdafx.h"
#include "Point2D.h"
#include "Colour.h"
#include "rapidxml.hpp"

using namespace rapidxml;
using namespace Gdiplus;

class Text {
protected:
	Point2D textPos;
	float fontSize, dx, dy;
	string content;
	string fontFamily;
	string textAnchor;
	string fontStyle;
	Colour fill;
	Stroke stroke;

public:
	Text();
	~Text();

	Point2D getTextPos();
	float getFontSize();
	float getDx();
	float getDy();
	string getContent();
	string getFontFamily();
	string getTextAnchor();
	string getFontStyle();
	Colour getFillColour();
	float getStrokeWidth();
	Colour getStrokeColour();

	void setTextPos(float x, float y);
	void setFontSize(float size);
	void setDx(float val);
	void setDy(float val);
	void setContent(const string& str);
	void setFontFamily(const string& family);
	void setTextAnchor(const string& anchor);
	void setFontStyle(const string& style);
	void setFillColour(Colour colour);
	void setStrokeWidth(float width);
	void setStrokeColour(Colour colour);
};

vector<Text> parseText(const string& filename);
void drawText(Graphics* graphics, const vector<Text>& texts);

#endif

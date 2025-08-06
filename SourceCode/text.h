#pragma once

#include "libs.h"
#include "point2D.h"
#include "Colour.h"
#include <algorithm>

class text {
protected:
	float fontSize, dx, dy;
	point2D textPos;
	string content, fontFamily, textAnchor, fontStyle;
	Colour fill;
	Stroke stroke;
public:
	text();
	~text();

	point2D getTextPos();
	float getDx();
	float getDy();
	float getFontSize();
	string getFontFamily();
	string getFontStyle();
	string getTextAnchor();
	string& getContent();
	Colour getColour();
	float getStrokeWidth();
	Colour getStrokeColour();

	// Setters
	void setTextPos(float x, float y);
	void setDx(float dx);
	void setDy(float dy);
	void setFontSize(float size);
	void setFontFamily(string family);
	void setFontStyle(string style);
	void setTextAnchor(string anchor);
	void setContent(string text);
	void setColour(Colour colour);
	void setStrokeWidth(float w);
	void setStrokeColour(Colour StkColour);

};
void drawText(Graphics* graphics, vector<text>& texts);


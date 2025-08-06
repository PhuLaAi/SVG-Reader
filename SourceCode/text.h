#pragma once

#include "libs.h"
#include "point2D.h"
#include "ColorManager.h"
#include "utils.h"
class text {
protected:
	float fontSize, dx, dy;
	point2D textPos;
	string content, fontFamily, textAnchor, fontStyle;
	Color fill;
	Color strokeColor;
	float strokeWidth;
	string fillStr;
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
	Color getColour();
	float getStrokeWidth();
	Color getStrokeColour();
	string getFillStr();

	// Setters
	void setTextPos(float x, float y);
	void setDx(float dx);
	void setDy(float dy);
	void setFontSize(float size);
	void setFontFamily(string family);
	void setFontStyle(string style);
	void setTextAnchor(string anchor);
	void setContent(string text);
	void setColour(Color colour);
	void setStrokeWidth(float w);
	void setStrokeColour(Color StkColour);
	void setFillStr(const string& str);
};

void drawText(Graphics* graphics, vector<text>& texts);

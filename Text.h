#ifndef Text_H
#define Text_H

#include "stdafx.h"
#include "Point2D.h"
#include "Colour.h"
#include "rapidxml.hpp"

using namespace rapidxml;
using namespace Gdiplus;

class Text {
protected:
	float fontSize, dx, dy;
	Point2D textPos;
	string content, fontFamily, textAnchor, fontStyle;
	Colour fill;
	Stroke stroke;
public:
	Text();
	~Text();

	Point2D getTextPos() const;
	float getDx() const;
	float getDy() const;
	float getFontSize() const;
	string getFontFamily() const;
	string getFontStyle() const;
	string getTextAnchor() const;
	string getContent() const;
	Colour getColour() const;
	float getStrokeWidth() const;
	Colour getStrokeColour() const;

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
vector<Text> parseText(const string& filename);
void drawText(Graphics* graphics,const vector<Text>& texts);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



#endif



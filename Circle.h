#ifndef Circle_H
#define Circle_H

#include "stdafx.h"
#include "Point2D.h"
#include "Colour.h"
#include "rapidxml.hpp"

using namespace rapidxml;
using namespace Gdiplus;

class Circle {
protected:
	float rx, ry;
	Point2D center;
	Colour fill;
	Stroke stroke;
public:
	Circle();
	~Circle();

	Colour getColour();
	float getRX();
	float getRY();
	Point2D getCenter();
	float getStrokeWidth();
	Colour getStrokeColour();

	void setColour(Colour colour);
	void setRX(float x);
	void setRY(float y);
	void setCenter(float x, float y);
	void setStrokeWidth(float w);
	void setStrokeColour(Colour StkColour);

};
vector<Circle> parseCircle(const string& filename);
void drawCircle(Graphics* graphics, vector<Circle>& circles);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



#endif


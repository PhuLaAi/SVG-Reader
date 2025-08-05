#ifndef Colour_H
#define Colour_H

#include <iostream>

using namespace std;

class Colour {
public:
	float r = 0, g = 0, b = 0, o = 1;
	Colour() {}
	Colour(float r, float g, float b, float o) : r(r), g(g), b(b), o(o) {}
};

class Stroke {
private:
	Colour strokeColour;
	float strokeWidth;
public:
	Stroke();
	~Stroke();

	Colour getStrokeColour();
	float getStrokeWidth();

	void setStrokeColour(Colour color);
	void setStrokeWidth(float width);

};

#endif 

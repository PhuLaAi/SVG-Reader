#ifndef Point2D_H
#define Point2D_H

#include <iostream>

using namespace std;

class Point2D {
private:
	float x, y;

public:

	Point2D();
	~Point2D();

	float getX();
	float getY();

	void setX(float a);
	void setY(float b);
};

#endif 
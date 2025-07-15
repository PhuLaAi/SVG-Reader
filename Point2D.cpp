#include "Point2D.h"

Point2D::Point2D() {
	x = y = 0;
}
Point2D::~Point2D() {}

float Point2D::getX() {
	return this->x;
}
float Point2D::getY() {
	return this->y;
}

void Point2D::setX(float a) {
	x = a;
}
void Point2D::setY(float b) {
	y = b;
}
#include "Polyline.h"

Polyline::Polyline() {
}
Polyline::~Polyline() {
}

vector<Point2D> Polyline::getPoints() {
	return points;
}
float Polyline::getStrokeWidth() {
	return stroke.getStrokeWidth();
}
Colour Polyline::getStrokeColour() {
	return stroke.getStrokeColour();
}
void Polyline::setPoints(vector<Point2D> a) {
	points = a;
}
void Polyline::setStrokeWidth(float w) {
	stroke.setStrokeWidth(w);
}
void Polyline::setStrokeColour(Colour StkColour) {
	stroke.setStrokeColour(StkColour);
}
#include "Polylines.h"

Polylines::Polylines() {
}
Polylines::~Polylines() {
}

vector<Point2D> Polylines::getPoints() {
	return points;
}
float Polylines::getStrokeWidth() {
	return stroke.getStrokeWidth();
}
Colour Polylines::getStrokeColour() {
	return stroke.getStrokeColour();
}
void Polylines::setPoints(vector<Point2D> a) {
	points = a;
}
void Polylines::setStrokeWidth(float w) {
	stroke.setStrokeWidth(w);
}
void Polylines::setStrokeColour(Colour StkColour) {
	stroke.setStrokeColour(StkColour);
}

vector<Polylines> parsePolylines(const string& filename) {

}
void drawPolylines(Graphics* graphics, vector<Polylines>& Polyliness);

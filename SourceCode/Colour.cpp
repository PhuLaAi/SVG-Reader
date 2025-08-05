#include "Colour.h"

Stroke::Stroke() {
	strokeWidth = 1;
}
Stroke::~Stroke() {
	strokeWidth = 0;
}

Colour Stroke::getStrokeColour() {
	return this->strokeColour;
}
float Stroke::getStrokeWidth() {
	return this->strokeWidth;
}

void Stroke::setStrokeColour(Colour color) {
	strokeColour = color;
}
void Stroke::setStrokeWidth(float width) {
	strokeWidth = width;
}
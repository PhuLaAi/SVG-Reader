#pragma once

#include "libs.h"
#include "point2D.h"

Color parseRGB(const char* rgbStr, int alpha = 255);
int parseInt(const char* s, int def = 0);
float parseFloat(const char* s, float def = 0.0f);
vector<point2D> parsePoints(const string& pointsStr);
vector<Point> toGdiplusPoints(const vector<point2D>& pts);
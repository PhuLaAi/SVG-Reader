#pragma once

#include "libs.h"

#include "shape.h"
#include "parser.h"
#include "rectangle.h"
#include "ellipse.h"
#include "circle.h"
#include "polygon.h"
#include "polyline.h"
#include "line.h"
#include "text.h"

void LoadShapesFromSVG(const char* filename, vector<Shape*>& shapes);

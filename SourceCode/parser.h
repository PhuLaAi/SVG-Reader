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
#include "path.h"
#include "group.h"

void LoadShapesFromSVG(const char* filename, vector<Shape*>& shapes);
vector<text> parseText(const string& filename);
const char* getAttrRecursive(xml_node<>* node, const char* name);





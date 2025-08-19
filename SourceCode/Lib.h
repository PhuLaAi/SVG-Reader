#ifndef _LIB_H_
#define _LIB_H_

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <unordered_map>
#include <stack>
#include <locale>
#include <codecvt>

#include "framework.h"
#include "rapidxml.hpp"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>

using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#include "Circle.h"
#include "Color.h"
#include "Ellipse.h"
#include "FactoryFigure.h"
#include "Figure.h"
#include "Gradient.h"
#include "Group.h"
#include "Image.h"
#include "LinearGradient.h"
#include "line.h"
#include "parser.h"
#include "path.h"
#include "point2D.h"
#include "polygon.h"
#include "polyline.h"
#include "RadialGradient.h"
#include "rectangle.h"
#include "Renderer.h"
#include "text.h"
#include "Viewbox.h"

#endif 

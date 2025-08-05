#ifndef Circle_H
#define Circle_H

#include "Ellipses.h"
#include "rapidxml.hpp"

using namespace rapidxml;
using namespace Gdiplus;

class Circle : public Ellipses {
public:
    Circle();
    ~Circle();
};

vector<Circle> parseCircle(const string& filename);
void drawCircle(Graphics* graphics, vector<Circle>& circles);

#endif



#endif


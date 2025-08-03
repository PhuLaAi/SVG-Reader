#ifndef PATH_H
#define PATH_H

#include "stdafx.h"
#include "Point2D.h"
#include "Colour.h"
#include "ColourManager.h"
#include "ColourData.h"
#include "rapidxml.hpp"


using namespace Gdiplus;
using namespace rapidxml;

class Path {
private:
    vector<Point2D> points;       
    vector<char> commands;    
    Stroke stroke;
    Colour fill;

public:
    Path();
    ~Path();

    void addCommand(char cmd, const vector<Point2D>& pts);
    vector<Point2D>& getPoints();
    vector<char>& getCommands();

    Colour getColour();
    float getStrokeWidth();
    Colour getStrokeColour();

    void setColour(Colour colour);
    void setStrokeWidth(float w);
    void setStrokeColour(Colour colour);
};

vector<Path> parsePaths(const string& filename);
void drawPaths(Graphics* graphics,vector<Path>& paths);

#endif 


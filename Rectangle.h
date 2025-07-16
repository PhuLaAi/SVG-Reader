#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

#include "Libs.h"

class Rectangle : public Figure
{
protected:
    float x, y;       
    float width, length;
    Colour fill;
    Stroke stroke;

public:
    Rectangle();
    ~Rectangle();

    float getX();
    float getY();
    float getWidth();
    float getLength();
    Colour getColour();
    float getStrokeWidth();
    Colour getStrokeColour();

    void setX(float x);
    void setY(float y);
    void setWidth(float width);
    void setLength(float length);
    void setColour(Colour colour);
    void setStrokeWidth(float w);
    void setStrokeColour(Colour StkColour);
};

#endif // _RECTANGLE_H_
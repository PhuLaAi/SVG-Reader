#pragma once

#include "libs.h"

class point2D {
public:
    int x, y;
    point2D(int x = 0, int y = 0) : x(x), y(y) {}
    int getX() { return x; };
    int getY() { return y; };

    void setX(float a) {
        x = a;
    };
    void setY(float b) {
        y = b;
    };
}; 

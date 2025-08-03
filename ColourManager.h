#ifndef COLOUR_MANAGER_H
#define COLOUR_MANAGER_H

#include <unordered_map>
#include "Colour.h"


class ColourManager {
private:
    static unordered_map<string, Colour> namedColors;

public:
    static void loadNamedColours(const string& filename); 
    static Colour getColourByName(const string& name, float opacity);
};

#endif
#ifndef COLOUR_MANAGER_H
#define COLOUR_MANAGER_H

#include <unordered_map>

class ColourManager {
private:
    static unordered_map<string, Color> namedColors;

public:
    static void loadNamedColours(const string& filename); 
    static Color getColourByName(const string& name, float opacity);
};

#endif



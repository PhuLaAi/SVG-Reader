#ifndef COLOUR_MANAGER_H
#define COLOUR_MANAGER_H

#include <unordered_map>

class ColourManager {
private:
    static unordered_map<string, ColourManager> namedColors;

public:
    static void loadNamedColours(const string& filename); 
    static ColourManager getColourByName(const string& name, float opacity);
};

#endif

#include "ColourManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>

unordered_map<string, Color> ColourManager::namedColors;

void ColourManager::loadNamedColours(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string name, hex;
        if (getline(iss, name, '\t') && getline(iss, hex)) {
        
            name.erase(remove_if(name.begin(), name.end(), ::isspace), name.end());
            transform(name.begin(), name.end(), name.begin(), ::tolower);
            if (hex[0] == '#') {
                int r = stoi(hex.substr(1, 2), nullptr, 16);
                int g = stoi(hex.substr(3, 2), nullptr, 16);
                int b = stoi(hex.substr(5, 2), nullptr, 16);
                namedColors[name] = Color(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
            }
        }
    }
}

Colour ColourManager::getColourByName(const string& name, float opacity) {
    string key = name;
    key.erase(remove_if(key.begin(), key.end(), ::isspace), key.end());
    transform(key.begin(), key.end(), key.begin(), ::tolower);

    auto it = namedColors.find(key);
    if (it != namedColors.end()) {
        Color c = it->second;
        c.o = opacity;
        return c;
    }
    return Color(0.5f, 0.5f, 0.5f, opacity); 

}

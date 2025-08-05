#include "utils.h"

Color parseRGB(const char* rgbStr, int alpha) {
    int r = 0, g = 0, b = 0;
    if (rgbStr && strncmp(rgbStr, "rgb(", 4) == 0) {
        sscanf(rgbStr, "rgb(%d,%d,%d)", &r, &g, &b);
    }
    return Color(alpha, r, g, b);
}

int parseInt(const char* s, int def) {
    return s ? atoi(s) : def;
}

float parseFloat(const char* s, float def) {
    return s ? atof(s) : def;
}

vector<point2D> parsePoints(const string& pointsStr) {
    vector<point2D> result;
    stringstream ss(pointsStr);
    string coord;
    while (getline(ss, coord, ' ')) {
        size_t comma = coord.find(',');
        if (comma != string::npos) {
            int x = stoi(coord.substr(0, comma));
            int y = stoi(coord.substr(comma + 1));
            result.emplace_back(x, y);
        }
    }
    return result;
}

vector<Point> toGdiplusPoints(const vector<point2D>& pts) {
    vector<Point> result;
    for (const auto& p : pts) {
        result.emplace_back(p.x, p.y);
    }
    return result;
}
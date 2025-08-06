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
    if (!s || strlen(s) == 0) return def;
    return atof(s);
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

bool isNumber(const string& s) {
    if (s.empty()) return false;
    char* end;
    strtod(s.c_str(), &end);
    return (*end == '\0');
}

// Tách từng token từ chuỗi "d"
vector<string> tokenizePath(const string& d) {
    vector<string> tokens;
    string token;
    for (size_t i = 0; i < d.length(); ++i) {
        char c = d[i];
        if (isalpha(c)) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.emplace_back(1, c);
        } else if (c == '-' || c == '+' || c == '.' || isdigit(c)) {
            if (c == '-' && !token.empty() && (isdigit(token.back()) || token.back() == '.')) {
                tokens.push_back(token);
                token = "-";
            } else {
                token += c;
            }
        } else if (c == ',' || isspace(c)) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        }
    }
    if (!token.empty()) tokens.push_back(token);
    return tokens;
}

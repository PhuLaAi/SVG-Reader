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

Gdiplus::Color parseColourString(const std::string& colorStr, float opacity) {
    std::string str = colorStr;
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());

    // HEX #RRGGBB
    if (str[0] == '#' && str.length() == 7) {
        int r = std::stoi(str.substr(1, 2), nullptr, 16);
        int g = std::stoi(str.substr(3, 2), nullptr, 16);
        int b = std::stoi(str.substr(5, 2), nullptr, 16);
        return Gdiplus::Color(BYTE(opacity * 255), r, g, b);
    }

    // RGB / RGBA
    if (str.find("rgb(") == 0 || str.find("rgba(") == 0) {
        std::regex rgbRegex(R"(rgba?\((\d+),(\d+),(\d+)(?:,([\d.]+))?\))");
        std::smatch match;
        if (std::regex_match(str, match, rgbRegex)) {
            int r = std::stoi(match[1]);
            int g = std::stoi(match[2]);
            int b = std::stoi(match[3]);
            float a = match[4].matched ? std::stof(match[4]) : 1.0f;
            return Gdiplus::Color(BYTE(opacity * a * 255), r, g, b);
        }
    }
    return ColorManager::getColourByName(str, opacity);
}

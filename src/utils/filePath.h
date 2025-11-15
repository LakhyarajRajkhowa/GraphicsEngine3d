#pragma once
#include <string>

static std::string ExtractNameFromPath(const std::string& path) {
    // Find last slash or backslash
    size_t slash = path.find_last_of("/\\");
    size_t start = (slash == std::string::npos) ? 0 : slash + 1;

    // Find last dot for extension
    size_t dot = path.find_last_of('.');
    if (dot == std::string::npos || dot < start)
        dot = path.length();

    return path.substr(start, dot - start);
}

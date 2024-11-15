#include "stringToWString.hpp"

std::wstring stringToWString(const std::string& str) {
    return std::wstring(str.begin(), str.end());
}
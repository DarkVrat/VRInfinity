#pragma once

#include <iomanip>
#include <sstream>
#include <iostream>
#include <vector>

enum TokenField {
    NAME = 0,
    SURNAME = 1,
    EMAIL = 2,
    ROLE = 3
};

std::string parseToken(const std::string& input, TokenField field) {
    size_t start = 0;
    size_t end = input.find(':');
    size_t currentField = 0;

    while (end != std::string::npos && currentField < field) {
        start = end + 1;
        end = input.find(':', start);
        ++currentField;
    }

    if (currentField == field) {
        if (end == std::string::npos)
            return input.substr(start);
        else 
            return input.substr(start, end - start);
    }

    return "";
}

std::string getValue(const std::string& str, const std::string& key) {
    std::string searchKey = key + "=";
    size_t startPos = str.find(searchKey);
    if (startPos == std::string::npos) 
        return "";
    
    startPos += searchKey.length();
    size_t endPos = str.find('&', startPos);
    if (endPos == std::string::npos) 
        endPos = str.length();
    
    return str.substr(startPos, endPos - startPos);
}

std::string url_decode(const std::string& value) {
    std::string result;
    result.reserve(value.size());

    for (size_t i = 0; i < value.size(); ++i) {
        if (value[i] == '%' && i + 2 < value.size()) {
            std::istringstream hex_stream(value.substr(i + 1, 2));
            int hex_value;
            if (hex_stream >> std::hex >> hex_value) {
                result += static_cast<char>(hex_value);
                i += 2;
            }
        }
        else if (value[i] == '+') {
            result += ' ';
        }
        else {
            result += value[i];
        }
    }

    return result;
}

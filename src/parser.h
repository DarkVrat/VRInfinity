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

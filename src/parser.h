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
    std::stringstream ss(input);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(ss, token, ':')) {
        tokens.push_back(token);
    }

    if (field < tokens.size()) {
        return tokens[field];
    }
    else {
        return "Invalid input";
    }
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

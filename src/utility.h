#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include "crow.h"
#include "localParser.h"

std::string g_executablePath;
void setExecutablePath(const std::string& executablePath)
{
    size_t found = executablePath.find_last_of("/\\");
    g_executablePath = executablePath.substr(0, found);
    g_executablePath += "\\";
}

std::string readFile(const std::string& filename)
{
    std::ifstream file(g_executablePath + filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::vector<char> readFileImage(const std::string& filepath) {
    std::ifstream file(g_executablePath + filepath, std::ios::binary);
    if (!file) {
        return {};
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return buffer;
}

void setAuthToken(crow::response& res, const std::string& token)
{
    res.add_header("Set-Cookie", "token=" + token + "; Path=/; Max-Age=" + std::to_string(60 * 60 * 24 * 30));
}

std::string getAuthToken(const crow::request& req)
{
    std::string token = localParser::getValue(req.get_header_value("Cookie"), "token");
    return token;
}
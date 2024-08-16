#pragma once

#include <iostream>
#include <vector>

class utility
{
public:
    // ������ ������ � �������
    static void setExecutablePath(const std::string& executablePath);
    static std::string readFile(const std::string& filename);
    static std::vector<char> readFileImage(const std::string& filepath);
private:
    static std::string g_executablePath;
};

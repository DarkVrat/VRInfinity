#include "utility.h"

#include <sstream>
#include <fstream>

std::string utility::g_executablePath;

// Установка пути к программе
void utility::setExecutablePath(const std::string& executablePath)
{
    size_t found = executablePath.find_last_of("/\\");
    g_executablePath = executablePath.substr(0, found);
    g_executablePath += "\\";
}

// Получение текстового файла
std::string utility::readFile(const std::string& filename)
{
    std::ifstream file(g_executablePath + filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Получение изображения
std::vector<char> utility::readFileImage(const std::string& filepath)
{
    std::ifstream file(g_executablePath + filepath, std::ios::binary);
    if (!file) return {};

    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return buffer;
}

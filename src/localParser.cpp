#include "localParser.h"

#include <sstream>

// Парсинг Токена аутентификации
std::string localParser::parseToken(const std::string& input, TokenField field)
{
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

// Парсинг формулы услуги
int localParser::parseFormula(const std::string& formula, FORMULA index)
{
    return std::stoi(formula.substr(index * 4, 2 - index / 2));
}

// получение данных из строки формата "name=value&name2=value2"
std::string localParser::getValue(const std::string& str, const std::string& key)
{
    std::string searchKey = key + "=";
    size_t startPos = 0;

    while ((startPos = str.find(searchKey, startPos)) != std::string::npos)
    {
        if (startPos == 0 || str[startPos - 1] == '&')
        {
            startPos += searchKey.length();
            size_t endPos = str.find('&', startPos);
            if (endPos == std::string::npos)
                endPos = str.length();
            return str.substr(startPos, endPos - startPos);
        }
        startPos += searchKey.length();
    }

    return "";
}

// Декодирование из url формата
std::string localParser::urlDecode(const std::string& value)
{
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

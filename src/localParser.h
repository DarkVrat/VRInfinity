#pragma once

#include <iostream>

class localParser
{
public:
    // enum ��� ��������� ������ �� ����� ������ �������, ����� ��� ����� �������������� � ������� �������
    enum TokenField {
        NAME = 0,
        SURNAME = 1,
        PHONE = 2,
        ROLE = 3
    };

    enum FORMULA {
        HOURS = 0,
        MINUTES = 1,
        VR = 2,
        SMALLHALL = 3
    };

    // ������� ��� �������� ������ � �������
    static std::string parseToken(const std::string& input, TokenField field);
    static int parseFormula(const std::string& formula, FORMULA index);

    // ��������� ������ �� ������ ������� "name=value&name2=value2"
    static std::string getValue(const std::string& str, const std::string& key);

    // ������� ��� ������������� ������ �� url
    static std::string urlDecode(const std::string& value);
};

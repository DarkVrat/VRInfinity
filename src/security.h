#pragma once

#include <iostream>
#include <array>
#include "crow.h"

class security
{
public:
    // ������� ����������� ������
    static std::string hashPassword(const std::string& password);

    // ������� ��������� � ����������� ������
    static std::string generateToken(const std::string& name, const std::string& surname, const std::string& phone, const std::string& role);
    static bool verifyToken(const std::string& token);
    static void setAuthToken(crow::response& res, const std::string& token);
    static std::string getAuthToken(const crow::request& req);

    //��������� ���������� ������
    static std::string generatePassword(int length);
private:
    // ���������������� ������� ��� ����������� � ��������� ������
    static std::array<unsigned int, 8> sha256(const std::string& input);
    static std::string charToHex(unsigned char c);
    static std::string createHmac(const std::string& key, const std::string& data);
};

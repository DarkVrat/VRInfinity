#pragma once

#include <iostream>
#include <array>
#include "crow.h"

class security
{
public:
    // Функция генерации и верефикации токена
    static std::string generateAdminToken();
    static bool verifyAdminToken(const std::string& token);
    static void setAdminToken(crow::response& res, const std::string& token);
    static std::string getAdminToken(const crow::request& req);
private:
    // Вспомогательныйе функции генерации токена
    static std::string charToHex(unsigned char c);
    static std::string createHmac(const std::string& key, const std::string& data);
};

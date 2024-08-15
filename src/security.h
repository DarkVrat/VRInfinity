#pragma once

#include <iostream>
#include <array>

class security
{
public:
    // Функция хеширования пароля
    static std::string hashPassword(const std::string& password);

    // Функция генерации и верефикации токена
    static std::string generateToken(const std::string& name, const std::string& surname, const std::string& phone, const std::string& role);
    static bool verifyToken(const std::string& token);

    //Генерация случайного пароля
    static std::string generatePassword(int length);
private:
    // Вспомогательныйе функции для хеширования и генерации токена
    static std::array<unsigned int, 8> sha256(const std::string& input);
    static std::string charToHex(unsigned char c);
    static std::string createHmac(const std::string& key, const std::string& data);
};

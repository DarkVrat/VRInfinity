#include "security.h"

#include <vector>
#include <iomanip>
#include <sstream>
#include "localParser.h"

// Генерация токена аутентификации
std::string security::generateAdminToken()
{
    std::string secretKey = "Q2t7HE9lfdIB";
    time_t now = time(0);
    std::string payload = std::to_string(now);

    std::string signature = createHmac(secretKey, payload);

    std::string token = payload + ":" + signature;
    return token;
}

// Проверка токена аутентификации
bool security::verifyAdminToken(const std::string& token)
{
    std::string secretKey = "Q2t7HE9lfdIB";

    size_t pos = token.find_last_of(':');
    if (pos == std::string::npos)
        return false;

    std::string payload = token.substr(0, pos);
    std::string receivedSignature = token.substr(pos + 1);

    std::string expectedSignature = createHmac(secretKey, payload);
    return (receivedSignature == expectedSignature);
}

// Установка токена
void security::setAdminToken(crow::response& res, const std::string& token)
{
    res.add_header("Set-Cookie", "token=" + token + "; Path=/; Max-Age=" + std::to_string(60 * 60 * 24 * 30));
}

// Получение токена
std::string security::getAdminToken(const crow::request& req)
{
    return localParser::getValue(req.get_header_value("Cookie"), "token");
}

// Вспомогательные функции для токена аутентификации
std::string security::charToHex(unsigned char c)
{
    std::ostringstream oss;
    oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    return oss.str();
}

std::string security::createHmac(const std::string& key, const std::string& data)
{
    std::string result;
    size_t keyLen = key.length();
    size_t dataLen = data.length();

    for (size_t i = 0; i < dataLen; ++i) {
        unsigned char xorResult = data[i] ^ key[i % keyLen];
        result += charToHex(xorResult);
    }

    return result;
}

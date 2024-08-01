#pragma once

#include <iomanip>
#include <sstream>
#include <iostream>

constexpr unsigned int ROTR (unsigned int a, unsigned int b) { return (a >> b) | (a << (32 - b));   }
constexpr unsigned int SHR  (unsigned int a, unsigned int b) { return a >> b;                       }
constexpr unsigned int Ch   (unsigned int x, unsigned int y, unsigned int z) { return (x & y) ^ (~x & z);           }
constexpr unsigned int Maj  (unsigned int x, unsigned int y, unsigned int z) { return (x & y) ^ (x & z) ^ (y & z);  }
constexpr unsigned int Sig0(unsigned int x) { return ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22);}
constexpr unsigned int Sig1(unsigned int x) { return ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25);}
constexpr unsigned int sig0(unsigned int x) { return ROTR(x, 7) ^ ROTR(x, 18) ^ SHR(x, 3);  }
constexpr unsigned int sig1(unsigned int x) { return ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10);}

const std::array<unsigned int, 64> k = { {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
} };

std::array<unsigned int, 8> sha256(const std::string& input) {
    std::array<unsigned int, 8> h = { {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    } };

    std::vector<unsigned char> bytes(input.begin(), input.end());
    bytes.push_back(0x80);
    while (bytes.size() % 64 != 56) 
        bytes.push_back(0x00);

    unsigned long long bit_length = input.size() * 8;
    for (int i = 7; i >= 0; --i) 
        bytes.push_back(bit_length >> (i * 8) & 0xff);

    for (size_t chunk = 0; chunk < bytes.size(); chunk += 64) {
        unsigned int w[64];
        for (int i = 0; i < 16; ++i) 
            w[i] = (bytes[chunk + 4 * i] << 24) | (bytes[chunk + 4 * i + 1] << 16) | (bytes[chunk + 4 * i + 2] << 8) | bytes[chunk + 4 * i + 3];
        for (int i = 16; i < 64; ++i) 
            w[i] = sig1(w[i - 2]) + w[i - 7] + sig0(w[i - 15]) + w[i - 16];

        unsigned int a = h[0], b = h[1], c = h[2], d = h[3], e = h[4], f = h[5], g = h[6], h0 = h[7];

        for (int i = 0; i < 64; ++i) {
            unsigned int t1 = h0 + Sig1(e) + Ch(e, f, g) + k[i] + w[i];
            unsigned int t2 = Sig0(a) + Maj(a, b, c);
            h0 = g;
            g = f;
            f = e;
            e = d + t1;
            d = c;
            c = b;
            b = a;
            a = t1 + t2;
        }

        h[0] += a;
        h[1] += b;
        h[2] += c;
        h[3] += d;
        h[4] += e;
        h[5] += f;
        h[6] += g;
        h[7] += h0;
    }

    return h;
}

std::string hashPassword(const std::string& password) {
    std::array<unsigned int, 8> hash = sha256(password);
    std::ostringstream result;
    for (const auto& part : hash)
        result << std::hex << std::setw(8) << std::setfill('0') << part;
    return result.str();
}

std::string charToHex(unsigned char c) {
    std::ostringstream oss;
    oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    return oss.str();
}

std::string createHmac(const std::string& key, const std::string& data) {
    std::string result;
    size_t keyLen = key.length();
    size_t dataLen = data.length();

    for (size_t i = 0; i < dataLen; ++i) {
        unsigned char xorResult = data[i] ^ key[i % keyLen];
        result += charToHex(xorResult);
    }

    return result;
}

std::string generateToken(const std::string& name, const std::string& surname, const std::string& phone, const std::string& role) {
    std::string secretKey = "Q2t7HE9lfdIB";
    time_t now = time(0);
    std::string payload = name + ":" + surname + ":" + phone + ":" + role + ":" + std::to_string(now);

    std::string signature = createHmac(secretKey, payload);

    std::string token = payload + ":" + signature;
    return token;
}

bool verifyToken(const std::string& token) {
    std::string secretKey = "Q2t7HE9lfdIB";

    size_t pos = token.find_last_of(':');
    if (pos == std::string::npos)
        return false;

    std::string payload = token.substr(0, pos);
    std::string receivedSignature = token.substr(pos + 1);

    std::string expectedSignature = createHmac(secretKey, payload);
    return (receivedSignature == expectedSignature);
}

std::string generatePassword(int length) {
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string password;
    std::srand(std::time(nullptr));

    for (int i = 0; i < length; ++i) 
        password += characters[std::rand() % characters.length()];

    return password;
}

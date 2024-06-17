#include "crow.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

// ������� ��� ������ ����������� �����
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main()
{
    crow::SimpleApp app;

    // ������� �������� � ������ � ������� �� �������� 2
    CROW_ROUTE(app, "/")([]() {
        return readFile("res/page1.html");
    });

    // �������� 2 � ������� �� ������� ��������
    CROW_ROUTE(app, "/page2")([]() {
        return readFile("res/page2.html");
    });

    // ���������� ��� �����
    CROW_ROUTE(app, "/submit").methods("POST"_method)([](const crow::request& req) {
        auto body = req.body;
        // ����� ����� ���������� ��������� �����, ���� ����������
        return "�� �����: " + body;
    });

    app.port(18080).multithreaded().run();
}
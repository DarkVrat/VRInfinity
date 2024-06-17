#include "crow.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

// Функция для чтения содержимого файла
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main()
{
    crow::SimpleApp app;

    // Главная страница с формой и ссылкой на страницу 2
    CROW_ROUTE(app, "/")([]() {
        return readFile("res/page1.html");
    });

    // Страница 2 с ссылкой на главную страницу
    CROW_ROUTE(app, "/page2")([]() {
        return readFile("res/page2.html");
    });

    // Обработчик для формы
    CROW_ROUTE(app, "/submit").methods("POST"_method)([](const crow::request& req) {
        auto body = req.body;
        // Здесь можно обработать введенный текст, если необходимо
        return "Вы ввели: " + body;
    });

    app.port(18080).multithreaded().run();
}
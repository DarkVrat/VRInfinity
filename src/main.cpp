#include "crow.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include <nanodbc/nanodbc.h>

//#include "dataBase/usersCRUD.h"
#include "dataBase/DBController.h"
using namespace DB;

// Функция для чтения содержимого файла
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main()
{
    /*crow::SimpleApp app;

    CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::POST)
        ([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "Invalid JSON");
        }

        std::string name = body["name"].s();
        std::string surname = body["surname"].s();
        std::string email = body["email"].s();
        std::string password = body["password"].s();

        DB::Users user(0, email, password, name, surname, "USER");

        DB::DBController dbController("localhost:3306", "root", "", "vrinfinity");

        if (DB::usersCRUD::createUser(&dbController, user)) {
            return crow::response(200, "User registered successfully");
        }
        else {
            return crow::response(400, "User already exists");
        }
    });*/

    /*app.port(18080).multithreaded().run();*/

    setlocale(LC_ALL, "Russian");

    try {
        // Подключение к базе данных
        nanodbc::connection conn("DRIVER={MySQL ODBC 8.4 ANSI Driver};SERVER=localhost;DATABASE=vrinfinity;USER=root;PORT=3306;");

        std::cout << "Successfully connected to the database!" << std::endl;

        // Получение surname
        nanodbc::result results = nanodbc::execute(conn, "SELECT id, name, surname FROM users");
        std::cout << "Query executed. Fetching results:" << std::endl;

        while (results.next()) {
            int id = results.get<int>(0);
            std::string name = results.get<std::string>(1);
            std::string surname = results.get<std::string>(2);

            std::cout << "ID: " << id << ", name: " << name << ", surname: " << surname << std::endl;
        }
    }
    catch (const nanodbc::database_error& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}

#include "crow.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include <nanodbc/nanodbc.h>

#include "dataBase/usersCRUD.h"
#include "dataBase/DBController.h"

using namespace DB;

// Функция для чтения содержимого файла
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


std::string g_executablePath;
void setExecutablePath(const std::string& executablePath) {
    size_t found = executablePath.find_last_of("/\\");
    g_executablePath = executablePath.substr(0, found);
    g_executablePath += "\\";
}

int main(int argc, char** argv)
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
    setExecutablePath(argv[0]);

    try {
        DBController cont(readFile(g_executablePath + "res\\DBConfig.txt"));

        
        
    }
    catch (const nanodbc::database_error& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    int i;
    std::cin >> i;

    return 0;
}

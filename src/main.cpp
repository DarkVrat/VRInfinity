#include "crow.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <nanodbc/nanodbc.h>

#include "dataBase/CRUD/userCRUD.h"
#include "dataBase/DBController.h"
#include "security.h"
#include "parser.h"

using namespace DB;

std::string g_executablePath;
void setExecutablePath(const std::string& executablePath) 
{
    size_t found = executablePath.find_last_of("/\\");
    g_executablePath = executablePath.substr(0, found);
    g_executablePath += "\\";
}

std::string readFile(const std::string& filename) 
{
    std::ifstream file(g_executablePath+filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void setAuthToken(crow::response& res, const std::string& token)
{
    res.add_header("Set-Cookie", "token=" + token + "; Path=/; Max-Age=" + std::to_string(60*60*24*30));
    res.write("Cookie set successfully");
}

std::string getAuthToken(const crow::request& req)
{
    std::string cookie_value = req.get_header_value("Cookie");
    return getValue(cookie_value, "token");
}

int main(int argc, char** argv)
{
    setExecutablePath(argv[0]);
    crow::SimpleApp app;

    CROW_ROUTE(app, "/").methods("GET"_method, "POST"_method)
        ([&](const crow::request& req, crow::response& res) {
        std::string token = getAuthToken(req);

        if (verifyToken(token)) {
            res.set_header("Content-Type", "text/html");
            res.write(readFile("res/indexLogin.html"));
        }
        else {
            res.set_header("Content-Type", "text/html");
            res.write(readFile("res/indexNONLogin.html"));
        }
        res.end();
    });

    CROW_ROUTE(app, "/login")
        .methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        res.set_header("Content-Type", "text/html");
        res.write(readFile("res/login.html"));
        res.end();
    });

    CROW_ROUTE(app, "/login")
        .methods("POST"_method)
        ([](const crow::request& req, crow::response& res) {


        std::string username = "123";
        std::string password = "qwe";

        DBController dbCon(readFile("res/DBConfig.txt"));
        user USER = usersCRUD::getUserByEmail(&dbCon, username);

        if (USER.getPasswordHash() == password) {
            std::string token = generateToken(USER.getName(), USER.getSurname(), USER.getEmail(), USER.getRole());
            setAuthToken(res, token);
            res.redirect("/");
            res.end();
        }
        else {
            res.code = 401;
            res.end("Unauthorized");
        }
    });

    CROW_ROUTE(app, "/logout")
        .methods("POST"_method)
        ([](const crow::request& req, crow::response& res) {
        setAuthToken(res, "NULL");
        res.redirect("/");
        res.end();
    });

    app.port(18080).multithreaded().run();
}

#include "crow.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <locale>
#include <codecvt>
#include <cstdint>
#include <nanodbc/nanodbc.h>

#include "dataBase/CRUD/userCRUD.h"
#include "dataBase/DBController.h"
#include "security.h"
#include "parser.h"
#include "genericHTML.h"
#include "stringConverter.h"

using namespace DB;

std::string g_executablePath;
void setExecutablePath(const std::string& executablePath) 
{
    size_t found = executablePath.find_last_of("/\\");
    g_executablePath = executablePath.substr(0, found);
    g_executablePath += "\\";
}

std::wstring readFile_W(const std::string& filename) 
{
    std::wifstream file(g_executablePath + filename);
    file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));
    std::wstringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string readFile(const std::string& filename)
{
    std::ifstream file(g_executablePath + filename);
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
    std::locale::global(std::locale("ru_RU.UTF-8"));
    setExecutablePath(argv[0]);
    crow::SimpleApp app;

    CROW_ROUTE(app, "/css/<string>")
        ([](const std::string& filename) {
        std::string content = readFile("res/css/" + filename);

        if (content.empty()) {
            return crow::response(404);
        }

        crow::response res;
        res.set_header("Content-Type", "text/css");
        res.write(content);
        return res;
    });

    CROW_ROUTE(app, "/js/<string>")
        ([](const std::string& filename) {
        std::string content = readFile("res/js/" + filename);

        if (content.empty()) {
            return crow::response(404);
        }

        crow::response res;
        res.set_header("Content-Type", "application/javascript");
        res.write(content);
        return res;
    });

    CROW_ROUTE(app, "/image/<string>")
        ([](const std::string& filename) {
        std::string content = readFile("res/image/" + filename);

        if (content.empty()) {
            return crow::response(404);
        }

        crow::response res;
        res.set_header("Content-Type", "image/jpeg");
        res.write(content);
        return res;
    });

    CROW_ROUTE(app, "/").methods("GET"_method, "POST"_method)
        ([](const crow::request& req, crow::response& res) 
    {
        std::string token = getAuthToken(req);
        std::wstring html = readFile_W("res/index.html");
        genLoginState(html, verifyToken(token));

        res.set_header("Content-Type", "text/html");
        res.write(to_string(html));
        res.end();
    });

    CROW_ROUTE(app, "/news").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        auto query = crow::query_string(req.url_params);
        std::string page_param = query.get("page") ? query.get("page") : "1"; // По умолчанию страница 1
        int page_number = std::stoi(page_param);

        std::string token = getAuthToken(req);
        std::wstring html = readFile_W("res/news.html");
        genLoginState(html, verifyToken(token));

        DBController dbController(readFile("res/DBConfig.txt"));
        genNews(html, &dbController, page_number);

        res.set_header("Content-Type", "text/html");
        res.write(to_string(html));
        res.end();
    });



    CROW_ROUTE(app, "/login")
        .methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        res.set_header("Content-Type", "text/html");
        //res.write(readFile("res/login.html"));
        res.end();
    });

    CROW_ROUTE(app, "/login")
        .methods("POST"_method)
        ([](const crow::request& req, crow::response& res) {


        std::string username = "123";
        std::string password = "qwe";

        //DBController dbCon(readFile("res/DBConfig.txt"));
        //user USER = usersCRUD::getUserByEmail(&dbCon, username);

        /*if (USER.getPasswordHash() == password) {
            std::string token = generateToken(USER.getName(), USER.getSurname(), USER.getEmail(), USER.getRole());
            setAuthToken(res, token);
            res.redirect("/");
            res.end();
        }
        else {
            res.code = 401;
            res.end("Unauthorized");
        }*/
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

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

#include "dataBase/CRUD/user_CRUD.h"
#include "dataBase/CRUD/news_CRUD.h"
#include "dataBase/CRUD/reset_password_CRUD.h"
#include "dataBase/DBController.h"
#include "security.h"
#include "parser.h"
#include "genericHTML.h"

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
    std::ifstream file(g_executablePath + filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::vector<char> readFileImage(const std::string& filepath) {
    std::ifstream file(g_executablePath + filepath, std::ios::binary);
    if (!file) {
        return {};
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return buffer;
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

std::vector<std::string> getTimes(uint32_t service, const std::string& date) {
    return { "10:00", "12:00", "14:00" };
}

int main(int argc, char** argv)
{
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
        std::vector<char> content = readFileImage("res/image/" + filename);

        if (content.empty()) {
            return crow::response(404);
        }

        crow::response res;
        res.set_header("Content-Type", "image/jpeg");
        res.body.assign(content.begin(), content.end());
        return res;
    });

    CROW_ROUTE(app, "/").methods("GET"_method, "POST"_method)
        ([](const crow::request& req, crow::response& res) 
    {
        std::string token = getAuthToken(req);
        std::string html = readFile("res/index.html");
        genLoginState(html, verifyToken(token));

        res.set_header("Content-Type", "text/html");
        res.write(html);
        res.end();
    }); 

    CROW_ROUTE(app, "/news").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        auto query = crow::query_string(req.url_params);
        std::string page_param = query.get("page") ? query.get("page") : "1";
        int page_number = std::stoi(page_param);

        std::string token = getAuthToken(req);
        std::string html = readFile("res/news.html");
        genLoginState(html, verifyToken(token));

        DBController dbController(readFile("res/DBConfig.txt"));
        genNews(html, &dbController, page_number);
         
        res.set_header("Content-Type", "text/html");
        res.write(html);
        res.end();
    });

    CROW_ROUTE(app, "/full_news").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        auto query = crow::query_string(req.url_params);
        std::string page_param = query.get("news") ? query.get("news") : "1";
        int newsId = std::stoi(page_param);

        std::string token = getAuthToken(req);
        std::string html = readFile("res/full_news.html");
        genLoginState(html, verifyToken(token));

        DBController dbController(readFile("res/DBConfig.txt"));
        genFullNews(html, &dbController, newsId);

        res.set_header("Content-Type", "text/html");
        res.write(html);
        res.end();
    });

    CROW_ROUTE(app, "/catalog").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        auto query = crow::query_string(req.url_params);
        std::string page_param = query.get("page") ? query.get("page") : "1";
        int page_number = std::stoi(page_param);

        std::string token = getAuthToken(req);
        std::string html = readFile("res/catalog_games.html");
        genLoginState(html, verifyToken(token));
         
        DBController dbController(readFile("res/DBConfig.txt"));
        genGames(html, &dbController, page_number); 

        res.set_header("Content-Type", "text/html");
        res.write(html);
        res.end();
    });
     
    CROW_ROUTE(app, "/login").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        std::string token = getAuthToken(req);
        std::string html = readFile("res/login.html");
        genLoginState(html, verifyToken(token));
        genErrorMassege(html);

        res.set_header("Content-Type", "text/html");
        res.write(html);
        res.end();
    });

    CROW_ROUTE(app, "/login").methods("POST"_method)
        ([](const crow::request& req, crow::response& res) {
        std::string body = req.body;
        std::string phone = getValue(body, "phone");
        std::string password = getValue(body, "password");
        decodePhone(phone); 

        DBController dbCon(readFile("res/DBConfig.txt"));
        user USER = user_CRUD::getUserByPhone(&dbCon, phone);

        if (USER.getPasswordHash() == hashPassword(password))
        {
            std::string token = generateToken(USER.getName(), USER.getSurname(), USER.getPhone(), USER.getRole());
            setAuthToken(res, token);
            res.redirect("/");
            res.end();
        }
        else {
            std::string token = getAuthToken(req);
            std::string html = readFile("res/login.html");
            genLoginState(html, verifyToken(token));
            genErrorMassege(html, L"Неверная почта или пароль.");

            res.set_header("Content-Type", "text/html");
            res.write(html);
            res.end();
        }
    });

    CROW_ROUTE(app, "/account").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        std::string token = getAuthToken(req);
        std::string html = readFile("res/account.html");
        genLoginState(html, verifyToken(token));

        DBController dbController(readFile("res/DBConfig.txt"));
        genAccountHTML(html, token, &dbController);

        res.set_header("Content-Type", "text/html");
        res.write(html);
        res.end();
    });

    CROW_ROUTE(app, "/settings").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        std::string token = getAuthToken(req);
        std::string html = readFile("res/settings.html");
        genLoginState(html, verifyToken(token));

        res.set_header("Content-Type", "text/html");
        res.write(html);
        res.end();
    });

    CROW_ROUTE(app, "/settings").methods("POST"_method) //Изменение записей в визитах
        ([](const crow::request& req, crow::response& res) {
        std::string body = req.body;
        std::string name = getValue(body, "name");
        std::string surname = getValue(body, "surname");
        std::string phone = getValue(body, "phone");
        std::string password = getValue(body, "password");;
        decodePhone(phone);

        std::string token = getAuthToken(req);

        DBController dbCon(readFile("res/DBConfig.txt"));
        user USER = user_CRUD::getUserByPhone(&dbCon, parseToken(token, TokenField::PHONE));

        if (name != "")     USER.setName(url_decode(name));
        if (surname != "")  USER.setSurname(url_decode(surname));
        if (phone != "")    USER.setPhone(phone);
        if (password != "") USER.setPasswordHash(hashPassword(password));
        user_CRUD::updateUser(&dbCon, USER);

        token = generateToken(USER.getName(), USER.getSurname(), USER.getPhone(), USER.getRole());
        setAuthToken(res, token);
        res.redirect("/");
        res.end();
    });

    CROW_ROUTE(app, "/logout").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        setAuthToken(res, "");
        res.redirect("/");
        res.end();
    });

    CROW_ROUTE(app, "/register").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        std::string token = getAuthToken(req);
        std::string html = readFile("res/register.html");
        genLoginState(html, verifyToken(token));
        genErrorMassege(html);

        res.set_header("Content-Type", "text/html");
        res.write(html);
        res.end();
    });

    CROW_ROUTE(app, "/register").methods("POST"_method)
        ([](const crow::request& req, crow::response& res) {
        std::string body = req.body;
        std::string name = url_decode(getValue(body, "name"));
        std::string surname = url_decode(getValue(body, "surname"));
        std::string phone = getValue(body, "phone"); 
        std::string password = getValue(body, "password");
        decodePhone(phone);

        DBController dbCon(readFile("res/DBConfig.txt"));
        user USER_check = user_CRUD::getUserByPhone(&dbCon, phone);

        if (USER_check.getId() == 0) { 
            user USER(0, phone, hashPassword(password), name, surname);
            user_CRUD::createUser(&dbCon, USER); // не создавать куки при ошибке в БД
            
            std::string token = generateToken(name, surname, phone, "USER");
            setAuthToken(res, token);
            res.redirect("/");
            res.end();
        }
        else
        {
            std::string token = getAuthToken(req);
            std::string html = readFile("res/register.html");
            genLoginState(html, verifyToken(token));
            genErrorMassege(html, L"Пользователь с такой почтой уже существует. Забыли пароль? <a href=\"/reset_password\">Сброс пароля</a>");

            res.set_header("Content-Type", "text/html");
            res.write(html);
            res.end();
        }
    });

    CROW_ROUTE(app, "/reset_password").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        std::string token = getAuthToken(req);
        std::string html = readFile("res/reset_password.html");
        genLoginState(html, verifyToken(token));
        genErrorMassege(html);

        res.set_header("Content-Type", "text/html");
        res.write(html);
        res.end();
    });

    CROW_ROUTE(app, "/reset_password").methods("POST"_method)
        ([](const crow::request& req, crow::response& res) {
        std::string body = req.body;
        std::string phone = getValue(body, "phone");
        decodePhone(phone);

        DBController dbCon(readFile("res/DBConfig.txt"));
        user USER_check = user_CRUD::getUserByPhone(&dbCon, phone);

        if (USER_check.getId() == 0) {
            std::string token = getAuthToken(req);
            std::string html = readFile("res/reset_password.html");
            genLoginState(html, verifyToken(token));
            genErrorMassege(html, L"Пользователь с такой почтой не найден.");

            res.set_header("Content-Type", "text/html");
            res.write(html);
            res.end();
        }
        else
        {
            std::string token = getAuthToken(req);
            std::string html = readFile("res/reset_password.html");
            genLoginState(html, verifyToken(token));
            genErrorMassege(html, L"Заявка для сброса пароля создна. Ожидайте сообщение от администратора.");
            std::string new_password = generatePassword(12);

            USER_check.setPasswordHash(hashPassword(new_password));
            user_CRUD::updateUser(&dbCon, USER_check);

            reset_password rp(0, phone, new_password);
            reset_password_CRUD::createResetPassword(&dbCon, rp);

            res.set_header("Content-Type", "text/html");
            res.write(html);
            res.end();
        }
    });

    CROW_ROUTE(app, "/booking").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        std::string token = getAuthToken(req);
        std::string html = readFile("res/booking.html");
        genLoginState(html, verifyToken(token));

        DBController dbCon(readFile("res/DBConfig.txt"));
        genAccountHTML(html, token, &dbCon);
        genServices(html, &dbCon); 

        res.set_header("Content-Type", "text/html"); 
        res.write(html);
        res.end();
    });

    CROW_ROUTE(app, "/booking").methods("POST"_method)
        ([](const crow::request& req, crow::response& res) {
        std::string body = req.body;
        std::string name = url_decode(getValue(body, "name"));
        std::string surname = url_decode(getValue(body, "surname"));
        std::string phone = getValue(body, "phone");
        std::string service = getValue(body, "service");
        std::string data = getValue(body, "datepicker");
        std::string time = getValue(body, "time");
        decodePhone(phone);


         

        res.redirect("/"); 
        res.end();
    });

    CROW_ROUTE(app, "/get-available-times").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto request_data = crow::json::load(req.body);
        if (!request_data) {
            return crow::response(400, "Invalid JSON");
        }

        uint32_t serviceId = request_data["serviceId"].i();
        std::string date = request_data["date"].s();

        std::vector<std::string> times = getTimes(serviceId, date);

        std::string response_data = "{\"times\":[";
        for (size_t i = 0; i < times.size(); ++i) {
            response_data += "\"" + times[i] + "\"";
            if (i < times.size() - 1) {
                response_data += ",";
            }
        }
        response_data += "]}";

        crow::response res{ response_data };
        res.add_header("Content-Type", "application/json");
        return res;

    });

    app.port(18080).multithreaded().run();
}

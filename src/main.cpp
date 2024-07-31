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
#include "dataBase/CRUD/newsCRUD.h"
#include "dataBase/CRUD/resetPasswordCRUD.h"
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
        std::string email = getValue(body, "email");
        std::string password = getValue(body, "password");

        std::string placeholder = "%40";
        size_t pos = email.find(placeholder);
        if (pos != std::string::npos)
            email.replace(pos, placeholder.length(), "@");

        DBController dbCon(readFile("res/DBConfig.txt"));
        user USER = usersCRUD::getUserByEmail(&dbCon, email);

        if (USER.getPasswordHash() == hashPassword(password))
        {
            std::string token = generateToken(USER.getName(), USER.getSurname(), USER.getEmail(), USER.getRole());
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
        std::string email = getValue(body, "email");
        std::string password = getValue(body, "password");

        std::string placeholder = "%40";
        size_t pos = email.find(placeholder);   
        if (pos != std::string::npos)
            email.replace(pos, placeholder.length(), "@");

        std::string token = getAuthToken(req);

        DBController dbCon(readFile("res/DBConfig.txt"));
        user USER = usersCRUD::getUserByEmail(&dbCon, parseToken(token, TokenField::EMAIL));

        if (name != "")     USER.setName(url_decode(name));
        if (surname != "")  USER.setSurname(url_decode(surname));
        if (email != "")    USER.setEmail(email);
        if (password != "") USER.setPasswordHash(hashPassword(password));
        usersCRUD::updateUser(&dbCon, USER);

        token = generateToken(USER.getName(), USER.getSurname(), USER.getEmail(), USER.getRole());
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
        std::string email = getValue(body, "email");
        std::string password = getValue(body, "password");

        std::string placeholder = "%40";
        size_t pos = email.find(placeholder);
        if (pos != std::string::npos)
            email.replace(pos, placeholder.length(), "@");

        DBController dbCon(readFile("res/DBConfig.txt"));
        user USER_check = usersCRUD::getUserByEmail(&dbCon, email);

        if (USER_check.getID() == 0) {
            user USER(0, email, hashPassword(password), name, surname);
            usersCRUD::createUser(&dbCon, USER);

            std::string token = generateToken(name, surname, email, "USER");
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
        std::string email = getValue(body, "email");

        std::string placeholder = "%40";
        size_t pos = email.find(placeholder);
        if (pos != std::string::npos)
            email.replace(pos, placeholder.length(), "@");

        DBController dbCon(readFile("res/DBConfig.txt"));
        user USER_check = usersCRUD::getUserByEmail(&dbCon, email);

        if (USER_check.getID() == 0) {
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
            usersCRUD::updateUser(&dbCon, USER_check);

            resetPassword rp(0, email, new_password);
            resetPasswordCRUD::createResetPassword(&dbCon, rp);

            res.set_header("Content-Type", "text/html");
            res.write(html);
            res.end();
        }
    });

    CROW_ROUTE(app, "/booking1").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        std::string token = getAuthToken(req);
        std::string html = readFile("res/booking1.html");
        genLoginState(html, verifyToken(token));

        DBController dbCon(readFile("res/DBConfig.txt"));
        genAccountHTML(html, token, &dbCon);

        res.set_header("Content-Type", "text/html"); 
        res.write(html);
        res.end();
    });

    CROW_ROUTE(app, "/booking1").methods("POST"_method)
        ([](const crow::request& req, crow::response& res) {
        std::string body = req.body;
        std::string name = url_decode(getValue(body, "name"));
        std::string surname = url_decode(getValue(body, "surname"));
        std::string email = getValue(body, "email");
        std::string service = getValue(body, "service");

        std::string placeholder = "%40";
        size_t pos = email.find(placeholder);
        if (pos != std::string::npos)
            email.replace(pos, placeholder.length(), "@");

        res.add_header("Set-Cookie", "booking=" + name + ":" + surname + ":" + email + ":" + service + "; Path=/; Max-Age=" + std::to_string(60 * 5));
        res.write("Cookie set successfully");
        res.redirect("/booking2");
        res.end();
    });

    CROW_ROUTE(app, "/booking2").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        std::string token = getAuthToken(req);
        std::string html = readFile("res/booking2.html");
        genLoginState(html, verifyToken(token));

        res.set_header("Content-Type", "text/html");
        res.write(html);
        res.end(); 
    });

    CROW_ROUTE(app, "/booking2").methods("POST"_method)
        ([](const crow::request& req, crow::response& res) {
        std::string body = req.body;
        std::string datepicker = getValue(body, "datepicker");
        std::string time = getValue(body, "time");

        // реализовать функцию генерации минимальной и максимальной даты
        // реализовать функцию просчёта свободных окон для записи
        //реализовать логику записи

        res.redirect("/");
        res.end();
    });

    app.port(18080).multithreaded().run();
}

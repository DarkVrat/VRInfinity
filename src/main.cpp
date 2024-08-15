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
#include "dataBase/CRUD/visits_vr_CRUD.h"
#include "dataBase/CRUD/visits_smallhall_CRUD.h"
#include "dataBase/DBController.h"
#include "security.h"
#include "localParser.h"
#include "genericHTML.h"
#include "bookingLogic.h"
#include "utility.h"
#include "getMethods.h"
#include "postMethods.h"

using namespace DB;

int main(int argc, char** argv)
{
    setExecutablePath(argv[0]);
    crow::SimpleApp app;

    // -=-=- Потом удалить, и настроить через nginx -=-=-
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
    // -=-=- Потом удалить, и настроить через nginx -=-=-

    // -=-=- Get Методы  -=-=- 
    CROW_ROUTE(app, "/").methods("GET"_method, "POST"_method)
        ([](const crow::request& req, crow::response& res) { 
        getMethods::getIndex(req, res); 
    });

    CROW_ROUTE(app, "/news").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        getMethods::getNews(req, res);
    });

    CROW_ROUTE(app, "/full_news").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        getMethods::getFullNews(req, res);
    });

    CROW_ROUTE(app, "/catalog").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        getMethods::getCatalog(req, res);
    });
     
    CROW_ROUTE(app, "/login").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        getMethods::getLogin(req, res);
    });

    CROW_ROUTE(app, "/account").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        getMethods::getAccount(req, res);
    });

    CROW_ROUTE(app, "/logout").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        getMethods::getLogout(req, res);
    });

    CROW_ROUTE(app, "/register").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        getMethods::getRegister(req, res);
    });

    CROW_ROUTE(app, "/settings").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        getMethods::getSettings(req, res);
    });

    CROW_ROUTE(app, "/reset_password").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        getMethods::getResetPassword(req, res);
    });

    CROW_ROUTE(app, "/booking").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        getMethods::getBooking(req, res);
    });

    CROW_ROUTE(app, "/adminPanel").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        getMethods::getAdminPanel(req, res);
    });
    // -=-=- Get Методы  -=-=- 


    CROW_ROUTE(app, "/login").methods("POST"_method)
        ([](const crow::request& req, crow::response& res) {
        std::string body = req.body;
        std::string phone = localParser::urlDecode(localParser::getValue(body, "phone"));
        std::string password = localParser::getValue(body, "password");

        DBController dbCon(readFile("res/DBConfig.txt"));
        user USER = user_CRUD::getUserByPhone(&dbCon, phone);

        if (USER.getPasswordHash() == security::hashPassword(password))
        {
            std::string token = security::generateToken(USER.getName(), USER.getSurname(), USER.getPhone(), USER.getRole());
            setAuthToken(res, token);
            res.redirect("/");
            res.end();
        }
        else {
            std::string token = getAuthToken(req);
            std::string html = readFile("res/login.html");
            genLoginState(html, security::verifyToken(token));
            genErrorMassege(html, L"Неверный номер или пароль.");

            res.set_header("Content-Type", "text/html");
            res.write(html);
            res.end();
        }
    });

    

    

    CROW_ROUTE(app, "/settings").methods("POST"_method) //Изменение записей в визитах
        ([](const crow::request& req, crow::response& res) {
        std::string body = req.body;
        std::string name = localParser::getValue(body, "name");
        std::string surname = localParser::getValue(body, "surname");
        std::string phone = localParser::urlDecode(localParser::getValue(body, "phone"));
        std::string password = localParser::getValue(body, "password");;

        std::string token = getAuthToken(req);

        DBController dbCon(readFile("res/DBConfig.txt"));
        user USER = user_CRUD::getUserByPhone(&dbCon, localParser::parseToken(token, localParser::TokenField::PHONE));

        if (name != "")     USER.setName(localParser::urlDecode(name));
        if (surname != "")  USER.setSurname(localParser::urlDecode(surname));
        if (phone != "")    USER.setPhone(phone);
        if (password != "") USER.setPasswordHash(security::hashPassword(password));
        user_CRUD::updateUser(&dbCon, USER);

        token = security::generateToken(USER.getName(), USER.getSurname(), USER.getPhone(), USER.getRole());
        setAuthToken(res, token);
        res.redirect("/");
        res.end();
    });

    

    CROW_ROUTE(app, "/register").methods("POST"_method)
        ([](const crow::request& req, crow::response& res) {
        std::string body = req.body;
        std::string name = localParser::urlDecode(localParser::getValue(body, "name"));
        std::string surname = localParser::urlDecode(localParser::getValue(body, "surname"));
        std::string phone = localParser::urlDecode(localParser::getValue(body, "phone"));
        std::string password = localParser::getValue(body, "password");

        DBController dbCon(readFile("res/DBConfig.txt"));
        user USER_check = user_CRUD::getUserByPhone(&dbCon, phone);

        if (USER_check.getId() == 0) { 
            user USER(0, phone, security::hashPassword(password), name, surname);
            uint32_t id = user_CRUD::createUser(&dbCon, USER);

            if (id == 0)
            {
                std::string html = readFile("res/register.html");
                genLoginState(html, false);
                genErrorMassege(html, L"Ошибка регистрации, попробуйте позже или обратитесь к администратору");
                res.set_header("Content-Type", "text/html");
                res.write(html);
                res.end();
                return;
            }

            std::string token = security::generateToken(name, surname, phone, "USER");
            setAuthToken(res, token);
            res.redirect("/");
            res.end();
        }
        else
        {
            std::string html = readFile("res/register.html");
            genLoginState(html, false);
            genErrorMassege(html, L"Пользователь с такой почтой уже существует. Забыли пароль? <a href=\"/reset_password\">Сброс пароля</a>");

            res.set_header("Content-Type", "text/html");
            res.write(html);
            res.end();
        }
    });

    

    CROW_ROUTE(app, "/reset_password").methods("POST"_method)
        ([](const crow::request& req, crow::response& res) {
        std::string body = req.body;
        std::string phone = localParser::urlDecode(localParser::getValue(body, "phone"));

        DBController dbCon(readFile("res/DBConfig.txt"));
        user USER_check = user_CRUD::getUserByPhone(&dbCon, phone);

        if (USER_check.getId() == 0) {
            std::string token = getAuthToken(req);
            std::string html = readFile("res/reset_password.html");
            genLoginState(html, security::verifyToken(token));
            genErrorMassege(html, L"Пользователь с такой почтой не найден.");

            res.set_header("Content-Type", "text/html");
            res.write(html);
            res.end();
        }
        else
        {
            std::string token = getAuthToken(req);
            std::string html = readFile("res/reset_password.html");
            genLoginState(html, security::verifyToken(token));
            genErrorMassege(html, L"Заявка для сброса пароля создна. Ожидайте сообщение от администратора.");
            std::string new_password = security::generatePassword(12);

            USER_check.setPasswordHash(security::hashPassword(new_password));
            user_CRUD::updateUser(&dbCon, USER_check);

            reset_password rp(0, phone, new_password);
            reset_password_CRUD::createResetPassword(&dbCon, rp);

            res.set_header("Content-Type", "text/html");
            res.write(html);
            res.end();
        }
    });

    

    CROW_ROUTE(app, "/booking").methods("POST"_method)
        ([](const crow::request& req, crow::response& res) {
        std::string body = req.body;
        std::string name = localParser::urlDecode(localParser::getValue(body, "name"));
        std::string surname = localParser::urlDecode(localParser::getValue(body, "surname"));
        std::string phone = localParser::urlDecode(localParser::getValue(body, "phone"));
        uint32_t service = std::atoi(localParser::getValue(body, "service").c_str());
        std::string data = localParser::getValue(body, "datepicker");
        std::string time = localParser::urlDecode(localParser::getValue(body, "time"));

        DBController dbCon(readFile("res/DBConfig.txt"));
        std::string formula = service_CRUD::getServiceByID(&dbCon, service).getFormula();

        int Hours = localParser::parseFormula(formula, localParser::FORMULA::HOURS);
        int Minutes = localParser::parseFormula(formula, localParser::FORMULA::MINUTES);
        int VR = localParser::parseFormula(formula, localParser::FORMULA::VR);
        int SmallHall = localParser::parseFormula(formula, localParser::FORMULA::SMALLHALL);

        std::string start = data + " " + time + ":00";
        std::string end;

        std::tm tm = {};
        std::istringstream ss(start);
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        tm.tm_hour += Hours;
        tm.tm_min += Minutes;
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        end = oss.str();

        res.redirect("/");

        dbCon.startTransaction();
        uint32_t id = visit_CRUD::createVisit(&dbCon ,visit(0, name, surname, phone, start, service));
        if (id == 0)
        {
            dbCon.rollbackTransaction();
            res.end();
            return;
        }

        for (int i = 0; i < VR; ++i)
        {
            uint32_t id_vr = visits_vr_CRUD::createVisitsVr(&dbCon, visits_vr(0, start, end, id));
            if (id_vr == 0)
            {
                dbCon.rollbackTransaction();
                res.end();
                return;
            }
        }

        if (SmallHall > 0)
        {
            uint32_t id_sh = visits_smallhall_CRUD::createVisitsSmallhall(&dbCon, visits_smallhall(0, start, end, id));
            if (id_sh == 0)
            {
                dbCon.rollbackTransaction();
                res.end();
                return;
            }
        }


        dbCon.commitTransaction();
        res.end();
    });

    CROW_ROUTE(app, "/get-available-times").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto request_data = crow::json::load(req.body);
        if (!request_data) {
            return crow::response(400, "Invalid JSON");
        }

        uint32_t serviceId = request_data["serviceId"].i();
        std::string date = request_data["date"].s();

        DBController dbCon(readFile("res/DBConfig.txt"));
        std::string TIMES = bookingLogic::getAvailableTime(date, &dbCon, serviceId);

        crow::response res{ TIMES };
        res.add_header("Content-Type", "application/json");
        return res;

    });

   

    app.port(18080).multithreaded().run();
}

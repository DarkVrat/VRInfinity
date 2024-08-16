#include "postMethods.h"

#include "localParser.h"
#include "security.h"
#include "utility.h"
#include "genericHTML.h"
#include "bookingLogic.h"
#include "dataBase/CRUD/user_CRUD.h"
#include "dataBase/CRUD/reset_password_CRUD.h"
#include "dataBase/CRUD/visits_vr_CRUD.h"
#include "dataBase/CRUD/visits_smallhall_CRUD.h"
#include "dataBase/poolControllers.h"

using namespace DB;

void postMethods::postLogin(const crow::request& req, crow::response& res)
{
    std::string body = req.body;
    std::string phone = localParser::urlDecode(localParser::getValue(body, "phone"));
    std::string password = localParser::getValue(body, "password");

    DBController* dbController = DB::poolControllers::getController();
    if (!dbController)
    {
        res.code = 500;
        res.write("Internal Server Error: Could not obtain DB controller.");
        res.end();
        return;
    }

    user USER = user_CRUD::getUserByPhone(dbController, phone);
    DB::poolControllers::releaseController(dbController);

    if (USER.getPasswordHash() == security::hashPassword(password))
    {
        std::string token = security::generateToken(USER.getName(), USER.getSurname(), USER.getPhone(), USER.getRole());
        security::setAuthToken(res, token);
        res.redirect("/");
        res.end();
    }
    else {
        std::string token = security::getAuthToken(req);
        std::string html = utility::readFile("res/login.html");
        genericHTML::genLoginState(html, security::verifyToken(token));
        genericHTML::genErrorMassege(html, L"Неверный номер или пароль.");

        res.set_header("Content-Type", "text/html");
        res.write(html);
        res.end();
    }
}

void postMethods::postSettings(const crow::request& req, crow::response& res) //Изменение записей в визитах
{
    std::string body = req.body;
    std::string name = localParser::getValue(body, "name");
    std::string surname = localParser::getValue(body, "surname");
    std::string phone = localParser::urlDecode(localParser::getValue(body, "phone"));
    std::string password = localParser::getValue(body, "password");;

    std::string token = security::getAuthToken(req);

    DBController* dbController = DB::poolControllers::getController();
    if (!dbController)
    {
        res.code = 500;
        res.write("Internal Server Error: Could not obtain DB controller.");
        res.end();
        return;
    }

    user USER = user_CRUD::getUserByPhone(dbController, localParser::parseToken(token, localParser::TokenField::PHONE));

    if (name != "")     USER.setName(localParser::urlDecode(name));
    if (surname != "")  USER.setSurname(localParser::urlDecode(surname));
    if (phone != "")    USER.setPhone(phone);
    if (password != "") USER.setPasswordHash(security::hashPassword(password));
    user_CRUD::updateUser(dbController, USER);
    DB::poolControllers::releaseController(dbController);

    token = security::generateToken(USER.getName(), USER.getSurname(), USER.getPhone(), USER.getRole());
    security::setAuthToken(res, token);
    res.redirect("/");
    res.end();
}

void postMethods::postRegister(const crow::request& req, crow::response& res)
{
    std::string body = req.body;
    std::string name = localParser::urlDecode(localParser::getValue(body, "name"));
    std::string surname = localParser::urlDecode(localParser::getValue(body, "surname"));
    std::string phone = localParser::urlDecode(localParser::getValue(body, "phone"));
    std::string password = localParser::getValue(body, "password");

    DBController* dbController = DB::poolControllers::getController();
    if (!dbController)
    {
        res.code = 500;
        res.write("Internal Server Error: Could not obtain DB controller.");
        res.end();
        return;
    }

    user USER_check = user_CRUD::getUserByPhone(dbController, phone);

    if (USER_check.getId() == 0) {
        user USER(0, phone, security::hashPassword(password), name, surname);
        uint32_t id = user_CRUD::createUser(dbController, USER);
        DB::poolControllers::releaseController(dbController);

        if (id == 0)
        {
            std::string html = utility::readFile("res/register.html");
            genericHTML::genLoginState(html, false);
            genericHTML::genErrorMassege(html, L"Ошибка регистрации, попробуйте позже или обратитесь к администратору");
            res.set_header("Content-Type", "text/html");
            res.write(html);
            res.end();
            return;
        }

        std::string token = security::generateToken(name, surname, phone, "USER");
        security::setAuthToken(res, token);
        res.redirect("/");
        res.end();
    }
    else
    {
        DB::poolControllers::releaseController(dbController);
        std::string html = utility::readFile("res/register.html");
        genericHTML::genLoginState(html, false);
        genericHTML::genErrorMassege(html, L"Пользователь с такой почтой уже существует. Забыли пароль? <a href=\"/reset_password\">Сброс пароля</a>");

        res.set_header("Content-Type", "text/html");
        res.write(html);
        res.end();
    }
}

void postMethods::postResetPassword(const crow::request& req, crow::response& res)
{
    std::string body = req.body;
    std::string phone = localParser::urlDecode(localParser::getValue(body, "phone"));

    DBController* dbController = DB::poolControllers::getController();
    if (!dbController)
    {
        res.code = 500;
        res.write("Internal Server Error: Could not obtain DB controller.");
        res.end();
        return;
    }

    user USER_check = user_CRUD::getUserByPhone(dbController, phone);

    if (USER_check.getId() == 0) {
        DB::poolControllers::releaseController(dbController);
        std::string token = security::getAuthToken(req);
        std::string html = utility::readFile("res/reset_password.html");
        genericHTML::genLoginState(html, security::verifyToken(token));
        genericHTML::genErrorMassege(html, L"Пользователь с такой почтой не найден.");

        res.set_header("Content-Type", "text/html");
        res.write(html);
        res.end();
    }
    else
    {
        std::string token = security::getAuthToken(req);
        std::string html = utility::readFile("res/reset_password.html");
        genericHTML::genLoginState(html, security::verifyToken(token));
        genericHTML::genErrorMassege(html, L"Заявка для сброса пароля создна. Ожидайте сообщение от администратора.");
        std::string new_password = security::generatePassword(12);

        USER_check.setPasswordHash(security::hashPassword(new_password));
        user_CRUD::updateUser(dbController, USER_check);

        reset_password rp(0, phone, new_password);
        reset_password_CRUD::createResetPassword(dbController, rp);
        DB::poolControllers::releaseController(dbController);

        res.set_header("Content-Type", "text/html");
        res.write(html);
        res.end();
    }
}

void postMethods::postBooking(const crow::request& req, crow::response& res)
{
    std::string body = req.body;
    std::string name = localParser::urlDecode(localParser::getValue(body, "name"));
    std::string surname = localParser::urlDecode(localParser::getValue(body, "surname"));
    std::string phone = localParser::urlDecode(localParser::getValue(body, "phone"));
    uint32_t service = std::atoi(localParser::getValue(body, "service").c_str());
    std::string data = localParser::getValue(body, "datepicker");
    std::string time = localParser::urlDecode(localParser::getValue(body, "time"));

    DBController* dbController = DB::poolControllers::getController();
    if (!dbController)
    {
        res.code = 500;
        res.write("Internal Server Error: Could not obtain DB controller.");
        res.end();
        return;
    }

    std::string formula = service_CRUD::getServiceByID(dbController, service).getFormula();

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

    dbController->startTransaction();
    uint32_t id = visit_CRUD::createVisit(dbController, visit(0, name, surname, phone, start, service));
    if (id == 0)
    {
        dbController->rollbackTransaction();
        DB::poolControllers::releaseController(dbController);
        res.end();
        return;
    }

    for (int i = 0; i < VR; ++i)
    {
        uint32_t id_vr = visits_vr_CRUD::createVisitsVr(dbController, visits_vr(0, start, end, id));
        if (id_vr == 0)
        {
            dbController->rollbackTransaction();
            DB::poolControllers::releaseController(dbController);
            res.end();
            return;
        }
    }

    if (SmallHall > 0)
    {
        uint32_t id_sh = visits_smallhall_CRUD::createVisitsSmallhall(dbController, visits_smallhall(0, start, end, id));
        if (id_sh == 0)
        {
            dbController->rollbackTransaction();
            DB::poolControllers::releaseController(dbController);
            res.end();
            return;
        }
    }


    dbController->commitTransaction();
    DB::poolControllers::releaseController(dbController);
    res.end();
}

crow::response postMethods::postGetAvailableTimes(const crow::request& req)
{
    auto request_data = crow::json::load(req.body);
    if (!request_data) {
        return crow::response(400, "Invalid JSON");
    }

    uint32_t serviceId = request_data["serviceId"].i();
    std::string date = request_data["date"].s();

    DBController* dbController = DB::poolControllers::getController();
    if (!dbController)
    {
        crow::response res;
        res.code = 500;
        res.write("Internal Server Error: Could not obtain DB controller.");
        res.end();
        return res;
    }

    std::string TIMES = bookingLogic::getAvailableTime(date, dbController, serviceId);
    DB::poolControllers::releaseController(dbController);

    crow::response res{ TIMES };
    res.add_header("Content-Type", "application/json");
    return res;
}

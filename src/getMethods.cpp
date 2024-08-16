#include "getMethods.h"

#include "localParser.h"
#include "security.h"
#include "utility.h"
#include "genericHTML.h"
#include "dataBase/poolControllers.h"

void getMethods::getIndex(const crow::request& req, crow::response& res)
{
    std::string token = security::getAuthToken(req);
    std::string html = utility::readFile("res/index.html");
    genericHTML::genLoginState(html, security::verifyToken(token));

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}

void getMethods::getNews(const crow::request& req, crow::response& res)
{
    auto query = crow::query_string(req.url_params);
    std::string page_param = query.get("page") ? query.get("page") : "1";
    int page_number = std::stoi(page_param);

    std::string token = security::getAuthToken(req);
    std::string html = utility::readFile("res/news.html");
    genericHTML::genLoginState(html, security::verifyToken(token));

    DBController* dbController = DB::poolControllers::getController();
    if (!dbController) 
    {
        res.code = 500;
        res.write("Internal Server Error: Could not obtain DB controller.");
        res.end();
        return;
    }

    genericHTML::genNews(html, dbController, page_number);
    DB::poolControllers::releaseController(dbController);

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}

void getMethods::getFullNews(const crow::request& req, crow::response& res)
{
    auto query = crow::query_string(req.url_params);
    std::string page_param = query.get("news") ? query.get("news") : "1";
    int newsId = std::stoi(page_param);

    std::string token = security::getAuthToken(req);
    std::string html = utility::readFile("res/full_news.html");
    genericHTML::genLoginState(html, security::verifyToken(token));

    DBController* dbController = DB::poolControllers::getController();
    if (!dbController)
    {
        res.code = 500;
        res.write("Internal Server Error: Could not obtain DB controller.");
        res.end();
        return;
    }

    genericHTML::genFullNews(html, dbController, newsId);
    DB::poolControllers::releaseController(dbController);

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}

void getMethods::getCatalog(const crow::request& req, crow::response& res)
{
    auto query = crow::query_string(req.url_params);
    std::string page_param = query.get("page") ? query.get("page") : "1";
    int page_number = std::stoi(page_param);

    std::string token = security::getAuthToken(req);
    std::string html = utility::readFile("res/catalog_games.html");
    genericHTML::genLoginState(html, security::verifyToken(token));

    DBController* dbController = DB::poolControllers::getController();
    if (!dbController)
    {
        res.code = 500;
        res.write("Internal Server Error: Could not obtain DB controller.");
        res.end();
        return;
    }

    genericHTML::genGames(html, dbController, page_number);
    DB::poolControllers::releaseController(dbController);

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}

void getMethods::getLogin(const crow::request& req, crow::response& res)
{
    std::string token = security::getAuthToken(req);
    std::string html = utility::readFile("res/login.html");
    genericHTML::genLoginState(html, security::verifyToken(token));
    genericHTML::genErrorMassege(html);

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}

void getMethods::getAccount(const crow::request& req, crow::response& res)
{
    std::string token = security::getAuthToken(req);
    std::string html = utility::readFile("res/account.html");
    genericHTML::genLoginState(html, security::verifyToken(token));
    genericHTML::genAdminButton(html, token);

    DBController* dbController = DB::poolControllers::getController();
    if (!dbController)
    {
        res.code = 500;
        res.write("Internal Server Error: Could not obtain DB controller.");
        res.end();
        return;
    }

    genericHTML::genAccountHTML(html, token, dbController);
    DB::poolControllers::releaseController(dbController);

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}

void getMethods::getLogout(const crow::request& req, crow::response& res)
{
    security::setAuthToken(res, "");
    res.redirect("/");
    res.end();
}

void getMethods::getResetPassword(const crow::request& req, crow::response& res)
{
    std::string token = security::getAuthToken(req);
    std::string html = utility::readFile("res/reset_password.html");
    genericHTML::genLoginState(html, security::verifyToken(token));
    genericHTML::genErrorMassege(html);

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}

void getMethods::getBooking(const crow::request& req, crow::response& res)
{
    std::string token = security::getAuthToken(req);
    std::string html = utility::readFile("res/booking.html");
    genericHTML::genLoginState(html, security::verifyToken(token));

    DBController* dbController = DB::poolControllers::getController();
    if (!dbController)
    {
        res.code = 500;
        res.write("Internal Server Error: Could not obtain DB controller.");
        res.end();
        return;
    }

    genericHTML::genAccountHTML(html, token, dbController);
    genericHTML::genServices(html, dbController);
    DB::poolControllers::releaseController(dbController);

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}

void getMethods::getAdminPanel(const crow::request& req, crow::response& res)
{
    std::string token = security::getAuthToken(req);
    if (!security::verifyToken(token) || localParser::parseToken(token, localParser::TokenField::ROLE) != "ADMIN")
    {
        security::setAuthToken(res, "");
        res.redirect("/");
        res.end();
    }

    std::string html = utility::readFile("res/adminPanel.html");

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}

void getMethods::getRegister(const crow::request& req, crow::response& res)
{
    std::string token = security::getAuthToken(req);
    std::string html = utility::readFile("res/register.html");
    genericHTML::genLoginState(html, security::verifyToken(token));
    genericHTML::genErrorMassege(html);

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}

void getMethods::getSettings(const crow::request& req, crow::response& res)
{
    std::string token = security::getAuthToken(req);
    std::string html = utility::readFile("res/settings.html");
    genericHTML::genLoginState(html, security::verifyToken(token));

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}



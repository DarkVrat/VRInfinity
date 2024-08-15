#include "getMethods.h"

#include "localParser.h"
#include "security.h"
#include "utility.h"
#include "genericHTML.h"

void getMethods::getIndex(const crow::request& req, crow::response& res)
{
    std::string token = getAuthToken(req);
    std::string html = readFile("res/index.html");
    genLoginState(html, security::verifyToken(token));

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}

void getMethods::getNews(const crow::request& req, crow::response& res)
{
    auto query = crow::query_string(req.url_params);
    std::string page_param = query.get("page") ? query.get("page") : "1";
    int page_number = std::stoi(page_param);

    std::string token = getAuthToken(req);
    std::string html = readFile("res/news.html");
    genLoginState(html, security::verifyToken(token));

    DBController dbController(readFile("res/DBConfig.txt"));
    genNews(html, &dbController, page_number);

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}

void getMethods::getFullNews(const crow::request& req, crow::response& res)
{
    auto query = crow::query_string(req.url_params);
    std::string page_param = query.get("news") ? query.get("news") : "1";
    int newsId = std::stoi(page_param);

    std::string token = getAuthToken(req);
    std::string html = readFile("res/full_news.html");
    genLoginState(html, security::verifyToken(token));

    DBController dbController(readFile("res/DBConfig.txt"));
    genFullNews(html, &dbController, newsId);

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}

void getMethods::getCatalog(const crow::request& req, crow::response& res)
{
    auto query = crow::query_string(req.url_params);
    std::string page_param = query.get("page") ? query.get("page") : "1";
    int page_number = std::stoi(page_param);

    std::string token = getAuthToken(req);
    std::string html = readFile("res/catalog_games.html");
    genLoginState(html, security::verifyToken(token));

    DBController dbController(readFile("res/DBConfig.txt"));
    genGames(html, &dbController, page_number);

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}

void getMethods::getLogin(const crow::request& req, crow::response& res)
{
    std::string token = getAuthToken(req);
    std::string html = readFile("res/login.html");
    genLoginState(html, security::verifyToken(token));
    genErrorMassege(html);

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}

void getMethods::getAccount(const crow::request& req, crow::response& res)
{
    std::string token = getAuthToken(req);
    std::string html = readFile("res/account.html");
    genLoginState(html, security::verifyToken(token));
    genAdminButton(html, token);

    DBController dbController(readFile("res/DBConfig.txt"));
    genAccountHTML(html, token, &dbController);

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}

void getMethods::getLogout(const crow::request& req, crow::response& res)
{
    setAuthToken(res, "");
    res.redirect("/");
    res.end();
}

void getMethods::getResetPassword(const crow::request& req, crow::response& res)
{
    std::string token = getAuthToken(req);
    std::string html = readFile("res/reset_password.html");
    genLoginState(html, security::verifyToken(token));
    genErrorMassege(html);

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}

void getMethods::getBooking(const crow::request& req, crow::response& res)
{
    std::string token = getAuthToken(req);
    std::string html = readFile("res/booking.html");
    genLoginState(html, security::verifyToken(token));

    DBController dbCon(readFile("res/DBConfig.txt"));
    genAccountHTML(html, token, &dbCon);
    genServices(html, &dbCon);

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}

void getMethods::getAdminPanel(const crow::request& req, crow::response& res)
{
    std::string token = getAuthToken(req);
    if (!security::verifyToken(token) || localParser::parseToken(token, localParser::TokenField::ROLE) != "ADMIN")
    {
        setAuthToken(res, "");
        res.redirect("/");
        res.end();
    }

    std::string html = readFile("res/adminPanel.html");

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}

void getMethods::getRegister(const crow::request& req, crow::response& res)
{
    std::string token = getAuthToken(req);
    std::string html = readFile("res/register.html");
    genLoginState(html, security::verifyToken(token));
    genErrorMassege(html);

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}

void getMethods::getSettings(const crow::request& req, crow::response& res)
{
    std::string token = getAuthToken(req);
    std::string html = readFile("res/settings.html");
    genLoginState(html, security::verifyToken(token));

    res.set_header("Content-Type", "text/html");
    res.write(html);
    res.end();
}



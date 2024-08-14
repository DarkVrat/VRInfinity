#pragma once

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

#include "parser.h"
#include "dataBase/CRUD/visit_CRUD.h"
#include "dataBase/CRUD/game_vr_CRUD.h"
#include "dataBase/CRUD/news_CRUD.h"
#include "dataBase/CRUD/text_news_CRUD.h"
#include "dataBase/CRUD/image_news_CRUD.h"
#include "dataBase/CRUD/service_CRUD.h"

using namespace DB;

std::string to_string(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
    return converter.to_bytes(wstr);
}

void genLoginState(std::string& html, bool flag)
{
    std::string placeholder = "PLACE_FOR_LOGIN_STATE";
    size_t pos = html.find(placeholder);
    if (pos != std::string::npos) {
        html.replace(pos, placeholder.length(), flag ? to_string(L"<li><a href=\"/account\">Аккаунт</a></li>") : to_string(L"<li><a href=\"/login\">Войти</a></li>"));
    }
}

void genAccountHTML(std::string& html, const std::string& authCookie, DBController* dbController)
{
    std::string placeholder = "PLACE_FOR_NAME";
    size_t pos = html.find(placeholder);
    if (pos != std::string::npos)
        html.replace(pos, placeholder.length(), parseToken(authCookie, NAME));

    placeholder = "PLACE_FOR_SURNAME";
    pos = html.find(placeholder);
    if (pos != std::string::npos)
        html.replace(pos, placeholder.length(), parseToken(authCookie, SURNAME));

    std::string phone = parseToken(authCookie, PHONE);
    placeholder = "PLACE_FOR_PHONE";
    pos = html.find(placeholder);
    if (pos != std::string::npos)
        html.replace(pos, placeholder.length(), phone);

    placeholder = "PLACE_FOR_TBODY";
    pos = html.find(placeholder);
    if (pos != std::string::npos) {
        std::vector<visit> visits = visit_CRUD::getAllVisitsByPhone(dbController, phone);
        std::string table = "";
        for (const auto& vis : visits) 
        {
            service Service = service_CRUD::getServiceByID(dbController, vis.getServiceId());
            table += "<tr><td><timeForJS>" + vis.getStart() + "</timeForJS></td><td>" + Service.getName() + "</td></tr>";
        }
        html.replace(pos, placeholder.length(), table);
    }
}

void genPages(std::string& html, const std::string& page, uint32_t maxPage, uint32_t countPage)
{
    std::string placeholder = "PLACE_FOR_PAGES";
    size_t pos = html.find(placeholder);
    if (pos != std::string::npos)
    {
        std::string result = "";
        if (maxPage > 1)
        {
            result += "<a href=\"/" + page + "?page=1\">&laquo;</a>";
            if (countPage > 1)
                result += "<a href=\"/" + page + "?page=" + std::to_string(countPage - 1) + "\">&lt;</a>";

            for (int i = (countPage - 2 < 1 ? 1 : countPage - 2); i <= (countPage + 2 > maxPage ? maxPage : countPage + 2); ++i)
                result += "<a href=\"/" + page + "?page=" + std::to_string(i) + "\">" + std::to_string(i) + "</a>";

            if (countPage < maxPage)
                result += "<a href=\"/" + page + "?page=" + std::to_string(countPage + 1) + "\">&gt;</a>";
            result += "<a href=\"/" + page + "?page=" + std::to_string(maxPage) + "\">&raquo;</a>";
        }
        html.replace(pos, placeholder.length(), result);
    }
}


void genGames(std::string& html, DBController* dbController, uint32_t page)
{
    auto stmt = dbController->statement("SELECT MAX(id) FROM game_vr;");
    auto result = nanodbc::execute(stmt);
    int max_id = 0;
    if (result.next()) {
        max_id = result.get<int>(0);
    }

    genPages(html, "catalog", (max_id + 9) / 10, page);

    std::string placeholder = "PLACE_FOR_GAMES";
    size_t pos = html.find(placeholder);
    if (pos != std::string::npos)
    {
        std::string result = "";

        uint32_t start_id = max_id - (page - 1) * 10;
        uint32_t end_id = start_id > 9 ? start_id - 9 : 1;

        for (uint32_t id = start_id; id >= end_id && id > 0; --id)
        {
            game_vr GAME = game_vr_CRUD::getGameByID(dbController, id);

            result += "<div class=\"game-item\">";
            result +=   "<div class=\"game-info\">";
            result +=       "<img src=\"/image/" + GAME.getImage() + ".jpeg\" alt=\"Game\">";
            result +=       "<div class=\"game-overlay\">";
            result +=           "<h2>" + GAME.getName() + "</h2>";
            result +=           "<p>" + GAME.getText() + "</p>";
            result +=       "</div>";
            result +=   "</div>";
            result += "</div>";
        }

        html.replace(pos, placeholder.length(), result);
    }
}


void genNews(std::string& html, DBController* dbController, uint32_t page)
{
    auto stmt = dbController->statement("SELECT MAX(id) FROM news;");
    auto result = nanodbc::execute(stmt);
    int max_id = 0;
    if (result.next()) {
        max_id = result.get<int>(0);
    }

    genPages(html, "news", (max_id + 9) / 10, page);

    std::string placeholder = "PLACE_FOR_NEWS";
    size_t pos = html.find(placeholder);
    if (pos != std::string::npos)
    {
        std::string result = "";

        uint32_t start_id = max_id - (page - 1) * 10;
        uint32_t end_id = start_id > 9 ? start_id - 9 : 1;

        for (uint32_t id = start_id; id >= end_id && id > 0; --id)
        {
            news NEWS = news_CRUD::getNewsByID(dbController, id);
            image_news image = image_news_CRUD::getImageNewsByID(dbController, NEWS.getImageNewsId());
            text_news text = text_news_CRUD::getTextNewsByID(dbController, NEWS.getTextNewsId());

            result += "<section class=\"news-list\">";
            result +=   "<div class=\"news-item\">";
            result +=       "<a href=\"/full_news?news=" + std::to_string(NEWS.getId()) + "\">";
            result +=           "<img src=\"/image/" + image.getImage() + ".jpeg\" alt=\"News\">";
            result +=           "<div class=\"news-content\">";
            result +=               "<h2>" + NEWS.getSubject() + "</h2>";
            result +=               "<p class=\"date\"><timeForJS>" + NEWS.getTime() + "</timeForJS></p>";
            result +=               "<p>" + text.getText() + "...</p>";
            result +=           "</div>";
            result +=       "</a>";
            result +=   "</div>";
            result += "</section>";
        }

        html.replace(pos, placeholder.length(), result);
    }
}

void genFullNews(std::string& html, DBController* dbController, uint32_t id)
{
    news NEWS = news_CRUD::getNewsByID(dbController, id);
    std::string placeholder = "PLACE_FOR_SUBJECT";
    size_t pos = html.find(placeholder);
    if (pos != std::string::npos)
        html.replace(pos, placeholder.length(), NEWS.getSubject());

    placeholder = "PLACE_FOR_TIMESTAMP";
    pos = html.find(placeholder);
    if (pos != std::string::npos)
        html.replace(pos, placeholder.length(), "<timeForJS>" + NEWS.getTime() + "</timeForJS>");
    
    placeholder = "PLACE_FOR_IMAGES";
    pos = html.find(placeholder);
    if (pos != std::string::npos)
    {
        std::string allImg = "";
        image_news img = image_news_CRUD::getImageNewsByID(dbController, NEWS.getImageNewsId());
        allImg += "<div class=\"item\"><img src=\"/image/" + img.getImage() + ".jpeg\" alt=\"image\"></div>";
        while (img.getNextId() != 0)
        {
            img = image_news_CRUD::getImageNewsByID(dbController, img.getNextId());
            allImg += "<div class=\"item\"><img src=\"/image/" + img.getImage() + ".jpeg\" alt=\"image\"></div>";
        }

        html.replace(pos, placeholder.length(), allImg);
    }

    placeholder = "PLACE_FOR_FULL_TEXT";
    pos = html.find(placeholder);
    if (pos != std::string::npos)
    {
        std::string fullText = "";
        text_news text = text_news_CRUD::getTextNewsByID(dbController, NEWS.getTextNewsId());
        fullText += text.getText();
        while (text.getNextId() != 0)
        {
            text = text_news_CRUD::getTextNewsByID(dbController, text.getNextId());
            fullText += text.getText();
        }

        html.replace(pos, placeholder.length(), fullText);
    }
}

void genErrorMassege(std::string& html, const std::wstring& massege=L"")
{
    std::string placeholder = "PLACE_FOR_ERROR_MASSEGE";
    size_t pos = html.find(placeholder);
    if (pos != std::string::npos) {
        if (massege == L"")
        {
            html.replace(pos, placeholder.length(), "");
            return;
        }
        html.replace(pos, placeholder.length(), "<div class=\"error-message\" id=\"error-message\">" + to_string(massege) + "</div>");
    }
}

void genServices(std::string& html, DBController* dbController)
{
    std::string placeholder = "PLACE_FOR_SERVICES";
    size_t pos = html.find(placeholder);
    if (pos != std::string::npos)
    {
        std::string result = "";
        std::vector<service> list = service_CRUD::getAllServices(dbController);

        for (auto& serv: list)
            result += "<option value=\"" + std::to_string(serv.getId()) + "\">" + serv.getName() + "</option>";

        html.replace(pos, placeholder.length(), result);
    }
}

void genAdminButton(std::string& html, const std::string& token)
{
    std::string placeholder = "PLACE_FOR_ADMIN";
    size_t pos = html.find(placeholder);
    if (pos != std::string::npos)
    {
        std::string result = "";
        
        if (parseToken(token, TokenField::ROLE) == "ADMIN")
        {
            result = "<a href=\"/adminPanel\" class=\"btn\">ADMIN</a>";
        }

        html.replace(pos, placeholder.length(), result);
    }
}

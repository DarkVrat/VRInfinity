#pragma once

#include <iostream>
#include "parser.h"
#include "dataBase/CRUD/visitCRUD.h"
#include "dataBase/CRUD/gameCRUD.h"
#include "dataBase/CRUD/newsCRUD.h"
#include "dataBase/CRUD/textNewsCRUD.h"
#include "dataBase/CRUD/imageNewsCRUD.h"
#include "stringConverter.h"
#include "Utf8Conv.hpp"

using namespace DB;

void genLoginState(std::wstring& html, bool flag)
{
    std::wstring placeholder = L"PLACE_FOR_LOGIN_STATE";
    size_t pos = html.find(placeholder);
    if (pos != std::wstring::npos) {
        html.replace(pos, placeholder.length(), flag ? L"<li><a href=\"/account\">Аккаунт</a></li>" : L"<li><a href=\"/login\">Войти</a></li>");
    }
}

void genAccountHTML(std::wstring& html, const std::string& authCookie, DBController* dbController)
{
    std::wstring placeholder = L"PLACE_FOR_NAME";
    size_t pos = html.find(placeholder);
    if (pos != std::wstring::npos)
        html.replace(pos, placeholder.length(), to_wstring(parseToken(authCookie, NAME)));

    placeholder = L"PLACE_FOR_SURNAME";
    pos = html.find(placeholder);
    if (pos != std::wstring::npos)
        html.replace(pos, placeholder.length(), to_wstring(parseToken(authCookie, SURNAME)));

    std::wstring email = to_wstring(parseToken(authCookie, EMAIL));
    placeholder = L"PLACE_FOR_EMAIL";
    pos = html.find(placeholder);
    if (pos != std::wstring::npos)
        html.replace(pos, placeholder.length(), email);

    placeholder = L"PLACE_FOR_TBODY";
    pos = html.find(placeholder);
    if (pos != std::wstring::npos) {
        std::vector<visit> visits = VisitsCRUD::getAllVisitsByEmail(dbController, to_string(email));
        std::wstring table = L"";
        for (auto& vis : visits)
            table += L"<tr><td>" + to_wstring(vis.getStart()) + L"</td><td>" + to_wstring(vis.getService()) + L"</td></tr>";
        html.replace(pos, placeholder.length(), table);
    }
}

void genPages(std::wstring& html, const std::wstring& page, uint32_t maxPage, uint32_t countPage)
{
    std::wstring placeholder = L"PLACE_FOR_PAGES";
    size_t pos = html.find(placeholder);
    if (pos != std::wstring::npos)
    {
        std::wstring result = L"";
        if (maxPage > 1)
        {
            result += L"<a href=\"/" + page + L"?page=1\">&laquo;</a>";
            if (countPage > 1) result += L"<a href=\"/" + page + L"?page=" + std::to_wstring(countPage - 1) + L"\">&lt;</a>";

            for (int i = (countPage - 2 < 1 ? 1 : countPage - 2); i <= (countPage + 2 > maxPage ? maxPage : countPage + 2); ++i)
                result += L"<a href=\"/" + page + L"?page=" + std::to_wstring(i) + L"\">" + std::to_wstring(i) + L"</a>";

            if (countPage < maxPage) result += L"<a href=\"/" + page + L"?page=" + std::to_wstring(countPage + 1) + L"\">&gt;</a>";
            result += L"<a href=\"/" + page + L"?page=" + std::to_wstring(maxPage) + L"\">&raquo;</a>";
        }
        html.replace(pos, placeholder.length(), result);
    }
}

void genGames(std::wstring& html, DBController* dbController, uint32_t page)
{
    auto stmt = dbController->statement("SELECT MAX(id) FROM game;");
    auto result = nanodbc::execute(stmt);
    int max_id = 0;
    if (result.next()) {
        max_id = result.get<int>(0);
    }

    genPages(html, L"catalog", (max_id + 9) / 10, page);

    std::wstring placeholder = L"PLACE_FOR_GAMES";
    size_t pos = html.find(placeholder);
    if (pos != std::wstring::npos)
    {
        std::wstring result = L"";

        uint32_t start_id = max_id - (page - 1) * 10;
        uint32_t end_id = start_id > 9 ? start_id - 9 : 1;

        for (uint32_t id = start_id; id >= end_id && id > 0; --id)
        {
            game GAME = gameCRUD::getGameByID(dbController, id);

            result += L"<div class=\"game-item\">";
            result +=   L"<div class=\"game-info\">";
            result +=       L"<img src=\"/image/" + to_wstring(GAME.getImage()) + L"\" alt=\"Game\">";
            result +=       L"<div class=\"game-overlay\">";
            result +=           L"<h2>" + Utf8Conv::Utf8ToUtf16(GAME.getName()) + L"</h2>";
            result +=           L"<p>" + to_wstring(GAME.getText()) + L"</p>";
            result +=       L"</div>";
            result +=   L"</div>";
            result += L"</div>";
        }

        html.replace(pos, placeholder.length(), result);
    }
}

void genNews(std::wstring& html, DBController* dbController, uint32_t page)
{
    auto stmt = dbController->statement("SELECT MAX(id) FROM news;");
    auto result = nanodbc::execute(stmt);
    int max_id = 0;
    if (result.next()) {
        max_id = result.get<int>(0);
    }

    genPages(html, L"news", (max_id + 9) / 10, page);

    std::wstring placeholder = L"PLACE_FOR_NEWS";
    size_t pos = html.find(placeholder);
    if (pos != std::wstring::npos)
    {
        std::wstring result = L"";

        uint32_t start_id = max_id - (page - 1) * 10;
        uint32_t end_id = start_id > 9 ? start_id - 9 : 1;

        for (uint32_t id = start_id; id >= end_id && id > 0; --id)
        {
            news NEWS = newsCRUD::getNewsByID(dbController, id);
            imageNews image = imageNewsCRUD::getImageNewsByID(dbController, NEWS.getImageNewsID());
            textNews text = textNewsCRUD::getTextNewsByID(dbController, NEWS.getTextNewsID());

            result += L"<section class=\"news-list\">";
            result +=   L"<div class=\"news-item\">";
            result +=       L"<a href=\"/full_news?news=" + std::to_wstring(NEWS.getID()) + L"\">";
            result +=           L"<img src=\"/image/" + to_wstring(image.getImage()) + L"\" alt=\"News\">";
            result +=           L"<div class=\"news-content\">";
            result +=               L"<h2>" + Utf8Conv::Utf8ToUtf16(NEWS.getSubject()) + L"</h2>";
            result +=               L"<p class=\"date\">" + to_wstring(NEWS.getTime()) + L"</p>";
            result +=               L"<p>" + to_wstring(text.getText()) + L"...</p>";
            result +=           L"</div>";
            result +=       L"</a>";
            result +=   L"</div>";
            result += L"</section>";
        }

        html.replace(pos, placeholder.length(), result);
    }
}

void genFullNews(std::wstring& html, DBController* dbController, uint32_t id)
{
    news NEWS = newsCRUD::getNewsByID(dbController, id);
    std::wstring placeholder = L"PLACE_FOR_SUBJECT";
    size_t pos = html.find(placeholder);
    if (pos != std::wstring::npos)
        html.replace(pos, placeholder.length(), to_wstring(NEWS.getSubject()));

    placeholder = L"PLACE_FOR_TIMESTAMP";
    pos = html.find(placeholder);
    if (pos != std::wstring::npos)
        html.replace(pos, placeholder.length(), to_wstring(NEWS.getTime()));

    placeholder = L"PLACE_FOR_IMAGES";
    pos = html.find(placeholder);
    if (pos != std::wstring::npos)
    {
        std::wstring allImg = L"";
        imageNews img = imageNewsCRUD::getImageNewsByID(dbController, NEWS.getImageNewsID());
        allImg += L"<div class=\"item\"><img src=\"/image/" + to_wstring(img.getImage()) + L"\" alt=\"image\"></div>";
        while (img.getNextID() != 0)
        {
            img = imageNewsCRUD::getImageNewsByID(dbController, img.getNextID());
            allImg += L"<div class=\"item\"><img src=\"/image/" + to_wstring(img.getImage()) + L"\" alt=\"image\"></div>";
        }

        html.replace(pos, placeholder.length(), allImg);
    }

    placeholder = L"PLACE_FOR_FULL_TEXT";
    pos = html.find(placeholder);
    if (pos != std::wstring::npos)
    {
        std::wstring fullText = L"";
        textNews text = textNewsCRUD::getTextNewsByID(dbController, NEWS.getTextNewsID());
        fullText += to_wstring(text.getText());
        while (text.getNextID() != 0)
        {
            text = textNewsCRUD::getTextNewsByID(dbController, text.getNextID());
            fullText += to_wstring(text.getText());
        }

        html.replace(pos, placeholder.length(), fullText);
    }
}

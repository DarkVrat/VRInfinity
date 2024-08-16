#pragma once

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

#include "localParser.h"
#include "dataBase/CRUD/visit_CRUD.h"
#include "dataBase/CRUD/game_vr_CRUD.h"
#include "dataBase/CRUD/news_CRUD.h"
#include "dataBase/CRUD/text_news_CRUD.h"
#include "dataBase/CRUD/image_news_CRUD.h"
#include "dataBase/CRUD/service_CRUD.h"

using namespace DB;

class genericHTML
{
public:
    static std::string to_string(const std::wstring& wstr);

    static void genLoginState(std::string& html, bool flag);
    static void genAccountHTML(std::string& html, const std::string& authCookie, DB::DBController* dbController);
    static void genPages(std::string& html, const std::string& page, uint32_t maxPage, uint32_t countPage);
    static void genGames(std::string& html, DB::DBController* dbController, uint32_t page);
    static void genNews(std::string& html, DB::DBController* dbController, uint32_t page);
    static void genFullNews(std::string& html, DB::DBController* dbController, uint32_t id);
    static void genErrorMassege(std::string& html, const std::wstring& massege = L"");
    static void genServices(std::string& html, DB::DBController* dbController);
    static void genAdminButton(std::string& html, const std::string& token);
private:

};

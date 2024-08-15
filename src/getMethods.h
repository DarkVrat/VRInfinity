#pragma once

#include "crow.h"

class getMethods
{
public:
	// Простые страницы
	static void getIndex(const crow::request& req, crow::response& res);
	static void getNews(const crow::request& req, crow::response& res);
	static void getFullNews(const crow::request& req, crow::response& res);
	static void getCatalog(const crow::request& req, crow::response& res);

	// Страницы работы с пользователем
	static void getLogin(const crow::request& req, crow::response& res);
	static void getAccount(const crow::request& req, crow::response& res);
	static void getLogout(const crow::request& req, crow::response& res);
	static void getRegister(const crow::request& req, crow::response& res);
	static void getSettings(const crow::request& req, crow::response& res);
	static void getResetPassword(const crow::request& req, crow::response& res);

	// Спец. страницы
	static void getBooking(const crow::request& req, crow::response& res);
	static void getAdminPanel(const crow::request& req, crow::response& res);
};
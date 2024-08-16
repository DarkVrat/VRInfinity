#pragma once

#include "crow.h"

class postMethods
{
public:
	// post методы работы с пользователем
	static void postLogin(const crow::request& req, crow::response& res);
	static void postSettings(const crow::request& req, crow::response& res);
	static void postRegister(const crow::request& req, crow::response& res);
	static void postResetPassword(const crow::request& req, crow::response& res);

	// post методы бронирования
	static void postBooking(const crow::request& req, crow::response& res);
	static crow::response postGetAvailableTimes(const crow::request& req);
};
#pragma once

#include "crow.h"

class postMethods
{
public:
	// post ������ ������ � �������������
	static void postLogin(const crow::request& req, crow::response& res);
	static void postSettings(const crow::request& req, crow::response& res);
	static void postRegister(const crow::request& req, crow::response& res);
	static void postResetPassword(const crow::request& req, crow::response& res);

	// post ������ ������������
	static void postBooking(const crow::request& req, crow::response& res);
	static crow::response postGetAvailableTimes(const crow::request& req);
};
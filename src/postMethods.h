#pragma once

#include "crow.h"

class postMethods
{
public:
	// post ������ ������������
	static void postBooking(const crow::request& req, crow::response& res);
	static crow::response postGetAvailableTimes(const crow::request& req);
};